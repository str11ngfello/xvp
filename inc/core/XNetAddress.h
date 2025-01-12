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
//	Module Name:	XNetAddress.h
//	Author:			Lynn Duke
//	Creation Date:	8-27-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NET_ADDRESS_H__
#define __X_NET_ADDRESS_H__



#define X_NET_IP_ADDR_SIZE		16		//255.255.255.255 + NULL char



//*****************************************************************************
//! Represents a network address consisting of an IP address and port number.
/**
The XNetAddress object represents a network address consisting of a 32 bit
IP address and a port number.       

See the member functions below for more information.
*/
//*****************************************************************************
class XNetAddress
{					  
private:
	char			m_strIpAddress[X_NET_IP_ADDR_SIZE];
	XU32			m_numIpAddress;
	int				m_port;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XNetAddress();
	XNetAddress(XS8* pIpAddress,int port);

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	void setIpAddressStr(XS8* pIpAddress);
	void setIpAddressNum(XU32 ipAddress);
	void setPort(int port);
	XS8* getStrIpAddress();
	XU32 getNumIpAddress();
	int getPort();
	void clear();
};

#endif