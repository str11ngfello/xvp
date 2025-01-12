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
//	Module Name:	XVertexBuffer.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VERTEX_BUFFER_H__
#define __X_VERTEX_BUFFER_H__

#include <XResource.h>
#include <XOSMgr.h>
#include <XResource.h>
#include <XVector2D.h>
#include <XVector3D.h>
#include <XVector4D.h>
#include <XVertexDeclaration.h>
#include <XBaseIndexBuffer.h>
#include <XColor.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


//*****************************************************************************
//! Vertex Buffer.
//*****************************************************************************
class XVertexBuffer : public XResource
{
private:
	ID3D10Buffer*						m_pPositionsVB;
	XVector3D*							m_pPositions;

	ID3D10Buffer**						m_pColorsVB;
	XU32**								m_pColors;

	ID3D10Buffer**						m_pNormalsVB;
	XVector3D**							m_pNormals;

	ID3D10Buffer**						m_pUVsVB;
	XVector2D**							m_pUVs;

	ID3D10Buffer**						m_pTangentsVB;
	XVector3D**							m_pTangents;

	ID3D10Buffer**						m_pBinormalsVB;
	XVector3D**							m_pBinormals;

	ID3D10Buffer**						m_pBlendIndicesVB;
	XVector4D**							m_pBlendIndices;

	ID3D10Buffer**						m_pBlendWeightsVB;
	XVector4D**							m_pBlendWeights;
	
	XVertexDeclaration*					m_pVertexDeclaration;
	XVertexDesc							m_desc;

	X_PRIMITIVE_TYPE					m_primitiveType;
	XU32								m_numVertices;
	XU32								m_sizeInBytes;
	XBOOL								m_dynamic;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XVertexBuffer();
	~XVertexBuffer();

	//*****************************************************************************
	//Standard Vertex Buffer Functions
	//*****************************************************************************
	XStatus init(XHandle vdHandle,X_PRIMITIVE_TYPE primType,XU32 numVertices,
				 bool dynamic);
	XStatus destroy();
	//static XStatus initDeclarations();
	//static XStatus destroyDeclarations();
	static XStatus renderUserVertexBuffer(X_PRIMITIVE_TYPE pType,
											  XHandle vertexDeclaration,
											XU32 numPrimitives,void* pPrimitiveArray,
											XU32 primitiveStride);/*
	static XStatus renderUserIndexedVertexBuffer(X_PRIMITIVE_TYPE pType,X_VERTEX_TYPE vType,
									XU32 numVertices,void* pVertices,XU32 numIndices,void* pIndices,
									XU32 numPrimitives,	XU32 vertexStride);*/

	XStatus lockAllStreams(X_LOCK_TYPE lockType);
	XStatus unlockAllStreams();
	XStatus lockStream(X_SEMANTIC_TYPE semantic,XU32 streamIndex,X_LOCK_TYPE lockType);
	XStatus unlockStream(X_SEMANTIC_TYPE semantic,XU32 streamIndex);
	XU32 getNumVerts();
	//XU32 getPrimCount();
	//X_VERTEX_TYPE getType();
	X_PRIMITIVE_TYPE getPrimType();
	XStatus render();
	XStatus render(XU32 primCount);
	XStatus renderWithIndexBuffer(XBaseIndexBuffer* pIndexBuffer);

	XVertexDesc* getVertexDesc();

	XVector3D* getPositionStream();
	void setPosition(XU32 vertIndex,XVector3D& pos); 
	void setPosition(XU32 vertIndex,XF32 x,XF32 y,XF32 z); 
	void setPositionData(void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XVector3D* getNormalStream(XU32 streamIndex);
	void setNormal(XU32 vertIndex,XU32 streamIndex,XVector3D& normal); 
	void setNormal(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z); 
	void setNormalData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XU32* getColorStream(XU32 streamIndex);
	void setColor(XU32 vertIndex,XU32 streamIndex,XColor& color);
	void setColor(XU32 vertIndex,XU32 streamIndex,XU8 r,XU8 g,XU8 b,XU8 a); 
	void setColor(XU32 vertIndex,XU32 streamIndex,XU32 ABGR); 
	void setColorData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XVector2D* getUVStream(XU32 streamIndex);
	void setUV(XU32 vertIndex,XU32 streamIndex,XVector2D& uv); 
	void setUV(XU32 vertIndex,XU32 streamIndex,XF32 u,XF32 v); 
	void setUVData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XVector3D* getTangentStream(XU32 streamIndex);
	void setTangent(XU32 vertIndex,XU32 streamIndex,XVector3D& tangent); 
	void setTangent(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z); 
	void setTangentData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XVector3D* getBinormalStream(XU32 streamIndex);
	void setBinormal(XU32 vertIndex,XU32 streamIndex,XVector3D& binormal); 
	void setBinormal(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z); 
	void setBinormalData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XVector4D* getBlendIndicesStream(XU32 streamIndex);
	void setBlendIndices(XU32 vertIndex,XU32 streamIndex,XVector4D& indices); 
	void setBlendIndices(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z,XF32 w); 
	void setBlendIndicesData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes);
	
	XVector4D* getBlendWeightsStream(XU32 streamIndex);
	void setBlendWeights(XU32 vertIndex,XU32 streamIndex,XVector4D& weights); 
	void setBlendWeights(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z,XF32 w);
	void setBlendWeightsData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes); 

#ifdef XVP_EDITOR
	XBOOL verifyInputSignature();
#endif
};


#endif