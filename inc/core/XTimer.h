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
//	Module Name:	XTimer.h
//	Author:			Lynn Duke
//	Creation Date:	5-8-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TIMER_H__
#define __X_TIMER_H__



//*****************************************************************************
//! Represents a basic timer.
/**
The XTimer object is used to keep track of time in various ways.  You can
create as many timers as needed.
*/
//*****************************************************************************
class XTimer
{
private:
	XBOOL		m_running;
	XF64		m_duration;
	XF64		m_startTime;
	XF64		m_stopTime;
	XF64		m_lastElapsedTime;


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTimer();
	~XTimer();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XF32 getAbsoluteTime();
	XF32 getElapsedTime();
	XF32 getDuration();
	void reset();
	void start();
	void stop();
	XBOOL isRunning();
};

#endif