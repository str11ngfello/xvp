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
//	Module Name:	XHLSLVertexShader.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_HLSL_VERTEX_SHADER_H__
#define __X_HLSL_VERTEX_SHADER_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XBaseVertexShader.h>



#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! HLSL Vertex Shader.
//*****************************************************************************
class XHLSLVertexShader : public XBaseVertexShader
{
protected:
    ID3D10Blob* m_pSignatureBlob;

#ifdef XVP_EDITOR
	ID3D10ShaderReflection* m_pReflection;
#endif

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XHLSLVertexShader();
	~XHLSLVertexShader();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus initFromSourceCode(XS8* pFilename,XS8* entryFunctionName);
	XStatus initFromSourceCodeString(XS8* pBuffer,XU32 bufferLen,XS8* entryFunctionName);
	XStatus initFromObjectCode(void* pObjectCode,XU32 objectCodeSize);
	XStatus destroy();
	ID3D10VertexShader* getD3DVertexShader();
	void* getShaderSignature();
	XU32 getShaderSignatureLength();

#ifdef XVP_EDITOR
	void* getShaderReflection() { return m_pReflection; }
#endif
};



#endif