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
//	Module Name:	XOggStream.h
//	Author:			Lynn Duke
//	Creation Date:	1-30-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_OGG_STREAM_H__
#define __X_OGG_STREAM_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XBaseSound.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <XOggVorbisHelpers.h>



//*****************************************************************************
//! Provides streaming .ogg file capability.
//*****************************************************************************
class XOggStream : public XBaseSound
{
public:
	OggVorbis_File	m_oggFileStruct;
	vorbis_info*	m_pVorbisInfo;
	ALuint*			m_pALBuffers;
	XU8			m_numBuffers;
	ALenum			m_format;
	char			m_pData[X_AL_BUFFER_SIZE];
	XU32			m_numBuffersInQueue;
	XS8*			m_pSoundData;
	bool fillBuffer(ALuint buffer);

	SOggFile	m_oggMemoryFile;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XOggStream();
	~XOggStream();

	//*****************************************************************************
	//Standard Sound Functions
	//*****************************************************************************
	XStatus initFromFile(XS8* pFilename);
	XStatus initFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize);
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