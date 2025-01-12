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
//	Module m_pName:	XProfilerNode.h
//	Author:			Lynn Duke
//	Creation Date:	5-3-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PROFILER_NODE_H__
#define __X_PROFILER_NODE_H__

#include <XOSMgr.h>


//*****************************************************************************
//*****************************************************************************
class XProfilerNode
{
protected:
	XS8*			m_pName;
	int				m_totalCalls;
	XF32			m_totalTime;
	XF32			m_slowestTime;
	XF32			m_lastTime;
	XF32			m_startTime;
	int				m_recursionCounter;
	XProfilerNode*	m_pParent;
	XProfilerNode*	m_pChild;
	XProfilerNode*	m_pSibling;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XProfilerNode(XS8* pName,XProfilerNode* pParent);
	XProfilerNode();
	~XProfilerNode();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XProfilerNode* getSubNode(XS8* pName);
	XProfilerNode* getParent(){return m_pParent;}
	void setParent(XProfilerNode* pParent){m_pParent = pParent;}
	XProfilerNode* getSibling(){return m_pSibling;}
	XProfilerNode* getChild(){return m_pChild;}
	void reset();
	void call();
	XBOOL stop();
	XS8* getName(){return m_pName;}
	void setName(XS8* pName){m_pName = pName;}
	int getTotalCalls(){return m_totalCalls;}
	XF32 getTotalTime(){return m_totalTime;}
	XF32 getLastTime(){return m_lastTime;}
	XF32 getSlowestTime(){return m_slowestTime;}
};

#endif
