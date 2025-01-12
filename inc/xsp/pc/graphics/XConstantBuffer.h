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
//	Module Name:	XConstantBuffer.h
//	Author:			Lynn Duke
//	Creation Date:	2-20-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CONSTANT_BUFFER_H__
#define __X_CONSTANT_BUFFER_H__

#include <XResource.h>
#include <XOSMgr.h>
#include <XResource.h>
#include <XGraphicsMgrTypes.h>
#include <d3d10.h>


//*****************************************************************************
//! Constant Buffer.
//*****************************************************************************
class XConstantBuffer : public XResource
{
private:
	ID3D10Buffer*			m_pConstantBuffer;
	XBOOL					m_dynamic;
	XBOOL					m_locked;
	
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XConstantBuffer();
	~XConstantBuffer();

	//*****************************************************************************
	//Standard Vertex Buffer Functions
	//*****************************************************************************
	XStatus init(XU32 sizeInBytes,XBOOL dynamic);
	XStatus destroy();
	void* lock(D3D10_MAP mapType);
	XStatus unlock();
	XBOOL isLocked();
	XStatus setData(void* pData,XU32 dataSizeInBytes);
	ID3D10Buffer* getD3DConstantBuffer();
	XBOOL isDynamic();
};


#endif