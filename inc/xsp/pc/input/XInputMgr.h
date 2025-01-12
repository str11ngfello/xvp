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
//	Module Name:	XInputMgr.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_INPUT_MGR_H__
#define __X_INPUT_MGR_H__

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>
#include <XInputMgrTypes.h>
#include <XVPDesc.h>
#include <XTimer.h>



//*****************************************************************************
//Locals
//*****************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext);



//*****************************************************************************
//Defines
//*****************************************************************************
#define KEYDOWN(key)		(m_keyboardBuffer[key] & 0x80)




//*****************************************************************************
//! Input device and resource manager.
/**
The XInputMgr object represents the main input driver for X-VP.  The engine
manages the input driver which deals with aspects of input devices as well as
resources and data needed by the devices.  You can use the input driver to read
input devices such as keyboards, mice, and joysticks/gamespads.

See below for more information.
*/
//*****************************************************************************
class XInputMgr
{
private:
	//*****************************************************************************
	//DirectX System Variables
	//*****************************************************************************
	LPDIRECTINPUTDEVICE8	m_keyboard; 
    LPDIRECTINPUTDEVICE8	m_mouse; 
	DIJOYSTATE2				m_js[X_MAX_JOYSTICKS];
	DIDEVCAPS				m_devCaps;
	bool					m_windowed;

	//*****************************************************************************
	//Driver State Information
	//*****************************************************************************
	DIMOUSESTATE			m_dims;
	XU8					m_mouseButtonBufferDownOnce[X_MAX_MOUSE_BUTTONS];
	XU8					m_mouseButtonBufferUpOnce[X_MAX_MOUSE_BUTTONS];
	XU8					m_mouseButtonBufferOld[X_MAX_MOUSE_BUTTONS];
	XU8					m_keyboardBuffer[X_MAX_KEYS];
	XU8					m_keyboardBufferDownOnce[X_MAX_KEYS];
	XU8					m_keyboardBufferUpOnce[X_MAX_KEYS];
	XU8					m_keyboardBufferOld[X_MAX_KEYS];
	XU8					m_jsButtonBufferDownOnce[X_MAX_JOYSTICKS][X_MAX_JOY_BUTTONS];
	XU8					m_jsButtonBufferUpOnce[X_MAX_JOYSTICKS][X_MAX_JOY_BUTTONS];
	XU8					m_jsButtonBufferOld[X_MAX_JOYSTICKS][X_MAX_JOY_BUTTONS];
	HRESULT					m_hr;
	bool					m_inited;
	XTimer					m_keyRepeatTimer;
	bool					m_keyRepeatEnabled;
	bool					m_keyRepeatOn;
	XF32					m_keyRepeatTimeout;
	XTimer					m_mouseRepeatTimer;
	bool					m_mouseRepeatEnabled;
	bool					m_mouseRepeatOn;
	XF32					m_mouseRepeatTimeout;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus initLuaGlue();

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XInputMgr();
	~XInputMgr();

	//*****************************************************************************
	//Standard XInputMgr Functions
	//*****************************************************************************
	XStatus init(XVPDesc& initDesc);
	XStatus update(XF32 dt);
	XStatus pollDevices();
	XStatus destroy();
	XStatus setKeyRepeatTimeout(float seconds);
	XF32 getKeyRepeatTimeout();
	XStatus setMouseRepeatTimeout(float seconds);
	XF32 getMouseRepeatTimeout();

	//*****************************************************************************
	//Joystick Functions
	//*****************************************************************************
	XU32 getNumJoysticks();
	XF32 getJoyXExtent(XU32 jsIndex);
	XF32 getJoyYExtent(XU32 jsIndex);
	XF32 getJoyZExtent(XU32 jsIndex);
	XF32 getJoyZRollExtent(XU32 jsIndex);
	bool joyButtonDown(XU32 jsIndex,X_JOY_BUTTON button);
	bool joyButtonDownOnce(XU32 jsIndex,X_JOY_BUTTON button);
	bool joyButtonUpOnce(XU32 jsIndex,X_JOY_BUTTON button);
	bool anyJoyButtonInput(XU32 jsIndex);

	//*****************************************************************************
	//Mouse Functions
	//*****************************************************************************
	bool mouseButtonDown(X_MOUSE_BUTTON button);
	bool mouseButtonDownOnce(X_MOUSE_BUTTON button);
	bool mouseButtonDownOnceRepeat(X_MOUSE_BUTTON button);
	bool mouseButtonUpOnce(X_MOUSE_BUTTON button);
	XS32 getMouseRelXMovement();
	XS32 getMouseRelYMovement();
	XS32 getMouseRelZMovement();
	bool anyMouseButtonInput();

	//*****************************************************************************
	//Keyboard Functions
	//*****************************************************************************
	bool keyDown(X_KEY key);
	bool keyDownOnce(X_KEY key);
	bool keyDownOnceRepeat(X_KEY key);
	bool keyUpOnce(X_KEY key);
	bool anyKeyboardInput();
};

#endif