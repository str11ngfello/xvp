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
//	Module Name:	XUFOController.h
//	Author:			Lynn Duke
//	Creation Date:	2-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_FIRST_PERSON_CONTROLLER_H__
#define __X_FIRST_PERSON_CONTROLLER_H__




//*****************************************************************************
//! Simulates a "First Person Shooter" style controller.
/** 
The XUFOController object simulates classic first person shooter
style controls.  The controller can attach to a camera (or any scene node for
that matter) and can control the orientation and movement of the camera similar
to FPS games.  

See below for details.
*/
//*****************************************************************************
class XUFOController : public XController
{
private:
	XVector3D m_lookVector;
	XVector3D m_upVector;
	XVector3D m_rightVector;
	XVector3D m_posVector;
	XVector3D m_velocity;
	XVector3D m_dummyZVelocityVector;
	XF32 m_maxPitch;
	XF32 m_minPitch;
	XF32 m_pitch;
	XBOOL m_anchored;
	XSmoothFilter m_xFilter;
	XSmoothFilter m_yFilter;
	XBOOL m_inited;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUFOController();
	~XUFOController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XF32 smoothFactor = 0,XF32 dampFactor = .2f);
	XStatus destroy();
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	XStatus update(XF32 dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
									   XFreeArray<XMatrix4x4>* pMatrixStack);
	void setSmoothness(XF32 smoothFactor);
	XF32 getSmoothness();
	void setDamping(XF32 dampFactor);
	XF32 getDamping();
	void enableAnchor();
	void disableAnchor();
	XBOOL isAnchored();
	void setPosition(XVector3D position);
	XVector3D getPosition();
	void setMaxPitch(XF32 maxPitch);
	XF32 getMaxPitch();
	void setMinPitch(XF32 minPitch);
	XF32 getMinPitch();
	void setRightVelocity(XF32 velocity);
	XF32 getRightVelocity();
	void setUpVelocity(XF32 velocity);
	XF32 getUpVelocity();
	void setLookVelocity(XF32 velocity);
	XF32 getLookVelocity();
};

#endif