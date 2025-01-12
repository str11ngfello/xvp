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

#ifndef __X_HEAP_FACTORY_H__
#define __X_HEAP_FACTORY_H__

#include "XHeap.h"


#define X_MAX_HEAPS	128

class XHeapFactory
{
public:
    static XHeap * CreateHeap (char * name);
    static XHeap * CreateHeap (char * name, char * parent);
	static void DestroyHeap (XHeap * pHeap);

	static XHeap * GetDefaultHeap ();
    static void   PrintInfo ();

    static int  GetMemoryBookmark ();
    //static void ReportMemoryLeaks (/*int nBookmark*/);

#ifndef XVP_FINAL
    static void dumpAllocations (/*int nBookmark1, int nBookmark2*/);
#endif
    
private:
    static XStatus init();
	static XHeap * GetRootHeap ();
    static XHeap * FindHeap (char * name);
    static XHeap * CreateNewHeap (char * name);
    static void PrintHeapTree(XHeap * pHeap);


    static XHeap * s_pRootHeap;   
    static XHeap * s_pDefaultHeap;   
    static XHeap s_heaps[X_MAX_HEAPS];
};


#endif