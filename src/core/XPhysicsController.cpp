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
//	Module Name:	XPhysicsController.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-31-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Default constructor for the physics controller.  Initializes all velocities and 
accelerations to 0.  All constraints, such as maximum and minimum velocities
as well as acceleration calculations, are disabled.
*/
//*****************************************************************************
XPhysicsController::XPhysicsController()
{
	m_scaledRightVel = 0;
	m_scaledUpVel = 0;
	m_scaledLookVel = 0;
	m_scaledYawVel = 0;
	m_scaledPitchVel = 0;
	m_scaledRollVel = 0;
	m_targetRightVel = 0;
	m_targetUpVel = 0;
	m_targetLookVel = 0;
	m_targetYawVel = 0;
	m_targetPitchVel = 0;
	m_targetRollVel = 0;
	m_rightAccel = 0;
	m_upAccel = 0;
	m_lookAccel = 0;
	m_yawAccel = 0;
	m_pitchAccel = 0;
	m_rollAccel = 0;
	m_currentRightVel = 0;
	m_currentUpVel = 0;
	m_currentLookVel = 0;
	m_currentYawVel = 0;
	m_currentPitchVel = 0;
	m_currentRollVel = 0;

	m_lookVector = XVector3D(0,0,1);
	m_rightVector = XVector3D(1,0,0);
	m_upVector = XVector3D(0,1,0);
	m_posVector = XVector3D(0,0,0);

	m_updateCallback = NULL;
	m_pUserCallbackData = NULL;
}



//*****************************************************************************
/**
Destructor for the physics controller.
*/
//*****************************************************************************
XPhysicsController::~XPhysicsController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XPhysicsController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the physics controller.  This is the
description that appears in log files and other debug output.

@return A string description of the physics controller.
*/
//*****************************************************************************
XS8* XPhysicsController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XPhysicsController";
	return description; 
}



//*****************************************************************************
/**
Updates a physics controller.  This gives the controller a chance to update
any internal, time-dependent data.  This function is called by X-VP
once per frame.

@param dt	The amount of time, in seconds, that has passed since
					the last update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XPhysicsController::update(XF32 dt)
{
	//First lets' see if accelerations have been enabled for this object.
	//If so, then we need to update the current velocities with the
	//appropriate acceleration.  If there is no acceleration, then just
	//set the current velocity to the target velocity.
	if (m_rightAccel != 0)
	{
		if (m_currentRightVel < m_targetRightVel)
		{
			m_currentRightVel += m_rightAccel * dt;
			if (m_currentRightVel > m_targetRightVel)
				m_currentRightVel = m_targetRightVel;
		}
		if (m_currentRightVel > m_targetRightVel)
		{
			m_currentRightVel += -m_rightAccel * dt;
			if (m_currentRightVel < m_targetRightVel)
				m_currentRightVel = m_targetRightVel;
		}
	}
	else
		m_currentRightVel = m_targetRightVel;

	if (m_upAccel != 0)
	{
		if (m_currentUpVel < m_targetUpVel)
		{
			m_currentUpVel += m_upAccel * dt;
			if (m_currentUpVel > m_targetUpVel)
				m_currentUpVel = m_targetUpVel;
		}
		if (m_currentUpVel > m_targetUpVel)
		{
			m_currentUpVel += -m_upAccel * dt;
			if (m_currentUpVel < m_targetUpVel)
				m_currentUpVel = m_targetUpVel;
		}
	}
	else
		m_currentUpVel = m_targetUpVel;

	if (m_lookAccel != 0)
	{
		if (m_currentLookVel < m_targetLookVel)
		{		
			m_currentLookVel += m_lookAccel * dt;
			if (m_currentLookVel > m_targetLookVel)
				m_currentLookVel = m_targetLookVel;
		}

		if (m_currentLookVel > m_targetLookVel)
		{
			m_currentLookVel += -m_lookAccel * dt;
			if (m_currentLookVel < m_targetLookVel)
				m_currentLookVel = m_targetLookVel;
		}
	}
	else
		m_currentLookVel = m_targetLookVel;

	if (m_yawAccel != 0)
	{
		if (m_currentYawVel < m_targetYawVel)
		{
			m_currentYawVel += m_yawAccel * dt;
			if (m_currentYawVel > m_targetYawVel)
				m_currentYawVel = m_targetYawVel;
		}
		if (m_currentYawVel > m_targetYawVel)
		{
			m_currentYawVel += -m_yawAccel * dt;
			if (m_currentYawVel < m_targetYawVel)
				m_currentYawVel = m_targetYawVel;
		}
	}
	else
		m_currentYawVel = m_targetYawVel;

	if (m_pitchAccel != 0)
	{
		if (m_currentPitchVel < m_targetPitchVel)
		{
			m_currentPitchVel += m_pitchAccel * dt;
			if (m_currentPitchVel > m_targetPitchVel)
				m_currentPitchVel = m_targetPitchVel;
		}
		if (m_currentPitchVel > m_targetPitchVel)
		{
			m_currentPitchVel += -m_pitchAccel * dt;
			if (m_currentPitchVel < m_targetPitchVel)
				m_currentPitchVel = m_targetPitchVel;
		}
	}
	else
		m_currentPitchVel = m_targetPitchVel;

	if (m_rollAccel != 0)
	{
		if (m_currentRollVel < m_targetRollVel)
		{
			m_currentRollVel += m_rollAccel * dt;
			if (m_currentRollVel > m_targetRollVel)
				m_currentRollVel = m_targetRollVel;
		}
		if (m_currentRollVel > m_targetRollVel)
		{
			m_currentRollVel += -m_rollAccel * dt;
			if (m_currentRollVel < m_targetRollVel)
				m_currentRollVel = m_targetRollVel;
		}
	}
	else
		m_currentRollVel = m_targetRollVel;

	
	//Now make sure that we update the scaled velocites so that
	//when someone references them, they are up to date with the
	//above calculations based on the passed in dt value.
	m_scaledRightVel = m_currentRightVel * dt;
	m_scaledUpVel = m_currentUpVel * dt;
	m_scaledLookVel = m_currentLookVel * dt;
	m_scaledYawVel = m_currentYawVel * dt;
	m_scaledPitchVel = m_currentPitchVel * dt;
	m_scaledRollVel = m_currentRollVel * dt;



		XMatrix4x4 m_rotMatrix;

	//Calculate the yaw rotation and use it to rotate the look and
	//right vectors around the up vector.
	m_rotMatrix.createArbRotation(m_upVector,m_scaledYawVel);
	m_lookVector = m_rotMatrix * m_lookVector;
	m_rightVector = m_rotMatrix * m_rightVector;

	//Calculate the pitch rotation and use it to rotate the look and
	//up vectors around the right vector.
	m_rotMatrix.createArbRotation(m_rightVector,m_scaledPitchVel);
	m_lookVector = m_rotMatrix * m_lookVector;
	m_upVector = m_rotMatrix * m_upVector;

	//Calculate the roll rotation and use it to rotate the up and
	//right vectors around the look vector.
	m_rotMatrix.createArbRotation(m_lookVector,m_scaledRollVel);
	m_upVector = m_rotMatrix * m_upVector;	
	m_rightVector = m_rotMatrix * m_rightVector;

	XVector3D saveVec = m_posVector;
	
	//Calculate the position of the object using the velocity vector.
	m_posVector += m_scaledRightVel * m_rightVector;
	m_posVector += m_scaledUpVel * m_upVector;
	m_posVector += m_scaledLookVel * m_lookVector;

	//if (m_updateCallback)
	//	m_updateCallback(saveVec,m_posVector,this,pNode,m_pUserCallbackData);
	


	return X_OK;
}



//*****************************************************************************
/**
Updates a scene node during the update pass of the scene graph.  This function
is called during the update pass of the scene graph by a scene node that has
this physics controller attached.  

@param pNode	A pointer to the scene node requesting an update.
@param pParentNode	Pointer to the parent node of the scene node that wants
					to be updated.
@param pMatrixStack Pointer to the current matrix stack of the scene graph.
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XPhysicsController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,
														 XSceneNode* pParentNode,
														 XFreeArray<XMatrix4x4>* pMatrixStack)
{
	XMatrix4x4 matrix;//,ymatrix,zmatrix;
	matrix._11 = m_rightVector.x; matrix._21 = m_upVector.x; matrix._31 = m_lookVector.x;
	matrix._12 = m_rightVector.y; matrix._22 = m_upVector.y; matrix._32 = m_lookVector.y;
	matrix._13 = m_rightVector.z; matrix._23 = m_upVector.z; matrix._33 = m_lookVector.z;
	matrix._41 = m_posVector.x; 
	matrix._42 = m_posVector.y;
	matrix._43 = m_posVector.z;
	//xmatrix.createXRotation(m_scaledPitchVel);
	//ymatrix.createYRotation(m_scaledYawVel);
	//zmatrix.createZRotation(m_scaledRollVel);

	
	pNode->setModelTransform(/*pNode->getInvObjTransform() */ matrix); 

	return X_OK;
}



//*****************************************************************************
/**
Registers a user callback to be called before this controller updates 
any scene nodes it is attached to, but after the physics controller has been
updated.  This allows gives the user a chance to accept or reject the 
new position and orientation information or to change it.

@param callback	The user callback.
@return None.
*/
//*****************************************************************************
void XPhysicsController::registerUpdateCallback(void (*callback) (XVector3D& originalPos, XVector3D& destinationPos,
																	XPhysicsController* pPC,XSceneNode* pNode,
																	void* pUserData))
{
	m_updateCallback = callback;
}



//*****************************************************************************
/**
Sets callback data to be passed to the user's callback function.

@param pData	User data.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setUserCallbackData(void* pData)
{
	m_pUserCallbackData = pData;
}



//*****************************************************************************
/**
Sets the yaw acceleration.  The yaw acceleration only takes affect if it has
been enabled with enableYawAccel(). The yaw acceleration is used to reach the
target yaw velocity and should always be positive.  Depending on the target
velocity, the acceleration will be automatically changed to negative or positive
when necessary.  Therefore, negative values are not valid.

@param acceleration	The yaw acceleration to use in yaw velocity calculations.
					Negative values are not valid.  This value is in world
					units per second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setYawAccel(XF32 acceleration)
{
	//Set the data.
	m_yawAccel = acceleration;
}
	


//*****************************************************************************
/**
Sets the pitch acceleration.  The pitch acceleration only takes affect if it has
been enabled with enablePitchAccel(). The pitch acceleration is used to reach the
target pitch velocity and should always be positive.  Depending on the target
velocity, the acceleration will be automatically changed to negative or positive
when necessary.  Therefore, negative values are not valid.

@param acceleration	The pitch acceleration to use in pitch velocity calculations.
					Negative values are not valid.  This value is in world
					units per second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setPitchAccel(XF32 acceleration)
{
	//Set the data.
	m_pitchAccel = acceleration;
}
	


//*****************************************************************************
/**
Sets the roll acceleration.  The roll acceleration only takes affect if it has
been enabled with enableRollAccel(). The roll acceleration is used to reach the
target roll velocity and should always be positive.  Depending on the target
velocity, the acceleration will be automatically changed to negative or positive
when necessary.  Therefore, negative values are not valid.

@param acceleration	The roll acceleration to use in roll velocity calculations.
					Negative values are not valid.  This value is in world
					units per second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setRollAccel(XF32 acceleration)
{
	//Set the data.
	m_rollAccel = acceleration;
}



//*****************************************************************************
/**
Sets the right acceleration.  The right acceleration only takes affect if it has
been enabled with enableRightAccel(). The right acceleration is used to reach the
target right velocity and should always be positive.  Depending on the target
velocity, the acceleration will be automatically changed to negative or positive
when necessary.  Therefore, negative values are not valid.

@param acceleration	The right acceleration to use in right velocity calculations.
					Negative values are not valid.  This value is in world
					units per second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setRightAccel(XF32 acceleration)
{
	//Set the data.
	m_rightAccel = acceleration;
}



//*****************************************************************************
/**
Sets the up acceleration.  The up acceleration only takes affect if it has
been enabled with enableUpAccel(). The up acceleration is used to reach the
target up velocity and should always be positive.  Depending on the target
velocity, the acceleration will be automatically changed to negative or positive
when necessary.  Therefore, negative values are not valid.

@param acceleration	The up acceleration to use in up velocity calculations.
					Negative values are not valid.  This value is in world
					units per second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setUpAccel(XF32 acceleration)
{
	//Set the data.
	m_upAccel = acceleration;
}



//*****************************************************************************
/**
Sets the look acceleration.  The look acceleration only takes affect if it has
been enabled with enableLookAccel(). The look acceleration is used to reach the
target look velocity and should always be positive.  Depending on the target
velocity, the acceleration will be automatically changed to negative or positive
when necessary.  Therefore, negative values are not valid.

@param acceleration	The look acceleration to use in look velocity calculations.
					Negative values are not valid.  This value is in world
					units per second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setLookAccel(XF32 acceleration)
{
	//Set the data.
	m_lookAccel = acceleration;
}



//*****************************************************************************
/**
Sets the target right velocity.  If right acceleration is not enabled, the
right velocity is immediately set to the passed in value.  However, if right
acceleration is enabled, the calculations will use the acceleration to
reach the passed in value gradually over time.

@param velocity	The target right velocity.  This value is in world units per
				second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTargetRightVel(XF32 velocity)
{
	//Set the data.
	m_targetRightVel = velocity;
}



//*****************************************************************************
/**
Sets the target up velocity.  If up acceleration is not enabled, the
up velocity is immediately set to the passed in value.  However, if up
acceleration is enabled, the calculations will use the acceleration to
reach the passed in value gradually over time.

@param velocity	The target up velocity. This value is in world units per
				second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTargetUpVel(XF32 velocity)
{
	//Set the data.
	m_targetUpVel = velocity;
}



//*****************************************************************************
/**
Sets the target look velocity.  If look acceleration is not enabled, the
look velocity is immediately set to the passed in value.  However, if look
acceleration is enabled, the calculations will use the acceleration to
reach the passed in value gradually over time.

@param velocity	The target look velocity. This value is in world units per
				second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTargetLookVel(XF32 velocity)
{
	//Set the data.
	m_targetLookVel = velocity;
}



//*****************************************************************************
/**
Sets the target yaw velocity.  If yaw acceleration is not enabled, the
yaw velocity is immediately set to the passed in value.  However, if yaw
acceleration is enabled, the calculations will use the acceleration to
reach the passed in value gradually over time.

@param velocity	The target yaw velocity. This value is in world units per
				second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTargetYawVel(XF32 velocity)
{
	//Set the data.
	m_targetYawVel = velocity;
}



//*****************************************************************************
/**
Sets the target pitch velocity.  If pitch acceleration is not enabled, the
pitch velocity is immediately set to the passed in value.  However, if pitchpitch
acceleration is enabled, the calculations will use the acceleration to
reach the passed in value gradually over time.

@param velocity	The target pitch velocity. This value is in world units per
				second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTargetPitchVel(XF32 velocity)
{
	//Set the data.
	m_targetPitchVel = velocity;
}



//*****************************************************************************
/**
Sets the target roll velocity.  If roll acceleration is not enabled, the
roll velocity is immediately set to the passed in value.  However, if rollroll
acceleration is enabled, the calculations will use the acceleration to
reach the passed in value gradually over time.

@param velocity	The target roll velocity. This value is in world units per
				second.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTargetRollVel(XF32 velocity)
{
	//Set the data.
	m_targetRollVel = velocity;
}



//*****************************************************************************
/**
Gets the yaw acceleration.

@return The yaw acceleration.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getYawAccel()
{
	//Return the data.
	return m_yawAccel;	
}

	

//*****************************************************************************
/**
Gets the pitch acceleration.

@return The pitch acceleration.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getPitchAccel()
{
	//Return the data.
	return m_pitchAccel;	
}
	
	

//*****************************************************************************
/**
Gets the roll acceleration.

@return The roll acceleration.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getRollAccel()
{
	//Return the data.
	return m_rollAccel;	
}



//*****************************************************************************
/**
Gets the right acceleration.

@return The right acceleration.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getRightAccel()
{
	//Return the data.
	return m_rightAccel;	
}



//*****************************************************************************
/**
Gets the up acceleration.

@return The up acceleration.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getUpAccel()
{
	//Return the data.
	return m_upAccel;	
}



//*****************************************************************************
/**
Gets the look acceleration.

@return The look acceleration.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getLookAccel()
{
	//Return the data.
	return m_lookAccel;	
}



//*****************************************************************************
/**
Gets the target right velocity.

@return The target right velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getTargetRightVel()
{
	//Return the data.
	return m_targetRightVel;	
}



//*****************************************************************************
/**
Gets the target up velocity.

@return The target up velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getTargetUpVel()
{
	//Return the data.
	return m_targetUpVel;	
}



//*****************************************************************************
/**
Gets the target look velocity.

@return The target look velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getTargetLookVel()
{
	//Return the data.
	return m_targetLookVel;	
}



//*****************************************************************************
/**
Gets the target yaw velocity.

@return The target yaw velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getTargetYawVel()
{
	//Return the data.
	return m_targetYawVel;	
}



//*****************************************************************************
/**
Gets the target pitch velocity.

@return The target pitch velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getTargetPitchVel()
{
	//Return the data.
	return m_targetPitchVel;	
}



//*****************************************************************************
/**
Gets the target roll velocity.

@return The target roll velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getTargetRollVel()
{
	//Return the data.
	return m_targetRollVel;	
}



//*****************************************************************************
/**
Gets the scaled right velocity.

@return The scaled right velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getScaledRightVel()
{
	//Return the data.
	return m_scaledRightVel;	
}



//*****************************************************************************
/**
Gets the scaled up velocity.

@return The scaled up velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getScaledUpVel()
{
	//Return the data.
	return m_scaledUpVel;	
}



//*****************************************************************************
/**
Gets the scaled look velocity.

@return The scaled look velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getScaledLookVel()
{
	//Return the data.
	return m_scaledLookVel;	
}



//*****************************************************************************
/**
Gets the scaled yaw velocity.

@return The scaled yaw velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getScaledYawVel()
{
	//Return the data.
	return m_scaledYawVel;	
}



//*****************************************************************************
/**
Gets the scaled pitch velocity.

@return The scaled pitch velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getScaledPitchVel()
{
	//Return the data.
	return m_scaledPitchVel;	
}



//*****************************************************************************
/**
Gets the scaled roll velocity.

@return The scaled roll velocity.  This value is in world units per second.
*/
//*****************************************************************************
XF32 XPhysicsController::getScaledRollVel()
{
	//Return the data.
	return m_scaledRollVel;	
}



//*****************************************************************************
/**
Sets the position.

@param position The position.  This position will be relative to the scene
				node's parent.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setPosition(XVector3D position)
{
	m_posVector = position;
}




//*****************************************************************************
/**
Gets the position.

@return The position relative to the scene node's parent.
*/
//*****************************************************************************
XVector3D XPhysicsController::getPosition()
{
	return m_posVector;
}



//*****************************************************************************
/**
Sets the entire 4x4 transform the physics controller will use when updating
any nodes it is attached to.

@param transform	The 4x4 transform matrix to set.
@return None.
*/
//*****************************************************************************
void XPhysicsController::setTransform(XMatrix4x4& transform)
{
	m_posVector = XVector3D(transform._41,transform._42,transform._43);
	m_lookVector = XVector3D(transform._31,transform._32,transform._33);
	m_upVector = XVector3D(transform._21,transform._22,transform._23);
	m_rightVector = XVector3D(transform._11,transform._12,transform._13);
}



//*****************************************************************************
/**
Gets the physics controllers transform used to transform nodes.

@param transform	A variable used to hold the returned transform. 
*/
//*****************************************************************************
void XPhysicsController::getTransform(XMatrix4x4& transform)
{
	transform._11 = m_rightVector.x; transform._21 = m_upVector.x; transform._31 = m_lookVector.x;
	transform._12 = m_rightVector.y; transform._22 = m_upVector.y; transform._32 = m_lookVector.y;
	transform._13 = m_rightVector.z; transform._23 = m_upVector.z; transform._33 = m_lookVector.z;
	transform._41 = m_posVector.x; 
	transform._42 = m_posVector.y;
	transform._43 = m_posVector.z;
}



