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
//	Module Name:	XTimer.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-8-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
/**
Constructor for the timer.  
*/
//*****************************************************************************
XTimer::XTimer()
{
	m_running = false;
	m_duration = 0;
	m_startTime = 0;
	m_stopTime = 0;
	m_lastElapsedTime = 0;

	reset();
}



//*****************************************************************************
/**
Destructor for the timer.  
*/
//*****************************************************************************
XTimer::~XTimer()
{
}



//*****************************************************************************
/**
Determines if the time is running.

@return True if the time is running, false othewise.
*/
//*****************************************************************************
XBOOL XTimer::isRunning()
{
	return m_running;
}



//*****************************************************************************
/**
Gets the time, in seconds, that has elapsed since the last getElapsedTime()
function call.  This function call is valid regardless of whether the timer
is running or not.

@return The elasped time, in seconds, since the last time this function
		was called.
*/
//*****************************************************************************
XF32 XTimer::getElapsedTime()
{
	//Return the elapsed time
	XF32 currentTime = 0;
	XF32 dt = 0;
	currentTime = XOSMgr::getSysTime();
	if (m_lastElapsedTime == 0)
		m_lastElapsedTime = currentTime;
	dt = (XF32)(currentTime - m_lastElapsedTime);
	m_lastElapsedTime = currentTime;

	return dt;
}
 


//*****************************************************************************
/**
Gets the amount of time, in seconds, the timer has been running.

@return The amount of time, in seconds, the timer has been running.
*/
//*****************************************************************************
XF32 XTimer::getDuration()
{
	if (m_running)
		return (XF32)(m_duration + (XOSMgr::getSysTime() - m_startTime));
	else
		return (XF32)m_duration;
}



//*****************************************************************************
/**
Resets the timer.

@return None.
*/
//*****************************************************************************
void XTimer::reset()
{
	m_startTime = XOSMgr::getSysTime();
	m_duration = 0;
}



//*****************************************************************************
/**
Starts the timer.  

@return None.
*/
//*****************************************************************************
void XTimer::start()
{
	if (!m_running)
	{
		m_running = true;
		m_startTime = XOSMgr::getSysTime();
	}
}
    


//*****************************************************************************
/**
Stops the timer.

@return None.
*/
//*****************************************************************************
void XTimer::stop()
{
	if (m_running)
	{
		m_stopTime = XOSMgr::getSysTime();
		m_running = false;
		m_duration += m_stopTime - m_startTime;
	}
}
    


//*****************************************************************************
/**
Gets the absolute time of the system in seconds.

@return The absolute system time in seconds.
*/
//*****************************************************************************
XF32 XTimer::getAbsoluteTime()
{
	return (XF32)XOSMgr::getSysTime();	
}
