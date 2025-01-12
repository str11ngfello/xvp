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
//	Module Name:	XLua.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-30-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


lua_State*			XLua::m_pMasterLuaState = NULL;
XMemBlockList<XLuaScript*>	XLua::m_scripts;
XS8					XLua::m_lastError[X_MAX_LUA_ERR_STRING_SIZE];
XS32				XLua::m_numResults = 0;


//*****************************************************************************
/**
Initializes the Lua subsystem.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XLua::init()
{
	//Create a Lua state.
	m_pMasterLuaState = lua_open();
	if (!m_pMasterLuaState)
		return X_ERR_OPERATION_FAILED;
		
	//Load in some Lua support.
	luaopen_base(m_pMasterLuaState);
	luaopen_table(m_pMasterLuaState);
	luaopen_string(m_pMasterLuaState);
	luaopen_math(m_pMasterLuaState);
#ifndef XVP_FINAL
	luaopen_debug(m_pMasterLuaState);
#endif

	//XScriptBindings::vBind(m_pMasterLuaState);

	//Open script lib
	const luaL_reg scriptLib[] =
	{
		{"Beep",		LuaBeep},
		{"waitFrames",	waitFrames},
		{"waitTime",    waitTime},
		{"waitSeconds", waitSeconds},
		{NULL, NULL}
	};
	luaL_openlib(m_pMasterLuaState,"Script",scriptLib,0);

	//Register XVP constants/globals/config
	XS8 path[X_MAX_PATH_SIZE];
	executeScript("XVPConfig.lua");
	
	//Set paths ASAP.  If we are in editor mode...grab some from the environment
#ifdef XVP_EDITOR
	XS8* pInstallPath = getenv("XVPSDK_DIR");
	X_ASSERTM(pInstallPath,"Could not find XVPSDK_DIR environment variable");
	X_ASSERTM(strlen(pInstallPath) < X_MAX_PATH_SIZE,"XVPSDK_DIR string lenth is too long")
#endif

#ifdef XVP_EDITOR
	sprintf(path,"%s/xvpsdk/system/",pInstallPath);
#else
	getStringGlobalVar("XVP_CONF_pSystemPath",path,X_MAX_PATH_SIZE);
#endif
	X_ASSERTF(XOSMgr::pathExists(path),"Failed to set XVP_CONF_pSystemPath because path does not exist - %s",path);
	XOSMgr::setPath(X_PATH_SYSTEM,path);

#ifdef XVP_EDITOR
	sprintf(path,"%s/xvpsdk/system/shaders/hlsl/",pInstallPath);
#else
	getStringGlobalVar("XVP_CONF_pShaderPath",path,X_MAX_PATH_SIZE);
#endif
	X_ASSERTF(XOSMgr::pathExists(path),"Failed to set XVP_CONF_pShaderPath because path does not exist - %s",path);
	XOSMgr::setPath(X_PATH_SHADER,path);

	getStringGlobalVar("XVP_CONF_pTexturePath",path,X_MAX_PATH_SIZE);
	X_ASSERTF(XOSMgr::pathExists(path),"Failed to set XVP_CONF_pTexturePath because path does not exist - %s",path);
	XOSMgr::setPath(X_PATH_TEXTURE,path);
	getStringGlobalVar("XVP_CONF_pMoviePath",path,X_MAX_PATH_SIZE);
	X_ASSERTF(XOSMgr::pathExists(path),"Failed to set XVP_CONF_pMoviePath because path does not exist - %s",path);
	XOSMgr::setPath(X_PATH_MOVIE,path);
	getStringGlobalVar("XVP_CONF_pAudioPath",path,X_MAX_PATH_SIZE);
	X_ASSERTF(XOSMgr::pathExists(path),"Failed to set XVP_CONF_pAudioPath because path does not exist - %s",path);
	XOSMgr::setPath(X_PATH_AUDIO,path);
	getStringGlobalVar("XVP_CONF_pUITemplatePath",path,X_MAX_PATH_SIZE);
	X_ASSERTF(XOSMgr::pathExists(path),"Failed to set XVP_CONF_pUITemplatePath because path does not exist - %s",path);
	XOSMgr::setPath(X_PATH_UI_TEMPLATE,path);
	XOSMgr::buildPath(X_PATH_SYSTEM,"lua/XConstants.lua",path);
	executeScript(path);

	m_numResults = 0;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLua::destroy()
{
	//Destroy all scripts
	XMemBlockList<XLuaScript*>::iterator i(m_scripts.begin());
	while (i.isValid())
	{
		i.getValue()->destroy();
		XDelete(i.getValue());
		i.getNext();
	}
	m_scripts.clear();

	return X_OK;
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XLua::update(float dt)
{
	//Update each script
	XMemBlockList<XLuaScript*>::iterator i(m_scripts.begin());
	while (i.isValid())
	{
		i.getValue()->update(dt);
		i.getNext();
	}
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLua::createScriptFromFile(XS8* pFilename,XHandle& scriptHandle)
{
	//Make sure we have a valid filename.
	if (!pFilename)
	{
		scriptHandle = X_INVALID_HANDLE;
		return X_ERR_INVALID_PARAMS;
	}

	XLuaScript* pScript = NULL;
	XNew(pScript,XLuaScript,XOSMgr::getDefaultHeap());
	XStatus status = pScript->initFromFile(pFilename);
	if (status == X_OK)
	{
		//Add the resource to the script list
		scriptHandle = (XHandle)pScript;
		m_scripts.insertBack(pScript);
		return X_OK;
	}
	else
	{
		XDelete(pScript);
		return status;
	}
}

XStatus XLua::createScriptFromMemory(XS8* pMemoryChunk,XU32 memChunkSize,XHandle& scriptHandle)
{
	X_ASSERT(pMemoryChunk);

	XLuaScript* pScript = NULL;
	XNew(pScript,XLuaScript,XOSMgr::getDefaultHeap());
	XStatus status = pScript->initFromMemory(pMemoryChunk,memChunkSize);
	if (status == X_OK)
	{
		//Add the resource to the script list
		scriptHandle = (XHandle)pScript;
		m_scripts.insertBack(pScript);
		return X_OK;
	}
	else
	{
		XDelete(pScript);
		return status;
	}
}


//*****************************************************************************
//*****************************************************************************
XStatus XLua::destroyScript(XHandle scriptHandle)
{
	if (scriptHandle == X_INVALID_HANDLE)
		return X_ERR_INVALID_PARAMS;
	else
	{
		XBOOL found = false;
		XMemBlockList<XLuaScript*>::iterator i(m_scripts.begin());
		while (i.isValid())
		{
			if (i.getValue() == (XLuaScript*)(scriptHandle))
			{
				i.getValue()->destroy();
				XDelete(i.getValue());
				found = true;
				break;
			}
			i.getNext();
		}

		//If we deleted the script, clear the null pointer
		if (found)
		{
			m_scripts.remove(NULL);
			return X_OK;
		}
		else
			return X_ERR_NO_SUCH_OBJECT;
	}
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XLua::runScript(XHandle scriptHandle)
{
	//Find the script and run it.
	XMemBlockList<XLuaScript*>::iterator i(m_scripts.begin());
	while (i.isValid())
	{
		if (i.getValue() == (XLuaScript*)(scriptHandle))
		{
			i.getValue()->run();
			return X_OK;
		}
		i.getNext();
	}
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Gets the master Lua state.

@return A pointer to the master lua state.
*/
//*****************************************************************************
lua_State* XLua::getLuaState()
{
	return m_pMasterLuaState;
}


//*****************************************************************************
/**
Executes a command string inside of X-VP's internal Lua state.

@param pString	A NULL terminated string representing the command to execute.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XLua::executeString(XS8* pString)
{
	//First, pop off any return values from the previous call
	flushResults();

	if (pString)
	{
		if (luaL_loadbuffer(m_pMasterLuaState,pString,strlen(pString),NULL) != 0)
		{
			sprintf(m_lastError, "ERROR: %s\n",luaL_checkstring(m_pMasterLuaState,-1));
			lua_pop(m_pMasterLuaState,1);
			return X_ERR_OPERATION_FAILED;
		}

		if (lua_pcall(m_pMasterLuaState,0,0,0) != 0)
		{
			sprintf(m_lastError, "ERROR: %s\n",luaL_checkstring(m_pMasterLuaState,-1));
			lua_pop(m_pMasterLuaState,1);
			return X_ERR_OPERATION_FAILED;
		}
		
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLua::executeFunction(XS8* pFunctionName,XLuaArgument* pArguments,
							  XU32 numArguments,XU32 numResults)
{
	//First, pop off any return values from the previous call
	flushResults();

	if (pFunctionName)
	{
		lua_getglobal(XLua::getLuaState(),pFunctionName);
		
		//If there are arguments, push them onto the stack next.
		if (numArguments)
			pushFunctionArguments(pArguments,numArguments);

		if (lua_pcall(XLua::getLuaState(),numArguments,numResults,0) != 0)
		{
			sprintf(m_lastError, "ERROR: %s\n",luaL_checkstring(m_pMasterLuaState,-1));
			lua_pop(m_pMasterLuaState,1);
			X_ASSERT_FORCED(m_lastError);
			return X_ERR_OPERATION_FAILED;
		}
		
		m_numResults = numResults;

		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLua::executeScript(XS8* pScriptFilename)
{
	X_ASSERT(pScriptFilename);
	X_ASSERTF(XOSMgr::fileExists(pScriptFilename),"Could not find script %s",pScriptFilename)

	//First, pop off any return values from the previous call
	flushResults();
	
	if (luaL_loadfile(m_pMasterLuaState,pScriptFilename) != 0)
	{
		sprintf(m_lastError, "Lua Error - Script Load\nScript Name:%s\nError Message:%s\n",
			    pScriptFilename,luaL_checkstring(m_pMasterLuaState,-1));
		lua_pop(m_pMasterLuaState,1);
		X_ASSERT_FORCED(m_lastError);
		return X_ERR_OPERATION_FAILED;
	}

	if (lua_pcall(m_pMasterLuaState,0,0,0) != 0)
	{
		sprintf(m_lastError, "Lua Error - Script Run\nScript Name:%s\nError Message:%s\n",
				pScriptFilename,luaL_checkstring(m_pMasterLuaState,-1));
		lua_pop(m_pMasterLuaState,1);
		X_ASSERT_FORCED(m_lastError);
		return X_ERR_OPERATION_FAILED;
	}

	return X_OK;
}



XU32 XLua::getXU32GlobalVar(XS8* pVarName)
{
	lua_getglobal(m_pMasterLuaState,pVarName);     
    X_ASSERT(lua_isnumber(m_pMasterLuaState, -1));
	XU32 var = (XU32)lua_tonumber(m_pMasterLuaState, -1);
	lua_pop(m_pMasterLuaState,1);
	return var; 
}

XS32 XLua::getXS32GlobalVar(XS8* pVarName)
{
	lua_getglobal(m_pMasterLuaState,pVarName);     
    X_ASSERT(lua_isnumber(m_pMasterLuaState, -1));
	XS32 var = (XS32)lua_tonumber(m_pMasterLuaState, -1);
	lua_pop(m_pMasterLuaState,1);
	return var; 
}


XF32 XLua::getXF32GlobalVar(XS8* pVarName)
{
	lua_getglobal(m_pMasterLuaState,pVarName);     
    X_ASSERT(lua_isnumber(m_pMasterLuaState, -1));
	XF32 var = (XF32)lua_tonumber(m_pMasterLuaState, -1);
	lua_pop(m_pMasterLuaState,1);
	return var; 
}


void XLua::getStringGlobalVar(XS8* pVarName,XS8* pString,XU32 stringBufferSize)
{
	lua_getglobal(m_pMasterLuaState,pVarName);     
    X_ASSERT(lua_isstring(m_pMasterLuaState, -1));
	const XS8* pLuaString = lua_tostring(m_pMasterLuaState, -1);
	X_ASSERTM(strlen(pLuaString) < stringBufferSize,"The lua string requires a larger buffer to copy into");
	strcpy(pString,pLuaString);
	lua_pop(m_pMasterLuaState,1);
}


XBOOL XLua::getXBOOLGlobalVar(XS8* pVarName)
{
	lua_getglobal(m_pMasterLuaState,pVarName);     
    X_ASSERT(lua_isboolean(m_pMasterLuaState, -1));
	XBOOL var = lua_toboolean(m_pMasterLuaState, -1) == 0 ? false:true;
	lua_pop(m_pMasterLuaState,1);
	return var; 
}


//*****************************************************************************
//*****************************************************************************
XS8* XLua::getLastErrorString()
{
	return m_lastError;
}

	

//*****************************************************************************
//*****************************************************************************
XU32 XLua::getStackSize()
{
	return lua_gettop(m_pMasterLuaState);
}



//*****************************************************************************
//*****************************************************************************
XStatus XLua::registerLibrary(XS8* pLibName,const luaL_reg* pLibFunctions)
{
	if ((!pLibName) || (!pLibFunctions))
		return X_ERR_INVALID_PARAMS;
	else if (!m_pMasterLuaState)
		return X_ERR_NOT_INITIALIZED;
	else
	{
		luaL_openlib(m_pMasterLuaState,pLibName,pLibFunctions,0);
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
int XLua::waitFrames(lua_State* pLuaState)
{
	XLuaScript* pScript = getScriptFromLua(pLuaState);
	pScript->m_waitFrame = (XS32)luaL_checknumber(pLuaState,1);
	pScript->m_state = X_LSS_WAITFRAME;

	return (lua_yield(pLuaState, 1));
}



//*****************************************************************************
//*****************************************************************************
int XLua::waitTime(lua_State* pLuaState)
{
	XLuaScript* pScript = getScriptFromLua(pLuaState);	
	pScript->m_waitTimestamp = (XF32)luaL_checknumber(pLuaState,1);
	pScript->m_state = X_LSS_WAITTIME;
	
	return (lua_yield(pLuaState, 1));
}



//*****************************************************************************
//*****************************************************************************
int XLua::waitSeconds(lua_State* pLuaState)
{
	XLuaScript* pScript = getScriptFromLua(pLuaState);
	pScript->m_waitTimestamp = pScript->m_time + (XF32)luaL_checknumber(pLuaState,1);
	pScript->m_state = X_LSS_WAITTIME;

	return (lua_yield(pLuaState, 1));
}



//*****************************************************************************
//*****************************************************************************
XLuaScript* XLua::getScriptFromLua(lua_State* pLuaState)
{
	lua_pushlightuserdata(pLuaState,pLuaState);
	lua_gettable(pLuaState,LUA_GLOBALSINDEX);

	return ((XLuaScript*)lua_touserdata(pLuaState,-1));
}


 int XLua::LuaBeep(lua_State* pLuaState)
    {
    Beep((int)luaL_checknumber(pLuaState,1),(XS32)luaL_checknumber(pLuaState,2));
    return(0);
    }




//*****************************************************************************
//*****************************************************************************
XStatus XLua::flushResults()
{
	if (m_numResults != 0)
	{
		lua_pop(m_pMasterLuaState,m_numResults);
		m_numResults = 0;
	}
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XLua::getBoolResult(XU32 resultIndex)
{
	if (m_numResults != 0)
		return lua_toboolean(m_pMasterLuaState,(XS32)resultIndex - (XS32)m_numResults) == 0 ? false:true; 
	else
		return false;
}






//*****************************************************************************
//*****************************************************************************
double XLua::getNumberResult(XU32 resultIndex)
{
	if (m_numResults != 0)
		return lua_tonumber(m_pMasterLuaState,(XS32)resultIndex - (XS32)m_numResults); 
	else
		return 0.0;
}






//*****************************************************************************
//*****************************************************************************
const XS8* XLua::getStringResult(XU32 resultIndex)
{
	if (m_numResults != 0)
		return lua_tostring(m_pMasterLuaState,(XS32)resultIndex - (XS32)m_numResults); 
	else
		return NULL;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLua::pushFunctionArguments(XLuaArgument* pArguments,XU32 numArguments)
{
	if (pArguments)
	{
		for (XU32 i = 0;i < numArguments;++i)
		{
			switch (pArguments[i].type)
			{
			case X_LAT_BOOL:
				lua_pushboolean(m_pMasterLuaState,pArguments[i].luaBool);
				break;
			case X_LAT_NUMBER:
				lua_pushnumber(m_pMasterLuaState,pArguments[i].luaNumber);
				break;
			case X_LAT_STRING:
				lua_pushstring(m_pMasterLuaState,pArguments[i].luaString);
				break;
			default:
				break;
			}
		}
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}


//*****************************************************************************
//*****************************************************************************
