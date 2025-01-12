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
//	Module Name:	XNetHost.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NET_HOST_H__
#define __X_NET_HOST_H__





//*****************************************************************************
//*****************************************************************************
class XNetHost
{
public:
	XU32						m_lastSeqNumReceived;
	XU32						m_nextTransmitSeqNum;
	char						m_name[X_MAX_PLAYER_NAME_SIZE+1];
	XNetAddress					m_address;
	X_NET_CONNECTION_STATE		m_connState;
	XTimer						m_connectionDuration;
	XTCPPacketizer				m_packetizer;
	XHandle						m_connHandle;
	XMemBlockList<XS8*>				m_sendQueue;

	XNetHost()
	{
		m_connState = X_NCS_INVALID;
		m_lastSeqNumReceived = 0;
		m_nextTransmitSeqNum = 1;
		m_connectionDuration.start();
		m_connHandle = X_INVALID_HANDLE;
	}

	~XNetHost()
	{
		m_sendQueue.clear();
	}
};

#endif

