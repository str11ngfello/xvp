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
//	Module Name:	XModel.h
//	Author:			Lynn Duke
//	Creation Date:	5-22-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_H__
#define __X_MODEL_H__


class XLight;


//*****************************************************************************
//! Represents a hierarchical collection of scene nodes.
/** 
The XModel object represents a hierarchical collection of scene nodes.  Since
the model is made up of scene nodes, you can query for the individual scene
nodes and treat them as you would any other XSceneNode object.  Also, models
can be added to X-VP's scene graph like any other node.  When inside
the scene graph, the model functions as its own "mini" scene graph.  When the
model receives an update or render message from the engine, it propogates this
message to all of its scene nodes in a depth-first traversal similar to that
of the main scene graph.

In fact, the XSkinMesh object is a derivation of the XModel object that takes
advantage of the hierarchical nature of the model's construction.  You can
derive from XModel and implement your own object, which to the engine, 
functions as a single scene node, but internally is made up of many
different scene nodes.

See below for more information.
*/
//*****************************************************************************
class XModel : public XSceneNode 
{
protected:
	XPose*						m_pPose;
	XBOOL						m_inited;

public:	
	XU32						m_numNodes;			
	XSceneGraph					m_nodeTree;
 	XSceneNode**				m_pNodeArray;
	void*						m_pDataTemplate;
	XMemBlockList<XSkinObject*> m_skins;


	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModel();
	virtual ~XModel();
	
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc(); 
	virtual XStatus init();
	virtual XStatus destroy();
	void dumpScene(ofstream& outputFile,XU32 index);
	XStatus attachController(XS8* pNodeName,XController* pController,XBOOL recursive = false);
	XStatus detachController(XS8* pNodeName,XController* pController,XBOOL recursive = false);
	XBOOL isControllerAttached(XS8* pNodeName,XController* pController);
	XStatus detachAllControllers(XS8* pNodeName,XBOOL recursive = false);
	XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);

	virtual XStatus attachNode(XS8* pParentNodeName,XS8* pNodeName,XSceneNode* pNode);
	virtual XStatus detachNode(XS8* pNodeName);
	virtual XBOOL isNodeAttached(XS8* pNodeName);
	XSceneNode* getNode(XS8* pNodeName);
	virtual XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus prepForRender(XF32 dt,XScene* pScene);
	static XSceneGraphNode* onLevelDown(XSceneGraphNode* pNode,void* pUserData);
	static XSceneGraphNode* onLevelUp(XSceneGraphNode* pNode,void* pUserData);

	XStatus walkNodes(XS8* pStartNodeName,void (*callback)(XSceneNode* pNode));
	XBOOL containsNode(XSceneNode* pSceneNode);
	
	XHandle getMaterial(XS8* pMaterialName);

	/*XHandle getMaterialHandle();
	XStatus setMaterialHandle(XHandle materialHandle);

	XStatus setMaterialHandle(XS8* pNodeName,XHandle matHandle,XBOOL recursive = false);
	XHandle getMaterialHandle(XS8* pNodeName);

	XStatus setTextureHandle(XS8* pNodeName,XU32 texStage,XHandle texHandle,
							 XBOOL recursive);
	XHandle getTextureHandle(XS8* pNodeName,XU32 texStage);
	*/
	XStatus setPose(XPose* pPose);
	XPose* getPose();

	XStatus enableUpdate(XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isUpdateEnabled(XS8* pNodeName);

	XStatus enableRender(XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isRenderEnabled(XS8* pNodeName);

	XStatus enableFrustumCull(XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isFrustumCullEnabled(XS8* pNodeName);

	XStatus enableRenderBoundingSphere(XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isRenderBoundingSphereEnabled(XS8* pNodeName);

	XStatus setViewportMask(XU8 mask,XS8* pNodeName,XBOOL recursive = false);
	XU8 getViewportMask(XS8* pNodeName);

	XStatus enableViewport(XU32 viewportIndex,XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isViewportEnabled(XU32 viewportIndex,XS8* pNodeName);


/*	XStatus enableAlphaTest(XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isAlphaTestEnabled(XS8* pNodeName);

	XStatus enableAlphaBlend(XBOOL enable,XS8* pNodeName,XBOOL recursive = false);
	XBOOL isAlphaBlendEnabled(XS8* pNodeName);

	XStatus setBlendOp(XS8* pNodeName,X_BLEND_OP_TYPE value,XBOOL recursive = false);
	X_BLEND_OP_TYPE getBlendOp(XS8* pNodeName);

	XStatus setFillMode(XS8* pNodeName,X_FILL_MODE_TYPE value,XBOOL recursive = false);
	X_FILL_MODE_TYPE getFillMode(XS8* pNodeName);
	
	XStatus setSrcBlend(XS8* pNodeName,X_BLEND_TYPE value,XBOOL recursive = false);
	X_BLEND_TYPE getSrcBlend(XS8* pNodeName);

	XStatus setDestBlend(XS8* pNodeName,X_BLEND_TYPE value,XBOOL recursive = false);
	X_BLEND_TYPE getDestBlend(XS8* pNodeName);
	
	XStatus setDepthFunc(XS8* pNodeName,X_CMP_FUNC_TYPE value,XBOOL recursive = false);
	X_CMP_FUNC_TYPE getDepthFunc(XS8* pNodeName);
	
	XStatus setAlphaFunc(XS8* pNodeName,X_CMP_FUNC_TYPE value,XBOOL recursive = false);
	X_CMP_FUNC_TYPE getAlphaFunc(XS8* pNodeName);
	
	XStatus setAlphaRef(XS8* pNodeName,XU32 value,XBOOL recursive = false);
	XU32 getAlphaRef(XS8* pNodeName);
	
	XStatus setDepthBias(XS8* pNodeName,XF32 value,XBOOL recursive = false);
	XF32 getDepthBias(XS8* pNodeName);
	
	XStatus setAddressU(XS8* pNodeName,XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value,XBOOL recursive = false);
	X_TEXTURE_ADDRESS_TYPE getAddressU(XS8* pNodeName,XU32 stageNum);

    XStatus setAddressV(XS8* pNodeName,XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value,XBOOL recursive = false);
	X_TEXTURE_ADDRESS_TYPE getAddressV(XS8* pNodeName,XU32 stageNum);

   	XStatus setAddressW(XS8* pNodeName,XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value,XBOOL recursive = false);
	X_TEXTURE_ADDRESS_TYPE getAddressW(XS8* pNodeName,XU32 stageNum);
	
	XStatus setMagFilter(XS8* pNodeName,XU32 stageNum,X_TEXTURE_FILTER_TYPE value,XBOOL recursive = false);
	X_TEXTURE_FILTER_TYPE getMagFilter(XS8* pNodeName,XU32 stageNum);

	XStatus setMinFilter(XS8* pNodeName,XU32 stageNum,X_TEXTURE_FILTER_TYPE value,XBOOL recursive = false);
	X_TEXTURE_FILTER_TYPE getMinFilter(XS8* pNodeName,XU32 stageNum);

   	XStatus setMipFilter(XS8* pNodeName,XU32 stageNum,X_TEXTURE_FILTER_TYPE value,XBOOL recursive = false);
	X_TEXTURE_FILTER_TYPE getMipFilter(XS8* pNodeName,XU32 stageNum);
*/

	XStatus traverseDepthFirst(XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData);

	void pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits);
	void pickLine(XVector3D& lineStart,XVector3D& lineEnd,XMemBlockList<HitInfo>& hits);
	void pickAll(XMemBlockList<XSceneNode*>& hits);
	XF32 getSurfaceArea();

	XSphere getActorContainerBoundingSphere();
	void generateLightmap(XU32 numModels,XModel** ppModels,XU32 numLights,XLight** ppLights,XS8* pLMFilename,XU32 texSize,XF32 blurSize);
	void generateAOMap(XU32 numModels,XModel** ppModels,XU32 numRays,XVector3D* pRays,XS8* pAOMapFilename,XU32 texSize,XF32 blurSize);
	friend XModelObject;
	friend XSkinObject;
};

#endif
