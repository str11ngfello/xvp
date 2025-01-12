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
//	Module Name:	XTCPConnection.h
//	Author:			Lynn Duke
//	Creation Date:	2-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TCP_CONNECTION_H__
#define __X_TCP_CONNECTION_H__

#include <XOSMgr.h>
#include <XNetGlobals.h>
#include <XResource.h>
#include <XBaseTCPConnection.h>




//*****************************************************************************
//! TCP Connection.
//*****************************************************************************
class XTCPConnection : public XBaseTCPConnection
{
private:

	SOCKET		m_socket;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTCPConnection();
	~XTCPConnection();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus init(XNetAddress& localAddress,XNetAddress& remoteAddress);
	XStatus destroy();
	XStatus sendData(XS8* pData,XU32 dataLength,XU32& sentDataLength);
	XStatus receiveData(XS8* pData,XU32 dataLength,XU32& returnDataLength);
	XStatus setBlockingState(bool blocking);
	XStatus setBroadcast(bool broadcast);
	XStatus setSocket(SOCKET sock);
};


#endif