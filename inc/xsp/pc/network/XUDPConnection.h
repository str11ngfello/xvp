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
//	Module Name:	XUDPConnection.h
//	Author:			Lynn Duke
//	Creation Date:	2-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UDP_CONNECTION_H__
#define __X_UDP_CONNECTION_H__

#include <XOSMgr.h>
#include <XNetGlobals.h>
#include <XResource.h>
#include <XBaseUDPConnection.h>




//*****************************************************************************
//! UDP Connection.
//*****************************************************************************
class XUDPConnection : public XBaseUDPConnection
{
private:

	SOCKET		sock;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUDPConnection();
	~XUDPConnection();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus init(XNetAddress& localAddress);
	XStatus destroy();
	XStatus sendData(XS8* pData,XU32 dataLength,XU32& sentDataLength,
					XNetAddress& toAddress);
	XStatus receiveData(XS8* pData,XU32 dataLength,
									XU32& returnDataLength,XNetAddress& fromAddress);
	XStatus setBlockingState(bool blocking);
	XStatus setBroadcast(bool broadcast);

};


#endif