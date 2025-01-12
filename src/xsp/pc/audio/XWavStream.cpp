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
//	Module Name:	XWavStream.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XWavStream::XWavStream()
{
	m_pFile = NULL;
	m_readMoreData = false;
	m_numBuffersInQueue = X_AL_MAX_BUFFERS;
}



//*****************************************************************************
//*****************************************************************************
XWavStream::~XWavStream()
{
}


//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::init(XS8* pFilename)
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

		//Open the file and read in the wav header
		m_pFile = fopen(pFilename,"rb");
		fread(&m_wavHeader,1,sizeof(WavHeader),m_pFile);

		//We only support uncompressed/PCM format.  Perform a sanity check
		//to ensure that we don't have an unsupported format on our hands.
		if ((m_wavHeader.fmtSize != 16) || (m_wavHeader.Format != 1))
			returnCode = X_ERR_NOT_SUPPORTED;
		else
		{
			//Determine the format.  We only support the formats below.
			if ((m_wavHeader.Channels == 1) && (m_wavHeader.BitsPerSample == 16))
			{
				m_format = AL_FORMAT_MONO16;
				m_isMono = true;
			}
			else if ((m_wavHeader.Channels == 1) && (m_wavHeader.BitsPerSample == 8))
			{
				m_format = AL_FORMAT_MONO8;
				m_isMono = true;
			}
			else if ((m_wavHeader.Channels == 2) && (m_wavHeader.BitsPerSample == 16))
			{
				m_format = AL_FORMAT_STEREO16;
				m_isMono = false;
			}
			else if ((m_wavHeader.Channels == 2) && (m_wavHeader.BitsPerSample == 8))
			{
				m_format = AL_FORMAT_STEREO8;
				m_isMono = false;
			}
			else
				returnCode = X_ERR_NOT_SUPPORTED;
		}
	}

	if (returnCode != X_OK)
	{
		if (createdOpenALBuffers)
			alDeleteBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
		XDeleteArray(m_pALBuffers);
	}

	return returnCode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::destroy()
{
	//First stop the stream in case it's playing.  The source contained in this
	//object is only valid if we are not finished playing, otherwise, the source
	//data member is invalid.
	if (m_playing)
		handleStopNotification(m_ALSource);

	//Destroy the buffers
	alDeleteBuffers(X_AL_MAX_BUFFERS,m_pALBuffers);
			
	//Close the file
	if (m_pFile)
	{
		fclose(m_pFile);		
		m_pFile = NULL;
	}

	XDeleteArray(m_pALBuffers);
	XResource::destroy();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::handlePlayNotification(ALuint source)
{
	//If the stream is already playing, don't play it again.
	if (m_playing)
		return X_ERR_NOT_READY;

	//Prepare the stream for playback.
	fseek(m_pFile,sizeof(WavHeader),SEEK_SET);
	m_dataSize = m_wavHeader.dataSize;
	m_readMoreData = true;
	m_playing = true;
	m_numBuffersInQueue = 0;
	
	for (XU32 i = 0;i < X_AL_MAX_BUFFERS;++i)
	{
		XU32 bytesRead = (XU32)fread(m_pData, 1, X_AL_BUFFER_SIZE, m_pFile);
		m_dataSize -= bytesRead;
		alBufferData(m_pALBuffers[i], m_format, 
				 m_pData, bytesRead, m_wavHeader.SamplesPerSec);
		++m_numBuffersInQueue;
		if (alGetError() != AL_NO_ERROR)
			return X_ERR_INVALID_PARAMS;
		if (m_dataSize == 0)
			break;
	}

	//Queue the buffers on the source
	alSourceQueueBuffers(source,m_numBuffersInQueue,m_pALBuffers);
	if (alGetError() != AL_NO_ERROR)
		return X_ERR_INVALID_PARAMS;

    //Set all properties of the sound to the source
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
		return X_ERR_OPERATION_FAILED;

	m_ALSource = source;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::handleStopNotification(ALuint source)
{
	ALuint bufferID;

	//Stop the source
	alSourceStop(source);

	//Unqueue used and unused buffers		
	for (XU32 j = 0;j < X_AL_MAX_BUFFERS;++j)
		alSourceUnqueueBuffers(source,1,&bufferID);
	
	//Mark the stream as not playing.
	m_numBuffersInQueue = 0;
	m_playing = false;
	m_readMoreData = false;
	m_ALSource = 0;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::handleUpdateNotification(ALuint source)
{
	ALint processed;
	ALuint bufferID;
	ALuint dataToRead;

	alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

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

				if (m_readMoreData)
				{
					dataToRead = (m_dataSize > X_AL_BUFFER_SIZE) ? X_AL_BUFFER_SIZE : m_dataSize;
				
					fread(m_pData,1,dataToRead,m_pFile);
					m_dataSize -= dataToRead;
						
					if (m_dataSize == 0)
					{
						//If we aren't suppose to loop mark this as finished. Otherwise,
						//we need to start over in the file.
						if (!m_loop)
							m_readMoreData = false;
						else
						{
							fseek(m_pFile,sizeof(WavHeader),SEEK_SET);
							m_dataSize = m_wavHeader.dataSize;
						}

					}

					alBufferData(bufferID,m_format,m_pData,dataToRead,m_wavHeader.SamplesPerSec);
					if (alGetError() != AL_NO_ERROR)
						return X_ERR_OPERATION_FAILED;

					//Queue buffer
					alSourceQueueBuffers(source,1,&bufferID);
					if (alGetError() != AL_NO_ERROR)
						return X_ERR_OPERATION_FAILED;

					--processed;
				}
				else
				{
					--m_numBuffersInQueue;
					--processed;

					if (m_numBuffersInQueue == 0)
					{
						alSourceStop(source);
						m_playing = false;
							
						return X_ERR_NOT_RUNNING;
					}
				}
			}
		}
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::setVolume(XF32 volume)
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
XF32 XWavStream::getVolume()
{
	return m_volume;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::setPitch(XF32 pitch)
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
XF32 XWavStream::getPitch()
{
	return m_pitch;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::setLoopingState(XBOOL enableLooping)
{
	m_loop = enableLooping;
	
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XWavStream::getLoopingState()
{
	return m_loop;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::setRolloffFactor(XF32 rolloffFactor)
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
XF32 XWavStream::getRolloffFactor()
{
	return m_rolloffFactor;
}



//*****************************************************************************
//*****************************************************************************
XStatus XWavStream::setPosition(XVector3D& position)
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
XStatus XWavStream::getPosition(XVector3D& position)
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
XStatus XWavStream::setVelocity(XVector3D& velocity)
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
XStatus XWavStream::getVelocity(XVector3D& velocity)
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





