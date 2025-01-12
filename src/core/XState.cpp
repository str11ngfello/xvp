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
//	Module Name:	XState.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-11-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the state.
*/
//*****************************************************************************
XState::XState()
{
	m_initialized = false;
}



//*****************************************************************************
/**
Initializes the state by performing one time initialization tasks.
If the state is registered in the engine, and this method has not
yet been called, the engine will call this method before making any update
calls.  In other words, update calls will not happen until this function has
been called, either manually or by the engine.  

In most cases, you should call this function before registering the state
with the engine.  For example, if the function performs any time intensive
setup tasks, such as loading in bitmaps from disk, you can call this function
manually when it is convenient for your application.  Then when activation of
the state occurs later, the initialization has already been done and the
state is ready for activation.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::init()
{
	m_initialized = true;
	return X_OK;
}



//*****************************************************************************
/**
Destroys the state by freeing any resources allocated during the init method.
This method is never called by the engine.  Instead, it's up to your application
to decide when it's convenient to call this method.  

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::destroy()
{
	m_initialized = false;
	return X_OK;
}



//*****************************************************************************
/**
Prepares the state to be activated.  This function is called by the engine
automatically and ensures that the state has been initialized.  

@param pUserData	User data that is passed from the previous state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::preActivate(void* pUserData)
{
	if (!m_initialized)
	{
		m_initialized = true;
		init();
	}

	return activate(pUserData);
}



//*****************************************************************************
/**
Activates the state. Once the state has been activated, it will
begin receiving update calls from the engine.  This function is called once
for each time it is registered to the engine to begin execution.

@param pUserData	User data passed in from the previous state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::activate(void* pUserData)
{
	return X_OK;
}



//*****************************************************************************
/**
Deactivates the state.  This function is called automatically by the engine
when the state is deactivated.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::deactivate()
{
	return X_OK;
}



//*****************************************************************************
/**
Pauses the state.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::pause()
{
	return X_OK;
}



//*****************************************************************************
/**
Resumes a paused state.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::resume()
{
	return X_OK;
}



//*****************************************************************************
/**
Updates the state.  Once the state has been registered with the engine,
initialized and activated, this function is called once per frame and allows the
state to update any internal, time-dependent data.  Once the state has been
unregistered and deactivated, it no longer receives update calls.

@param dt	The amount of time, in seconds, that has passed since the
					last update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XState::update(XF32 dt)
{
	return X_OK;
}



//*****************************************************************************
/**
Gets a string description of the state.

@return A pointer to a null terminated description string.
*/
//*****************************************************************************
XS8* XState::getStateDesc()
{
	static char description[X_STATE_DESC_LEN] = "UNKNOWN STATE";
	return description;
}


