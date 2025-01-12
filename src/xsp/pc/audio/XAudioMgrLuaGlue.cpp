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
//	Module Name:	XAudioMgrLuaGlue.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-26-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


#ifdef X_CONSOLE
static const XS8* XAudioLuaLibDirectory[] = 
{
	"XStatus createSoundFromFile(XS8* pFilename,XHandle& soundHandle)",
	"XStatus destroySound(XHandle soundHandle)",
	"XStatus destroyAllSounds()",
	"XStatus playSound(XHandle soundHandle,XU8 priority = 255)",
	"XStatus stopSound(XHandle soundHandle)",
	"XStatus stopAllSound()",
	"XBOOL isSoundPlaying(XHandle soundHandle)",
	"XStatus setSoundVolume(XHandle soundHandle,XF32 volume)",
	"XF32 getSoundVolume(XHandle soundHandle)",
	"XStatus setSoundPitch(XHandle soundHandle,XF32 pitch)",
	"XF32 getSoundPitch(XHandle soundHandle)",
	"XStatus setSoundLoopingState(XHandle soundHandle,XBOOL enableLooping)",
	"XBOOL getSoundLoopingState(XHandle soundHandle)",
	"XStatus setSoundRolloffFactor(XHandle soundHandle,XF32 rolloffFactor)",
	"XF32 getSoundRolloffFactor(XHandle soundHandle)",
	"XStatus setSoundPosition(XHandle soundHandle,XF32 x,XF32 y,XF32 z)",
	"x y z = getSoundPosition(XHandle soundHandle)",
	"XStatus setSoundVelocity(XHandle soundHandle,XF32 x,XF32 y,XF32 z)",
	"x y z = getSoundVelocity(XHandle soundHandle)",
	"XStatus setListenerTransform(XF32 lookX,XF32 lookY,XF32 lookZ,XF32 upX,XF32 upY,XF32 upZ)",
	"lookX lookY lookZ upX upY upZ = getListenerTransform(XVector3D& lookVector,XVector3D& upVector)",
	"XStatus setListenerPosition(XF32 x,XF32 y,XF32 z)",
	"x y z = getListenerPosition()",
	"XStatus setListenerVelocity(XF32 x,XF32 y,XF32 z)",
	"x y z = getListenerVelocity()",
	"XStatus setDopplerFactor(XF32 dopplerFactor)",
	"XF32 getDopplerFactor()",
	"XStatus setDopplerVelocity(XF32 dopplerVelocity)",
	"XF32 getDopplerVelocity()",
	NULL
};




int LAPI_audio(lua_State* pLuaState)
{
	XS8** p = (XS8**)XAudioLuaLibDirectory;
	while (*p)
	{
		X_CONSOLE(XConsole::setLastCommandResponseString(NULL,*p));
		++p;
	}
	return 0;
}
#endif


int LAPI_createSoundFromFile(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XHandle soundHandle;
	XStatus status = XVP::AudioMgr.createSoundFromFile((XS8*)lua_tostring(pLuaState,1),soundHandle);
	lua_pushnumber(pLuaState,(lua_Number)soundHandle);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)

	return 1;
}

int LAPI_destroySound(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.destroySound((XHandle)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)

	return 1;
}

int LAPI_destroyAllSounds(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.destroyAllSounds();
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_playSound(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.playSound((XHandle)lua_tonumber(pLuaState,1),
											(XU8)lua_tonumber(pLuaState,2));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_stopSound(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.stopSound((XHandle)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_stopAllSound(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.stopAllSound();
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_isSoundPlaying(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XBOOL isPlaying = XVP::AudioMgr.isSoundPlaying((XHandle)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,isPlaying);
	X_CONSOLE(XConsole::setLastCommandResponseBool(isPlaying);)
	return 1;
}

int LAPI_setSoundVolume(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.setSoundVolume((XHandle)lua_tonumber(pLuaState,1),
												(XF32)lua_tonumber(pLuaState,2));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getSoundVolume(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XF32 volume = XVP::AudioMgr.getSoundVolume((XHandle)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,volume);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,volume);)
	return 1;
}



int LAPI_setSoundPitch(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setSoundPitch((XHandle)lua_tonumber(pLuaState,1),
							(XF32)lua_tonumber(pLuaState,2));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getSoundPitch(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XF32 pitch = XVP::AudioMgr.getSoundPitch((XHandle)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,pitch);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,pitch);)
	return 1;
}


int LAPI_setSoundLoopingState(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setSoundLoopingState((XHandle)lua_tonumber(pLuaState,1),
													    (lua_toboolean(pLuaState,2)==1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getSoundLoopingState(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XBOOL loop = XVP::AudioMgr.getSoundLoopingState((XHandle)lua_tonumber(pLuaState,1));
	lua_pushboolean(pLuaState,loop);
	X_CONSOLE(XConsole::setLastCommandResponseBool(loop);)
	return 1;
}


int LAPI_setSoundRolloffFactor(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setSoundRolloffFactor((XHandle)lua_tonumber(pLuaState,1),
													(XF32)lua_tonumber(pLuaState,2));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getSoundRolloffFactor(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XF32 rolloff = XVP::AudioMgr.getSoundRolloffFactor((XHandle)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,rolloff);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,rolloff);)
	return 1;
}


int LAPI_setSoundPosition(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 4)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setSoundPosition((XHandle)lua_tonumber(pLuaState,1),
												XVector3D((XF32)lua_tonumber(pLuaState,2),
														  (XF32)lua_tonumber(pLuaState,3),
														  (XF32)lua_tonumber(pLuaState,4)));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}



int LAPI_getSoundPosition(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XVector3D pos;
	XVP::AudioMgr.getSoundPosition((XHandle)lua_tonumber(pLuaState,1),pos);

	lua_pushnumber(pLuaState,pos.x);
	lua_pushnumber(pLuaState,pos.y);
	lua_pushnumber(pLuaState,pos.z);
	X_CONSOLE(XConsole::setLastCommandResponse3DVec(NULL,pos);)

	return 3;
}



int LAPI_setSoundVelocity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 4)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setSoundVelocity((XHandle)lua_tonumber(pLuaState,1),
												XVector3D((XF32)lua_tonumber(pLuaState,2),
														  (XF32)lua_tonumber(pLuaState,3),
														  (XF32)lua_tonumber(pLuaState,4)));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getSoundVelocity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XVector3D vel;
	XVP::AudioMgr.getSoundVelocity((XHandle)lua_tonumber(pLuaState,1),vel);

	lua_pushnumber(pLuaState,vel.x);
	lua_pushnumber(pLuaState,vel.y);
	lua_pushnumber(pLuaState,vel.z);
	X_CONSOLE(XConsole::setLastCommandResponse3DVec(NULL,vel);)

	return 3;
}



int LAPI_setListenerTransform(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 6)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setListenerTransform(XVector3D((XF32)lua_tonumber(pLuaState,1),
														  (XF32)lua_tonumber(pLuaState,2),
														  (XF32)lua_tonumber(pLuaState,3)),
												XVector3D((XF32)lua_tonumber(pLuaState,4),
														  (XF32)lua_tonumber(pLuaState,5),
														  (XF32)lua_tonumber(pLuaState,6)));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getListenerTransform(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XVector3D lookVector,upVector;
	XVP::AudioMgr.getListenerTransform(lookVector,upVector);

	lua_pushnumber(pLuaState,lookVector.x);
	lua_pushnumber(pLuaState,lookVector.y);
	lua_pushnumber(pLuaState,lookVector.z);
	lua_pushnumber(pLuaState,upVector.x);
	lua_pushnumber(pLuaState,upVector.y);
	lua_pushnumber(pLuaState,upVector.z);
	X_CONSOLE(XConsole::setLastCommandResponse3DVec("look vector -> ",lookVector);)
	X_CONSOLE(XConsole::setLastCommandResponse3DVec("up vector -> ",upVector);)

	return 6;
}


int LAPI_setListenerPosition(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 3)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setListenerPosition(XVector3D((XF32)lua_tonumber(pLuaState,1),
														  (XF32)lua_tonumber(pLuaState,2),
														  (XF32)lua_tonumber(pLuaState,3)));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getListenerPosition(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XVector3D pos;
	XVP::AudioMgr.getListenerPosition(pos);

	lua_pushnumber(pLuaState,pos.x);
	lua_pushnumber(pLuaState,pos.y);
	lua_pushnumber(pLuaState,pos.z);
	X_CONSOLE(XConsole::setLastCommandResponse3DVec(NULL,pos);)

	return 3;
}


int LAPI_setListenerVelocity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 3)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setListenerVelocity(XVector3D((XF32)lua_tonumber(pLuaState,1),
														  (XF32)lua_tonumber(pLuaState,2),
														  (XF32)lua_tonumber(pLuaState,3)));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getListenerVelocity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XVector3D vel;
	XVP::AudioMgr.getListenerVelocity(vel);

	lua_pushnumber(pLuaState,vel.x);
	lua_pushnumber(pLuaState,vel.y);
	lua_pushnumber(pLuaState,vel.z);
	X_CONSOLE(XConsole::setLastCommandResponse3DVec(NULL,vel);)

	return 3;
}


int LAPI_setDopplerFactor(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::AudioMgr.setDopplerFactor((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getDopplerFactor(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XF32 doppler = XVP::AudioMgr.getDopplerFactor();
	lua_pushnumber(pLuaState,doppler);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,doppler);)
	return 1;
}


int LAPI_setDopplerVelocity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::AudioMgr.setDopplerVelocity((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getDopplerVelocity(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XF32 dopplerVel = XVP::AudioMgr.getDopplerVelocity();
	lua_pushnumber(pLuaState,dopplerVel);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,dopplerVel);)
	return 1;
}

