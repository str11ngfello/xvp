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
//	Module Name:	XLuaScript.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-31-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XLuaScript::XLuaScript()
{
	m_state = X_LSS_NOT_LOADED;
	m_time = 0;

	//Create a brand new thread for this Lua script to use.
	threadState = lua_newthread(XLua::getLuaState());
   
	//Store this object into Lua's global table using the thread as the key.
	lua_State* pLuaState = XLua::getLuaState();
	lua_pushlightuserdata(pLuaState,threadState);
	lua_pushlightuserdata(pLuaState,this);
	lua_settable(pLuaState,LUA_GLOBALSINDEX);
}



//*****************************************************************************
//*****************************************************************************
XLuaScript::~XLuaScript()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XLuaScript::initFromFile(XS8* pFileName)
{
	XS32 status = luaL_loadfile(threadState,pFileName);
    if (status == 0)
		return X_OK;
    else 
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLuaScript::initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize)
{
	XS32 status = luaL_loadbuffer(threadState,pMemoryChunk,memChunkSize,NULL);
    if (status == 0)
		return X_OK;
    else 
		return X_ERR_OPERATION_FAILED;
}

//*****************************************************************************
//*****************************************************************************
XStatus XLuaScript::destroy()
{
	//Give the script a change to clean up
	abort();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLuaScript::run()
{
	return resumeScript(0.0f);
}



//*****************************************************************************
//*****************************************************************************
XStatus XLuaScript::update(XF32 dt)
{
	m_time += dt;

	switch(m_state)
	{
	case X_LSS_WAITTIME:
		if (m_time >= m_waitTimestamp)
			resumeScript(0.0f);
		break;
	case X_LSS_WAITFRAME:
		--m_waitFrame;
		if (m_waitFrame <= 0)
			resumeScript(0.0f);
		break;
	case X_LSS_NOT_LOADED:
		break;
	default:
		break;
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XLuaScript::abort()
{
	resumeScript(1.0f);
}



//*****************************************************************************
//*****************************************************************************
XStatus XLuaScript::resumeScript(XF32 param)
{
    //Change the m_state to "running"
    m_state = X_LSS_RUNNING;

    //Push parameter back to the yielding function
    lua_pushnumber(threadState, param);

	//Resume the script
    XS32 status = lua_resume(threadState,1);

    if (status)
		return X_ERR_OPERATION_FAILED;
	else
		return X_OK;
}

