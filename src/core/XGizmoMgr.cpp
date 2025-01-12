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
//	Module Name:	XGizmoMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-19-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



XModelTemplate	XGizmoMgr::m_moveModelTemplate;
XModelTemplate	XGizmoMgr::m_rotateModelTemplate;
XModelTemplate	XGizmoMgr::m_scaleModelTemplate;

//*****************************************************************************
//*****************************************************************************
XGizmoMgr::XGizmoMgr()
{
	for (int i = 0;i < MAX_VIEWPORTS;++i)
		m_pCurrentGizmos[i] = NULL;
	m_gizmoMode = X_GM_NONE;
	m_pScene = NULL;
	m_pActiveGizmo = NULL;

	m_moveSnapEnabled = false;
	m_rotateSnapEnabled = false;
	m_scaleSnapEnabled = false;
	m_moveSnapAmount = 1.5f;
	m_rotateSnapAmount = 3.14159f/4.0f;
	m_scaleSnapAmount = 2.0f;
}


XStatus XGizmoMgr::initStaticData()
{
	//Initialize model templates
	XS8 buf[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_SYSTEM,"models/moveGizmo.xmf",buf);
	m_moveModelTemplate.init(buf);

	XOSMgr::buildPath(X_PATH_SYSTEM,"models/rotateGizmo.xmf",buf);
	m_rotateModelTemplate.init(buf);

	XOSMgr::buildPath(X_PATH_SYSTEM,"models/scaleGizmo.xmf",buf);
	m_scaleModelTemplate.init(buf);

	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XGizmoMgr::init(XScene* pScene)
{
	X_ASSERTM(pScene,"Scene cannot be null");
	m_pScene = pScene;

	//Create the gizmos
	for (int i = 0;i < MAX_VIEWPORTS;++i)
	{
		m_gizmoMove[i].init(this,pScene,i);
		m_gizmoRotate[i].init(this,pScene,i);
		m_gizmoScale[i].init(this,pScene,i);
	}

	return X_OK;
}

XStatus XGizmoMgr::destroyStaticData()
{
	//Destroy model templates
	m_moveModelTemplate.destroy();
	m_rotateModelTemplate.destroy();
	m_scaleModelTemplate.destroy();

	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XGizmoMgr::destroy()
{
	//Deactivate current gizmo
	if (m_pCurrentGizmos[0] != NULL)
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
		{
			m_pCurrentGizmos[i]->setActive(false);
			m_pCurrentGizmos[i] = NULL;
		}
	}

	//Destroy the gizmos
	for (int i = 0;i < MAX_VIEWPORTS;++i)
	{
		m_gizmoMove[i].destroy();
		m_gizmoRotate[i].destroy();
		m_gizmoScale[i].destroy();
	}

	return X_OK;
}


XStatus XGizmoMgr::update(XF32 dt)
{
	if (m_pCurrentGizmos[0] != NULL)
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i]->update(dt);
	}
	
	return X_OK;
}


XStatus XGizmoMgr::updateHover(XS32 x,XS32 y)
{
	if (m_pCurrentGizmos[0] != NULL)
	{
		XSceneNode* pSceneNode = m_pScene->selectGizmoNode(x,y);

		if (pSceneNode)
		{
			for (int i = 0;i < MAX_VIEWPORTS;++i)
				m_pCurrentGizmos[i]->onMouseHover(pSceneNode);
		}
		else
		{
			for (int i = 0;i < MAX_VIEWPORTS;++i)
				m_pCurrentGizmos[i]->onMouseHover(NULL);
		}
	}

	return X_OK;
}


XStatus XGizmoMgr::updateDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY)
{
	//Only the active gizmo instance gets the mouse drag message
	/*if (m_pActiveGizmo)
	{
		m_pActiveGizmo->onMouseDrag(startX,startY,endX,endY);
	}*/
	if (m_pCurrentGizmos[0])
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i]->onMouseDrag(startX,startY,endX,endY);
	}

	

	//After sending the mouse drag message, update the others with the changed position
	//of the active gizmo, if there is one...
	if (m_pActiveGizmo && m_pCurrentGizmos[0])
	{
		XVector3D newPos = m_pActiveGizmo->getPosition();
		for (int i = 0;i < MAX_VIEWPORTS;++i)
		{
			if (m_pCurrentGizmos[i] != m_pActiveGizmo)
				m_pCurrentGizmos[i]->onInstanceMoved(newPos);
		}
	}

	/*if (m_pCurrentGizmos[0])
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i]->onMouseDrag(startX,startY,endX,endY);
	}*/

	return X_OK;
}

XStatus XGizmoMgr::onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY)
{
	if (m_pCurrentGizmos[0])
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i]->onMouseDown(pGizmoNode,screenX,screenY);
	}

	return X_OK;
}

XStatus XGizmoMgr::onMouseUp()
{
	if (m_pCurrentGizmos[0])
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i]->onMouseUp();
	}

	m_pActiveGizmo = NULL;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoMgr::onMouseOver(XF32 x,XF32 y)
{
	return X_OK;
}


XStatus XGizmoMgr::setMode(X_GIZMO_MODE mode)
{
	//If the mode is already set, bail
	if (m_gizmoMode == mode)
		return X_OK;

	m_gizmoMode = mode;

	//Clear current gizmo 
	if (m_pCurrentGizmos[0] != NULL)
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
		{
			m_pCurrentGizmos[i]->setActive(false);
			m_pCurrentGizmos[i] = NULL;
		}
	}

	//Is there a selected node?  If not, don't activate gizmo
	if (m_pScene->getSelectedNodes()->getNumItems() == 0)
		return X_OK;

	switch (mode)
	{
	case X_GM_MOVE:
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i] = &m_gizmoMove[i];
		break;
	case X_GM_ROTATE:
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i] = &m_gizmoRotate[i];
		break;
	case X_GM_SCALE:
		for (int i = 0;i < MAX_VIEWPORTS;++i)
			m_pCurrentGizmos[i] = &m_gizmoScale[i];
		break;
	}

	if (m_pCurrentGizmos[0])
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
		{
			m_pCurrentGizmos[i]->setActive(true);
			m_pCurrentGizmos[i]->onSelectionChanged(m_pScene->getSelectedNodes()->getFront());
		}
	}

	return X_OK;
}



XStatus XGizmoMgr::setActiveGizmo(XGizmo* pGizmo)
{
	m_pActiveGizmo = pGizmo;
	return X_OK;
}

XStatus XGizmoMgr::onSelectionChanged()
{
	//Is there a selected node?  If not, deactivate current gizmo
	if (m_pScene->getSelectedNodes()->getNumItems() == 0)
	{
		if (m_pCurrentGizmos[0] != NULL)
		{
			for (int i = 0;i < MAX_VIEWPORTS;++i)
			{
				m_pCurrentGizmos[i]->setActive(false);		
				m_pCurrentGizmos[i] = NULL;
			}
		}
	}
	else
	{
		//If gizmo mode isn't active, set it if we have a valid mode
		if (m_pCurrentGizmos[0] == NULL && m_gizmoMode != X_GM_NONE)
		{
			switch (m_gizmoMode)
			{
			case X_GM_MOVE:
				for (int i = 0;i < MAX_VIEWPORTS;++i)
					m_pCurrentGizmos[i] = &m_gizmoMove[i];
				break;
			case X_GM_ROTATE:
				for (int i = 0;i < MAX_VIEWPORTS;++i)
					m_pCurrentGizmos[i] = &m_gizmoRotate[i];
				break;
			case X_GM_SCALE:
				for (int i = 0;i < MAX_VIEWPORTS;++i)
					m_pCurrentGizmos[i] = &m_gizmoScale[i];
				break;
			}

			for (int i = 0;i < MAX_VIEWPORTS;++i)
				m_pCurrentGizmos[i]->setActive(true);
		}
		
		if (m_pCurrentGizmos[0])
		{
			for (int i = 0;i < MAX_VIEWPORTS;++i)
				m_pCurrentGizmos[i]->onSelectionChanged(m_pScene->getSelectedNodes()->getFront());
		}
	}

	return X_OK;
}

XBOOL XGizmoMgr::isGizmo(XSceneNode* pSceneNode)
{
	XBOOL contains = false;
	if (m_pCurrentGizmos[0] != NULL)
	{
		for (int i = 0;i < MAX_VIEWPORTS;++i)
		{
			if (m_pCurrentGizmos[i]->contains(pSceneNode))
			{
				contains = true;
				break;
			}
		}
	}
	
	return contains;
}


XBOOL XGizmoMgr::isMoveSnapEnabled()
{
	return m_moveSnapEnabled;
}

XStatus XGizmoMgr::enableMoveSnap(XBOOL enable)
{
	m_moveSnapEnabled = enable;
	return X_OK;
}

XF32 XGizmoMgr::getMoveSnapAmount()
{
	return m_moveSnapAmount;
}
XStatus XGizmoMgr::setMoveSnapAmount(XF32 amount)
{
	m_moveSnapAmount = X_MATH_MAX(amount,.000001f);
	return X_OK;
}

XBOOL XGizmoMgr::isRotateSnapEnabled()
{
	return m_rotateSnapEnabled;
}

XStatus XGizmoMgr::enableRotateSnap(XBOOL enable)
{
	m_rotateSnapEnabled = enable;
	return X_OK;
}

XF32 XGizmoMgr::getRotateSnapAmount()
{
	return m_rotateSnapAmount;
}
XStatus XGizmoMgr::setRotateSnapAmount(XF32 amount)
{
	m_rotateSnapAmount = X_MATH_MAX(amount,.000001f);
	return X_OK;
}

XBOOL XGizmoMgr::isScaleSnapEnabled()
{
	return m_scaleSnapEnabled;
}
XStatus XGizmoMgr::enableScaleSnap(XBOOL enable)
{
	m_scaleSnapEnabled = enable;
	return X_OK;
}
XF32 XGizmoMgr::getScaleSnapAmount()
{
	return m_scaleSnapAmount;
}
XStatus XGizmoMgr::setScaleSnapAmount(XF32 amount)
{
	m_scaleSnapAmount = X_MATH_MAX(amount,.000001f);
	return X_OK;
}

XModelTemplate* XGizmoMgr::getMoveModelTemplate()
{
	return &m_moveModelTemplate;
}
XModelTemplate* XGizmoMgr::getRotateModelTemplate()
{
	return &m_rotateModelTemplate;
}
XModelTemplate* XGizmoMgr::getScaleModelTemplate()
{
	return &m_scaleModelTemplate;
}