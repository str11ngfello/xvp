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
//	Module Name:	XTCPListener.h
//	Author:			Lynn Duke
//	Creation Date:	2-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TCP_LISTENER_H__
#define __X_TCP_LISTENER_H__

#include <XOSMgr.h>
#include <XNetGlobals.h>
#include <XResource.h>
#include <XBaseTCPListener.h>


DWORD WINAPI X_TCPListenerPumper(LPVOID pParam);


//*****************************************************************************
//! TCP Listener.
//*****************************************************************************
class XTCPListener : public XBaseTCPListener
{
private:

	SOCKET					m_listenSocket;
	bool					m_initialized;
	bool					m_listenerThreadOn;
	HANDLE					m_closeEvent;	
	HANDLE					m_listenerThreadHandle; 
	ULONG32					m_listenerThreadID;
	void*					m_pUserData;
	X_NET_MSG_ACTION_TYPE	(*m_eventCallback) (X_NET_MSG_TYPE msgType,
												XNetAddress& clientAddress,
												XHandle tcpConnHandle,
												void* pUserData);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTCPListener();
	~XTCPListener();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus init(XNetAddress& listenAddress,X_NET_MSG_ACTION_TYPE (*eventCallback)
																  (X_NET_MSG_TYPE msgType,
																   XNetAddress& clientAddress,
																   XHandle tcpConnHandle,
																   void* pUserData),
																   void* pUserData);
	XStatus destroy();
	void listenPump();
};


#endif