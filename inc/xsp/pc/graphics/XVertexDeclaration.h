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
//	Module Name:	XVertexDeclaration.h
//	Author:			Lynn Duke
//	Creation Date:	5-8-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VERTEX_DECLARATION_H__
#define __X_VERTEX_DECLARATION_H__

#include <XGraphicsMgrTypes.h>
#include <XResource.h>
#include <d3d10.h>


#define	X_MAX_NUMBER_OF_STREAMS		16

//*****************************************************************************
//! Vertex Declaration.
//*****************************************************************************
class XVertexDeclaration : public XResource
{
private:
	XVertexDesc						m_desc;
	ID3D10InputLayout*				m_pInputLayout;
	XU32							m_numStreams;

#ifdef XVP_EDITOR
	D3D10_INPUT_ELEMENT_DESC		m_inputDesc[X_MAX_NUMBER_OF_STREAMS];
#endif

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XVertexDeclaration();
	~XVertexDeclaration();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XVertexDesc& desc,XS8* pShaderName);
	XStatus destroy();
	ID3D10InputLayout* getDeclaration();
	XStatus getDesc(XVertexDesc& desc);
	XU32 getNumStreams();

#ifdef XVP_EDITOR
	D3D10_INPUT_ELEMENT_DESC* getInputDesc() { return m_inputDesc; }
#endif
};


#endif