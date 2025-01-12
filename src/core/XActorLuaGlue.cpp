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
//	Module Name:	XUserIntefaceLuaGlue.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-27-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



int LAPI_load(lua_State* pLuaState)
{
	X_ASSERTF(lua_gettop(pLuaState) == 1,"Wrong number of parameters passed to lua script function call.")
	
	//if (XUserInterface::m_currentWorkingUI)
	//	XUserInterface::m_currentWorkingUI->setNativeResolution((XU32)lua_tonumber(pLuaState,1),
	//															(XU32)lua_tonumber(pLuaState,2));

	return 0; 
}



