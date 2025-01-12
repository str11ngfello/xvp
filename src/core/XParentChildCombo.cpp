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
//	Module Name:	XParentChildCombo.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

	XParentChildCombo::XParentChildCombo()
	{
		m_pParentName = NULL;
		m_pChildName = NULL;
	}

	void XParentChildCombo::destroy()
	{
		XOSMgr::destroyString(&m_pParentName);
		XOSMgr::destroyString(&m_pChildName);
	}

	void XParentChildCombo::setParentName(XS8* pParentName)
	{
		m_pParentName = XOSMgr::createString(pParentName);
	}

	void XParentChildCombo::setChildName(XS8* pChildName)
	{
		m_pChildName = XOSMgr::createString(pChildName);
	}

	XS8* XParentChildCombo::getParentName()
	{
		return m_pParentName;
	}

	XS8* XParentChildCombo::getChildName()
	{
		return m_pChildName;
	}