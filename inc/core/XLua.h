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
//	Module Name:	XLua.h
//	Author:			Lynn Duke
//	Creation Date:	5-30-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_LUA_H__
#define __X_LUA_H__



#define X_MAX_LUA_ERR_STRING_SIZE	256

enum X_LUA_ARG_TYPE
	{
		X_LAT_BOOL,
		X_LAT_NUMBER,
		X_LAT_STRING
	};


struct XLuaArgument
{
	struct 
	{
		union
		{
			XF64 luaNumber;
			XBOOL luaBool;
			XS8* luaString;	
		};
	};
	X_LUA_ARG_TYPE type;
};


//*****************************************************************************
//*****************************************************************************
class XLua 
{
private:
	static lua_State*			m_pMasterLuaState;
	static XMemBlockList<XLuaScript*>	m_scripts;
	static XS8					m_lastError[X_MAX_LUA_ERR_STRING_SIZE];
	static XS32					m_numResults;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	static int LuaBeep(lua_State* pLuaState);
	static int waitFrames(lua_State* pLuaState);
	static int waitTime(lua_State* pLuaState);
	static int waitSeconds(lua_State* pLuaState);
	static XLuaScript* getScriptFromLua(lua_State* pLuaState);
	static XStatus pushFunctionArguments(XLuaArgument* pArguments,XU32 numArguments);


public:
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus init();
	static XStatus update(float dt);
	static XStatus destroy();
	static XStatus createScriptFromFile(XS8* pFilename,XHandle& scriptHandle);
	static XStatus createScriptFromMemory(XS8* pMemoryChunk,XU32 memChunkSize,XHandle& scriptHandle);
	static XStatus runScript(XHandle scriptHandle);
	static XStatus destroyScript(XHandle scriptHandle);
	static lua_State* getLuaState();
	static XStatus executeString(XS8* pString);
	static XStatus executeFunction(XS8* pFunctionName,XLuaArgument* pArguments,
									XU32 numArguments,XU32 numResults);
	static XStatus executeScript(XS8* pScriptFilename);
	static XU32 getXU32GlobalVar(XS8* pVarName);
	static XS32 getXS32GlobalVar(XS8* pVarName);
	static XF32 getXF32GlobalVar(XS8* pVarName);
	static void getStringGlobalVar(XS8* pVarName,XS8* pString,XU32 stringBufferSize);
	static XBOOL getXBOOLGlobalVar(XS8* pVarName);
	static XBOOL getBoolResult(XU32 resultIndex);
	static XF64 getNumberResult(XU32 resultIndex);
	static const XS8* getStringResult(XU32 resultIndex);
	static XS8* getLastErrorString();
	static XU32 getStackSize();
	static XStatus flushResults();
	static XStatus registerLibrary(XS8* pLibName,const luaL_reg* pLibFunctions);
};


#endif