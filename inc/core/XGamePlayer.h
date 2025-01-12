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
//	Module Name:	XGamePlayer.h
//	Author:			Lynn Duke
//	Creation Date:	8-15-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GAME_PLAYER_H__
#define __X_GAME_PLAYER_H__



//*****************************************************************************
//Global Thread Functions.
//*****************************************************************************
DWORD WINAPI X_GamePlayerSendPacketPumper(LPVOID pParam);
DWORD WINAPI X_GamePlayerReceivePacketPumper(LPVOID pParam);



//*****************************************************************************
//! Represents a client in X-VP's client/server network architecture.
/**
The XGamePlayer object represents the client in X-VP's client/server network
model.  It provides the functionality necessary to communicate with 
XGameServer objects.       

See the member functions below for more information.
*/
//*****************************************************************************
class XGamePlayer
{					  
	XHandle					m_playerBroadcastConnection;
	XNetAddress				m_localNetAddress;
	XMemBlockList<XS8*>		m_broadcastQueue;
	XMemoryPool				m_packetPool;
	XU32					m_packetSize;
	ULONG32					m_outgoingThreadID;
	ULONG32					m_incomingThreadID;
	HANDLE					m_outgoingThreadHandle; 
	HANDLE					m_incomingThreadHandle; 
	char					m_playerName[X_MAX_PLAYER_NAME_SIZE+1];
	char					m_returnPasswordBuffer[X_MAX_PASSWORD_SIZE+1];
	char					m_sessionID[X_MAX_SESSION_ID_SIZE+1];
	X_NET_MSG_ACTION_TYPE	(*m_eventCallback) (X_NET_MSG_TYPE msgType,XS8* pPacket,
												XU32 packetLength,XNetAddress& fromAddress);
	CRITICAL_SECTION		m_broadcastQueueCS;
	CRITICAL_SECTION		m_serverCS;
	HANDLE					m_workerThreadEvents[2];					
	XBOOL					m_sendPacketThreadOn;
	XBOOL					m_receivePacketThreadOn;
	XRsa					m_playerRSA;
	XNetHost				m_server;


	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus internalBroadcastPacket(XS8* pPacket);
	XBOOL isSessionIDValid(XS8* pData);
	XStatus internalDisconnect();
	void buildValidationResponse(XS8* pPacket);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGamePlayer();
	~XGamePlayer();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pPlayerName,XS8* pSessionID,XGamePlayerDesc& desc);
	XStatus destroy();
	XStatus flushBroadcastQueue();
	XS8* getNewPacket();
	XStatus freePacket(XS8* pPacket);
	XU32 getNumFreePackets();
	XU32 getNumUsedPackets();
	XStatus sendReliablePacket(XS8* pPacket,XU32 sizeInBytes);
	XStatus broadcastPacket(XS8* pPacket,XU32 sizeInBytes);
	XStatus getLocalNetAddress(XNetAddress& address);
	XStatus getServerNetAddress(XNetAddress& address);
	XStatus registerEventCallback(X_NET_MSG_ACTION_TYPE (*eventCallback)
								 (X_NET_MSG_TYPE msgType,XS8* pPacket,
								  XU32 packetLength,XNetAddress& fromAddress));
	XStatus queryForServers();
	XStatus connect(XNetAddress& localNetAddress,XNetAddress& serverNetAddress);
	XStatus disconnect();
	XBOOL isConnected();
	void outgoingPacketPump();
	void incomingPacketPump();
};

#endif