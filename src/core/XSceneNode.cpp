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
//	Module Name:	XSceneNode.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-6-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructs a scene node by initializing the scene node properties to
the following default values:<br><br>

<ul>
<li> ID = 0 </li>
<li> Model Transform = 4x4 Identity Matrix </li>
<li> Property Flags = X_SNP_UPDATE | X_SNP_RENDER | X_SNP_FRUSTUM_CULL</li>
<li> Alpha Queue Priority = 0 </li>
<li> Render State Handle = X_INVALID_HANDLE </li>
<li> Vertex Buffer Handle = X_INVALID_HANDLE </li>
<li> Index Buffer Handle = X_INVALID_HANDLE </li>
<li> Frustum Sphere Radius = 0 </li>
</ul>
*/
//*****************************************************************************
XSceneNode::XSceneNode()
{
	m_propertyFlags = X_SNP_UPDATE | X_SNP_RENDER | X_SNP_FRUSTUM_CULL;
	m_viewportMask = 0xFF;

	m_pActorContainer = NULL;
}



//*****************************************************************************
/**
Destructor for the scene node.  
*/
//*****************************************************************************
XSceneNode::~XSceneNode()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneNode::destroy()
{
	//Notify the controllers so they can finish any tasks with the node,
	//then clear the controller array
	XMemBlockList<XController*>::iterator i(m_preUpdateControllerArray.begin());
	while (i.isValid())
	{
		(i.getValue())->deallocationNotify(this);
		i.getNext();
	}

	i.set(m_postUpdateControllerArray.begin());
	while (i.isValid())
	{
		(i.getValue())->deallocationNotify(this);
		i.getNext();
	}

	m_preUpdateControllerArray.clear();
	m_postUpdateControllerArray.clear();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XSceneNode::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_UNKNOWN,X_SNID_UNKNOWN);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the scene node.  This is the description that
appears in log files and other debug output.  Typically, your derived classes
should override this function to provide a more detailed description of
your derived scene node.  The default description for this object is
"XSceneNode".

@return A string description of the scene node.
*/
//*****************************************************************************
XS8* XSceneNode::getTypeDesc()
{
	static char description[X_TYPE_DESC_LEN] = "XSceneNode";
	return description;
}



//*****************************************************************************
/**
Enables the scene node to process an update message.  During the update pass
of X-VP's scene graph, each scene node in the graph will receive a 
notification to update itself.  By default, a scene node will process this
update request, however, if this processing has been disabled with a call to
disableUpdate(), you can re-enable it with this function.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::enableUpdate(XBOOL enable)
{
	if (enable)
		m_propertyFlags |= X_SNP_UPDATE;
	else
		m_propertyFlags &= ~X_SNP_UPDATE;
	return X_OK;
}



//*****************************************************************************
/**
Determines if the scene node is currently processing update notifications from
the engine.

@return True if the scene node is currently processing update notifications, false
		otherwise.
*/
//*****************************************************************************
XBOOL XSceneNode::isUpdateEnabled()
{
	if (m_propertyFlags & X_SNP_UPDATE)
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Enables the scene node to process a render message.  During the render pass
of the X-VP's scene graph, each scene node in the graph will receive a 
notification to render itself.  By default, a scene node will process this
render request, however, if this processing has been disabled with a call to
disableRender(), you can re-enable it with this function.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::enableRender(XBOOL enable)
{
	if (enable)
		m_propertyFlags |= X_SNP_RENDER;
	else
		m_propertyFlags &= ~X_SNP_RENDER;
	return X_OK;
}



//*****************************************************************************
/**
Determines if the scene node is currently processing render notifications from
the engine.

@return True if the scene node is currently processing render notifications, false
		otherwise.
*/
//*****************************************************************************
XBOOL XSceneNode::isRenderEnabled()
{
	if (m_propertyFlags & X_SNP_RENDER)
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Enables frustum culling on the scene node.  During the render pass
of X-VP's scene graph, each scene node in the graph will receive a 
notification to render itself.  When the scene node receives this notification,
it can cull itself against the current viewing frustum and make rendering
decisions based upon the result.  Frusum culling is not a requirement and
is left up to each specific scene node to implement.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::enableFrustumCull(XBOOL enable)
{
	if (enable)
		m_propertyFlags |= X_SNP_FRUSTUM_CULL;
	else
		m_propertyFlags &= ~X_SNP_FRUSTUM_CULL;
	return X_OK;
}



//*****************************************************************************
/**
Determines if the scene node is currently culling itself against the viewing
frustum.

@return True if the scene node is currently culling itself against the
		viewing frustum, false otherwise.
*/
//*****************************************************************************
XBOOL XSceneNode::isFrustumCullEnabled()
{
	if (m_propertyFlags & X_SNP_FRUSTUM_CULL)
		return true;
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneNode::enableRenderBoundingSphere(XBOOL enable)
{
	if (enable)
		m_propertyFlags |= X_SNP_RENDER_BOUNDING_SPHERE;
	else
		m_propertyFlags &= ~X_SNP_RENDER_BOUNDING_SPHERE;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XSceneNode::isRenderBoundingSphereEnabled()
{
	if (m_propertyFlags & X_SNP_RENDER_BOUNDING_SPHERE)
		return true;
	else
		return false;
}


XStatus XSceneNode::setViewportMask(XU8 mask)
{
	m_viewportMask = mask;
	return X_OK;
}

XU8 XSceneNode::getViewportMask()
{
	return m_viewportMask;
}
XStatus XSceneNode::enableViewport(XU32 viewportIndex,XBOOL enable)
{
	X_ASSERTM(viewportIndex <= 7,"Viewport index out of range");

	if (enable)
		m_viewportMask |= (1 << viewportIndex);
	else
		m_viewportMask &= ~(1 << viewportIndex);

	return X_OK;
}
XBOOL XSceneNode::isViewportEnabled(XU32 viewportIndex)
{
	return m_viewportMask & (1 << viewportIndex) ? true:false;
}

//*****************************************************************************
/**
Dumps a textual representation of the scene node to a text file.

@param outputFile	The text file to receive the output.  The file must be open
					prior to calling this function.
@param index	The indentation level.
@return None.
*/
//*****************************************************************************
void XSceneNode::dumpScene(ofstream& outputFile,XU32 index)
{
}



//*****************************************************************************
/**
Gets the world transform of the scene node.  This transform is
the combination of all parent transforms above this scene node including the
scene node's own local transform.  It represents the absolute orientation and
position in world space.

This transform is calculated by the scene graph.

@return A 4x4 matrix representing the scene node's world space transform.
*/
//*****************************************************************************
XMatrix4x4& XSceneNode::getWorldTransform()
{
	return m_worldTransform;
}



//*****************************************************************************
/**
Sets the world transform of the scene node.  This transform is
the combination of all parent transforms above this scene node including the
scene node's own local transform.  It represents the absolute orientation and
position in world space.

It's important to note that this transform is continually refreshed if the
object is in X-VP's scene graph.  To override the world transform for
a node in the scene graph, this function should be called after the node's
transformation is compolete, such as in a post update controller.

@param worldTransform A 4x4 matrix representing the scene node's world transform.
@return None.
*/
//*****************************************************************************
void XSceneNode::setWorldTransform(XMatrix4x4& worldTransform)
{
	//Set the world transform.
	m_worldTransform = worldTransform;
}



//*****************************************************************************
//*****************************************************************************
void XSceneNode::setWorldPosition(XVector3D& worldPosition)
{
	m_worldTransform._41 = worldPosition.x;
	m_worldTransform._42 = worldPosition.y;
	m_worldTransform._43 = worldPosition.z;
}



//*****************************************************************************
//*****************************************************************************
void XSceneNode::getWorldPosition(XVector3D& worldPosition)
{
	worldPosition.x = m_worldTransform._41;
	worldPosition.y = m_worldTransform._42;
	worldPosition.z = m_worldTransform._43;
}



//*****************************************************************************
/**
Gets the model transform of the scene node.  This transform defines how to 
transform the node into model space.  It get's post-multiplied by it's parent's
world transformation.  If the node is parentless, then model space is equal to
world space.

@return A 4x4 matrix representing the scene node's model space transform.
*/
//*****************************************************************************
XMatrix4x4& XSceneNode::getModelTransform()
{
	return m_modelTransform;
}



//*****************************************************************************
/**
Sets the model transform of the scene node.  This transform defines how to 
transform the node into model space.  It get's post-multiplied by it's parent's
world transformation.  If the node is parentless, then model space is equal to
world space.

@param modelTransform A 4x4 matrix representing the scene node's model space transform.
@return None.
*/
//*****************************************************************************
void XSceneNode::setModelTransform(XMatrix4x4& modelTransform)
{
	//Set the model space transform.
	m_modelTransform = modelTransform;
}



//*****************************************************************************
/**
Sets the position of the node relative to its parent.  This position is based
on how the node is added to the scene graph.  For example, if the node is
added to the root of the scene graph, then this position will be an absolute
world space position.  On the other hand, if this node has been added as a child
of another node, the position will represent an offset from the parent node
with respect to the parent's coordinate system.

@param position	The new relative position of the scene node.
@return None.
*/
//*****************************************************************************
void XSceneNode::setModelPosition(XVector3D& modelPosition)
{
	m_modelTransform._41 = modelPosition.x;
	m_modelTransform._42 = modelPosition.y;
	m_modelTransform._43 = modelPosition.z;
}



//*****************************************************************************
//*****************************************************************************
void XSceneNode::getModelPosition(XVector3D& modelPosition)
{
	modelPosition.x = m_modelTransform._41;
	modelPosition.y = m_modelTransform._42;
	modelPosition.z = m_modelTransform._43;
}


XMatrix4x4& XSceneNode::getPrevWorldTransform()
{
	return m_prevWorldTransform;
}



void XSceneNode::setPrevWVPTransform(XMatrix4x4& transform)
{
	m_prevWorldTransform = transform;
}



//*****************************************************************************
/**
Attaches an update controller to the scene node.  The controller will then
be notified when this scene node is being updated by the engine and will have
an opportunity to modify the node's properties.  The controller will be
notified only on the update pass of the scene graph. You may attach as many update
controllers as necessary while keeping the following things in mind.

1. Controllers are notified to update the node in the order they were attached.<br>
2. Controllers may not know about other controllers and therefore interfere
with each other.  For example, two controllers may want to modify the local
transform of a node, however since they aren't aware of each other, the second
controller will prevail by virtue of being the last controller acting on the
node.  It is possible to create special controllers that are aware of each other's
modifications to a scene node during an update.  

@param pController	The update controller to attach.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::attachController(XController* pController)
{
	if (!pController)
		return X_ERR_INVALID_PARAMS;
	else
	{
		//Are we attaching a pre update, post update or render controller?
		XMemBlockList<XController*>::iterator i;
		XMemBlockList<XController*>* pList;

		XU8 typeFlags = pController->getTypeFlags();
		if (typeFlags & X_CTRT_PRE_UPDATE)
		{
			i.set(m_preUpdateControllerArray.begin());
			pList = &m_preUpdateControllerArray;
		}
		else if (typeFlags & X_CTRT_POST_UPDATE)
		{
			i.set(m_postUpdateControllerArray.begin());
			pList = &m_postUpdateControllerArray;
		}
		else
		{
			return X_ERR_OPERATION_FAILED;
		}

		//First check to see if the controller is already attached.  If it
		//is, we don't want to attach it again.
		XBOOL alreadyAttached = false;
		while (i.isValid())
		{
			if ((i.getValue()) == pController)
			{
				alreadyAttached = true;
				break;
			}
			i.getNext();
		}
		
		if (alreadyAttached)
			return X_ERR_REDUNDANT_OPERATION;
		else
		{
			//Try to attach the controller if compatible.  If the controller tells us
			//everything went ok with the attachment, then add the
			//controller to this node.
			if (pController->isCompatible(this))
			{
				XStatus status = pController->prepForAttachment(this);
				if (status == X_OK)
					pList->insertBack(pController);
				
				return status;
			}
			else
				return X_ERR_NOT_SUPPORTED;
		}
	}
}
	


//*****************************************************************************
/**
Detaches an update controller from the scene node.  The controller will no
longer be notified to update this scene node. 

@param pController	The update controller to detach.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::detachController(XController* pController)
{
	if (!pController)
		return X_ERR_INVALID_PARAMS;
	else
	{
		//Are we detaching a pre update, post update or render controller?
		XMemBlockList<XController*>::iterator i;
		XMemBlockList<XController*>* pList;

		XU8 typeFlags = pController->getTypeFlags();
		if (typeFlags & X_CTRT_PRE_UPDATE)
		{
			i.set(m_preUpdateControllerArray.begin());
			pList = &m_preUpdateControllerArray;
		}
		else if (typeFlags & X_CTRT_POST_UPDATE)
		{
			i.set(m_postUpdateControllerArray.begin());
			pList = &m_postUpdateControllerArray;
		}
		else
		{
			return X_ERR_OPERATION_FAILED;
		}

		//First check to see if the controller was ever attached in
		//the first place.  If it wasn't, don't bother continuing.
		XBOOL attached = false;
		while (i.isValid())
		{
			if ((i.getValue()) == pController)
			{
				attached = true;
				break;
			}
			i.getNext();
		}
		
		if (!attached)
			return X_ERR_NO_SUCH_OBJECT;
		else
		{
			//Remove the controller pointer no matter what.
			pList->remove(pController);
			
			//Tell the controller we are detaching
			return pController->prepForDetachment(this);		
		}
	}
}



//*****************************************************************************
/**
Determines if a specified updated controller is attached to the scene node.

@param pController	The update controller to check for.
@return True if the controller is attached, false otherwise.
*/
//*****************************************************************************
XBOOL XSceneNode::isControllerAttached(XController* pController)
{
	if (!pController)
		return false;
	else
	{
		//Are we looking for a pre update, post update or render controller?
		XMemBlockList<XController*>::iterator i;

		XU8 typeFlags = pController->getTypeFlags();
		if (typeFlags & X_CTRT_PRE_UPDATE)
		{
			i.set(m_preUpdateControllerArray.begin());
		}
		else if (typeFlags & X_CTRT_POST_UPDATE)
		{
			i.set(m_postUpdateControllerArray.begin());
		}
		else
		{
			return false;
		}

		XBOOL attached = false;
		while (i.isValid())
		{
			if ((i.getValue()) == pController)
			{
				attached = true;
				break;
			}
			i.getNext();
		}
		
		return attached;
	}
}



//*****************************************************************************
/**
Detaches all update and render controllers from the node.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::detachAllControllers()
{
	//Tell each controller we are detaching
	XMemBlockList<XController*>::iterator i(m_preUpdateControllerArray.begin());
	while (i.isValid())
	{
		(i.getValue())->prepForDetachment(this);
		i.getNext();
	}
	i.set(m_postUpdateControllerArray.begin());
	while (i.isValid())
	{
		(i.getValue())->prepForDetachment(this);
		i.getNext();
	}

	m_preUpdateControllerArray.clear();
	m_postUpdateControllerArray.clear();
	
	return X_OK;
}



//*****************************************************************************
/**
Notifies the scene node that one of its attached controllers has been
deallocated.  This message gives the scene node a chance to do any required
clean up such as removing the controller from its attached list.

@param pController	The controller being destroyed.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::deallocationNotify(XController* pController)
{
	if (!pController)
		return X_ERR_INVALID_PARAMS;

	m_preUpdateControllerArray.remove(pController);
	m_postUpdateControllerArray.remove(pController);

	return X_OK;
}


XStatus XSceneNode::updateTransforms(XF32 dt, XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//We premultiply the model transform with the parent's transform.
	//This causes the node to inherit all motion of the parent in addition
	//to it's own.
	m_prevWorldTransform = m_worldTransform;
	m_worldTransform = m_modelTransform * (*pMatrixStack->getBack());

	//Also, we push this nodes matrix on the stack so that children may
	//access it in the same manner.
	XMatrix4x4* pMat = pMatrixStack->insertBack();
	(*pMat) = m_worldTransform;

	return X_OK;
}

//*****************************************************************************
/**
Prepares a scene node for the update pass.  This function will do all the
necessary background tasks to allow the scene node to be updated, including
notifying the attached update controllers.  Once this function has notified all
the attached update controllers, it will call update() to allow the node to
update itself.

@param dt	The amount of time, in seconds, that has elapsed since the last
					update.
@param pParentNode	The parent node of the node.  This value may be NULL.
@param pMatrixStack	The scene graph matrix stack.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::prepForUpdate(XF32 dt,XScene* pScene,XSceneNode* pParentNode,
									 XFreeArray<XMatrix4x4>* pMatrixStack)								  
{				   
	//Update only if update is allowed
	if (!(X_SNP_UPDATE & m_propertyFlags))
		return X_OK;
	
	//Spin through the currently attached controllers and give them a chance
	//to act on this node.
	XMemBlockList<XController*>::iterator i(m_preUpdateControllerArray.begin());
	while (i.isValid())
	{
		/*X_DEBUG
		(
			std::string s = "CONTROLLER INFLUENCE-> ";
			s += (i.getValue())->getTypeDesc();
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_VERBOSE);
		)*/
		if (i.getValue()->isEnabled())
			(i.getValue())->handleNodePreUpdate(pScene,this,pParentNode,pMatrixStack);
		i.getNext();
	}
	
	//Update transformation for this node
	updateTransforms(dt,pMatrixStack);

	//At this point, the node's transformation pipeline is complete and ready be passed
	//to children.  Spin through the currently attached controllers and give them a chance
	//to act on this node.  They must take on the responsibility of recalculating the pipeline
	//if they modify it.
	i.set(m_postUpdateControllerArray.begin());
	while (i.isValid())
	{
		X_DEBUG
		(
			std::string s = "CONTROLLER INFLUENCE-> ";
			s += (i.getValue())->getTypeDesc();
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_VERBOSE);
		)
		(i.getValue())->handleNodePostUpdate(pScene,this,pParentNode,pMatrixStack);
		i.getNext();
	}
	
	//Call the node's update function.
	update(dt,pScene,pMatrixStack);

	return X_OK;
}
  


//*****************************************************************************
/**
Updates a scene node.  This function is called during the update pass in the
scene graph to give the node a chance to update itself.  Typically, you will
override this function to provide custom functionality for new types of
scene nodes.

@param dt	The amount of time, in seconds, that has elapsed since the last
					update.
@param pMatrixStack	The scene graph matrix stack.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)								  
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneNode::prepForRender(XF32 dt,XScene* pScene)								  
{			

	return X_OK;
}



//*****************************************************************************
/**
Renders a scene node.  This function is called during the render pass in the
scene graph to give the node a chance to render itself.  Typically, you will
override this function to provide custom functionality for new types of
scene nodes.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSceneNode::render(XRenderEvent* pRenderEvent)								  
{
	return X_OK;
}



//*****************************************************************************
/**
Sets the scene node's frustum sphere radius.  The center of the sphere
originates from the center of the node's coordinate system.  This sphere is used
to determine whether or not the scene node is inside the viewing frustum.

@param radius	The radius of the node's frustum sphere.
*/
//*****************************************************************************
XStatus XSceneNode::setBoundingSphere(XSphere& sphere)
{
	m_boundingSphere = sphere;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XSphere& XSceneNode::getBoundingSphere()
{
	return m_boundingSphere;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneNode::attachShaderController(XS8* pMaterialName,XShaderController* pMatController)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneNode::traverseDepthFirst(XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XSceneNode::pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits)
{
	if (m_boundingSphere.radius > 0)
	{
		if (XIRaySphereQuick(pickRay.origin,pickRay.normal,m_boundingSphere.center,m_boundingSphere.radius))
		{
			HitInfo hi;
			hi.pSceneNode = this;
			getWorldPosition(hi.intersectionPoint);
			hits.insertBack(hi);
		}
	}
}

void XSceneNode::pickLine(XVector3D& lineStart,XVector3D& lineEnd,XMemBlockList<HitInfo>& hits)
{
	if (m_boundingSphere.radius > 0)
	{
		XVector3D intersectionPoint;
		XF32 time;
		if (XILineSphere(lineStart,lineEnd,m_boundingSphere.center,m_boundingSphere.radius,intersectionPoint,time))
		{
			HitInfo hi;
			hi.pSceneNode = this;
			hi.intersectionPoint = intersectionPoint;
			hits.insertBack(hi);
		}
	}
}


void XSceneNode::pickAll(XMemBlockList<XSceneNode*>& hits)
{
	if (m_boundingSphere.radius > 0)
		hits.insertBack(this);
}


void XSceneNode::fillLightmap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numLights,XLight** ppLights)
{
}

void XSceneNode::fillAOMap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numRays,XVector3D* pRays)
{
}

XF32 XSceneNode::getSurfaceArea()
{
	return 0;
}

XActorContainer* XSceneNode::getActorContainer()
{
	return m_pActorContainer;
}

void XSceneNode::setActorContainer(void* pActorContainer)
{
	XActorContainer* pAC = (XActorContainer*)pActorContainer;
	if (pAC)
	{
		pAC->m_sceneNodes.insertBack(this);
	}
	else
		m_pActorContainer->m_sceneNodes.remove(this);
	m_pActorContainer = pAC;
}


XSphere XSceneNode::getActorContainerBoundingSphere()
{
	return m_boundingSphere;
}

/*
XStatus XSceneNode::onAddedToScene(XScene* pScene,XBOOL updateNode)
{
	return X_OK;
}

XStatus XSceneNode::onRemovedFromScene(XScene* pScene,XBOOL updateNode)
{
	return X_OK;
}
*/
#if 0

//*****************************************************************************
/**
Gets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
XU32 XSceneNode::getMaterialHandle()
{
	return X_INVALID_HANDLE;
}



//*****************************************************************************
/**
Gets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
XHandle XSceneNode::getVertexBufferHandle()
{
	return X_INVALID_HANDLE;
}



//*****************************************************************************
/**
Gets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
XHandle XSceneNode::getIndexBufferHandle()
{
	return X_INVALID_HANDLE;
}



//*****************************************************************************
/**
Sets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
XStatus XSceneNode::setMaterialHandle(XHandle materialHandle)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Sets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
XStatus XSceneNode::setVertexBufferHandle(XHandle vertexBufferHandle)
{
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Sets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
XStatus XSceneNode::setIndexBufferHandle(XHandle indexBufferHandle)
{
	return X_ERR_NO_SUCH_OBJECT;
}
#endif

