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
//	Module Name:	XOggStream.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-30-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XOggStream::XOggStream()
{
	m_numBuffersInQueue = 0;
	m_pSoundData = NULL;
}



//*****************************************************************************
//*****************************************************************************
XOggStream::~XOggStream()
{
}


//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::initFromFile(XS8* pFilename)
{
	//Make sure we have a valid filename.
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	XStatus returnCode = X_OK;
	XBOOL createdOpenALBuffers = false;

	//Generate buffers for the new stream
	XNewArray(m_pALBuffers,ALuint,X_AL_MAX_BUFFERS,XOSMgr::getDefaultHeap());
	alGenBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
	if (alGetError() != AL_NO_ERROR)
		returnCode = X_ERR_OPERATION_FAILED;
	else
	{
		createdOpenALBuffers = true;

		//Open file.
		FILE* pFile = fopen(pFilename,"rb");
		if (!pFile)
        	returnCode = X_ERR_OPERATION_FAILED;
		else
		{
			ov_open(pFile,&m_oggFileStruct, NULL, 0);

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
		}
	}

	//If we ran into an error, free data that won't be used.
	if (returnCode != X_OK)
	{
		if (createdOpenALBuffers)
			alDeleteBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
		
		XDeleteArray(m_pALBuffers);
	}

	return returnCode;
}


XStatus XOggStream::initFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize)
{
	X_ASSERT(pMemoryChunk);

	//Since we are going to stream from memory, let's make a copy of the sound 
	//data for our own use.  We'll stream from this buffer.
	XNewArray(m_pSoundData,XS8,memChunkSize,XOSMgr::getDefaultHeap());
	memcpy(m_pSoundData,pMemoryChunk,memChunkSize);

	XStatus returnCode = X_OK;
	XBOOL createdOpenALBuffers = false;

	//Generate buffers for the new stream
	XNewArray(m_pALBuffers,ALuint,X_AL_MAX_BUFFERS,XOSMgr::getDefaultHeap());
	alGenBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
	if (alGetError() != AL_NO_ERROR)
		returnCode = X_ERR_OPERATION_FAILED;
	else
	{
		createdOpenALBuffers = true;

		ov_callbacks	vorbisCallbacks;
		vorbisCallbacks.read_func = VorbisRead;
		vorbisCallbacks.close_func = VorbisClose;
		vorbisCallbacks.seek_func = VorbisSeek;
		vorbisCallbacks.tell_func = VorbisTell;

		m_oggMemoryFile.dataPtr = m_pSoundData;
		m_oggMemoryFile.dataSize = memChunkSize;
		m_oggMemoryFile.dataRead = 0;

		XS32 success = ov_open_callbacks(&m_oggMemoryFile, &m_oggFileStruct, NULL, 0, vorbisCallbacks);
		X_ASSERTM(success == 0,"Could not read Ogg file from memory, ov_open_callbacks failed");

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
	}

	//If we ran into an error, free data that won't be used.
	if (returnCode != X_OK)
	{
		if (createdOpenALBuffers)
			alDeleteBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
		
		XDeleteArray(m_pALBuffers);

		XDeleteArray(m_pSoundData);
	}

	return returnCode;
}


//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::destroy()
{
	//First stop the stream in case it's playing.  The source contained in this
	//object is only valid if we are not finished playing, otherwise, the source
	//data member is invalid.
	if (m_playing)
		handleStopNotification(m_ALSource);

	//Destroy the ogg bitstream.
	ov_clear(&m_oggFileStruct);

	//Destroy the buffers
	alDeleteBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
	
	XDeleteArray(m_pALBuffers);
	XResource::destroy();

	XDeleteArray(m_pSoundData);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::handlePlayNotification(ALuint source)
{
	//If the stream is already playing, don't play it again.
	if (m_playing)
		return X_ERR_NOT_READY;

	//Prepare the stream for playback.
	ov_time_seek(&m_oggFileStruct,0);
	m_playing = true;
	m_numBuffersInQueue = 0;
	
	for (XU32 i = 0;i < X_AL_MAX_BUFFERS;++i)
	{
		if (fillBuffer(m_pALBuffers[i]))
			++m_numBuffersInQueue;
		else
			break;
	}

	//Queue the buffers on the source
	alSourceQueueBuffers(source, m_numBuffersInQueue,m_pALBuffers);
	if (alGetError() != AL_NO_ERROR)
		return X_ERR_INVALID_PARAMS;

	//Set all properties of the sound to the source.
	//Set looping to false, we handle our own looping
	//for streams.
	alSourcef(source,AL_GAIN,m_volume);
	alSourcef(source,AL_PITCH,m_pitch);
	alSourcei(source,AL_LOOPING,false);

				
	if (m_isMono)
	{
		alSourcefv(source,AL_POSITION,m_position);
		alSourcefv(source,AL_VELOCITY,m_velocity);
		alSourcef(source,AL_ROLLOFF_FACTOR,m_rolloffFactor);
	}

	//Start playing source
	alSourcePlay(source);
	if (alGetError() != AL_NO_ERROR)
		return X_ERR_INVALID_PARAMS;

	m_ALSource = source;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::handleStopNotification(ALuint source)
{
	ALuint bufferID;

	//Stop the source
	alSourceStop(source);

	//Unqueue used and unused buffers		
	for (XU32 j = 0;j < m_numBuffersInQueue;++j)
		alSourceUnqueueBuffers(source,1,&bufferID);
	
	//Mark the stream as not playing.
	m_numBuffersInQueue = 0;
	m_playing = false;
	m_ALSource = 0;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::handleUpdateNotification(ALuint source)
{
	ALint processed;
	ALuint bufferID;

	alGetSourcei(source,AL_BUFFERS_PROCESSED,&processed);

	if (m_playing)
	{
		//If some buffers have been played, unqueue them and load new
		//audio into them, then add them to the queue.
		if (processed > 0)
		{
			while (processed)
			{
				alSourceUnqueueBuffers(source,1,&bufferID);
				if (alGetError() != AL_NO_ERROR)
					return X_ERR_OPERATION_FAILED;

				if (!fillBuffer(bufferID))
				{
					--m_numBuffersInQueue;
				}
				else
				{
					//Queue buffer
					alSourceQueueBuffers(source,1,&bufferID);
					if (alGetError() != AL_NO_ERROR)
						return X_ERR_OPERATION_FAILED;
				}

				--processed;

				if (m_numBuffersInQueue == 0)
				{
					if (!m_loop)
					{
						alSourceStop(source);
						m_playing = false;
						return X_ERR_NOT_RUNNING;
					}
					else
					{
                        //Restart the sound
						m_playing = false;
						handlePlayNotification(source);
						return X_OK;
					}
				}
			}
		}
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOggStream::fillBuffer(ALuint buffer)
{    
	int size = 0;
	int section;
	int result;
	
	while(size < X_AL_BUFFER_SIZE)
	{
		result = ov_read(&m_oggFileStruct, m_pData + size, X_AL_BUFFER_SIZE - size, 0, 2, 1, & section);
		if(result > 0)
			size += result;
		else
		{
			if(result < 0)
				return false;
			else
				break;
		}
	}
	
	if(size == 0)
		return false;
	
	alBufferData(buffer, m_format, m_pData, size, m_pVorbisInfo->rate);
	if (alGetError() != AL_NO_ERROR)
		return false;

	return true;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::setVolume(XF32 volume)
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
XF32 XOggStream::getVolume()
{
	return m_volume;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::setPitch(XF32 pitch)
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
XF32 XOggStream::getPitch()
{
	return m_pitch;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::setLoopingState(XBOOL enableLooping)
{
	m_loop = enableLooping;
	
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XOggStream::getLoopingState()
{
	return m_loop;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::setRolloffFactor(XF32 rolloffFactor)
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
XF32 XOggStream::getRolloffFactor()
{
	return m_rolloffFactor;
}



//*****************************************************************************
//*****************************************************************************
XStatus XOggStream::setPosition(XVector3D& position)
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
XStatus XOggStream::getPosition(XVector3D& position)
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
XStatus XOggStream::setVelocity(XVector3D& velocity)
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
XStatus XOggStream::getVelocity(XVector3D& velocity)
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





