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
//	Module Name:	XBaseTCPConnection.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_TCP_CONNECTION_H__
#define __X_BASE_TCP_CONNECTION_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XNetAddress.h>



//*****************************************************************************
//! Base TCP Connection.
//*****************************************************************************
class XBaseTCPConnection : public XResource
{
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseTCPConnection();
	virtual ~XBaseTCPConnection();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus init(XNetAddress& localAddress,XNetAddress& remoteAddress) = 0;
	virtual XStatus destroy() = 0;
	virtual XStatus sendData(XS8* pData,XU32 dataLength,XU32& sentDataLength) = 0;
	virtual XStatus receiveData(XS8* pData,XU32 dataLength,XU32& returnDataLength) = 0;
	virtual XStatus setBlockingState(bool blocking) = 0;
	virtual XStatus setBroadcast(bool broadcast) = 0;
};


#endif