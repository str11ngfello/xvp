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
//	Module Name:	XUDPConnection.cpp
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
XUDPConnection::XUDPConnection()
{
	sock = INVALID_SOCKET;
}



//*****************************************************************************
//*****************************************************************************
XUDPConnection::~XUDPConnection()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUDPConnection::init(XNetAddress& localAddress)
{

	//Try and create the UDP socket
	if((sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1)
	{
		X_ASSERT_FORCED("Could not create UDP connection.");
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
		if(bind(sock,(struct sockaddr*)&address,sizeof(address)) != -1)
			return X_OK;
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUDPConnection::destroy()
{
#ifdef XSP_WINDOWS
	closesocket(sock);
#else
	close(sock);
#endif
	XResource::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUDPConnection::sendData(XS8* pData,XU32 dataLength,XU32& sentDataLength,
								 XNetAddress& toAddress)
{
	struct sockaddr_in sendToAddress;
			memset((char *)&sendToAddress,0,sizeof(sendToAddress));

			sendToAddress.sin_addr.s_addr = htonl((toAddress.getNumIpAddress()));
			sendToAddress.sin_port = htons(toAddress.getPort());
			sendToAddress.sin_family = AF_INET;

			int	ret = sendto(sock,(XS8*)pData,dataLength,0,
							(struct sockaddr*)&sendToAddress,sizeof(sendToAddress));
			if(ret == -1)
			{
				sentDataLength = 0;
#ifdef XSP_WINDOWS
				int status = WSAGetLastError();			
				if(status == WSAEWOULDBLOCK)
					return X_OK;
#else
				if(errno != EWOULDBLOCK)
					return X_OK;
#endif
				return X_ERR_OPERATION_FAILED;
			}

			sentDataLength = ret;

			return X_OK;

}



//*****************************************************************************
//*****************************************************************************
XStatus XUDPConnection::receiveData(XS8* pData,XU32 dataLength,
									XU32& returnDataLength,XNetAddress& fromAddress)
{
	struct sockaddr_in tempFrom;
			socklen_t fromlen = sizeof(sockaddr);

			int ret = recvfrom(sock,(XS8*)pData,dataLength,0,
							  (struct sockaddr *)&tempFrom,&fromlen);

			// Copy the address to the from pointer
			//if(from != NULL)
			//	*(struct sockaddr *) from = tempFrom;

			if(ret == -1)
			{
				returnDataLength = 0;
#ifdef XSP_WINDOWS
				int status = WSAGetLastError();			
				if(status == WSAEWOULDBLOCK)
					return X_OK;
#else
				if(errno != EWOULDBLOCK)
					return X_OK;
#endif
				return X_ERR_OPERATION_FAILED;
			}
			

			returnDataLength = ret;
			
			//Fill in the "from" information.
			if (ret > 0)
			{
				fromAddress.setIpAddressNum(ntohl(tempFrom.sin_addr.s_addr));
				fromAddress.setPort(htons(tempFrom.sin_port));
			}

			return X_OK;				
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUDPConnection::setBlockingState(XBOOL blocking)
{
#ifdef XSP_WINDOWS
	DWORD inParam;
	DWORD outParam;
	ULONG32 bytesReturned;
	if (blocking)
		inParam = 0;
	else
		inParam = 1;
			
	if (WSAIoctl(sock,FIONBIO,&inParam,
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
XStatus XUDPConnection::setBroadcast(XBOOL broadcast)
{
#ifdef XSP_WINDOWS
	BOOL optionValue;
	if (broadcast)
		optionValue = 1;
	else
		optionValue = 0;
	int optionValueLength = sizeof(BOOL);

	if (setsockopt(sock,SOL_SOCKET,SO_BROADCAST,
				  (XS8*)&optionValue,optionValueLength) == 0)
		return X_OK;
	else
		return X_ERR_OPERATION_FAILED;
#else
	//Need unix implemenation.
#endif
}


