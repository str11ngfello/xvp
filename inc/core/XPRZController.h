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
//	Module Name:	XPRZController.h
//	Author:			Lynn Duke
//	Creation Date:	5-11-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_NAV_CONTROLLER_H__
#define __X_NAV_CONTROLLER_H__




//*****************************************************************************
//! Look at constraint to force a scene node to face a particluar point in space.
/** 
The XPRZController object is a constraining controller you can attach to
scene nodes to force them to always face towards a particluar point in space.

See below for details.
*/
//*****************************************************************************
class XPRZController : public XController
{
public:
	enum INPUT_DEVICE
	{
		MOUSE,
		GAMEPAD,
		EXTERNAL
	};

private:
	INPUT_DEVICE	m_deviceType;
	XVector3D		m_target;
	XMatrix4x4		m_mat;
	XVector3D		m_panVec;
	XVector3D		m_panVecBuffer;
	XF32			m_xAngle;
	XF32			m_yAngle;
	XF32			m_radius;

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XPRZController();
	~XPRZController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus update(XF32 dt);
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	bool isCompatible(XSceneNode* pNode);
	XVector3D getTarget();
    XStatus setTarget(XVector3D pTarget);
	XStatus handleNodePostUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
									   XFreeArray<XMatrix4x4>* pMatrixStack);

	INPUT_DEVICE getInputDevice();
	XStatus setInputDevice(INPUT_DEVICE device);
	XVector2D getRotation();
	XStatus setRotation(XF32 xAngle,XF32 yAngle);
	XStatus addRotation(XF32 xAngle,XF32 yAngle);
	XF32 getZoom();
	XStatus setZoom(XF32 distance);
	XStatus addZoom(XF32 distance);
	XVector3D getPan();
	XStatus setPan(XVector3D& panVec);
	XStatus addPan(XVector3D& panVec);
	XStatus reset();
};


#endif