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
//	Module Name:	XGizmoScale.cpp
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
XGizmoScale::XGizmoScale()
{
	m_pXAxisSN = NULL;
	m_pYAxisSN = NULL;
	m_pZAxisSN = NULL;
	m_pAllAxisSN = NULL;
	m_moveState = NONE;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoScale::init(XGizmoMgr* pParentMgr,XScene* pScene,XU32 viewportIndex)
{
	XGizmo::init(pParentMgr,pScene,viewportIndex);

	//Initialize model for gizmo
	pParentMgr->getScaleModelTemplate()->generateModel(m_model,true);

	m_model.enableRender(false,"xpole_coll");
	m_model.enableRender(false,"ypole_coll");
	m_model.enableRender(false,"zpole_coll");
	m_model.enableRender(false,"all_coll");

	//Only allow this gizmo to be visible in one viewport
	m_model.setViewportMask(0x0,NULL,true);
	m_model.enableViewport(viewportIndex,true,NULL,true);
	m_gizmoController.setViewportIndex(viewportIndex);
	m_model.attachController(NULL,&m_gizmoController);

	//Initialize shader controllers
	m_xAxisSC.init();
	m_yAxisSC.init();
	m_zAxisSC.init();
	
	//Set default colors for gizmo
	m_xAxisSC.setColor(XColor::Red);
	m_yAxisSC.setColor(XColor::Lime);
	m_zAxisSC.setColor(XColor::Blue);

	//Set render levels
	/*XU32 lastRenderLevel = pScene->getNumRenderLevels() - 1;
	m_xAxisSC.setRenderLevel(lastRenderLevel);
	m_yAxisSC.setRenderLevel(lastRenderLevel);
	m_zAxisSC.setRenderLevel(lastRenderLevel);*/

	//Grab all the materials in the gizmo and set the appropriate shader controllers
	//and render states
	XHandle mat = m_model.getMaterial("xAxisMat");
	XMaterial* pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_xAxisSC);

	mat = m_model.getMaterial("yAxisMat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_yAxisSC);

	mat = m_model.getMaterial("zAxisMat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_zAxisSC);

	//Cache some pointers to specific objects in the gizmo for fast access later
	m_pXAxisSN = m_model.getNode("xpole_coll");
	m_pYAxisSN = m_model.getNode("ypole_coll");
	m_pZAxisSN = m_model.getNode("zpole_coll");
	m_pAllAxisSN = m_model.getNode("all_coll");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoScale::destroy()
{
	//Destroy model
	m_model.destroy();

	//Destroy shader controllers
	m_xAxisSC.destroy();
	m_yAxisSC.destroy();
	m_zAxisSC.destroy();

	return X_OK;
}



XStatus XGizmoScale::setActive(XBOOL active)
{
	if (active && !m_active)
	{
		m_pScene->addGizmoNode(&m_model);	
	}
	else
		m_pScene->removeGizmoNode(&m_model);	
	
	XGizmo::setActive(active);

	return X_OK;
}

void XGizmoScale::onSelectionChanged(XSceneNode* pSceneNode)
{
	//Call base class first
	XGizmo::onSelectionChanged(pSceneNode);

	if (m_pClientSceneNode)
	{
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);

		m_gizmoController.setTransform(m_pClientSceneNode->getWorldTransform());
	}
}


XStatus XGizmoScale::update(XF32 dt)
{
	return X_OK;
}



XStatus XGizmoScale::onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY)
{
	//Depending on which axis has been selected, set a state
	if (pGizmoNode == m_pXAxisSN)
	{
		m_moveState = X_AXIS;
	}
	else if (pGizmoNode == m_pYAxisSN)
	{
		m_moveState = Y_AXIS;
	}
	else if (pGizmoNode == m_pZAxisSN)
	{
		m_moveState = Z_AXIS;
	}	
	else if (pGizmoNode == m_pAllAxisSN)
	{
		m_moveState = ALL_AXIS;
	}

	//Save the initial offset between client node and click
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		m_initialXScale = m_pClientSceneNode->getModelTransform().getXScale();
		m_initialYScale = m_pClientSceneNode->getModelTransform().getYScale();
		m_initialZScale = m_pClientSceneNode->getModelTransform().getZScale();
		m_initialMouseX = (XF32)screenX;
	}

	m_gizmoController.setTransform(m_pClientSceneNode->getWorldTransform());
	
	return X_OK;
}


XStatus XGizmoScale::onMouseUp()
{
	m_moveState = NONE;
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
void XGizmoScale::onMouseDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY)
{
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		XF32 deltaX = endX - m_initialMouseX;
		
		//Can't have a 0 scale
		if (deltaX == 0.0f)
			return;

		XF32 newXScale = m_initialXScale;
		XF32 newYScale = m_initialYScale;
		XF32 newZScale = m_initialZScale;
		if (m_moveState == X_AXIS)
		{
			newXScale = m_initialXScale + (deltaX *.005f);
			if (newXScale < .000001f)
				newXScale = .000001f;
		}
		else if (m_moveState == Y_AXIS)
		{
			newYScale = m_initialYScale + (deltaX *.005f);
			if (newYScale < .000001f)
				newYScale = .000001f;
		}
		else if (m_moveState == Z_AXIS)
		{
			newZScale = m_initialZScale + (deltaX *.005f);
			if (newZScale < .000001f)
				newZScale = .000001f;
		}
		else if (m_moveState == ALL_AXIS)
		{
			newXScale = m_initialXScale + (deltaX *.005f);
			if (newXScale < .000001f)
				newXScale = .000001f;
			newYScale = m_initialYScale + (deltaX *.005f);
			if (newYScale < .000001f)
				newYScale = .000001f;
			newZScale = m_initialZScale + (deltaX *.005f);
			if (newZScale < .000001f)
				newZScale = .000001f;
		}

		XF32 snapSize = m_pParentMgr->getScaleSnapAmount();
		XF32 tempX,tempY,tempZ;
		if (m_pParentMgr->isScaleSnapEnabled())
		{
			tempX = ((XS32)(newXScale/snapSize) * snapSize);
			if (tempX > .000001f)
				newXScale = tempX;
			else
				newXScale = m_initialXScale;
			tempY = ((XS32)(newYScale/snapSize) * snapSize);
			if (tempY > .000001f)
				newYScale = tempY;
			else
				newYScale = m_initialYScale;
			tempZ = ((XS32)(newZScale/snapSize) * snapSize);
			if (tempZ > .000001f)
				newZScale = tempZ;
			else
				newZScale = m_initialZScale;
		}
		
		XMatrix4x4 modelTransform = m_pClientSceneNode->getModelTransform();
		modelTransform.setXScale(newXScale);
		modelTransform.setYScale(newYScale);
		modelTransform.setZScale(newZScale);

		m_pClientSceneNode->setModelTransform(modelTransform);
		m_gizmoController.setTransform(m_pClientSceneNode->getModelTransform());
	}
}


void XGizmoScale::onMouseHover(XSceneNode* pSceneNode)
{
	//If the gizmo is in a move state, we're done...
	if (m_moveState != NONE)
		return;

	//Depending on the particular object we are hovering over, color it
	if (pSceneNode == m_pXAxisSN)
	{
		m_xAxisSC.setColor(XColor::Yellow);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
	}
	else if (pSceneNode == m_pYAxisSN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Yellow);
		m_zAxisSC.setColor(XColor::Blue);
	}
	else if (pSceneNode == m_pZAxisSN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Yellow);
	}
	else if (pSceneNode == m_pAllAxisSN)
	{
		m_xAxisSC.setColor(XColor::Yellow);
		m_yAxisSC.setColor(XColor::Yellow);
		m_zAxisSC.setColor(XColor::Yellow);
	}
	else
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
	}
}


void XGizmoScale::onInstanceMoved(XVector3D& newPosition)
{
	if (m_pClientSceneNode)
	{
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);

		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		m_gizmoController.setTransform(worldTransform);
	}
}


XVector3D XGizmoScale::getPosition()
{
	if (m_pClientSceneNode)
	{
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);
		return nodePos;
	}
	else
		return XVector3D(0,0,0);
}


XBOOL XGizmoScale::contains(XSceneNode* pSceneNode)
{
	return m_model.containsNode(pSceneNode);
}
