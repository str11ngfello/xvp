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
//	Module Name:	XBaseTCPListener.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_TCP_LISTENER_H__
#define __X_BASE_TCP_LISTENER_H__

#include <XOSMgr.h>
#include <XNetGlobals.h>
#include <XNetAddress.h>
#include <XResource.h>



//*****************************************************************************
//! Base TCP Listener.
//*****************************************************************************
class XBaseTCPListener : public XResource
{
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseTCPListener();
	virtual ~XBaseTCPListener();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus init(XNetAddress& listenAddress,X_NET_MSG_ACTION_TYPE (*eventCallback)
																  (X_NET_MSG_TYPE msgType,
																   XNetAddress& clientAddress,
																   XHandle tcpConnHandle,
																   void* pUserData),
																   void* pUserData) = 0;
	virtual XStatus destroy() = 0;
};


#endif