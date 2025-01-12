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
//	Module Name:	XPRZController.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-11-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the look at controller.
*/
//*****************************************************************************
XPRZController::XPRZController()
{
	m_deviceType = EXTERNAL;
	reset();
}



//*****************************************************************************
/**
Destructor for the look at controller.
*/
//*****************************************************************************
XPRZController::~XPRZController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XPRZController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_POST_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the look at controller.  This is the
description that appears in log files and other debug output.

@return A string description of the look at controller.
*/
//*****************************************************************************
XS8* XPRZController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XPRZController";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
bool XPRZController::isCompatible(XSceneNode* pNode)
{
	if (pNode)
	{
		X_SCENE_NODE_TYPE snt = pNode->getType();
		return (X_SNT_GET_BASE_SNID(snt) == X_SNID_MODEL_OBJECT) ||
			   (X_SNT_GET_EXT_SNID(snt) == X_SNID_CAMERA);
			
			/*(snt == X_SNT_MODEL_OBJECT) || 
				(snt == X_SNT_BONE) ||
				(snt == X_SNT_CAMERA_OBJECT) ||
				(snt == X_SNT_PARTICLE_EMITTER));*/
	}
	else
		return false;
}


XVector3D XPRZController::getTarget()
{
	return m_target;
}


//*****************************************************************************
/**
Sets the target.  The target is the coordinate in world space where the scene
node will be forced to look at. 

@param pTarget	A pointer to an XVector3D object.  Notice that this parameter
				is a pointer.  Once set, the look at controller will continue
				to poll this pointer for the look at target.  You will need to
				ensure that this pointer stays valid for the length of time
				the controller is using it.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XPRZController::setTarget(XVector3D target)
{
	m_target = target;
	return X_OK;
}


#if 0
//*****************************************************************************
/**
Removes the current target.

@return A return code of type XStatus.
*/
//*****************************************************************************
void XPRZController::removeTarget()
{
	m_pTarget = NULL;
}
#endif



XStatus XPRZController::update(XF32 dt)
{
	//Depending on the device, update the navigation
	switch (m_deviceType)
	{
	case MOUSE:
		//if (XVP::InputMgr.mouseButtonDown(X_MB_MIDDLE))
		//	addPan(XVP::InputMgr.getMouseRelXMovement() * -.1f,XVP::InputMgr.getMouseRelYMovement() * .1f);

		if (XVP::InputMgr.mouseButtonDown(X_MB_RIGHT))
			addZoom((XF32)XVP::InputMgr.getMouseRelYMovement());

		if (XVP::InputMgr.mouseButtonDown(X_MB_LEFT))
			addRotation(XVP::InputMgr.getMouseRelXMovement() * .01f * -1.0f,XVP::InputMgr.getMouseRelYMovement() * .01f);

		//Mouse wheel affects zoom
		addZoom(XVP::InputMgr.getMouseRelZMovement() * -.1f);
			
		break;
	case GAMEPAD:
#pragma message ("Joystick function don't support dual analog sticks.. yet")
				//addPan(XVP::InputMgr.getMouseRelXMovement() * -.1f,XVP::InputMgr.getMouseRelYMovement() * .1f);
				addZoom(XVP::InputMgr.getJoyYExtent(0));
				addRotation(XVP::InputMgr.getJoyXExtent(0) * .05f * -1.0f,XVP::InputMgr.getJoyYExtent(0) * .05f);
		break;
	default:
		//EXTERNAL does nothing.  Input data should be provided externally
		break;
	}

	return X_OK;
}



//*****************************************************************************
/**
Updates a scene node during the update pass of the scene graph.  This function
is called during the update pass of the scene graph by a scene node that has
this look at controller attached.  

Builds a look at matrix to force the scene node to face the target.

@param pNode	A pointer to the scene node requesting an update.
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XPRZController::handleNodePostUpdate(XScene* pScene,XSceneNode* pNode,
														 XSceneNode* pParentNode,
														 XFreeArray<XMatrix4x4>* pMatrixStack)
{
	XVector3D fromPos;
	pNode->getWorldPosition(fromPos);

	//If from and at positions are identical, we can't look at the target exactly.
	//if (fromPos == m_target)
	//	return X_OK;

	//First, get a look at matrix using the world positions.
	XMatrix4x4 lookAtMat = XMatrix4x4::getObjectLookat(fromPos,m_target);
	pNode->setWorldTransform(lookAtMat);
	pMatrixStack->removeBack();

	//Also, we push this nodes matrix on the stack so that children may
	//access it in the same manner.
	XMatrix4x4* pMat = pMatrixStack->insertBack();
	(*pMat) = pNode->getWorldTransform();

	XMatrix4x4 trans;
	trans.createTranslation(m_panVecBuffer);
	m_target = trans * XVector3D(0,0,0) ;//+= m_panVecBuffer;
	//m_target += XVector3D(pNode->getWorldTransform().row1) * m_panVec.x;
	//m_target += XVector3D(pNode->getWorldTransform().row2) * m_panVec.y;
	//m_panVecBuffer.x = m_panVecBuffer.y = m_panVecBuffer.z = 0.0f;

	m_mat._41 = cosf(m_xAngle) * cosf(m_yAngle) * m_radius;
	m_mat._42 = sinf(m_yAngle) * m_radius;
	m_mat._43 = sinf(m_xAngle) * cosf(m_yAngle) * m_radius;
	m_mat._41 += m_panVecBuffer.x;
	m_mat._42 += m_panVecBuffer.y;
	m_mat._43 += m_panVecBuffer.z;

	pNode->setModelTransform(m_mat);
	
	return X_OK;
}



XPRZController::INPUT_DEVICE XPRZController::getInputDevice()
{
	return m_deviceType;
}



XStatus XPRZController::setInputDevice(INPUT_DEVICE device)
{
	m_deviceType = device;
	return X_OK;
}

XVector2D XPRZController::getRotation()
{
	return XVector2D(m_xAngle,m_yAngle);
}

XStatus XPRZController::setRotation(XF32 xAngle,XF32 yAngle)
{
	m_xAngle = xAngle;
	m_yAngle = yAngle;

	if (m_yAngle >= 1.57f)
		m_yAngle = 1.57f;
	if (m_yAngle <= -1.57f)
		m_yAngle = -1.57f;

	return X_OK;
}

XStatus XPRZController::addRotation(XF32 xAngle,XF32 yAngle)
{
	m_xAngle += xAngle;
	m_yAngle += yAngle;	
	if (m_yAngle >= 1.57f)
		m_yAngle = 1.57f;
	if (m_yAngle <= -1.57f)
		m_yAngle = -1.57f;

	return X_OK;
}

XF32 XPRZController::getZoom()
{
	return m_radius;
}

XStatus XPRZController::setZoom(XF32 distance)
{
	//Zoom can never be exactly 0
	if (distance < .00001f)
		m_radius = .00001f;
	else
		m_radius = distance;

	return X_OK;
}


XStatus XPRZController::addZoom(XF32 distance)
{
	m_radius += distance;
	if (m_radius < .00001f)
		m_radius = .00001f;

	return X_OK;
}


XVector3D XPRZController::getPan()
{
	return m_panVecBuffer;
}

XStatus XPRZController::setPan(XVector3D& panVec)
{
	m_panVecBuffer = panVec;
	return X_OK;
}

XStatus XPRZController::addPan(XVector3D& panVec)
{
	//X_DEBUG_LOGF("addPan= %f %f %f \n",X_DLL_NONE,panVec.x,panVec.y,panVec.z);
	m_panVecBuffer += panVec;	
	return X_OK;
}


XStatus XPRZController::reset()
{
	m_panVec = XVector3D(0,0,0);
	m_panVecBuffer = XVector3D(0,0,0);
	m_xAngle = -1.570795f;
	m_yAngle = 0.0f;
	m_radius = 40;
	return X_OK;
}