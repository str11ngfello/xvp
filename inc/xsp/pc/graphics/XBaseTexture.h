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
//	Module Name:	XBaseTexture.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_TEXTURE_H__
#define __X_BASE_TEXTURE_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Base Texture.
//*****************************************************************************
class XBaseTexture : public XResource
{
protected:
	D3DX10_IMAGE_INFO m_info;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseTexture();
	virtual ~XBaseTexture();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus initFromFile(XS8* pFilename) = 0;
	virtual XStatus initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize) = 0;

	virtual XStatus destroy() = 0;
	virtual ID3D10ShaderResourceView* getD3DTexture() = 0;

	XU32 getWidth()				{ return m_info.Width; }
    XU32 getHeight()			{ return m_info.Height; }
    XU32 getDepth()				{ return m_info.Depth; }
    XU32 getMipLevels()			{ return m_info.MipLevels; }
    XU32 getMiscFlags()			{ return m_info.MiscFlags; }
    DXGI_FORMAT getFormat()		{ return m_info.Format; }
};


#endif