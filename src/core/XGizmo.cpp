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
//	Module Name:	XGizmo.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-19-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XGizmo::XGizmo()
{
	m_active = false;
	m_pScene = NULL;
	m_pClientSceneNode = NULL;
	m_pParentMgr = NULL;
	m_viewportIndex = 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmo::init(XGizmoMgr* pParentMgr,XScene* pScene,XU32 viewportIndex)
{
	m_pParentMgr = pParentMgr;
	m_pScene = pScene;
	m_viewportIndex = viewportIndex;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmo::destroy()
{
	return X_OK;
}


XStatus XGizmo::update(XF32 dt)
{
	return X_OK;
}


XBOOL XGizmo::isActive()
{
	return m_active;
}



XStatus XGizmo::setActive(XBOOL active)
{
	m_active = active;
	
	if (!active)
		m_pClientSceneNode = NULL;
	
	return X_OK;
}


XStatus XGizmo::onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY)
{
	return X_OK;
}

XStatus XGizmo::onMouseUp()
{
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
void XGizmo::onMouseDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY)
{

}


void XGizmo::onMouseHover(XSceneNode* pSceneNode)
{
}



void XGizmo::onSelectionChanged(XSceneNode* pSceneNode)
{
	m_pClientSceneNode = pSceneNode;
}


void XGizmo::onInstanceMoved(XVector3D& newPosition)
{
}

XVector3D XGizmo::getPosition()
{
	return XVector3D(0,0,0);
}

XBOOL XGizmo::contains(XSceneNode* pSceneNode)
{
	return false;
}