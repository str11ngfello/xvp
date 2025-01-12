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
//	Module Name:	XHeapFactory.h
//	Author:			Lynn Duke
//	Creation Date:	5-22-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_HEAP_H__
#define __X_HEAP_H__

#define X_MAX_HEAP_NAME_SIZE	32

struct AllocHeader;

class XHeap
{
public:
    void Initialize();
    
    void * Allocate(size_t bytes
#ifndef XVP_FINAL
					,char* pFilename,
					int lineNum
#endif
					);

    static void Deallocate (void * pMem);

    void Activate (char * name);
    void Deactivate ();

    void AttachTo (XHeap * pParent);

    bool IsActive () { return m_bActive; }
    char * GetName();

    void PrintTreeInfo (int indentLevel = 0);
    void PrintInfo (int indentLevel = 0);

#ifndef XVP_FINAL
    int dumpAllocations();
#endif

    static int GetMemoryBookmark ();

private:
    void Deallocate(AllocHeader * pHeader);
    void GetTreeStats (size_t & totalBytes, size_t & totalPeak, int & totalInstances);

   
    bool   m_bActive;
    char   m_name[X_MAX_HEAP_NAME_SIZE];
    size_t m_bytesAllocated;
    size_t m_bytesPeak;
    int    m_nInstances;
    AllocHeader * m_pHeadAlloc;

    XHeap * m_pParent;
    XHeap * m_pFirstChild;
    XHeap * m_pNextSibling;
    XHeap * m_pPrevSibling;

    static int s_nNextAllocNum;
};


#endif