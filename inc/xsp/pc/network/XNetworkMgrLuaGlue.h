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
//	Module Name:	XNetworkMgrLuaGlue.h
//	Author:			Lynn Duke
//	Creation Date:	5-29-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NETWORK_MGR_LUA_GLUE_H__
#define __X_NETWORK_MGR_LUA_GLUE_H__

#include <XOSMgr.h>
extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#ifdef X_CONSOLE
int LAPI_network(lua_State* pLuaState);
#endif






#endif