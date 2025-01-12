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
//	Module Name:	XProfilerIterator.cpp
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
XProfilerIterator::XProfilerIterator( XProfilerNode * start )
{
	m_pCurrentParent = start;
	m_pCurrentChild = m_pCurrentParent->getChild();
}



//*****************************************************************************
//*****************************************************************************
void XProfilerIterator::first()
{
	m_pCurrentChild = m_pCurrentParent->getChild();
}



//*****************************************************************************
//*****************************************************************************
void XProfilerIterator::next()
{
	m_pCurrentChild = m_pCurrentChild->getSibling();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XProfilerIterator::isDone()
{
	return m_pCurrentChild == NULL;
}



//*****************************************************************************
//*****************************************************************************
void XProfilerIterator::enterChild(int index)
{
	m_pCurrentChild = m_pCurrentParent->getChild();
	while ((m_pCurrentChild != NULL) && (index != 0))
	{
		index--;
		m_pCurrentChild = m_pCurrentChild->getSibling();
	}

	if ( m_pCurrentChild != NULL )
	{
		m_pCurrentParent = m_pCurrentChild;
		m_pCurrentChild = m_pCurrentParent->getChild();
	}
}



//*****************************************************************************
//*****************************************************************************
void XProfilerIterator::enterParent()
{
	if ( m_pCurrentParent->getParent() != NULL )
		m_pCurrentParent = m_pCurrentParent->getParent();

	m_pCurrentChild = m_pCurrentParent->getChild();
}

