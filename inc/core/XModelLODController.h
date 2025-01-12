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
//	Module Name:	XAnimController.h
//	Author:			Lynn Duke
//	Creation Date:	8-9-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_LOD_CONTROLLER_H__
#define __X_MODEL_LOD_CONTROLLER_H__





//*****************************************************************************
//*****************************************************************************
class XModelLODController : public XController
{
private:
	


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModelLODController();
	virtual ~XModelLODController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	bool isCompatible(XSceneNode* pNode);
	XStatus update(XF32 dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
							 XFreeArray<XMatrix4x4>* pMatrixStack);
};


#endif