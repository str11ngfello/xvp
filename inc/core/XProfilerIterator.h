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
//	Module Name:	XProfilerIterator.h
//	Author:			Lynn Duke
//	Creation Date:	5-3-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PROFILER_ITERATOR_H__
#define __X_PROFILER_ITERATOR_H__

#include <XOSMgr.h> 
#include <XProfilerNode.h> 



//*****************************************************************************
//*****************************************************************************
class XProfilerIterator
{
protected:
	XProfilerNode*	m_pCurrentParent;
	XProfilerNode*	m_pCurrentChild;

	XProfilerIterator(XProfilerNode* pStart);
	friend class XProfiler;

public:
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	void first();
	void next();
	XBOOL isDone();
	void enterChild(int index);
	void enterLargestChild();
	void enterParent();
	XS8* getCurrentName(){return m_pCurrentChild->getName();}
	int getCurrentTotalCalls(){return m_pCurrentChild->getTotalCalls();}
	XF32 getCurrentTotalTime(){return m_pCurrentChild->getTotalTime();}
	XF32 getCurrentLastTime(){return m_pCurrentChild->getLastTime();}
	XF32 getCurrentSlowestTime(){return m_pCurrentChild->getSlowestTime();}
	XS8* getCurrentParentName(){return m_pCurrentParent->getName();}
	int	getCurrentParentTotalCalls(){return m_pCurrentParent->getTotalCalls();}
	XF32 getCurrentParentTotalTime(){return m_pCurrentParent->getTotalTime();}
};

#endif
