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
//	Module Name:	X2DMovie.h
//	Author:			Lynn Duke
//	Creation Date:	4-29-06
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SKIN_GEOMETRY_H__
#define __X_SKIN_GEOMETRY_H__



//# of LODS for a skin geometry object.  Must be at least 1 because this number includes
//the highest LOD.  
#define X_MAX_SKIN_OBJECT_LODS 6
#define MAX_BONES 8

struct SkinVert
{
	XVector3D position;
	XU32 color;
	XVector3D normal;
	XVector2D uv;
	XVector3D tangent;
	XVector3D binormal;
	XU32 numBones;
	XF32 boneWeights[MAX_BONES];
	XF32 boneIndices[MAX_BONES];

	SkinVert()
	{
		numBones = 0;
		for (XU32 i = 0;i < MAX_BONES;++i)
		{
			boneWeights[i] = 0.0f;
			boneIndices[i] = 0.0f;
		}
	}
};


//*****************************************************************************
//*****************************************************************************
class XSkinGeometry
{
public:
	XVertexDesc				m_vertexDesc;
	X_PRIMITIVE_TYPE		m_primType;
	SkinVert*				m_pOriginalVB;
	XHandle					m_ibHandle;
	XHandle					m_matHandle;
	XU32					m_numVerts;
	XVector3D*				m_pSkinnedVertCache;
	XU16*					m_pIndexCache;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSkinGeometry();
	~XSkinGeometry();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XVertexDesc& desc,X_PRIMITIVE_TYPE pt,SkinVert* pOriginalVB,XHandle ibHandle,XHandle matHandle,XU32 numVerts,
				XVector3D* pSkinnedVertCache,XU16* pIndexCache);
	XStatus destroy();
};

#endif