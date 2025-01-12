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
//	Module Name:	XTCPPacketizer.cpp
//	Author:			Lynn Duke
//	Creation Date:	9-17-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the TCP packetizer.
*/
//*****************************************************************************
XTCPPacketizer::XTCPPacketizer()
{
	m_tcpConnHandle = X_INVALID_HANDLE;
	m_maxPacketSizeInBytes = 0;
	m_packetReceiveState = X_PRS_RCV_LEN;
	m_receivePacketSize = 0;
	m_bytesLeftToRead = sizeof(XU32);
	m_tmpBytesRead = 0;
	m_receiveStatus = X_OK;
	m_pReceiveBuffer = NULL;
	m_pSendBuffer = NULL;
	m_sending = false;
	m_sendPacketSize = 0;
	m_bytesLeftToSend = 0;
	m_tmpBytesSent = 0;
	m_sendStatus = X_OK;
}



//*****************************************************************************
/**
Destructor for the TCP packetizer.
*/
//*****************************************************************************
XTCPPacketizer::~XTCPPacketizer()
{
	destroy();
}



//*****************************************************************************
/**
Initializes the TCP packetizer with a TCP connection handle and the maximum
packet size it should expect to send or receive.  

@param tcpConnHandle	An TCP connection handle obtained from the Network Driver.
@param maxPacketSizeInBytes	The maximum packet size in bytes that the TCP
							packetizer should expect to send or receive.
@return A return code of type XStatus. 
*/
//*****************************************************************************
XStatus XTCPPacketizer::init(XHandle tcpConnHandle,XU32 maxPacketSizeInBytes)
{
	//Cleanup any previous data.
	destroy();
	XTCPPacketizer();

	//Make sure we have valid parameters.
	if ((tcpConnHandle == X_INVALID_HANDLE) || (maxPacketSizeInBytes == 0) ||
		(maxPacketSizeInBytes > X_TCPP_MAX_PKT_SIZE))
		return X_ERR_INVALID_PARAMS;

	m_maxPacketSizeInBytes = maxPacketSizeInBytes;
	m_tcpConnHandle = tcpConnHandle;

	//Allocate storage buffers.  Buffer length should be the max packet size
	//plus size of a XU32 in bytes
	XNewArray(m_pSendBuffer,XS8,maxPacketSizeInBytes + sizeof(XU32),XOSMgr::getDefaultHeap());
	if (!m_pSendBuffer)
		return X_ERR_OPERATION_FAILED;
	XNewArray(m_pReceiveBuffer,XS8,maxPacketSizeInBytes + sizeof(XU32),XOSMgr::getDefaultHeap());
	if (!m_pReceiveBuffer)
	{
		XDeleteArray(m_pSendBuffer);
		return X_ERR_OPERATION_FAILED;
	}
	
	return X_OK;
}



//*****************************************************************************
/**
Destroys the TCP packetizer by freeing all allocated resources.

@return None.
*/
//*****************************************************************************
XStatus XTCPPacketizer::destroy()
{
	//Destroy the storage buffers.
	XDeleteArray(m_pSendBuffer);
	XDeleteArray(m_pReceiveBuffer);

	return X_OK;
}



//*****************************************************************************
/**
Receives a packet by causing the TCP packetizer to try and assemble a packet
from the bytes streaming in from the TCP connection.  

@param pPacket	A variable to hold a pointer to the received packet.  This 
				parameter will be valid once a full packet has been received.
@param packetSizeInBytes	The size of the received packet.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTCPPacketizer::receivePacket(XS8*& pPacket,XU32& packetSizeInBytes)
{
	switch (m_packetReceiveState)
	{
	case X_PRS_RCV_LEN:
		
		m_receiveStatus = XVP::NetworkMgr.receiveTCPData(m_tcpConnHandle,((XS8*)&m_receivePacketSize) +
						(sizeof(XU32) - m_bytesLeftToRead),m_bytesLeftToRead,m_tmpBytesRead);
		
		//If there was an error reading from the connection, then return.
		if (m_receiveStatus != X_OK)
			return m_receiveStatus;
		
		//Reduce the number of bytes we are waiting to read.
		m_bytesLeftToRead -= m_tmpBytesRead;

		//If we haven't received all of the bytes for the length field, then break
		//and wait for the function to be called again.
		if (m_bytesLeftToRead != 0)
			break;

		//Make sure the length field will not cause a buffer overflow.  If it will,
		//then truncate the size.
		if (m_receivePacketSize > m_maxPacketSizeInBytes)
			m_receivePacketSize = m_maxPacketSizeInBytes;
		m_packetReceiveState = X_PRS_RCV_DATA;
		m_bytesLeftToRead = m_receivePacketSize;

	case X_PRS_RCV_DATA:

		m_receiveStatus = XVP::NetworkMgr.receiveTCPData(m_tcpConnHandle,m_pReceiveBuffer +
						(m_receivePacketSize - m_bytesLeftToRead),m_bytesLeftToRead,m_tmpBytesRead);

		//If there was an error reading from the connection, then return.
		if (m_receiveStatus != X_OK)
			return m_receiveStatus;

		//Reduce the number of bytes we are waiting to read.
		m_bytesLeftToRead -= m_tmpBytesRead;

		//If we haven't received all of the bytes for the data, then break
		//and wait for the function to be called again.
		if (m_bytesLeftToRead != 0)
			break;	
	
		m_packetReceiveState = X_PRS_PKT_READY;
		m_bytesLeftToRead = sizeof(XU32);
		break;

	default:
		break;
	}

	if (m_packetReceiveState == X_PRS_PKT_READY)
	{
		pPacket = m_pReceiveBuffer;
		packetSizeInBytes = m_receivePacketSize;
		m_packetReceiveState = X_PRS_RCV_LEN;
		return X_OK;
	}
	else
		return X_ERR_NOT_READY;
}



//*****************************************************************************
/**
Sends a packet.  

@param pPacket	A pointer to the packet data.
@param packetSizeInBytes	The packet size in bytes.  This size must be less
							than or equal to the max packet size passed to
							the init method.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTCPPacketizer::sendPacket(XS8* pPacket,XU32 packetSizeInBytes)
{
	//Are we currently sending a packet?  If so, bail.
	if (m_sending)
		return X_ERR_NOT_READY;

	//Validate parameters.
	if ((!pPacket) || (packetSizeInBytes > m_maxPacketSizeInBytes))
		return X_ERR_INVALID_PARAMS;

	//Let's start the sending process.  First, copy the user's packet into
	//our own send buffer.  Make sure to prepend the length field.  Then
	//set the total packet size.
	*((XU32*)m_pSendBuffer) = packetSizeInBytes;
	memcpy(m_pSendBuffer + sizeof(XU32),pPacket,packetSizeInBytes);
	m_sendPacketSize = m_bytesLeftToSend = packetSizeInBytes + sizeof(XU32);

	//Try to send the packet.
	m_sending = true;
	return sendNextDataChunk();
}



//*****************************************************************************
/**
Determines if the TCP packetizer is ready to send a new packet.

@return True if the packetizer can send a new packet, false if it cannot.
*/
//*****************************************************************************
XBOOL XTCPPacketizer::canSendNewPacket()
{
	return !m_sending;
}



//*****************************************************************************
/**
Flushes the packetizer's send queue.  When a packet is sent through the TCP
packetizer, it may or may not be sent completely.  If it is not sent completely
during the initial send call, the packet must be flushed until the TCP 
packetizer is ready to send a new packet.  This indicates that the last send
was successful.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTCPPacketizer::flushCurrentSendPacket()
{
	if (m_sending)
        return sendNextDataChunk();
	else
		return X_OK;
}



//*****************************************************************************
/**
Sends the next data chunk of a packet.  This function is called internally
by the TCP packetizer.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTCPPacketizer::sendNextDataChunk()
{
	//Try to send the packet.
	m_sendStatus = XVP::NetworkMgr.sendTCPData(m_tcpConnHandle,m_pSendBuffer + 
		(m_sendPacketSize - m_bytesLeftToSend),m_bytesLeftToSend,m_tmpBytesSent);

	//If there was an error sending to the connection, then return.
	if (m_sendStatus != X_OK)
		return m_sendStatus;

	//Reduce the number of bytes we are waiting to send.
	m_bytesLeftToSend -= m_tmpBytesSent;

	//Are we done sending?  If so, set the sending state to reflect that.
	if (m_bytesLeftToSend == 0)
		m_sending = false;
		
	return X_OK;
}



//*****************************************************************************
/**
Sets the TCP packetizer connection handle.  The TCP connection handle must be
open and ready to send and receive data.

@param tcpConnHandle	A TCP connection handle from the Network Driver.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTCPPacketizer::setTCPConnectionHandle(XHandle tcpConnHandle)
{
	m_tcpConnHandle = tcpConnHandle;
	return X_OK;
}



//*****************************************************************************
/**
Returns the packetizers TCP connection handle.

@return The packetizers TCP connection handle.
*/
//*****************************************************************************
XHandle XTCPPacketizer::getTCPConnectionHandle()
{
	return m_tcpConnHandle;
}
