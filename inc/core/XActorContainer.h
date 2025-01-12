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
//	Module Name:	XActorContainer.h
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ACTOR_CONTAINER_H__
#define __X_ACTOR_CONTAINER_H__




//*****************************************************************************
//*****************************************************************************
class XActorContainer : public XSceneNode
{					  
protected:
	

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************

public:
	XMemBlockList<XSceneNode*>		m_sceneNodes;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XActorContainer();
	virtual ~XActorContainer();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	virtual XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual void pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits);

};

#endif
