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
//	Module Name:	XModelObjectTemplate.h
//	Author:			Lynn Duke
//	Creation Date:	6-10-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_OBJECT_TEMPLATE_H__
#define __X_MODEL_OBJECT_TEMPLATE_H__




//# of LODS for a geometry object.  Must be at least 1 because this number includes
//the highest LOD.  
#define X_MAX_MODEL_OBJECT_LODS 6


//*****************************************************************************
//*****************************************************************************
class XModelObjectTemplate : public XSceneNode
{
public:
	XU16		m_hierarchyID;
	XU8			m_lastLODIndex;
	XAABB						m_aabb;
	vector<vector<XGeometry*>>		m_pGeometryLists;
	XBOOL m_isBone;
	XMatrix4x4 m_invBoneTransform;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModelObjectTemplate();
	virtual ~XModelObjectTemplate();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	XStatus setLODInfo(XU32 LODIndex,XHandle vbHandle,XHandle ibHandle);
	XU16 getHierarchyID();
	XStatus setHierarchyID(XU16 ID);
	XStatus addGeometry(XU32 LODIndex,XHandle vbHandle,XHandle ibHandle,XVector3D* pPositionCache,XVector3D* pNormalCache,XVector2D* pUVCache,
						XU16* pIndexCache,XHandle matHandle);
	XU32 getNumLODs();
	XStatus getGeometryList(XU32 LODIndex,vector<XGeometry*>& geometryList);
	//XHandle getMaterialHandle();
	//XMaterial* getMaterialInterface();
	//XHandle getVertexBufferHandle();
	//XHandle getIndexBufferHandle();
	//XStatus setMaterialHandle(XHandle materialHandle);
	//XStatus setVertexBufferHandle(XHandle vertexBufferHandle);
	//XStatus setIndexBufferHandle(XHandle indexBufferHandle);
	
};

#endif