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
//	Module Name:	XBaseIndexBuffer.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_INDEX_BUFFER_H__
#define __X_BASE_INDEX_BUFFER_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <d3d10.h>



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Base Index Buffer.
//*****************************************************************************
class XBaseIndexBuffer : public XResource
{
protected:
	XU32						m_numIndices;
	ID3D10Buffer*				m_ib;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseIndexBuffer();
	virtual ~XBaseIndexBuffer();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus init(XU32 numIndices,bool dynamic) = 0;
	virtual XStatus destroy() = 0;
	virtual XStatus setToRenderingDevice() = 0;
	virtual XU16* lock(X_LOCK_TYPE lockType) = 0;
	virtual XStatus unlock() = 0;
	virtual XU32 getLength() = 0;		
};


#endif