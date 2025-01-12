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
//	Module Name:	XProfiler.h
//	Author:			Lynn Duke
//	Creation Date:	5-3-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PROFILER_H__
#define __X_PROFILER_H__

#include <XOSMgr.h>
#include <XProfilerIterator.h>
#include <XUserInterface.h>
#include <XUITextBox.h>


class XProfilerNode;


//*****************************************************************************
//*****************************************************************************
class XProfiler
{
private:
	static XProfilerNode*		m_pRoot;
	static XProfilerNode*		m_pCurrentNode;
	static int					m_frameCounter;
	static XF32				m_resetTime;
	static XUserInterface		m_UI;
	static XF32				m_updateRate;
	static XF32				m_timePassed;
	static XUITextBox			m_textBox;
	static char					m_textBuffer[4096];
	static XBOOL			m_active;
	static XBOOL			m_inited;

public:
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus init(XHandle charSetHandle);
	static XStatus update(XF32 dt);
	static XStatus destroy();
	static XStatus show();
	static XStatus hide();
	static XBOOL isVisible();
	static XStatus setUpdateRate(XF32 updateRate);
	static XF32 getUpdateRate();
	static void	startProfile(XS8* pName);
	static void	stopProfile();
	static void	reset();
	static XBOOL isInitialized();
	static void	incrementFrameCounter();
	static int getFrameCountSinceReset(){return m_frameCounter;}
	static XF32 getTimeSinceReset();
	static void	releaseIterator(XProfilerIterator* iterator){delete iterator;}
	static XProfilerIterator* getIterator(){return new XProfilerIterator(m_pRoot);}
};

#endif

