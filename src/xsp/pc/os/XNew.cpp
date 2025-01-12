
#include <xvpsdk.h>



#ifdef XVP_FINAL

//*****************************************************************************
//*****************************************************************************
void * operator new (size_t size) 
{
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return XHeapFactory::GetDefaultHeap()->Allocate(size);
	}
	else
		return XHeapFactory::GetDefaultHeap()->Allocate(size);
}


//*****************************************************************************
//*****************************************************************************
void * operator new[] (size_t size) 
{
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return XHeapFactory::GetDefaultHeap()->Allocate(size);
	}
	else
		return XHeapFactory::GetDefaultHeap()->Allocate(size);
}



//*****************************************************************************
//*****************************************************************************
void * operator new (size_t size, XHeap * pHeap) 
{
    X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return pHeap->Allocate(size);
	}
	else
		return pHeap->Allocate(size);    
}



//*****************************************************************************
//*****************************************************************************
void * operator new[] (size_t size, XHeap * pHeap)  
{
	X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return pHeap->Allocate(size);
	}
	else
		return pHeap->Allocate(size);    
}



//*****************************************************************************
//*****************************************************************************
void operator delete (void * pMem) 
{
	X_ASSERTM(pMem != NULL,"NULL pointer being deleted.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	
	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY)
	{
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. Delete the memory before shutting down XVP.")
	}
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK && XOSMgr::isInternalFreeListsEnabled())
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);         
}


//*****************************************************************************
//*****************************************************************************
void operator delete[] (void * pMem) 
{
    X_ASSERTM(pMem != NULL,"NULL array pointer being deleted.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY)
	{
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. Delete the memory before shutting down XVP.")
	}
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK && XOSMgr::isInternalFreeListsEnabled())
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);           
}



//*****************************************************************************
//*****************************************************************************
void operator delete (void* pMem, XHeap * pHeap) 
{
    X_ASSERTM(pMem != NULL,"NULL pointer being deleted.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY)
	{
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. Delete the memory before shutting down XVP.")
	}
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK && XOSMgr::isInternalFreeListsEnabled())
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);         
}



//*****************************************************************************
//*****************************************************************************
void operator delete[] (void* pMem, XHeap * pHeap)  
{
    X_ASSERTM(pMem != NULL,"NULL pointer being deleted.");
	X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY && XOSMgr::isInternalFreeListsEnabled())
	{
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. Delete the memory before shutting down XVP.")
	}
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK)
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);         
}
 
/*
//*****************************************************************************
//*****************************************************************************
void * operator new (size_t size) 
{
	//char buf[256];
	//sprintf(buf," Unknown memory allocation of size %d bytes.\n",size);
	//X_ASSERT_FORCED(buf);
	return XHeapFactory::GetDefaultHeap()->Allocate(size);
}


//*****************************************************************************
//*****************************************************************************
void * operator new[] (size_t size) 
{
	//char buf[256];
	//sprintf(buf," Unknown array allocation of size %d bytes.\n",size);
	//X_ASSERT_FORCED(buf);
	return XHeapFactory::GetDefaultHeap()->Allocate(size);
}


//*****************************************************************************
//*****************************************************************************
void * operator new (size_t size,XHeap* pHeap) 
{
	//X_ASSERT_FORCED("Replace this new call with \"XNew\".");
	//return NULL;
    X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
    return pHeap->Allocate(size);
}



//*****************************************************************************
//*****************************************************************************
void * operator new[] (size_t size,XHeap* pHeap) 
{
	//X_ASSERT_FORCED("Replace this new call with \"XNew\".");
	//return NULL;
    X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
    return pHeap->Allocate(size);
}



//*****************************************************************************
//*****************************************************************************
void operator delete (void * pMem) 
{
	X_ASSERTM(pMem != NULL,"NULL memory pointer being deleted.");
	XHeap::Deallocate(pMem);    
}


//*****************************************************************************
//*****************************************************************************
void operator delete[] (void * pMem) 
{
    X_ASSERTM(pMem != NULL,"NULL array pointer being deleted.");
	XHeap::Deallocate (pMem);    
}



//*****************************************************************************
//*****************************************************************************
void operator delete (void * pMem,XHeap* pHeap) 
{
    X_ASSERTM(pMem != NULL,"NULL array pointer being deleted.");
	XHeap::Deallocate (pMem);    
}



//*****************************************************************************
//*****************************************************************************
void operator delete[] (void * pMem,XHeap* pHeap) 
{
    X_ASSERTM(pMem != NULL,"NULL array pointer being deleted.");
	XHeap::Deallocate (pMem);    
}
*/
#else //XVP_FINAL


//*****************************************************************************
//*****************************************************************************
void * operator new (size_t size) 
{
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size,NULL,0);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return XHeapFactory::GetDefaultHeap()->Allocate(size,NULL,0);
	}
	else
		return XHeapFactory::GetDefaultHeap()->Allocate(size,NULL,0);
}


//*****************************************************************************
//*****************************************************************************
void * operator new[] (size_t size) 
{
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size,NULL,0);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return XHeapFactory::GetDefaultHeap()->Allocate(size,NULL,0);
	}
	else
		return XHeapFactory::GetDefaultHeap()->Allocate(size,NULL,0);
}



//*****************************************************************************
//*****************************************************************************
void * operator new (size_t size, XHeap * pHeap,const char* pFilename,int lineNum) 
{
    X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size,(XS8*)pFilename,lineNum);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return pHeap->Allocate(size,(char*)pFilename,lineNum);
	}
	else
		return pHeap->Allocate(size,(char*)pFilename,lineNum);    
}



//*****************************************************************************
//*****************************************************************************
void * operator new[] (size_t size, XHeap * pHeap,const char* pFilename,int lineNum)  
{
	X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
	if (size <= X_OS_MAX_FREE_BLOCK_SIZE && XOSMgr::isInternalFreeListsEnabled())
	{
		void* pNewMemBlock = XOSMgr::getFreeMemBlock((XU32)size,(XS8*)pFilename,lineNum);
		if (pNewMemBlock)
			return pNewMemBlock;
		else
			return pHeap->Allocate(size,(char*)pFilename,lineNum);
	}
	else
		return pHeap->Allocate(size,(char*)pFilename,lineNum);    
}



//*****************************************************************************
//*****************************************************************************
void operator delete (void * pMem) 
{
	X_ASSERTM(pMem != NULL,"NULL pointer being deleted.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	
	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY)
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. This usually happens because the c++ runtime has released memory that was allocated from XVP's freelists after XVP was shut down.  Delete the memory explicitly before shutting down XVP.")
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK && XOSMgr::isInternalFreeListsEnabled())
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);         
}


//*****************************************************************************
//*****************************************************************************
void operator delete[] (void * pMem) 
{
    X_ASSERTM(pMem != NULL,"NULL array pointer being deleted.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY)
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. This usually happens because the c++ runtime has released memory that was allocated from XVP's freelists after XVP was shut down.  Delete the memory explicitly before shutting down XVP.")
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK && XOSMgr::isInternalFreeListsEnabled())
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);           
}



//*****************************************************************************
//*****************************************************************************
void operator delete (void* pMem, XHeap * pHeap,const char* pFilename,int lineNum) 
{
    X_ASSERTM(pMem != NULL,"NULL pointer being deleted.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY)
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. This usually happens because the c++ runtime has released memory that was allocated from XVP's freelists after XVP was shut down.  Delete the memory explicitly before shutting down XVP.")
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK && XOSMgr::isInternalFreeListsEnabled())
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);         
}



//*****************************************************************************
//*****************************************************************************
void operator delete[] (void* pMem, XHeap * pHeap,const char* pFilename,int lineNum)  
{
    X_ASSERTM(pMem != NULL,"NULL pointer being deleted.");
	X_ASSERTM(pHeap != NULL,"Heap pointer is NULL.");
	XU32 size;
	XStatus status = XOSMgr::getMemBlockSize(pMem,size);

	//If size is positive and the status code indicates that the free blocks
	//are not ready for operation, then chances are the free block system
	//has already been deallocated, so assert and tell the user they should
	//free the memory before shutting down XOSMgr.
 	if (size != 0 && status == X_ERR_NOT_READY && XOSMgr::isInternalFreeListsEnabled())
		X_ASSERT_FORCED("Delete called too late on memory that was allocated from XVP's free blocks. This usually happens because the c++ runtime has released memory that was allocated from XVP's freelists after XVP was shut down.  Delete the memory explicitly before shutting down XVP.")
	//else if the size is positive and the return status is ok, we can continue
	//the requested deallocation by return the mem block.
	else if (size != 0 && status == X_OK)
		XOSMgr::returnMemBlock(size,pMem);
	//otherwise, this allocation did not originate from the free mem blocks so
	//let it continue to normal system of deallocation.
	else
		XHeap::Deallocate(pMem);         
}


#endif // XVP_FINAL