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
//	Module Name:	XNetworkMgr.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NETWORK_MGR_H__
#define __X_NETWORK_MGR_H__

#include <XOSMgr.h>
#include <XNetGlobals.h>
#include <XTCPListener.h>
#include <XResourceList.h>
#include <XVPDesc.h>




//*****************************************************************************
//! Network Driver and resource manager.
//*****************************************************************************
class XNetworkMgr
{
private:
	//SOCKET*					m_UDPConnectionArray;
	//XU32					m_maxUDPConnections;
 	//XU32					m_numAllocatedUDPConnections;
	XResourceList			m_UDPConnections;
	CRITICAL_SECTION		m_UDPConnectionCS;

	XResourceList			m_TCPConnections;
	CRITICAL_SECTION		m_TCPConnectionCS;

	XResourceList			m_TCPListeners;
	CRITICAL_SECTION		m_TCPListenerCS;

	bool					m_inited;

	//*****************************************************************************
	//Private Interface.
	//*****************************************************************************
	XStatus addTCPConnection(XHandle& connHandle,SOCKET sock);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XNetworkMgr();
	~XNetworkMgr();

	//*****************************************************************************
	//Standard XNetworkMgr Functions
	//*****************************************************************************
	XStatus init(XVPDesc& initDesc);
	XStatus update(XF32 dt);
	XStatus destroy();
	XStatus createUDPConnection(XNetAddress& localAddress,XHandle& ncHandle);
	XStatus createTCPConnection(XNetAddress& localAddress,XNetAddress& remoteAddress,
								XHandle& ncHandle);
	XStatus createTCPListener(XNetAddress& listenAddress,X_NET_MSG_ACTION_TYPE (*eventCallback)
																			(X_NET_MSG_TYPE msgType,
																			 XNetAddress& clientAddress,
																			 XHandle tcpConnHandle,
																			 void* pUserData),
																			 void* pUserData,
																			 XHandle& lHandle);
	XStatus destroyUDPConnection(XHandle ncHandle);
	XStatus destroyTCPConnection(XHandle ncHandle);
	XStatus destroyTCPListener(XHandle lHandle);
	XStatus destroyAllUDPConnections();
	XStatus destroyAllTCPConnections();
	XStatus destroyAllTCPListeners();
	XStatus sendUDPData(XHandle ncHandle,XS8* pData,XU32 dataLength,XU32& sentDataLength,
						XNetAddress& toAddress);
	XStatus receiveUDPData(XHandle ncHandle,XS8* pData,XU32 dataLength,XU32& returnDataLength,
						XNetAddress& fromAddress);
	XStatus sendTCPData(XHandle ncHandle,XS8* pData,XU32 dataLength,XU32& sentDataLength);
	XStatus receiveTCPData(XHandle ncHandle,XS8* pData,XU32 dataLength,XU32& returnDataLength);
	XStatus setUDPBlockingState(XHandle ncHandle,bool blocking);
	XStatus setTCPBlockingState(XHandle ncHandle,bool blocking);
	XStatus setUDPBroadcast(XHandle ncHandle,bool allowBroadcast);
	XStatus setTCPBroadcast(XHandle ncHandle,bool allowBroadcast);
	XStatus addTCPConnectionRef(XHandle ncHandle);
	XStatus addUDPConnectionRef(XHandle ncHandle);
	XStatus addTCPListenerRef(XHandle ncHandle);

	friend XTCPListener;
};

#endif