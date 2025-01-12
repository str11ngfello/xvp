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
//	Module Name:	XGizmoRotate.cpp
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
XGizmoRotate::XGizmoRotate()
{
	m_pXAxisSN = NULL;
	m_pYAxisSN = NULL;
	m_pZAxisSN = NULL;
	m_moveState = NONE;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoRotate::init(XGizmoMgr* pParentMgr,XScene* pScene,XU32 viewportIndex)
{
	XGizmo::init(pParentMgr,pScene,viewportIndex);

	//Initialize model for gizmo
	pParentMgr->getRotateModelTemplate()->generateModel(m_model,true);

	m_model.enableRender(false,"xpole_coll");
	m_model.enableRender(false,"ypole_coll");
	m_model.enableRender(false,"zpole_coll");

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

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoRotate::destroy()
{
	//Destroy model
	m_model.destroy();

	//Destroy shader controllers
	m_xAxisSC.destroy();
	m_yAxisSC.destroy();
	m_zAxisSC.destroy();

	return X_OK;
}



XStatus XGizmoRotate::setActive(XBOOL active)
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

void XGizmoRotate::onSelectionChanged(XSceneNode* pSceneNode)
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


XStatus XGizmoRotate::update(XF32 dt)
{
	return X_OK;
}



XStatus XGizmoRotate::onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY)
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

	//Save the initial offset between client node and click
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		m_initialTransform = m_pClientSceneNode->getModelTransform();
		m_initialPosition.x = (XF32)screenX;
		m_initialPosition.y = (XF32)screenY;
	}


		m_gizmoController.setTransform(m_pClientSceneNode->getWorldTransform());
	/*
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		//Tell the manager that this gizmo is active
		m_pParentMgr->setActiveGizmo(this);

		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);
		XMatrix4x4 nodeMat;
		nodeMat = m_pClientSceneNode->getWorldTransform();

		XVector3D lineStart,lineEnd;
		XICreatePickLine(screenX,
						 screenY,
						 m_pScene->getViewportCamera(m_viewportIndex)->getViewportWidth(),
						 m_pScene->getViewportCamera(m_viewportIndex)->getViewportHeight(),
						 m_pScene->getViewportCamera(m_viewportIndex)->getViewMatrix(),
						 m_pScene->getViewportCamera(m_viewportIndex)->getProjMatrix(),
						 lineStart,
						 lineEnd);

		XVector3D pickLineDir = (lineEnd-lineStart).normalize();
		XVector3D planeNormal;	

		//Find the best possible plane to intersect with that will give least likely
		//give us a non-intersection condition... such as our line being parallel to
		//our plane.
		XVector3D camPos = XVector3D(m_pScene->getViewportCamera(m_viewportIndex)->getWorldTransform().row4);
		XVector3D intersectionPoint;
		XVector3D newIntersectionPoint;
		if (m_moveState == X_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row2);

			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			XF32 time;
			if (fabsf(pickLineDir.dot(planeNormal)) < .3f)
				planeNormal = XVector3D(nodeMat.row3);

			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);
			
			newIntersectionPoint = intersectionPoint;
			newIntersectionPoint.y = nodePos.y;
			newIntersectionPoint.z = nodePos.z;
		}
		else if (m_moveState == Y_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row1);
		
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			XF32 time;
			if (fabsf(pickLineDir.dot(planeNormal)) < .3f)
				planeNormal = XVector3D(nodeMat.row3);

			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			newIntersectionPoint = intersectionPoint;
			newIntersectionPoint.x = nodePos.x;
			newIntersectionPoint.z = nodePos.z;
		}
		else if (m_moveState == Z_AXIS)
		{
			//Pick a plane to move in...
			planeNormal = XVector3D(nodeMat.row2);

			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			XF32 time;
			if (fabsf(pickLineDir.dot(planeNormal)) < .3f)
				planeNormal = XVector3D(nodeMat.row1);

			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			newIntersectionPoint = intersectionPoint;
			newIntersectionPoint.x = nodePos.x;
			newIntersectionPoint.y = nodePos.y;
		}

		m_initialOffset = nodePos - newIntersectionPoint;
		m_pClientSceneNode->setModelPosition(newIntersectionPoint + m_initialOffset);


		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		XVector3D finalPos = newIntersectionPoint + m_initialOffset;
		worldTransform._41 = finalPos.x;
		worldTransform._42 = finalPos.y;
		worldTransform._43 = finalPos.z;
		m_gizmoController.setTransform(worldTransform);
	}*/

	return X_OK;
}


XStatus XGizmoRotate::onMouseUp()
{
	m_moveState = NONE;
	m_initialOffset = XVector3D(0,0,0);
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
void XGizmoRotate::onMouseDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY)
{
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		XVector2D delta;
		delta.x = endX-m_initialPosition.x;
		delta.y = endY-m_initialPosition.y;

		XF32 distance = delta.mag();		
		XF32 snappedRotation = delta.x*.01f;

		XF32 snapSize = m_pParentMgr->getRotateSnapAmount();
		if (m_pParentMgr->isRotateSnapEnabled())
		{
			snappedRotation = ((XS32)(snappedRotation/snapSize) * snapSize);
		}

		XMatrix4x4 rot;
		if (m_moveState == X_AXIS)
		{
			rot.createXRotation(snappedRotation);
		}
		else if (m_moveState == Y_AXIS)
		{
			
			rot.createYRotation(snappedRotation);
		}
		else if (m_moveState == Z_AXIS)
		{

			rot.createZRotation(snappedRotation);
		}
		
		m_pClientSceneNode->setModelTransform(rot * m_initialTransform);
		m_gizmoController.setTransform(m_pClientSceneNode->getModelTransform());
	}
	/*
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);
		XMatrix4x4 nodeMat;
		nodeMat = m_pClientSceneNode->getWorldTransform();

		XVector3D lineStart,lineEnd;
		XICreatePickLine((int)endX,
						 (int)endY,
						 m_pScene->getViewportCamera(m_viewportIndex)->getViewportWidth(),
						 m_pScene->getViewportCamera(m_viewportIndex)->getViewportHeight(),
						 m_pScene->getViewportCamera(m_viewportIndex)->getViewMatrix(),
						 m_pScene->getViewportCamera(m_viewportIndex)->getProjMatrix(),
						 lineStart,
						 lineEnd);

		XVector3D intersectionPoint;
		XVector3D planeNormal;
		XVector3D pickLineDir = (lineEnd-lineStart).normalize();

		if (m_moveState == X_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row2);

			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			XF32 time;
			if (fabsf(pickLineDir.dot(planeNormal)) < .3f)
				planeNormal = XVector3D(nodeMat.row3);

			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			nodePos.x = intersectionPoint.x;
		}
		else if (m_moveState == Y_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row1);

			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			XF32 time;
			if (fabsf(pickLineDir.dot(planeNormal)) < .3f)
				planeNormal = XVector3D(nodeMat.row3);

			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			nodePos.y = intersectionPoint.y;
		}
		else if (m_moveState == Z_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row2);
			
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			XF32 time;
			if (fabsf(pickLineDir.dot(planeNormal)) < .3f)
				planeNormal = XVector3D(nodeMat.row1);

			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			nodePos.z = intersectionPoint.z;
		}
		
		m_pClientSceneNode->setModelPosition(nodePos + m_initialOffset);
		
		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		XVector3D finalPos = nodePos + m_initialOffset;
		worldTransform._41 = finalPos.x;
		worldTransform._42 = finalPos.y;
		worldTransform._43 = finalPos.z;
		m_gizmoController.setTransform(worldTransform);
	}*/
}


void XGizmoRotate::onMouseHover(XSceneNode* pSceneNode)
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
	else
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
	}
}


void XGizmoRotate::onInstanceMoved(XVector3D& newPosition)
{
	if (m_pClientSceneNode)
	{
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);

		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		m_gizmoController.setTransform(worldTransform);
	}
}


XVector3D XGizmoRotate::getPosition()
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


XBOOL XGizmoRotate::contains(XSceneNode* pSceneNode)
{
	return m_model.containsNode(pSceneNode);
}
