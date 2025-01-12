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
//	Module Name:	XGameServer.cpp
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
Constructor for the game server.
*/
//*****************************************************************************
XGameServer::XGameServer()
{
	m_acceptingConnections = true;
	m_outgoingThreadID = 0;
	m_incomingThreadID = 0;
	m_outgoingThreadHandle = NULL; 
	m_incomingThreadHandle = NULL;
	m_eventCallback = NULL;
	m_packetSize = 0;
	m_sendPacketThreadOn = false;
	m_receivePacketThreadOn = false;
	m_broadcastAddress.setIpAddressStr("255.255.255.255");
	m_nextBroadcastSeqNum = 0;
	m_numCurrentPlayers = 0;

	//Initialize the critical sections.
	InitializeCriticalSection(&m_broadcastQueueCS);
	InitializeCriticalSection(&m_playerListCS);
}



//*****************************************************************************
/**
Destructor for the game server.
*/
//*****************************************************************************
XGameServer::~XGameServer()
{
	destroy();
}



//*****************************************************************************
/**
Initializes the game server and prepares it to accept connections from game
clients..  

@param pSessionID	The session ID of the game server.  The session ID is a 
					user-defined string that identifies what game clients the
					server is compatible with.  An example value would be 
					"QuakeIIIv1.3.4Patch2.1".  This same string is stored
					in game clients.  The server and client must agree on the
					string in order for them to communicate.
@param pServerName	A name for the server.
@param pPassword	The password that clients must use to log onto the server.
@param desc	A game server description structure.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::init(XS8* pSessionID,XS8* pServerName,XS8* pPassword,
						  XGameServerDesc& desc)
{
	//The session ID cannot be NULL.
	if (!pSessionID)
		return X_ERR_INVALID_PARAMS;

	m_workerThreadEvents[0] = CreateEvent(NULL,true,false,NULL);
	m_workerThreadEvents[1] = CreateEvent(NULL,true,false,NULL);

	//Create the server's broadcast connection.
	XNetAddress broadcastAddress(NULL,X_NET_BROADCAST_PORT);
	if (XVP::NetworkMgr.createUDPConnection(broadcastAddress,m_serverBroadcastConnection) != X_OK)
		return X_ERR_OPERATION_FAILED;
	XVP::NetworkMgr.setUDPBlockingState(m_serverBroadcastConnection,false);
	XVP::NetworkMgr.setUDPBroadcast(m_serverBroadcastConnection,true);

	//Create the server's TCP listener.
	XVP::NetworkMgr.createTCPListener(desc.address,X_GamerServerListenCallback,
								 this,m_tcpListenerHandle);

	//Create the packet pool and record the maximum expected packet size.
	m_packetPool.init(desc.packetPoolSize,desc.maxPacketSizeInBytes + X_NET_MIN_PKT_SIZE);
	m_packetSize = desc.maxPacketSizeInBytes + X_NET_MIN_PKT_SIZE;

	//Set max number of players.
	m_maxPlayers = desc.maxPlayers;

	//Copy over the session ID. //Make sure the session ID will fit,
	//otherwise, truncate it.
	if (strlen(pSessionID) <= X_MAX_SESSION_ID_SIZE)
		strcpy(m_sessionID,pSessionID);
	else
	{
		memcpy(m_sessionID,pSessionID,X_MAX_SESSION_ID_SIZE);
		m_sessionID[X_MAX_SESSION_ID_SIZE] = '\0';
	}

	//Copy over the server name.
	if (!pServerName)
		strcpy(m_serverName,"Default Server");
	else
	{
		//Make sure the server name will fit, otherwise, truncate it.
		if (strlen(pServerName) <= X_MAX_SERVER_NAME_SIZE)
			strcpy(m_serverName,pServerName);
		else
		{
			memcpy(m_serverName,pServerName,X_MAX_SERVER_NAME_SIZE);
			m_serverName[X_MAX_SERVER_NAME_SIZE] = '\0';
		}
	}

	//Copy over the password.
	if (!pPassword)
		m_password[0] = '\0';
	else
	{
		//Make sure the password will fit, otherwise, we can't really
		//truncate the password and expect the user to be aware that
		//we did this, so just don't put the password into effect.
		if (strlen(pPassword) <= X_MAX_PASSWORD_SIZE)
			strcpy(m_password,pPassword);
	}

	m_acceptingConnections = desc.acceptingConnections;

	//Create a "send" thread and a "receive" thread.
	m_sendPacketThreadOn = true;
	m_outgoingThreadHandle = CreateThread(NULL,0,X_GameServerSendPacketPumper,
										  this,0,(LPDWORD)&m_outgoingThreadID);	
	m_receivePacketThreadOn = true;
	m_incomingThreadHandle = CreateThread(NULL,0,X_GameServerReceivePacketPumper,
										  this,0,(LPDWORD)&m_incomingThreadID);

	return X_OK;
}



//*****************************************************************************
/**
Destroys the game server and frees all allocated resources.  Closes all player
connections and frees all queued packets.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::destroy()
{
	//First, disable connection acceptance and then disconnect all players.
	m_acceptingConnections = false;
	disconnectAllPlayers();

	//Second, we must stop the worker threads.  Set the flags to false
	//to indicate to the threads that they should stop.  Then, wait for them
	//to stop.
	m_sendPacketThreadOn = false;
	m_receivePacketThreadOn = false;
	WaitForMultipleObjectsEx(2,m_workerThreadEvents,true,INFINITE,false);

	flushBroadcastQueue();
	//if (m_pX)
	//{
		XVP::NetworkMgr.destroyUDPConnection(m_serverBroadcastConnection);
		XVP::NetworkMgr.destroyTCPListener(m_tcpListenerHandle);
	//}
	DeleteCriticalSection(&m_broadcastQueueCS);
	DeleteCriticalSection(&m_playerListCS);
	CloseHandle(m_workerThreadEvents[0]);
	CloseHandle(m_workerThreadEvents[1]);

	return X_OK;
}



//*****************************************************************************
/**
Closes all player connections.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::disconnectAllPlayers()
{
	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		destroyPlayer(i.getValue());
		i.getNext();
	}
	
	m_numCurrentPlayers = 0;
	m_playerList.clear();
	LeaveCriticalSection(&m_playerListCS);

	return X_OK;
}



//*****************************************************************************
/**
Flushes all remaining packets from the broadcast queue.  Flushed packts will not
be sent out on the network and are immediately freed.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::flushBroadcastQueue()
{
	EnterCriticalSection(&m_broadcastQueueCS);
	
	//Return any remaining packets in the send queue to the server's packet
	//pool.  Since the packets won't be sent, they won't be freed by the
	//send thread, so we must to it here.
	while (m_broadcastQueue.getNumItems() != 0)
	{
		m_packetPool.releasePacket(m_broadcastQueue.getFront());
		m_broadcastQueue.removeFront();
	}

	LeaveCriticalSection(&m_broadcastQueueCS);

	return X_OK;
}



//*****************************************************************************
/**
Returns the number of connected players.

@return Returns the number of connected players.
*/
//*****************************************************************************
XU32 XGameServer::getNumCurrentPlayers()
{
	return m_numCurrentPlayers;
}



//*****************************************************************************
/**
Gets a player's connection duration.

@param playerID	The player ID for which to get the connection duration.
@return The connection duration in seconds.
*/
//*****************************************************************************
XF32 XGameServer::getPlayerConnectionDuration(XU32 playerID)
{
	XF32 duration = 0;

	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		if ((i.getValue())->m_address.getNumIpAddress() == playerID)
			duration = (i.getValue())->m_connectionDuration.getDuration();
		i.getNext();
	}
	LeaveCriticalSection(&m_playerListCS);

	return duration;
}



//*****************************************************************************
/**
Determines if a specific player is connected.

@param playerID	The player ID for which to check for a connection.
@return True is the player is connected, false otherwise.
*/
//*****************************************************************************
XBOOL XGameServer::isPlayerConnected(XU32 playerID)
{
	XBOOL isConnected = false;

	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		if ((i.getValue())->m_address.getNumIpAddress() == playerID)
			isConnected = true;
		i.getNext();
	}
	LeaveCriticalSection(&m_playerListCS);

	return isConnected;
}



//*****************************************************************************
/**
Sets the server to accept player connections or not.

@param accepting	True to allow the server to accept player connections,
					false to ignore player connections.
@param A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::setAcceptingConnections(XBOOL accepting)
{
	m_acceptingConnections = accepting;
	return X_OK;
}



//*****************************************************************************
/**
Returns whether or not the server is accepting player connections.

@return True if the server is accepting player connections, false otherwise.
*/
//*****************************************************************************
XBOOL XGameServer::getAcceptingConnections()
{
	return m_acceptingConnections;
}



//*****************************************************************************
/**
Sets the server's maximum player connection limit.

@param maxPlayers	The maximum number of player connections the server can
					have at any given point in time.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGameServer::setMaxPlayers(XU32 maxPlayers)
{
	if (maxPlayers < m_playerList.getNumItems())
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_maxPlayers = maxPlayers;
		return X_OK;
	}
}



//*****************************************************************************
/**
Gets a new packet from the server's packet pool. 

@return A pointer to the new packtet. 
*/
//*****************************************************************************
XS8* XGameServer::getNewPacket()
{
	return X_NET_USER_PKT(m_packetPool.getFreePacket());
}



//*****************************************************************************
/**
Frees a packet obtained from the getNewPacket method.  The packet is returned
to the server's memory pool.  

@param pPacket	A pointer to the packet to be freed. 
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::freePacket(XS8* pPacket)
{
	return m_packetPool.releasePacket(X_NET_SYS_PKT(pPacket));
}



//*****************************************************************************
/**
Gets the number of free packets in the server's packet pool.

@return The number of free packets in the server's packet pool.
*/
//*****************************************************************************
XU32 XGameServer::getNumFreePackets()
{
	return m_packetPool.getNumFreePackets();
}



//*****************************************************************************
/**
Gets the number of used packets in the server's memory pool.

@return The number of used packets in the server's memory pool.
*/
//*****************************************************************************
XU32 XGameServer::getNumUsedPackets()
{
	return m_packetPool.getNumUsedPackets();
}



//*****************************************************************************
/**
Broadcasts a packet from the server using the UDP protocol.  The packet is sent
with a destination address of 255.255.255.255 and is not guaranteed to reach
any specific host.

@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the server once it is sent, therefore
				the packet should not be manually freed.
@param sizeInBytes	The size of the packet.  This size cannot exceed the size
					specified in the server's description structure.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGameServer::broadcastPacket(XS8* pPacket,XU32 sizeInBytes)
{
	if (!pPacket) 
		return X_ERR_INVALID_PARAMS;

	if (m_playerList.getNumItems() == 0)
	{
		m_packetPool.releasePacket(X_NET_SYS_PKT(pPacket));
		return X_ERR_NOT_READY;
	}
	
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
Sends a reliable packet from the server to all connected players.  Packet
delivery is guaranteed through the TCP protocol.  

@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the server once it is sent, therefore
				the packet should not be manually freed.
@param sizeInBytes	The size of the packet.  This size cannot exceed the size
					specified in the server's description structure.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGameServer::sendReliablePacketToAllPlayers(XS8* pPacket,XU32 sizeInBytes)
{
	if (!pPacket) 
		return X_ERR_INVALID_PARAMS;

	if (m_playerList.getNumItems() == 0)
	{
		m_packetPool.releasePacket(X_NET_SYS_PKT(pPacket));
		return X_ERR_NOT_READY;
	}


	//Convert to system packet so we can send it and also fill in the header
	//information.
	XS8* pSendPacket = X_NET_SYS_PKT(pPacket);
	X_SET_MSG_CODE(pSendPacket,X_NMT_USER);
	X_SET_PKT_LEN(pSendPacket,sizeInBytes + X_NET_PKT_HEADER_SIZE);

	EnterCriticalSection(&m_playerListCS);

	//We set the reference count only when the player list is locked so
	//that player's aren't added or removed while trying to determine
	//the reference count for the packet.
	m_packetPool.setReferenceCount(pSendPacket,m_playerList.getNumItems());

	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		(i.getValue())->m_sendQueue.insertBack(pSendPacket);
		i.getNext();
	}

	LeaveCriticalSection(&m_playerListCS);

	return X_OK;
}



//*****************************************************************************
/**
Sends a reliable packet from the server to a specific player.  Packet
delivery is guaranteed through the TCP protocol.  

@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the server once it is sent, therefore
				the packet should not be manually freed.
@param sizeInBytes	The size of the packet.  This size cannot exceed the size
					specified in the server's description structure.
@param playerID	The player ID of the player who should receive the packet.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGameServer::sendReliablePacket(XS8* pPacket,XU32 sizeInBytes,
										XU32 playerID)
{
	if (!pPacket) 
		return X_ERR_INVALID_PARAMS;

	if (m_playerList.getNumItems() == 0)
	{
		m_packetPool.releasePacket(X_NET_SYS_PKT(pPacket));
		return X_ERR_NOT_READY;
	}

	XS8* pSendPacket = X_NET_SYS_PKT(pPacket);
	X_SET_MSG_CODE(pSendPacket,X_NMT_USER);
	X_SET_PKT_LEN(pSendPacket,sizeInBytes + X_NET_PKT_HEADER_SIZE);
	XStatus status = X_ERR_OPERATION_FAILED;
	
	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		if ((i.getValue())->m_address.getNumIpAddress() == playerID)
		{
			i.getValue()->m_sendQueue.insertBack(pSendPacket);
			status = X_OK;
			break;
		}
		i.getNext();
	}
	LeaveCriticalSection(&m_playerListCS);

	return status;
}



//*****************************************************************************
/**
Internally broadcasts a packet from the server using the UDP protocol.  The
packet is sent with a destination address of 255.255.255.255 and is not
guaranteed to reach any specific host.  This method assumes the packet is
ready to be sent.
 
@param pPacket	A pointer to the packet to be sent.  This packet must have
				been obtained from the getFreePacket method.  The packet will
				be automatically freed by the server once it is sent, therefore
				the packet should not be manually freed.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XGameServer::internalBroadcastPacket(XS8* pPacket)
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
Gets the server's network address.

@param address	A variable that will be filled with the server's network address.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::getServerNetAddress(XNetAddress& address)
{
	address = m_serverNetAddress;
	return X_OK;
}
	


//*****************************************************************************
/**
Registers an event callback with the server.  The server will notify the
application via this callback when events occur. 

@param eventCallback	A pointer to a function callback.  If this parameter is
						NULL, the current callback is unregistered.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::registerEventCallback(X_NET_MSG_ACTION_TYPE (*eventCallback)
											(X_NET_MSG_TYPE msgType,XS8* pPacket,
											 XU32 packetLength,XNetAddress& fromAddress,
											 XU32 playerID))
{
	m_eventCallback = eventCallback;
	return X_OK;
}



//*****************************************************************************
/**
Closes a player's connection.

@param playerID	The ID of the player whose connection will be closed.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::disconnectPlayer(XU32 playerID)
{		
	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		if ((i.getValue())->m_address.getNumIpAddress() == playerID) 
		{
			i.getValue()->m_connState = X_NCS_INVALID;
		}
		i.getNext();
	}
	LeaveCriticalSection(&m_playerListCS);

	return X_OK;
}

	
	
//*****************************************************************************
/**
Destroys a player and frees their associated resources in the server.

@param pPlayer	A pointer to the player who will be destroyed.
@return None.
*/
//*****************************************************************************
void XGameServer::destroyPlayer(XNetHost*& pPlayer)
{
	//NOTE: This function can only be called when the player list is locked
	//from multiple thread entry.

	XVP::NetworkMgr.destroyTCPConnection(pPlayer->m_connHandle);

	//Return any remaining packets in the player's queue to the server's packet
	//pool.  Since the packets won't be sent, they won't be freed by the
	//send thread, so we must do it here.
	while (pPlayer->m_sendQueue.getNumItems() != 0)
	{
		m_packetPool.releasePacket(pPlayer->m_sendQueue.getFront());
		pPlayer->m_sendQueue.removeFront();
	}

	XDelete(pPlayer);
	--m_numCurrentPlayers;
}

	
	
//*****************************************************************************
/**
Validates a player's information and determines if they are compatible with
the server.

@param pPacket	The packet containing information about the player who wants
				to connect to the server.
@param pPlayer	The player who wants to connect.
@return True if the player is compatible, false otherwise. 
*/
//*****************************************************************************
XBOOL XGameServer::validatePlayer(XS8* pPacket,XNetHost* pPlayer)
{
	//First, check for a matching session ID.  If the session ID doesn't match,
	//the don't even consider allowing the player to connect.
	if (!isSessionIDValid(pPacket + X_PKT_DATA_IDX))
		return false;

	//Set the player name.
	XS8* pPlayerNameData = pPacket + X_PKT_DATA_IDX + pPacket[X_PKT_DATA_IDX] + 1;
	if ((pPlayerNameData[0] <= X_MAX_PLAYER_NAME_SIZE) && (pPlayerNameData[0] > 0))
	{
		memcpy(pPlayer->m_name,pPlayerNameData+1,XU32(pPlayerNameData[0]));
		pPlayer->m_name[pPlayerNameData[0]] = '\0';
	}
	
	return true;
}



//*****************************************************************************
/**
Processes and responds to a server query from the network.  

@param playerAddress	The network address of the player who is querying for
						servers.
@param pPacket	The query packet.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGameServer::handleServerQuery(XNetAddress& playerAddress,XS8* pPacket)
{
	//First, we need to see if the server should respond to this query.  We
	//only respond if the session ID in the query packet matches the
	//server's session ID.
	if (isSessionIDValid(pPacket + X_PKT_DATA_IDX))
	{
		//Notify the user that the query will be responded to. 
		if (m_eventCallback)
			m_eventCallback(X_NMT_SERVER_ANNOUNCE,X_NET_USER_PKT(pPacket),0,playerAddress,
							playerAddress.getNumIpAddress());

		//The server should respond to this query.
		XS8* pNewPacket = m_packetPool.getFreePacket();
		XS8* pInsert = pNewPacket + X_PKT_DATA_IDX;
		XU32 dataLength = 0;
		
		X_SET_MSG_CODE(pNewPacket,X_NMT_SERVER_ANNOUNCE);

		//Put the session ID into the packet.
		XU32 length = strlen(m_sessionID);
		*pInsert++ = length;
		memcpy(pInsert,m_sessionID,length);
		pInsert += length;
		dataLength += length + 1;

		//Put the server name into the packet.
		length = strlen(m_serverName);
		*pInsert++ = length;
		memcpy(pInsert,m_serverName,length);
		pInsert += length;
		dataLength += length + 1;

		X_SET_PKT_LEN(pNewPacket,X_NET_PKT_HEADER_SIZE + dataLength);

		//Finally, send the announcement.
		XU32 sentDataLength = 0;
		XVP::NetworkMgr.sendUDPData(m_serverBroadcastConnection,pNewPacket,
							   X_GET_PKT_LEN(pNewPacket),sentDataLength,
							   playerAddress);
		m_packetPool.releasePacket(pNewPacket);
	}

	//Free query packet.
	m_packetPool.releasePacket(pPacket);

	return X_OK;
}



//*****************************************************************************
/**
Determines if a player exists in the server's list of connected players.

@param playerAddress	The address of the player to search the list for.
@return True if the player exists in the list, false otherwise.
*/
//*****************************************************************************
XBOOL XGameServer::isPlayerInPlayerList(XNetAddress& playerAddress)
{
	XBOOL result = false;

	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		if (i.getValue()->m_address.getNumIpAddress() == playerAddress.getNumIpAddress())
		{
			result = true;
			break;
		}
		i.getNext();
	}
	LeaveCriticalSection(&m_playerListCS);

	return result;
}



//*****************************************************************************
/**
Compares a session ID with the server's session ID to see if they match.

@param pData	A pointer to a buffer where the session ID is stored.
@return True if the passed in session ID matches the server's session ID,
		false otherwise.
*/
//*****************************************************************************
XBOOL XGameServer::isSessionIDValid(XS8* pData)
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
This method is the main entry point for the server's outgoing packet thread.
It's responsible for sending packets from the send queues and updating connected
players.

@return None.
*/
//*****************************************************************************
void XGameServer::outgoingPacketPump()
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
	XMemBlockList<XNetHost*>::iterator i;
	XU32 sentDataLength = 0;
	XBOOL playerRemoved = false;

	while (m_broadcastQueue.getNumItems() != 0)
	{
		//Get the packet and broadcast it.
		pPacket = m_broadcastQueue.getFront();
		m_broadcastQueue.removeFront();
		
		//Insert the next sequence number.
		X_SET_SEQ_NUM(pPacket,m_nextBroadcastSeqNum);
		m_nextBroadcastSeqNum++;
		XVP::NetworkMgr.sendUDPData(m_serverBroadcastConnection,pPacket,X_GET_PKT_LEN(pPacket),
							   sentDataLength,m_broadcastAddress);			
		m_packetPool.releasePacket(pPacket);
	}		

	LeaveCriticalSection(&m_broadcastQueueCS);

	//Now process each of the player's state machines
	XS8* pNewPacket = NULL;

	EnterCriticalSection(&m_playerListCS);
	i.set(m_playerList.begin());
	while (i.isValid())
	{
		switch(i.getValue()->m_connState)
		{
		case X_NCS_WAITING:
			break;

		case X_NCS_INVALID:
			//This player is in the invalid state, so delete it.  Later,
			//we will remove the NULL pointer left behind.

			//Notify the user that the connection is being closed.  
			if (m_eventCallback)
				m_eventCallback(X_NMT_CONNECTION_CLOSED,NULL,0,i.getValue()->m_address,
								i.getValue()->m_address.getNumIpAddress());
			destroyPlayer(i.getValue());
			playerRemoved = true;
			i.getNext();
            continue;

		case X_NCS_VALIDATION:
				pNewPacket = m_packetPool.getFreePacket();
				X_SET_MSG_CODE(pNewPacket,X_NMT_VALIDATION_REQUEST);
				X_SET_PKT_LEN(pNewPacket,X_NET_PKT_HEADER_SIZE);
				if (m_password[0] != '\0')
				{
					X_SET_FLAGS_BIT5(pNewPacket);
				}
				else
				{
					X_CLEAR_FLAGS_BIT5(pNewPacket);
				}
				
				//Change the player state to show that we are waiting for a player
				//response.  Also, start the wait timer.
				i.getValue()->m_connState = X_NCS_WAITING;

				//Queue the packet.
				i.getValue()->m_sendQueue.insertBack(pNewPacket);
			break;

		case X_NCS_CONNECT:
				//Inform the player that the connection was accepted and they
				//have been logged on to the server.
				pNewPacket = m_packetPool.getFreePacket();
				X_SET_MSG_CODE(pNewPacket,X_NMT_CONNECTION_ACCEPTED);
				X_SET_PKT_LEN(pNewPacket,X_NET_PKT_HEADER_SIZE);

				//Reset the duration timer.
				i.getValue()->m_connectionDuration.reset();
				++m_numCurrentPlayers;

				//Queue the packet.
				i.getValue()->m_sendQueue.insertBack(pNewPacket);
				
				//Notify the user that the connection was accepted.  
				if (m_eventCallback)
					m_eventCallback(X_NMT_CONNECTION_ACCEPTED,NULL,0,i.getValue()->m_address,
									i.getValue()->m_address.getNumIpAddress());				

				//This player should transition.
				i.getValue()->m_connState = X_NCS_CURRENT;
			break;

		case X_NCS_DISCONNECT:
				//Inform the player that the connection will be disconnected.
				pNewPacket = m_packetPool.getFreePacket();
				X_SET_MSG_CODE(pNewPacket,X_NMT_CONNECTION_REJECTED);
				X_SET_PKT_LEN(pNewPacket,X_NET_PKT_HEADER_SIZE);

				//Queue the packet.
				i.getValue()->m_sendQueue.insertBack(pNewPacket);
				
				//Notify the user that the connection was rejected.  
				if (m_eventCallback)
					m_eventCallback(X_NMT_CONNECTION_REJECTED,NULL,0,i.getValue()->m_address,
									i.getValue()->m_address.getNumIpAddress());
				
				//This player should transition to invalid.
				i.getValue()->m_connState = X_NCS_INVALID;
			break;
				
		case X_NCS_CURRENT:
			break;
		}

		//Now that we've processed the player's state machine for this time step,
		//let's process its send queue.
		while (i.getValue()->m_sendQueue.getNumItems() != 0)
		{
			//Check the status of the last send operation.
			if (!i.getValue()->m_packetizer.canSendNewPacket())
			{
				i.getValue()->m_packetizer.flushCurrentSendPacket();
				break;
			}
			else
			{
				//Get the packet and send it.
				pPacket = i.getValue()->m_sendQueue.getFront();
				i.getValue()->m_sendQueue.removeFront();
				i.getValue()->m_packetizer.sendPacket(pPacket,X_GET_PKT_LEN(pPacket));
				m_packetPool.releasePacket(pPacket);		
			}		
		}
		i.getNext();
	}

	//If there were any player's in the invalid state during the loop above,
	//they have been removed but there is still a NULL pointer in the player
	//for each one.  Let's remove them.
	if (playerRemoved)
		m_playerList.remove(NULL);

	LeaveCriticalSection(&m_playerListCS);
}



//*****************************************************************************
/**
This method is the main entry point for the server's incoming packet thread.
It's responsible for receiving packets and processing them appropriately.

@return None.
*/
//*****************************************************************************
void XGameServer::incomingPacketPump()
{
	//Check to see if we are supposed to kill this thread.
	if (!m_receivePacketThreadOn)
	{
		SetEvent(m_workerThreadEvents[1]);
		ExitThread(0);
	}

	XS8* pPacket = NULL;
	XU32 packetLength = 0;
	X_NET_MSG_ACTION_TYPE retAction;
	XStatus status;

	//See if a packet has arrived on the player's port.
	EnterCriticalSection(&m_playerListCS);
	XMemBlockList<XNetHost*>::iterator i(m_playerList.begin());
	while (i.isValid())
	{
		status = i.getValue()->m_packetizer.receivePacket(pPacket,packetLength);
		switch (status)
		{
		case X_ERR_NOT_READY:
			//No new packet has arrived so just skip this player.
			i.getNext();
			continue;
			break;
		case X_ERR_NOT_RUNNING:
			//The connection has been closed.
			i.getValue()->m_connState = X_NCS_INVALID;
			break;
		case X_OK:
			//A new packet has arrived and is ready to be processed.
			//Let's see what type of packet this is.
			switch (pPacket[0])
			{
			case X_NMT_USER:
				//If the user has registered a callback, then send the packet to the callback.
				//By default, when no callback is registered, we send the packet to the players.
				if (m_eventCallback)
				{
					retAction = m_eventCallback(X_NMT_USER,X_NET_USER_PKT(pPacket),
												packetLength - X_NET_PKT_HEADER_SIZE,i.getValue()->m_address,
												i.getValue()->m_address.getNumIpAddress());
					
					//If the user wants to forward the packet, then send it, otherwise, drop it.
					if (retAction == X_NMAT_FORWARD)
						internalBroadcastPacket(pPacket);
					else
						m_packetPool.releasePacket(pPacket);
				}
				else 
					internalBroadcastPacket(pPacket);
				
				break;

			case X_NMT_VALIDATION_RESPONSE:
				if (validatePlayer(pPacket,i.getValue()))
					i.getValue()->m_connState = X_NCS_CONNECT;
				else
					i.getValue()->m_connState = X_NCS_DISCONNECT;
				break;

			default:
				//Don't know what kind of packet this is.
				break;
			}

			break;
	
		}
		i.getNext();
	}
					
	LeaveCriticalSection(&m_playerListCS);


	//Check the server's broadcast port to see if there are any server queries.
	XNetAddress fromAddress;
	XU32 returnDataLength = 0;
	pPacket = m_packetPool.getFreePacket();
	XVP::NetworkMgr.receiveUDPData(m_serverBroadcastConnection,pPacket,m_packetSize,returnDataLength,
	 					   fromAddress);
	if (returnDataLength > 0)
	{
		//Let's see what type of packet this is.
		switch (pPacket[0])
		{
		case X_NMT_SERVER_QUERY:
			//Someone is querying for games on the network.
			handleServerQuery(fromAddress,pPacket);
			break;
		default:
			//Don't know what kind of packet this is, so drop it and return the
			//packet to the pool for reuse.
			m_packetPool.releasePacket(pPacket);
			break;
		}
	}
	else
		m_packetPool.releasePacket(pPacket);
}



//*****************************************************************************
//*****************************************************************************
DWORD WINAPI X_GameServerSendPacketPumper(LPVOID pParam)
{
	XGameServer* pGameServer = (XGameServer*)pParam;

	while(true)
	{	
		Sleep(1);
		pGameServer->outgoingPacketPump();
	}

	return 0;
}


//*****************************************************************************
//*****************************************************************************
DWORD WINAPI X_GameServerReceivePacketPumper(LPVOID pParam)
{
	XGameServer* pGameServer = (XGameServer*)pParam;

	while(true)
	{	
		Sleep(1);
		pGameServer->incomingPacketPump();
	}

	return 0;
}



//*****************************************************************************
/**
Notifies the server when a new player connection is received.


*/
//*****************************************************************************
X_NET_MSG_ACTION_TYPE XGameServer::newConnectionNotify(X_NET_MSG_TYPE msgType,
													   XNetAddress& playerAddress,
													   XHandle tcpConnHandle)
{
	switch (msgType)
	{
	case X_NMT_CONNECTION_REQUEST:
		if (m_acceptingConnections)
			return X_NMAT_ACCEPT;
		else
			return X_NMAT_REJECT;
		break;

	case X_NMT_CONNECTION_ACCEPTED:
		
		//If we are under the player limit, and the player is not already in the list,
		//then add the player.
		if ((m_playerList.getNumItems() < m_maxPlayers-1) &&
			(!isPlayerInPlayerList(playerAddress)))
		{
			XNetHost* pNewPlayer = NULL;//new XNetHost();
			XNew(pNewPlayer,XNetHost,XOSMgr::getDefaultHeap());
			pNewPlayer->m_address.setPort(playerAddress.getPort());
			pNewPlayer->m_address.setIpAddressNum(playerAddress.getNumIpAddress());
			pNewPlayer->m_connHandle = tcpConnHandle;
			pNewPlayer->m_packetizer.init(tcpConnHandle,m_packetSize);
			XVP::NetworkMgr.setTCPBlockingState(tcpConnHandle,false);
			pNewPlayer->m_connState = X_NCS_VALIDATION;

			EnterCriticalSection(&m_playerListCS);
			m_playerList.insertBack(pNewPlayer);
			LeaveCriticalSection(&m_playerListCS);	
		}
	
		return X_NMAT_NO_ACTION;

		break;

	default:
		return X_NMAT_NO_ACTION;
		break;
	}
}


//*****************************************************************************
/**
Notifies the server when a player connection is received.  This function is
called when the server's TCP listener responds with information.

@param msgType	The type of message the TCP listener is reporting.
@param playerAddress	The network address of the player the message is about.
@param tcpConnHandle	The TCP connection index created by the listener.  This 
					connection can be used to communicate with the player.
@param pUserData	User defined data.
@return The network action to take.
*/
//*****************************************************************************
X_NET_MSG_ACTION_TYPE X_GamerServerListenCallback(X_NET_MSG_TYPE msgType,
												  XNetAddress& playerAddress,
                                                  XHandle tcpConnHandle,
												  void* pUserData)
{
	return ((XGameServer*)pUserData)->newConnectionNotify(msgType,playerAddress,
														  tcpConnHandle);
}

