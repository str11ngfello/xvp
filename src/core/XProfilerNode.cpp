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
//	Module m_pName:	XProfilerNode.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-3-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
//*****************************************************************************
XProfilerNode::XProfilerNode(XS8* pName,XProfilerNode* pParent) :
	m_pName(pName),
	m_totalCalls(0),
	m_totalTime(0),
	m_slowestTime(0),
	m_lastTime(0),
	m_startTime(0),
	m_recursionCounter(0),
	m_pParent(pParent),
	m_pChild(NULL),
	m_pSibling(NULL)
{
	reset();
}



//*****************************************************************************
//*****************************************************************************
XProfilerNode::XProfilerNode() :
	m_pName(NULL),
	m_totalCalls(0),
	m_totalTime(0),
	m_slowestTime(0),
	m_lastTime(0),
	m_startTime(0),
	m_recursionCounter(0),
	m_pParent(NULL),
	m_pChild(NULL),
	m_pSibling(NULL)
{
	reset();
}



//*****************************************************************************
//*****************************************************************************
XProfilerNode::~XProfilerNode()
{
	XDelete(m_pChild);
	XDelete(m_pSibling);
	//XHeap::Deallocate(m_pChild);
	//XHeap::Deallocate(m_pSibling);
}



//*****************************************************************************
//*****************************************************************************
XProfilerNode* XProfilerNode::getSubNode(XS8* pName )
{
	// Try to find this sub node
	XProfilerNode * child = m_pChild;
	while (child)
	{
		if (child->m_pName == pName)
			return child;
		
		child = child->m_pSibling;
	}

	// We didn't find it, so add it
	XProfilerNode* node = NULL;
	XNew(node,XProfilerNode(pName,this),XOSMgr::getDebugHeap());
	//XProfilerNode* node = (XProfilerNode*)XOSMgr::getDebugHeap()->Allocate(sizeof(XProfilerNode),__FILE__,__LINE__);
	//node = new (node) XProfilerNode(pName,this);
	node->m_pSibling = m_pChild;
	
	m_pChild = node;
	return node;
}



//*****************************************************************************
//*****************************************************************************
void XProfilerNode::reset()
{
	m_totalCalls = 0;
	m_totalTime = 0;

	if (m_pChild)
		m_pChild->reset();
	
	if (m_pSibling) 
		m_pSibling->reset();	
}



//*****************************************************************************
//*****************************************************************************
void XProfilerNode::call()
{
	m_totalCalls++;
	if (m_recursionCounter++ == 0)
		m_startTime = XOSMgr::getSysTime();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XProfilerNode::stop()
{
	if (--m_recursionCounter == 0 && m_totalCalls != 0) 
	{ 
		XF32 endTime = XOSMgr::getSysTime();
		endTime -= m_startTime;
		m_totalTime += endTime;

		if (endTime > m_slowestTime)
			m_slowestTime = endTime;
		m_lastTime = endTime;
	}
	return (m_recursionCounter == 0);
}
