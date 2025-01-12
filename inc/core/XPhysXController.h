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
//	Module Name:	XPhysXController.h
//	Author:			Lynn Duke
//	Creation Date:	10-7-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PHYSX_CONTROLLER_H__
#define __X_PHYSX_CONTROLLER_H__



//*****************************************************************************
//! A physics object that allows nodes to move in 3D space.
/** 
The XPhysXController object provides a simple and effective means to move
scene nodes around in 3D space using basic physical properties such as linear
velocity and acceleration, rotational velocity and acceleration, etc...
For more complex or custom types of movements, you may want to derive your
own controller to control scene nodes in more specific ways; however, the
XPhysXController object provides a quick way to get things moving.

First of all, the terminology used with the XPhysXController object needs
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
class XPhysXController : public XController
{
protected:
	static NxPhysicsSDK* gPhysicsSDK;
	static NxScene* gScene;
	static NxActor* groundPlane;

	NxActor* box;
	

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XPhysXController();
	~XPhysXController();

	//*************************************************************************
	//Public Interface
	//*************************************************************************
	static XStatus initPhysics();
	static XStatus destroyPhysics();
	static void updatePhysics(XF32 dt);
	static void StartPhysics();
	static void GetPhysicsResults();

	static NxActor* CreateGroundPlane();
	static NxActor* CreateBox(const NxVec3& pos, const NxVec3& boxDim, const NxReal density);
	static NxActor* CreateSphere(const NxVec3& pos, const NxReal radius, const NxReal density);
	static NxActor* CreateCapsule(const NxVec3& pos, const NxReal height, const NxReal radius, const NxReal density);


	XStatus init();
	XStatus destroy();
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	XStatus update(XF32 dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
									   XFreeArray<XMatrix4x4>* pMatrixStack);

	void addForce(XVector3D& forceVec);
};

#endif