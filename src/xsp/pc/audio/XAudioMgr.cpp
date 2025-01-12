//*****************************************************************************
//*****************************************************************************
//     _  __         _    ______ 
//    | |/ /        | |  / / __ \
//    |   / ______  | | / / /_/ /
//   /   | /_____/  | |/ / ____/ 
//  /_/|_|          |___/_/       
//
//	Extensible Visualization Platform (X-VP) (c) 
//	Copyright 2000. All Rights Reserved.
// 
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	XAudioMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

#define X_AUDIO_LUA_LIB_NAME	"Audio"

static const luaL_reg XAudioLuaLib[] = 
{
#ifdef X_CONSOLE
	{"showMethods",LAPI_audio},
#endif
	{"createSoundFromFile",LAPI_createSoundFromFile},
	{"destroySound",LAPI_destroySound},
	{"destroyAllSounds",LAPI_destroyAllSounds},
	{"playSound",LAPI_playSound},
	{"stopSound",LAPI_stopSound},
	{"stopAllSound",LAPI_stopAllSound},
	{"isSoundPlaying",LAPI_isSoundPlaying},
	{"setSoundVolume",LAPI_setSoundVolume},
	{"getSoundVolume",LAPI_getSoundVolume},
	{"setSoundPitch",LAPI_setSoundPitch},
	{"getSoundPitch",LAPI_getSoundPitch},
	{"setSoundLoopingState",LAPI_setSoundLoopingState},
	{"getSoundLoopingState",LAPI_getSoundLoopingState},
	{"setSoundRolloffFactor",LAPI_setSoundRolloffFactor},
	{"getSoundRolloffFactor",LAPI_getSoundRolloffFactor},
	{"setSoundPosition",LAPI_setSoundPosition},
	{"getSoundPosition",LAPI_getSoundPosition},
	{"setSoundVelocity",LAPI_setSoundVelocity},
	{"getSoundVelocity",LAPI_getSoundVelocity},
	{"setListenerTransform",LAPI_setListenerTransform},
	{"getListenerTransform",LAPI_getListenerTransform},
	{"setListenerPosition",LAPI_setListenerPosition},
	{"getListenerPosition",LAPI_getListenerPosition},
	{"setListenerVelocity",LAPI_setListenerVelocity},
	{"getListenerVelocity",LAPI_getListenerVelocity},
	{"setDopplerFactor",LAPI_setDopplerFactor},
	{"getDopplerFactor",LAPI_getDopplerFactor},
	{"setDopplerVelocity",LAPI_setDopplerVelocity},
	{"getDopplerVelocity",LAPI_getDopplerVelocity},
	{NULL,NULL}
};



//*****************************************************************************
/**
Constructor for the audio driver.  
*/
//*****************************************************************************
XAudioMgr::XAudioMgr()
{
	m_pOpenALDevice = NULL;
	m_pOpenALContext = NULL;
	m_openALError = 0;

	InitializeCriticalSection(&m_soundCS);
	m_threadID = 0;
	m_threadHandle = NULL;	
	m_threadOn = false;
	m_audioStreamThreshold = 0;
	m_pGenericHeap = NULL;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the audio driver.  
*/
//*****************************************************************************
XAudioMgr::~XAudioMgr()
{
}



//*****************************************************************************
/**
Initializes the audio driver.  This routine is called by X-VP during
start up.  

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAudioMgr::init(XVPDesc& desc)
{
	m_inited = true;
	
	X_DEBUG_LOG_LN("\n*** BEGIN AUDIO MANAGER INIT ***",X_DLL_INFO);

	m_pGenericHeap = XOSMgr::createHeap("XAudioMgr");

	//Make sure the streaming threshold isn't too small.
	if (desc.audioStreamThreshold < (X_AL_BUFFER_SIZE*X_AL_MAX_BUFFERS))
		m_audioStreamThreshold = (X_AL_BUFFER_SIZE*X_AL_MAX_BUFFERS);
	else
		m_audioStreamThreshold = desc.audioStreamThreshold;

	m_sounds.init("Sounds",desc.maxSounds);
	XWavSound::m_soundBuffers.init("WavSoundBuffers",desc.maxSounds);
	XOggSound::m_soundBuffers.init("OggSoundBuffers",desc.maxSounds);

	//Initialize OpenAL with the preferred device.
	m_pOpenALDevice = alcOpenDevice(NULL); 
	if (m_pOpenALDevice == NULL)
		return X_ERR_OPERATION_FAILED;

	//Create context
	m_pOpenALContext = alcCreateContext(m_pOpenALDevice,NULL);
	if (m_pOpenALContext == NULL)
		return X_ERR_OPERATION_FAILED;

	//Set active context
	//alcGetError(m_pOpenALDevice);
	alcMakeContextCurrent(m_pOpenALContext);
	if (alcGetError(m_pOpenALDevice) != ALC_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	//Generate the maximum amount of sources and push them onto the free
	//source list. 
	SourceInfo sourceInfo;
	for (XU32 i = 0;i < 32;++i)
	{
		alGenSources(1,&sourceInfo.source);
		if (alGetError() == AL_NO_ERROR)
		{	
			sourceInfo.soundHandle = X_INVALID_HANDLE;
			m_freeSourceList.insertBack(sourceInfo);
		}
		else
			break;
	}
		
	//By default, place the listener at the world identity.  Remember that OpenAL's 
	//z component is positive coming out of the screen and negative going in.  Since
	//XVP specifies z coordinates exactly the opposite, we must flip z coordinates.
	XF32 orientation[6] = {0,0,-1,0,1,0};
	XF32 position[3] = {0,0,0};
	alListenerfv(AL_ORIENTATION,orientation);
	alListenerfv(AL_POSITION,position);	

	//Create a thread to update sounds
	m_threadEvent = CreateEvent(NULL,true,false,NULL);
	m_threadOn = true;
	m_threadHandle = CreateThread(NULL,0,XAudioDriverUpdatePumper,this,0,(LPDWORD)&m_threadID);

	//Initialize lua glue
	initLuaGlue();

	X_DEBUG_LOG_LN("*** END AUDIO MANAGER INIT ***",X_DLL_INFO);

	//If we make it here then everything was OK.
	return X_OK;
}




//*****************************************************************************
/**
Shuts down the audio driver.  This routine is called by X-VP during
shutdown and gives the driver an opportunity to clean up any remaining
resources and shut down the sound device.

@return A return code of type XStatus.
*/ 
//***************************************************************************** 
XStatus XAudioMgr::destroy()
{
	X_DEBUG_LOG_LN("\n*** BEGIN AUDIO MANAGER SHUTDOWN ***",X_DLL_INFO);

	//First stop all playing sounds.
	stopAllSound();

	//Kill update thread before deallocation because we can't control the
	//order in which objects are destroyed.  If we kill the thread here in
	//shutdown() we know that all objects are still alive so we can safely
	//exit the thread.
	m_threadOn = false;
	WaitForMultipleObjectsEx(1,&m_threadEvent,true,INFINITE,false);

	//Free sources
	XMemBlockList<SourceInfo>::iterator i(m_freeSourceList.begin());
	while (i.isValid())
	{
		alDeleteSources(1,&(i.getValue().source));
		i.getNext();
	}
	i.set(m_usedSourceList.begin());
	while (i.isValid())
	{
		alDeleteSources(1,&(i.getValue().source));
		i.getNext();
	}
	m_freeSourceList.clear();
	m_usedSourceList.clear();
	
	m_sounds.destroy();
	XWavSound::m_soundBuffers.destroy();
	XOggSound::m_soundBuffers.destroy();

	//Disable context
	alcMakeContextCurrent(NULL);

	//Release context(s)
	alcDestroyContext(m_pOpenALContext);

	//Close device
	alcCloseDevice(m_pOpenALDevice);

	//Destroy the critical section.
    DeleteCriticalSection(&m_soundCS);
	CloseHandle(m_threadEvent);

	m_inited = false;

	X_DEBUG_LOG_LN("*** END AUDIO MANAGER SHUTDOWN ***",X_DLL_INFO);

	return X_OK;
}



//*****************************************************************************
/**
Updates the audio driver.  This routine is called by X-VP during
an update sequence and gives the driver an opportunity to udpate any internal,
time dependent data.

@param dt	The amount of time, in seconds, that has passed since the last
					update.
@return A return code of type XStatus.
*/  
//***************************************************************************** 
XStatus XAudioMgr::update(XF32 dt)
{
	return X_OK;
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XStatus XAudioMgr::createSoundFromFile(XS8* pFilename,XHandle& soundHandle,bool absolutePath)
{
	//Make sure we have a valid filename.
	if (!pFilename)
	{
		soundHandle = X_INVALID_HANDLE;
		return X_ERR_INVALID_PARAMS;
	}

	//Have we already loaded this sound? If so, bump the reference count
	//and return the handle.
	XResource* pResource = m_sounds.findResourceByName(pFilename,soundHandle);
	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}

	//Get the filename extension
	XS8* pExtension;
	pExtension = strrchr(pFilename,'.');
	if (!pExtension)
		return X_ERR_INVALID_PARAMS;
	
	//Create the audio path
	char* pFinalFilename = pFilename;
	char fullPath[X_MAX_PATH_SIZE];
	if (!absolutePath)
	{
		//Create the full path
		XOSMgr::buildPath(X_PATH_AUDIO,pFilename,fullPath);
		pFinalFilename = fullPath;
	}

	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (strcmp(pExtension,".wav") == 0)
	{
		//If the wav file is below the streaming threshold, attempt to load
		//the entire file into memory.
		if (XOSMgr::fileGetSize(pFinalFilename) < (m_audioStreamThreshold))
		{   
			XWavSound* pSound = NULL;
			XNew(pSound,XWavSound,m_pGenericHeap);	
			returnCode = pSound->init(pFinalFilename);
			if (returnCode == X_OK)
			{
				//Add the resource to the audio driver's stream list
				soundHandle = m_sounds.addResource(pSound,pFilename);
				if (soundHandle != X_INVALID_HANDLE)
					returnCode = X_OK;
				else
					returnCode = X_ERR_OPERATION_FAILED;
			}
			else
				XDelete(pSound);
				//XDelete(pSound);
		}
		//else it's bigger than the streaming threshold so let's
		//attempt to stream it in from disk.
		else
		{
			XWavStream* pWavStream = NULL;
			XNew(pWavStream,XWavStream,m_pGenericHeap);
			returnCode = pWavStream->init(pFinalFilename);
			if (returnCode == X_OK)
			{
				//Add the resource to the audio driver's stream list
				soundHandle = m_sounds.addResource(pWavStream,pFilename);
				if (soundHandle != X_INVALID_HANDLE)
					returnCode = X_OK;
				else
					returnCode = X_ERR_OPERATION_FAILED;
			}

			//if we ran into an error, free data
			if (returnCode != X_OK)
				XDelete(pWavStream);//X_SAFE_DELETE(pWavStream);
		}
	}
	else if (strcmp(pExtension,".ogg") == 0)
	{
		XOggSound* pSound = NULL;
		XNew(pSound,XOggSound,m_pGenericHeap);
		returnCode = pSound->initFromFile(pFinalFilename,m_audioStreamThreshold);
		if (returnCode == X_OK)
		{
			//Add the resource to the audio driver's stream list
			soundHandle = m_sounds.addResource(pSound,pFilename);
			if (soundHandle != X_INVALID_HANDLE)
				returnCode = X_OK;
			else
				returnCode = X_ERR_OPERATION_FAILED;
		}
		else
		{
			//Free data from last attempt
			XDelete(pSound);//X_SAFE_DELETE(pSound);

			XOggStream* pOggStream = NULL;
			XNew(pOggStream,XOggStream,m_pGenericHeap);

			returnCode = pOggStream->initFromFile(pFinalFilename);
			if (returnCode == X_OK)
			{
				//Add the resource to the audio driver's stream list
				soundHandle = m_sounds.addResource(pOggStream,pFilename);
				if (soundHandle != X_INVALID_HANDLE)
					returnCode = X_OK;
				else
					returnCode = X_ERR_OPERATION_FAILED;
			}
			
			//if we ran into an error, free data
			if (returnCode != X_OK)
				XDelete(pOggStream);//X_SAFE_DELETE(pOggStream);
		}
	}
	else
		returnCode = X_ERR_NOT_SUPPORTED;

	LeaveCriticalSection(&m_soundCS);

	return returnCode;
}


XStatus XAudioMgr::createSoundFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize,XHandle& soundHandle)
{
	X_ASSERT(pMemoryChunk);

	//Have we already loaded this sound? If so, bump the reference count
	//and return the handle.
	XResource* pResource = m_sounds.findResourceByName(pName,soundHandle);
	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	XOggSound* pSound = NULL;
	XNew(pSound,XOggSound,m_pGenericHeap);
	returnCode = pSound->initFromMemory(pName,pMemoryChunk,memChunkSize,m_audioStreamThreshold);
	if (returnCode == X_OK)
	{
		//Add the resource to the audio driver's stream list
		soundHandle = m_sounds.addResource(pSound,pName);
		if (soundHandle != X_INVALID_HANDLE)
			returnCode = X_OK;
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
	{
		//Free data from last attempt
		XDelete(pSound);//X_SAFE_DELETE(pSound);

		XOggStream* pOggStream = NULL;
		XNew(pOggStream,XOggStream,m_pGenericHeap);

		returnCode = pOggStream->initFromMemory(pName,pMemoryChunk,memChunkSize);
		if (returnCode == X_OK)
		{
			//Add the resource to the audio driver's stream list
			soundHandle = m_sounds.addResource(pOggStream,pName);
			if (soundHandle != X_INVALID_HANDLE)
				returnCode = X_OK;
			else
				returnCode = X_ERR_OPERATION_FAILED;
		}
		
		//if we ran into an error, free data
		if (returnCode != X_OK)
			XDelete(pOggStream);//X_SAFE_DELETE(pOggStream);
	}
	
	LeaveCriticalSection(&m_soundCS);

	return returnCode;
}

//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::destroySound(XHandle soundHandle)
{
	XStatus returnCode;

	if (soundHandle == X_INVALID_HANDLE)
		return X_ERR_INVALID_PARAMS;

	//First try to stop the sound in case it's playing.  stopSound() enters
	//the critical section so we don't need to do it before calling stopSound().
	if (stopSound(soundHandle) == X_OK)
	{
        EnterCriticalSection(&m_soundCS);
		returnCode = m_sounds.removeResource(soundHandle);
		LeaveCriticalSection(&m_soundCS);
	}
	else
		returnCode = X_ERR_OPERATION_FAILED;

	return returnCode;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XAudioMgr::addSoundRef(XHandle soundHandle)
{
	XBaseSound* pSound = (XBaseSound*)m_sounds.getResource(soundHandle);
	if (pSound)
	{
		pSound->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


//*****************************************************************************
/**
Destroys all currently allocated sounds and all associated resources in the driver.  

@return None.
*/
//***************************************************************************** 
XStatus XAudioMgr::destroyAllSounds()
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);
	returnCode = m_sounds.destroyAllResources();
	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::playSound(XHandle soundHandle,XU8 priority)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			//Let's see if there is a free source to play on.  If not, then we
			//will look through the used source list for a currently playing sound
			//whose priority is less than the priority of the sound we want to
			//play.  If we find one, stop it and free the source so it can be
			//used for the sound we want to play.
			XBOOL foundSource = false;
			SourceInfo sourceInfo;
			if (m_freeSourceList.getNumItems() != 0)
			{
				sourceInfo = m_freeSourceList.getFront();
				foundSource = true;
			}
			else
			{
				if (internalHijackSound(priority) == X_OK)
				{
					sourceInfo = m_freeSourceList.getFront();
					foundSource = true;
				}
			}

			//If we were able to find a free source to play on, continue.
			if (foundSource)
			{
				sourceInfo.soundHandle = soundHandle;
				sourceInfo.priority = priority;

				returnCode = pBaseSound->handlePlayNotification(sourceInfo.source);
				if (returnCode == X_OK)
				{
					m_freeSourceList.removeFront();
					m_usedSourceList.insertBack(sourceInfo);
				}
			}
			else
				returnCode = X_ERR_OPERATION_FAILED;
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::stopSound(XHandle soundHandle)
{
	XStatus returnCode = X_OK;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			XMemBlockList<SourceInfo>::iterator i(m_usedSourceList.begin());
			while (i.isValid())
			{
				if (i.getValue().soundHandle == soundHandle)
				{				
					pBaseSound->handleStopNotification(i.getValue().source);
					m_removeList.insertBack(i.getValue());
				}
				i.getNext();
			}

			//For each source on the remove list, remove the source from the stream
			//list and put it back onto the free list.  
			i.set(m_removeList.begin());
			while (i.isValid())
			{
				m_usedSourceList.remove(i.getValue());
				m_freeSourceList.insertBack(i.getValue());
				i.getNext();
			}
			m_removeList.clear();
		}
		else
			returnCode = X_ERR_NO_SUCH_OBJECT;
	}
	else
		returnCode = X_ERR_INVALID_PARAMS;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::stopAllSound()
{
	EnterCriticalSection(&m_soundCS);
	XMemBlockList<SourceInfo>::iterator i(m_usedSourceList.begin());
	
	while (i.isValid())
	{
		XWavStream* pBaseSound = (XWavStream*)m_sounds.getResource(i.getValue().soundHandle);
		
		pBaseSound->handleStopNotification(i.getValue().source);
		m_removeList.insertBack(i.getValue());
		i.getNext();
	}
	
	//For each source on the remove list, remove the source from the used
	//list and put it back onto the free list. 
	i.set(m_removeList.begin());
	while (i.isValid())
	{
		m_usedSourceList.remove(i.getValue());
		m_freeSourceList.insertBack(i.getValue());
		i.getNext();
	}

	m_removeList.clear();
	
	LeaveCriticalSection(&m_soundCS);
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XAudioMgr::isSoundPlaying(XHandle soundHandle)
{
	XBOOL isPlaying = false;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			XMemBlockList<SourceInfo>::iterator i(m_usedSourceList.begin());
			while (i.isValid())
			{
				if (i.getValue().soundHandle == soundHandle)
				{
					ALint state;
					alGetSourcei(i.getValue().source,AL_SOURCE_STATE,&state);
					if (state == AL_PLAYING)
						isPlaying = true;
				}
				i.getNext();
			}
		}
	}

	LeaveCriticalSection(&m_soundCS);
	return isPlaying;
}



/*
//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::muteSound(XHandle soundHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::unmuteSound(XHandle soundHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::pauseSound(XHandle soundHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::resumeSound(XHandle soundHandle)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XAudioMgr::isSoundPaused(XHandle soundHandle)
{
	//Let's take a look at the sound object in the passed in position.
	//If it is NULL, then bail; however, if there is a sound object
	//present we need to return if it is paused or not.
	if ((soundHandle > 0) && (soundHandle <= m_maxSounds-1))
		if (m_soundArray[soundHandle] != NULL)
		{
			//Run through the used source list looking for the sound.
			//If a source is found that is playing the sound and the source
			//is currently paused, return true, otherwise return false.
			//Remember that if one source is paused, all other sources playing
			//the same sound are paused as well, so we just need to find
			//once source playing the sound to determine paused status.
			ALint state;
			list<SourceInfo>::iterator i;
			for (i = m_usedSourceList.begin();i != m_usedSourceList.end();++i)
			{
				if ((*i).soundHandle == soundHandle)
				{
					alGetSourcei((*i).source,AL_SOURCE_STATE,&state);
					if (state == AL_PAUSED)
						return true;
					else
						return false;
				}
			}
		}	

	return false;
}
*/


//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setSoundVolume(XHandle soundHandle,XF32 volume)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->setVolume(volume);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XF32 XAudioMgr::getSoundVolume(XHandle soundHandle)
{

	XF32 volume = 0;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			volume = pBaseSound->getVolume();
		}
	}

	LeaveCriticalSection(&m_soundCS);
	return volume;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setSoundPitch(XHandle soundHandle,XF32 pitch)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->setPitch(pitch);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XF32 XAudioMgr::getSoundPitch(XHandle soundHandle)
{
	XF32 pitch = 0;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			pitch = pBaseSound->getPitch();
		}
	}

	LeaveCriticalSection(&m_soundCS);
	return pitch;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setSoundLoopingState(XHandle soundHandle,XBOOL loop)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->setLoopingState(loop);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}




//*****************************************************************************
//*****************************************************************************
XBOOL XAudioMgr::getSoundLoopingState(XHandle soundHandle)
{
	XBOOL looping = false;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			looping = pBaseSound->getLoopingState();
		}
	}

	LeaveCriticalSection(&m_soundCS);
	return looping;
}




//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setSoundRolloffFactor(XHandle soundHandle,XF32 rolloffFactor)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->setRolloffFactor(rolloffFactor);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XF32 XAudioMgr::getSoundRolloffFactor(XHandle soundHandle)
{
	XF32 rolloff = 0;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			rolloff = pBaseSound->getRolloffFactor();
		}
	}

	LeaveCriticalSection(&m_soundCS);
	return rolloff;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setSoundPosition(XHandle soundHandle,XVector3D& position)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->setPosition(position);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::getSoundPosition(XHandle soundHandle,XVector3D& position)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->getPosition(position);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setSoundVelocity(XHandle soundHandle,XVector3D& velocity)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->setVelocity(velocity);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::getSoundVelocity(XHandle soundHandle,XVector3D& velocity)
{
	XStatus returnCode;
	EnterCriticalSection(&m_soundCS);

	if (soundHandle != X_INVALID_HANDLE)
	{
		XBaseSound* pBaseSound = (XBaseSound*) m_sounds.getResource(soundHandle);
		if (pBaseSound)
		{
			returnCode = pBaseSound->getVelocity(velocity);
		}
		else
			returnCode = X_ERR_OPERATION_FAILED;
	}
	else
		returnCode = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_soundCS);
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setListenerTransform(XMatrix4x4& transform)
{
	//OpenAL's z coordinate is inverted from XVP's.
	XF32 orientation[6] = {transform._31,transform._32,-transform._33,transform._21,transform._22,-transform._23};
	alListenerfv(AL_ORIENTATION,orientation);
	if ((m_openALError = alGetError()) != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setListenerTransform(XVector3D& lookVector,XVector3D& upVector)
{
	XF32 orientation[6] = {lookVector.x,lookVector.y,-lookVector.z,upVector.x,upVector.y,-upVector.z};
	alListenerfv(AL_ORIENTATION,orientation);
	
	//OpenAL's z coordinate is inverted from XVP's.
	lookVector.z = -lookVector.z;

	if ((m_openALError = alGetError()) != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::getListenerTransform(XVector3D& lookVector,XVector3D& upVector)
{
	XF32 orientation[6];
	alGetListenerfv(AL_ORIENTATION,orientation);
	lookVector.x = orientation[0];
	lookVector.y = orientation[1];
	lookVector.z = -orientation[2];
	upVector.x = orientation[3];
	upVector.y = orientation[4];
	upVector.z = -orientation[5];

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setListenerPosition(XVector3D& position)
{	
	//OpenAL's z coordinate is inverted from XVP's.
	XVector3D newPosition = position;
	newPosition.z = -newPosition.z;
	alListenerfv(AL_POSITION,(XF32*)&newPosition);
	if ((m_openALError = alGetError()) != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::getListenerPosition(XVector3D& position)
{
	alGetListenerfv(AL_POSITION,(XF32*)&position);
	
	//OpenAL's z coordinate is inverted from XVP's.
	position.z = -position.z;

	if ((m_openALError = alGetError()) != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setListenerVelocity(XVector3D& velocity)
{
	//OpenAL's z coordinate is inverted from XVP's.
	XVector3D newVelocity = velocity;
	newVelocity.z = -newVelocity.z;

	alListenerfv(AL_VELOCITY,(XF32*)&newVelocity);
	if ((m_openALError = alGetError()) != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::getListenerVelocity(XVector3D& velocity)
{
	alGetListenerfv(AL_VELOCITY,(XF32*)&velocity);

	//OpenAL's z coordinate is inverted from XVP's.
	velocity.z = -velocity.z;

	if ((m_openALError = alGetError()) != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setDopplerFactor(XF32 dopplerFactor)
{
	alDopplerFactor(dopplerFactor);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XAudioMgr::getDopplerFactor()
{
	return alGetFloat(AL_DOPPLER_FACTOR);
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::setDopplerVelocity(XF32 dopplerVelocity)
{
	alDopplerVelocity(dopplerVelocity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XAudioMgr::getDopplerVelocity()
{
	return alGetFloat(AL_DOPPLER_VELOCITY);
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::streamPump()
{
	//Check to see if we are supposed to kill this thread.
	if (!m_threadOn)
	{
		SetEvent(m_threadEvent);
		ExitThread(0);
	}

	EnterCriticalSection(&m_soundCS);
	XMemBlockList<SourceInfo>::iterator i(m_usedSourceList.begin());
	while (i.isValid())
	{
		XBaseSound* pBaseSound = (XBaseSound*)m_sounds.getResource(i.getValue().soundHandle);
		
		//if pBaseSound is NULL, then it was destroyed outside this thread
		//so let's remove it from the used source list.
		if (!pBaseSound)
			m_removeList.insertBack(i.getValue());
		else
		{
			if (pBaseSound->handleUpdateNotification(i.getValue().source) != X_OK)
				m_removeList.insertBack(i.getValue());
		}
		i.getNext();
	}
	
	//For each source on the remove list, remove the source from the used
	//list and put it back onto the free list.  
	i.set(m_removeList.begin());
	while (i.isValid())
	{
		m_usedSourceList.remove(i.getValue());
		m_freeSourceList.insertBack(i.getValue());
		i.getNext();
	}

	m_removeList.clear();
	
	LeaveCriticalSection(&m_soundCS);

	return X_OK;
}





//*****************************************************************************
//*****************************************************************************
DWORD WINAPI XAudioDriverUpdatePumper(LPVOID pParam)
{

	static XAudioMgr* pSoundDriver = (XAudioMgr*)pParam;
	while(true)
	{	
		Sleep(100);
		pSoundDriver->streamPump();
	}

	return 0;
}


//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::internalHijackSound(XU8 priority)
{
	XStatus returnCode = X_ERR_OPERATION_FAILED;

	XMemBlockList<SourceInfo>::iterator i(m_usedSourceList.begin());
	while (i.isValid())
	{
		if (i.getValue().priority > priority)
		{		
			XBaseSound* pBaseSound = (XBaseSound*)m_sounds.getResource(i.getValue().soundHandle);
			pBaseSound->handleStopNotification(i.getValue().source);
			m_removeList.insertBack(i.getValue());
			break;
		}
		i.getNext();
	}

	//For each source on the remove list, remove the source from the stream
	//list and put it back onto the free list.
	i.set(m_removeList.begin());
	while (i.isValid())
	{
		m_usedSourceList.remove(i.getValue());
		m_freeSourceList.insertBack(i.getValue());
		i.getNext();
	}

	//Return X_OK if we were able to hijack (free) a source.
	if (m_removeList.getNumItems() != 0)
		returnCode = X_OK;
	
	m_removeList.clear();
				
	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAudioMgr::initLuaGlue()
{
	return XLua::registerLibrary(X_AUDIO_LUA_LIB_NAME,XAudioLuaLib);
}


