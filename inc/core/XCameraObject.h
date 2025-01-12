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
//	Module Name:	XCameraObject.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CAMERA_OBJECT_H__
#define __X_CAMERA_OBJECT_H__



//*****************************************************************************
//! Camera objects used in the camera implementation.
/**
XCameraObject objects are used in the creation of cameras and provide movement
functionality.
*/
//*****************************************************************************
class XCameraObject : public XModelObject
{							        
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XCameraObject();
	~XCameraObject();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XStatus updateTransforms(XF32 dt, XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus prepForRender(XF32 dt,XScene* pScene);
	XStatus render();
	XStatus getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared);
	XStatus setLODIndex(XU8 LODIndex);
};


#endif
