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
//	Module Name:	XOggSound.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XResourceList XOggSound::m_soundBuffers;



//*****************************************************************************
//*****************************************************************************
XOggSound::XOggSound()
{
	m_soundBufferHandle = X_INVALID_HANDLE;
}



//*****************************************************************************
//*****************************************************************************
XOggSound::~XOggSound()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggSound::initFromFile(XS8* pFilename,XU32 streamThreshold)
{
	//Make sure we have a valid path
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	XStatus returnCode = X_OK;
	XBOOL createdOpenALBuffer = false;
	XBOOL createdOggBitStream = false;
	SoundBuffer* pSoundBuffer = NULL;
	XS8* pData = NULL;

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

			//Open file.
			FILE* pFile = fopen(pFilename,"rb");
			if (!pFile)
				returnCode = X_ERR_OPERATION_FAILED;
			else
			{
				ov_open(pFile,&m_oggFileStruct, NULL, 0);
				createdOggBitStream = true;

				ALenum m_format;
				m_pVorbisInfo = ov_info(&m_oggFileStruct, -1);
				if (m_pVorbisInfo->channels == 1)
				{
					m_format = AL_FORMAT_MONO16;
					m_isMono = true;
				}
				else 
				{
					m_format = AL_FORMAT_STEREO16;
					m_isMono = false;
				}

				//Copy sound data into the openAL buffer. 
				int size = 0;
				int section;
				int result;
				XNewArray(pData,XS8,streamThreshold,XOSMgr::getDefaultHeap());
				while((XU32)size < streamThreshold)
				{
					result = ov_read(&m_oggFileStruct, pData + size, streamThreshold - size, 0, 2, 1, &section);
					if(result > 0)
						size += result;
					else
					{
						if(result < 0)
						{
							returnCode = X_ERR_OPERATION_FAILED;
							break;
						}
						else
							break;
					}
				}

				//We're finished with the file .
				fclose(pFile);

				//Make sure we didn't fail.
				if (returnCode == X_OK)
				{
					//If the file was too large to be decompressed and fit in the
					//threshold limit, we quit.
					if (size == streamThreshold)
					{
						returnCode = X_ERR_OPERATION_FAILED;
					}
					else
					{
						//We only make it here if we successfully loaded the entire ogg
						//file into memory...up to the threshold.
						alBufferData(pSoundBuffer->alBuffer, m_format, pData, size, m_pVorbisInfo->rate);
						if (alGetError() != AL_NO_ERROR)
							returnCode = X_ERR_OPERATION_FAILED;
						else
						{
							m_soundBufferHandle = m_soundBuffers.addResource(pSoundBuffer,pFilename);
							if (m_soundBufferHandle != X_INVALID_HANDLE)
								returnCode = X_OK;
							else
								returnCode = X_ERR_OPERATION_FAILED;
						}
					}
				}
			}
		}
	}

	//Free the temp data
	XDeleteArray(pData);

	//Destroy the ogg bitstream.
	if (createdOggBitStream)
		ov_clear(&m_oggFileStruct);

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
XStatus XOggSound::initFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize,XU32 streamThreshold)
{
	X_ASSERT(pMemoryChunk);

	XStatus returnCode = X_OK;
	XBOOL createdOpenALBuffer = false;
	XBOOL createdOggBitStream = false;
	SoundBuffer* pSoundBuffer = NULL;
	XS8* pData = NULL;
	
	ov_callbacks	vorbisCallbacks;
	vorbisCallbacks.read_func = VorbisRead;
	vorbisCallbacks.close_func = VorbisClose;
	vorbisCallbacks.seek_func = VorbisSeek;
	vorbisCallbacks.tell_func = VorbisTell;
	
	SOggFile oggMemoryFile;
	oggMemoryFile.dataPtr = pMemoryChunk;
	oggMemoryFile.dataSize = memChunkSize;
	oggMemoryFile.dataRead = 0;

	XResource* pResource = (SoundBuffer*)m_soundBuffers.findResourceByName(pName,m_soundBufferHandle);

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

			XS32 success = ov_open_callbacks(&oggMemoryFile, &m_oggFileStruct, NULL, 0, vorbisCallbacks);
			X_ASSERTM(success == 0,"Could not read Ogg file from memory, ov_open_callbacks failed");

			createdOggBitStream = true;

			ALenum m_format;
			m_pVorbisInfo = ov_info(&m_oggFileStruct, -1);
			if (m_pVorbisInfo->channels == 1)
			{
				m_format = AL_FORMAT_MONO16;
				m_isMono = true;
			}
			else 
			{
				m_format = AL_FORMAT_STEREO16;
				m_isMono = false;
			}

			//Copy sound data into the openAL buffer. 
			int size = 0;
			int section;
			int result;
			XNewArray(pData,XS8,streamThreshold,XOSMgr::getDefaultHeap());
			while((XU32)size < streamThreshold)
			{
				result = ov_read(&m_oggFileStruct, pData + size, streamThreshold - size, 0, 2, 1, &section);
				if(result > 0)
					size += result;
				else
				{
					if(result < 0)
					{
						returnCode = X_ERR_OPERATION_FAILED;
						break;
					}
					else
						break;
				}
			}

			//Make sure we didn't fail.
			if (returnCode == X_OK)
			{
				//If the file was too large to be decompressed and fit in the
				//threshold limit, we quit.
				if (size == streamThreshold)
				{
					returnCode = X_ERR_OPERATION_FAILED;
				}
				else
				{
					//We only make it here if we successfully loaded the entire ogg
					//file into memory...up to the threshold.
					alBufferData(pSoundBuffer->alBuffer, m_format, pData, size, m_pVorbisInfo->rate);
					if (alGetError() != AL_NO_ERROR)
						returnCode = X_ERR_OPERATION_FAILED;
					else
					{
						m_soundBufferHandle = m_soundBuffers.addResource(pSoundBuffer,pName);
						if (m_soundBufferHandle != X_INVALID_HANDLE)
							returnCode = X_OK;
						else
							returnCode = X_ERR_OPERATION_FAILED;
					}
				}
			}
		}
	}

	//Free the temp data
	XDeleteArray(pData);

	//Destroy the ogg bitstream.
	if (createdOggBitStream)
		ov_clear(&m_oggFileStruct);

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
XStatus XOggSound::destroy()
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
XStatus XOggSound::handlePlayNotification(ALuint source)
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
XStatus XOggSound::handleStopNotification(ALuint source)
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
XStatus XOggSound::handleUpdateNotification(ALuint source)
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
XStatus XOggSound::setVolume(XF32 volume)
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
XF32 XOggSound::getVolume()
{
	return m_volume;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggSound::setPitch(XF32 pitch)
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
XF32 XOggSound::getPitch()
{
	return m_pitch;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggSound::setLoopingState(XBOOL enableLooping)
{
	m_loop = enableLooping;
	
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XOggSound::getLoopingState()
{
	return m_loop;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggSound::setRolloffFactor(XF32 rolloffFactor)
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
XF32 XOggSound::getRolloffFactor()
{
	return m_rolloffFactor;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggSound::setPosition(XVector3D& position)
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
XStatus XOggSound::getPosition(XVector3D& position)
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
XStatus XOggSound::setVelocity(XVector3D& velocity)
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
XStatus XOggSound::getVelocity(XVector3D& velocity)
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




