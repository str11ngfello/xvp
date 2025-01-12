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
//	Module Name:	XAudioMgrLuaGlue.h
//	Author:			Lynn Duke
//	Creation Date:	5-26-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_AUDIO_MGR_LUA_GLUE_H__
#define __X_AUDIO_MGR_LUA_GLUE_H__

#include <XOSMgr.h>
extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#ifdef X_CONSOLE
int LAPI_audio(lua_State* pLuaState);
#endif
int LAPI_createSoundFromFile(lua_State* pLuaState);
int LAPI_destroySound(lua_State* pLuaState);
int LAPI_destroyAllSounds(lua_State* pLuaState);
int LAPI_playSound(lua_State* pLuaState);
int LAPI_stopSound(lua_State* pLuaState);
int LAPI_stopAllSound(lua_State* pLuaState);
int LAPI_isSoundPlaying(lua_State* pLuaState);
int LAPI_setSoundVolume(lua_State* pLuaState);
int LAPI_getSoundVolume(lua_State* pLuaState);
int LAPI_setSoundPitch(lua_State* pLuaState);
int LAPI_getSoundPitch(lua_State* pLuaState);
int LAPI_setSoundLoopingState(lua_State* pLuaState);
int LAPI_getSoundLoopingState(lua_State* pLuaState);
int LAPI_setSoundRolloffFactor(lua_State* pLuaState);
int LAPI_getSoundRolloffFactor(lua_State* pLuaState);
int LAPI_setSoundPosition(lua_State* pLuaState);
int LAPI_getSoundPosition(lua_State* pLuaState);
int LAPI_setSoundVelocity(lua_State* pLuaState);
int LAPI_getSoundVelocity(lua_State* pLuaState);
int LAPI_setListenerTransform(lua_State* pLuaState);
int LAPI_getListenerTransform(lua_State* pLuaState);
int LAPI_setListenerPosition(lua_State* pLuaState);
int LAPI_getListenerPosition(lua_State* pLuaState);
int LAPI_setListenerVelocity(lua_State* pLuaState);
int LAPI_getListenerVelocity(lua_State* pLuaState);
int LAPI_setDopplerFactor(lua_State* pLuaState);
int LAPI_getDopplerFactor(lua_State* pLuaState);
int LAPI_setDopplerVelocity(lua_State* pLuaState);
int LAPI_getDopplerVelocity(lua_State* pLuaState);



#endif