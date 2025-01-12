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
//	Module Name:	XScreenFader.h
//	Author:			Lynn Duke
//	Creation Date:	7-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SCREEN_FADER_H__
#define __X_SCREEN_FADER_H__





//*****************************************************************************
//! Performs screen fading operations.
/**
The XScreenFader object performs various types of screen fading operations
by modifying the screen's gamma controls in various ways.  For example, this
object can be used to fade the screen to black or any other rgb color.     

See the member functions below for more information.
*/
//*****************************************************************************
class XScreenFader
{
private:
	XF32		m_initialRed;
	XF32		m_initialGreen;
	XF32		m_initialBlue;
	XF32		m_destRed;
	XF32		m_destGreen;
	XF32		m_destBlue;
	XF32		m_fadeLength;
	XF32		m_time;
	XBOOL		m_fading;

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XScreenFader();
	~XScreenFader();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XBOOL update(XF32 dt);
	XBOOL isFading();
	XStatus startFade(XF32 initialRed,XF32 initialGreen,XF32 initialBlue,
					   XF32 destRed,XF32 destGreen,XF32 destBlue,XF32 timeInSeconds);
	XStatus stopFade();
};

#endif