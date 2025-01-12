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
//	Module Name:	XInputActionMap.h
//	Author:			Lynn Duke
//	Creation Date:	12-21-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_INPUT_ACTION_MAP_H__
#define __X_INPUT_ACTION_MAP_H__





enum ACTION_MAP_DEVICE_TYPE
{
	X_AMDT_KEY,
	X_AMDT_MOUSE_BUTTON,
	X_AMDT_JOY_BUTTON,
	X_AMDT_JOY_X_AXIS,
	X_AMDT_JOY_Y_AXIS,
	X_AMDT_JOY_Z_AXIS,
	X_AMDT_JOY_ZROLL_AXIS
};


enum ACTION_MAP_NOTIFY_TYPE
{
	X_AMNT_CONTINUOUS,
	X_AMNT_DISCRETE
};
 

struct XAction
{
	XS32 actionID;
	XU8 deviceType;
	XU8 deviceTrigger;
	XU8 notifyType;
	XF32 actionValue;
};



//*****************************************************************************
//! Abstracts device input from logical actions.
/**
Action maps provide a layer of abstraction between code that uses
input devices and how those input devices are actually mapped to conceptual
data/actions in your application. 

For example, you may have a character in your game that can walk around in a
3D level.  Now, you could just check for the x axis and y axis on the
joystick and move him appropriately.  This would work, but the movement
of your character now is hardcoded relative to the input of the x and y axis.

Now, imagine that in your application, the user wants to control the character
with the keyboard instead, or maybe with a different axis on the joystick?
This is where action maps attempt to simplify things.

You could define an action map that gets input from the keyboard and another
action map to get input from various joystick functions.  The code that
actually moves your character would no longer query individual devices, but
instead query the action map to see which actions have occurred.
*/
//*****************************************************************************
class XInputActionMap : public XActionMap
{
private:
	XAction*		m_pActionSet;	
	XU32			m_numActions;
	XU32			m_joystickIndex;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XInputActionMap();
	virtual ~XInputActionMap();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XAction* pActionSet);
	XStatus destroy();
	virtual XStatus update();
	virtual XF32 getActionValue(XU32 actionID);
	XStatus load(XS8* pFilename);
	XStatus save(XS8* pFilename);
	XStatus setJoystickIndex(XU32 index);
	ACTION_MAP_DEVICE_TYPE getActionDeviceType(XU32 actionID);
	XU8 getActionDeviceTrigger(XU32 actionID);
	ACTION_MAP_NOTIFY_TYPE getActionNotifyType(XU32 actionID);
	XStatus setActionDeviceType(XU32 actionID,ACTION_MAP_DEVICE_TYPE deviceType);
	XStatus setActionDeviceTrigger(XU32 actionID,XU8 deviceTrigger);
	XStatus setActionNotifyType(XU32 actionID,ACTION_MAP_NOTIFY_TYPE notifyType);
	XStatus setAction(XU32 actionID,ACTION_MAP_DEVICE_TYPE deviceType,
					  XU8 deviceTrigger,ACTION_MAP_NOTIFY_TYPE notifyType);
};


#endif