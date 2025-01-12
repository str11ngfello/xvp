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

#ifndef __X_SKIN_OBJECT_TEMPLATE_H__
#define __X_SKIN_OBJECT_TEMPLATE_H__





//*****************************************************************************
//*****************************************************************************
class XSkinObjectTemplate : public XSceneNode
{
public:
	
public:
	XS8*												m_pName;
	XU8													m_lastLODIndex;
	vector<vector<XSkinGeometry*>>						m_pGeometryLists;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSkinObjectTemplate();
	virtual ~XSkinObjectTemplate();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	XS8* getName();
	void setName(XS8* pName);
	/*
	XStatus setLODInfo(XU32 LODIndex,XHandle vbHandle,XHandle ibHandle);
	XU16 getHierarchyID();
	XStatus setHierarchyID(XU16 ID);*/
	XStatus addGeometry(XU32 LODIndex,XVertexDesc& desc,X_PRIMITIVE_TYPE pt,SkinVert* pOriginalVB,XHandle ibHandle,XHandle matHandle,XU32 numVerts,
						XVector3D* pSkinnedVertCache,XU16* pIndexCache);
	XU32 getNumLODs();
	XStatus getGeometryList(XU32 LODIndex,vector<XSkinGeometry*>& geometryList);
};

#endif