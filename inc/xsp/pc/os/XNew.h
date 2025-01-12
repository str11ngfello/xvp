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
//	Module Name:	XNew.h
//	Author:			Lynn Duke
//	Creation Date:	5-22-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NEW_H__
#define __X_NEW_H__

#include "XHeapFactory.h"


#ifdef XVP_FINAL

void * operator new (size_t size);
void * operator new[] (size_t size);
void * operator new (size_t size, XHeap * pHeap); 
void * operator new[] (size_t size, XHeap * pHeap);
void operator delete (void * pMem); 
void operator delete[] (void * pMem); 
void operator delete (void * pMem, XHeap * pHeap); 
void operator delete[] (void * pMem, XHeap * pHeap);

//void * operator new (size_t size); 
//void * operator new[] (size_t size); 
//void * operator new (size_t size,XHeap* pHeap); 
//void * operator new[] (size_t size, XHeap * pHeap);
//void operator delete (void * pMem);
//void operator delete[] (void * pMem); 
//void operator delete (void * pMem,XHeap* pHeap); 
//void operator delete[] (void * pMem, XHeap * pHeap);

#else //XVP_FINAL

void * operator new (size_t size);
void * operator new[] (size_t size);
void * operator new (size_t size, XHeap * pHeap,const char* pFilename,int lineNum); 
void * operator new[] (size_t size, XHeap * pHeap,const char* pFilename,int lineNum);
void operator delete (void * pMem); 
void operator delete[] (void * pMem); 
void operator delete (void * pMem, XHeap * pHeap,const char* pFilename,int lineNum); 
void operator delete[] (void * pMem, XHeap * pHeap,const char* pFilename,int lineNum);

#endif //XVP_FINAL






#define DECLARE_HEAP \
    public: \
        static void * operator new(size_t size); \
        static void operator delete(void * p, size_t size); \
    private: \
        static XHeap * s_pHeap; 


#define DEFINE_HEAP(className,heapName) \
    XHeap * className::s_pHeap = NULL; \
    void * className::operator new(size_t size) { \
        if (s_pHeap==NULL)  \
            s_pHeap = HeapFactory::CreateHeap(heapName); \
        return ::operator new(size, s_pHeap); \
    } \
    void className::operator delete(void * p, size_t size) { \
        ::operator delete(p); \
    }


#define DEFINE_HIERARCHICALHEAP(className,heapName,parentName) \
    XHeap * className::s_pHeap = NULL; \
    void * className::operator new(size_t size) { \
        if (s_pHeap==NULL)  \
            s_pHeap = HeapFactory::CreateHeap(heapName,parentName); \
        return ::operator new(size, s_pHeap); \
    } \
    void className::operator delete(void * p, size_t size) { \
        ::operator delete(p); \
    }





#endif