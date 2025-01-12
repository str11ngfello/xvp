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
//	Module Name:	XTCPListener.cpp
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
XTCPListener::XTCPListener()
{
}



//*****************************************************************************
//*****************************************************************************
XTCPListener::~XTCPListener()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPListener::init(XNetAddress& listenAddress,X_NET_MSG_ACTION_TYPE (*eventCallback)
																  (X_NET_MSG_TYPE msgType,
																   XNetAddress& clientAddress,
																   XHandle tcpConnHandle,
																   void* pUserData),
																   void* pUserData)
{
	m_closeEvent = CreateEvent(NULL,true,false,NULL);
	m_eventCallback = eventCallback;
	m_pUserData = pUserData;

	//Create a TCP socket to listen on.
	if((m_listenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1)
	{
		X_ASSERT_FORCED("Could not create TCP Listener socket.");
	}
	else
	{
		struct sockaddr_in address;
		memset(&address,0,sizeof(sockaddr_in));

		//If no IP address is specified, assume the default.
		if (listenAddress.getNumIpAddress() == 0)
			address.sin_addr.s_addr = htonl(INADDR_ANY);
		else
			address.sin_addr.s_addr = htonl(listenAddress.getNumIpAddress());

		//If 0 is specified for the port number, choose an available port number
		if (listenAddress.getPort() == 0)
			address.sin_port = 0;
		else
			address.sin_port = htons((short)listenAddress.getPort());
		
		address.sin_family = AF_INET;

		//Bind the address to the socket
		if(bind(m_listenSocket,(struct sockaddr*)&address,sizeof(address)) == -1)
		{
			X_ASSERT_FORCED("Could not open TCP Listener Socket.");
		}

#pragma message("how did this ever work?  Will I still be able to kill the listener?")
		//Set socket to non-blocking so we can kill it later.
		//XVP::NetworkMgr.setTCPBlockingState(m_listenSocket,false);

#ifdef XSP_WINDOWS
		DWORD inParam = 1;
		DWORD outParam;
		ULONG32 bytesReturned;
				
		if (WSAIoctl(m_listenSocket,FIONBIO,&inParam,
					sizeof(inParam),&outParam,sizeof(outParam),(LPDWORD)&bytesReturned,
					NULL,NULL) != 0)
		{
			//Cannot return here or XVP will HANG!!! We should just log an error!
			//return X_ERR_OPERATION_FAILED;
		}
#else
		//Need unix implementation.
#endif
	}

	//Create the listen queue.
	if (listen(m_listenSocket,256) != 0)
	{
		X_ASSERT_FORCED("Could not listen on TCP Listener.");
	}

	//Create a "listen" thread.
	m_listenerThreadOn = true;
	m_listenerThreadHandle = CreateThread(NULL,0,X_TCPListenerPumper,
										  this,0,(LPDWORD)&m_listenerThreadID);	

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTCPListener::destroy()
{
	m_eventCallback = NULL;

	//We must stop the listener thread.  
	m_listenerThreadOn = false;
	WaitForSingleObject(m_closeEvent,INFINITE);
		
	CloseHandle(m_closeEvent);

	XResource::destroy();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XTCPListener::listenPump()
{
	//Check to see if we are supposed to kill this thread.
	if (!m_listenerThreadOn)
	{
		SetEvent(m_closeEvent);
		ExitThread(0);
	}

	//See if there are any pending connection requests.
	struct sockaddr clientAddress;
	int size = sizeof(clientAddress);
	SOCKET s = accept(m_listenSocket,&clientAddress,&size);


	if (s != INVALID_SOCKET)
	{
		//See if the user wants to accept or reject this connection
		//request.
		XBOOL keepConnection = false;
		XNetAddress fromAddress;
		sockaddr_in* p = (sockaddr_in*)&clientAddress;
		fromAddress.setIpAddressNum(ntohl(p->sin_addr.s_addr));
		fromAddress.setPort(htons(p->sin_port));
		if (m_eventCallback)
		{
			if (m_eventCallback(X_NMT_CONNECTION_REQUEST,fromAddress,0,m_pUserData) == X_NMAT_ACCEPT)
			{
				XHandle connHandle = X_INVALID_HANDLE;
				if (XVP::NetworkMgr.addTCPConnection(connHandle,s) == X_OK)
				{
					m_eventCallback(X_NMT_CONNECTION_ACCEPTED,fromAddress,connHandle,m_pUserData);
					keepConnection = true;
				}
			}
		}
		
		if (!keepConnection)
			closesocket(s);
	}
}



//*****************************************************************************
//*****************************************************************************
DWORD WINAPI X_TCPListenerPumper(LPVOID pParam)
{
	XTCPListener* pTCPListener = (XTCPListener*)pParam;

	while(true)
	{		
		Sleep(100);
		pTCPListener->listenPump();
	}

	return 0;
}


