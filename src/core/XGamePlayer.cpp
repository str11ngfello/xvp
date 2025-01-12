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
//	Module Name:	XGamePlayer.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-15-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the game player.
*/
//*****************************************************************************
XGamePlayer::XGamePlayer()
{
	m_outgoingThreadID = 0;
	m_incomingThreadID = 0; 
	m_outgoingThreadHandle = NULL; 
	m_incomingThreadHandle = NULL;
	m_eventCallback = NULL;
	m_packetSize = 0;
	m_sendPacketThreadOn = false;
	m_receivePacketThreadOn = false;
	m_server.m_connState = X_NCS_INVALID;

	//Initialize the critical sections.
	InitializeCriticalSection(&m_broadcastQueueCS);
	InitializeCriticalSection(&m_serverCS);
}



//*****************************************************************************
/**
Destructor for the game player.
*/
//*****************************************************************************
XGamePlayer::~XGamePlayer()
{
	destroy();
}



//*****************************************************************************
/**
Initializes the game player.

@param pPlayerName	The name to assign to the player.
@param pSessionID	The session ID of the game player.  The session ID is a 
					user-defined string that identifies what game servers the
					player is compatible with.  An example value would be 
					"QuakeIIIv1.3.4Patch2.1".  This same string is stored
					in game servers.  The server and client must agree on the
					string in order for them to communicate.
@param desc	A game player description structure.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::init(XS8* pPlayerName,XS8* pSessionID,XGamePlayerDesc& desc)
{
	//The session ID cannot be NULL.
	if (!pSessionID)
		return X_ERR_INVALID_PARAMS;

	m_workerThreadEvents[0] = CreateEvent(NULL,true,false,NULL);
	m_workerThreadEvents[1] = CreateEvent(NULL,true,false,NULL);
	
	//Create the player side broadcast connection.
	if (XVP::NetworkMgr.createUDPConnection(desc.address,m_playerBroadcastConnection) != X_OK)
		return X_ERR_OPERATION_FAILED;
	XVP::NetworkMgr.setUDPBlockingState(m_playerBroadcastConnection,false);
	XVP::NetworkMgr.setUDPBroadcast(m_playerBroadcastConnection,true);

	//Create the packet pool.
	m_packetPool.init(desc.packetPoolSize,desc.maxPacketSizeInBytes + X_NET_MIN_PKT_SIZE);
	m_packetSize = desc.maxPacketSizeInBytes + X_NET_MIN_PKT_SIZE;

	//Copy over the player name.
	if (!pPlayerName)
		strcpy(m_playerName,"Default Player");
	else
	{
		//Make sure the player name will fit, otherwise, truncate it.
		if (strlen(pPlayerName) <= X_MAX_PLAYER_NAME_SIZE)
			strcpy(m_playerName,pPlayerName);
		else
		{
			memcpy(m_playerName,pPlayerName,X_MAX_PLAYER_NAME_SIZE);
			m_playerName[X_MAX_PLAYER_NAME_SIZE] = '\0';
		}
	}
	
	//Copy over the session ID. Make sure the session ID will fit,
	//otherwise, truncate it.
	if (strlen(pSessionID) <= X_MAX_SESSION_ID_SIZE)
		strcpy(m_sessionID,pSessionID);
	else
	{
		memcpy(m_sessionID,pSessionID,X_MAX_SESSION_ID_SIZE);
		m_sessionID[X_MAX_SESSION_ID_SIZE] = '\0';
	}

	//Create a "send" thread and a "receive" thread.
	m_sendPacketThreadOn = true;
	m_outgoingThreadHandle = CreateThread(NULL,0,X_GamePlayerSendPacketPumper,
										  this,0,(LPDWORD)&m_outgoingThreadID);	
	m_receivePacketThreadOn = true;
	m_incomingThreadHandle = CreateThread(NULL,0,X_GamePlayerReceivePacketPumper,
										  this,0,(LPDWORD)&m_incomingThreadID);

	return X_OK;
}



//*****************************************************************************
/**
Destroys the game player by freeing any allocated resources.  If the player is
connected to a server, the connection is closed.  All queued packets are freed.

@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGamePlayer::destroy()
{
	//We must stop the worker threads.  Set the flags to false
	//to indicate to the threads that they should stop.  Then, wait for them
	//to stop.
	m_sendPacketThreadOn = false;
	m_receivePacketThreadOn = false;
	WaitForMultipleObjectsEx(2,m_workerThreadEvents,true,INFINITE,false);

	flushBroadcastQueue();
	//if (m_pX)
	//{
		EnterCriticalSection(&m_serverCS);
		internalDisconnect();
		LeaveCriticalSection(&m_serverCS);

		XVP::NetworkMgr.destroyUDPConnection(m_playerBroadcastConnection);
	//}
	DeleteCriticalSection(&m_broadcastQueueCS);
	DeleteCriticalSection(&m_serverCS);
	CloseHandle(m_workerThreadEvents[0]);
	CloseHandle(m_workerThreadEvents[1]);

	return X_OK;
}



//*****************************************************************************
/**
Flushes all remaining packets from the broadcast queue.  Flushed packts will not
be sent out on the network and are immediately freed.

@return A return code of type XStatus.
*/

//*****************************************************************************
XStatus XGamePlayer::flushBroadcastQueue()
{
	EnterCriticalSection(&m_broadcastQueueCS);
	
	//Return any remaining packets in the broadcast queue to the packet
	//pool.  Since the packets won't be sent, they won't be freed by the
	//send thread, so we must to it here.
	while (m_broadcastQueue.getNumItems() != 0)
	{
		m_packetPool.releaseBlock(m_broadcastQueue.getFront());
		m_broadcastQueue.removeFront();
	}

	LeaveCriticalSection(&m_broadcastQueueCS);

	return X_OK;
}



//*****************************************************************************
/**
Gets a new packet from the player's packet pool. 

@return A pointer to the new packtet. 
*/
//*****************************************************************************
XS8* XGamePlayer::getNewPacket()
{
	return X_NET_USER_PKT(m_packetPool.getFreeBlock());
}



//*****************************************************************************
/**
Frees a packet obtained from the getNewPacket method.  The packet is returned
to the player's memory pool.  

@param pPacket	A pointer to the packet to be freed.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::freePacket(XS8* pPacket)
{
	return m_packetPool.releaseBlock(X_NET_SYS_PKT(pPacket));
}



//*****************************************************************************
/**
Gets the number of free packets in the player's packet pool.

@return The number of free packets in the player's packet pool.
*/
//*****************************************************************************
XU32 XGamePlayer::getNumFreePackets()
{
	return m_packetPool.getNumFreeBlocks();
}



//*****************************************************************************
/**
Gets the number of used packets in the player's memory pool.

@return The number of used packets in the player's memory pool.
*/
//*****************************************************************************
XU32 XGamePlayer::getNumUsedPackets()
{
	return m_packetPool.getNumUsedBlocks();
}



//*****************************************************************************
/**
Sends a reliable packet from the player to the connected server.  Packet
delivery is guaranteed through the TCP protocol.  

@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the player once it is sent, therefore
				the packet should not be manually freed.
@param sizeInBytes	The size of the packet.  This size cannot exceed the size
					specified in the player's description structure.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGamePlayer::sendReliablePacket(XS8* pPacket,XU32 sizeInBytes)
{
	if (!pPacket) 
		return X_ERR_INVALID_PARAMS;

	//Make sure we have a server connection.
	if (m_server.m_connState != X_NCS_CURRENT)
		return X_ERR_NOT_READY;

	XS8* pSendPacket = X_NET_SYS_PKT(pPacket);
	X_SET_MSG_CODE(pSendPacket,X_NMT_USER);
	X_SET_PKT_LEN(pSendPacket,sizeInBytes + X_NET_PKT_HEADER_SIZE);
	
	EnterCriticalSection(&m_serverCS);
	m_server.m_sendQueue.insertBack(pSendPacket);
	LeaveCriticalSection(&m_serverCS);

	return X_OK;
}



//*****************************************************************************
/**
Broadcasts a packet from the player using the UDP protocol.  The packet is sent
with a destination address of 255.255.255.255 and is not guaranteed to reach
any specific host.

@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the player once it is sent, therefore
				the packet should not be manually freed.
@param sizeInBytes	The size of the packet.  This size cannot exceed the size
					specified in the player's description structure.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGamePlayer::broadcastPacket(XS8* pPacket,XU32 sizeInBytes)
{
	if (!pPacket)
		return X_ERR_INVALID_PARAMS;
	
	//Make sure we have a server connection.
	if (m_server.m_connState != X_NCS_CURRENT)
		return X_ERR_NOT_READY;
	
	//Convert to system packet.
	XS8* pSysPacket = X_NET_SYS_PKT(pPacket);
	X_SET_MSG_CODE(pSysPacket,X_NMT_USER);
	X_SET_PKT_LEN(pSysPacket,sizeInBytes + X_NET_PKT_HEADER_SIZE);

	EnterCriticalSection(&m_broadcastQueueCS);
	m_broadcastQueue.insertBack(pSysPacket);
	LeaveCriticalSection(&m_broadcastQueueCS);
	
	return X_OK;
}



//*****************************************************************************
/**
Internally broadcasts a packet from the player using the UDP protocol.  The
packet is sent with a destination address of 255.255.255.255 and is not
guaranteed to reach any specific host.  This method assumes the packet is
ready to be sent.
 
@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the player once it is sent, therefore
				the packet should not be manually freed.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGamePlayer::internalBroadcastPacket(XS8* pPacket)
{
	if (!pPacket)
		return X_ERR_INVALID_PARAMS;
	
	EnterCriticalSection(&m_broadcastQueueCS);
	m_broadcastQueue.insertBack(pPacket);
	LeaveCriticalSection(&m_broadcastQueueCS);

	return X_OK;
}



//*****************************************************************************
/**
Gets the player's network address.

@param address	A variable to hold the returned network address.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::getLocalNetAddress(XNetAddress& address)
{
	XStatus status;
	EnterCriticalSection(&m_serverCS);

	//If we are currently connected to the server, then return the
	//player network address.
	if (m_server.m_connState == X_NCS_CURRENT)
	{
		address = m_localNetAddress;
		status = X_OK;
	}
	else
		status = X_ERR_NOT_READY;

	LeaveCriticalSection(&m_serverCS);

	return status;
}
	


//*****************************************************************************
/**
Gets the network address of the server which the player is connected to.  If the
player is not connected to a server, this function returns returns
X_ERR_NOT_READY.

@param address	A variable to hold the returned network address.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGamePlayer::getServerNetAddress(XNetAddress& address)
{
	XStatus status;
	EnterCriticalSection(&m_serverCS);

	//If we are currently connected to the server, the return the
	//server's network address.
	if (m_server.m_connState == X_NCS_CURRENT)
	{
		address = m_server.m_address;
		status = X_OK;
	}
	else
		status = X_ERR_NOT_READY;

	LeaveCriticalSection(&m_serverCS);

	return status;
}



//*****************************************************************************
/**
Registers an event callback with the player.  The player will notify the
application via this callback when events occur. 

@param eventCallback	A pointer to a function callback.  If this parameter is
						NULL, the current callback is unregistered.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::registerEventCallback(X_NET_MSG_ACTION_TYPE (*eventCallback)
										  (X_NET_MSG_TYPE msgType,XS8* pPacket,
										  XU32 packetLength,XNetAddress& fromAddress))
{
	m_eventCallback = eventCallback;
	return X_OK;
}



//*****************************************************************************
/**
Compares a session ID with the player's session ID to see if they match.

@param pData	A pointer to a buffer where the session ID is stored.
@return True if the passed in session ID matches the player's session ID,
		false otherwise.
*/
//*****************************************************************************
XBOOL XGamePlayer::isSessionIDValid(XS8* pData)
{
	if ((pData[0] <= X_MAX_SESSION_ID_SIZE) && (pData[0] > 0))
		if (strlen(m_sessionID) == pData[0])
			if (memcmp(m_sessionID,pData+1,XU32(pData[0])) == 0)
				return true;

	//Session ID was not valid.
	return false;
}



//*****************************************************************************
/**
Querys the network for game servers that are compatible with this player.  In
other words, it determines if there are any servers on the network that have 
the same session ID as the player.  

The player will notify the application via the event callback if it finds
suitable servers.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::queryForServers()
{
	//Let's broadcast a query packet out onto the network
	//and look for XGameServers with our session ID.
	XNetAddress toAddress("255.255.255.255",X_NET_BROADCAST_PORT);
	XS8* pNewPacket = m_packetPool.getFreeBlock();
	X_SET_MSG_CODE(pNewPacket,X_NMT_SERVER_QUERY);
	
	//Put the session ID into the query packet.
	XU32 length = strlen(m_sessionID);
	pNewPacket[X_PKT_DATA_IDX] = length;
	memcpy(pNewPacket + X_PKT_DATA_IDX + 1,m_sessionID,length);

	//Length = header size + length of sessionID + length field
	X_SET_PKT_LEN(pNewPacket,X_NET_PKT_HEADER_SIZE + length + 1);

	//Finally, send the query.
	XU32 sentDataLength = 0;
	XStatus status = XVP::NetworkMgr.sendUDPData(m_playerBroadcastConnection,pNewPacket,
										   X_GET_PKT_LEN(pNewPacket),sentDataLength,
										   toAddress);
	m_packetPool.releaseBlock(pNewPacket);

	return status;
}


//*****************************************************************************
/**
Disconnects the player from a server.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::disconnect()
{
	EnterCriticalSection(&m_serverCS);
	internalDisconnect();
	LeaveCriticalSection(&m_serverCS);

	return X_OK;
}



//*****************************************************************************
/**
Determines if the player is connected to a server.

@return True if the player is connected to a server, false otherwise.
*/
//*****************************************************************************
XBOOL XGamePlayer::isConnected()
{
	if (m_server.m_connState == X_NCS_CURRENT)
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Internal method that disconnects the player from a server.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::internalDisconnect()
{
	//NOTE: This function can only be called by a thread if it has locked
	//the m_serverCS critical section.

	if (m_server.m_connState == X_NCS_CURRENT)
	{
		if (m_eventCallback)
			m_eventCallback(X_NMT_CONNECTION_CLOSED,NULL,0,m_server.m_address);

		//Close the player side connection to the server.
		XVP::NetworkMgr.destroyTCPConnection(m_server.m_connHandle);	
		m_localNetAddress.clear();

		//Return any remaining packets in the server's queue to the packet
		//pool.  Since the packets won't be sent, they won't be freed by the
		//send thread, so we must do it here.
		while (m_server.m_sendQueue.getNumItems() != 0)
		{
			m_packetPool.releaseBlock(m_server.m_sendQueue.getFront());
			m_server.m_sendQueue.removeFront();
		}

		//Return any remaining packets in the broadcast queue to the server's packet
		//pool.  Since the packets won't be sent, they won't be freed by the
		//send thread, so we must to it here.
		while (m_broadcastQueue.getNumItems() != 0)
		{
			m_packetPool.releaseBlock(m_broadcastQueue.getFront());
			m_broadcastQueue.removeFront();
		}
		
		//Now set the server state to invalid.
		m_server.m_connState = X_NCS_INVALID;
		m_server.m_connHandle = X_INVALID_HANDLE;
	}
	
	return X_OK;	
}

																						    

//*****************************************************************************
/**
Connects the player to a server.  If the server is not compatible or the player
is already connected to a server, the connection attempt will fail.

@param localNetAddress	The network address to assign to the player.  If the
						network address contains an IP address of 0 and a port
						number of 0, the player will automatically choose a
						suitable network address. 
@param serverNetAddress	The network address of the server to connect with.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGamePlayer::connect(XNetAddress& localNetAddress,XNetAddress& serverNetAddress)
{
	XStatus status = X_OK;
	EnterCriticalSection(&m_serverCS);
	
	//First, determine if we are already connected to a server.
	//If we are then we can't connect to another server.  The user
	//has to disconnect first.
	if (m_server.m_connState != X_NCS_INVALID)
		 status = X_ERR_OPERATION_FAILED;
	else
	{
		//Okay, let's try to connect to the server specified.
		status = XVP::NetworkMgr.createTCPConnection(localNetAddress,serverNetAddress,
												m_server.m_connHandle);
		if (status == X_OK)
		{
			m_server.m_packetizer.destroy();
			m_server.m_packetizer.init(m_server.m_connHandle,m_packetSize);
			m_localNetAddress = localNetAddress;
			m_server.m_address = serverNetAddress;
			XVP::NetworkMgr.setTCPBlockingState(m_server.m_connHandle,false);
			m_server.m_connState = X_NCS_CURRENT;
		}
	}

	LeaveCriticalSection(&m_serverCS);
	return status;
}



//*****************************************************************************
/**
Builds a validation response.

@param pPacket	A pointer to the packet where the validation response data
				will be inserted.
@return None.
*/
//*****************************************************************************
void XGamePlayer::buildValidationResponse(XS8* pPacket)
{
	XS8* pInsert = pPacket + X_PKT_DATA_IDX;
	XU32 dataLength = 0;

	X_SET_MSG_CODE(pPacket,X_NMT_VALIDATION_RESPONSE);
	
	//Put the session ID into the packet.
	XU32 length = strlen(m_sessionID);
	*pInsert++ = length;
	memcpy(pInsert,m_sessionID,length);
	pInsert += length;
	dataLength += length + 1;

	//Put the player name into the packet.
	length = strlen(m_playerName);
	*pInsert++ = length;
	memcpy(pInsert,m_playerName,length);
	pInsert += length;
	dataLength += length + 1;

	X_SET_PKT_LEN(pPacket,X_NET_PKT_HEADER_SIZE + dataLength);
}



//*****************************************************************************
/**
This method is the main entry point for the player's outgoing packet thread.
It's responsible for sending packets from the send queues and updating the
player's internal state machine.

@return None.
*/
//*****************************************************************************
void XGamePlayer::outgoingPacketPump()
{
	//Check to see if we are supposed to kill this thread.
	if (!m_sendPacketThreadOn)
	{
		SetEvent(m_workerThreadEvents[0]);
		ExitThread(0);
	}

	XS8* pPacket;

	//If there is a packet to send, send it.
	EnterCriticalSection(&m_broadcastQueueCS);
	EnterCriticalSection(&m_serverCS);
	XU32 sentDataLength = 0;
	while (m_broadcastQueue.getNumItems() != 0)
	{
		//Get the packet and broadcast is to the server.
		pPacket = m_broadcastQueue.getFront();
		m_broadcastQueue.removeFront();
		
		if (m_server.m_connState == X_NCS_CURRENT)
		{
			//Insert the next sequence number.
			X_SET_SEQ_NUM(pPacket,m_server.m_nextTransmitSeqNum);
			m_server.m_nextTransmitSeqNum++;
			XVP::NetworkMgr.sendUDPData(m_playerBroadcastConnection,pPacket,X_GET_PKT_LEN(pPacket),
								   sentDataLength,m_server.m_address);
		}
			
		m_packetPool.releaseBlock(pPacket);
	}		

	//Now process the server's state machine
	XS8* pNewPacket = NULL;
	
	switch(m_server.m_connState)
	{
	case X_NCS_INVALID:
		//Do nothing, we aren't connected to the server.
		break;

	case X_NCS_WAITING:
		break;

	case X_NCS_VALIDATION:
		pNewPacket = m_packetPool.getFreeBlock();
		buildValidationResponse(pNewPacket);

		//Queue the packet.
		m_server.m_sendQueue.insertBack(pNewPacket);

		m_server.m_connState = X_NCS_WAITING;

		break;

	case X_NCS_CURRENT:
		break;

	}

	//Now that we've processed the server's state machine for this time step,
	//let's process its send queue.
	while (m_server.m_sendQueue.getNumItems() != 0)
	{
		//Check the status of the last send operation.
		if (!m_server.m_packetizer.canSendNewPacket())
		{
			m_server.m_packetizer.flushCurrentSendPacket();
			break;
		}
		else
		{
			//Get the packet and send it.
			pPacket = m_server.m_sendQueue.getFront();
			m_server.m_sendQueue.removeFront();
			m_server.m_packetizer.sendPacket(pPacket,X_GET_PKT_LEN(pPacket));
			m_packetPool.releaseBlock(pPacket);		
		}		
	}

	LeaveCriticalSection(&m_serverCS);
	LeaveCriticalSection(&m_broadcastQueueCS);
}



//*****************************************************************************
/**
This method is the main entry point for the player's incoming packet thread.
It's responsible for receiving packets and processing them appropriately.

@return None.
*/
//*****************************************************************************
void XGamePlayer::incomingPacketPump()
{
	//Check to see if we are supposed to kill this thread.
	if (!m_receivePacketThreadOn)
	{
		SetEvent(m_workerThreadEvents[1]);
		ExitThread(0);
	}

	XS8* pPacket = NULL;
	XS8* pNewPacket = NULL;
	XU32 returnDataLength = 0;
	XNetAddress fromAddress;
	XStatus status;

	X_NET_MSG_ACTION_TYPE retAction;

	//See if a packet has arrived on the player's broadcast port.
	pPacket = m_packetPool.getFreeBlock();
	XVP::NetworkMgr.receiveUDPData(m_playerBroadcastConnection,pPacket,m_packetSize,returnDataLength,
	 					   fromAddress);
	EnterCriticalSection(&m_serverCS);
	if (returnDataLength > 0)
	{
		XU32 sentDataLength = 0;
		
		//Let's see what type of packet this is.
		switch (pPacket[0])
		{
		case X_NMT_SERVER_ANNOUNCE:
			//Notify the user of the server announcement if the session ID
			//matches the player's session id. 
			if (isSessionIDValid(pPacket + X_PKT_DATA_IDX))
			{
				if (m_eventCallback)
					m_eventCallback(X_NMT_SERVER_ANNOUNCE,pPacket,0,fromAddress);
			}
			m_packetPool.releaseBlock(pPacket);
			break;

		default:
			//Don't know what kind of packet this is, so drop it and return the
			//packet to the pool for reuse.
			m_packetPool.releaseBlock(pPacket);
			break;
		}

	}
	else
		m_packetPool.releaseBlock(pPacket);


	//Now let's check the TCP port if we are connected to the server.
	XU32 packetLength = 0;
	if (m_server.m_connState != X_NCS_INVALID)
	{
		status = m_server.m_packetizer.receivePacket(pPacket,packetLength);
		switch (status)
		{
		case X_ERR_NOT_READY:
			//No new packet has arrived so just skip this player.
			break;
		case X_ERR_NOT_RUNNING:
			//The server has closed the connection, we will clean
			//up the player side connection.
			internalDisconnect();

			break;
		case X_OK:
			//A new packet has arrived from the server.  What type
			//is it?
			switch(pPacket[0])
			{
			case X_NMT_VALIDATION_REQUEST:
				m_server.m_connState = X_NCS_VALIDATION;
				break;
			case X_NMT_CONNECTION_ACCEPTED:
				if (m_eventCallback)
					m_eventCallback(X_NMT_CONNECTION_ACCEPTED,NULL,0,fromAddress);
				m_server.m_connState = X_NCS_CURRENT;
				break;
			case X_NMT_CONNECTION_REJECTED:
				if (m_eventCallback)
					m_eventCallback(X_NMT_CONNECTION_REJECTED,NULL,0,fromAddress);
				internalDisconnect();
				break;

			}
			break;
		}
	}
		
	LeaveCriticalSection(&m_serverCS);

}



//*****************************************************************************
//*****************************************************************************
DWORD WINAPI X_GamePlayerSendPacketPumper(LPVOID pParam)
{
	XGamePlayer* pGamePlayer = (XGamePlayer*)pParam;

	while(true)
	{	
		Sleep(1);
		pGamePlayer->outgoingPacketPump();
	}

	return 0;
}


//*****************************************************************************
//*****************************************************************************
DWORD WINAPI X_GamePlayerReceivePacketPumper(LPVOID pParam)
{
	XGamePlayer* pGamePlayer = (XGamePlayer*)pParam;

	while(true)
	{	
		Sleep(1);
		pGamePlayer->incomingPacketPump();
	}

	return 0;
}




