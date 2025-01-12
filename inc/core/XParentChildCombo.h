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
//	Module Name:	XParentChildCombo.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PARENT_CHILD_COMBO_H__
#define __X_PARENT_CHILD_COMBO_H__



//****************************************************************************
//This is a utility structure that allows us to store parent/child names for
//faster model instance creation.
//****************************************************************************
class XParentChildCombo
{
private:
	XS8* m_pParentName;
	XS8* m_pChildName;

public:
	XParentChildCombo();
	void destroy();
	void setParentName(XS8* pParentName);
	void setChildName(XS8* pChildName);
	XS8* getParentName();
	XS8* getChildName();
};

#endif