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
//	Module Name:	XSkinObject.h
//	Author:			Lynn Duke
//	Creation Date:	6-10-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SKIN_OBJECT_H__
#define __X_SKIN_OBJECT_H__





//*****************************************************************************
//! Sub object used to build hierarchical models.
/**
The XSkinObject object is used to build hierarchical models.  Each model
object has its own vertex buffer, index buffer and render state.  In fact,
an XModel object is a scene graph of X_MODEL_OBJECTS that function together to
form a rigid body model.
*/
//*****************************************************************************
class XSkinObject : public XSceneNode
{
private:
	struct SkinInstance
	{
		XHandle skinnedVB;
		XHandle ibHandle;
	};

public:
	XS8*						m_pName;
	XU16						m_hierarchyID;
	//XHandle						m_vbHandle;
	//XHandle						m_ibHandle;
	XU8							m_LODIndex;
	//XHandle						m_matHandle;
	void*						m_pDataTemplate;
	XModel*						m_pParentModel;
	//XVector3D					m_modelSpaceBSphCenter;
	XMatrix4x4					m_texMatrix;
	XSphere						m_modelSpaceBoundingSphere;
	//XAABB						m_aabb;
	//XAABB					m_modelSpaceAABB;

	vector<SkinInstance>	m_skinInstances;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSkinObject();
	virtual ~XSkinObject();
	
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XS8* getName();
	void setName(XS8* pName);
	XU16 getHierarchyID();
	XStatus setHierarchyID(XU16 ID);
	XStatus setGeometryList(vector<XSkinGeometry*>& geometryList);
	//XHandle getSkinnedVertexBuffer();
	//XStatus setSkinnedVertexBuffer(XHandle vbHandle);
	//XHandle getMaterialHandle();
	XModel* getParentModel();
	XStatus getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared);
	XStatus setLODIndex(XU8 LODIndex);
	XU8 getLODIndex();
	XStatus setTexMatrix(XMatrix4x4& texMatrix);
	XMatrix4x4& getTexMatrix();
	//virtual XU32 getNumLODs();
	//virtual XStatus getGeometryList(XU32 LODIndex,vector<XGeometry*>& geometryList);
	virtual XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
	//XMaterial* getMaterialInterface();
	//XHandle getVertexBufferHandle();
	//XHandle getIndexBufferHandle();
	//XStatus setMaterialHandle(XHandle materialHandle);
	//XStatus setVertexBufferHandle(XHandle vertexBufferHandle);
	//XStatus setIndexBufferHandle(XHandle indexBufferHandle);
	XStatus setBoundingSphere(XSphere& sphere);
	virtual XStatus prepForRender(XF32 dt,XScene* pScene);
	XStatus render(XRenderEvent* pRenderEvent);
	XMatrix4x4& getPoseTransform();
	XStatus updateTransforms(XF32 dt, XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);

	virtual void pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits);
	virtual XF32 getSurfaceArea(); 
	virtual XActorContainer* getActorContainer();
};

#endif