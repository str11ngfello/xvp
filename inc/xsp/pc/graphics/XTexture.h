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
//	Module Name:	XTexture.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TEXTURE_H__
#define __X_TEXTURE_H__

#include <XOSMgr.h>
#include <XBaseTexture.h>



//*****************************************************************************
//! Texture Object.
//*****************************************************************************
class XTexture : public XBaseTexture
{
private:
	ID3D10ShaderResourceView*		m_pTexture;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTexture();
	~XTexture();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus initFromFile(XS8* pFilename);
	XStatus initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize);
	ID3D10ShaderResourceView* getD3DTexture();
	XStatus destroy();
};


#endif