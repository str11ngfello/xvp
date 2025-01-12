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
//	Module Name:	XGameServer.h
//	Author:			Lynn Duke
//	Creation Date:	8-15-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GAME_SERVER_H__
#define __X_GAME_SERVER_H__





//*****************************************************************************
//Global Functions.
//*****************************************************************************
DWORD WINAPI X_GameServerSendPacketPumper(LPVOID pParam);
DWORD WINAPI X_GameServerReceivePacketPumper(LPVOID pParam);
X_NET_MSG_ACTION_TYPE X_GamerServerListenCallback(X_NET_MSG_TYPE msgType,
												  XNetAddress& playerAddress,
                                                  XHandle tcpConnHandle,
												  void* pUserData);



//*****************************************************************************
//! Represents a server in X-VP's client/server network architecture.
/**
The XGameServer object represents the server in X-VP's client/server network
model.  It provides the functionality necessary to communicate with 
XGameClient objects.       

See the member functions below for more information.
*/
//*****************************************************************************
class XGameServer
{					  
private:
	XHandle					m_serverBroadcastConnection;
	XNetAddress				m_serverNetAddress;
	XNetAddress				m_broadcastAddress;
	XU32					m_nextBroadcastSeqNum;
	XMemBlockList<XNetHost*>	m_playerList;
	XMemBlockList<XS8*>				m_broadcastQueue;
	XPacketPool				m_packetPool;
	XBOOL					m_acceptingConnections;
	XU32					m_maxPlayers;
	XU32					m_packetSize;
	ULONG32					m_outgoingThreadID;
	ULONG32					m_incomingThreadID;
	HANDLE					m_outgoingThreadHandle; 
	HANDLE					m_incomingThreadHandle; 
	char					m_serverName[X_MAX_SERVER_NAME_SIZE+1];
	char					m_password[X_MAX_PASSWORD_SIZE+1];
	char					m_sessionID[X_MAX_SESSION_ID_SIZE+1];
	X_NET_MSG_ACTION_TYPE	(*m_eventCallback) (X_NET_MSG_TYPE msgType,XS8* pPacket,
												XU32 packetLength,XNetAddress& fromAddress,
												XU32 playerID);
	CRITICAL_SECTION		m_broadcastQueueCS;
	CRITICAL_SECTION		m_playerListCS;
	HANDLE					m_workerThreadEvents[2];					
	XBOOL					m_sendPacketThreadOn;
	XBOOL					m_receivePacketThreadOn;
	XRsa					m_serverRSA;
	XHandle					m_tcpListenerHandle;
	XU32					m_numCurrentPlayers;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus internalBroadcastPacket(XS8* pPacket);
	XBOOL validatePlayer(XS8* pPacket,XNetHost* pPlayer);
	XStatus handleServerQuery(XNetAddress& playerAddress,XS8* pPacket);
	XBOOL isPlayerInPlayerList(XNetAddress& playerAddress);
	XBOOL isSessionIDValid(XS8* pData);
	void destroyPlayer(XNetHost*& pPlayer);



public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGameServer();
	~XGameServer();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pSessionID,XS8* pServerName,XS8* pPassword,XGameServerDesc& desc);
	XStatus destroy();
	XStatus disconnectPlayer(XU32 playerID);
	XStatus disconnectAllPlayers();
	XStatus setMaxPlayers(XU32 maxPlayers);
	XU32 getNumCurrentPlayers();
	XF32 getPlayerConnectionDuration(XU32 playerID);
	XBOOL isPlayerConnected(XU32 playerID);
	XStatus flushBroadcastQueue();
	XStatus setAcceptingConnections(XBOOL accepting);
	XBOOL getAcceptingConnections();
	XS8* getNewPacket();
	XStatus freePacket(XS8* pPacket);
	XU32 getNumFreePackets();
	XU32 getNumUsedPackets();
	XStatus getServerNetAddress(XNetAddress& address);
	XStatus broadcastPacket(XS8* pPacket,XU32 sizeInBytes);
	XStatus sendReliablePacket(XS8* pPacket,XU32 sizeInBytes,XU32 playerID);
	XStatus sendReliablePacketToAllPlayers(XS8* pPacket,XU32 sizeInBytes);
	XStatus registerEventCallback(X_NET_MSG_ACTION_TYPE (*eventCallback)
								 (X_NET_MSG_TYPE msgType,XS8* pPacket,
								  XU32 packetLength,XNetAddress& fromAddress,
								  XU32 playerID));
	X_NET_MSG_ACTION_TYPE newConnectionNotify(X_NET_MSG_TYPE msgType,
											  XNetAddress& playerAddress,
											  XHandle tcpConnHandle);
	void outgoingPacketPump();
	void incomingPacketPump();
};

#endif