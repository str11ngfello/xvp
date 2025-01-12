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
//	Module Name:	XIndexBuffer.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_INDEX_BUFFER_H__
#define __X_INDEX_BUFFER_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XBaseIndexBuffer.h>



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Index Buffer.
//*****************************************************************************
class XIndexBuffer : public XBaseIndexBuffer
{
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XIndexBuffer();
	~XIndexBuffer();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus init(XU32 numIndices,bool dynamic);
	XStatus destroy();
	XStatus setToRenderingDevice();
	WORD* lock(X_LOCK_TYPE lockType);
	XStatus unlock();
	XU32 getLength();
	void setData(void* pData,XU32 offset,XU32 dataSizeInBytes);
};


#endif