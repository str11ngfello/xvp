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
//	Module Name:	XModelLODController.cpp
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
Constructor for the model LOD controller.  
*/
//*****************************************************************************
XModelLODController::XModelLODController()
{
}



//*****************************************************************************
/**
Destructor for the model LOD controller.  
*/
//*****************************************************************************
XModelLODController::~XModelLODController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XModelLODController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the model LOD controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model LOD controller.
*/
//*****************************************************************************
XS8* XModelLODController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XModelLODController";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
bool XModelLODController::isCompatible(XSceneNode* pNode)
{
	if (pNode)
	{
		X_SCENE_NODE_TYPE snt = pNode->getType();
		return (X_SNT_GET_EXT_SNID(snt) == X_SNID_MODEL_OBJECT);
	}
	else
		return false;
}



//*****************************************************************************
/**
Allows the controller to influence a scene node.  Normally, the user does
not call this method manually.  Instead, once this controller has been 
attached to a node and the node has been added to the scene graph, this method
is called by the node whenever the update pass occurs in the main scene graph.

@param pNode Pointer to the current scene node which is requesting an update.
			 This parameter will never be NULL.  
@param pParentNode Pointer to the parent scene node of pNode.  If the node
				   does not have a parent, this parameter will be NULL.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
					This parameter will never be NULL.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XModelLODController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,
												 XSceneNode* pParentNode,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	XCamera* pCamera = pScene->getActiveCamera();
	XModelObject* pObject = (XModelObject*)pNode;
	XU32 numLODRanges = 0;
	XF32* pLODRangesSquared = NULL;
	XU8 finalLODIndex = 0;
	XF32 distanceSquared = 0;

	//First, get the object's LOD ranges
	pObject->getLODRanges(numLODRanges,&pLODRangesSquared);

	//If it has LOD ranges and there is an active camera...
	if ((pCamera) && (numLODRanges > 0))
	{
		XVector3D camPos,worldPos;
		pCamera->getWorldPosition(camPos);
		pNode->getWorldPosition(worldPos);
		distanceSquared = camPos.distanceSquared(worldPos);

		for (XS32 i = numLODRanges-1;i >= 0;--i)
		{
			if (distanceSquared >= pLODRangesSquared[i])
			{
				finalLODIndex = i+1;
				break;
			}
		}
				
		pObject->setLODIndex(finalLODIndex);
	}

	return X_OK;
}
	  


//*****************************************************************************
/**
Allows the animation to update any time dependant internal variables.  X-VP
calls this method once per frame.

@param timeElapsed	The amount of time in seconds that has passed since the last
					frame update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModelLODController::update(XF32 timeElapsed)
{
	return X_OK;
}



