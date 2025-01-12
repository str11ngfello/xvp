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
//	Module Name:	XAnimTrackInfo.h
//	Author:			Lynn Duke
//	Creation Date:	7-24-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ANIM_TRACK_INFO_H__
#define __X_ANIM_TRACK_INFO_H__




//*****************************************************************************
//Animation Track Defines
//*****************************************************************************
#define X_AT_PLAYING				0x00000001 
#define X_AT_PAUSED				0x00000002


struct XAnimTrackInfo
{
	XS32 playCount;						//number of times to loop animation
	XU32 currentPlayTime;				//current animation time in milliseconds
	XF32 speedScale;					//playback speed
	XF32 weight;						//animation weight
	XU32 flags;						//boolean flags describing animation
	XMemBlockList<XAnimTimeEvent> timeEvents;	//registered time events
	void* pEventCallbackData;			//event callback data
	void (*pEventCallback)	(X_ANIM_EVENT_TYPE type,XU32 time,void* pUserData);


	XAnimTrackInfo::XAnimTrackInfo()
	{
		playCount = 0;
		currentPlayTime = 0;
		speedScale = 0;
		weight = 0;
		flags = 0;
		pEventCallbackData = NULL;
		pEventCallback = NULL;
	}

	XAnimTrackInfo::~XAnimTrackInfo()
	{
		timeEvents.clear();
	}
};


#endif