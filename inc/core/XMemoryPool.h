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
//	Module Name:	XMemoryPool.h
//	Author:			Lynn Duke
//	Creation Date:	8-13-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MEMORY_POOL_H__
#define __X_MEMORY_POOL_H__





//*****************************************************************************
//! Memory allocation and management.
/**
The XMemoryPool object provides memory allocation and managment servies to 
avoid memory fragmentation and provide fast memory allocation and deallocation
requests.
.
It allocates a pool of memory consisting of a number of contiguous memory 
blocks.  The can be used and returned without making any actual system calls to
allocate memory.

See the member functions below for more information.
*/
//*****************************************************************************
class XMemoryPool
{
private:
	XMemBlockList<XS8*>			m_usedList;
	XMemBlockList<XS8*>			m_freeList;
	XS8*				m_pBlockList;
	XU32				m_numBlocks;
	XU32				m_blockSize;
	CRITICAL_SECTION	m_criticalSection;

public:
	XMemoryPool();
	~XMemoryPool();
	XStatus init(XU32 numBlocks,XU32 blockSizeInBytes);
	XStatus destroy();
	XS8* getFreeBlock();
	XStatus releaseBlock(XS8* pBlock);
	XU32 getNumFreeBlocks();
	XU32 getNumUsedBlocks();
	XU32 getNumTotalBlocks();
	XU32 getBlockSize();
};


#endif