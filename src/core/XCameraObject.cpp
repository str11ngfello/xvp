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
//	Module Name:	XCameraObject.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the camera object.
*/
//*****************************************************************************
XCameraObject::XCameraObject()
{
}



//*****************************************************************************
/**
Destructor for the camera object.
*/
//*****************************************************************************
XCameraObject::~XCameraObject()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XCameraObject::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL_OBJECT,X_SNID_CAMERA_OBJECT);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the camera object.  This is the
description that appears in log files and other debug output.

@return A string description of the bone.
*/
//*****************************************************************************
XS8* XCameraObject::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XCameraObject";
	return description; 
}



//*****************************************************************************
/**
Updates the bones world space transform and updates the skin transform used
during the skinning process.

@param dt	The amount of time, in seconds, that has elapsed since the last
					update.
@param pMatrixStack	The scene graph matrix stack.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XCameraObject::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)	
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCameraObject::prepForRender(XF32 dt,XScene* pScene)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCameraObject::render()	
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XCameraObject::getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared)
{
	return X_ERR_NOT_SUPPORTED;
}


//*****************************************************************************
//*****************************************************************************
XStatus XCameraObject::setLODIndex(XU8 LODIndex)
{
	return X_ERR_NOT_SUPPORTED;
}
