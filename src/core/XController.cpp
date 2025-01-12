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
//	Module Name:	XController.cpp
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
Constructor for the controller.
*/
//*****************************************************************************
XController::XController()
{
	m_refCount = 0;
	m_enabled = true;
}



//*****************************************************************************
//*****************************************************************************
XController::~XController()
{
}



//*****************************************************************************
/**
Destructor for the controller.  If the controller is deallocated while it
is still attached to scene nodes, it will send a deallocation notification
to each of the nodes informing them that the controller is no longer available.
<br><br>

It's better practice to take care of these dependencies on your own by detaching
the controller from the nodes it's attached to, but this destructor will do the
clean up if necessary.
*/
//*****************************************************************************
XStatus XController::destroy()
{
	//Notify the client scene nodes that the controller is going out of scope.
	XMemBlockList<XSceneNode*>::iterator i(m_updateClients.begin());
	while (i.isValid())
	{
		i.getValue()->deallocationNotify(this);
		i.getNext();
	}

	m_updateClients.clear();

	i.set(m_renderClients.begin());
	while (i.isValid())
	{
		i.getValue()->deallocationNotify(this);
		i.getNext();
	}

	m_renderClients.clear();

	XVP::removeController(this);
	return X_OK;
}



//*****************************************************************************
/**
Returns the number of client scene nodes.  This is the number of scene nodes
that have this controller attached to them.

@return The number of scene nodes that have this controller attached.
*/
//*****************************************************************************
XU32 XController::getNumClients()
{
	return m_refCount;
}



//*****************************************************************************
//*****************************************************************************
XU8 XController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the controller.  This is the description that
appears in log files and other debug output.  You should override this method
to pass back a specific description for new controllers.

@return A string description of the controller.
*/
//*****************************************************************************
XS8* XController::getTypeDesc()
{
	static char description[X_TYPE_DESC_LEN] = "XController";
	return description;
}



//*****************************************************************************
//*****************************************************************************
bool XController::isCompatible(XSceneNode* pNode)
{
	return true;
}



//*****************************************************************************
/**
Notifies the controller that it is being attached to a scene node.  
It gives the controller a chance to add the scene node as a "client" that will
be requiring updates in the future.  Also, if necessary, X-VP is
notified that it now must update this controller once per frame.

@param pNode	A pointer to the scene node that this controller is being attached
				to.
@param passType	An enum indicating that this controller has been attached as either
				an update or render controller.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XController::prepForAttachment(XSceneNode* pNode)
{
	//Validate parameters
	if (!pNode)
		return X_ERR_INVALID_PARAMS;
		
	//Increment the reference count
	++m_refCount;
	
	//Only add the controller to the engine once.  If the refCount = 1 then
	//this is the first time the controller is being attached to a node,
	//therefore we add it to the engine.
	if (m_refCount == 1)
		XVP::addController(this);
	
	//Get the type flags for the derived class
	XU8 typeFlags = getTypeFlags();

	if ((typeFlags & X_CTRT_PRE_UPDATE) || (typeFlags & X_CTRT_POST_UPDATE))
	{
		m_updateClients.insertBack(pNode);
		return handleUpdateAttachment(pNode);
	}
	else if (typeFlags & X_CTRT_RENDER)
	{
		m_renderClients.insertBack(pNode);
		return handleRenderAttachment(pNode);
	}
	else 
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Notifies the controller that it is being detached from a scene node.  
It gives the controller a chance to remove the scene node as a "client" that will
be requiring updates in the future.  Also, if necessary, X-VP is notified
that it does not have to update the controller anymore.

@param pNode	A pointer to the scene node that this controller is being detached
				from.
@param passType	An enum indicating that this controller has been attached as either
				an update or render controller.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XController::prepForDetachment(XSceneNode* pNode)
{
	//Validate parameters
	if (!pNode)
		return X_ERR_INVALID_PARAMS;
	
	//Decrement the reference count.
	if (m_refCount > 0)
		--m_refCount;

	//If this controller is no longer attached to any nodes then remove from
	//xvp.
	if (m_refCount == 0)
		XVP::removeController(this);

	//Get the type flags for the derived class
	XU8 typeFlags = getTypeFlags();

	if ((typeFlags & X_CTRT_PRE_UPDATE) || (typeFlags & X_CTRT_POST_UPDATE))
	{
		m_updateClients.remove(pNode);
		return handleUpdateDetachment(pNode);
	}
	else if (typeFlags & X_CTRT_RENDER)
	{
		m_renderClients.remove(pNode);
		return handleRenderDetachment(pNode);
	}
	else 
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Responds to an update attachment event.  This type of event occurs when the
controller is attached to a scene node as an update controller.  You can override
this method for your derived controller to add custom functionality in response
to this type of event.

@param pNode	The node that this controller is being attached to.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleUpdateAttachment(XSceneNode* pNode)
{
	return X_OK;
}



//*****************************************************************************
/**
Responds to an update detachment event.  This type of event occurs when the
controller is detached from a scene node as an update controller.  You can override
this method for your derived controller to add custom functionality in response to
this type of event.

@param pNode	The node that this controller is being detached from.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleUpdateDetachment(XSceneNode* pNode)
{
	return X_OK;
}


//*****************************************************************************
/**
Responds to a render attachment event.  This type of event occurs when the
controller is attached to a scene node as a render controller.  You can override
this method for your derived controller to add custom functionality in response
to this type of event.

@param pNode	The node that this controller is being attached to.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleRenderAttachment(XSceneNode* pNode)
{
	return X_OK;
}



//*****************************************************************************
/**
Responds to a render detachment event.  This type of event occurs when the
controller is detached from a scene node as a render controller.  You can override
this method for your derived controller to add custom functionality in response to
this type of event.

@param pNode	The node that this controller is being detached from.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleRenderDetachment(XSceneNode* pNode)
{
	return X_OK;
}



//*****************************************************************************
/**
Notifies the controller that a scene node is being deallocated.  This method
is called by a scene node when it is being deallocated and still has attached
controllers.  The notified controller will then remove the scene node from its
client list.

It's better practice to take care of these dependencies on your own by detaching
controllers manually, but this method will do the clean up if necessary.

@param pNode	Pointer to the scene node that is being deallocated.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XController::deallocationNotify(XSceneNode* pNode)
{
	//Validate parameters
	if (!pNode)
		return X_ERR_INVALID_PARAMS;

	if (m_refCount > 0)
		--m_refCount;
	
	//If this controller is no longer attached to any nodes then remove it
	//from xvp
	if (m_refCount == 0)
		XVP::removeController(this);
	
	m_updateClients.remove(pNode);
	m_renderClients.remove(pNode);

	return X_OK;
}

XBOOL XController::isEnabled()
{
	return m_enabled;
}

XStatus XController::enable(XBOOL enable)
{
	m_enabled = enable;
	return X_OK;
}

//*****************************************************************************
/**
Updates a scene node during the update pass of the scene graph.  This method
is called during the update pass of the scene graph by a scene
node that has this particular controller attached.  When this is called, the
controller has the chance to influence the scene node. 

You should override this method to implement custom functionality for new
controllers.

@param pNode	A pointer to the scene node requesting an update.  
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.  This value may be NULL.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
								XFreeArray<XMatrix4x4>* pMatrixStack)
{
	return X_OK;
}



//*****************************************************************************
/**
Updates a scene node during the update pass of the scene graph.  This method
is called during the update pass of the scene graph by a scene
node that has this particular controller attached.  When this is called, the
controller has the chance to influence the scene node. 

You should override this method to implement custom functionality for new
controllers.

@param pNode	A pointer to the scene node requesting an update.  
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.  This value may be NULL.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleNodePostUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
								XFreeArray<XMatrix4x4>* pMatrixStack)
{
	return X_OK;
}



//*****************************************************************************
/**
Updates a scene node during the render pass of the scene graph.  This method
is called during the render pass of the scene graph by a scene
node that has this particular controller attached.  When this is called, the
controller has the chance to influence the scene node. 

You should override this method to implement custom functionality for new
controllers.

@param pNode	A pointer to the scene node requesting an update.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XController::handleNodeRender(XScene* pScene,XSceneNode* pNode)
{
	return X_OK;
}



//*****************************************************************************
/**
Updates time dependent data associated with the controller.  X-VP calls this
method if the controller is attached and the node its attached to is in
the scene graph.  You should override this method to implement custom
functionality for new controllers.

@param dt The amount of time, in seconds, that has elapsed since the
				 last update.
@return None.
*/
//*****************************************************************************
XStatus XController::update(XF32 dt)
{
	return X_OK;
}
