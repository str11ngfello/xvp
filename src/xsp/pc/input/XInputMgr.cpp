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
//	Module Name:	XInputMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************
 
#include <xvpsdk.h>



#define X_INPUT_LUA_LIB_NAME	"Input"

static const luaL_reg XInputLuaLib[] = 
{
	{"showMethods",LAPI_input},
	{"setKeyRepeatTimeout",LAPI_setKeyRepeatTimeout},
	{"getKeyRepeatTimeout",LAPI_getKeyRepeatTimeout},
	{"getNumJoysticks",LAPI_getNumJoysticks},
	{"getJoyXExtent",LAPI_getJoyXExtent},
	{"getJoyYExtent",LAPI_getJoyYExtent},
	{"getJoyZExtent",LAPI_getJoyZExtent},
	{"getJoyZRollExtent",LAPI_getJoyZRollExtent},
	{"joyButtonDown",LAPI_joyButtonDown},
	{"joyButtonDownOnce",LAPI_joyButtonDownOnce},
	{"joyButtonUpOnce",LAPI_joyButtonUpOnce},
	{"anyJoyButtonInput",LAPI_anyJoyButtonInput},
	{"mouseButtonDown",LAPI_mouseButtonDown},
	{"mouseButtonDownOnce",LAPI_mouseButtonDownOnce},
	{"mouseButtonUpOnce",LAPI_mouseButtonUpOnce},
	{"getMouseRelXMovement",LAPI_getMouseRelXMovement},
	{"getMouseRelYMovement",LAPI_getMouseRelYMovement},
	{"getMouseRelZMovement",LAPI_getMouseRelZMovement},
	{"anyMouseButtonInput",LAPI_anyMouseButtonInput},
	{"keyDown",LAPI_keyDown},
	{"keyDownOnce",LAPI_keyDownOnce},
	{"keyDownOnceRepeat",LAPI_keyDownOnceRepeat},
	{"keyUpOnce",LAPI_keyUpOnce},
	{"anyKeyboardInput",LAPI_anyKeyboardInput},
	{NULL,NULL}
};



//*****************************************************************************
//These objects must be global because of the nature of directX callbacks.
//*****************************************************************************
LPDIRECTINPUT8 X_DI;         
LPDIRECTINPUTDEVICE8 X_joysticks[X_MAX_JOYSTICKS] = {NULL};
XU32 X_numAttachedJoysticks = 0;



//*****************************************************************************
/**
Constructor for input driver.  
*/
//*****************************************************************************
XInputMgr::XInputMgr()
{
	m_keyboard = NULL;
	m_mouse = NULL;

	m_keyRepeatTimer.reset();
	m_keyRepeatTimer.start();
	m_keyRepeatOn = false;
	m_keyRepeatTimeout = (XF32).6;
	
	m_mouseRepeatTimer.reset();
	m_mouseRepeatTimer.start();
	m_mouseRepeatOn = false;
	m_mouseRepeatTimeout = (XF32).6;

	//Zero out the buffers
	memset(m_keyboardBuffer,0,sizeof(XU8)*X_MAX_KEYS);
	memset(m_keyboardBufferDownOnce,0,sizeof(XU8)*X_MAX_KEYS);
	memset(m_keyboardBufferUpOnce,0,sizeof(XU8)*X_MAX_KEYS);
	memset(m_keyboardBufferOld,0,sizeof(XU8)*X_MAX_KEYS);

	for (XU32 i = 0;i < X_MAX_JOYSTICKS;i++)
	{
		memset(&m_js[i],0,sizeof(DIJOYSTATE2));
		memset(&m_jsButtonBufferDownOnce[i],0,sizeof(XU8)*X_MAX_JOY_BUTTONS);
		memset(&m_jsButtonBufferUpOnce[i],0,sizeof(XU8)*X_MAX_JOY_BUTTONS);
		memset(&m_jsButtonBufferOld[i],0,sizeof(XU8)*X_MAX_JOY_BUTTONS);
	}

	memset(&m_dims,0,sizeof(DIMOUSESTATE));
	memset(&m_mouseButtonBufferDownOnce,0,sizeof(XU8)*X_MAX_MOUSE_BUTTONS);
	memset(&m_mouseButtonBufferUpOnce,0,sizeof(XU8)*X_MAX_MOUSE_BUTTONS);
	memset(&m_mouseButtonBufferOld,0,sizeof(XU8)*X_MAX_MOUSE_BUTTONS);
	m_windowed = false;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the input driver.  
*/
//*****************************************************************************
XInputMgr::~XInputMgr()
{
}



//*****************************************************************************
/**
Initializes the input driver.  This routine is called by X-VP during
start up.  

@param initDesc	Initialization description structure.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputMgr::init(XVPDesc& initDesc)
{
	m_inited = true;

	X_DEBUG_LOG_LN("\n*** BEGIN INPUT MANAGER INIT ***",X_DLL_INFO);

	//m_windowed = initDesc.windowed;

	//First, create the main DirectInput object.
	m_hr = DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION, 
                       IID_IDirectInput8,(VOID**)&X_DI,NULL);
	if (m_hr != DI_OK)
	{
		X_ASSERT_FORCED("DirectInput8Create() failed.");
		return X_ERR_OPERATION_FAILED;
	}
	X_DEBUG_LOG_LN("DirectInput8Create() succeeded.",X_DLL_INFO);

	//If the user want control of the keyboard
	if (initDesc.enableKeyboard)
	{
		// Obtain an interface to the system keyboard device.
		m_hr = X_DI->CreateDevice(GUID_SysKeyboard,&m_keyboard,NULL);
		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not create keyboard device.",X_DLL_WARNING);
			return X_ERR_OPERATION_FAILED;
		}
		X_DEBUG_LOG_LN("Created keyboard device.",X_DLL_INFO);

		//Set the data format to "keyboard format" - a predefined data format 
		//
		//A data format specifies which controls on a device we
		//are interested in, and how they should be reported.
		//
		//This tells DirectInput that we will be passing an array
		//of 256 bytes to IDirectInputDevice::GetDeviceState.
		m_hr = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not create keyboard data format.",X_DLL_WARNING);
			return X_ERR_OPERATION_FAILED;
		}
		X_DEBUG_LOG_LN("Created keyboard data format.",X_DLL_INFO);
		
		//Set the cooperativity level to let DirectInput know how
		//this device should interact with the system and with other
		//DirectInput applications.
		//if (m_windowed)
		//	m_hr = m_keyboard->SetCooperativeLevel((HWND)initDesc.windowHandle,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
		//else
		//	m_hr = m_keyboard->SetCooperativeLevel((HWND)initDesc.windowHandle,DISCL_FOREGROUND | DISCL_EXCLUSIVE);

		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not set keyboard co-op level.",X_DLL_WARNING);
			//return X_ERR_OPERATION_FAILED;
		}
		else
			X_DEBUG_LOG_LN("Set keyboard co-op level.",X_DLL_INFO); 
		
		// Acquire the newly created device
		m_hr = m_keyboard->Acquire();
	}

	//If the user wants mouse support.
	if (initDesc.enableMouse)
	{
		//Obtain an interface to the system mouse device.
		m_hr = X_DI->CreateDevice(GUID_SysMouse,&m_mouse,NULL);
		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not create mouse device.",X_DLL_WARNING);
			return X_ERR_OPERATION_FAILED;
		}
		else
			X_DEBUG_LOG_LN("Created mouse device.",X_DLL_INFO);

    	//Set the data format to "mouse format" - a predefined data format 
		//A data format specifies which controls on a device we
		//are interested in, and how they should be reported.
		//This tells DirectInput that we will be passing a
		//DIMOUSESTATE structure to IDirectInputDevice::GetDeviceState.
		m_hr = m_mouse->SetDataFormat(&c_dfDIMouse);
		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not set mouse format.",X_DLL_WARNING);
			return X_ERR_OPERATION_FAILED;
		}
		X_DEBUG_LOG_LN("Set mouse format.",X_DLL_INFO);
    
		//Set the cooperativity level to let DirectInput know how
		//this device should interact with the system and with other
		//DirectInput applications.
		//m_hr = m_mouse->SetCooperativeLevel((HWND)initDesc.windowHandle,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	
		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not set mouse co-op level.",X_DLL_WARNING);
			//return X_ERR_OPERATION_FAILED;
		}
		else
			X_DEBUG_LOG_LN("Set mouse co-op level.",X_DLL_INFO);
		
		//Finally, aquire the mouse.
		m_hr = m_mouse->Acquire();
	}

	//If the user wants to support a joystick
	if (initDesc.enableJoysticks)
	{
		//Now lets see what joysticks are connected.
		m_hr = X_DI->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,
						  NULL, DIEDFL_ATTACHEDONLY);
		if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not create joystick devices.",X_DLL_WARNING);
			return X_ERR_OPERATION_FAILED;
		}
		X_DEBUG_LOG_LN("Created joystick devices.",X_DLL_INFO);
		
		//Setup each attached joystick.
		for (XU32 i = 0;i < X_numAttachedJoysticks;i++)
		{
			// Set the data format to "simple joystick" - a predefined data format 
			//
			// A data format specifies which controls on a device we are interested in,
			// and how they should be reported. This tells DInput that we will be
			// passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
			m_hr = X_joysticks[i]->SetDataFormat(&c_dfDIJoystick2);
			if (m_hr != DI_OK)
			{
				X_DEBUG_LOG_LN("Could not set joystick data format.",X_DLL_WARNING);
				return X_ERR_OPERATION_FAILED;
			}
			X_DEBUG_LOG_LN("Set joystick data format.",X_DLL_INFO);

			// Set the cooperative level to let DInput know how this device should
			// interact with the system and with other DInput applications.
			//if (m_windowed)
			//	m_hr = X_joysticks[i]->SetCooperativeLevel((HWND)initDesc.windowHandle,DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
			//else
			//	m_hr = X_joysticks[i]->SetCooperativeLevel((HWND)initDesc.windowHandle,DISCL_FOREGROUND | DISCL_EXCLUSIVE);

			if (m_hr != DI_OK)
			{
				X_DEBUG_LOG_LN("Could not set joystick co-op level.",X_DLL_WARNING);
				return X_ERR_OPERATION_FAILED;
			}
			X_DEBUG_LOG_LN("Set joystick co-op level.",X_DLL_INFO);

			// Determine how many axis the joystick has (so we don't error out setting
			// properties for unavailable axis)
			m_devCaps.dwSize = sizeof(DIDEVCAPS);
			X_joysticks[i]->GetCapabilities(&m_devCaps);

			// Enumerate the axes of the joyctick and set the range of each axis. Note:
			// we could just use the defaults, but we're just trying to show an example
			// of enumerating device objects (axes, buttons, etc.).
			XU32 numJoy = i;
			X_joysticks[i]->EnumObjects(EnumAxesCallback,(VOID*)&numJoy/*(VOID*)hwnd*/,DIDFT_AXIS);


			// Set the dead zone for the joystick axes (because many joysticks
			// aren't perfectly calibrated to be zero when centered).
			/*DIPROPDWORD dipdw; 
			dipdw.diph.dwSize       = sizeof(DIPROPDWORD); 
			dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
			dipdw.diph.dwHow        = DIPH_DEVICE;	//This will make dead zone apply to all axis. 
			dipdw.dwData            = 0;			// Here, 5000 = 50%

			dipdw.diph.dwObj = DIJOFS_X;			// Set the x-axis deadzone

			//Set dead zone for the entire device.
			X_joysticks[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
			*/
			
			//Finally acquire joystick.
			m_hr = X_joysticks[i]->Acquire();
		}
	}

	//Initialize lua glue
	initLuaGlue();

	X_DEBUG_LOG_LN("*** END INPUT MANAGER INIT ***",X_DLL_INFO);

	//If we make it here then everything went okay.
	return X_OK;
}



//***************************************************************************** 
/**
Shuts down the input driver.  This routine is called by X-VP during
shutdown and gives the driver an opportunity to clean up any remaining
resources and shut down the input device(s).

@return A return code of type XStatus.
*/ 
//***************************************************************************** 
XStatus XInputMgr::destroy()
{
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
	}
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
	}
	for (XU32 i = 0;i < X_numAttachedJoysticks;i++)
	{	
		X_joysticks[i]->Unacquire();
		X_joysticks[i]->Release();
	}
	if (X_DI)
		X_DI->Release();

	m_inited = false;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XInputMgr::setKeyRepeatTimeout(XF32 seconds)
{
	if (seconds >= 0)
	{
		m_keyRepeatTimeout = seconds;
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//***************************************************************************** 
//***************************************************************************** 
XF32 XInputMgr::getKeyRepeatTimeout()
{
	return m_keyRepeatTimeout;
}

	
	
//***************************************************************************** 
//***************************************************************************** 
XStatus XInputMgr::setMouseRepeatTimeout(XF32 seconds)
{
	if (seconds >= 0)
	{
		m_mouseRepeatTimeout = seconds;
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//***************************************************************************** 
//***************************************************************************** 
XF32 XInputMgr::getMouseRepeatTimeout()
{
	return m_mouseRepeatTimeout;
}



//***************************************************************************** 
/**
Updates the input driver.  This routine is called by X-VP during
an update sequence and gives the driver an opportunity to udpate any internal,
time dependent data.

@param dt	The amount of time, in seconds, that has passed since the last
					update.
@return A return code of type XStatus.
*/  
//***************************************************************************** 
XStatus XInputMgr::update(XF32 dt)
{
	X_DEBUG_LOG_LN("---------- BEGIN DEVICE POLL ----------",X_DLL_VERBOSE);
	pollDevices();
	X_DEBUG_LOG_LN("---------- END DEVICE POLL ----------",X_DLL_VERBOSE);

	if (!anyKeyboardInput() ||
		keyDown(X_KEY_LSHIFT) ||
		keyDown(X_KEY_RSHIFT) ||
		keyDown(X_KEY_LCONTROL) ||
		keyDown(X_KEY_RCONTROL) ||
		keyDown(X_KEY_LMENU) ||
		keyDown(X_KEY_RMENU))
	{
		m_keyRepeatTimer.reset();
		m_keyRepeatOn = false;
	}
	else
	{
		if (m_keyRepeatTimer.getDuration() >= m_keyRepeatTimeout)
			m_keyRepeatOn = true;
	}


	if (!anyMouseButtonInput())
	{
		m_mouseRepeatTimer.reset();
		m_mouseRepeatOn = false;
	}
	else
	{
		if (m_mouseRepeatTimer.getDuration() >= m_mouseRepeatTimeout)
			m_mouseRepeatOn = true;
	}
	

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XInputMgr::getNumJoysticks()
{
	return X_numAttachedJoysticks;
}



//*****************************************************************************
/**
Gets the extent of a joystick's X axis.  This value can range from -1.0 to 
1.0.  For example, if the joystick is pressed fully in the negative X direction,
this function will return -1.0.  On the other hand, if the joystick is pressed
fully in the positive X direction, this function will return 1.0.  If the 
joystick is neutral on the X axis, the function will return 0.

@param jsIndex	The joystick to query.  This value is 0 for joystick one, 1 for
				joystick two, 2 for joystick three, and so on.
@return The current extent of the joystick's X axis mapped onto the range -1.0
		to 1.0.
*/
//*****************************************************************************
XF32 XInputMgr::getJoyXExtent(XU32 jsIndex)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return 0;

	if (m_js[jsIndex].lX == 0)
		return 0;
	else
		return m_js[jsIndex].lX / (XF32)X_JOY_AXIS_EXTENT;
}



//*****************************************************************************
/**
Gets the extent of a joystick's Y axis.  This value can range from -1.0 to 
1.0.  For example, if the joystick is pressed fully in the negative Y direction,
this function will return -1.0.  On the other hand, if the joystick is pressed
fully in the positive Y direction, this function will return 1.0.  If the 
joystick is neutral on the Y axis, the function will return 0.

@param jsIndex	The joystick to query.  This value is 0 for joystick one, 1 for
				joystick two, 2 for joystick three, and so on.
@return The current extent of the joystick's Y axis mapped onto the range -1.0
		to 1.0.
*/
//*****************************************************************************
XF32 XInputMgr::getJoyYExtent(XU32 jsIndex)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return 0;

	if (m_js[jsIndex].lY == 0)
		return 0;
	else
		return m_js[jsIndex].lY / (XF32)X_JOY_AXIS_EXTENT;
}




//*****************************************************************************
/**
Gets the extent of a joystick's Z axis.  This value can range from -1.0 to 
1.0.  For example, if the joystick is pressed fully in the negative Z direction,
this function will return -1.0.  On the other hand, if the joystick is pressed
fully in the positive Z direction, this function will return 1.0.  If the 
joystick is neutral on the Z axis, the function will return 0.

@param jsIndex	The joystick to query.  This value is 0 for joystick one, 1 for
				joystick two, 2 for joystick three, and so on.
@return The current extent of the joystick's Z axis mapped onto the range -1.0
		to 1.0.
*/
//*****************************************************************************
XF32 XInputMgr::getJoyZExtent(XU32 jsIndex)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return 0;

	if (m_js[jsIndex].lZ == 0)
		return 0;
	else
		return m_js[jsIndex].lZ / (XF32)X_JOY_AXIS_EXTENT;
}



//*****************************************************************************
/**
Gets the extent of a joystick's Z Roll axis.  This value can range from -1.0 to 
1.0.  For example, if the joystick is pressed fully in the negative Z Roll
direction, this function will return -1.0.  On the other hand, if the joystick
is pressed fully in the positive Z Roll direction, this function will return 1.0.
If the joystick is neutral on the Z Roll axis, the function will return 0.

@param jsIndex	The joystick to query.  This value is 0 for joystick one, 1 for
				joystick two, 2 for joystick three, and so on.
@return The current extent of the joystick's Z Roll axis mapped onto the range -1.0
		to 1.0.
*/
//*****************************************************************************
XF32 XInputMgr::getJoyZRollExtent(XU32 jsIndex)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return 0;

	if (m_js[jsIndex].lRz == 0)
		return 0;
	else
		return m_js[jsIndex].lRz / (XF32)X_JOY_AXIS_EXTENT;
}



//*****************************************************************************
/**
Determines if a joystick button is currently pressed.

@param jsIndex	The joystick to query.  This value is 0 for joystick one, 1 for
				joystick two, 2 for joystick three, and so on.
@param	button The joystick button to query. 
@return True if the button is depressed, false othewise.
*/
//*****************************************************************************
XBOOL XInputMgr::joyButtonDown(XU32 jsIndex,X_JOY_BUTTON button)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return false;

	if (m_js[jsIndex].rgbButtons[button] & 0x80)
		return true;
	else 
		return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XInputMgr::joyButtonDownOnce(XU32 jsIndex,X_JOY_BUTTON button)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return false;

	if (m_jsButtonBufferDownOnce[jsIndex][button])
		return true;
	else 
		return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XInputMgr::joyButtonUpOnce(XU32 jsIndex,X_JOY_BUTTON button)
{
	if (jsIndex > X_numAttachedJoysticks-1)
		return false;

	if (m_jsButtonBufferUpOnce[jsIndex][button])
		return true;
	else 
		return false;
}



//*****************************************************************************
/**
Determines if any joystick buttons are currently depressed.

@return True if any mouse button is depressed, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::anyJoyButtonInput(XU32 jsIndex)
{

	if (jsIndex > X_numAttachedJoysticks-1)
		return false;

	//Check for joystick button press...
	for (XU32 i = 0;i < X_MAX_JOY_BUTTONS;i++)
		if (m_js[jsIndex].rgbButtons[i] != 0)
			return true;

	return false;
}



//*****************************************************************************
/**
Gets the mouse's last relative movement in screen space in the x direction. 

@return The current relative movement of the mouse in the x direction.
*/
//*****************************************************************************
XS32 XInputMgr::getMouseRelXMovement()
{
	return m_dims.lX;
}



//*****************************************************************************
/**
Gets the mouse's last relative movement in screen space in the y direction. 

@return The current relative movement of the mouse in the x direction.
*/
//*****************************************************************************
XS32 XInputMgr::getMouseRelYMovement()
{
	return m_dims.lY;
}



//*****************************************************************************
/**
Gets the mouse's last relative movement in screen space in the z direction. 

@return The current relative movement of the mouse in the z direction.
*/
//*****************************************************************************
XS32 XInputMgr::getMouseRelZMovement()
{
	return m_dims.lZ;
}



//*****************************************************************************
/**
Determines if a particular mouse button is currently depressed.

@param The mouse button to check.
@return True if the mouse button is depressed, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::mouseButtonDown(X_MOUSE_BUTTON button)
{
	switch (button)
	{
	case X_MB_LEFT:
		if (m_dims.rgbButtons[0] & 0x80)
			return true;
		break;
	case X_MB_RIGHT:
		if (m_dims.rgbButtons[1] & 0x80)
			return true;
		break;
	case X_MB_MIDDLE:
		if (m_dims.rgbButtons[2] & 0x80)
			return true;
		break;
	}

	//No button was depressed.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XInputMgr::mouseButtonDownOnce(X_MOUSE_BUTTON button)
{
	switch (button)
	{
	case X_MB_LEFT:
		if (m_mouseButtonBufferDownOnce[0])
			return true;
		break;
	case X_MB_RIGHT:
		if (m_mouseButtonBufferDownOnce[1])
			return true;
		break;
	case X_MB_MIDDLE:
		if (m_mouseButtonBufferDownOnce[2])
			return true;
		break;
	}

	//No button change to report.
	return false;
	
}



//*****************************************************************************
/**
Determines if a mouse button has been pressed.  This function will only return
true once per event but is affected by mouse repeat.  So, if the button is held
down for the mouse button repeat timeout or longer, it will return true as long
as the button is down.

@param 	The button to check.
@return True if the button is pressed once or is in repeat mode, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::mouseButtonDownOnceRepeat(X_MOUSE_BUTTON button)
{
	//First, if mouse button repeat is on and the button is depressed, then return true
	if (m_mouseRepeatOn && mouseButtonDown(button))
		return true;
	else
	{
		switch (button)
		{
		case X_MB_LEFT:
			if (m_mouseButtonBufferDownOnce[0])
				return true;
			break;
		case X_MB_RIGHT:
			if (m_mouseButtonBufferDownOnce[1])
				return true;
			break;
		case X_MB_MIDDLE:
			if (m_mouseButtonBufferDownOnce[2])
				return true;
			break;
		}
	}

	//No button change to report.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XInputMgr::mouseButtonUpOnce(X_MOUSE_BUTTON button)
{
	switch (button)
	{
	case X_MB_LEFT:
		if (m_mouseButtonBufferUpOnce[0])
			return true;
		break;
	case X_MB_RIGHT:
		if (m_mouseButtonBufferUpOnce[1])
			return true;
		break;
	case X_MB_MIDDLE:
		if (m_mouseButtonBufferUpOnce[2])
			return true;
		break;
	}

	//No button change to report.
	return false;
}



//*****************************************************************************
/**
Determines if any mouse buttons are currently depressed.

@return True if any mouse button is depressed, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::anyMouseButtonInput()
{
	//Check for mouse button press...
	for (XU32 i = 0;i < X_MAX_MOUSE_BUTTONS;i++)
		if (m_dims.rgbButtons[i] != 0)
			return true;

	return false;
}



//*****************************************************************************
/**
Determines if any keys on the keyboard are currently depressed.

@return True if any key is depressed, false if no keys are depressed.
*/
//*****************************************************************************
XBOOL XInputMgr::anyKeyboardInput()
{
	//Check for any key press...
	for (XU32 i = 0;i < X_MAX_KEYS;i++)
		if (m_keyboardBuffer[i] != 0)
			return true;

	return false;
}



//*****************************************************************************
/**
Determines if a key has been released from a depressed state.  This function
will only return true once per event.  In other words, when a key is released,
it only counts as input once.  

@param key	The key to check.
@return True if the key is released once, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::keyUpOnce(X_KEY key)
{
	if (m_keyboardBufferUpOnce[key])
		return true;
	else 
		return false;
}



//*****************************************************************************
/**
Determines if a key has been pressed.  This function will only return true once
per event.  In other words, when a key is pressed, it only counts as input once,
even if the key is held down.

@param key	The key to check.
@return True if the key is pressed once, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::keyDownOnce(X_KEY key)
{
	if (m_keyboardBufferDownOnce[key])
		return true;
	else 
		return false;
}



//*****************************************************************************
/**
Determines if a key has been pressed.  This function will only return true once
per event but is affected by key repeat.  So, if the key is held down for the
key repeat timeout or longer, it will return true as long as the key is down.

@param key	The key to check.
@return True if the key is pressed once or is in repeat mode, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::keyDownOnceRepeat(X_KEY key)
{
	//First, if key repeat is on and the key is depressed, then return true
	if (m_keyRepeatOn && keyDown(key))
		return true;
	else
	{
		if (m_keyboardBufferDownOnce[key])
			return true;
		else 
			return false;
	}
}



//*****************************************************************************
/**
Determines if a key is currently in the depressed state.  This function
will return true as long as the key is in the depressed state.  

@param key	The key to check.
@return True if the key is currently in the depressed state, false otherwise.
*/
//*****************************************************************************
XBOOL XInputMgr::keyDown(X_KEY key)
{
	if (KEYDOWN(key))
		return true;
	else 
		return false;
}



//*****************************************************************************
/**
Polls all initialized devices such as keyboards, mice, joysticks/gamepads and
takes a snapshot of the device states. This function is called by X-VP
to keep track of input device states.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputMgr::pollDevices()
{
	//Poll the keyboard and record a snapshot of the pressed keys if the
	//the keyboard has been aquired.
    if (m_keyboard)
	{
		m_hr = m_keyboard->GetDeviceState(sizeof(m_keyboardBuffer),&m_keyboardBuffer);
		if (m_hr != DI_OK)
		{
			//X_DEBUG_LOG_LN("Could not get keyboard device state.",X_DLL_WARNING);
			m_hr = m_keyboard->Acquire();
			if (m_hr != DI_OK)
				return X_ERR_OPERATION_FAILED;

			m_hr = m_keyboard->GetDeviceState(sizeof(m_keyboardBuffer),&m_keyboardBuffer);
			if (m_hr != DI_OK)
				return X_ERR_OPERATION_FAILED;
		}


		//Now we need to fill out the keyUpOnce and keyDownOnce buffers. First, we
		//zero out the keyup and keydown buffers.  Then, if the previous keyboard
		//snapshot and the current snapshot differ, then a key has transitioned
		//to another state.  Depending on the state, set the key in the key up once
		//or key down once buffers.
		for (XU32 i = 0;i < X_MAX_KEYS;i++)
		{
			m_keyboardBufferUpOnce[i] = 0;
			m_keyboardBufferDownOnce[i] = 0;
			
 			if (m_keyboardBuffer[i] ^ m_keyboardBufferOld[i])
			{
				if (m_keyboardBuffer[i])
				{
					m_keyRepeatTimer.reset();
					m_keyRepeatOn = false;
					m_keyboardBufferDownOnce[i] |= 0x80;
				}
				else
					m_keyboardBufferUpOnce[i] |= 0x80;
			}
			
			m_keyboardBufferOld[i] = m_keyboardBuffer[i];
		}
		X_DEBUG_LOG_LN("Successfully polled keyboard.",X_DLL_VERBOSE);
	}

	
	//Poll the mouse if it has been aquired.
	if (m_mouse)
	{
		m_hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE),&m_dims);
		if (m_hr != DI_OK)
		{
			m_mouse->Acquire();
			m_hr = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE),&m_dims);
			if (m_hr != DI_OK)
			{
				//Zero out mouse information
				memset(&m_dims,0,sizeof(m_dims));
				memset(m_mouseButtonBufferDownOnce,0,sizeof(m_mouseButtonBufferDownOnce));
				memset(m_mouseButtonBufferUpOnce,0,sizeof(m_mouseButtonBufferUpOnce));
				memset(m_mouseButtonBufferOld,0,sizeof(m_mouseButtonBufferOld));

				return X_ERR_NOT_READY;
			}
		}

		//Now we need to fill out the mouseUpOnce and mouseDownOnce buffers. First, we
		//zero out the mouseup and mousedown buffers.  Then, if the previous mouse
		//snapshot and the current snapshot differ, then a button has transitioned
		//to another state.  Depending on the state, set the button in the button up once
		//or button down once buffers.
		for (XU32 i = 0;i < X_MAX_MOUSE_BUTTONS;i++)
		{
			m_mouseButtonBufferUpOnce[i] = 0;
			m_mouseButtonBufferDownOnce[i] = 0;
			
 			if (m_dims.rgbButtons[i] ^ m_mouseButtonBufferOld[i])
			{
				if (m_dims.rgbButtons[i])
				{
					m_mouseRepeatTimer.reset();
					m_mouseRepeatOn = false;
					m_mouseButtonBufferDownOnce[i] |= 0x80;
				}
				else
					m_mouseButtonBufferUpOnce[i] |= 0x80;
			}
			
			m_mouseButtonBufferOld[i] = m_dims.rgbButtons[i];
		}

		X_DEBUG_LOG_LN("Successfully polled mouse.",X_DLL_VERBOSE);
	}

	//Poll the attatched joysticks if any have been aquired.
	for (XU32 i = 0;i < X_numAttachedJoysticks;i++)
	{
		m_hr = X_joysticks[i]->Poll();
		m_hr = X_joysticks[i]->GetDeviceState(sizeof(DIJOYSTATE2),&m_js[i]);
	    if (m_hr != DI_OK)
		{
			X_DEBUG_LOG_LN("Could not set joystick state.",X_DLL_WARNING);
			m_hr = X_joysticks[i]->Acquire();
			if (m_hr != DI_OK)
				return X_ERR_OPERATION_FAILED;

			m_hr = X_joysticks[i]->Poll();
			if (m_hr != DI_OK)
			{
				X_DEBUG_LOG_LN("Second attempt to aquire and poll the joystick state failed. Zeroing...",X_DLL_WARNING);
				memset(&m_js[i],0,sizeof(DIJOYSTATE2));
				return X_ERR_OPERATION_FAILED;
			}

			m_hr = X_joysticks[i]->GetDeviceState(sizeof(DIJOYSTATE2),&m_js[i]);
			if (m_hr != DI_OK)
				return X_ERR_OPERATION_FAILED;
		}

		X_DEBUG_LOG_LN("Successfully polled joystick.",X_DLL_VERBOSE);
	}

	//Now we need to fill out the joystick button up and down once buffers. First, we
	//zero out the joy up and joy down buffers.  Then, if the previous joystick
	//snapshot and the current snapshot differ, then a button has transitioned
	//to another state.  Depending on the state, set the button in the button up once
	//or button down once buffers.
	for (XU32 i = 0;i < X_numAttachedJoysticks;i++)
		for (XU32 j = 0;j < X_MAX_JOY_BUTTONS;j++)
		{
			m_jsButtonBufferUpOnce[i][j] = 0;
			m_jsButtonBufferDownOnce[i][j] = 0;
			
 			if (m_js[i].rgbButtons[j] ^ m_jsButtonBufferOld[i][j])
			{
				if (m_js[i].rgbButtons[j])
					m_jsButtonBufferDownOnce[i][j] |= 0x80;
				else
					m_jsButtonBufferUpOnce[i][j] |= 0x80;
			}
			
			m_jsButtonBufferOld[i][j] = m_js[i].rgbButtons[j];
		}

	//Everything went ok.
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext)
{
	HRESULT m_hr;
	
    // Obtain an interface to the enumerated joystick.
	if (X_numAttachedJoysticks < X_MAX_JOYSTICKS)
	{
		m_hr = X_DI->CreateDevice(pdidInstance->guidInstance,&(X_joysticks[X_numAttachedJoysticks]),NULL);
		X_numAttachedJoysticks++;
	}
	else
		return DIENUM_STOP;
    
	//Continue enumerating joysticks.
    return DIENUM_CONTINUE;
}



//*****************************************************************************
//*****************************************************************************
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext)
{
    DIPROPRANGE diprg; 
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYID; 
    diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
    diprg.lMin              = -X_JOY_AXIS_EXTENT; 
    diprg.lMax              = X_JOY_AXIS_EXTENT; 
    
	// Set the range for the axis
	XU32 i = *((XU32*)pContext);
	if(FAILED( X_joysticks[i]->SetProperty(DIPROP_RANGE, &diprg.diph ) ) )
		return DIENUM_STOP;

    return DIENUM_CONTINUE;
}



//*****************************************************************************
//*****************************************************************************
XStatus XInputMgr::initLuaGlue()
{
	//Add input methods
	XLua::registerLibrary(X_INPUT_LUA_LIB_NAME,XInputLuaLib);

	return X_OK;
}