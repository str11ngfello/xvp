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
//	Module Name:	XWavSound.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XResourceList XWavSound::m_soundBuffers;


//*****************************************************************************
//*****************************************************************************
XWavSound::XWavSound()
{
	m_soundBufferHandle = X_INVALID_HANDLE;
	
}



//*****************************************************************************
//*****************************************************************************
XWavSound::~XWavSound()
{
}

//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::init(XS8* pFilename)
{
#pragma message ("XWavSound::init disabled due to new OpenAL 1.1 changes...FIX ME!! alutLoadWAVFile/unload deprecated")
	return X_ERR_OPERATION_FAILED;

	//Make sure we have a valid path
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	XStatus returnCode = X_OK;
	XBOOL createdOpenALBuffer = false;
	SoundBuffer* pSoundBuffer = NULL;

	XResource* pResource = (SoundBuffer*)m_soundBuffers.findResourceByName(pFilename,m_soundBufferHandle);

	if (pResource)
	{
		pResource->incRefCount();
	}
	else
	{
		XNew(pSoundBuffer,SoundBuffer,XOSMgr::getDefaultHeap());
		
		//Generate an openAL buffer for the new sound
		alGenBuffers(1,&(pSoundBuffer->alBuffer));
		if (alGetError() != AL_NO_ERROR)
			returnCode = X_ERR_OPERATION_FAILED;
		else
		{
			createdOpenALBuffer = true;

			//Load the sound file data
			ALsizei size,freq;
			ALenum format;
			ALvoid* pSoundData;
			//ALboolean loop;

			//alutLoadWAVFile(pFilename,&format,&pSoundData,&size,&freq,&loop);
			if (alGetError() != AL_NO_ERROR)
				returnCode = X_ERR_OPERATION_FAILED;
			else
			{
				//Mark sound as mono or stereo.  Mono sounds are required
				//to set position, velocity and other 3D properties.
				m_isMono = ((freq == AL_FORMAT_MONO8) || (freq == AL_FORMAT_MONO16));
						
				//Copy sound data into the openAL buffer. 
				alBufferData(pSoundBuffer->alBuffer,format,pSoundData,size,freq);
				if (alGetError() != AL_NO_ERROR)
					returnCode = X_ERR_OPERATION_FAILED;
				else
				{
					//Unload the sound data.
					//alutUnloadWAV(format,pSoundData,size,freq);
					if (alGetError() != AL_NO_ERROR)
						returnCode = X_ERR_OPERATION_FAILED;
					else
					{
						m_soundBufferHandle = m_soundBuffers.addResource(pSoundBuffer,pFilename);

						if (m_soundBufferHandle == X_INVALID_HANDLE)
							returnCode = X_ERR_OPERATION_FAILED;
					}
				}
			}
		}
	}

	//If we ran into an error, free data that won't be used.
	if (returnCode != X_OK)
	{
		if (createdOpenALBuffer)
			alDeleteBuffers(1,&(pSoundBuffer->alBuffer));
		
		XDelete(pSoundBuffer);
	}

	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::destroy()
{
	//First stop the stream in case it's playing.  The source contained in this
	//object is only valid if we are not finished playing, otherwise, the source
	//data member is invalid.
	if (m_playing)
		handleStopNotification(m_ALSource);

	m_soundBuffers.removeResource(m_soundBufferHandle);

	XResource::destroy();

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::handlePlayNotification(ALuint source)
{
	//Bind the sound's buffer to the source.
	SoundBuffer* pBuffer = (SoundBuffer*)m_soundBuffers.getResource(m_soundBufferHandle);
	alSourcei(source,AL_BUFFER,pBuffer->alBuffer);
	if (alGetError() != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;

	//Set all properties of the sound to the source
	alSourcef(source,AL_GAIN,m_volume);
	alSourcef(source,AL_PITCH,m_pitch);
	alSourcei(source,AL_LOOPING,m_loop);
				
	if (m_isMono)
	{
		alSourcefv(source,AL_POSITION,m_position);
		alSourcefv(source,AL_VELOCITY,m_velocity);
		alSourcef(source,AL_ROLLOFF_FACTOR,m_rolloffFactor);
	}

	//Play the sound on the source.
	alSourcePlay(source);
	if (alGetError() != AL_NO_ERROR)
		return X_ERR_OPERATION_FAILED;
	
	m_ALSource = source;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::handleStopNotification(ALuint source)
{
	//Stop the source and remove the buffer by using 0.
	alSourceStop(source);
	alSourcei(source,AL_BUFFER,0);
	
	//Mark the stream as not playing.
	m_playing = false;
	m_ALSource = 0;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::handleUpdateNotification(ALuint source)
{
	ALint state;
	alGetSourcei(source,AL_SOURCE_STATE,&state);
	if (state == AL_STOPPED) 
	{
		//Remove the buffer from the source by using 0.
		alSourcei(source,AL_BUFFER,0);
		return X_ERR_NOT_RUNNING;
	}
	else
		return X_OK;
}




//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::setVolume(XF32 volume)
{
	if (m_playing)
	{
		alSourcef(m_ALSource,AL_GAIN,volume);
		if (alGetError() != AL_NO_ERROR)
			return X_ERR_OPERATION_FAILED;
	}

	m_volume = volume;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XWavSound::getVolume()
{
	return m_volume;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::setPitch(XF32 pitch)
{
	if (m_playing)
	{
		alSourcef(m_ALSource,AL_PITCH,pitch);
		if (alGetError() != AL_NO_ERROR)
			return X_ERR_OPERATION_FAILED;
	}

	m_pitch = pitch;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XWavSound::getPitch()
{
	return m_pitch;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::setLoopingState(XBOOL enableLooping)
{
	m_loop = enableLooping;
	
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XWavSound::getLoopingState()
{
	return m_loop;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::setRolloffFactor(XF32 rolloffFactor)
{
	if (m_isMono)
	{
		if (m_playing)
		{
			alSourcef(m_ALSource,AL_ROLLOFF_FACTOR,rolloffFactor);
			if (alGetError() != AL_NO_ERROR)
				return X_ERR_OPERATION_FAILED;
		}

		m_rolloffFactor = rolloffFactor;
		
		return X_OK;
	}
	else
		return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XF32 XWavSound::getRolloffFactor()
{
	return m_rolloffFactor;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::setPosition(XVector3D& position)
{
	if (m_isMono)
	{
		m_position[0] = position.x;
		m_position[1] = position.y;
		m_position[2] = -position.z;

		if (m_playing)
		{
			alSourcefv(m_ALSource,AL_POSITION,m_position);
			if (alGetError() != AL_NO_ERROR)
				return X_ERR_OPERATION_FAILED;
		}
		
		return X_OK;
	}
	else
		return X_ERR_NOT_SUPPORTED;
	
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::getPosition(XVector3D& position)
{
	if (m_isMono)
	{
		position.x = m_position[0];
		position.y = m_position[1];
		position.z = -m_position[2];

		return X_OK;
	}
	else
		return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::setVelocity(XVector3D& velocity)
{
	if (m_isMono)
	{
		m_velocity[0] = velocity.x;
		m_velocity[1] = velocity.y;
		m_velocity[2] = -velocity.z;

		if (m_playing)
		{
			alSourcefv(m_ALSource,AL_VELOCITY,m_velocity);
			if (alGetError() != AL_NO_ERROR)
				return X_ERR_OPERATION_FAILED;
		}

		return X_OK;
	}
	else
		return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavSound::getVelocity(XVector3D& velocity)
{
	if (m_isMono)
	{
		velocity.x = m_velocity[0];
		velocity.y = m_velocity[1];
		velocity.z = -m_velocity[2];

		return X_OK;
	}
	else
		return X_ERR_NOT_SUPPORTED;
}





