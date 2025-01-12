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
//	Module Name:	XNetworkMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
/**
Constructor for the network driver.  
*/
//*****************************************************************************
XNetworkMgr::XNetworkMgr()
{
	//m_maxUDPConnections = 0;
 	//m_numAllocatedUDPConnections = 0;
	//m_maxTCPConnections = 0;
 	//m_numAllocatedTCPConnections = 0;
	//m_maxTCPListeners = 0;
 	//m_numAllocatedTCPListeners = 0;

	//Create the critical sections.
    InitializeCriticalSection(&m_UDPConnectionCS);
    InitializeCriticalSection(&m_TCPConnectionCS);
    InitializeCriticalSection(&m_TCPListenerCS);

	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the network driver.  
*/
//*****************************************************************************
XNetworkMgr::~XNetworkMgr()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XNetworkMgr::init(XVPDesc& desc)
{
	m_inited = true;

	X_DEBUG_LOG_LN("\n*** BEGIN NETWORK MANAGER INIT ***",X_DLL_INFO);

#ifdef XSP_WINDOWS
	WORD versionRequested;
	WSADATA wsaData;
	DWORD bufferSize = 0;
	LPWSAPROTOCOL_INFO SelectedProtocol;
	int NumProtocols;

	//Start WinSock2. If it fails, log error and return.
	versionRequested = MAKEWORD(2, 0);
	int error = WSAStartup(versionRequested, &wsaData);
    
	//If winsock2 did not start...
	if(error)
	{
		X_ASSERT_FORCED("networking engine failed to initialize!");
		return X_ERR_OPERATION_FAILED;
	}
	else
	{
		X_DEBUG_LOG_LN("networking engine initialized.",X_DLL_INFO);

		//Confirm that the WinSock2 DLL supports version 2.
		if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
		{
			X_ASSERT_FORCED("networking engine started, but insufficient system support was found.");

			WSACleanup();
			return X_ERR_OPERATION_FAILED;
		}
	}

	//Call WSAEnumProtocols() to find out the system supported protocols.
	NumProtocols = WSAEnumProtocols(NULL,NULL,&bufferSize);
	if( (NumProtocols != SOCKET_ERROR) && (WSAGetLastError() != WSAENOBUFS))
	{
		X_ASSERT_FORCED("No Networking Protocols found.");
		WSACleanup();
		return X_ERR_OPERATION_FAILED;
	}

	//Allocate a storage buffer, call WSAEnumProtocols to get an array of
	//WSAPROTOCOL_INFO structs
	SelectedProtocol = (LPWSAPROTOCOL_INFO) malloc(bufferSize);

	//Allocate memory for protocol list and define what protocols we are interested in.
	int *protos = (int *) calloc(2, sizeof(int));
	protos[0] = IPPROTO_TCP;
	protos[1] = IPPROTO_UDP;
	NumProtocols = WSAEnumProtocols(protos, SelectedProtocol, &bufferSize);
	free(protos);
	protos = NULL;
	free(SelectedProtocol);
	SelectedProtocol = NULL;

	if(NumProtocols == SOCKET_ERROR)
	{
		X_ASSERT_FORCED("Required networking protocols not found.");
		WSACleanup();
		return X_ERR_OPERATION_FAILED;
	}

	

#else // we aren't using Windows so there is no "winsock"
#endif

	//Record the initial number of network connections
	//m_maxUDPConnections = desc.numInitialUDPConnections;
	//m_maxTCPConnections = desc.numInitialTCPConnections;
	///m_maxTCPListeners = desc.numInitialTCPListeners;

	//Assign -1 to each slot to represent empty state.
	/*m_UDPConnectionArray = new SOCKET[m_maxUDPConnections];
	if (!m_UDPConnectionArray)
		return X_ERR_MEMORY_EXHAUSTED;
	for (XU32 i = 0;i < m_maxUDPConnections;i++)
		m_UDPConnectionArray[i] = -1;
	X_DEBUG_LOG_LN("Created UDP connection array.",X_DLL_INFO);
	*/
	m_UDPConnections.init("UDPConnections",desc.maxUDPConnections);


	//Assign -1 to each slot to represent empty state.
	/*m_TCPConnectionArray = new SOCKET[m_maxTCPConnections];
	if (!m_TCPConnectionArray)
		return X_ERR_MEMORY_EXHAUSTED;
	for (i = 0;i < m_maxTCPConnections;i++)
		m_TCPConnectionArray[i] = -1;
	X_DEBUG_LOG_LN("Created TCP connection array.",X_DLL_INFO);*/
	m_TCPConnections.init("TCPConnections",desc.maxTCPConnections);

	//Assign NULL to each slot to represent empty state.
	/*m_TCPListenerArray = new XTCPListener*[m_maxTCPListeners];
	if (!m_TCPListenerArray)
		return X_ERR_MEMORY_EXHAUSTED;
	for (i = 0;i < m_maxTCPListeners;i++)
		m_TCPListenerArray[i] = NULL;
	X_DEBUG_LOG_LN("Created TCP listener array.",X_DLL_INFO);
	*/
	m_TCPListeners.init("TCPListeners",desc.maxTCPListeners);

	X_DEBUG_LOG_LN("*** END NETWORK MANAGER INIT ***",X_DLL_INFO);

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroy()
{
	X_DEBUG_LOG_LN("\n*** BEGIN NETWORK MANAGER SHUTDOWN ***",X_DLL_INFO);

	//Clean up UDP connection array
	m_UDPConnections.destroy();

	//Clean up TCP connection array
	m_TCPConnections.destroy();

	//Clean up TCP listener array
	m_TCPListeners.destroy();

	//Destoy the UDP critical section.
    DeleteCriticalSection(&m_UDPConnectionCS);

	//Destoy the TCP critical section.
    DeleteCriticalSection(&m_TCPConnectionCS);

	//Destoy the TCP listener critical section.
    DeleteCriticalSection(&m_TCPListenerCS);

#ifdef XSP_WINDOWS
	WSACleanup();
#endif

	m_inited = false;

	X_DEBUG_LOG_LN("*** END NETWORK MANAGER SHUTDOWN ***",X_DLL_INFO);

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::update(XF32 dt)
{
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::createUDPConnection(XNetAddress& localAddress,XHandle& ncHandle)
{
	EnterCriticalSection(&m_UDPConnectionCS);
	
	XStatus status = X_OK;
	XUDPConnection* pUDPConnection = NULL;
	XNew(pUDPConnection,XUDPConnection,XOSMgr::getDefaultHeap())
	status = pUDPConnection->init(localAddress);
	if (status == X_OK)
	{
		
		ncHandle = m_UDPConnections.addResource(pUDPConnection,NULL);

		if (ncHandle == X_INVALID_HANDLE)
			status = X_ERR_OPERATION_FAILED;
	}

	LeaveCriticalSection(&m_UDPConnectionCS);
	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::createTCPConnection(XNetAddress& localAddress,
											XNetAddress& remoteAddress,
											XHandle& ncHandle)
{
	EnterCriticalSection(&m_TCPConnectionCS);
	
	XStatus status = X_OK;
	XTCPConnection* pTCPConnection = NULL;
	XNew(pTCPConnection,XTCPConnection,XOSMgr::getDefaultHeap())
	pTCPConnection->init(localAddress,remoteAddress);
	ncHandle = m_TCPConnections.addResource(pTCPConnection,NULL);

	if (ncHandle == X_INVALID_HANDLE)
		status = X_ERR_OPERATION_FAILED;

	LeaveCriticalSection(&m_TCPConnectionCS);
	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::createTCPListener(XNetAddress& listenAddress,X_NET_MSG_ACTION_TYPE (*eventCallback)
																			(X_NET_MSG_TYPE msgType,
																			 XNetAddress& clientAddress,
																			 XHandle tcpConnHandle,
																			 void* pUserData),
																			 void* pUserData,
																			 XHandle& lIndex)
{	
	EnterCriticalSection(&m_TCPListenerCS);
	
	XStatus status = X_OK;
	XTCPListener* pTCPListener = NULL;
	XNew(pTCPListener,XTCPListener,XOSMgr::getDefaultHeap())
	pTCPListener->init(listenAddress,eventCallback,pUserData);
	lIndex = m_TCPListeners.addResource(pTCPListener,NULL);

	if (lIndex == X_INVALID_HANDLE)
		status = X_ERR_OPERATION_FAILED;

	LeaveCriticalSection(&m_TCPListenerCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroyUDPConnection(XHandle ncHandle)
{
	//Make sure we have a valid network connection.
	EnterCriticalSection(&m_UDPConnectionCS);

	XStatus status = X_OK;
	if (ncHandle != X_INVALID_HANDLE)
	{
		status = m_UDPConnections.removeResource(ncHandle);
	}
	else
		status = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_UDPConnectionCS);
	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::addUDPConnectionRef(XHandle ncHandle)
{
	XBaseUDPConnection* pUDPConnection = (XBaseUDPConnection*)m_UDPConnections.getResource(ncHandle);
	if (pUDPConnection)
	{
		pUDPConnection->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroyTCPConnection(XHandle ncHandle)
{
	//Make sure we have a valid network connection.
	EnterCriticalSection(&m_TCPConnectionCS);

	XStatus status = X_OK;
	if (ncHandle != X_INVALID_HANDLE)
	{
		status = m_TCPConnections.removeResource(ncHandle);
	}
	else
		status = X_ERR_NO_SUCH_OBJECT;

	LeaveCriticalSection(&m_TCPConnectionCS);
	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::addTCPConnectionRef(XHandle ncHandle)
{
	XBaseTCPConnection* pTCPConnection = (XBaseTCPConnection*)m_TCPConnections.getResource(ncHandle);
	if (pTCPConnection)
	{
		pTCPConnection->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroyTCPListener(XHandle lIndex)
{
	//Make sure we have a valid listener.
	EnterCriticalSection(&m_TCPListenerCS);
	
	XStatus status = X_OK;
	if (lIndex != X_INVALID_HANDLE)
	{
		status = m_TCPListeners.removeResource(lIndex);
	}
	else
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_TCPListenerCS);
	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::addTCPListenerRef(XHandle ncHandle)
{
	XBaseTCPListener* pTCPListener = (XBaseTCPListener*)m_TCPListeners.getResource(ncHandle);
	if (pTCPListener)
	{
		pTCPListener->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroyAllUDPConnections()
{
	XStatus returnCode;
	EnterCriticalSection(&m_UDPConnectionCS);
	returnCode = m_UDPConnections.destroyAllResources();
	LeaveCriticalSection(&m_UDPConnectionCS);
	return returnCode;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroyAllTCPConnections()
{
	XStatus returnCode;
	EnterCriticalSection(&m_TCPConnectionCS);
	returnCode = m_TCPConnections.destroyAllResources();
	LeaveCriticalSection(&m_TCPConnectionCS);
	return returnCode;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::destroyAllTCPListeners()
{
	XStatus returnCode;
	EnterCriticalSection(&m_TCPListenerCS);
	returnCode = m_TCPListeners.destroyAllResources();
	LeaveCriticalSection(&m_TCPListenerCS);
	return returnCode;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::sendUDPData(XHandle ncHandle,XS8* pData,XU32 dataLength,
									XU32& sentDataLength,XNetAddress& toAddress)
{
	XBaseUDPConnection* pUDPConnection = (XBaseUDPConnection*)m_UDPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_UDPConnectionCS);

	if (pUDPConnection)
	{
		status = pUDPConnection->sendData(pData,dataLength,sentDataLength,toAddress);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_UDPConnectionCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::receiveUDPData(XHandle ncHandle,XS8* pData,XU32 dataLength,
									XU32& returnDataLength,XNetAddress& fromAddress)
{
	XBaseUDPConnection* pUDPConnection = (XBaseUDPConnection*)m_UDPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_UDPConnectionCS);

	if (pUDPConnection)
	{
		status = pUDPConnection->receiveData(pData,dataLength,returnDataLength,fromAddress);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_UDPConnectionCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::sendTCPData(XHandle ncHandle,XS8* pData,XU32 dataLength,XU32& sentDataLength)
{
	XBaseTCPConnection* pTCPConnection = (XBaseTCPConnection*)m_TCPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_TCPConnectionCS);

	if (pTCPConnection)
	{
		status = pTCPConnection->sendData(pData,dataLength,sentDataLength);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_TCPConnectionCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::receiveTCPData(XHandle ncHandle,XS8* pData,XU32 dataLength,
									   XU32& returnDataLength)
{
	XBaseTCPConnection* pTCPConnection = (XBaseTCPConnection*)m_TCPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_TCPConnectionCS);

	if (pTCPConnection)
	{
		status = pTCPConnection->receiveData(pData,dataLength,returnDataLength);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_TCPConnectionCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::setUDPBlockingState(XHandle ncHandle,XBOOL blocking)
{
	XBaseUDPConnection* pUDPConnection = (XBaseUDPConnection*)m_UDPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_UDPConnectionCS);

	if (pUDPConnection)
	{
		status = pUDPConnection->setBlockingState(blocking);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_UDPConnectionCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::setUDPBroadcast(XHandle ncHandle,XBOOL allowBroadcast)
{
	XBaseUDPConnection* pUDPConnection = (XBaseUDPConnection*)m_UDPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_UDPConnectionCS);

	if (pUDPConnection)
	{
		status = pUDPConnection->setBroadcast(allowBroadcast);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_UDPConnectionCS);

	return status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::setTCPBlockingState(XHandle ncHandle,XBOOL blocking)
{
	XBaseTCPConnection* pTCPConnection = (XBaseTCPConnection*)m_TCPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_TCPConnectionCS);

	if (pTCPConnection)
	{
		status = pTCPConnection->setBlockingState(blocking);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_TCPConnectionCS);

	return status;

}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::setTCPBroadcast(XHandle ncHandle,XBOOL allowBroadcast)
{
	XBaseTCPConnection* pTCPConnection = (XBaseTCPConnection*)m_TCPConnections.getResource(ncHandle);
	XStatus status;

	EnterCriticalSection(&m_TCPConnectionCS);

	if (pTCPConnection)
	{
		status = pTCPConnection->setBroadcast(allowBroadcast);
	}
	else 
		status = X_ERR_NO_SUCH_OBJECT;
	
	LeaveCriticalSection(&m_TCPConnectionCS);

	return status;

}



//***************************************************************************** 
//***************************************************************************** 
XStatus XNetworkMgr::addTCPConnection(XHandle& ncHandle,SOCKET sock)
{
	EnterCriticalSection(&m_TCPConnectionCS);
	
	XStatus status = X_OK;
	XTCPConnection* pTCPConnection = NULL;
	XNew(pTCPConnection,XTCPConnection,XOSMgr::getDefaultHeap())
	
	//Instead of calling init(), lets just set the socket.
	pTCPConnection->setSocket(sock);
	ncHandle = m_TCPConnections.addResource(pTCPConnection,NULL);

	if (ncHandle == X_INVALID_HANDLE)
		status = X_ERR_OPERATION_FAILED;

	LeaveCriticalSection(&m_TCPConnectionCS);
	return status;
}


