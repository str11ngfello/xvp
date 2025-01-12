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
//	Module Name:	XBaseSound.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_SOUND_H__
#define __X_BASE_SOUND_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <al.h>
#include <alc.h>
#include <al/alut.h>
#include <XVector3D.h>


#define X_AL_BUFFER_SIZE		16384 
#define X_AL_MAX_BUFFERS		4  



//*****************************************************************************
//! Base Sound Object.
//*****************************************************************************
class XBaseSound : public XResource
{
protected:
	ALuint			m_ALSource;
	bool			m_playing;
	bool			m_loop;
	XF32			m_volume;
	XF32			m_pitch;
	XF32			m_rolloffFactor;
	bool			m_isMono;
	XF32			m_position[3];
	XF32			m_velocity[3];

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseSound();
	virtual ~XBaseSound();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus destroy() = 0;
	virtual XStatus handlePlayNotification(ALuint source) = 0;
	virtual XStatus handleStopNotification(ALuint source) = 0;
	virtual XStatus handleUpdateNotification(ALuint source) = 0;

	/*virtual XStatus pause() = 0;
	virtual XStatus resume() = 0;
	virtual bool isPaused() = 0;

	virtual XStatus mute() = 0;
	virtual XStatus unmute() = 0;
	virtual XStatus isMuted() = 0;
*/
	virtual XStatus setVolume(XF32 volume) = 0;
	virtual XF32 getVolume() = 0;

	virtual XStatus setPitch(XF32 pitch) = 0;
	virtual XF32 getPitch() = 0;

	virtual XStatus setLoopingState(bool enableLooping) = 0;
	virtual bool getLoopingState() = 0;

	virtual XStatus setPosition(XVector3D& position) = 0;
	virtual XStatus getPosition(XVector3D& position) = 0;

	virtual XStatus setVelocity(XVector3D& velocity) = 0;
	virtual XStatus getVelocity(XVector3D& velocity) = 0;

	virtual XStatus setRolloffFactor(XF32 rolloffFactor) = 0;
	virtual XF32 getRolloffFactor() = 0;
};


#endif