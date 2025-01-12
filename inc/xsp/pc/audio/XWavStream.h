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
//	Module Name:	XWavStream.h
//	Author:			Lynn Duke
//	Creation Date:	1-22-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_WAV_STREAM_H__
#define __X_WAV_STREAM_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XBaseSound.h>




//*****************************************************************************
//! Provides streaming .wav file capability.
//*****************************************************************************
class XWavStream : public XBaseSound
{
public:

	//Wav file header
	struct WavHeader
	{
		ALubyte		riff[4];		
		ALsizei		riffSize;
		ALubyte		wave[4];		
		ALubyte		fmt[4];			
		ALuint		fmtSize;
		ALushort	Format;                              
		ALushort	Channels;
		ALuint		SamplesPerSec;
		ALuint		BytesPerSec;
		ALushort	BlockAlign;
		ALushort	BitsPerSample;
		ALubyte		data[4];		
		ALuint		dataSize;
	} ;


	ALuint*			m_pALBuffers;
	XU8			m_numBuffers;
	FILE*			m_pFile;
	ALenum			m_format;
	XU32			m_dataSize;
	XU8			m_pData[X_AL_BUFFER_SIZE];
	WavHeader		m_wavHeader;
	XU32			m_numBuffersInQueue;
	bool			m_readMoreData;
	

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XWavStream();
	~XWavStream();

	//*****************************************************************************
	//Standard Sound Functions
	//*****************************************************************************
	XStatus init(XS8* pFilename);
	XStatus destroy();

	XStatus handlePlayNotification(ALuint source);
	XStatus handleStopNotification(ALuint source);
	XStatus handleUpdateNotification(ALuint source);

	XStatus setVolume(XF32 volume);
	XF32 getVolume();

	XStatus setPitch(XF32 pitch);
	XF32 getPitch();

	XStatus setLoopingState(bool enableLooping);
	bool getLoopingState();

	XStatus setPosition(XVector3D& position);
	XStatus getPosition(XVector3D& position);

	XStatus setVelocity(XVector3D& velocity);
	XStatus getVelocity(XVector3D& velocity);

	XStatus setRolloffFactor(XF32 rolloffFactor);
	XF32 getRolloffFactor();
};


#endif