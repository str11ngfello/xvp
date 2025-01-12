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
//	Module Name:	XSkinMeshTemplate.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SKIN_MESH_TEMPLATE_H__
#define __X_SKIN_MESH_TEMPLATE_H__




//*****************************************************************************
//XSkinVertex structure.
//*****************************************************************************
struct XBoneInfo
{
	XU32 numBones;
	XU32* pBoneIDList;
	XF32* pBoneWeightList;
};



//*****************************************************************************
//! Loads in a skin mesh from a .xsm file.
/**
The XSkinMeshTemplate object is used to hold data for a skin mesh in memory.
The XSkinMeshTemplate can load in a predefined skin mesh from a .xsm file.
Once a skin mesh template has been loaded, you can then create any number of 
skin mesh instances from this template by using one or more XSkinMesh objects.  

The steps below illustrate typical usage of an XSkinMeshTemplate object in 
conjunction with XSkinMesh objects:

1.  Initialize an XSkinMeshTemplate with a .xsm file. <br>
2.  Generate any number of XSkinMesh objects from the XSkinMeshTemplate
	object.<br>
3.  Add the XSkinMesh objects to the scene.

The XSkinMeshTemplate object by itself is not enough to render the skin mesh.
It only provides a way to generate skin mesh instances.  You must create
at least one skin mesh instance from the template to actually start rendering
the skin mesh.
*/
//*****************************************************************************
class XSkinMeshTemplate
{
public:
	struct XSkinVertex
	{
		XVector3D	pos;			/*!< Vertex position */
		XVector4D	blendWeights;	/*!< Up to 4 blend weights */
		XVector4D	blendIndices;	/*!< Up to 4 blend indices */
		XVector3D	normal;			/*!< Vertex Normal */
		XU32		diffuse;		/*!< Diffuse color */
		XVector2D	texCoords;		/*!< 2 texture coordinates */
		XVector2D	numBones;		/*!< Up to 4 bones can affect this vertex */
	};

private:
	XGeometry						m_geometry;
	//XHandle							m_vbHandle;
	//XHandle							m_ibHandle;
	//XHandle							m_renderVBHandle;
	//XHandle							m_matHandle;
	XSceneGraph						m_nodeTree;
	vector<XBone*>					m_nodeArray;
	XU32							m_refCount;
	XU32							m_numVerts;
	XSphere							m_boundingSphere;
	vector<XBoneInfo>				m_boneInfoList;
	vector<XParentChildCombo>		m_parentChildList;
 	XU32							m_numBones;
	static XBOOL					m_determineCaps;
	XPose							m_defaultPose;
	XBOOL							m_inited;

public:	
	//*************************************************************************
	//Constructors/Destructors
	//*************************************************************************
	XSkinMeshTemplate();
	~XSkinMeshTemplate();

	//*************************************************************************
	//Public Interface
	//*************************************************************************
	XStatus init(XS8* pFilename);
	XStatus destroy();
	XStatus generateSkinMesh(XSkinMesh& skinMesh/*,XBOOL uniqueRS = false*/);
	XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
	friend XSkinMesh;
};


#endif