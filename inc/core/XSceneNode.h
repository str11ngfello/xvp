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
//	Module Name:	XSceneNode.h
//	Author:			Lynn Duke
//	Creation Date:	6-6-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SCENE_NODE_H__
#define __X_SCENE_NODE_H__

class XScene;
class XController;
class XShaderController;
class XActorContainer;
class XSceneGraphNode;
class XLight;

typedef ULONG ulong;
typedef UINT uint;
typedef USHORT ushort;
#include <cimg.h>
using namespace cimg_library;

struct HitInfo
{
	XSceneNode* pSceneNode;
	XVector3D intersectionPoint;

	XBOOL operator== (HitInfo& hi)
	{
		return (pSceneNode == hi.pSceneNode);
	}
};
//*****************************************************************************
//IDs for scene node types.  A scene node type is a 32 bit number where the upper
//16 bits make up the baseID and the lower 16 bits make up the extID.  An example
//scene node type might have a baseID of X_SNID_MODEL and an extID of 
//X_SNID_SKIN_MESH.  This means that the scene node's base type is model but
//the actual type is a skin mesh.  The enum list can never go over 65535 elements.  
//Users may define new IDs by declaring an enum starting at X_SNID_USER_DEFINED.
//*****************************************************************************
typedef XU32 X_SCENE_NODE_TYPE;
enum X_SCENE_NODE_IDS
{
	X_SNID_UNKNOWN,
	X_SNID_MODEL,
	X_SNID_MODEL_OBJECT,
	X_SNID_SKIN_MESH,
	X_SNID_BONE,
	X_SNID_CAMERA,
	X_SNID_CAMERA_OBJECT,
	X_SNID_PARTICLE_SYSTEM,
	X_SNID_PARTICLE_EMITTER,
	X_SNID_LIGHT,
	X_SNID_ACTOR_CONTAINER,
	X_SNID_DECAL_MGR,
	X_SNID_USER_DEFINED
};

#define X_SNT_MAKE(baseID,extID) ((X_SCENE_NODE_TYPE)(baseID << 16) | extID)
#define X_SNT_GET_BASE_SNID(type) ((X_SCENE_NODE_TYPE)(type >> 16))
#define X_SNT_GET_EXT_SNID(type) ((X_SCENE_NODE_TYPE)(type & 0x0000FFFF))
#define X_SNT_SET_BASE_SNID(type,baseID) (type = type & 0x0000FFFF;type = type | (baseID << 16))
#define X_SNT_SET_EXT_SNID(type,extID) (type = type & 0xFFFF0000;type = type | extID)



//*****************************************************************************
//Scene Node Property Defines
//*****************************************************************************
#define X_SNP_RENDER					0x01 
#define X_SNP_UPDATE					0x02
#define X_SNP_ALPHA_QUEUE				0x04
#define X_SNP_FRUSTUM_CULL				0x08
#define X_SNP_RENDER_BOUNDING_SPHERE	0x10
#define X_SNP_UNIQUE_RS					0x20



//*****************************************************************************
//! The fundamental object managed by X-VP's Scene Graph.
/**
The XSceneNode object is one of the most fundamental objects in the entire
engine.  It is a base object from which all types of scene nodes derive.  It 
provides basic functionality required by all objects that wish to be managed
by X-VP's scene graph.  It also knows how to communicate with controllers
and other important engine objects.

You can derive from this object and override key functions to create any type
of object you want.  For example, X-VP ships with specific types of 
nodes such as particle systems, skin meshes, hierarchical models, cameras,
etc....all of which inherit from XSceneNode somewhere up their class hierarchy.
Although these nodes are derived from XSceneNode, they are all very different
objects designed to function a very specific way.  Some nodes render their geometry
to the screen (particle system), while others don't even have geometry and
therefore don't render anything to the screen. Some of the nodes even contain
there own private "mini" scene graphs with additional scene nodes packed inside
(models and skin meshes). The possibilities are virtually infinite as to the type
of scene nodes you can create with simple derivation from XSceneNode.

The XSceneNode object provides vertex buffer, index buffer and render state
indicies that may be used if needed as well as 3D transformation information
such as a model space transform, local transform, etc.  

Any node that is derived from XSceneNode gains the ability to have controllers
attached to it.  These controllers can influence the node with specific
behaviors.  See the XController object for more information.

See the member documentation for details.
*/
//*****************************************************************************
class XSceneNode : public XPropertyBag
{
protected:
	XMatrix4x4					m_modelTransform;
 	XMatrix4x4					m_worldTransform;
	XMatrix4x4					m_prevWorldTransform;
	XMemBlockList<XController*>			m_preUpdateControllerArray;
	XMemBlockList<XController*>			m_postUpdateControllerArray;
	XSphere						m_boundingSphere;
	XU8							m_propertyFlags;
	X_SCENE_NODE_TYPE			m_type;
	XU8							m_viewportMask;
	
	XActorContainer*			m_pActorContainer;


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSceneNode();
	virtual ~XSceneNode();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	virtual X_SCENE_NODE_TYPE getType();
	virtual XS8* getTypeDesc();
	virtual void dumpScene(ofstream& outputFile,XU32 index);
	XMatrix4x4& getWorldTransform();
	virtual void setWorldTransform(XMatrix4x4& worldTransform);
	void getWorldPosition(XVector3D& worldPosition);
	virtual void setWorldPosition(XVector3D& worldPosition);
	XMatrix4x4& getModelTransform();
	virtual void setModelTransform(XMatrix4x4& modelTransform);
	void getModelPosition(XVector3D& modelPosition);
	virtual void setModelPosition(XVector3D& modelPosition);
	virtual void setPrevWVPTransform(XMatrix4x4& transform);
	XMatrix4x4& getPrevWorldTransform();

	XStatus enableUpdate(XBOOL enable);
	XBOOL isUpdateEnabled();
	XStatus enableRender(XBOOL enable);
	XBOOL isRenderEnabled();
	XStatus enableFrustumCull(XBOOL enable);
	XBOOL isFrustumCullEnabled();
	XStatus enableRenderBoundingSphere(XBOOL enable);
	XBOOL isRenderBoundingSphereEnabled();
	XStatus setViewportMask(XU8 mask);
	XU8 getViewportMask();
	XStatus enableViewport(XU32 viewportIndex,XBOOL enable);
	XBOOL isViewportEnabled(XU32 viewportIndex);
	virtual XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
	//virtual XStatus setMaterialHandle(XHandle materialHandle);
	//virtual XHandle getMaterialHandle();
	//virtual XHandle getVertexBufferHandle();
	//virtual XHandle getIndexBufferHandle();
	//virtual XStatus setVertexBufferHandle(XHandle vertexBufferHandle);
	//virtual XStatus setIndexBufferHandle(XHandle indexBufferHandle);
	//virtual XGeometry* getGeometry(
	XStatus attachController(XController* pController);
	XStatus detachController(XController* pController);
	XBOOL isControllerAttached(XController* pController);
	XStatus detachAllControllers();
	XStatus deallocationNotify(XController* pController);
	XStatus prepForUpdate(XF32 dt,XScene* pScene,XSceneNode* pParentNode,XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus updateTransforms(XF32 dt, XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus prepForRender(XF32 dt,XScene* pScene);
	virtual XStatus render(XRenderEvent* pRenderEvent);
	XSphere& getBoundingSphere();
	XStatus setBoundingSphere(XSphere& sphere);
	virtual XStatus traverseDepthFirst(XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
								   XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData);
	virtual void pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits);
	virtual void pickLine(XVector3D& lineStart,XVector3D& lineEnd,XMemBlockList<HitInfo>& hits);
	virtual void pickAll(XMemBlockList<XSceneNode*>& hits);
	virtual void fillLightmap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numLights,XLight** ppLights);
	virtual void fillAOMap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numRays,XVector3D* pRays);
	virtual XF32 getSurfaceArea();
	/*virtual XStatus onAddedToScene(XScene* pScene,XBOOL updateNode);
	virtual XStatus onRemovedFromScene(XScene* pScene,XBOOL updateNode);*/

	virtual XActorContainer* getActorContainer();
	virtual void setActorContainer(void* pActorContainer);
	virtual XSphere getActorContainerBoundingSphere();
};




#endif