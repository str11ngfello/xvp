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
//	Module Name:	XAnimTimeEvent.h
//	Author:			Lynn Duke
//	Creation Date:	7-24-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ANIM_TIME_EVENT_H__
#define __X_ANIM_TIME_EVENT_H__



//*****************************************************************************
//Animation Event Types
//*****************************************************************************
enum X_ANIM_EVENT_TYPE
{
	X_AET_TIME,
	X_AET_START,
	X_AET_FINISH
};



//*****************************************************************************
//Structures
//*****************************************************************************
struct XAnimTimeEvent
{
	XU32 time;		//time the event should occur relative to anim start
	XU8 consumed;	//1 for consumed, 0 otherwise

	bool XAnimTimeEvent::operator == (XAnimTimeEvent E)
	{
		if (time == E.time)
			return true;
		else
			return false;
	}
};

#endif
