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
//	Module Name:	XLookAtController.h
//	Author:			Lynn Duke
//	Creation Date:	8-3-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_LOOK_AT_CONTROLLER_H__
#define __X_LOOK_AT_CONTROLLER_H__




//*****************************************************************************
//! Look at constraint to force a scene node to face a particluar point in space.
/** 
The XLookAtController object is a constraining controller you can attach to
scene nodes to force them to always face towards a particluar point in space.

See below for details.
*/
//*****************************************************************************
class XLookAtController : public XController
{
private:
	XVector3D*		m_pTarget;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XLookAtController();
	~XLookAtController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	bool isCompatible(XSceneNode* pNode);
    XStatus setTarget(XVector3D* pTarget);
    void removeTarget();
	XStatus handleNodePostUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
									   XFreeArray<XMatrix4x4>* pMatrixStack);
};


#endif