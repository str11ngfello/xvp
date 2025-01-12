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
//	Module Name:	XFreeList.h
//	Author:			Lynn Duke
//	Creation Date:	5-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_FREE_LIST_H__
#define __X_FREE_LIST_H__


 

#define X_ALIGN_OF(T)						__alignof(T)
#define X_ALIGNED_MALLOC(size,alignment)	_aligned_malloc(size,alignment)
#define X_ALIGNED_FREE(pMemory)				_aligned_free(pMemory)
#define X_FL_MAX_FN_SIZE					32	
#define X_FL_MAX_ADJ_FN_SIZE				(X_FL_MAX_FN_SIZE*2)	



//*****************************************************************************
//*****************************************************************************
template<typename T>
class XFreeList
{
private:
	struct FreeBlock
	{
		FreeBlock *pNext;
	};

#ifndef XVP_FINAL
	struct FreeBlockFooter
	{
		XS8	filename[X_FL_MAX_FN_SIZE];
		XU32 lineNum;
		XBOOL allocated;
	};
#endif
	
	XBOOL			m_inited;
	XS8*			m_pName;
	XU32			m_numBlocksAllocated;
	XU32			m_numBlocksInUse;
	XU32			m_peakBlocksInUse;
	XU32			m_blockSize;
	XS8*			m_pMemChunk;
	XS8*			m_pBaseAddress;
	XS8*			m_pEndAddress;
	XHeap*			m_pHeap;
	FreeBlock*		m_pFreeBlocks;

	
	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	void* getFreeItem();
	void push(void* pItem);
	void* pop();
	void grow(XU32 numBlocks);
	void incNumBlocksInUse();

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XFreeList();
	~XFreeList();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XU32 numInitialItems,XS8* pName,XHeap* pHeap = NULL);
	XStatus destroy();
	XS8* getName();
	T* allocateItem();
#ifndef XVP_FINAL
	T* allocateItem(XS8* pFilename,XU32 lineNum);
#endif
	void freeItem(T* pItem);
	XU32 getNumBlocksInUse();
	XU32 getPeakBlocksInUse();
	XU32 getMaxBlockCapacity();
	XBOOL verifyItem(T* pItem);
};



//*****************************************************************************
//*****************************************************************************
template<typename T>
XFreeList<T>::XFreeList()
{
	m_pName = NULL;
	m_numBlocksAllocated = 0;
	m_numBlocksInUse = 0;
	m_peakBlocksInUse = 0;
	m_blockSize = 0;
	m_pFreeBlocks = NULL;
	m_pHeap = NULL;
	m_pMemChunk = NULL;
	m_pBaseAddress = NULL;
	m_pEndAddress = NULL;
	m_inited = false;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XFreeList<T>::~XFreeList()
{
	//if (m_pMemChunk)
		//destroy();
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XFreeList<T>::init(XU32 numInitialItems,XS8* pName,XHeap* pHeap)
{
	X_ASSERTM(numInitialItems,"Cannot initialize free list with 0 items")
	X_ASSERTM(!m_inited,"Cannot initialize free list with 0 items")

	//Create the name
	if (pName)
		m_pName = XOSMgr::createString(pName);

	if (!pHeap)
		m_pHeap = XOSMgr::getDefaultHeap();
	else
		m_pHeap = pHeap;

	//Grow the free list by specified amount
	grow(numInitialItems);

	m_inited = true;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XStatus XFreeList<T>::destroy()
{
#ifndef XVP_FINAL
	//Debug check for items still remaining in list.  If the user is destroying
	//the free list but all items have not been returned, warn them...
	if (m_numBlocksInUse != 0)
	{
		XS8 buf[256];
		if (m_pName)
			sprintf(buf,"Freelist \"%s\" is being destroyed before all items returned. Dumping tracked allocations:",m_pName);
		else
			sprintf(buf,"Freelist <noname> is being destroyed before all items returned. Dumping tracked allocations:");

		X_DEBUG_LOG_LN(buf,X_DLL_NONE);
		sprintf(buf,"Num outstanding allocations: %d",m_numBlocksInUse);
		X_DEBUG_LOG_LN(buf,X_DLL_NONE);

		//Spit out the outstanding allocations...
		for (XU32 i = 0;i < m_numBlocksAllocated;++i)
		{
			XU8* pItem = (XU8*)m_pMemChunk + (i * m_blockSize);
			FreeBlockFooter* pFbf = (FreeBlockFooter*)((XU8*)pItem + sizeof(T));
			if (pFbf->allocated && (strlen(pFbf->filename) != 0))
			{
				sprintf(buf,"File: %s  LineNum: %d",pFbf->filename,pFbf->lineNum);
				X_DEBUG_LOG_LN(buf,X_DLL_NONE);
			}
		}
	}
#endif
	
	//Destroy the name and memchunk
	if (m_pName)
	{
		XOSMgr::destroyString(&m_pName);
		m_pName = NULL;
	}

	if (m_pMemChunk)
	{
		XHeap::Deallocate(m_pMemChunk);
		m_pMemChunk = NULL;
	}

	XFreeList();
	m_inited = false;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
void XFreeList<T>::incNumBlocksInUse()
{
	++m_numBlocksInUse;
	if(m_numBlocksInUse > m_peakBlocksInUse)
		m_peakBlocksInUse = m_numBlocksInUse;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
T* XFreeList<T>::allocateItem()
{
	void* pItem = getFreeItem();

#ifdef XVP_FINAL
	if(!pItem)
		return NULL;
	else
		return new (pItem) T;
#else
	if(!pItem)
		return NULL;
	else
	{
		FreeBlockFooter* pFbf = (FreeBlockFooter*)((XU8*)pItem + sizeof(T));
		pFbf->filename[0] = '\0';
		pFbf->lineNum = 0;
		pFbf->allocated = true;
		return new (pItem) T;
	}
#endif
}



//*****************************************************************************
//*****************************************************************************
#ifndef XVP_FINAL
template<typename T>
T* XFreeList<T>::allocateItem(XS8* pFilename,XU32 lineNum)
{
	void* pItem = getFreeItem();
	if(!pItem)
		return NULL;
	else
	{
		FreeBlockFooter* pFbf = (FreeBlockFooter*)((XU8*)pItem + sizeof(T));
		if (pFilename)
		{
			char adjustedFilename[X_FL_MAX_ADJ_FN_SIZE];
			if (strchr(pFilename,'\\') != NULL)
				strncpy(adjustedFilename,strrchr(pFilename,'\\')+1,X_FL_MAX_ADJ_FN_SIZE-1);
			else if (strchr(pFilename,'/') != NULL)
				strncpy(adjustedFilename,strrchr(pFilename,'/')+1,X_FL_MAX_ADJ_FN_SIZE-1);
			else
				strncpy(adjustedFilename,pFilename,X_FL_MAX_ADJ_FN_SIZE-1);
			
			strncpy(pFbf->filename,adjustedFilename,X_FL_MAX_FN_SIZE-1);
			pFbf->lineNum = lineNum;
		}
		else
		{
			pFbf->filename[0] = '\0';
			pFbf->lineNum = 0;

		}
		pFbf->allocated = true;

		return new (pItem) T;
	}
}
#endif



//*****************************************************************************
//*****************************************************************************
template<typename T>
void XFreeList<T>::freeItem(T* pItem)
{
	X_ASSERTM(pItem,"NULL pointer passed to freeItem");

	//Debug check - did we allocate this item?  If it doesn't look like it, then assert
#ifndef XVP_FINAL
	XS8 buf[256];
	if (m_pName)
		sprintf(buf,"Returning item to freelist \"%s\" that doesn't belong",m_pName);
	else
		sprintf(buf,"Returning item to freelist <noname> that doesn't belong");

	if ((m_numBlocksInUse == 0) || (pItem < (void*)m_pMemChunk)||(pItem >= (void*)(m_pMemChunk +(m_numBlocksAllocated * m_blockSize))))
	{
		int a = 0;
		a++;
	}
		
	X_ASSERTM(m_numBlocksInUse != 0,buf);
	X_ASSERTM((pItem >= (void*)m_pMemChunk)&&(pItem < (void*)(m_pMemChunk +(m_numBlocksAllocated * m_blockSize))),buf);

	FreeBlockFooter* pFbf = (FreeBlockFooter*)((XU8*)pItem + sizeof(T));
	pFbf->allocated = false;
#endif
	
	//Call destructor and return the item
	pItem->~T();
	push(pItem);
	--m_numBlocksInUse;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
void* XFreeList<T>::getFreeItem()
{
	void* pItem = pop();
	
	//Debug check availability
	X_DEBUG
	(	
		if (!pItem)
		{
			XS8 buf[256];
			if (m_pName)
				sprintf(buf,"no free items left in XFreeList \"%s\" ...capacity has been exhausted",m_pName);
			else
				sprintf(buf,"no free items left in XFreeList <noname> ...capacity has been exhausted");

			X_ASSERT_FORCED(buf);
		}
	)

	return pItem;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
void XFreeList<T>::push(void* pItem)
{
	FreeBlock* pNewHead = reinterpret_cast<FreeBlock*>(pItem);
	pNewHead->pNext = m_pFreeBlocks;
	m_pFreeBlocks = pNewHead;
}

	
	
//*****************************************************************************
//*****************************************************************************
template<typename T>
void* XFreeList<T>::pop()
{
	if( !m_pFreeBlocks )
		return NULL;

	incNumBlocksInUse();
	void* pReturn = m_pFreeBlocks;
	m_pFreeBlocks = m_pFreeBlocks->pNext;
	return pReturn;
}

	
	
//*****************************************************************************
//*****************************************************************************
template<typename T>
void XFreeList<T>::grow(XU32 numBlocks)
{
	XS8* pNewChunk = NULL;

	//We must allocate at least enough space for a pointer. In debug modes
	//we also want to allocate a footer for each block to track allocation
	//information.
	XU32 blockSize = sizeof(T);
	if (blockSize < sizeof(void*))
		blockSize = sizeof(void*);
	m_blockSize = blockSize;

#ifndef XVP_FINAL
		blockSize += sizeof(FreeBlockFooter);
		m_blockSize = blockSize;
#endif

	//Don't allocate using XNew macro
	if (m_pHeap)
#ifdef XVP_FINAL
		pNewChunk = (XS8*)m_pHeap->Allocate(numBlocks*blockSize);
#else
		pNewChunk = (XS8*)m_pHeap->Allocate(numBlocks*blockSize,__FILE__,__LINE__);
#endif
	else
#ifdef XVP_FINAL
		pNewChunk = (XS8*)XHeapFactory::GetDefaultHeap()->Allocate(numBlocks*blockSize);
#else
		pNewChunk = (XS8*)XHeapFactory::GetDefaultHeap()->Allocate(numBlocks*blockSize,__FILE__,__LINE__);
#endif
	
	m_numBlocksAllocated += numBlocks;

	//Set up the links for each block
	for(XU32 i = 0;i < numBlocks;++i)
	{
		push(pNewChunk + (i * blockSize));

#ifndef XVP_FINAL
		FreeBlockFooter* pFbf = (FreeBlockFooter*)((XU8*)(pNewChunk + (i * blockSize)) + sizeof(T));
		pFbf->filename[0] = '\0';
		pFbf->lineNum = 0;
		pFbf->allocated = false;
#endif
	}

	m_pMemChunk = pNewChunk;

	//Cache the base and end address so we can verify if pointers came from this free list
	//or not.  Even after the free list is destroyed, we keep these addresses around
	//for future requests.
	m_pBaseAddress = pNewChunk;
	m_pEndAddress = pNewChunk + ((m_numBlocksAllocated * m_blockSize) - 1);
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XFreeList<T>::getNumBlocksInUse()
{
	return m_numBlocksInUse;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XFreeList<T>::getPeakBlocksInUse()
{
	return m_peakBlocksInUse;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XS8* XFreeList<T>::getName()
{
	return m_pName;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XU32 XFreeList<T>::getMaxBlockCapacity()
{
	return m_numBlocksAllocated;
}



//*****************************************************************************
//*****************************************************************************
template<typename T>
XBOOL XFreeList<T>::verifyItem(T* pItem)
{
	//If the pointer falls between the beginning of the free list AND the
	//end, then this item must have come from this free list to the best of
	//our knowledge.  This isn't a guarantee of course that we allocated this item
	//but it's a quick check.  We don't want to include the first or last address in the
	//filter.
	if (((void*)pItem >= m_pBaseAddress) && ((void*)pItem <= m_pEndAddress))
		return true;
	else
		return false;
}


#endif