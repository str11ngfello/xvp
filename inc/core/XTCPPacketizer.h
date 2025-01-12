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
//	Module Name:	XTCPPacketizer.h
//	Author:			Lynn Duke
//	Creation Date:	9-17-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TCPPACKETIZER_H__
#define __X_TCPPACKETIZER_H__




//A limit has to be chosen.  We choose roughly 10 Megs, but in reality, packet
//sizes should be much smaller.
#define X_TCPP_MAX_PKT_SIZE		10000000


//*****************************************************************************
//! Sends/Receives logical packets over TCP connections.
/**
The XTCPPacketizer object is a utility that sends logical packets over TCP
connections.  Since TCP is a stream-oriented protocol, there is no way to
tell when a complete, logical chunk of data has been received.  The
XTCPPacketizer uses a small amount of meta data to describe byte streams which
allows the user to send and recieve packets without having to perform message
reconstruction.

See the member functions below for more information.
*/

//*****************************************************************************
class XTCPPacketizer
{
private:

	enum X_PKT_RCV_STATE
	{
		X_PRS_RCV_LEN,
		X_PRS_RCV_DATA,
		X_PRS_PKT_READY
	};

	XHandle m_tcpConnHandle;
	XU32 m_maxPacketSizeInBytes;
	X_PKT_RCV_STATE m_packetReceiveState;
	XU32 m_receivePacketSize;
	XS8* m_pReceiveBuffer;
	XS8* m_pSendBuffer;
	XU32 m_bytesLeftToRead;
	XU32 m_tmpBytesRead;
	XStatus m_receiveStatus;
	XBOOL m_sending;

	XU32 m_sendPacketSize;
	XU32 m_bytesLeftToSend;
	XU32 m_tmpBytesSent;
	XStatus m_sendStatus;

	//*****************************************************************************
	//Private Interface.
	//*****************************************************************************
	XStatus sendNextDataChunk();

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTCPPacketizer();
	~XTCPPacketizer();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XHandle tcpConnHandle,XU32 maxPacketSizeInBytes);
	XStatus destroy();
	XStatus receivePacket(XS8*& pPacket,XU32& packetSizeInBytes);
	XStatus sendPacket(XS8* pPacket,XU32 packetSizeInBytes);
	XBOOL canSendNewPacket();
	XStatus flushCurrentSendPacket();
	XStatus setTCPConnectionHandle(XHandle tcpConnHandle);
	XHandle getTCPConnectionHandle();
};


#endif