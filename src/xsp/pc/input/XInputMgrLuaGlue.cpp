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
//	Module Name:	XInputMgrLuaGlue.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-29-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


static const XS8* XInputLuaLibDirectory[] = 
{
	"XStatus setKeyRepeatTimeout(XF32 seconds)",
	"XF32 getKeyRepeatTimeout()",
	"XU32 getNumJoysticks()",
	"XF32 getJoyXExtent(XU32 jsIndex)",
	"XF32 getJoyYExtent(XU32 jsIndex)",
	"XF32 getJoyZExtent(XU32 jsIndex)",
	"XF32 getJoyZRollExtent(XU32 jsIndex)",
	"XBOOL joyButtonDown(XU32 jsIndex,X_JOY_BUTTON button)",
	"XBOOL joyButtonDownOnce(XU32 jsIndex,X_JOY_BUTTON button)",
	"XBOOL joyButtonUpOnce(XU32 jsIndex,X_JOY_BUTTON button)",
	"XBOOL anyJoyButtonInput(XU32 jsIndex)",
	"XBOOL mouseButtonDown(X_MOUSE_BUTTON button)",
	"XBOOL mouseButtonDownOnce(X_MOUSE_BUTTON button)",
	"XBOOL mouseButtonUpOnce(X_MOUSE_BUTTON button)",
	"XS32 getMouseRelXMovement()",
	"XS32 getMouseRelYMovement()",
	"XS32 getMouseRelZMovement()",
	"XBOOL anyMouseButtonInput()",
	"XBOOL keyDown(X_KEY key)",
	"XBOOL keyDownOnce(X_KEY key)",
	"XBOOL keyDownOnceRepeat(X_KEY key)",
	"XBOOL keyUpOnce(X_KEY key)",
	"XBOOL anyKeyboardInput()",
	NULL
};

int LAPI_input(lua_State* pLuaState)
{
	XS8** p = (XS8**)XInputLuaLibDirectory;
	while (*p)
	{
		X_CONSOLE(XConsole::setLastCommandResponseString(NULL,*p));
		++p;
	}
	return 0;
}

int LAPI_setKeyRepeatTimeout(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::InputMgr.setKeyRepeatTimeout((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getKeyRepeatTimeout(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XF32 timeout = XVP::InputMgr.getKeyRepeatTimeout();
	lua_pushnumber(pLuaState,timeout);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,timeout);)
	return 1;
}


int LAPI_getNumJoysticks(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XU32 numJoysticks = XVP::InputMgr.getNumJoysticks();
	lua_pushnumber(pLuaState,numJoysticks);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,numJoysticks);)
	return 1;
}

int LAPI_getJoyXExtent(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XF32 extent = XVP::InputMgr.getJoyXExtent((XU32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,extent);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,extent);)
	return 1;
}


int LAPI_getJoyYExtent(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XF32 extent = XVP::InputMgr.getJoyYExtent((XU32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,extent);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,extent);)
	return 1;
}

int LAPI_getJoyZExtent(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XF32 extent = XVP::InputMgr.getJoyZExtent((XU32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,extent);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,extent);)
	return 1;
}


int LAPI_getJoyZRollExtent(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XF32 extent = XVP::InputMgr.getJoyZRollExtent((XU32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,extent);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,extent);)
	return 1;
}


int LAPI_joyButtonDown(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.joyButtonDown((XU32)lua_tonumber(pLuaState,1),(X_JOY_BUTTON)(XU32)lua_tonumber(pLuaState,2));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_joyButtonDownOnce(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.joyButtonDownOnce((XU32)lua_tonumber(pLuaState,1),(X_JOY_BUTTON)(XU32)lua_tonumber(pLuaState,2));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_joyButtonUpOnce(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.joyButtonUpOnce((XU32)lua_tonumber(pLuaState,1),(X_JOY_BUTTON)(XU32)lua_tonumber(pLuaState,2));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_anyJoyButtonInput(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.anyJoyButtonInput((XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_mouseButtonDown(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.mouseButtonDown((X_MOUSE_BUTTON)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_mouseButtonDownOnce(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.mouseButtonDownOnce((X_MOUSE_BUTTON)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_mouseButtonUpOnce(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.mouseButtonUpOnce((X_MOUSE_BUTTON)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_getMouseRelXMovement(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XS32 movement = XVP::InputMgr.getMouseRelXMovement();
	lua_pushnumber(pLuaState,movement);
	X_CONSOLE(XConsole::setLastCommandResponseInt32(NULL,movement);)
	return 1;
}

int LAPI_getMouseRelYMovement(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XS32 movement = XVP::InputMgr.getMouseRelYMovement();
	lua_pushnumber(pLuaState,movement);
	X_CONSOLE(XConsole::setLastCommandResponseInt32(NULL,movement);)
	return 1;
}

int LAPI_getMouseRelZMovement(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XS32 movement = XVP::InputMgr.getMouseRelZMovement();
	lua_pushnumber(pLuaState,movement);
	X_CONSOLE(XConsole::setLastCommandResponseInt32(NULL,movement);)
	return 1;
}


int LAPI_anyMouseButtonInput(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.anyMouseButtonInput();
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_keyDown(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.keyDown((X_KEY)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_keyDownOnce(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.keyDownOnce((X_KEY)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_keyDownOnceRepeat(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.keyDownOnceRepeat((X_KEY)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}

int LAPI_keyUpOnce(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.keyUpOnce((X_KEY)(XU32)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}


int LAPI_anyKeyboardInput(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	
	XBOOL down = XVP::InputMgr.anyKeyboardInput();
	lua_pushboolean(pLuaState,down);
	X_CONSOLE(XConsole::setLastCommandResponseBool(down);)
	return 1;
}
