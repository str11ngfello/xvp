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
//	Module Name:	XAudioMgr.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_AUDIO_MGR_H__
#define __X_AUDIO_MGR_H__

#include <XOSMgr.h>
#include <XResourceList.h>
#include <XVPDesc.h>
#include <XMemBlockList.h>
#include <XBaseSound.h>
#include <al.h>
#include <alc.h>
#include <al/alut.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>



//*****************************************************************************
//! Sound device and resource manager.
/**
The XAudioMgr object represents the main audio driver for X-VP.  The engine
manages the audio driver which deals with aspects of sound devices as well as
resources and data needed by the devices.  You can use the audio driver to load,
play and othewise manipulate sound data for your application.

See below for more information.
*/
//*****************************************************************************
class XAudioMgr
{
private:
	
	struct SourceInfo
	{
		ALuint source;
		XHandle soundHandle;
		XU8 priority;

		bool SourceInfo::operator == (const SourceInfo& S)
		{
			if(source == S.source)
				return true;
			else
				return false;
		}
	};

	
	//*****************************************************************************
	//Resource Array Information
	//*****************************************************************************
	XResourceList			m_sounds;
	XMemBlockList<SourceInfo>		m_freeSourceList;
	XMemBlockList<SourceInfo>		m_usedSourceList;
	XMemBlockList<SourceInfo>		m_removeList;

	//*****************************************************************************
	//Driver State Information
	//*****************************************************************************
	XHeap*				m_pGenericHeap;
	ALCdevice*			m_pOpenALDevice;
	ALCcontext*			m_pOpenALContext;
	ALint				m_openALError;
	XStatus				m_status;
	CRITICAL_SECTION	m_soundCS;
	ULONG32				m_threadID;
	HANDLE				m_threadHandle;	
	bool				m_threadOn;
	HANDLE				m_threadEvent;	
	bool				m_inited;
	XU32				m_audioStreamThreshold;

	XStatus internalHijackSound(XU8 priority);
	XStatus initLuaGlue();

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XAudioMgr();
	~XAudioMgr();

	//*****************************************************************************
	//Standard XAudioMgr Functions
	//*****************************************************************************
	XStatus init(XVPDesc& desc);
	XStatus update(XF32 dt);
	XStatus destroy();

	XStatus createSoundFromFile(XS8* pFilename,XHandle& soundHandle,bool absolutePath = false);
	XStatus createSoundFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize,XHandle& soundHandle);
	XStatus destroySound(XHandle soundHandle);
	XStatus destroyAllSounds();
	XStatus playSound(XHandle soundHandle,XU8 priority = 255);
	XStatus stopSound(XHandle soundHandle);
	XStatus stopAllSound();
	bool isSoundPlaying(XHandle soundHandle);
	
	
	/*XStatus muteSound(XHandle soundHandle);
	XStatus unmuteSound(XHandle soundHandle);
	//bool isSoundMuted(XHandle soundHandle);
	XStatus pauseSound(XHandle soundHandle);
	XStatus resumeSound(XHandle soundHandle);
	//bool isSoundPaused(XHandle soundHandle);
	XStatus setMasterVolume(XF32 volume);
	XF32 getMasterVolume();

	*/

	XStatus setSoundVolume(XHandle soundHandle,XF32 volume);
	XF32 getSoundVolume(XHandle soundHandle);

	XStatus setSoundPitch(XHandle soundHandle,XF32 pitch);
	XF32 getSoundPitch(XHandle soundHandle);

	XStatus setSoundLoopingState(XHandle soundHandle,bool enableLooping);
	bool getSoundLoopingState(XHandle soundHandle);

	XStatus setSoundRolloffFactor(XHandle soundHandle,XF32 rolloffFactor);
	XF32 getSoundRolloffFactor(XHandle soundHandle);

	XStatus setSoundPosition(XHandle soundHandle,XVector3D& position);
	XStatus getSoundPosition(XHandle soundHandle,XVector3D& position);

	XStatus setSoundVelocity(XHandle soundHandle,XVector3D& velocity);
	XStatus getSoundVelocity(XHandle soundHandle,XVector3D& velocity);
	
	XStatus setListenerTransform(XMatrix4x4& transform);
	XStatus setListenerTransform(XVector3D& lookVector,XVector3D& upVector);
	XStatus getListenerTransform(XVector3D& lookVector,XVector3D& upVector);
	XStatus setListenerPosition(XVector3D& position);
	XStatus getListenerPosition(XVector3D& position);
	XStatus setListenerVelocity(XVector3D& velocity);
	XStatus getListenerVelocity(XVector3D& velocity);
	XStatus setDopplerFactor(XF32 dopplerFactor);
	XF32 getDopplerFactor();
	XStatus setDopplerVelocity(XF32 dopplerVelocity);
	XF32 getDopplerVelocity();
	XStatus addSoundRef(XHandle soundHandle);
	XStatus streamPump();

};


DWORD WINAPI XAudioDriverUpdatePumper(LPVOID pParam);


#endif