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
//	Module Name:	XBaseUDPConnection.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_UDP_CONNECTION_H__
#define __X_BASE_UDP_CONNECTION_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XNetAddress.h>



//*****************************************************************************
//! Base UDP Connection.
//*****************************************************************************
class XBaseUDPConnection : public XResource
{
protected:


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseUDPConnection();
	virtual ~XBaseUDPConnection();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus init(XNetAddress& localAddress) = 0;
	virtual XStatus destroy() = 0;
	virtual XStatus sendData(XS8* pData,XU32 dataLength,XU32& sentDataLength,
					XNetAddress& toAddress) = 0;
	virtual XStatus receiveData(XS8* pData,XU32 dataLength,
									XU32& returnDataLength,XNetAddress& fromAddress) = 0;
	virtual XStatus setBlockingState(bool blocking) = 0;
	virtual XStatus setBroadcast(bool broadcast) = 0;

};


#endif