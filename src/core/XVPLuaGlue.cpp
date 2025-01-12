
#include <xvpsdk.h>

static const XS8* XVPLuaLibDirectory[] = 
{
	NULL
};




int LAPI_xvp(lua_State* pLuaState)
{
	XS8** p = (XS8**)XVPLuaLibDirectory;
	while (*p)
	{
		X_CONSOLE(XConsole::setLastCommandResponseString(NULL,*p));
		++p;
	}
	return 0;
}



int LAPI_setMediaPath(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::setMediaPath((XS8*)lua_tostring(pLuaState,1));
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_getMediaPath(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XS8* p = XVP::getMediaPath();
	lua_pushstring(pLuaState,p);
	X_CONSOLE(XConsole::setLastCommandResponseString(NULL,p);)
	return 1;
}


int LAPI_getElapsedTime(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XF32 time = XVP::getFrameDt();
	lua_pushnumber(pLuaState,time);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,time);)
	return 1;
}

/*
int LAPI_getMatrixStackCapacity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XU32 capacity = XVP::getMatrixStackCapacity();
	lua_pushnumber(pLuaState,capacity);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,capacity);)
	return 1;
}*/

int LAPI_getFPS(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XU32 fps = XVP::getFPS(XVP::getFrameDt());
	lua_pushnumber(pLuaState,fps);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,fps);)
	return 1;
}


int LAPI_pause(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::pause();
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_resume(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::resume();
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

/*
int LAPI_pushPreUpdateState(XState* pState,void* pUserData = NULL);
int LAPI_pushPostUpdateState(XState* pState,void* pUserData = NULL);
int LAPI_pushPostMaterial(XState* pState,void* pUserData = NULL);
int LAPI_popPreUpdateState();
int LAPI_popPostUpdateState();
int LAPI_popPostMaterial();
int LAPI_getNumPreUpdateStates();
int LAPI_getNumPostUpdateStates();
int LAPI_getNumPostMaterials();
int LAPI_isPreUpdateStateRegistered(XState* pState);
int LAPI_isPostUpdateStateRegistered(XState* pState);
int LAPI_isPostMaterialRegistered(XState* pState);
*/
/*
int LAPI_getNumSprites(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XU32 num = XVP::getNumSprites();
	lua_pushnumber(pLuaState,num);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,num);)
	return 1;
}

int LAPI_enableSprites(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::enableSprites();
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_disableSprites(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::disableSprites();
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_flushScene(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::flushScene();
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_dumpScene(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::dumpScene((XS8*)lua_tostring(pLuaState,1));
												
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}
*/