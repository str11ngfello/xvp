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
//	Module Name:	XSkinMesh.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SKIN_MESH_H__
#define __X_SKIN_MESH_H__


class XSkinMeshTemplate;


//*****************************************************************************
//! Represents a soft skin mesh deformable by bones.
/**
The XSkinMesh object is used to represent objects with "skin".  The term skin
in this context refers to a stretchable, deformable geometrical mesh.  Skin can 
be used to model human characters, plant life, or any other life-like object.

To initialize an XSkinMesh object, you must first load in an
XSkinMeshTemplate object.  Once you have a template, you can create any
number of XSkinMesh objects from it.  See XSkinMeshTemplate for details.
All skin mesh instances generated from the same template will share the same
geometry, render state, and other memory intensive data, but will contain their 
own instance data such as world space transforms, controller attachements, etc.
*/
//*****************************************************************************
class XSkinMesh : public XModel
{
public:
	//XHandle						m_ibHandle;
	//XHandle						m_renderVBHandle;
	XU32						m_numVerts;
	vector<XBone*>				m_boneIDIndexArray;
	XSkinMeshTemplate*			m_pSkinMeshTemplate;
	XSphere						m_modelSpaceBoundingSphere;


	//*****************************************************************************
	//Constructors/Destructors
	//****************************************************************************
	XSkinMesh();
	~XSkinMesh();

	//*****************************************************************************
	//Public Interface
	//****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	//XHandle getIndexBufferHandle();
	//XStatus setIndexBufferHandle(XHandle indexBufferHandle);
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus prepForRender(XF32 dt,XScene* pScene);
	XStatus render(XRenderEvent* pRenderEvent);
	XU32 getNumBones();
	vector<XBone*>* getBones();
};

#endif

