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
//	Module Name:	XModel.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-22-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>
typedef ULONG ulong;
typedef UINT uint;
typedef USHORT ushort;
#include <cimg.h>
using namespace cimg_library;



//*****************************************************************************
/**
Constructor for the model.  
*/
//*****************************************************************************
XModel::XModel()
{
	m_pDataTemplate = NULL;
	m_pNodeArray = NULL;
	m_numNodes = 0;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the model.
*/
//*****************************************************************************
XModel::~XModel()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XModel::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL,X_SNID_MODEL);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the model.  This is the
description that appears in log files and other debug output.

@return A string description of the model.
*/
//*****************************************************************************
XS8* XModel::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XModel";
	return description; 
}



//*****************************************************************************
/**
Dumps a textual representation of the model to a text file.

@param outputFile	The text file to receive the output.  The file must be open
					calling this function.
@param index	The indentation level.
@return None.
*/
//*****************************************************************************
void XModel::dumpScene(ofstream& outputFile,XU32 index)
{
	m_nodeTree.dumpScene(outputFile,index);
}


 
//*****************************************************************************
//*****************************************************************************
XStatus XModel::init()
{
	m_inited = true;
	return X_OK;
}



//*****************************************************************************
/**
Destroys the model and its associated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::destroy()
{
	if (!m_inited)
		return X_OK;

	//Destroy the objects
	XMemBlockList<XSkinObject*>::iterator i(m_skins.begin());
	while (i.isValid())
	{
		XDelete(i.getValue());
		i.getNext();
	}
	m_skins.clear();

	for (XU32 i = 0;i < m_numNodes;i++)
	{
		m_pNodeArray[i]->destroy();
		XDelete(m_pNodeArray[i]);
	}
	XDeleteArray(m_pNodeArray)
	m_nodeTree.destroy();

	m_numNodes = 0;
	XSceneNode::destroy();
	m_inited = false;

	return X_OK;
}



//*****************************************************************************
/**
Updates the model.  This function is called to allow the model to update any
time dependent data.  Normally, this function will be called by X-VP
at the appropriate times, so there is no need for you to call it manually.

@param	dt	The time, in seconds, that has passed since the last update.
@param	pMatrixStack	The scene graph's matrix stack.
@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{	
	//Update the model object tree.
	m_nodeTree.update(dt,pScene,pMatrixStack);

	XMemBlockList<XSkinObject*>::iterator i(m_skins.begin());
	while(i.isValid())
	{
		i.getValue()->update(dt,pScene,pMatrixStack);
		i.getNext();
	}

	return X_OK;
}

XStatus XModel::prepForRender(XF32 dt,XScene* pScene)
{
	//X_DEBUG_LOGF("----------prepForRender %d\n",0,m_numNodes);
	for (XU32 i = 0;i < m_numNodes;++i)
		m_pNodeArray[i]->prepForRender(dt,pScene);

	XMemBlockList<XSkinObject*>::iterator i(m_skins.begin());
	while(i.isValid())
	{
		i.getValue()->prepForRender(dt,pScene);
		i.getNext();
	}
	return X_OK;
}

//*****************************************************************************
/**
Attaches an update controller to a subsection of the model.  Since an 
XModel object is made up of XModelObject objects which are derived from
XSceneNode, the controller can be attached to the model itself (model space),
or any combination of the individual nodes that make up the model.  

To learn more about attaching controllers to XSceneNode objects see XSceneNode,
and XController.

@param pNodeName	The name of the node to attach the controller to.
					Nodes are usually named at design time in a content
					generation package such as 3D Studio Max.  If this parameter
					is NULL, the controller will be attached to the model itself
					(model space).
@param pController	The controller to attach.
@param recursive	If true, the controller will be attached to the specified node
					plus all children.  If false, the default, the controller
					will be attached only to the specified node.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::attachController(XS8* pNodeName,XController* pController,
										 XBOOL recursive)
{
	//Make sure the controller is valid.
	if (!pController)
		return X_ERR_INVALID_PARAMS;

	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->attachController(pController);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->attachController(pController);
			i.getNext();
		}
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->attachController(pController);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
/**
Detaches an update controller from a subsection of the model.  The controller
can be detached from the model itself (model space), or any combination of the
individual nodes that make up the model.  

To learn more about detaching controllers from XSceneNode objects see XSceneNode,
and XController.

@param pNodeName	The name of the node to detach the controller from.
					Nodes are usually named at design time in a content
					generation package such as 3D Studio Max.  If this parameter
					is NULL, the controller will be detached from the model itself
					(model space).
@param pController	The controller to detach.
@param recursive	If true, the controller will be detached from the specified node
					plus all children.  If false, the default, the controller
					will be detached only from the specified node.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::detachController(XS8* pNodeName,XController* pController,
									 	 XBOOL recursive)
{
	//Make sure the controller is valid.
	if (!pController)
		return X_ERR_INVALID_PARAMS;

	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->detachController(pController);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->detachController(pController);
			i.getNext();
		}
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->detachController(pController);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
/**
Determines is a specific update controller is attached to a model object.

@param pNodeName	The name of the node to query.
					Nodes are usually named at design time in a content
					generation package such as 3D Studio Max.  If this parameter
					is NULL, the model space will be queried.
@param pController	The update controller to check for.
@return True if the controller is attached, false otherwise.
*/
//*****************************************************************************
XBOOL XModel::isControllerAttached(XS8* pNodeName,XController* pController)
{
	//Make sure the controller is valid.
	if (!pController)
		return false;

	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		return ((XSceneNode*)this)->isControllerAttached(pController);
	else 
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			return pNode->m_pSceneNode->isControllerAttached(pController);
		}
		else
			return false;	
	}	
}


//*****************************************************************************
//*****************************************************************************
XStatus XModel::attachShaderController(XS8* pMaterialName,
										 XShaderController* pMatController)
{
	for (XU32 i = 0;i < m_numNodes;++i)
		m_pNodeArray[i]->attachShaderController(pMaterialName,pMatController);
	
	XMemBlockList<XSkinObject*>::iterator i(m_skins.begin());
	while (i.isValid())
	{
		i.getValue()->attachShaderController(pMaterialName,pMatController);
		i.getNext();
	}

	return X_OK;
}



#if 0						 
										 
//*****************************************************************************
/**
Determines is a specific update controller is attached to a model object.

@param pNodeName	The name of the node to query.
					Nodes are usually named at design time in a content
					generation package such as 3D Studio Max.  If this parameter
					is NULL, the model space will be queried.
@param pController	The update controller to check for.
@return True if the controller is attached, false otherwise.
*/
//*****************************************************************************
XBOOL XModel::isPostUpdateControllerAttached(XS8* pNodeName,XController* pController)
{
	//Make sure the controller is valid.
	if (!pController)
		return false;

	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		return ((XSceneNode*)this)->isPostUpdateControllerAttached(pController);
	else 
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			return pNode->m_pSceneNode->isPostUpdateControllerAttached(pController);
		}
		else
			return false;	
	}	
}



//*****************************************************************************
/**
Determines is a specific render controller is attached to a model object.

@param pNodeName	The name of the node to query.
					Nodes are usually named at design time in a content
					generation package such as 3D Studio Max.  If this parameter
					is NULL, model space will be checked.
@param pController	The render controller to check for.
@return True if the controller is attached, false otherwise.
*/
//*****************************************************************************
XBOOL XModel::isRenderControllerAttached(XS8* pNodeName,XController* pController)
{
	//Make sure the controller is valid.
	if (!pController)
		return false;

	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		return ((XSceneNode*)this)->isRenderControllerAttached(pController);
	else 
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			return pNode->m_pSceneNode->isRenderControllerAttached(pController);
		}
		else
			return false;	
	}	
}

#endif

//*****************************************************************************
/**
Detaches all update and render controllers from a subsection of the model.  The
controllers can be detached from the model itself (model space), or any combination
of the individual nodes that make up the model.  

To learn more about detaching controllers from XSceneNode objects see XSceneNode,
and XController.

@param pNodeName	The name of the node to detach the controllers from.
					Nodes are usually named at design time in a content
					generation package such as 3D Studio Max.  If this parameter
					is NULL, the controllers will be detached from the model itself
					(model space).
@param recursive	If true, the controllers will be detached from the specified node
					plus all children.  If false, the default, the controllers
					will be detached only from the specified node.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::detachAllControllers(XS8* pNodeName,XBOOL recursive)
{
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->detachAllControllers();
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->detachAllControllers();
			i.getNext();
		} 
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->detachAllControllers();
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
/**
Attaches a scene node to the model.  Nodes can be attached to the model
itself (model space), or to a specific node within the model.  Once the node
has been attached, it will inherit the transforms of its parent objects.

Its important to note that if the model already exists in the scene graph,
any attached nodes will also be seen by the scene graph, therefore, they
will start receiving update and render notifications along with the model.
If the model is removed from the scene graph, all attached nodes are removed as
well.  Take care not to add attached nodes to the scene graph or multiple
entries will occur.

@param pParentNodeName	The name of the node within the model to attach to.  This
						value cannot be NULL.
@param pNodeName The name to give the attached node.  This value cannot be NULL.
@param pNode The new node to attach.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::attachNode(XS8* pParentNodeName,XS8* pNodeName,XSceneNode* pNode)
{
	X_ASSERTM(pNode,"Invalid node pointer")
	X_ASSERTM(pNodeName,"Invalid node pointer")

	if (!m_nodeTree.createNode(pNode,pParentNodeName,pNodeName))
		return X_ERR_OPERATION_FAILED;	

	return X_OK;
}



//*****************************************************************************
/**
Detaches a scene node from the model.  Nodes can be detached from the model
itself (model space), or from a specific node within the model.  Once the node
has been detached, it will no longer inherit the transforms of its parent objects.
Also, it will disappear from the scene unless the node is specifically
added.

@param pNodeName The name of the node to detach.  This value cannot be NULL.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::detachNode(XS8* pNodeName)
{
	if (!pNodeName)
		return X_ERR_INVALID_PARAMS;

	m_nodeTree.destroyNode(pNodeName);	

	return X_OK;
}	



//*****************************************************************************
/**
Determines if a specific node is attached.

@param pNodeName	The name of the node to query for.
@return True if the node exists in the model, false otherwise.
*/
//*****************************************************************************
XBOOL XModel::isNodeAttached(XS8* pNodeName)
{
	if (!pNodeName)
		return false;

	if (m_nodeTree.getNode(pNodeName))
		return true;
	else
		return false;
}	



//*****************************************************************************
/**
Returns a specific node in the model. 

@param pNodeName	The name of the node that should be returned.  If this value
					is NULL, the root node is returned.
@return The requested node by name.  If the node requested cannot be found, this
		function will return NULL.
*/
//*****************************************************************************
XSceneNode* XModel::getNode(XS8* pNodeName)
{
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode;
	else
		return NULL;
}



//*****************************************************************************
/**
Enables a subsection of the model to process an update message.  By default,
the entire model and its nodes will process update messages.  This
function can operate on the model itself (model space), or any combination
of the individual nodes within the model.

@param pNodeName	The name of the node to operate on. Nodes are usually
					named at design time in a content generation package
					such as 3D Studio Max.  If this parameter is NULL, the
					model itself (model space) will be operated on.
@param recursive	If true, the specified node plus all of its children
					will be operated on.  If false, the default, only the
					specified node will be operated on.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::enableUpdate(XBOOL enable,XS8* pNodeName,XBOOL recursive)
{	
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->enableUpdate(enable);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->enableUpdate(enable);
			i.getNext();
		}
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->enableUpdate(enable);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
/**
Determines if a subsection of the model is processing update messages.

@param pNodeName	The name of the node to query. Nodes are usually
					named at design time in a content generation package
					such as 3D Studio Max.  If this parameter is NULL, the
					model itself (model space) will be queried.
@return True if the specified node is processing update messages, false
		otherwise.
*/
//*****************************************************************************
XBOOL XModel::isUpdateEnabled(XS8* pNodeName)
{	
	//Make sure that we have a valid pNodeName.
	if (!pNodeName)
		return ((XSceneNode*)this)->isUpdateEnabled();
	
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode->isUpdateEnabled();
	else
		return false;	
}



//*****************************************************************************
/**
Enables a subsection of the model to process a render message.  By default,
the entire model and its nodes will process render messages.  This
function can operate on the model itself (model space), or any combination
of the individual nodes within the model.

@param pNodeName	The name of the node to operate on. Nodes are usually
					named at design time in a content generation package
					such as 3D Studio Max.  If this parameter is NULL, the
					model itself (model space) will be operated on.
@param recursive	If true, the specified node plus all of its children
					will be operated on.  If false, the default, only the
					specified node will be operated on.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::enableRender(XBOOL enable,XS8* pNodeName,XBOOL recursive)
{	
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->enableRender(enable);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->enableRender(enable);
			i.getNext();
		}
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->enableRender(enable);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
/**
Determines if a subsection of the model is processing render messages.

@param pNodeName	The name of the node to query. Nodes are usually
					named at design time in a content generation package
					such as 3D Studio Max.  If this parameter is NULL, the
					model itself (model space) will be queried.
@return True if the specified node is processing render messages, false
		otherwise.
*/
//*****************************************************************************
XBOOL XModel::isRenderEnabled(XS8* pNodeName)
{	
	//Make sure that we have a valid pNodeName.
	if (!pNodeName)
		return ((XSceneNode*)this)->isRenderEnabled();
	
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode->isRenderEnabled();
	else
		return false;	
}




//*****************************************************************************
/**
Enables a subsection of the model to frustum cull itself.  By default,
the entire model and its nodes will perform frustum culling.  This
function can operate on the model itself (model space), or any combination
of the individual nodes within the model.

@param pNodeName	The name of the node to operate on. Nodes are usually
					named at design time in a content generation package
					such as 3D Studio Max.  If this parameter is NULL, the
					model itself (model space) will be operated on.
@param recursive	If true, the specified node plus all of its children
					will be operated on.  If false, the default, only the
					specified node will be operated on.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::enableFrustumCull(XBOOL enable,XS8* pNodeName,XBOOL recursive)
{	
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->enableFrustumCull(enable);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->enableFrustumCull(enable);
			i.getNext();
		}  
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->enableFrustumCull(enable);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
/**
Determines if a subsection of the model is performing frustum culling.

@param pNodeName	The name of the node to query. Nodes are usually
					named at design time in a content generation package
					such as 3D Studio Max.  If this parameter is NULL, the
					model itself (model space) will be queried.
@return True if the specified node is performing frustum culling, false
		otherwise.
*/
//*****************************************************************************
XBOOL XModel::isFrustumCullEnabled(XS8* pNodeName)
{	
	//Make sure that we have a valid pNodeName.
	if (!pNodeName)
		return ((XSceneNode*)this)->isFrustumCullEnabled();
	
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode->isFrustumCullEnabled();
	else
		return false;	
}


	
//*****************************************************************************
//*****************************************************************************
XStatus XModel::enableRenderBoundingSphere(XBOOL enable,XS8* pNodeName,XBOOL recursive)
{	
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->enableRenderBoundingSphere(enable);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->enableRenderBoundingSphere(enable);
			i.getNext();
		}  
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->enableRenderBoundingSphere(enable);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModel::isRenderBoundingSphereEnabled(XS8* pNodeName)
{	
	//Make sure that we have a valid pNodeName.
	if (!pNodeName)
		return ((XSceneNode*)this)->isRenderBoundingSphereEnabled();
	
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode->isRenderBoundingSphereEnabled();
	else
		return false;	
}

XStatus XModel::setViewportMask(XU8 mask,XS8* pNodeName,XBOOL recursive)
{
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->setViewportMask(mask);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->setViewportMask(mask);
			i.getNext();
		}  
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->setViewportMask(mask);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}

XU8 XModel::getViewportMask(XS8* pNodeName)
{
	//Make sure that we have a valid pNodeName.
	if (!pNodeName)
		return ((XSceneNode*)this)->getViewportMask();
	
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode->getViewportMask();
	else
		return 0x0;	
}

XStatus XModel::enableViewport(XU32 viewportIndex,XBOOL enable,XS8* pNodeName,XBOOL recursive)
{
	//if the node name is NULL, then apply to this node...
	if (!pNodeName)
		((XSceneNode*)this)->enableViewport(viewportIndex,enable);
	
	//If recursive flag is set...
	if (recursive)
	{
		//Get a list of the scene nodes from the pNodeName down and apply
		//the changes.
		XMemBlockList<XSceneGraphNode*> nodeList;
		m_nodeTree.getNodes(pNodeName,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			i.getValue()->m_pSceneNode->enableViewport(viewportIndex,enable);
			i.getNext();
		}  
		nodeList.clear();

		return X_OK;
	} 
	else if (pNodeName)
	{
		XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
		if (pNode)
		{
			pNode->m_pSceneNode->enableViewport(viewportIndex,enable);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;	
	}	
	else
		return X_OK;
}
XBOOL XModel::isViewportEnabled(XU32 viewportIndex,XS8* pNodeName)
{
	//Make sure that we have a valid pNodeName.
	if (!pNodeName)
		return ((XSceneNode*)this)->isViewportEnabled(viewportIndex);
	
	XSceneGraphNode* pNode = m_nodeTree.getNode(pNodeName);
	if (pNode)
		return pNode->m_pSceneNode->isViewportEnabled(viewportIndex);
	else
		return false;	
}

//*****************************************************************************
/**
Walks all of the nodes in them model in a depth first traversal, calling
the user specified callback for each node visited.

@param pStartNodeName	The name of the node to start walking from.
@param callback	The user callback to call for each visited node.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModel::walkNodes(XS8* pStartNodeName,void(*callback)(XSceneNode* pNode))
{
	//Make sure callback is valid
	if (!callback)
		return X_ERR_INVALID_PARAMS;

	//if the node name is NULL, then call back for this node
	if (!pStartNodeName)
		callback(((XSceneNode*)this));
	
	//Get a list of the scene nodes from the pStartNodeName down 
	//call the callback
	XMemBlockList<XSceneGraphNode*> nodeList;
	m_nodeTree.getNodes(pStartNodeName,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		callback(i.getValue()->m_pSceneNode);
		i.getNext();
	}  

	return X_OK;
}


XBOOL XModel::containsNode(XSceneNode* pSceneNode)
{
	for (XU32 i = 0;i < m_numNodes;++i)
	{
		if (pSceneNode == m_pNodeArray[i])
			return true;
	}

	return false;
}


XHandle XModel::getMaterial(XS8* pMaterialName)
{
	X_ASSERTM(pMaterialName,"Material name cannot be null");

	vector<XGeometry*> geomList;
	for (XU32 i = 0;i < m_numNodes;++i)
	{
		if (dynamic_cast<XModelObject*>(m_pNodeArray[i]))
		{
			XModelObject* pMo = dynamic_cast<XModelObject*>(m_pNodeArray[i]);
			XModelObjectTemplate* pMot = (XModelObjectTemplate*)pMo->m_pDataTemplate;
			X_ASSERTM(pMo->m_ppInstanceMats,"No instance materials exists in this model.  The model must be generated with instance materials before you can access them.  Otherwise, use the materials in the model template")
			
			for (XU32 j = 0;j < pMot->getNumLODs();++j)
			{
				pMot->getGeometryList(j,geomList);
				for (XU32 k = 0;k < geomList.size();++k)
				{
					if (strcmp(XVP::GraphicsMgr.getMaterialName(pMo->m_ppInstanceMats[j][k]),pMaterialName) == 0)
						return pMo->m_ppInstanceMats[j][k];
				}
			}
		}
	}
	return X_INVALID_HANDLE;
}


XStatus XModel::setPose(XPose* pPose)
{
	m_pPose = pPose;
	
	XFreeArray<XMatrix4x4>* pStack = NULL;
	XNew(pStack,XFreeArray<XMatrix4x4>,XOSMgr::getDefaultHeap());
	pStack->init(64,NULL,XOSMgr::getDefaultHeap());
	pStack->insertBack();
	m_nodeTree.walkNodes(NULL,onLevelDown,onLevelUp,pStack);
	pStack->destroy();
	XDelete(pStack);
#pragma message("I'm allocating memory when setPose() is called and I don't like it!")

	return X_OK;
}



XPose* XModel::getPose() 
{
	return m_pPose;
}


XSceneGraphNode* XModel::onLevelDown(XSceneGraphNode* pNode,void* pUserData)
{
	pNode->m_pSceneNode->updateTransforms(0,(XFreeArray<XMatrix4x4>*)pUserData);

	return pNode;
}

XSceneGraphNode* XModel::onLevelUp(XSceneGraphNode* pNode,void* pUserData)
{
	((XFreeArray<XMatrix4x4>*)pUserData)->removeBack();
	return pNode;
}

/*
XStatus XModel::drawSkeleton(XScene* pScene)
{	
	m_nodeTree.walkNodes(NULL,onLevelDown,onLevelUp,pScene);
	return X_OK;
}

XSceneGraphNode* XModel::onLevelDownDrawSkeleton(XSceneGraphNode* pNode,void* pUserData)
{
	XBone* pParentBone = dynamic_cast<XBone*>(pNode->m_pParentNode);
	XBone* pChildBone = dynamic_cast<XBone*>(pNode->m_pSceneNode);
	if (pParentBone && pChildBone)
	{
		X_DUL
		(
			XDUL::drawLine((XScene*)pUserData,XVector3D(m_worldTransform.row4),1.0f,XColor(1,1,1,1));
		)
	}
	//pNode->m_pSceneNode->updateTransforms(0,(XFreeArray<XMatrix4x4>*)pUserData);

	return pNode;
}
*/


//*****************************************************************************
//*****************************************************************************
XStatus XModel::traverseDepthFirst(XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData)
{
	m_nodeTree.traverseDepthFirst(onLevelDown,onLevelUp,pUserData);
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
void XModel::pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits)
{	
	//Get a list of the scene nodes
	XMemBlockList<XSceneGraphNode*> nodeList;
	m_nodeTree.getNodes(NULL,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		i.getValue()->m_pSceneNode->pick(pickRay,hits);
		i.getNext();
	}  
	nodeList.clear();

	//Pick skins
	XMemBlockList<XSkinObject*>::iterator i2(m_skins.begin());
	while (i2.isValid())
	{
		i2.getValue()->pick(pickRay,hits);
		i2.getNext();
	}
}

//*****************************************************************************
//*****************************************************************************
void XModel::pickLine(XVector3D& lineStart,XVector3D& lineEnd,XMemBlockList<HitInfo>& hits)
{	
	//Get a list of the scene nodes
	XMemBlockList<XSceneGraphNode*> nodeList;
	m_nodeTree.getNodes(NULL,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		i.getValue()->m_pSceneNode->pickLine(lineStart,lineEnd,hits);
		i.getNext();
	}  
	nodeList.clear();

	//Pick skins
	XMemBlockList<XSkinObject*>::iterator i2(m_skins.begin());
	while (i2.isValid())
	{
		i2.getValue()->pickLine(lineStart,lineEnd,hits);
		i2.getNext();
	}
}

void XModel::pickAll(XMemBlockList<XSceneNode*>& hits)
{	
	//Get a list of the scene nodes
	XMemBlockList<XSceneGraphNode*> nodeList;
	m_nodeTree.getNodes(NULL,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		if (i.getValue()->m_pSceneNode->getBoundingSphere().radius > 0)
			hits.insertBack(i.getValue()->m_pSceneNode);
		i.getNext();
	}  
	nodeList.clear();

	//Pick skins
	XMemBlockList<XSkinObject*>::iterator i2(m_skins.begin());
	while (i2.isValid())
	{
		if (i2.getValue()->getBoundingSphere().radius > 0)
			hits.insertBack(i2.getValue());
		i2.getNext();
	}
}



//*****************************************************************************
//*****************************************************************************
XF32 XModel::getSurfaceArea()
{	
	//Get a list of the scene nodes
	XMemBlockList<XSceneGraphNode*> nodeList;
	m_nodeTree.getNodes(NULL,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	XF32 surfaceArea = 0;
	while (i.isValid())
	{
		surfaceArea += i.getValue()->m_pSceneNode->getSurfaceArea();
		i.getNext();
	}  
	nodeList.clear();

	//skins
	XMemBlockList<XSkinObject*>::iterator i2(m_skins.begin());
	while (i2.isValid())
	{
		surfaceArea += i2.getValue()->getSurfaceArea();
		i2.getNext();
	}

	return surfaceArea;
}


XSphere XModel::getActorContainerBoundingSphere()
{
	//We need to build a sphere that encompasses all of the model objects
	XSphere boundingSphere;

	XMemBlockList<XSceneGraphNode*> nodeList;
	m_nodeTree.getNodes(NULL,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		XISphereEnclosingSpheres(boundingSphere,i.getValue()->m_pSceneNode->getBoundingSphere(),boundingSphere);
		i.getNext();
	}  
	nodeList.clear();

	XMemBlockList<XSkinObject*>::iterator i2(m_skins.begin());
	while (i2.isValid())
	{
		XISphereEnclosingSpheres(boundingSphere,i2.getValue()->getBoundingSphere(),boundingSphere);
		i2.getNext();
	}

	return boundingSphere;
}

void fillSeams(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask)
{
	int i, j;
	int x, y;
	int l, m;
	for (i=0; i< (int)lm.width; ++i)
	{
		for (j=0; j<(int)lm.height; ++j)
		{
			//Unfilled pixel found
			if (mask(i, j, 0, 0) == 0)
			{
				XBOOL filled = false;
				for (x=-1; x<=1; ++x)
				{
					for (y=-1; y<=1; ++y)
					{
						if (x==0 && y==0)
							continue;
						l = i + x;
						m = j + y;
						if (l < 0 || l >= (int)lm.width)
							continue;
						if (m < 0 || m >= (int)lm.height)
							continue;
						
						if (mask(l, m, 0, 0) == 1)
						{
							lm(i, j,0,0) = lm(l, m,0,0);
							lm(i, j,0,1) = lm(l, m,0,1);
							lm(i, j,0,2) = lm(l, m,0,2);
							filled = true;
							break;
						}
					}
				}
				if (!filled)
				{
					for (x=-2; x<=2; ++x)
					{
						for (y=-2; y<=2; ++y)
						{
							if (x==0 && y==0)
								continue;
							l = i + x;
							m = j + y;
							if (l < 0 || l >= (int)lm.width)
								continue;
							if (m < 0 || m >= (int)lm.height)
								continue;
							
							if (mask(l, m, 0, 0) == 1)
							{
								lm(i, j,0,0) = lm(l, m,0,0);
								lm(i, j,0,1) = lm(l, m,0,1);
								lm(i, j,0,2) = lm(l, m,0,2);
								filled = true;
								break;
							}
						}
					}
				}
				if (!filled)
				{
					for (x=-3; x<=3; ++x)
					{
						for (y=-3; y<=3; ++y)
						{
							if (x==0 && y==0)
								continue;
							l = i + x;
							m = j + y;
							if (l < 0 || l >= (int)lm.width)
								continue;
							if (m < 0 || m >= (int)lm.height)
								continue;
							
							if (mask(l, m, 0, 0) == 1)
							{
								lm(i, j,0,0) = lm(l, m,0,0);
								lm(i, j,0,1) = lm(l, m,0,1);
								lm(i, j,0,2) = lm(l, m,0,2);
								filled = true;
								break;
							}
						}
					}
				}
				if (!filled)
				{
					for (x=-4; x<=4; ++x)
					{
						for (y=-4; y<=4; ++y)
						{
							if (x==0 && y==0)
								continue;
							l = i + x;
							m = j + y;
							if (l < 0 || l >= (int)lm.width)
								continue;
							if (m < 0 || m >= (int)lm.height)
								continue;
							
							if (mask(l, m, 0, 0) == 1)
							{
								lm(i, j,0,0) = lm(l, m,0,0);
								lm(i, j,0,1) = lm(l, m,0,1);
								lm(i, j,0,2) = lm(l, m,0,2);
								filled = true;
								break;
							}
						}
					}
				}
				if (!filled)
				{
					for (x=-5; x<=5; ++x)
					{
						for (y=-5; y<=5; ++y)
						{
							if (x==0 && y==0)
								continue;
							l = i + x;
							m = j + y;
							if (l < 0 || l >= (int)lm.width)
								continue;
							if (m < 0 || m >= (int)lm.height)
								continue;
							
							if (mask(l, m, 0, 0) == 1)
							{
								lm(i, j,0,0) = lm(l, m,0,0);
								lm(i, j,0,1) = lm(l, m,0,1);
								lm(i, j,0,2) = lm(l, m,0,2);
								filled = true;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void XModel::generateLightmap(XU32 numModels,XModel** ppModels,XU32 numLights,XLight** ppLights,XS8* pLMFilename,XU32 texSize,XF32 blurSize)
{
	X_ASSERT(ppModels)
	X_ASSERT(ppLights)
	X_ASSERT(texSize > 0)
	X_ASSERT(pLMFilename)

	if (blurSize > 10)
		blurSize = 10;

	cimg_library::CImg<unsigned char> lm(texSize,texSize,1,3);
	cimg_library::CImg<unsigned char> mask(texSize,texSize,1,1);
	lm.fill(0);
	mask.fill(0);

	XMemBlockList<XSceneNode*> occluderList;
	for (XU32 i = 0;i < numModels;++i)
		ppModels[i]->pickAll(occluderList);
	
	XMemBlockList<XSceneNode*> nodeList;
	pickAll(nodeList);

	XMemBlockList<XSceneNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		i.getValue()->fillLightmap(lm,mask,occluderList,numLights,ppLights);
		i.getNext();
	}
	nodeList.clear();
	occluderList.clear();

	fillSeams(lm,mask);

	lm.blur(blurSize);
	lm.save(pLMFilename);
	lm.display(pLMFilename);
	mask.display("MASK");

}

void XModel::generateAOMap(XU32 numModels,XModel** ppModels,XU32 numRays,XVector3D* pRays,XS8* pAOMapFilename,XU32 texSize,XF32 blurSize)
{
	X_ASSERT(ppModels)
	X_ASSERT(texSize > 0)
	X_ASSERT(pAOMapFilename)

	if (blurSize > 10)
		blurSize = 10;

	cimg_library::CImg<unsigned char> lm(texSize,texSize,1,3);
	cimg_library::CImg<unsigned char> mask(texSize,texSize,1,1);
	lm.fill(1);
	mask.fill(0);

	XMemBlockList<XSceneNode*> occluderList;
	for (XU32 i = 0;i < numModels;++i)
		ppModels[i]->pickAll(occluderList);
	
	XMemBlockList<XSceneNode*> nodeList;
	pickAll(nodeList);

	XMemBlockList<XSceneNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		i.getValue()->fillAOMap(lm,mask,occluderList,numRays,pRays);
		i.getNext();
	}
	nodeList.clear();
	occluderList.clear();

	fillSeams(lm,mask);

	
	lm.blur(blurSize);
	lm.save(pAOMapFilename);
	lm.display(pAOMapFilename);
	mask.display("MASK");

}