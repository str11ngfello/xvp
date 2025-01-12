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
//	Module Name:	XLuaScript.h
//	Author:			Lynn Duke
//	Creation Date:	5-31-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_LUA_SCRIPT_H__
#define __X_LUA_SCRIPT_H__



//Lua Script States
enum X_LUA_SCRIPT_STATE
{
	X_LSS_WAITFRAME,
	X_LSS_WAITTIME,
	X_LSS_RUNNING,
	X_LSS_NOT_LOADED,
	X_LSS_FINISHED
};



//*****************************************************************************
//*****************************************************************************
class XLuaScript
{
private:                        
	lua_State*      threadState;
	char            lastErrorString[256];

	//*****************************************************************************
	//Private INT32erface
	//*****************************************************************************
	XStatus resumeScript(XF32 param);

public:                                    
    X_LUA_SCRIPT_STATE		m_state;              
    XF32					m_waitTimestamp;      
    XS32					m_waitFrame;          
    XF32					m_time;               

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XLuaScript();
	~XLuaScript();

	//*****************************************************************************
	//Public INT32erface
	//*****************************************************************************
	virtual XStatus destroy();
	XStatus update(XF32 dt);
	XStatus initFromFile(XS8* pFileName);
	XStatus initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize);
	XStatus run();
	//XS32 RunString(XS8* commandString);
	//void CallFn(XS8* fnName,XS32 iParam);
    void abort();
};



#endif 

