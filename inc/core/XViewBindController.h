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
//	Module Name:	XViewBindController.h
//	Author:			Lynn Duke
//	Creation Date:	2-22-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VIEW_BIND_CONTROLLER_H__
#define __X_VIEW_BIND_CONTROLLER_H__




//*****************************************************************************
//! Attaches and transform a scene node to the active camera.
/**
The XViewBindController is used to attach and transform a scene node with
the active camera.  To avoid floating point round off errors that cause
"camera jitter", the view bind controller will create a hard attachement
between the scene node and the camera.
*/
//*****************************************************************************
class XViewBindController : public XController
{
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XViewBindController();
	~XViewBindController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
									   XFreeArray<XMatrix4x4>* pMatrixStack);
};

#endif