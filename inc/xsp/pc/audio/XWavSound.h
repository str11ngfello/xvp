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
//	Module Name:	XWavSound.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_WAV_SOUND_H__
#define __X_WAV_SOUND_H__

#include <XOSMgr.h>
#include <XResourceList.h>
#include <XResource.h>
#include <XBaseSound.h>




//*****************************************************************************
//! Provides .wav file capability.
//*****************************************************************************
class XWavSound : public XBaseSound
{
private:

	class SoundBuffer : public XResource
	{
	public:
		ALuint alBuffer;

		XStatus SoundBuffer::destroy()
		{
			//Destroy the sound buffer
			alDeleteBuffers(1,&alBuffer);

			return X_OK;
		}
	};

	XHandle m_soundBufferHandle;
	
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XWavSound();
	virtual ~XWavSound();

	static XResourceList m_soundBuffers;


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