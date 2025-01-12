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
//	Module Name:	XModelObject.h
//	Author:			Lynn Duke
//	Creation Date:	6-10-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_OBJECT_H__
#define __X_MODEL_OBJECT_H__


class XModel;
class XLight;

//*****************************************************************************
//! Sub object used to build hierarchical models.
/**
The XModelObject object is used to build hierarchical models.  Each model
object has its own vertex buffer, index buffer and render state.  In fact,
an XModel object is a scene graph of X_MODEL_OBJECTS that function together to
form a rigid body model.
*/
//*****************************************************************************
class XModelObject : public XSceneNode
{
public:
	XU16						m_hierarchyID;
	//XHandle						m_vbHandle;
	//XHandle						m_ibHandle;
	XU8							m_LODIndex;
	XHandle**					m_ppInstanceMats;
	void*						m_pDataTemplate;
	XModel*						m_pParentModel;
	//XVector3D					m_modelSpaceBSphCenter;
	XMatrix4x4					m_texMatrix;
	XSphere						m_modelSpaceBoundingSphere;
	//XAABB						m_aabb;
	//XAABB					m_modelSpaceAABB;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModelObject();
	virtual ~XModelObject();
	
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XU16 getHierarchyID();
	XStatus setHierarchyID(XU16 ID);
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
	virtual void pickLine(XVector3D& lineStart,XVector3D& lineEnd,XMemBlockList<HitInfo>& hits);
	void GetLightIntensity(XVector3D &Position, XVector3D &Normal,XColor& lightIntensity,XMemBlockList<XSceneNode*>& nodeList,XU32 numLights,XLight** ppLights);
	void GetAmbientLightIntensity(XVector3D &Position, XVector3D &Normal,XColor& lightIntensity,XMemBlockList<XSceneNode*>& nodeList,XU32 numRays,XVector3D* pRays);
	virtual void fillLightmap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numLights,XLight** ppLights);
	virtual void fillAOMap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numRays,XVector3D* pRays);
	virtual XF32 getSurfaceArea();
	virtual XActorContainer* getActorContainer();
};

#endif