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
//	Module Name:	XNetGlobals.h
//	Author:			Lynn Duke
//	Creation Date:	9-5-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NET_GLOBALS_H__
#define __X_NET_GLOBALS_H__

#ifdef XSP_WINDOWS
	#include <winsock2.h>
	typedef int socklen_t;
#else
	#include <string.h>
	#include <memory.h>
	#include <errno.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <sys/socket.h>
	#include <sys/time.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	typedef int SOCKET;
	#define INVALID_SOCKET	-1 
#endif

/** @defgroup NetGlobals NetGlobals
These are network globals.
@{
*/


//*****************************************************************************
// Packet Header Format
//  
// XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX 
//  XU8 0   XU8 1   XU8 2   XU8 3   XU8 4   XU8 5   XU8 6   XU8 7
// |______| |______| |_________________________________| |_______________|
//    |        |                      |                          |
//	MSG CODE   |                      |                          |
//             |                      |                          |
//           FLAGS                    |                     DATA LENGTH
//                          PACKET SEQUENCE NUMBER
//
//
// MSG CODE - This byte is a code from 0 to 255 that indicates what type of
//            message is contained in the packet data.
// 
// FLAGS - These are bit flags used for various purposes, see below.
//			
//        BIT 7  BIT 6  BIT 5  BIT 4  BIT 3  BIT 2  BIT 1  BIT 0
//			X      X      X      X      X      X      X      X
//			|      |      |      |      |      |      |      |
//          |      |      |      |      |      |      |      |        
//          |      |      |      |      |      |      |      Reserved
//          |      |      |      |      |      |      Reserved
//          |      |      |      |      |      Reserved
//          |      |      |      |      Reserved
//          |      |      |      Reserved
//          |      |      Password Required in Response
//          |      Encrypted
//          Ack Requested
//
//
// PACKET SEQUENCE NUMBER - Sequence number identifying the order of packets.
//
// DATA LENGTH - The length of the packet in bytes.
//
//*****************************************************************************

//Packet field indicies.
#define X_PKT_MSG_CODE_IDX			0
#define X_PKT_FLAGS_IDX				1
#define X_PKT_SEQ_NUM_IDX			2
#define X_PKT_LEN_IDX				6
#define X_PKT_DATA_IDX				8

//Packet field setters/getters.
#define X_SET_MSG_CODE(pPacket,msgCode)			{pPacket[X_PKT_MSG_CODE_IDX] = msgCode;}			
#define X_SET_ACK_REQ_BIT(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x80;}				
#define X_SET_ENCRYPTED_BIT(pPacket)			{pPacket[X_PKT_FLAGS_IDX] |= 0x40;}			
#define X_SET_FLAGS_BIT5(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x20;}				
#define X_SET_FLAGS_BIT4(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x10;}				
#define X_SET_FLAGS_BIT3(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x08;}				
#define X_SET_FLAGS_BIT2(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x04;}				
#define X_SET_FLAGS_BIT1(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x02;}				
#define X_SET_FLAGS_BIT0(pPacket)				{pPacket[X_PKT_FLAGS_IDX] |= 0x01;}				
#define X_CLEAR_ACK_REQ_BIT(pPacket)			{pPacket[X_PKT_FLAGS_IDX] &= 0x7F;}				
#define X_CLEAR_ENCRYPTED_BIT(pPacket)			{pPacket[X_PKT_FLAGS_IDX] &= 0xBF;}			
#define X_CLEAR_FLAGS_BIT5(pPacket)				{pPacket[X_PKT_FLAGS_IDX] &= 0xDF;}				
#define X_CLEAR_FLAGS_BIT4(pPacket)				{pPacket[X_PKT_FLAGS_IDX] &= 0xEF;}				
#define X_CLEAR_FLAGS_BIT3(pPacket)				{pPacket[X_PKT_FLAGS_IDX] &= 0xF7;}				
#define X_CLEAR_FLAGS_BIT2(pPacket)				{pPacket[X_PKT_FLAGS_IDX] &= 0xFB;}				
#define X_CLEAR_FLAGS_BIT1(pPacket)				{pPacket[X_PKT_FLAGS_IDX] &= 0xFD;}				
#define X_CLEAR_FLAGS_BIT0(pPacket)				{pPacket[X_PKT_FLAGS_IDX] &= 0xFE;}				
#define X_SET_SEQ_NUM(pPacket,seqNum)			{(*((XU32*)(pPacket+X_PKT_SEQ_NUM_IDX))) = seqNum;}					
#define X_SET_PKT_LEN(pPacket,length)			{(*((XU16*)(pPacket+X_PKT_LEN_IDX))) = length;}			

#define X_GET_MSG_CODE(pPacket)					(pPacket[X_PKT_MSG_CODE_IDX])
#define X_GET_ACK_REQ_BIT(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x80)
#define X_GET_ENCRYPTED_BIT(pPacket)			(pPacket[X_PKT_FLAGS_IDX] & 0x40)		
#define X_GET_FLAGS_BIT5(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x20)				
#define X_GET_FLAGS_BIT4(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x10)				
#define X_GET_FLAGS_BIT3(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x08)				
#define X_GET_FLAGS_BIT2(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x04)				
#define X_GET_FLAGS_BIT1(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x02)				
#define X_GET_FLAGS_BIT0(pPacket)				(pPacket[X_PKT_FLAGS_IDX] & 0x01)				
#define X_GET_SEQ_NUM(pPacket)					(*((XU32*)(pPacket+X_PKT_SEQ_NUM_IDX)))
#define X_GET_PKT_LEN(pPacket)					(*((XU16*)(pPacket+X_PKT_LEN_IDX)))


//Packet writing utilities
#define X_WRITE_BYTE(pPacket,data)		{((XS8*)pPacket) = data;pPacket += sizeof(XU8);}
#define X_WRITE_USHORT(pPacket,data)	{((XU16*)pPacket) = data;pPacket += sizeof(XU16);}
#define X_WRITE_SHORT(pPacket,data)		{((XS16*)pPacket) = data;pPacket += sizeof(XS16);}
#define X_WRITE_UINT(pPacket,data)		{((XU32*)pPacket) = data;pPacket += sizeof(XU32);}
#define X_WRITE_INT(pPacket,data)		{((XS32*)pPacket) = data;pPacket += sizeof(XS32);}
#define X_WRITE_ULONG(pPacket,data)		{((XU32*)pPacket) = data;pPacket += sizeof(XU32);}
#define X_WRITE_LONG(pPacket,data)		{((XS32*)pPacket) = data;pPacket += sizeof(XS32);}

#define X_READ_BYTE(pPacket,data)		{data = (*((XS8*)pPacket));pPacket += sizeof(XU8);}
#define X_READ_USHORT(pPacket,data)		{data = (*((XU16*)pPacket));pPacket += sizeof(XU16);}
#define X_READ_SHORT(pPacket,data)		{data = (*((XS16*)pPacket));pPacket += sizeof(XS16);}
#define X_READ_UINT(pPacket,data)		{data = (*((XU32*)pPacket));pPacket += sizeof(XU32);}
#define X_READ_INT(pPacket,data)		{data = (*((XS32*)pPacket));pPacket += sizeof(XS32);}
#define X_READ_ULONG(pPacket,data)		{data = (*((XU32*)pPacket));pPacket += sizeof(XU32);}
#define X_READ_LONG(pPacket,data)		{data = (*((XS32*)pPacket));pPacket += sizeof(XS32);}


//General network defines.
#define X_MAX_SERVER_NAME_SIZE		32	//This can never be more than 255
#define X_MAX_PASSWORD_SIZE			32	//This can never be more than 255
#define X_MAX_PLAYER_NAME_SIZE		32	//This can never be more than 255
#define X_MAX_SESSION_ID_SIZE		32	//This can never be more than 255
#define X_NET_PKT_HEADER_SIZE		8	//Number of bytes reserved for system data
#define X_NET_USER_PKT(x)			(x+X_NET_PKT_HEADER_SIZE)
#define X_NET_SYS_PKT(x)			(x-X_NET_PKT_HEADER_SIZE)
#define X_NET_MIN_PKT_SIZE			(X_MAX_SERVER_NAME_SIZE + X_MAX_PASSWORD_SIZE + \
									X_MAX_PLAYER_NAME_SIZE + X_MAX_SESSION_ID_SIZE + \
									X_NET_PKT_HEADER_SIZE)

#define X_NET_KEEP_ALIVE_INTERVAL			20		//Number of seconds between keep-alive packets
#define X_NET_KEEP_ALIVE_TIMEOUT			90		//Number of seconds to wait for keep-alive response
#define X_NET_WAIT_TIMEOUT					30		//Number of seconds to wait for player's response
#define X_NET_BROADCAST_PORT				9002	//XGameServer/XGameClient reserved port

//Network Message Types.
enum X_NET_MSG_TYPE
{ 
	X_NMT_USER,				
	X_NMT_CONNECTION_REQUEST,
	X_NMT_DISCONNECT_REQUEST,
	X_NMT_VALIDATION_REQUEST,
	X_NMT_VALIDATION_RESPONSE,
	X_NMT_SERVER_QUERY,
	X_NMT_SERVER_ANNOUNCE,
	X_NMT_CONNECTION_CLOSED,
	X_NMT_CONNECTION_ACCEPTED,
	X_NMT_CONNECTION_REJECTED,
	X_NMT_KEEP_ALIVE_REQUEST,
	X_NMT_KEEP_ALIVE_RESPONSE
};

//User action types.
enum X_NET_MSG_ACTION_TYPE
{
	X_NMAT_NO_ACTION,
	X_NMAT_DROP,
	X_NMAT_FORWARD,
	X_NMAT_ACCEPT,
	X_NMAT_REJECT,
	X_NMAT_FINISH
};


//Player states.
enum X_NET_CONNECTION_STATE
{
	X_NCS_INVALID,
	X_NCS_WAITING,
	X_NCS_VALIDATION,
	X_NCS_CONNECTED,
	X_NCS_CONNECT,
	X_NCS_DISCONNECT,
	X_NCS_CURRENT
};


/** @} */ 

#endif
