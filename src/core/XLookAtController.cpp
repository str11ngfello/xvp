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
//	Module Name:	XLookAtController.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-3-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the look at controller.
*/
//*****************************************************************************
XLookAtController::XLookAtController()
{
	m_pTarget = NULL;
}



//*****************************************************************************
/**
Destructor for the look at controller.
*/
//*****************************************************************************
XLookAtController::~XLookAtController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XLookAtController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_POST_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the look at controller.  This is the
description that appears in log files and other debug output.

@return A string description of the look at controller.
*/
//*****************************************************************************
XS8* XLookAtController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XLookAtController";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
bool XLookAtController::isCompatible(XSceneNode* pNode)
{
	if (pNode)
	{
		X_SCENE_NODE_TYPE snt = pNode->getType();
		return (X_SNT_GET_BASE_SNID(snt) == X_SNID_MODEL_OBJECT) ||
			   (X_SNT_GET_EXT_SNID(snt) == X_SNID_CAMERA);
			
			/*(snt == X_SNT_MODEL_OBJECT) || 
				(snt == X_SNT_BONE) ||
				(snt == X_SNT_CAMERA_OBJECT) ||
				(snt == X_SNT_PARTICLE_EMITTER));*/
	}
	else
		return false;
}



//*****************************************************************************
/**
Sets the target.  The target is the coordinate in world space where the scene
node will be forced to look at. 

@param pTarget	A pointer to an XVector3D object.  Notice that this parameter
				is a pointer.  Once set, the look at controller will continue
				to poll this pointer for the look at target.  You will need to
				ensure that this pointer stays valid for the length of time
				the controller is using it.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XLookAtController::setTarget(XVector3D* pTarget)
{
	if (!pTarget)
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_pTarget = pTarget;
		return X_OK;
	}
}



//*****************************************************************************
/**
Removes the current target.

@return A return code of type XStatus.
*/
//*****************************************************************************
void XLookAtController::removeTarget()
{
	m_pTarget = NULL;
}



//*****************************************************************************
/**
Updates a scene node during the update pass of the scene graph.  This function
is called during the update pass of the scene graph by a scene node that has
this look at controller attached.  

Builds a look at matrix to force the scene node to face the target.

@param pNode	A pointer to the scene node requesting an update.
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XLookAtController::handleNodePostUpdate(XScene* pScene,XSceneNode* pNode,
														 XSceneNode* pParentNode,
														 XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//If we have a target to look at...
	if (m_pTarget)
	{
		XVector3D fromPos;
		pNode->getWorldPosition(fromPos);
		//XVector3D camPos = XVector3D(XVP::getActiveCamera()->getFinalWorldMatrix().row4);*/
		if (fromPos == *m_pTarget)
			return X_OK;
		
		//First, get a look at matrix using the world positions.
		XMatrix4x4 lookAtMat = XMatrix4x4::getObjectLookat(fromPos,*m_pTarget);
		
		//Now, pull that world look at matrix back into parent space if necessary and
		//also remove pose information.   Once this is done, we're left with a 
		//suitable model transform we can apply to this node.
		/*if (pParentNode)
			lookAtMat = lookAtMat * pParentNode->getWorldTransform().getInverse();
		else
			lookAtMat = lookAtMat * ((XModelObject*)pNode)->getParentModel()->getWorldTransform().getInverse();
		lookAtMat = lookAtMat * ((XModelObject*)pNode)->getPoseTransform().getInverse();
*/
		pNode->setWorldTransform(lookAtMat);
		
		
		pMatrixStack->removeBack();

		//We premultiply the model transform with the parent's transform.
		//This causes the node to inherit all motion of the parent in addition
		//to it's own.
		//pNode->setWorldTransform(lookAtMat * 
		//	((XModelObject*)pNode)->getPoseTransform() *
		//	(*pMatrixStack).back());

		//Also, we push this nodes matrix on the stack so that children may
		//access it in the same manner.
		XMatrix4x4* pMat = pMatrixStack->insertBack();
		(*pMat) = pNode->getWorldTransform();

	}
	return X_OK;
}


