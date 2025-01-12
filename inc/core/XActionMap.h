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
//	Module Name:	XActionMap.h
//	Author:			Lynn Duke
//	Creation Date:	12-21-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ACTION_MAP_H__
#define __X_ACTION_MAP_H__




//*****************************************************************************
//! Abstracts input from logical actions.
/**
Action maps provide a layer of separation between logical actions and the ways
in which those logical actions are triggered.
*/
//*****************************************************************************
class XActionMap
{
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XActionMap();
	virtual ~XActionMap();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus update() = 0;
	virtual XF32 getActionValue(XU32 actionID) = 0;
};


#endif