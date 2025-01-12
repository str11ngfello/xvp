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
//	Module Name:	XBasePixelShader.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_PIXEL_SHADER_H__
#define __X_BASE_PIXEL_SHADER_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <d3d10.h>



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Base Pixel Shader.
//*****************************************************************************
class XBasePixelShader : public XResource
{
protected:
	ID3D10PixelShader*		m_pPixelShader;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBasePixelShader();
	virtual ~XBasePixelShader();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus initFromSourceCode(XS8* pFilename,XS8* entryFunctionName) = 0;
	virtual XStatus initFromObjectCode(void* pObjectCode,XU32 objectCodeSize) = 0;
	virtual XStatus destroy() = 0;
	virtual ID3D10PixelShader* getD3DPixelShader() = 0;
};


#endif