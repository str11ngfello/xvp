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
//	Module Name:	XMemoryPool.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-13-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the memory pool.
*/
//*****************************************************************************
XMemoryPool::XMemoryPool()
{
	m_numBlocks = 0;
	m_blockSize = 0;
	m_pBlockList = NULL;
}
	


//*****************************************************************************
/**
Destructor for the memory pool.
*/
//*****************************************************************************
XMemoryPool::~XMemoryPool()
{
}



//*****************************************************************************
/**
Initializes the memory pool by allocating the requested number of memory blocks.

@param numBlocks	The number of blocks of memory to allocate.  A block is a
					contiguous segment of memory.
@param blockSizeInBytes	The number of bytes in each block.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMemoryPool::init(XU32 numBlocks,XU32 blockSizeInBytes)
{
	//Validate parameters.
	if ((numBlocks == 0) || (blockSizeInBytes == 0))
		return X_ERR_INVALID_PARAMS;

	//Okay, destroy any previous data.
	destroy();

	//Record the passed in data.
	m_numBlocks = numBlocks;
	m_blockSize = blockSizeInBytes;

	//Allocate the amount of memory requested.
	XNewArray(m_pBlockList,XS8,numBlocks * blockSizeInBytes,XOSMgr::getDefaultHeap());
	if (!m_pBlockList)
		return X_ERR_MEMORY_EXHAUSTED;

	//Determine block pointers and push them onto the free list.
	for (XU32 i = 0;i < numBlocks;++i)
		m_freeList.insertBack((XS8*)(((XU64)m_pBlockList) + (i * blockSizeInBytes)));

	InitializeCriticalSection(&m_criticalSection);

	return X_OK;
}



//*****************************************************************************
/**
Destroys the memory pool by freeing all memory associated with the memory pool.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMemoryPool::destroy()
{
	m_usedList.clear();
	m_freeList.clear();
	XDeleteArray(m_pBlockList);
	m_numBlocks = 0;
	m_blockSize = 0;
	DeleteCriticalSection(&m_criticalSection);

	return X_OK;
}



//*****************************************************************************
/**
Gets a free block of memory. The memory block passed back will be locked and
considered in use until the memory block is returned to the pool.

@return A pointer to the the memory block.  The size of the memory block will
		be equal to the block size specified when the memory pool was created.
		Be careful not to write past the length of the memory block, otherwise
		the memory pool will be corrupted. The memory is not initialized
		with any pattern of data, therefore you should consider the
		memory to be garbage when this function returns. 
*/
//*****************************************************************************
XS8* XMemoryPool::getFreeBlock()
{
	//If we have a free block, pop it off the free list and push it onto the
	//back list. Otherwise return NULL.
	XS8* pNewBlock = NULL;
	EnterCriticalSection(&m_criticalSection);
	if (m_freeList.getNumItems() > 0)
	{
		pNewBlock = m_freeList.getFront();
		m_freeList.removeFront();
		m_usedList.insertBack(pNewBlock);
	}

	LeaveCriticalSection(&m_criticalSection);

	return pNewBlock;
}



//*****************************************************************************
/**
Releases a memory block previously obtained from the memory pool.  

@param pBlock	A pointer to the memory block.  It is extremely important that
				the parameter be a valid memory block pointer. I.e, the pointer
				must have been obtained from the memory pool in order to return
				it.  If pointers are specified that do not exist in the memory
				pool, the behavior is undefined and the application will become
				unstable.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMemoryPool::releaseBlock(XS8* pBlock)
{
	//Make sure pBlock is valid
	if (!pBlock)
		return X_ERR_INVALID_PARAMS;
		
	//Move the pBlock from the used list to the free list.  For speed, we trust
	//that the user will pass in a valid pBlock.
	EnterCriticalSection(&m_criticalSection);
	m_usedList.remove(pBlock);
	m_freeList.insertBack(pBlock);
	LeaveCriticalSection(&m_criticalSection);

	return X_OK;
}



//*****************************************************************************
/**
Returns the number of free memory blocks in the memory pool.  

@return The number of free memory blocks in the memory pool.
*/
//*****************************************************************************
XU32 XMemoryPool::getNumFreeBlocks()
{
	return m_freeList.getNumItems();
}



//*****************************************************************************
/**
Returns the number of used memory blocks in the memory pool.  

@return The number of used memory blocks in the memory pool.
*/
//*****************************************************************************
XU32 XMemoryPool::getNumUsedBlocks()
{
	return m_usedList.getNumItems();
}



//*****************************************************************************
/**
Returns the number of total memory blocks in the memory pool.  

@return The number of total memory blocks in the memory pool.
*/
//*****************************************************************************
XU32 XMemoryPool::getNumTotalBlocks()
{
	return m_numBlocks;
}



//*****************************************************************************
/**
Returns the block size of the memory pool in bytes.  This is the size specified
when the memory pool was created.

@return Returns the block size of the memory pool in bytes.  This is the size specified
when the memory pool was created.
*/
//*****************************************************************************
XU32 XMemoryPool::getBlockSize()
{
	return m_blockSize;
}