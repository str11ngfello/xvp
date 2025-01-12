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
//	Module Name:	XScreenFader.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the screen fader.
*/
//*****************************************************************************
XScreenFader::XScreenFader()
{
	m_initialRed = 0;
	m_initialGreen = 0;
	m_initialBlue = 0;
	m_destRed = 0;
	m_destGreen = 0;
	m_destBlue = 0;
	m_fadeLength = 0;
	m_time = 0;
	m_fading = false;
}



//*****************************************************************************
/**
Destructor for the screen fader.
*/
//*****************************************************************************
XScreenFader::~XScreenFader()
{
}


																				
//*****************************************************************************
/**
Determines if the screen fader has a screen fade operation in progress.

@return True if a screen fade is in progress, false otherwise.
*/
//*****************************************************************************
XBOOL XScreenFader::isFading()
{
	return m_fading;
}



//*****************************************************************************
/**
Allows the screen fader to update any internal, time-dependent data.  This
method should be called once per frame by the user application to update the
screen fading process until the screen fade is over. 

@param dt	The time, in seconds, that has elapsed since the last 
					update.
@return	True if the screen fade needs more time to finish, false if
		the screen fade is complete.
*/
//*****************************************************************************
XBOOL XScreenFader::update(XF32 dt)
{
	if (m_fading)
	{
		m_time += dt;
		if (m_time >= m_fadeLength)
		{
			m_fading = false;
			m_time = m_fadeLength;
		}
		XF32 completionDelta = m_time/m_fadeLength;
#ifdef OLD
		XVP::GraphicsMgr.setRGBGammaPercent((completionDelta * (m_destRed - m_initialRed)) + m_initialRed,
									  (completionDelta * (m_destGreen - m_initialGreen)) + m_initialGreen,
									  (completionDelta * (m_destBlue - m_initialBlue)) + m_initialBlue);		
#endif
	}

	return m_fading;
}



//*****************************************************************************
/**
Starts a screen fade.  If a screen fade is already in progress, this method
will interrupt it and start a new fade.

@param initialRed	The initial red gamma setting.  The range for this parameter
					is 0 to 1.  0 means no red intensity and 1 means full red
					intensity.  If this value is -1, the red component will
					start at the current red intensity of the screen.
@param initialGreen	The initial red gamma setting.  The range for this parameter
					is 0 to 1.  0 means no red intensity and 1 means full red
					intensity.  If this value is -1, the red component will
					start at the current red intensity of the screen.
@param initialBlue	The initial red gamma setting.  The range for this parameter
					is 0 to 1.  0 means no red intensity and 1 means full red
					intensity.  If this value is -1, the red component will
					start at the current red intensity of the screen.
@param destRed		The destination red gamma setting.  The range for this parameter
					is 0 to 1.  0 means no red intensity and 1 means full red
					intensity.  If this value is -1, the red component will
					start at the current red intensity of the screen.
@param destGreen	The destination red gamma setting.  The range for this parameter
					is 0 to 1.  0 means no red intensity and 1 means full red
					intensity.  If this value is -1, the red component will
					start at the current red intensity of the screen.
@param destBlue		The destination red gamma setting.  The range for this parameter
					is 0 to 1.  0 means no red intensity and 1 means full red
					intensity.  If this value is -1, the red component will
					start at the current red intensity of the screen.
@param timeInSeconds	The time from the start of the fade to the
						end of the fade.  Valid values are 0 or greater.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScreenFader::startFade(XF32 initialRed,XF32 initialGreen,XF32 initialBlue,
								   XF32 destRed,XF32 destGreen,XF32 destBlue,
								   XF32 timeInSeconds)
{
#if OLD
	//Make sure that parameters are valid.
	if (timeInSeconds <= 0)
		return X_ERR_INVALID_PARAMS;
	
	if (initialRed < 0)
		m_initialRed = XVP::GraphicsMgr.getRedGammaPercent();
	else
		m_initialRed = initialRed;
	if (initialGreen < 0)
		m_initialGreen = XVP::GraphicsMgr.getGreenGammaPercent();
	else
		m_initialGreen = initialGreen;
	if (initialBlue < 0)
		m_initialBlue = XVP::GraphicsMgr.getBlueGammaPercent();
	else
		m_initialBlue = initialBlue;
	if (destRed < 0)
		m_destRed = XVP::GraphicsMgr.getRedGammaPercent();
	else
		m_destRed = destRed;
	if (destGreen < 0)
		m_destGreen = XVP::GraphicsMgr.getGreenGammaPercent();
	else
		m_destGreen = destGreen;
	if (destBlue < 0)
		m_destBlue = XVP::GraphicsMgr.getBlueGammaPercent();
	else
		m_destBlue = destBlue;

	m_fadeLength = timeInSeconds;
	m_time = 0;
	m_fading = true;
#endif
	return X_OK;
}



//*****************************************************************************
/**
Stops a screen fade that is in progress.  If no screen fade is in progress,
this method has no effect.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScreenFader::stopFade()
{
	m_fading = false;
	return X_OK;
}



