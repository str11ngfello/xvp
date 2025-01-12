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
//	Module Name:	XHLSLGeometryShader.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_HLSL_GEOMETRY_SHADER_H__
#define __X_HLSL_GEOMETRY_SHADER_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XBaseGeometryShader.h>



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! HLSL Geometry Shader.
//*****************************************************************************
class XHLSLGeometryShader : public XBaseGeometryShader
{
protected:
    ID3D10Blob* m_pSignatureBlob;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XHLSLGeometryShader();
	~XHLSLGeometryShader();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus initFromSourceCode(XS8* pFilename,XS8* entryFunctionName);
	XStatus initFromObjectCode(void* pObjectCode,XU32 objectCodeSize);
	XStatus destroy();
	ID3D10GeometryShader* getD3DGeometryShader();
};



#endif