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
//	Module Name:	XViewBindController.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-22-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the view bind controller.  
*/
//*****************************************************************************
XViewBindController::XViewBindController()
{
}



//*****************************************************************************
/**
Destructor for the view bind controller.  
*/
//*****************************************************************************
XViewBindController::~XViewBindController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XViewBindController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the view bind controller.  This is the
description that appears in log files and other debug output.

@return A string description of the view bind controller.
*/
//*****************************************************************************
XS8* XViewBindController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XViewBindController";
	return description; 
}



//*****************************************************************************
/**
Transforms and attaches the client scene node to the current active camera.

@param pNode	Pointer to the scene node that wants to be updated.
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XViewBindController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,
														   XSceneNode* pParentNode,
														   XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//Use the inverse view matrix to bind the node to the camera view. Make
	//sure there is an active camera.

	XCamera* pCamera = pScene->getActiveCamera();
	XMatrix4x4& view = pCamera->getViewMatrix();

	if (pCamera)
		pNode->setModelTransform(view.getInverse());
	
	return X_OK;
}
	  
