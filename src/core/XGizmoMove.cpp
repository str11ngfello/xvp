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
//	Module Name:	XGizmoMove.cpp
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
XGizmoMove::XGizmoMove()
{
	m_pXAxisSN = NULL;
	m_pYAxisSN = NULL;
	m_pZAxisSN = NULL;
	m_pXRidge1SN = NULL;
	m_pXRidge2SN = NULL;
	m_pYRidge1SN = NULL;
	m_pYRidge2SN = NULL;
	m_pZRidge1SN = NULL;
	m_pZRidge2SN = NULL;
	m_moveState = NONE;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoMove::init(XGizmoMgr* pParentMgr,XScene* pScene,XU32 viewportIndex)
{
	XGizmo::init(pParentMgr,pScene,viewportIndex);

	//Initialize model for gizmo
	pParentMgr->getMoveModelTemplate()->generateModel(m_model,true);

	m_model.enableRender(false,"xpole_coll");
	m_model.enableRender(false,"ypole_coll");
	m_model.enableRender(false,"zpole_coll");
	m_model.enableRender(false,"xridge1_coll");
	m_model.enableRender(false,"xridge2_coll");
	m_model.enableRender(false,"yridge1_coll");
	m_model.enableRender(false,"yridge2_coll");
	m_model.enableRender(false,"zridge1_coll");
	m_model.enableRender(false,"zridge2_coll");

	//Only allow this gizmo to be visible in one viewport
	m_model.setViewportMask(0x0,NULL,true);
	m_model.enableViewport(viewportIndex,true,NULL,true);
	m_gizmoController.setViewportIndex(viewportIndex);
	m_model.attachController(NULL,&m_gizmoController);
	m_gizmoController.enableTransformLock(true);

	//Initialize shader controllers
	m_xAxisSC.init();
	m_yAxisSC.init();
	m_zAxisSC.init();
	m_xRidge1SC.init();
	m_xRidge2SC.init();
	m_yRidge1SC.init();
	m_yRidge2SC.init();
	m_zRidge1SC.init();
	m_zRidge2SC.init();
	
	//Set default colors for gizmo
	m_xAxisSC.setColor(XColor::Red);
	m_yAxisSC.setColor(XColor::Lime);
	m_zAxisSC.setColor(XColor::Blue);
	m_xRidge1SC.setColor(XColor::Red);
	m_xRidge2SC.setColor(XColor::Red);
	m_yRidge1SC.setColor(XColor::Lime);
	m_yRidge2SC.setColor(XColor::Lime);
	m_zRidge1SC.setColor(XColor::Blue);
	m_zRidge2SC.setColor(XColor::Blue);

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

	mat = m_model.getMaterial("xridge1Mat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_xRidge1SC);

	mat = m_model.getMaterial("xridge2Mat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_xRidge2SC);

	mat = m_model.getMaterial("yridge1Mat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_yRidge1SC);

	mat = m_model.getMaterial("yridge2Mat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_yRidge2SC);

	mat = m_model.getMaterial("zridge1Mat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_zRidge1SC);

	mat = m_model.getMaterial("zridge2Mat");
	pMat = XVP::GraphicsMgr.lockMaterial(mat);
	pMat->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(mat);
	pMat->attachShaderController(&m_zRidge2SC);

	//Cache some pointers to specific objects in the gizmo for fast access later
	m_pXAxisSN = m_model.getNode("xpole_coll");
	m_pYAxisSN = m_model.getNode("ypole_coll");
	m_pZAxisSN = m_model.getNode("zpole_coll");
	m_pXRidge1SN = m_model.getNode("xridge1_coll");
	m_pXRidge2SN = m_model.getNode("xridge2_coll");
	m_pYRidge1SN = m_model.getNode("yridge1_coll");
	m_pYRidge2SN = m_model.getNode("yridge2_coll");
	m_pZRidge1SN = m_model.getNode("zridge1_coll");
	m_pZRidge2SN = m_model.getNode("zridge2_coll");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGizmoMove::destroy()
{
	m_pScene->removeRenderNode(&m_model);

	//Destroy model
	m_model.destroy();

	//Destroy shader controllers
	m_xAxisSC.destroy();
	m_yAxisSC.destroy();
	m_zAxisSC.destroy();
	m_xRidge1SC.destroy();
	m_xRidge2SC.destroy();
	m_yRidge1SC.destroy();
	m_yRidge2SC.destroy();
	m_zRidge1SC.destroy();
	m_zRidge2SC.destroy();

	return X_OK;
}



XStatus XGizmoMove::setActive(XBOOL active)
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

void XGizmoMove::onSelectionChanged(XSceneNode* pSceneNode)
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


XStatus XGizmoMove::update(XF32 dt)
{
	/*if (m_pScene->getViewportCamera(m_viewportIndex))
	{
		if (fabs(XVector3D::ZAxis.dot(XVector3D(m_pScene->getViewportCamera(m_viewportIndex)->getWorldTransform().row3))) > .997f)
		{
			m_model.enableRender(false,"zpole");
			m_model.enableRender(false,"zcone");
		}
		else
		{
			m_model.enableRender(true,"zpole");
			m_model.enableRender(true,"zcone");
		}
	}*/

	return X_OK;
}



XStatus XGizmoMove::onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY)
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
	else if (pGizmoNode == m_pXRidge1SN || pGizmoNode == m_pYRidge1SN)
	{
		m_moveState = XY_AXIS;
	}
	else if (pGizmoNode == m_pXRidge2SN || pGizmoNode == m_pZRidge2SN)
	{
		m_moveState = XZ_AXIS;
	}
	else if (pGizmoNode == m_pYRidge2SN || pGizmoNode == m_pZRidge1SN)
	{
		m_moveState = YZ_AXIS;
	}
	else
	{
		m_moveState = NONE;
	}

	//Save the initial offset between client node and click
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		//Tell the manager that this gizmo is active
		m_pParentMgr->setActiveGizmo(this);

		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);
		XMatrix4x4 nodeMat;
		nodeMat = m_pClientSceneNode->getWorldTransform();
		XCamera* pCam = m_pScene->getViewportCamera(m_viewportIndex);
		XVector3D lineStart,lineEnd;
		XICreatePickLine(screenX,
						 screenY,
						 pCam->getViewportWidth(),
						 pCam->getViewportHeight(),
						 pCam->getViewMatrix(),
						 pCam->getProjMatrix(),
						 lineStart,
						 lineEnd);

		XVector3D pickLineDir = (lineEnd-lineStart).normalize();
		XVector3D planeNormal;	

		//Find the best possible plane to intersect with that will give least likely
		//give us a non-intersection condition... such as our line being parallel to
		//our plane.
		XVector3D intersectionPoint;
		XVector3D newIntersectionPoint;
		if (m_moveState == X_AXIS)
		{
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			planeNormal = XVector3D(nodeMat.row2);
			if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
				planeNormal = XVector3D(nodeMat.row3);

			XF32 time;
			if (XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
				newIntersectionPoint = intersectionPoint;
				newIntersectionPoint.y = nodePos.y;
				newIntersectionPoint.z = nodePos.z;
			}
		}
		else if (m_moveState == Y_AXIS)
		{
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			planeNormal = XVector3D(nodeMat.row1);
			if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
				planeNormal = XVector3D(nodeMat.row3);
		
			XF32 time;
			if (XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
				newIntersectionPoint = intersectionPoint;
				newIntersectionPoint.x = nodePos.x;
				newIntersectionPoint.z = nodePos.z;
			}
		}
		else if (m_moveState == Z_AXIS)
		{
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			planeNormal = XVector3D(nodeMat.row2);
			if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
				planeNormal = XVector3D(nodeMat.row1);

			XF32 time;
			if (XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
				newIntersectionPoint = intersectionPoint;
				newIntersectionPoint.x = nodePos.x;
				newIntersectionPoint.y = nodePos.y;
			}

		}
		else if (m_moveState == XY_AXIS)
		{
			//Pick a plane to move in...
			planeNormal = XVector3D(nodeMat.row3);

			//Where does the pick line intersect the plane of movement? If it doesn't, then
			//use the other plane of movement for this axis
			XF32 time;
			if (!XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
			}
			newIntersectionPoint = intersectionPoint;
			newIntersectionPoint.z = nodePos.z;
		}
		else if (m_moveState == XZ_AXIS)
		{
			//Pick a plane to move in...
			planeNormal = XVector3D(nodeMat.row2);

			//Where does the pick line intersect the plane of movement? If it doesn't, then
			//use the other plane of movement for this axis
			XF32 time;
			if (!XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
			}
			newIntersectionPoint = intersectionPoint;
			newIntersectionPoint.y = nodePos.y;
		}
		else if (m_moveState == YZ_AXIS)
		{
			//Pick a plane to move in...
			planeNormal = XVector3D(nodeMat.row1);

			//Where does the pick line intersect the plane of movement? If it doesn't, then
			//use the other plane of movement for this axis
			XF32 time;
			if (!XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
			}
			newIntersectionPoint = intersectionPoint;
			newIntersectionPoint.x = nodePos.x;
		}

		m_initialOffset = nodePos - newIntersectionPoint;
		m_pClientSceneNode->setModelPosition(newIntersectionPoint + m_initialOffset);


		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		XVector3D finalPos = newIntersectionPoint + m_initialOffset;
		worldTransform._41 = finalPos.x;
		worldTransform._42 = finalPos.y;
		worldTransform._43 = finalPos.z;
		m_gizmoController.setTransform(worldTransform);
	}

	return X_OK;
}


XStatus XGizmoMove::onMouseUp()
{
	m_moveState = NONE;
	m_initialOffset = XVector3D(0,0,0);
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
void XGizmoMove::onMouseDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY)
{
	if (m_pClientSceneNode && m_moveState != NONE)
	{
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);
		XMatrix4x4 nodeMat;
		nodeMat = m_pClientSceneNode->getWorldTransform();

		XVector3D lineStart,lineEnd;
		XCamera* pCam = m_pScene->getViewportCamera(m_viewportIndex);
		XICreatePickLine((int)endX,
						 (int)endY,
						 pCam->getViewportWidth(),
						 pCam->getViewportHeight(),
						 pCam->getViewMatrix(),
						 pCam->getProjMatrix(),
						 lineStart,
						 lineEnd);

		XVector3D intersectionPoint;
		XVector3D planeNormal;
		XVector3D pickLineDir = (lineEnd-lineStart).normalize();

		if (m_moveState == X_AXIS)
		{
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			planeNormal = XVector3D(nodeMat.row2);
			if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
				planeNormal = XVector3D(nodeMat.row3);
				
			XF32 time;
			if (XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
				nodePos.x = intersectionPoint.x;
			}
		}
		else if (m_moveState == Y_AXIS)
		{
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			planeNormal = XVector3D(nodeMat.row1);
			if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
				planeNormal = XVector3D(nodeMat.row3);

			XF32 time;
			if (XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
			{
				nodePos.y = intersectionPoint.y;
			}
		}
		else if (m_moveState == Z_AXIS)
		{
			//Where does the pick line intersect the plane of movement? If the pick
			//line is close to being parallel with the plane, use the other plane
			planeNormal = XVector3D(nodeMat.row2);
			if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
				planeNormal = XVector3D(nodeMat.row1);

			XF32 time;
			if (XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time))
				nodePos.z = intersectionPoint.z;			
		}
		else if (m_moveState == XY_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row3);
			
			//Where does the pick line intersect the plane of movement? If it doesn't, then
			//use the other plane of movement for this axis
			XF32 time;
			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			nodePos.x = intersectionPoint.x;
			nodePos.y = intersectionPoint.y;
		}
		else if (m_moveState == XZ_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row2);
			
			//Where does the pick line intersect the plane of movement? If it doesn't, then
			//use the other plane of movement for this axis
			XF32 time;
			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			nodePos.x = intersectionPoint.x;
			nodePos.z = intersectionPoint.z;
		}
		else if (m_moveState == YZ_AXIS)
		{
			planeNormal = XVector3D(nodeMat.row1);
			
			//Where does the pick line intersect the plane of movement? If it doesn't, then
			//use the other plane of movement for this axis
			XF32 time;
			XILinePlane(lineStart,lineEnd,nodePos,planeNormal,intersectionPoint,time);

			nodePos.y = intersectionPoint.y;
			nodePos.z = intersectionPoint.z;
		}

		XVector3D snappedPos = nodePos + m_initialOffset;

		XF32 snapSize = m_pParentMgr->getMoveSnapAmount();
		if (m_pParentMgr->isMoveSnapEnabled())
		{
			snappedPos.x = ((XS32)(snappedPos.x/snapSize) * snapSize);
			snappedPos.y = ((XS32)(snappedPos.y/snapSize) * snapSize);
			snappedPos.z = ((XS32)(snappedPos.z/snapSize) * snapSize);
		}

		m_pClientSceneNode->setModelPosition(snappedPos);
		
		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		worldTransform._41 = snappedPos.x;
		worldTransform._42 = snappedPos.y;
		worldTransform._43 = snappedPos.z;
		m_gizmoController.setTransform(worldTransform);
	}
}


void XGizmoMove::onMouseHover(XSceneNode* pSceneNode)
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
		m_xRidge1SC.setColor(XColor::Red);
		m_xRidge2SC.setColor(XColor::Red);
		m_yRidge1SC.setColor(XColor::Lime);
		m_yRidge2SC.setColor(XColor::Lime);
		m_zRidge1SC.setColor(XColor::Blue);
		m_zRidge2SC.setColor(XColor::Blue);
	}
	else if (pSceneNode == m_pYAxisSN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Yellow);
		m_zAxisSC.setColor(XColor::Blue);
		m_xRidge1SC.setColor(XColor::Red);
		m_xRidge2SC.setColor(XColor::Red);
		m_yRidge1SC.setColor(XColor::Lime);
		m_yRidge2SC.setColor(XColor::Lime);
		m_zRidge1SC.setColor(XColor::Blue);
		m_zRidge2SC.setColor(XColor::Blue);
	}
	else if (pSceneNode == m_pZAxisSN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Yellow);
		m_xRidge1SC.setColor(XColor::Red);
		m_xRidge2SC.setColor(XColor::Red);
		m_yRidge1SC.setColor(XColor::Lime);
		m_yRidge2SC.setColor(XColor::Lime);
		m_zRidge1SC.setColor(XColor::Blue);
		m_zRidge2SC.setColor(XColor::Blue);
	}
	else if (pSceneNode == m_pXRidge1SN || pSceneNode == m_pYRidge1SN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
		m_xRidge1SC.setColor(XColor::Yellow);
		m_xRidge2SC.setColor(XColor::Red);
		m_yRidge1SC.setColor(XColor::Yellow);
		m_yRidge2SC.setColor(XColor::Lime);
		m_zRidge1SC.setColor(XColor::Blue);
		m_zRidge2SC.setColor(XColor::Blue);
	}
	else if (pSceneNode == m_pXRidge2SN || pSceneNode == m_pZRidge2SN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
		m_xRidge1SC.setColor(XColor::Red);
		m_xRidge2SC.setColor(XColor::Yellow);
		m_yRidge1SC.setColor(XColor::Lime);
		m_yRidge2SC.setColor(XColor::Lime);
		m_zRidge1SC.setColor(XColor::Blue);
		m_zRidge2SC.setColor(XColor::Yellow);
	}
	else if (pSceneNode == m_pYRidge2SN || pSceneNode == m_pZRidge1SN)
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
		m_xRidge1SC.setColor(XColor::Red);
		m_xRidge2SC.setColor(XColor::Red);
		m_yRidge1SC.setColor(XColor::Lime);
		m_yRidge2SC.setColor(XColor::Yellow);
		m_zRidge1SC.setColor(XColor::Yellow);
		m_zRidge2SC.setColor(XColor::Blue);
	}
	else
	{
		m_xAxisSC.setColor(XColor::Red);
		m_yAxisSC.setColor(XColor::Lime);
		m_zAxisSC.setColor(XColor::Blue);
		m_xRidge1SC.setColor(XColor::Red);
		m_xRidge2SC.setColor(XColor::Red);
		m_yRidge1SC.setColor(XColor::Lime);
		m_yRidge2SC.setColor(XColor::Lime);
		m_zRidge1SC.setColor(XColor::Blue);
		m_zRidge2SC.setColor(XColor::Blue);
	}
}


void XGizmoMove::onInstanceMoved(XVector3D& newPosition)
{
	if (m_pClientSceneNode)
	{
		XVector3D nodePos;
		m_pClientSceneNode->getWorldPosition(nodePos);

		XMatrix4x4 worldTransform = m_pClientSceneNode->getWorldTransform();
		m_gizmoController.setTransform(worldTransform);
	}
}


XVector3D XGizmoMove::getPosition()
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


XBOOL XGizmoMove::contains(XSceneNode* pSceneNode)
{
	return m_model.containsNode(pSceneNode);
}


