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
//	Module Name:	XTCPConnection.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XTCPConnection::XTCPConnection()
{
	m_socket = INVALID_SOCKET;
}



//*****************************************************************************
//*****************************************************************************
XTCPConnection::~XTCPConnection()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::init(XNetAddress& localAddress,XNetAddress& remoteAddress)
{
	//Try and create the TCP socket
	if((m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		X_ASSERT_FORCED("Could not create TCP connection.");
		return X_ERR_OPERATION_FAILED;
	}
	else
	{
		struct sockaddr_in address;
		memset(&address,0,sizeof(sockaddr_in));

		//If no IP address is specified, assume the default.
		if (localAddress.getNumIpAddress() == 0)
			address.sin_addr.s_addr = htonl(INADDR_ANY);
		else
			address.sin_addr.s_addr = htonl(localAddress.getNumIpAddress());

		//If 0 is specified for the port number, choose an available port number
		if (localAddress.getPort() == 0)
			address.sin_port = 0;
		else
			address.sin_port = htons((short)localAddress.getPort());
		
		address.sin_family = AF_INET;

		//Bind the address to the socket
		if(bind(m_socket,(struct sockaddr*)&address,sizeof(address)) == -1)
		{
			X_ASSERT_FORCED("Could not open local TCP socket.");
			return X_ERR_OPERATION_FAILED;
		}
				
		//Now try to connect to the remote address.
		memset(&address,0,sizeof(sockaddr_in));

		//Fill in the remote address.
		address.sin_addr.s_addr = htonl(remoteAddress.getNumIpAddress());
		address.sin_port = htons((short)remoteAddress.getPort());
		address.sin_family = AF_INET;
		
		//Try to connect
		if (connect(m_socket,(struct sockaddr*)&address,sizeof(address)) == 0)
			return X_OK;
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::destroy()
{
#ifdef XSP_WINDOWS
	closesocket(m_socket);
#else
	close(m_socket);
#endif

	XResource::destroy();
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::sendData(XS8* pData,XU32 dataLength,XU32& sentDataLength)
{
	sentDataLength = send(m_socket,(XS8*)pData,dataLength,0);
			
	if(sentDataLength == -1)
	{
		sentDataLength = 0;
#ifdef XSP_WINDOWS
		int status = WSAGetLastError();			
		if(status == WSAEWOULDBLOCK)
			return X_OK;
		else if ((status == WSAECONNRESET) || (status == WSAENETRESET)
					|| (status == WSAECONNABORTED))
			return X_ERR_NOT_RUNNING;
#else
		if(errno != EWOULDBLOCK)
			return X_OK;
#endif
		return X_ERR_OPERATION_FAILED;
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::receiveData(XS8* pData,XU32 dataLength,XU32& returnDataLength)
{
	returnDataLength = 0;
	int numRecvChars = recv(m_socket,(XS8*)pData,dataLength,0);
	if (numRecvChars == -1)
	{
		int status = WSAGetLastError();
		if (status == WSAEWOULDBLOCK)
			return X_OK;
		else if ((status == WSAECONNRESET) || (status == WSAENETRESET)
					|| (status == WSAECONNABORTED))
			return X_ERR_NOT_RUNNING;
		else
			return X_ERR_OPERATION_FAILED;
	}
	
	//Check to see if the connection has been closed.
	if (numRecvChars == 0)
		return X_ERR_NOT_RUNNING;
			
	//Connection was open and we received some data.
	returnDataLength = numRecvChars;
				
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::setBlockingState(XBOOL blocking)
{
#ifdef XSP_WINDOWS
	DWORD inParam;
	DWORD outParam;
	ULONG32 bytesReturned;
	if (blocking)
		inParam = 0;
	else
		inParam = 1;
			
	if (WSAIoctl(m_socket,FIONBIO,&inParam,
				 sizeof(inParam),&outParam,sizeof(outParam),(LPDWORD)&bytesReturned,
				 NULL,NULL) == 0)
		return X_OK;
	else
		return X_ERR_OPERATION_FAILED;
#else
	//Need unix implementation.
#endif

}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::setBroadcast(XBOOL broadcast)
{
#ifdef XSP_WINDOWS
	BOOL optionValue;
	if (broadcast)
		optionValue = 1;
	else
		optionValue = 0;
	int optionValueLength = sizeof(BOOL);

	if (setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,
				  (XS8*)&optionValue,optionValueLength) == 0)
		return X_OK;
	else
		return X_ERR_OPERATION_FAILED;
#else
	//Need unix implemenation.
#endif
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPConnection::setSocket(SOCKET socket)
{
	if (socket == INVALID_SOCKET)
		return X_ERR_INVALID_PARAMS;

	m_socket = socket;
	return X_OK;
}

