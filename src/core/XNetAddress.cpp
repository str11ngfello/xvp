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
//	Module Name:	XNetAddress.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-27-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
The default constructor for the network address.  This constructor will
initialize the port number to 0 and the IP address to 0.
*/
//*****************************************************************************
XNetAddress::XNetAddress()
{
	m_port = 0;
	m_numIpAddress = 0;
	m_strIpAddress[0] = '\0';
}



//*****************************************************************************
/**
This constructor allows you to initialize the IP address with a null terminated
string such as "192.168.10.1" and numeric port number.
*/
//*****************************************************************************
XNetAddress::XNetAddress(XS8* pIpAddress,int port)
{
	//Now let's take a look at the ip address.
	if (pIpAddress)
	{
		if (strlen(pIpAddress) <= X_NET_IP_ADDR_SIZE-1)
		{
			strcpy(m_strIpAddress,pIpAddress);
            m_numIpAddress = ntohl(inet_addr(pIpAddress));
		}
	}
	else
	{
		//There was not ip address passed in so set it to 0
		m_strIpAddress[0] = '\0';
		m_numIpAddress = 0;
	}
	
	m_port = port;
}



//*****************************************************************************
/**
Sets the IP address in the network address.

@param pIpAddress The IP address in string format.  For example "10.10.5.2".
@return None.
*/
//*****************************************************************************
void XNetAddress::setIpAddressStr(XS8* pIpAddress)
{
	if (pIpAddress)
	{
		if (strlen(pIpAddress) <= X_NET_IP_ADDR_SIZE-1)
		{
			strcpy(m_strIpAddress,pIpAddress);
            m_numIpAddress = ntohl(inet_addr(pIpAddress));
		}
	}
}



//*****************************************************************************
/**
Sets the IP address in the network address.

@param ipAddress	The ip address in 32 bit unsigned integer format.  For
					example, to set an ip address of "255.254.253.252" the value
					for this parameter should be 0xFFFEFDFC.
@return None.
*/
//*****************************************************************************
void XNetAddress::setIpAddressNum(XU32 ipAddress)
{ 
	m_numIpAddress = ipAddress;
	in_addr a;
	a.S_un.S_addr = htonl(m_numIpAddress);
	strcpy(m_strIpAddress,inet_ntoa(a));
}



//*****************************************************************************
/**
Sets the port number for the network address.

@param port	The port number.  This value should be non-negative.
@return None.
*/
//*****************************************************************************
void XNetAddress::setPort(int port)
{
	m_port = port;
}



//*****************************************************************************
/**
Gets the IP address in string format.

@return A pointer to a null terminated string representing the IP address of 
		the network address.
*/
//*****************************************************************************
XS8* XNetAddress::getStrIpAddress()
{
	return m_strIpAddress;
}



//*****************************************************************************
/**
Gets the IP address in unsigned integer format.  

@return The IP address in unsigned integer format.  For example, if the IP address
		of the network address is "255.254.253.252", the return value will be
		0xFFFEFDFC.
*/
//*****************************************************************************
XU32 XNetAddress::getNumIpAddress()
{
	return m_numIpAddress;
}



//*****************************************************************************
/**
Gets the port number of the IP address.

@return The port number of the IP address.
*/
//*****************************************************************************
int XNetAddress::getPort()
{
	return m_port;
}



//*****************************************************************************
/**
Clears the network address information by setting the IP address and port 
number to 0.

@return None.
*/
//*****************************************************************************
void XNetAddress::clear()
{
	m_strIpAddress[0] = '\0';
	m_numIpAddress = 0;
	m_port = 0;
}