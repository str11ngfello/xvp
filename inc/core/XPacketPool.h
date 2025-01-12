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
//	Module Name:	XPacketPool.h
//	Author:			Lynn Duke
//	Creation Date:	10-5-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PACKET_POOL_H__
#define __X_PACKET_POOL_H__




#define X_PP_REF_CNT_FIELD_LEN		sizeof(XU32)



//*****************************************************************************
//! Provides efficient memory allocation and management for network packets.
/**
The XPacketPool object provides efficient memory allocation and management as
well as reference counting for network packets.  The network packets are allocated
contiguously in memory and can be used and freed without requiring system calls
to allocate memory.       

See the member functions below for more information.
*/
//*****************************************************************************
class XPacketPool
{
private:
	XMemoryPool			m_memoryPool;

public:
	XPacketPool();
	~XPacketPool();
	XStatus init(XU32 numPackets,XU32 packetSizeInBytes);
	XStatus destroy();
	XS8* getFreePacket(XU32 numReferences = 1);
	XStatus releasePacket(XS8* pPacket);
	XU32 getNumFreePackets();
	XU32 getNumUsedPackets();
	XU32 getNumTotalPackets();
	XU32 getPacketSize();
	XU32 getReferenceCount(XS8* pPacket);
	void setReferenceCount(XS8* pPacket,XU32 refCount);
};


#endif