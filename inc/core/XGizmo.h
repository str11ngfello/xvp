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
//	Module Name:	XGizmo.h
//	Author:			Lynn Duke
//	Creation Date:	4-18-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GIZMO_H__
#define __X_GIZMO_H__

class XGizmoMgr;

//*****************************************************************************
//*****************************************************************************
class XGizmo
{
protected:
	XBOOL				m_active;
	XSceneNode*			m_pClientSceneNode;
	XScene*				m_pScene;
	XGizmoMgr*			m_pParentMgr;
	XU32				m_viewportIndex;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGizmo();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XGizmoMgr* pParentMgr,XScene* pScene,XU32 viewportIndex);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XBOOL isActive();
	virtual XStatus setActive(XBOOL active);
	virtual XStatus onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY);
	virtual XStatus onMouseUp();
	virtual void onMouseDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY);
	virtual void onMouseHover(XSceneNode* pSceneNode);
	virtual void onSelectionChanged(XSceneNode* pSceneNode);
	virtual void onInstanceMoved(XVector3D& newPosition);
	virtual XVector3D getPosition();
	virtual XBOOL contains(XSceneNode* pSceneNode);
};

#endif

