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
//	Module Name:	XState.h
//	Author:			Lynn Duke
//	Creation Date:	7-11-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_STATE_H__
#define __X_STATE_H__




//*****************************************************************************
//! Base object for application states.
/**
The XState object provides the base interface and functionality required for
X-VP application states.  The user derives custom application states from this
object and then overrides its methods to provide their own application
functionality.  There must be at least one state in every X-VP application, but
typically, many states are involved.

See the member functions below for more information.
*/
//*****************************************************************************
class XState
{
private:
	XBOOL m_initialized;

protected:
	XState();
	
public:
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus preActivate(void* pUserData);
	virtual XStatus init();
	virtual XStatus destroy();
	virtual XStatus activate(void* pUserData);
	virtual XStatus deactivate();
	virtual XStatus pause();
	virtual XStatus resume();
	virtual XStatus update(XF32 dt);
	virtual XS8* getStateDesc();
};


#endif