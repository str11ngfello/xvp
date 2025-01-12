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
//	Module Name:	XUFOController.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the first person controller.
*/
//*****************************************************************************
XUFOController::XUFOController()
{
#pragma message("Add support for a different input device like PRZ controller..specifically joysticks")
	m_lookVector = XVector3D(0,0,1);
	m_upVector = XVector3D(0,1,0);
	m_rightVector = XVector3D(1,0,0);
	m_dummyZVelocityVector = XVector3D(0,0,1);
	m_maxPitch = 90;
	m_minPitch = -90;
	m_pitch = 0;
	m_anchored = false;
	m_xFilter.setDampFactor(.5);
	m_xFilter.setSmoothFactor(.5f);
	m_yFilter.setDampFactor(.5);
	m_yFilter.setSmoothFactor(.5f);
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the first person controller.
*/
//*****************************************************************************
XUFOController::~XUFOController()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUFOController::init(XF32 smoothFactor,XF32 dampFactor)
{
	m_xFilter.init(10,smoothFactor,dampFactor);
	m_yFilter.init(10,smoothFactor,dampFactor);
	m_inited = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUFOController::destroy()
{
	if (!m_inited)
		return X_OK;

	m_xFilter.destroy();
	m_yFilter.destroy();

	XController::destroy();

	m_inited = false;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU8 XUFOController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the first person controller.  This is the
description that appears in log files and other debug output.

@return A string description of the first person controller.
*/
//*****************************************************************************
XS8* XUFOController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XUFOController";
	return description; 
}



//*****************************************************************************
/**
Updates a scene node during the update pass of the scene graph.  This function
is called during the update pass of the scene graph by a scene node that has
this first person controller attached.  

@param pNode	A pointer to the scene node requesting an update.
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XUFOController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,
															  XSceneNode* pParentNode,
															  XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//Finally, set the new transform.
	XMatrix4x4 matrix;
	matrix._11 = m_rightVector.x; matrix._21 = m_upVector.x; matrix._31 = m_lookVector.x;
	matrix._12 = m_rightVector.y; matrix._22 = m_upVector.y; matrix._32 = m_lookVector.y;
	matrix._13 = m_rightVector.z; matrix._23 = m_upVector.z; matrix._33 = m_lookVector.z;
	matrix._41 = m_posVector.x; matrix._42 = m_posVector.y;	matrix._43 = m_posVector.z;

	pNode->setModelTransform(matrix);
	
	return X_OK;
}
	  


//*****************************************************************************
/**
Updates a first person controller.  This gives the controller a chance to update
any internal, time-dependent data.  This function is called by X-VP
once per frame.

@param dt	The amount of time, in seconds, that has passed since
					the last update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUFOController::update(XF32 dt)
{			  
	XF32 xDelta = m_xFilter.update((XF32)XVP::InputMgr.getMouseRelXMovement());
	XF32 yDelta = m_yFilter.update((XF32)XVP::InputMgr.getMouseRelYMovement());
	m_pitch += yDelta;
	
	//Limit the pitch 
	if (m_pitch >  m_maxPitch)
	{
		yDelta -= (m_pitch - m_maxPitch);
		m_pitch = m_maxPitch;
	}
	if ( m_pitch < m_minPitch)
	{
		yDelta -= (m_pitch - m_minPitch);
		m_pitch = m_minPitch;
	}
	
	XMatrix4x4 rotMat;
	XVector4D temp4DVec;
	rotMat.createArbRotation(m_rightVector,X_MATH_DEG_TO_RAD(yDelta));	
	m_lookVector = rotMat.transformVector3D(m_lookVector);
	m_upVector = rotMat.transformVector3D(m_upVector);
	rotMat.createArbRotation(XVector3D(0,1,0),X_MATH_DEG_TO_RAD(xDelta));
	m_lookVector = rotMat.transformVector3D(m_lookVector);
	m_upVector = rotMat.transformVector3D(m_upVector);
	m_dummyZVelocityVector = rotMat.transformVector3D(m_dummyZVelocityVector);

	//Prevents small floating point inaccuracies from creeping
	//into the camera's coordinate axis.
	m_lookVector.normalize();
	m_rightVector = m_upVector.cross(m_lookVector);
	m_rightVector.normalize();
	m_upVector = m_lookVector.cross(m_rightVector);
	m_upVector.normalize();
	
	if (!m_anchored)
	{
		m_posVector.x += m_velocity.z * dt * m_lookVector.x;
		m_posVector.y += m_velocity.z * dt * m_lookVector.y;
		m_posVector.z += m_velocity.z * dt * m_lookVector.z;
	}
	else
	{
		m_posVector.x += m_velocity.z * dt * m_dummyZVelocityVector.x;
		m_posVector.z += m_velocity.z * dt * m_dummyZVelocityVector.z;
	}

	m_posVector.x += m_velocity.x * dt * m_rightVector.x;
	m_posVector.y += m_velocity.x * dt * m_rightVector.y;
	m_posVector.z += m_velocity.x * dt * m_rightVector.z;

	m_posVector.x += m_velocity.y * dt * m_upVector.x;
	m_posVector.y += m_velocity.y * dt * m_upVector.y;
	m_posVector.z += m_velocity.y * dt * m_upVector.z;

	m_velocity *= .8f;

	return X_OK;
}



//*****************************************************************************
/**
Sets the smoothness of the movement from the hardware device that is currently
being used to drive the first person controller.

@param smooth factor	The smoothFactor.  See the XSmoothFilter object.
@return None.
*/
//*****************************************************************************
void XUFOController::setSmoothness(XF32 smoothFactor)
{
	m_xFilter.setSmoothFactor(smoothFactor);
	m_yFilter.setSmoothFactor(smoothFactor);
}



//*****************************************************************************
/**
Gets the smoothness of the movement from the hardware device that is currently
being used to drive the first person controller.

@return The smooth factor.  See the XSmoothFilter object.
*/
//*****************************************************************************
XF32 XUFOController::getSmoothness()
{
	return m_xFilter.getSmoothFactor();
}



//*****************************************************************************
/**
Sets the damping of the movement from the hardware device that is currently
being used to drive the first person controller.

@param dampFactor	The damp factor.  See the XSmoothFilter object.
*/
//*****************************************************************************
void XUFOController::setDamping(XF32 dampFactor)
{
	m_xFilter.setDampFactor(dampFactor);
	m_yFilter.setDampFactor(dampFactor);
}



//*****************************************************************************
/**
Gets the damping of the movement from the hardware device that is currently
being used to drive the first person controller.

@return The damp factor.  See the XSmoothFilter object.
*/
//*****************************************************************************
XF32 XUFOController::getDamping()
{
	return m_xFilter.getDampFactor();
}



//*****************************************************************************
/**
Anchors the movement of the scene node such that the scene node does not move
in the y direction.

@return None.
*/
//*****************************************************************************
void XUFOController::enableAnchor()
{
	m_anchored = true;
}



//*****************************************************************************
/**
Disables the anchor.  This allows the scene node to move in any direction which
it is can face.

@return None.
*/
//*****************************************************************************
void XUFOController::disableAnchor()
{
	m_anchored = false;
}



//*****************************************************************************
/**
Determines if the controller is currently anchoring attached scene nodes.

@return True if the controller is anchoring, false otherwise.  By default,
		anchoring is disabled.
*/
//*****************************************************************************
XBOOL XUFOController::isAnchored()
{
	return m_anchored;
}



//*****************************************************************************
/**
Sets the position of attached scene nodes. 

@param position	The position.
@return None.
*/
//*****************************************************************************
void XUFOController::setPosition(XVector3D position)
{
	m_posVector = position;
}



//*****************************************************************************
/**
Gets the position of attached scene nodes. 

@return	The position.
*/
//*****************************************************************************
XVector3D XUFOController::getPosition()
{
	return m_posVector;
}



//*****************************************************************************
/**
Sets the maximum upward pitch the node is allowed to move in degrees.

@param maxPitch	The maximum upward pitch in degrees.
@return None.
*/
//*****************************************************************************
void XUFOController::setMaxPitch(XF32 maxPitch)
{
	m_maxPitch = maxPitch;
}



//*****************************************************************************
/**
Gets the maximum upward pitch the node is allowed to move in degrees.

@return The maximum upward pitch in degrees.
*/
//*****************************************************************************
XF32 XUFOController::getMaxPitch()
{
	return m_maxPitch;
}



//*****************************************************************************
/**
Sets the maximum downward pitch the node is allowed to move in degrees.

@param maxPitch	The maximum downward pitch in degrees.
@return None.
*/
//*****************************************************************************
void XUFOController::setMinPitch(XF32 minPitch)
{
	m_minPitch = minPitch;
}



//*****************************************************************************
/**
Gets the maximum downward pitch the node is allowed to move in degrees.

@return The maximum downward pitch in degrees.
*/
//*****************************************************************************
XF32 XUFOController::getMinPitch()
{
	return m_minPitch;
}



//*****************************************************************************
/**
Sets the right velocity.  This is the velocity in the direction of the local
right transform axis in world units per second.

@param velocity	The right velocity in world units per second.
@return None.
*/
//*****************************************************************************
void XUFOController::setRightVelocity(XF32 velocity)
{
	m_velocity.x = velocity;
}



//*****************************************************************************
/**
Gets the right velocity.  This is the velocity in the direction of the local
right transform axis in world units per second.

@return The right velocity in world units per second.
*/
//*****************************************************************************
XF32 XUFOController::getRightVelocity()
{
	return m_velocity.x;
}



//*****************************************************************************
/**
Sets the up velocity.  This is the velocity in the direction of the local
up transform axis in world units per second.

@param velocity	The up velocity in world units per second.
@return None.
*/
//*****************************************************************************
void XUFOController::setUpVelocity(XF32 velocity)
{
	m_velocity.y = velocity;
}



//*****************************************************************************
/**
Gets the up velocity.  This is the velocity in the direction of the local
up transform axis in world units per second.

@return The up velocity in world units per second.
*/
//*****************************************************************************
XF32 XUFOController::getUpVelocity()
{
	return m_velocity.y;
}



//*****************************************************************************
/**
Sets the look velocity.  This is the velocity in the direction of the local
look transform axis in world units per second.

@param velocity	The look velocity in world units per second.
@return None.
*/
//*****************************************************************************
void XUFOController::setLookVelocity(XF32 velocity)
{
	m_velocity.z = velocity;
}



//*****************************************************************************
/**
Gets the look velocity.  This is the velocity in the direction of the local
look transform axis in world units per second.

@return The look velocity in world units per second.
*/
//*****************************************************************************
XF32 XUFOController::getLookVelocity()
{
	return m_velocity.z;
}
