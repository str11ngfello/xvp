
#include <xvpsdk.h>

#define X_HEAP_MEMSYSTEM_SIGNATURE  0xDEADC0DE
#define X_HEAP_MAX_FILENAME_SIZE	32

int XHeap::s_nNextAllocNum = 1;


struct AllocHeader {
    int           nSignature;
    int           nAllocNum;
    int           nSize;

#ifndef XVP_FINAL
	char		  filename[X_HEAP_MAX_FILENAME_SIZE];
	int			  lineNum;
#endif

    XHeap *        pHeap;
    AllocHeader * pNext;
    AllocHeader * pPrev;

	AllocHeader::AllocHeader()
	{
		nSignature = 0;
		nAllocNum = 0;
		nSize = 0;

#ifndef XVP_FINAL
		filename[0] = '\0';
		lineNum = 0;
#endif

		pHeap = NULL;
		pNext = NULL;
		pPrev = NULL;
	}
};



void XHeap::Initialize()
{
    m_bActive        = false;
    m_bytesAllocated = 0;
    m_bytesPeak      = 0;
    m_nInstances     = 0;
    m_pHeadAlloc     = NULL;

    m_pParent        = NULL;
    m_pFirstChild    = NULL;
    m_pNextSibling   = NULL;
    m_pPrevSibling   = NULL;
}


void XHeap::Activate (char * name)
{
    X_ASSERTM(name != NULL,"XHeap must be given a name");
    X_ASSERTM(strlen(name) < X_MAX_HEAP_NAME_SIZE,"XHeap name too large");
    strcpy (m_name, name);
    m_bytesAllocated = 0;
    m_bytesPeak      = 0;
    m_nInstances     = 0;
    m_bActive        = true;
}

void XHeap::Deactivate ()
{
    strcpy(m_name, "");

    // Destroy chain
    m_bytesAllocated = 0;
    m_bytesPeak      = 0;
    m_nInstances     = 0;
    m_bActive = false;
}


char * XHeap::GetName() 
{
    return m_name;
}


void XHeap::PrintTreeInfo (int indentLevel) 
{
    PrintInfo(indentLevel);
    XHeap * pChild = m_pFirstChild;
    while (pChild != NULL)
    {
        pChild->PrintTreeInfo(indentLevel + 1);
        pChild = pChild->m_pNextSibling;
    }
}


void XHeap::PrintInfo (int indentLevel)
{
	char buf[512];
    for (int i=0; i<indentLevel; ++i)
        X_DEBUG_LOG("  ",X_DLL_NONE);

    size_t totalBytes = 0;
    size_t totalPeakBytes = 0;
    int totalInstances = 0;
    GetTreeStats(totalBytes, totalPeakBytes, totalInstances);

    int spacing = 30 - indentLevel*2;
    sprintf (buf,"%-*s %9d %12d %12d %13d %12d %12d", 
            spacing, m_name, 
            m_bytesAllocated, m_bytesPeak, m_nInstances,
            totalBytes, totalPeakBytes, totalInstances);
	X_DEBUG_LOG_LN(buf,X_DLL_NONE);
}


void XHeap::GetTreeStats (size_t & totalBytes, size_t & totalPeak, 
                         int & totalInstances)
{
    totalBytes     += m_bytesAllocated;
    totalPeak      += m_bytesPeak;
    totalInstances += m_nInstances;

    XHeap * pChild = m_pFirstChild;
    while (pChild != NULL)
    {
        pChild->GetTreeStats(totalBytes, totalPeak, totalInstances);
        pChild = pChild->m_pNextSibling;
    }
}




void * XHeap::Allocate (size_t bytes
#ifndef XVP_FINAL
						,char* pFilename,
						int lineNum
#endif
						)

{
    size_t nRequestedBytes = bytes + sizeof(AllocHeader);
    char * pMem = (char *)malloc (nRequestedBytes);
    AllocHeader * pHeader = (AllocHeader *)pMem;
    pHeader->nSignature = X_HEAP_MEMSYSTEM_SIGNATURE;
    pHeader->pHeap = this;
    pHeader->nSize = (XS32)bytes;
    pHeader->pNext = m_pHeadAlloc;
    pHeader->pPrev = NULL;
    pHeader->nAllocNum = s_nNextAllocNum++;

#ifndef XVP_FINAL
	//Strip path if necessary...we only want to store the filename.
	if (pFilename)
	{
		if (strchr(pFilename,'\\') != NULL)
			strncpy(pHeader->filename,strrchr(pFilename,'\\')+1,X_HEAP_MAX_FILENAME_SIZE-1);
		else if (strchr(pFilename,'/') != NULL)
			strncpy(pHeader->filename,strrchr(pFilename,'/')+1,X_HEAP_MAX_FILENAME_SIZE-1);
		else
			strncpy(pHeader->filename,pFilename,X_HEAP_MAX_FILENAME_SIZE-1);
	}
	else
		pHeader->filename[0] = '\0';
	pHeader->lineNum = lineNum;

#endif

    if (m_pHeadAlloc != NULL)
        m_pHeadAlloc->pPrev = pHeader;
    m_pHeadAlloc = pHeader;

    m_bytesAllocated += bytes;
    if (m_bytesAllocated > m_bytesPeak)
        m_bytesPeak = m_bytesAllocated;
    m_nInstances++;

    void * pStartMemBlock = pMem + sizeof(AllocHeader);
    return pStartMemBlock; 
}


void XHeap::Deallocate (void * pMem)
{
    AllocHeader * pHeader = (AllocHeader *)((char *)pMem -
                             sizeof(AllocHeader));
    X_ASSERTM(pHeader->nSignature == X_HEAP_MEMSYSTEM_SIGNATURE,"Corrupted header signature");
    pHeader->pHeap->Deallocate(pHeader);
}


void XHeap::Deallocate (AllocHeader * pHeader)
{
    if (pHeader->pPrev == NULL)
    {
        X_ASSERTM(pHeader == m_pHeadAlloc,"Header != head pointer");
        m_pHeadAlloc = pHeader->pNext;
    }        
    else
        pHeader->pPrev->pNext = pHeader->pNext;

    if (pHeader->pNext != NULL)
        pHeader->pNext->pPrev = pHeader->pPrev;   

    m_bytesAllocated -= pHeader->nSize;
    m_nInstances--;
    free (pHeader);
}

#ifndef XVP_FINAL

int XHeap::dumpAllocations()
{
    int numAllocations = 0;

    AllocHeader * pHeader = m_pHeadAlloc;
    while (pHeader != NULL) 
    {
        //if (pHeader->nAllocNum >= nBookmark1 &&
        //    pHeader->nAllocNum < nBookmark2 )
        //{
		XS8 buf[256];
		if (strlen(pHeader->filename) != 0)
			sprintf(buf,"Heap(%s) Size(%d) Filename(%s) Line(%d)",m_name, pHeader->nSize, 
				pHeader->filename,pHeader->lineNum);
		else
			sprintf(buf,"Heap(%s) Size(%d)",m_name, pHeader->nSize);

		X_DEBUG_LOG_LN(buf,X_DLL_NONE);
        numAllocations++;
        //}
        pHeader = pHeader->pNext;
    }
    return numAllocations;
}
#endif


int XHeap::GetMemoryBookmark () 
{
    return s_nNextAllocNum;
}


void XHeap::AttachTo (XHeap * pParent)
{
    X_ASSERTM(pParent != NULL,"Parent pointer = NULL");

    if (pParent == m_pParent)
        return;

    // First detach itself from its current parent
    if (m_pPrevSibling != NULL)
        m_pPrevSibling->m_pNextSibling = m_pNextSibling;

    if (m_pNextSibling != NULL)
        m_pNextSibling->m_pPrevSibling = m_pPrevSibling;

    if (m_pParent != NULL)
        if (m_pParent->m_pFirstChild == this)
            m_pParent->m_pFirstChild = m_pNextSibling;

    // Now attach itself to the new parent
    m_pNextSibling = pParent->m_pFirstChild;
    m_pPrevSibling = NULL;
    m_pParent      = pParent;
    pParent->m_pFirstChild = this;
}



/*
void XHeap::AddChild (XHeap * pChild)
{
    assert(pChild != NULL);

    if (m_pFirstChild != NULL)
        m_pFirstChild->m_pPrevSibling = pChild;
    pChild->m_pNextSibling = m_pFirstChild;
    pChild->m_pPrevSibling = NULL;
    m_pFirstChild = pChild;
}


void XHeap::RemoveChild (XHeap * pChild)
{
    assert(pChild != NULL);

    if (m_pFirstChild == pChild)
        m_pFirstChild = pChild->m_pNextSibling;

    if (pChild->m_pPrevSibling != NULL)
        pChild->m_pPrevSibling = pChild->m_pNextSibling;

    if (pChild->m_pNextSibling != NULL)
        pChild->m_pNextSibling = pChild->m_pPrevSibling;
}

*/
