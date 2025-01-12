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
//	Module Name:	XInputActionMap.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-21-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>
 


//*****************************************************************************
/**
Constructor for the action map.
*/
//*****************************************************************************
XInputActionMap::XInputActionMap()
{
	m_pActionSet = NULL;
	m_numActions = 0;
	m_joystickIndex = 0;
}



//*****************************************************************************
/**
Destructor for the action map.
*/
//*****************************************************************************
XInputActionMap::~XInputActionMap()
{
}


//*****************************************************************************
/**
Initializes an action map with an action set.  This method must be called
before the action map will function correctly.

@param pActionSet	The action set to initialize the action map with.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::init(XAction* pActionSet)
{
	//Validate action set
	if (!pActionSet)
		return X_ERR_INVALID_PARAMS;

	//Count the number of actions in the action set
	XAction* pAs = pActionSet;
	while (pAs->actionID != -1)
	{
		++m_numActions;
		++pAs;
	}
	
	//If there are no actions to create, just return.
	if (m_numActions == 0)
		return X_ERR_OPERATION_FAILED;

	//If there is a previous action set, delete it.
	XDeleteArray(m_pActionSet);

	//Allocate new action set
	XNewArray(m_pActionSet,XAction,m_numActions,XOSMgr::getDefaultHeap());
    
	//Fill in the action set
	memcpy(m_pActionSet,pActionSet,sizeof(XAction)*m_numActions);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XInputActionMap::destroy()
{
	XDeleteArray(m_pActionSet);
	m_numActions = 0;
	return X_OK;
}



//*****************************************************************************
/**
Sets the joystick index to use with any actions in the action map that reference
a joystick.

@param index	The index of the joystick to use for joystick actions.  0 for the
				first attached joystick, 1 for the second, etc...
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::setJoystickIndex(XU32 index)
{
	m_joystickIndex = index;
	return X_OK;
}



//*****************************************************************************
/**
Loads in an action map from a file.   This method can be used in place of the 
init() method.

@param pFilename	The filename containing the action map to load.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::load(XS8* pFilename)
{
	//Validate filename
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;
	
	FILE* pFile = fopen(pFilename,"r");
    
	//If there are no actions to create, just return.
	fscanf(pFile,"%d\n",&m_numActions);
	if (m_numActions == 0)
		return X_ERR_OPERATION_FAILED;

	//If there is a previous action set, delete it.
	XDeleteArray(m_pActionSet);

	//Allocate new action set
	XNewArray(m_pActionSet,XAction,m_numActions,XOSMgr::getDefaultHeap());	

	for (XU32 i = 0;i < m_numActions;++i)
	{
		fscanf(pFile,"%d %d %d %d\n",&m_pActionSet[i].actionID,
									 &m_pActionSet[i].deviceType,
									 &m_pActionSet[i].deviceTrigger,
                                     &m_pActionSet[i].notifyType);
		m_pActionSet[i].actionValue = 0;
	}
	fclose(pFile);

	return X_OK;
}



//*****************************************************************************
/**
Saves the action map to a file. 

@param pFilename	The filename to save the action map to.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::save(XS8* pFilename)
{
	//If there is an action set to save, and the filename
	//is valid...
	if ((m_pActionSet) && (pFilename))
	{
		FILE* pFile = fopen(pFilename,"w");
		fprintf(pFile,"%d\n",m_numActions);
		for (XU32 i = 0;i < m_numActions;++i)
		{
			fprintf(pFile,"%d %d %d %d\n",m_pActionSet[i].actionID,
										  m_pActionSet[i].deviceType,
										  m_pActionSet[i].deviceTrigger,
										  m_pActionSet[i].notifyType);
		}

		fclose(pFile);

		return X_OK;
	}

	return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Updates the action map.  This method should be called by the application 
each time step before it is queried for which action have occurred.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::update()
{
	for (XU32 i = 0;i < m_numActions;++i)
	{
		switch(m_pActionSet[i].deviceType)
		{
		m_pActionSet[i].actionValue = 0;
		case X_AMDT_KEY:
			switch(m_pActionSet[i].notifyType)
			{
			case X_AMNT_CONTINUOUS:
				m_pActionSet[i].actionValue = XVP::InputMgr.keyDown((X_KEY)m_pActionSet[i].deviceTrigger);
				break;
			case X_AMNT_DISCRETE:
				m_pActionSet[i].actionValue = XVP::InputMgr.keyDownOnce((X_KEY)m_pActionSet[i].deviceTrigger);
				break;
			}
			break;
		case X_AMDT_MOUSE_BUTTON:
			m_pActionSet[i].actionValue = 0;
			switch(m_pActionSet[i].notifyType)
			{
			case X_AMNT_CONTINUOUS:
				m_pActionSet[i].actionValue = XVP::InputMgr.mouseButtonDown((X_MOUSE_BUTTON)m_pActionSet[i].deviceTrigger);
				break;
			case X_AMNT_DISCRETE:
				m_pActionSet[i].actionValue = XVP::InputMgr.mouseButtonDownOnce((X_MOUSE_BUTTON)m_pActionSet[i].deviceTrigger);
				break;
			}
			break;
		case X_AMDT_JOY_BUTTON:
			m_pActionSet[i].actionValue = 0;
			switch(m_pActionSet[i].notifyType)
			{
			case X_AMNT_CONTINUOUS:
				m_pActionSet[i].actionValue = XVP::InputMgr.joyButtonDown(m_joystickIndex,
																	  (X_JOY_BUTTON)m_pActionSet[i].deviceTrigger);
				break;
			case X_AMNT_DISCRETE:
				m_pActionSet[i].actionValue = XVP::InputMgr.joyButtonDownOnce(m_joystickIndex,
																		   (X_JOY_BUTTON)m_pActionSet[i].deviceTrigger);
				break;
			}
			break;
		case X_AMDT_JOY_X_AXIS:
			m_pActionSet[i].actionValue = XVP::InputMgr.getJoyXExtent(m_joystickIndex);
			break;
		case X_AMDT_JOY_Y_AXIS:
			m_pActionSet[i].actionValue = XVP::InputMgr.getJoyYExtent(m_joystickIndex);
			break;
		case X_AMDT_JOY_Z_AXIS:
			m_pActionSet[i].actionValue = XVP::InputMgr.getJoyZExtent(m_joystickIndex);
			break;
		case X_AMDT_JOY_ZROLL_AXIS:
			m_pActionSet[i].actionValue = XVP::InputMgr.getJoyZRollExtent(m_joystickIndex);
			break;
		}
	}

	return X_OK;
}



//*****************************************************************************
/**
Queries the action map to determine if a specific action has occurred.  

@param actionID	The user defined action ID from the action set.
@return The value of the action.  For boolean actions this value is 1 for true,
		and 0 for false.
*/
//*****************************************************************************
XF32 XInputActionMap::getActionValue(XU32 actionID)
{
	if (actionID >= m_numActions)
		return 0;
	else
		return m_pActionSet[actionID].actionValue;
}



//*****************************************************************************
/**
Modifies an action in the current action map.

@param actionID	An ID used to uniquely identify an action within the 
				action map.
@param deviceType	The type of device the action refers to.
@param deviceTrigger	The trigger on the device that causes the action.
@param notifyType	The type of notification the action uses.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::setAction(XU32 actionID,ACTION_MAP_DEVICE_TYPE deviceType,
								   XU8 deviceTrigger,ACTION_MAP_NOTIFY_TYPE notifyType)
{
	if (actionID >= m_numActions)
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_pActionSet[actionID].deviceType = deviceType;
		m_pActionSet[actionID].deviceTrigger = deviceTrigger;
		m_pActionSet[actionID].notifyType = notifyType;

		return X_OK;
	}
}



//*****************************************************************************
/**
Gets the device type associated with a particular action in the action map.

@param actionID	The action for which to retrieve the device type.
@return The actions device type.
*/
//*****************************************************************************
ACTION_MAP_DEVICE_TYPE XInputActionMap::getActionDeviceType(XU32 actionID)
{
	if (actionID >= m_numActions)
		return (ACTION_MAP_DEVICE_TYPE)0;
	else
		return (ACTION_MAP_DEVICE_TYPE)m_pActionSet[actionID].deviceType;
}



//*****************************************************************************
/**
Gets the device trigger associated with a particular action in the action map.

@param actionID	The action for which to retrieve the device trigger.
@return The actions device trigger.
*/
//*****************************************************************************
XU8 XInputActionMap::getActionDeviceTrigger(XU32 actionID)
{
	if (actionID >= m_numActions)
		return 0;
	else
		return m_pActionSet[actionID].deviceTrigger;
}



//*****************************************************************************
/**
Gets the notification type associated with a particular action in the action map.

@param actionID	The action for which to retrieve the notification type.
@return The action's notification type.
*/
//*****************************************************************************
ACTION_MAP_NOTIFY_TYPE XInputActionMap::getActionNotifyType(XU32 actionID)
{
	if (actionID >= m_numActions)
		return (ACTION_MAP_NOTIFY_TYPE)0;
	else
		return (ACTION_MAP_NOTIFY_TYPE)m_pActionSet[actionID].notifyType;
}



//*****************************************************************************
/**
Sets the device type associated with a particular action in the action map.

@param actionID	The action for which to set the device type.
@param deviceType	The device type to associate the action with.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::setActionDeviceType(XU32 actionID,ACTION_MAP_DEVICE_TYPE deviceType)
{
	if (actionID >= m_numActions)
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_pActionSet[actionID].deviceType = deviceType;
		return X_OK;
	}
}



//*****************************************************************************
/**
Sets the device trigger associated with a particular action in the action map.

@param actionID	The action for which to set the device trigger.
@param deviceTrigger	The device trigger to associate the action with.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::setActionDeviceTrigger(XU32 actionID,XU8 deviceTrigger)
{
	if (actionID >= m_numActions)
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_pActionSet[actionID].deviceTrigger = deviceTrigger;
		return X_OK;
	}
}



//*****************************************************************************
/**
Sets the notification type associated with a particular action in the action map.

@param actionID	The action for which to set the device trigger.
@param notifyType	The notification type to associate the action with.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XInputActionMap::setActionNotifyType(XU32 actionID,ACTION_MAP_NOTIFY_TYPE notifyType)
{
	if (actionID >= m_numActions)
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_pActionSet[actionID].notifyType = notifyType;
		return X_OK;
	}
}


