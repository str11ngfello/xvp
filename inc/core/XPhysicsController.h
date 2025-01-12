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
//	Module Name:	XPhysicsController.h
//	Author:			Lynn Duke
//	Creation Date:	8-31-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PHYSICS_CONTROLLER_H__
#define __X_PHYSICS_CONTROLLER_H__



//*****************************************************************************
//! A physics object that allows nodes to move in 3D space.
/** 
The XPhysicsController object provides a simple and effective means to move
scene nodes around in 3D space using basic physical properties such as linear
velocity and acceleration, rotational velocity and acceleration, etc...
For more complex or custom types of movements, you may want to derive your
own controller to control scene nodes in more specific ways; however, the
XPhysicsController object provides a quick way to get things moving.

First of all, the terminology used with the XPhysicsController object needs
to be explained.  See the terms below:

1. Right - this corresponds to the right transform axis (the x axis)<br>
2. Up - this corresponds to the upwards transform axis (the y axis)<br>
3. Look - this corresponds to the frontward facing transform axis (the z axis)<br>
4. Yaw - this corresponds to rotation around the up transform axis <br>
5. Pitch - this corresponds to rotation around the right transform axis<br> 
6. Roll - this corresponds to rotation around the look transform axis<br>

For example, say that you have a ball that you want to move along its local right (x)
axis.  You can set the "target" right velocity to be positive to move the ball along
the right axis in the positive direction.  On the other hand, you can set the
right velocity to be negative to move the ball along the right axis in the negative
direction.  The same principles apply to rotational velocities...positive rotates
in one direction and negative rotates in the other.

When you want to set a velocity, you specify what is known as the "target" velocity.
This means that, depending on the settings of the controller, the velocity may
not be instantly set.  For example, you can set accelerations along the
different axis and when you set the "target" velocity, calculations take
into account the acceleration and increment accordingly.  However, if you set
a "target" velocity without any enabled accelerations, the velocity will be
immediately set.

All velocities are specified in world units per second. 

See below for details.
*/
//*****************************************************************************
class XPhysicsController : public XController
{
protected:
	//These variables will hold latest scaled deltas determined
	//from the last dt that was passed to the update function.
	XF32		m_scaledRightVel;
	XF32		m_scaledUpVel;
	XF32		m_scaledLookVel;
	XF32		m_scaledYawVel;
	XF32		m_scaledPitchVel;
	XF32		m_scaledRollVel;

	//These variables will hold the actual unscaled velocities,
	//accelerations etc...
	XF32		m_targetRightVel;
	XF32		m_targetUpVel;
	XF32		m_targetLookVel;
	XF32		m_targetYawVel;
	XF32		m_targetPitchVel;
	XF32		m_targetRollVel;
	XF32		m_rightAccel;
	XF32		m_upAccel;
	XF32		m_lookAccel;
	XF32		m_yawAccel;
	XF32		m_pitchAccel;
	XF32		m_rollAccel;
	XF32		m_currentRightVel;
	XF32		m_currentUpVel;
	XF32		m_currentLookVel;
	XF32		m_currentYawVel;
	XF32		m_currentPitchVel;
	XF32		m_currentRollVel;
	XVector3D	m_lookVector;
	XVector3D	m_rightVector;
	XVector3D	m_upVector;
	XVector3D	m_posVector;
	void*		m_pUserCallbackData;
	void		(*m_updateCallback) (XVector3D& originalPos, XVector3D& destinationPos,
									 XPhysicsController* pPC,XSceneNode* pNode,
									 void* pUserData);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XPhysicsController();
	~XPhysicsController();

	//*************************************************************************
	//Public Interface
	//*************************************************************************
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	XStatus update(XF32 dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
									   XFreeArray<XMatrix4x4>* pMatrixStack);
	void registerUpdateCallback(void (*callback) (XVector3D& originalPos, XVector3D& destinationPos,
												  XPhysicsController* pPC,XSceneNode* pNode,
												  void* pUserData));
	void setUserCallbackData(void* pData);
	void setYawAccel(XF32 acceleration);
	void setPitchAccel(XF32 acceleration);
	void setRollAccel(XF32 acceleration);
	void setRightAccel(XF32 acceleration);
	void setUpAccel(XF32 acceleration);
	void setLookAccel(XF32 acceleration);
	void setTargetRightVel(XF32 velocity);
	void setTargetUpVel(XF32 velocity);
	void setTargetLookVel(XF32 velocity);
	void setTargetYawVel(XF32 velocity);
	void setTargetPitchVel(XF32 velocity);
	void setTargetRollVel(XF32 velocity);
	void setVelVec(XVector3D& velocity);
	XF32 getYawAccel();
	XF32 getPitchAccel();
	XF32 getRollAccel();
	XF32 getRightAccel();
	XF32 getUpAccel();
	XF32 getLookAccel();
	XF32 getTargetRightVel();
	XF32 getTargetUpVel();
	XF32 getTargetLookVel();
	XF32 getTargetYawVel();
	XF32 getTargetPitchVel();
	XF32 getTargetRollVel();
	XF32 getScaledRightVel();
	XF32 getScaledUpVel();
	XF32 getScaledLookVel();
	XF32 getScaledYawVel();
	XF32 getScaledPitchVel();
	XF32 getScaledRollVel();
	void setPosition(XVector3D position);
	XVector3D getPosition();
	void setTransform(XMatrix4x4& transform);
	void getTransform(XMatrix4x4& transform);
};

#endif