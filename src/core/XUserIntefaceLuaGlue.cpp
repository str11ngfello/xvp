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
//	Module Name:	XUserIntefaceLuaGlue.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-27-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


int LAPI_UISetNativeResolution(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 2)
		return 0;

	if (XUserInterface::m_currentWorkingUI)
		XUserInterface::m_currentWorkingUI->setNativeResolution((XU32)lua_tonumber(pLuaState,1),
																(XU32)lua_tonumber(pLuaState,2));

	return 0; 
}



int LAPI_UIRegisterCreationHandler(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
		return 0;

	if (XUserInterface::m_currentWorkingUI)
		XUserInterface::m_currentWorkingUI->setCreationHandler((XS8*)lua_tostring(pLuaState,1));

	return 0;
}


int LAPI_UIRegisterDestructionHandler(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
		return 0;

	if (XUserInterface::m_currentWorkingUI)
		XUserInterface::m_currentWorkingUI->setDestructionHandler((XS8*)lua_tostring(pLuaState,1));

	return 0;
}


int LAPI_UIRegisterEventHandler(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
		return 0;

	if (XUserInterface::m_currentWorkingUI)
		XUserInterface::m_currentWorkingUI->setEventHandler((XS8*)lua_tostring(pLuaState,1));

	return 0;
}


int LAPI_UIRegisterUpdateHandler(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
		return 0;

	if (XUserInterface::m_currentWorkingUI)
		XUserInterface::m_currentWorkingUI->setUpdateHandler((XS8*)lua_tostring(pLuaState,1));

	return 0;
}




int LAPI_UIEnableScaling(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->enableScaling(lua_toboolean(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsScalingEnabled(lua_State* pLuaState)
{
	XBOOL enabled = false;
	if (XUserInterface::m_currentWorkingUI)
	{
		enabled = XUserInterface::m_currentWorkingUI->isScalingEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UIEnableToolTips(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->enableToolTips((XS8*)lua_tostring(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIDisableToolTips(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 0)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->disableToolTips();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsToolTipsEnabled(lua_State* pLuaState)
{
	XBOOL enabled = false;
	if (XUserInterface::m_currentWorkingUI)
	{
		enabled = XUserInterface::m_currentWorkingUI->isToolTipsEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UIGetXScale(lua_State* pLuaState)
{
	XF32 xScale = 0.0f;
	if (XUserInterface::m_currentWorkingUI)
	{
		xScale = XUserInterface::m_currentWorkingUI->getXScale();
	}

	lua_pushnumber(pLuaState,xScale);
	return 1;
}
int LAPI_UIGetYScale(lua_State* pLuaState)
{
	XF32 yScale = 0.0f;
	if (XUserInterface::m_currentWorkingUI)
	{
		yScale = XUserInterface::m_currentWorkingUI->getYScale();
	}

	lua_pushnumber(pLuaState,yScale);
	return 1;
}
int LAPI_UISetToolTipTimeout(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->setToolTipTimeout((XF32)lua_tonumber(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetToolTipTimeout(lua_State* pLuaState)
{
	XF32 timeOut = 0.0f;
	if (XUserInterface::m_currentWorkingUI)
	{
		timeOut = XUserInterface::m_currentWorkingUI->getToolTipTimeout();
	}

	lua_pushnumber(pLuaState,timeOut);
	return 1;
}

int LAPI_UISetOpacity(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->setOpacity((XF32)lua_tonumber(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UICreateCursor(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		status = XUserInterface::createCursor((XS8*)lua_tostring(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIDestroyCursor(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 0)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		status = XUserInterface::destroyCursor();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}


int LAPI_UIGetCursorHotSpot(lua_State* pLuaState)
{
	XVector2D vec;
	XStatus status  = X_OK;
	if (lua_gettop(pLuaState) != 0)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		vec = XUserInterface::m_currentWorkingUI->getCursor()->getHotSpot();
	}

	lua_pushnumber(pLuaState,vec.x);
	lua_pushnumber(pLuaState,vec.y);
	return 2;
}


int LAPI_UIGetCursorRelXMov(lua_State* pLuaState)
{
	XS32 value;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 0)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		value = XUserInterface::m_currentWorkingUI->getCursor()->getRelXMov();
	}

	lua_pushnumber(pLuaState,value);
	return 1;
}
int LAPI_UIGetCursorRelYMov(lua_State* pLuaState)
{
	XS32 value;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 0)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		value = XUserInterface::m_currentWorkingUI->getCursor()->getRelYMov();
	}

	lua_pushnumber(pLuaState,value);
	return 1;
}
int LAPI_UISetCursorState(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		status = XUserInterface::getCursor()->setState((X_POINTER_STATE)(XU64)lua_tonumber(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetCursorState(lua_State* pLuaState)
{
	X_POINTER_STATE value;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 0)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		value = XUserInterface::m_currentWorkingUI->getCursor()->getState();
	}

	lua_pushnumber(pLuaState,(XU32)value);
	return 1;
}


int LAPI_UIShowCursor(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->showCursor(lua_toboolean(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIIsCursorVisible(lua_State* pLuaState)
{
	XBOOL visible = false;
	if (XUserInterface::m_currentWorkingUI)
	{
		visible = XUserInterface::m_currentWorkingUI->isCursorVisible();
	}

	lua_pushboolean(pLuaState,visible);
	return 1;
}


int LAPI_UILockCursorState(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		status = XUserInterface::getCursor()->lockState((X_POINTER_STATE)(XU64)lua_tonumber(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIIsCursorStateLocked(lua_State* pLuaState)
{
	XBOOL locked = false;
	if (XUserInterface::m_currentWorkingUI)
	{
		locked = XUserInterface::m_currentWorkingUI->getCursor()->isLocked();
	}

	lua_pushboolean(pLuaState,locked);
	return 1;
}


int LAPI_UIEnableCursor(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		status = XUserInterface::m_currentWorkingUI->enableCursor(lua_toboolean(pLuaState,1));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}


int LAPI_UIIsCursorEnabled(lua_State* pLuaState)
{
	XBOOL enabled = false;
	if (XUserInterface::m_currentWorkingUI)
	{
		enabled = XUserInterface::m_currentWorkingUI->isCursorEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}



int LAPI_UICreateSprite(lua_State* pLuaState)
{
	XUISprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pSprite,XUISprite,XOSMgr::getDefaultHeap());
		status = pSprite->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pSprite->destroy();
			XDelete(pSprite);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pSprite);

	return 1;
}


int LAPI_UICreateAnimSprite(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pSprite,XUIAnimSprite,XOSMgr::getDefaultHeap());
		status = pSprite->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));

		if (status != X_OK)
		{
			pSprite->destroy();
			XDelete(pSprite);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pSprite);

	return 1;
}



int LAPI_UISetAnimSpriteBeginRow(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		status = pSprite->getAnimSprite()->setBeginRow((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}


int LAPI_UIGetAnimSpriteBeginRow(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XU32 row = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		row = pSprite->getAnimSprite()->getBeginRow();
	}

	lua_pushnumber(pLuaState,row);
	return 1;
}
int LAPI_UISetAnimSpriteEndRow(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		status = pSprite->getAnimSprite()->setEndRow((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetAnimSpriteEndRow(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XU32 row = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		row = pSprite->getAnimSprite()->getEndRow();
	}

	lua_pushnumber(pLuaState,row);
	return 1;
}
int LAPI_UIGetAnimSpriteCurrentRow(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XU32 row = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		row = pSprite->getAnimSprite()->getCurrentRow();
	}

	lua_pushnumber(pLuaState,row);
	return 1;
}
int LAPI_UIGetAnimSpriteCurrentColumn(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XU32 col = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		col = pSprite->getAnimSprite()->getCurrentColumn();
	}

	lua_pushnumber(pLuaState,col);
	return 1;
}
int LAPI_UIGetAnimSpriteFrameWidth(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XU32 width = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		width = pSprite->getAnimSprite()->getFrameWidth();
	}

	lua_pushnumber(pLuaState,width);
	return 1;
}
int LAPI_UIGetAnimSpriteFrameHeight(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XU32 height = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		height = pSprite->getAnimSprite()->getFrameHeight();
	}

	lua_pushnumber(pLuaState,height);
	return 1;
}
int LAPI_UISetAnimSpriteFrameDelay(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		status = pSprite->getAnimSprite()->setFrameDelay((XF32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetAnimSpriteFrameDelay(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XF32 delay = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		delay = pSprite->getAnimSprite()->getFrameDelay();
	}

	lua_pushnumber(pLuaState,delay);
	return 1;
}
int LAPI_UISetAnimSpritePlayCount(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		status = pSprite->getAnimSprite()->setPlayCount((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetAnimSpritePlayCount(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XS32 playCount = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		playCount = pSprite->getAnimSprite()->getPlayCount();
	}

	lua_pushnumber(pLuaState,playCount);
	return 1;
}
int LAPI_UIPlayAnimSprite(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		status = pSprite->getAnimSprite()->play((XS32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIStopAnimSprite(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		status = pSprite->getAnimSprite()->stop();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsAnimSpritePlaying(lua_State* pLuaState)
{
	XUIAnimSprite* pSprite = NULL;
	XBOOL playing = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSprite = (XUIAnimSprite*)(XU64)lua_tonumber(pLuaState,1);
		playing = pSprite->getAnimSprite()->isPlaying();
	}

	lua_pushboolean(pLuaState,playing);
	return 1;
}







int LAPI_UICreateTextBox(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pTextBox,XUITextBox,XOSMgr::getDefaultHeap());
		status = pTextBox->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pTextBox->destroy();
			XDelete(pTextBox);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pTextBox);

	return 1;
}


int LAPI_UISetTextBoxText(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setText((XS8*)lua_tostring(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIGetTextBoxText(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XS8* pText = NULL;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		pText = pTextBox->getText();
	}

	lua_pushstring(pLuaState,pText);
	return 1;
}

int LAPI_UIAppendTextBoxText(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->appendText((XS8*)lua_tostring(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIRemoveTextBoxTextFromEnd(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->removeTextFromEnd((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UISetTextBoxBufferSize(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setTextBufferSize((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIGetTextBoxBufferSize(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 len = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		len = pTextBox->getTextBufferSize();
	}

	lua_pushnumber(pLuaState,len);
	return 1;
}
int LAPI_UIGetTextBoxTextLength(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 len = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		len = pTextBox->getTextLength();
	}

	lua_pushnumber(pLuaState,len);
	return 1;
}

int LAPI_UISetTextBoxScrollStep(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setScrollStep((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxScrollStep(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 step = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		step = pTextBox->getScrollStep();
	}

	lua_pushnumber(pLuaState,step);
	return 1;
}
int LAPI_UISetTextBoxReadOnlyMarker(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setReadOnlyMarker((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxReadOnlyMarker(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 marker = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		marker = pTextBox->getReadOnlyMarker();
	}

	lua_pushnumber(pLuaState,marker);
	return 1;
}
int LAPI_UISetTextBoxJustification(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setJustification((X_TB_JUST_TYPE)(XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxJustification(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	X_TB_JUST_TYPE just = X_TBJ_LEFT;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		just = pTextBox->getJustification();
	}

	lua_pushnumber(pLuaState,just);
	return 1;
}
int LAPI_UISetTextBoxCursorBlinkRate(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setCursorBlinkRate((XF32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxCursorBlinkRate(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XF32 rate = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		rate = pTextBox->getCursorBlinkRate();
	}

	lua_pushnumber(pLuaState,rate);
	return 1;
}
int LAPI_UIShowTextBoxCursor(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->showCursor(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIClearTextBox(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->clear();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxLineHeight(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 height = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		height = pTextBox->getLineHeight();
	}

	lua_pushnumber(pLuaState,height);
	return 1;
}
int LAPI_UIGetNativeTextBoxHeight(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 height = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		height = pTextBox->getNativeTextBoxHeight();
	}

	lua_pushnumber(pLuaState,height);
	return 1;
}
int LAPI_UIGetNativeTextBoxWidth(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 width = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		width = pTextBox->getNativeTextBoxWidth();
	}

	lua_pushnumber(pLuaState,width);
	return 1;
}
int LAPI_UIGetNumTextBoxLinesVisible(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XU32 num = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		num = pTextBox->getNumLinesVisible();
	}

	lua_pushnumber(pLuaState,num);
	return 1;
}
int LAPI_UISetTextBoxCharSet(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setCharSet((XHandle)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxCharSet(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XHandle charSetHandle = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		charSetHandle = pTextBox->getCharSet();
	}

	lua_pushnumber(pLuaState,charSetHandle);
	return 1;
}
int LAPI_UIEnableTextBoxAutoScroll(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->enableAutoScroll(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsTextBoxAutoScrollEnabled(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pTextBox->isAutoScrollEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UIEnableTextBoxWordWrapping(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->enableWordWrapping(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsTextBoxWordWrappingEnabled(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pTextBox->isWordWrappingEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UIEnableTextBoxStickyHighlightMode(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->enableStickyHighlightMode(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsTextBoxStickyHighlightModeEnabled(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pTextBox->isStickyHighlightModeEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UISetTextBoxEditable(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setEditable(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsTextBoxEditable(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pTextBox->isEditable();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UISetTextBoxHoverIndex(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pTextBox->setHoverIndex((XS32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetTextBoxHoverIndex(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XS32 index = -1;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		index = pTextBox->getHoverIndex();
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}
int LAPI_UIGetScaledTextBoxRect(lua_State* pLuaState)
{
	XUITextBox* pTextBox = NULL;
	XRect r;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pTextBox = (XUITextBox*)(XU64)lua_tonumber(pLuaState,1);
		pTextBox->getScaledTextBoxRect(r);
	}

	lua_pushnumber(pLuaState,r.left);
	lua_pushnumber(pLuaState,r.top);
	lua_pushnumber(pLuaState,r.right);
	lua_pushnumber(pLuaState,r.bottom);
	return 4;
}





int LAPI_UICreateMovie(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pMovie,XUI2DMovie,XOSMgr::getDefaultHeap());
		status = pMovie->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pMovie->destroy();
			XDelete(pMovie);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pMovie);

	return 1;
}

int LAPI_UIPlayMovie(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		status = pMovie->getMovie()->play((XS32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}


int LAPI_UIPauseMovie(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		status = pMovie->getMovie()->pause();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIResumeMovie(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		status = pMovie->getMovie()->resume();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIStopMovie(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		status = pMovie->getMovie()->stop();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UISetMovieTime(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		status = pMovie->getMovie()->setTime((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIGetMovieTime(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XU32 time = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		time = pMovie->getMovie()->getTime();
	}

	lua_pushnumber(pLuaState,time);
	return 1;
}

int LAPI_UISetMoviePlayCount(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		status = pMovie->getMovie()->setPlayCount((XS32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}


int LAPI_UIGetMoviePlayCount(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XS32 playCount = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		playCount = pMovie->getMovie()->getPlayCount();
	}

	lua_pushnumber(pLuaState,playCount);
	return 1;
}
int LAPI_UIGetMovieDuration(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XU32 duration = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		duration = pMovie->getMovie()->getDuration();
	}

	lua_pushnumber(pLuaState,duration);
	return 1;
}


int LAPI_UIIsMoviePlaying(lua_State* pLuaState)
{
	XUI2DMovie* pMovie = NULL;
	XBOOL playing = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pMovie = (XUI2DMovie*)(XU64)lua_tonumber(pLuaState,1);
		playing = pMovie->getMovie()->isPlaying();
	}

	lua_pushboolean(pLuaState,playing);
	return 1;
}



int LAPI_UICreateVerticalSlider(lua_State* pLuaState)
{
	XUIVerticalSlider* pSlider = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pSlider,XUIVerticalSlider,XOSMgr::getDefaultHeap());
		status = pSlider->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pSlider->destroy();
			XDelete(pSlider);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pSlider);

	return 1;
}


int LAPI_UISetVerticalSliderPercentage(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIVerticalSlider* pElement = (XUIVerticalSlider*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setPercentage((XF32)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIGetVerticalSliderPercentage(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XF32 p = 0.0f;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIVerticalSlider* pElement = (XUIVerticalSlider*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			p = pElement->getPercentage();	
	}

	lua_pushnumber(pLuaState,p);
	return 1;
}
int LAPI_UIIsVerticalSliderSliding(lua_State* pLuaState)
{
	XUIVerticalSlider* pSlider = NULL;
	XBOOL sliding = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSlider = (XUIVerticalSlider*)(XU64)lua_tonumber(pLuaState,1);
		sliding = pSlider->isSliding();
	}

	lua_pushboolean(pLuaState,sliding);
	return 1;
}



int LAPI_UICreateHorizontalSlider(lua_State* pLuaState)
{
	XUIHorizontalSlider* pSlider = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pSlider,XUIHorizontalSlider,XOSMgr::getDefaultHeap());
		status = pSlider->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pSlider->destroy();
			XDelete(pSlider);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pSlider);

	return 1;
}

int LAPI_UISetHorizontalSliderPercentage(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIHorizontalSlider* pElement = (XUIHorizontalSlider*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setPercentage((XF32)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIGetHorizontalSliderPercentage(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XF32 p = 0.0f;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIHorizontalSlider* pElement = (XUIHorizontalSlider*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			p = pElement->getPercentage();	
	}

	lua_pushnumber(pLuaState,p);
	return 1;
}
int LAPI_UIIsHorizontalSliderSliding(lua_State* pLuaState)
{
	XUIHorizontalSlider* pSlider = NULL;
	XBOOL sliding = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pSlider = (XUIHorizontalSlider*)(XU64)lua_tonumber(pLuaState,1);
		sliding = pSlider->isSliding();
	}

	lua_pushboolean(pLuaState,sliding);
	return 1;
}

int LAPI_UICreateVerticalScrollbar(lua_State* pLuaState)
{
	XUIVerticalScrollbar* pScrollbar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 14)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pScrollbar,XUIVerticalScrollbar,XOSMgr::getDefaultHeap());
		status = pScrollbar->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					XRect((XS32)lua_tonumber(pLuaState,6),
							(XS32)lua_tonumber(pLuaState,7),
							(XS32)lua_tonumber(pLuaState,8),
							(XS32)lua_tonumber(pLuaState,9)),
					XRect((XS32)lua_tonumber(pLuaState,10),
							(XS32)lua_tonumber(pLuaState,11),
							(XS32)lua_tonumber(pLuaState,12),
							(XS32)lua_tonumber(pLuaState,13)),
					(XS8*)lua_tostring(pLuaState,14));
		if (status != X_OK)
		{
			pScrollbar->destroy();
			XDelete(pScrollbar);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pScrollbar);

	return 1;
}

int LAPI_UISetVerticalScrollbarStep(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIVerticalScrollbar* pElement = (XUIVerticalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setStep((XU64)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIGetVerticalScrollbarStep(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XU32 step = 0;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIVerticalScrollbar* pElement = (XUIVerticalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			step = pElement->getStep();	
	}

	lua_pushnumber(pLuaState,step);
	return 1;
}
int LAPI_UISetVerticalScrollbarMaxSteps(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIVerticalScrollbar* pElement = (XUIVerticalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setMaxSteps((XU64)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIGetVerticalScrollbarMaxSteps(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XU32 steps = 0;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIVerticalScrollbar* pElement = (XUIVerticalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			steps = pElement->getMaxSteps();	
	}

	lua_pushnumber(pLuaState,steps);
	return 1;
}
int LAPI_UIIncrementVerticalScrollbarStep(lua_State* pLuaState)
{
	XUIVerticalScrollbar* pScrollbar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pScrollbar = (XUIVerticalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		status = pScrollbar->incrementStep();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIDecrementVerticalScrollbarStep(lua_State* pLuaState)
{
	XUIVerticalScrollbar* pScrollbar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pScrollbar = (XUIVerticalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		status = pScrollbar->decrementStep();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}




int LAPI_UICreateHorizontalScrollbar(lua_State* pLuaState)
{
	XUIHorizontalScrollbar* pScrollbar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 14)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pScrollbar,XUIHorizontalScrollbar,XOSMgr::getDefaultHeap());
		status = pScrollbar->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					XRect((XS32)lua_tonumber(pLuaState,6),
							(XS32)lua_tonumber(pLuaState,7),
							(XS32)lua_tonumber(pLuaState,8),
							(XS32)lua_tonumber(pLuaState,9)),
					XRect((XS32)lua_tonumber(pLuaState,10),
							(XS32)lua_tonumber(pLuaState,11),
							(XS32)lua_tonumber(pLuaState,12),
							(XS32)lua_tonumber(pLuaState,13)),
					(XS8*)lua_tostring(pLuaState,14));
		if (status != X_OK)
		{
			pScrollbar->destroy();
			XDelete(pScrollbar);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pScrollbar);

	return 1;
}

int LAPI_UISetHorizontalScrollbarStep(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIHorizontalScrollbar* pElement = (XUIHorizontalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setStep((XU64)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIGetHorizontalScrollbarStep(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XU32 step = 0;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIHorizontalScrollbar* pElement = (XUIHorizontalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			step = pElement->getStep();	
	}

	lua_pushnumber(pLuaState,step);
	return 1;
}
int LAPI_UISetHorizontalScrollbarMaxSteps(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIHorizontalScrollbar* pElement = (XUIHorizontalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setMaxSteps((XU64)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIGetHorizontalScrollbarMaxSteps(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XU32 steps = 0;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIHorizontalScrollbar* pElement = (XUIHorizontalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			steps = pElement->getMaxSteps();	
	}

	lua_pushnumber(pLuaState,steps);
	return 1;
}
int LAPI_UIIncrementHorizontalScrollbarStep(lua_State* pLuaState)
{
	XUIHorizontalScrollbar* pScrollbar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pScrollbar = (XUIHorizontalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		status = pScrollbar->incrementStep();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIDecrementHorizontalScrollbarStep(lua_State* pLuaState)
{
	XUIHorizontalScrollbar* pScrollbar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pScrollbar = (XUIHorizontalScrollbar*)(XU64)lua_tonumber(pLuaState,1);
		status = pScrollbar->decrementStep();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}






int LAPI_UICreateButton(lua_State* pLuaState)
{
	XUIButton* pButton = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pButton,XUIButton,XOSMgr::getDefaultHeap());
		status = pButton->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pButton->destroy();
			XDelete(pButton);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pButton);

	return 1;
}


int LAPI_UIIsButtonPressed(lua_State* pLuaState)
{
	XUIButton* pButton = NULL;
	XBOOL pressed = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pButton = (XUIButton*)(XU64)lua_tonumber(pLuaState,1);
		pressed = pButton->isPressed();
	}

	lua_pushboolean(pLuaState,pressed);
	return 1;
}
int LAPI_UIEnableButtonRepeatMode(lua_State* pLuaState)
{
	XUIButton* pButton = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pButton = (XUIButton*)(XU64)lua_tonumber(pLuaState,1);
		status = pButton->enableRepeatMode(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsButtonRepeatModeEnabled(lua_State* pLuaState)
{
	XUIButton* pButton = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pButton = (XUIButton*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pButton->isRepeatModeEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}




int LAPI_UICreateToggle(lua_State* pLuaState)
{
	XUIToggle* pToggle = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pToggle,XUIToggle,XOSMgr::getDefaultHeap());
		status = pToggle->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pToggle->destroy();
			XDelete(pToggle);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pToggle);

	return 1;
}


int LAPI_UISetToggleState(lua_State* pLuaState)
{
	XUIToggle* pToggle = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pToggle = (XUIToggle*)(XU64)lua_tonumber(pLuaState,1);
		status = pToggle->setState(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}


int LAPI_UICreateProgressBar(lua_State* pLuaState)
{
	XUIProgressBar* pProgressBar = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pProgressBar,XUIProgressBar,XOSMgr::getDefaultHeap());
		status = pProgressBar->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pProgressBar->destroy();
			XDelete(pProgressBar);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pProgressBar);

	return 1;
}

int LAPI_UIRenderProgressBarCoverFirst(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIProgressBar* pElement = (XUIProgressBar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->renderCoverFirst((XBOOL)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}

int LAPI_UIIsProgressBarCoverFirst(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XBOOL first = false;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIProgressBar* pElement = (XUIProgressBar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			first = pElement->isCoverFirst();	
	}

	lua_pushboolean(pLuaState,first);
	return 1;
}

int LAPI_UISetProgressBarPercentage(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIProgressBar* pElement = (XUIProgressBar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setPercentage((XF32)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}


int LAPI_GetProgressBarPercentage(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XF32 p = 0.0f;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIProgressBar* pElement = (XUIProgressBar*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			p = pElement->getPercentage();	
	}

	lua_pushnumber(pLuaState,p);
	return 1;
}

int LAPI_UICreateComboBox(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 26)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pComboBox,XUIComboBox(),XOSMgr::getDefaultHeap());
		status = pComboBox->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					XRect((XS32)lua_tonumber(pLuaState,6),
							(XS32)lua_tonumber(pLuaState,7),
							(XS32)lua_tonumber(pLuaState,8),
							(XS32)lua_tonumber(pLuaState,9)),
					XRect((XS32)lua_tonumber(pLuaState,10),
							(XS32)lua_tonumber(pLuaState,11),
							(XS32)lua_tonumber(pLuaState,12),
							(XS32)lua_tonumber(pLuaState,13)),
					XRect((XS32)lua_tonumber(pLuaState,14),
							(XS32)lua_tonumber(pLuaState,15),
							(XS32)lua_tonumber(pLuaState,16),
							(XS32)lua_tonumber(pLuaState,17)),
					XRect((XS32)lua_tonumber(pLuaState,18),
							(XS32)lua_tonumber(pLuaState,19),
							(XS32)lua_tonumber(pLuaState,20),
							(XS32)lua_tonumber(pLuaState,21)),
					XRect((XS32)lua_tonumber(pLuaState,22),
							(XS32)lua_tonumber(pLuaState,23),
							(XS32)lua_tonumber(pLuaState,24),
							(XS32)lua_tonumber(pLuaState,25)),
					(XS8*)lua_tostring(pLuaState,26));
		if (status != X_OK)
		{
			pComboBox->destroy();
			XDelete(pComboBox);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pComboBox);

	return 1;
}


int LAPI_UIAddComboBoxItem(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->addItem((XS8*)lua_tostring(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIInsertComboBoxItem(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->insertItem((XU64)lua_tonumber(pLuaState,2),(XS8*)lua_tostring(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIFindComboBoxItem(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XS32 index = -1;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIComboBox* pElement = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			index = pElement->findItem((XS8*)lua_tostring(pLuaState,2));	
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}

int LAPI_UIRemoveComboBoxItem(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->removeItem((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetComboBoxItem(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		item = pComboBox->getItem((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushstring(pLuaState,item);
	return 1;
}
int LAPI_UIGetSelectedComboBoxItem(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		item = pComboBox->getSelectedItem();
	}

	lua_pushstring(pLuaState,item);
	return 1;
}
int LAPI_UIGetNumComboBoxItems(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XU32 num = 0;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		num = pComboBox->getNumItems();
	}

	lua_pushnumber(pLuaState,num);
	return 1;
}
int LAPI_UIClearComboBox(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->clear();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIRefreshComboBox(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->refresh();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}







int LAPI_UIShowComboBoxListBox(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->showListBox();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIHideComboBoxListBox(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->hideListBox();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsComboBoxListBoxVisible(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XBOOL showing = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		showing = pComboBox->isListBoxVisible();
	}

	lua_pushboolean(pLuaState,showing);
	return 1;
}
int LAPI_UISetComboBoxJustification(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->setJustification((X_TB_JUST_TYPE)(XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetComboBoxJustification(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	X_TB_JUST_TYPE just = X_TBJ_LEFT;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		just = pComboBox->getJustification();
	}

	lua_pushnumber(pLuaState,just);
	return 1;
}
int LAPI_UIEnableComboBoxSorting(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->enableSorting(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIDisableComboBoxSorting(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->disableSorting();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsComboBoxSortingEnabled(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pComboBox->isSortingEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UIClearComboBoxSelectedItem(lua_State* pLuaState)
{
	XUIComboBox* pComboBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pComboBox = (XUIComboBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pComboBox->clearSelectedItem();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}














int LAPI_UICreateSpinner(lua_State* pLuaState)
{
	XUISpinner* pSpinner = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 14)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pSpinner,XUISpinner(),XOSMgr::getDefaultHeap());
		status = pSpinner->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					XRect((XS32)lua_tonumber(pLuaState,6),
							(XS32)lua_tonumber(pLuaState,7),
							(XS32)lua_tonumber(pLuaState,8),
							(XS32)lua_tonumber(pLuaState,9)),
					XRect((XS32)lua_tonumber(pLuaState,10),
							(XS32)lua_tonumber(pLuaState,11),
							(XS32)lua_tonumber(pLuaState,12),
							(XS32)lua_tonumber(pLuaState,13)),
					(XS8*)lua_tostring(pLuaState,14));
		if (status != X_OK)
		{
			pSpinner->destroy();
			XDelete(pSpinner);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pSpinner);

	return 1;
}

int LAPI_UIGetSpinnerValue(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XF32 value = 0.0f;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUISpinner* pElement = (XUISpinner*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			value = pElement->getValue();	
	}

	lua_pushnumber(pLuaState,value);
	return 1;
}
int LAPI_UISetSpinnerValue(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUISpinner* pElement = (XUISpinner*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setValue((XF32)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}
int LAPI_UIIncrementSpinnerStep(lua_State* pLuaState)
{
	XUISpinner* pSpinner = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSpinner = (XUISpinner*)(XU64)lua_tonumber(pLuaState,1);
		status = pSpinner->incrementStep();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIDecrementSpinnerStep(lua_State* pLuaState)
{
	XUISpinner* pSpinner = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pSpinner = (XUISpinner*)(XU64)lua_tonumber(pLuaState,1);
		status = pSpinner->decrementStep();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UISetSpinnerStepSize(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUISpinner* pElement = (XUISpinner*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			status = pElement->setStepSize((XF32)lua_tonumber(pLuaState,2));	
		else 
			status = X_ERR_INVALID_PARAMS;
	}

	lua_pushnumber(pLuaState,status);
	return 1;
}



int LAPI_UICreateListBox(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 18)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pListBox,XUIListBox(),XOSMgr::getDefaultHeap());
		status = pListBox->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					XRect((XS32)lua_tonumber(pLuaState,6),
							(XS32)lua_tonumber(pLuaState,7),
							(XS32)lua_tonumber(pLuaState,8),
							(XS32)lua_tonumber(pLuaState,9)),
					XRect((XS32)lua_tonumber(pLuaState,10),
							(XS32)lua_tonumber(pLuaState,11),
							(XS32)lua_tonumber(pLuaState,12),
							(XS32)lua_tonumber(pLuaState,13)),
					XRect((XS32)lua_tonumber(pLuaState,14),
							(XS32)lua_tonumber(pLuaState,15),
							(XS32)lua_tonumber(pLuaState,16),
							(XS32)lua_tonumber(pLuaState,17)),
					(XS8*)lua_tostring(pLuaState,18));
		if (status != X_OK)
		{
			pListBox->destroy();
			XDelete(pListBox);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pListBox);

	return 1;
}



int LAPI_UIAddListBoxItem(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->addItem((XS8*)lua_tostring(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIInsertListBoxItem(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->insertItem((XU64)lua_tonumber(pLuaState,2),(XS8*)lua_tostring(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIRemoveListBoxItem(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->removeItem((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIFindListBoxItem(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XS32 index = -1;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIListBox* pElement = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			index = pElement->findItem((XS8*)lua_tostring(pLuaState,2));	
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}
int LAPI_UIGetListBoxItem(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		item = pListBox->getItem((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushstring(pLuaState,item);
	return 1;
}
int LAPI_UIClearSelectedListBoxIndex(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->clearSelectedIndex();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetSelectedListBoxIndex(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XS32 index = -1;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		index = pListBox->getSelectedIndex();
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}
int LAPI_UIGetSelectedListBoxItem(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		item = pListBox->getSelectedItem();
	}

	lua_pushstring(pLuaState,item);
	return 1;
}
int LAPI_UIGetNumListBoxItems(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XU32 num = 0;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		num = pListBox->getNumItems();
	}

	lua_pushnumber(pLuaState,num);
	return 1;
}
int LAPI_UIClearListBox(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->clear();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIRefreshListBox(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->refresh();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIEnableListBoxStickyHighlightMode(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->enableStickyHighlightMode(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsListBoxStickyHighlightModeEnabled(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pListBox->isStickyHighlightModeEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}

int LAPI_UISetListBoxHoverIndex(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->setHoverIndex((XS32)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListBoxHoverIndex(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XS32 index = -1;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		index = pListBox->getHoverIndex();
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}
int LAPI_UISetListBoxScrollStep(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->setScrollStep((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListBoxScrollStep(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XU32 step = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		step = pListBox->getScrollStep();
	}

	lua_pushnumber(pLuaState,step);
	return 1;
}
int LAPI_UISetListBoxJustification(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->setJustification((X_TB_JUST_TYPE)(XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListBoxJustification(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	X_TB_JUST_TYPE just = X_TBJ_LEFT;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		just = pListBox->getJustification();
	}

	lua_pushnumber(pLuaState,just);
	return 1;
}
int LAPI_UIEnableListBoxSorting(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->enableSorting(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIDisableListBoxSorting(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		status = pListBox->disableSorting();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsListBoxSortingEnabled(lua_State* pLuaState)
{
	XUIListBox* pListBox = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pListBox = (XUIListBox*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pListBox->isSortingEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}






int LAPI_UICreateListView(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;

	if (XUserInterface::m_currentWorkingUI)
	{
		XNew(pListView,XUIListView,XOSMgr::getDefaultHeap());
		XRect rects[23];
		XU32 numRects = (XU64)lua_tonumber(pLuaState,2);

		if (numRects < 20)
		{
			XU32 i = 0;
			for (i = 0;i < numRects+3;++i)
			{
				rects[i] = XRect((XS32)lua_tonumber(pLuaState,(i*4)+3),
								(XS32)lua_tonumber(pLuaState,(i*4)+4),
								(XS32)lua_tonumber(pLuaState,(i*4)+5),
								(XS32)lua_tonumber(pLuaState,(i*4)+6));
			}
		
			status = pListView->init(XUserInterface::m_currentWorkingUI,
						(XU64)lua_tonumber(pLuaState,1),
						numRects,
						rects,
						rects[numRects],
						rects[numRects+1],
						rects[numRects+2],
						(XS8*)lua_tostring(pLuaState,(numRects*4)+15));
		
			if (status != X_OK)
			{
				pListView->destroy();
				XDelete(pListView);
			}
		}
	}

	lua_pushnumber(pLuaState,(XU64)pListView);

	return 1;
}


int LAPI_UIAddListViewItem(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->addItem((XS8*)lua_tostring(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIInsertListViewItem(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->insertItem((XU64)lua_tonumber(pLuaState,2),
										(XS8*)lua_tostring(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListViewItem(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		item = pListView->getItem((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushstring(pLuaState,item);
	return 1;
}
int LAPI_UIFindListViewItem(lua_State* pLuaState)
{
	XStatus status = X_OK;
	XS32 index = -1;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIListView* pElement = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
			index = pElement->findItem((XS8*)lua_tostring(pLuaState,2));	
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}
int LAPI_UIRemoveListViewItem(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->removeItem((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UISetListViewSubItem(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 4)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->setSubItem((XU64)lua_tonumber(pLuaState,2),
									   (XU64)lua_tonumber(pLuaState,3),
									   (XS8*)lua_tostring(pLuaState,4));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIGetListViewSubItem(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		item = pListView->getSubItem((XU64)lua_tonumber(pLuaState,2),
									  (XU64)lua_tonumber(pLuaState,3));
	}

	lua_pushstring(pLuaState,item);
	return 1;
}

int LAPI_UIClearSelectedListViewIndex(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->clearSelectedIndex();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetSelectedListViewIndex(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XS32 index = -1;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		index = pListView->getSelectedIndex();
	}

	lua_pushnumber(pLuaState,index);
	return 1;
}
int LAPI_UIGetNumListViewItems(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XU32 num = 0;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		num = pListView->getNumItems();
	}

	lua_pushnumber(pLuaState,num);
	return 1;
}
int LAPI_UISetListViewColumnHeading(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->setColumnHeading((XU64)lua_tonumber(pLuaState,2),
											 (XS8*)lua_tostring(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListViewColumnHeading(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XS8* item = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		item = pListView->getColumnHeading((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushstring(pLuaState,item);
	return 1;
}
int LAPI_UISetListViewColumnJustification(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->setColumnJustification((XU64)lua_tonumber(pLuaState,2),
													(X_TB_JUST_TYPE)(XU64)lua_tonumber(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListViewColumnJustification(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	X_TB_JUST_TYPE just = X_TBJ_LEFT;
	if ((lua_gettop(pLuaState) == 2) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		just = pListView->getColumnJustification((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,just);
	return 1;
}
int LAPI_UISetListViewColumnHeadingJustification(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->setColumnHeadingJustification((XU64)lua_tonumber(pLuaState,2),
													(X_TB_JUST_TYPE)(XU64)lua_tonumber(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListViewColumnHeadingJustification(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	X_TB_JUST_TYPE just = X_TBJ_LEFT;
	if ((lua_gettop(pLuaState) == 2) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		just = pListView->getColumnHeadingJustification((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,just);
	return 1;
}

int LAPI_UISetListViewColumnHeadingToolTipText(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 3)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->setColumnHeadingToolTipText((XU64)lua_tonumber(pLuaState,2),
														  (XS8*)lua_tostring(pLuaState,3));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetListViewColumnHeadingToolTipText(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XS8* pText = NULL;
	if ((lua_gettop(pLuaState) == 2) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		pText = pListView->getColumnHeadingToolTipText((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushstring(pLuaState,pText);
	return 1;
}



int LAPI_UILockListViewColumnSizes(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->lockColumnSizes(lua_toboolean(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}

int LAPI_UIClearListView(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->clear();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIRefreshListView(lua_State* pLuaState)
{
	XUIListView* pListView = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pListView = (XUIListView*)(XU64)lua_tonumber(pLuaState,1);
		status = pListView->refresh();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}



int LAPI_UICreateFrame(lua_State* pLuaState)
{
	XUIFrame* pFrame = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 6)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XNew(pFrame,XUIFrame,XOSMgr::getDefaultHeap());
		status = pFrame->init(XUserInterface::m_currentWorkingUI,
					(XU64)lua_tonumber(pLuaState,1),
					XRect((XS32)lua_tonumber(pLuaState,2),
							(XS32)lua_tonumber(pLuaState,3),
							(XS32)lua_tonumber(pLuaState,4),
							(XS32)lua_tonumber(pLuaState,5)),
					(XS8*)lua_tostring(pLuaState,6));
		if (status != X_OK)
		{
			pFrame->destroy();
			XDelete(pFrame);
		}
	}

	lua_pushnumber(pLuaState,(XU64)pFrame);

	return 1;
}


int LAPI_UIAddElementToFrame(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		XUIFrame* pFrame = (XUIFrame*)(XU64)lua_tonumber(pLuaState,1);
		if (pFrame)
			status = pFrame->addElement((XUIElement*)(XU64)lua_tonumber(pLuaState,2));
		else
			status = X_ERR_INVALID_PARAMS;

	}
	
	lua_pushnumber(pLuaState,(XF64)status);

	return 1;
}




int LAPI_UIDestroyElement(lua_State* pLuaState)
{
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if (status == X_OK)
	{
		XUIElement* pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		if (pElement)
		{
			pElement->destroy();
			XDelete(pElement);
		}
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}



int LAPI_UISetElementZOrderOffset(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->setZOrderOffset((XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetElementZOrderOffset(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XU32 zOrderOffset = 0;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		zOrderOffset = pElement->getZOrderOffset();
	}

	lua_pushnumber(pLuaState,zOrderOffset);
	return 1;
}
int LAPI_UISetElementToolTipText(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->setToolTipText((XS8*)lua_tostring(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetElementToolTipText(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XS8* text = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		text = pElement->getToolTipText();
	}

	lua_pushstring(pLuaState,text);
	return 1;
}
int LAPI_UIEnableElement(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->enable();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIDisableElement(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->disable();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsElementEnabled(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XBOOL enabled = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		enabled = pElement->isEnabled();
	}

	lua_pushboolean(pLuaState,enabled);
	return 1;
}
int LAPI_UIShowElement(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->show();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIHideElement(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->hide();
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIIsElementVisible(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XBOOL showing = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		showing = pElement->isEnabled();
	}

	lua_pushboolean(pLuaState,showing);
	return 1;
}
int LAPI_UIElementHasFocus(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XBOOL focus = false;
	if ((lua_gettop(pLuaState) == 1) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		focus = pElement->hasFocus();
	}

	lua_pushboolean(pLuaState,focus);
	return 1;
}
int LAPI_UIIsPointInsideElement(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XBOOL inside = false;
	if ((lua_gettop(pLuaState) == 3) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		inside = pElement->isPointInside(XVector2D((XF32)lua_tonumber(pLuaState,2),
													(XF32)lua_tonumber(pLuaState,3)));
	}

	lua_pushboolean(pLuaState,inside);
	return 1;
}
int LAPI_UISetElementPointerState(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->setPointerState((X_POINTER_STATE)(XU64)lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetElementPointerState(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	X_POINTER_STATE ps = X_PS_NORMAL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		ps = pElement->getPointerState();
	}

	lua_pushnumber(pLuaState,(XU32)ps);
	return 1;
}
int LAPI_UISetElementOpacity(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 2)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		status = pElement->setOpacity(lua_tonumber(pLuaState,2));
	}

	lua_pushnumber(pLuaState,(XF64)status);
	return 1;
}
int LAPI_UIGetElementOpacity(lua_State* pLuaState)
{
	XUIElement* pElement = NULL;
	XF32 opacity = 1.0f;
	XStatus status = X_OK;
	if (lua_gettop(pLuaState) != 1)
		status = X_ERR_INVALID_PARAMS;

	if ((status == X_OK) && (XUserInterface::m_currentWorkingUI))
	{
		pElement = (XUIElement*)(XU64)lua_tonumber(pLuaState,1);
		opacity = pElement->getOpacity();
	}

	lua_pushnumber(pLuaState,opacity);
	return 1;
}
