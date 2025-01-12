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
//	Module Name:	XBaseGeometryShader.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_GEOMETRY_SHADER_H__
#define __X_BASE_GEOMETRY_SHADER_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>




#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Base Geometry Shader.
//*****************************************************************************
class XBaseGeometryShader : public XResource
{
protected:

	ID3D10GeometryShader*	m_pGeometryShader;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseGeometryShader();
	virtual ~XBaseGeometryShader();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus initFromSourceCode(XS8* pFilename,XS8* entryFunctionName) = 0;
	virtual XStatus initFromObjectCode(void* pObjectCode,XU32 objectCodeSize) = 0;
	virtual XStatus destroy() = 0;
	virtual ID3D10GeometryShader* getD3DGeometryShader() = 0;

};


#endif