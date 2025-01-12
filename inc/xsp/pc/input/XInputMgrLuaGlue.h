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
//	Module Name:	XInputMgrLuaGlue.h
//	Author:			Lynn Duke
//	Creation Date:	5-29-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_INPUT_MGR_LUA_GLUE_H__
#define __X_INPUT_MGR_LUA_GLUE_H__

#include <XOSMgr.h>
extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

int LAPI_input(lua_State* pLuaState);
int LAPI_setKeyRepeatTimeout(lua_State* pLuaState);
int LAPI_getKeyRepeatTimeout(lua_State* pLuaState);
int LAPI_getNumJoysticks(lua_State* pLuaState);
int LAPI_getJoyXExtent(lua_State* pLuaState);
int LAPI_getJoyYExtent(lua_State* pLuaState);
int LAPI_getJoyZExtent(lua_State* pLuaState);
int LAPI_getJoyZRollExtent(lua_State* pLuaState);
int LAPI_joyButtonDown(lua_State* pLuaState);
int LAPI_joyButtonDownOnce(lua_State* pLuaState);
int LAPI_joyButtonUpOnce(lua_State* pLuaState);
int LAPI_anyJoyButtonInput(lua_State* pLuaState);
int LAPI_mouseButtonDown(lua_State* pLuaState);
int LAPI_mouseButtonDownOnce(lua_State* pLuaState);
int LAPI_mouseButtonUpOnce(lua_State* pLuaState);
int LAPI_getMouseRelXMovement(lua_State* pLuaState);
int LAPI_getMouseRelYMovement(lua_State* pLuaState);
int LAPI_getMouseRelZMovement(lua_State* pLuaState);
int LAPI_anyMouseButtonInput(lua_State* pLuaState);
int LAPI_keyDown(lua_State* pLuaState);
int LAPI_keyDownOnce(lua_State* pLuaState);
int LAPI_keyDownOnceRepeat(lua_State* pLuaState);
int LAPI_keyUpOnce(lua_State* pLuaState);
int LAPI_anyKeyboardInput(lua_State* pLuaState);


#endif