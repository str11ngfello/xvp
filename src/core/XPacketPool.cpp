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
//	Module Name:	XPacketPool.cpp
//	Author:			Lynn Duke
//	Creation Date:	10-5-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the packet pool.
*/
//*****************************************************************************
XPacketPool::XPacketPool()
{
}
	


//*****************************************************************************
/**
Destructor for the packet pool.
*/
//*****************************************************************************
XPacketPool::~XPacketPool()
{
	destroy();
}



//*****************************************************************************
/**
Initializes the packet pool.

@param numPackets	The number of packets to allocate in the packet pool.
@param packetSizeInBytes	The size of each packet.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XPacketPool::init(XU32 numPackets,XU32 packetSizeInBytes)
{
	//Validate parameters.
	if ((numPackets == 0) || (packetSizeInBytes == 0))
		return X_ERR_INVALID_PARAMS;

	//Okay, destroy any previous data.
	destroy();

	return m_memoryPool.init(numPackets,packetSizeInBytes + X_PP_REF_CNT_FIELD_LEN);
}



//*****************************************************************************
/**
Destroys the packet pool and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XPacketPool::destroy()
{
	return m_memoryPool.destroy();
}



//*****************************************************************************
/**
Gets a new packet from the pool. The packet passed back will be locked and
considered in use until the packet is returned to the pool.

@return A pointer to the packet.  The size of the packet will
		be equal to the packet size specified when the packet pool was created.
		Be careful not to write past the length of the packet, otherwise
		the packet pool will be corrupted.  The packet is not initialized
		with any pattern of data, therefore you should consider the
		packet memory to be garbage when this function returns.
*/
//*****************************************************************************
XS8* XPacketPool::getFreePacket(XU32 numReferences)
{
	//Get a new memory block.
	XS8* pPacket = m_memoryPool.getFreeBlock();

	//If we were successful getting a new memory block, write in the user
	//specified reference count and pass back the packet buffer.  Otherwise,
	//return NULL.
	if (pPacket)
	{
		*((XU32*)pPacket) = numReferences;
		return pPacket + X_PP_REF_CNT_FIELD_LEN;
	}
	else
		return NULL;
}



//*****************************************************************************
/**
Releases a packet previously obtained from the packet pool.  This function will
only release a packet once the reference count reaches 0.  For example, if a
packet's reference count is 10, the function must be called 10 times on the
packet to truly release it and make it available again. 

@param pPacket	A pointer to the packet.  It is extremely important that the
				parameter be a valid packet pointer. I.e, the pointer
				must have been obtained from the packet pool in order to return
				it.  If pointers are specified that do not exist in the packet
				pool, the behavior is undefined and the application will become
				unstable.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XPacketPool::releasePacket(XS8* pPacket)
{
	//Make sure pPacket is valid
	if (!pPacket)
		return X_ERR_INVALID_PARAMS;
		
	//If the reference count is one, free the memory block, otherwise, decrement
	//the reference count withought freeing the memory block.
	XS8* p = pPacket - X_PP_REF_CNT_FIELD_LEN;
	if ((*((XU32*)p)) == 1)
		return m_memoryPool.releaseBlock(p);
	else
	{
		(*((XU32*)p)) -= 1;
		return X_OK;		
	}

}



//*****************************************************************************
/**
Returns the number of free packets in the packet pool.  

@return The number of free packets in the packet pool.
*/
//*****************************************************************************
XU32 XPacketPool::getNumFreePackets()
{
	return m_memoryPool.getNumFreeBlocks();
}



//*****************************************************************************
/**
Returns the number of used packets in the packet pool.  

@return The number of used packets in the packet pool.
*/
//*****************************************************************************
XU32 XPacketPool::getNumUsedPackets()
{
	return m_memoryPool.getNumUsedBlocks();
}



//*****************************************************************************
/**
Returns the number of total packets in the packet pool.  

@return The number of total packets in the packet pool.
*/
//*****************************************************************************
XU32 XPacketPool::getNumTotalPackets()
{
	return m_memoryPool.getNumTotalBlocks();
}



//*****************************************************************************
/**
Returns the packet size of the packed pool in bytes.  This is the size specified
when the packet pool was created.

@return Returns the packet size of the packet pool in bytes.  This is the size specified
when the packet pool was created.
*/
//*****************************************************************************
XU32 XPacketPool::getPacketSize()
{
	return m_memoryPool.getBlockSize();
}



//*****************************************************************************
/**
Gets the reference count of a packet in the packet pool.

@param pPacket	A pointer to the packet for which to get the reference count.
@return The reference count of the packet.
*/
//*****************************************************************************
XU32 XPacketPool::getReferenceCount(XS8* pPacket)
{
	//Make sure pPacket is valid
	if (!pPacket)
		return 0;

	return (*((XU32*)(pPacket - X_PP_REF_CNT_FIELD_LEN)));
}



//*****************************************************************************
/**
Sets the reference count of a packet.

@param pPacket	A pointer to the packet for which to set the reference count.
@param refCount	The reference count.
@return None.
*/
//*****************************************************************************
void XPacketPool::setReferenceCount(XS8* pPacket,XU32 refCount)
{
	//Make sure pPacket is valid
	if (!pPacket)
		return;
	
	(*((XU32*)(pPacket - X_PP_REF_CNT_FIELD_LEN))) = refCount;
}
