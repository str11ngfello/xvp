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
//	Module Name:	XGizmoMove.h
//	Author:			Lynn Duke
//	Creation Date:	4-18-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GIZMO_MOVE_H__
#define __X_GIZMO_MOVE_H__





//*****************************************************************************
//*****************************************************************************
class XGizmoMove : public XGizmo
{
private:

	enum MOVE_STATE
	{
		NONE,
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		XZ_AXIS,
		XY_AXIS,
		YZ_AXIS
	};

	MOVE_STATE				m_moveState;


	XModel					m_model;

	XGizmoController		m_gizmoController;

	XMaterialColorSC	m_xAxisSC;
	XMaterialColorSC	m_yAxisSC;
	XMaterialColorSC	m_zAxisSC;
	XMaterialColorSC	m_xRidge1SC;
	XMaterialColorSC	m_xRidge2SC;
	XMaterialColorSC	m_yRidge1SC;
	XMaterialColorSC	m_yRidge2SC;
	XMaterialColorSC	m_zRidge1SC;
	XMaterialColorSC	m_zRidge2SC;

	XSceneNode*				m_pXAxisSN;
	XSceneNode*				m_pYAxisSN;
	XSceneNode*				m_pZAxisSN;
	XSceneNode*				m_pXRidge1SN;
	XSceneNode*				m_pXRidge2SN;
	XSceneNode*				m_pYRidge1SN;
	XSceneNode*				m_pYRidge2SN;
	XSceneNode*				m_pZRidge1SN;
	XSceneNode*				m_pZRidge2SN;
	XVector3D				m_initialOffset;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGizmoMove();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XGizmoMgr* pParentMgr,XScene* pScene,XU32 viewportIndex);
	virtual XStatus destroy();
	virtual XStatus setActive(XBOOL active);	
	virtual XStatus update(XF32 dt);
	virtual XStatus onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY);
	virtual XStatus onMouseUp();
	virtual void onMouseDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY);
	virtual void onMouseHover(XSceneNode* pSceneNode);
	virtual void onSelectionChanged(XSceneNode* pSceneNode);
	virtual void onInstanceMoved(XVector3D& newPosition);
	virtual XVector3D getPosition();
	virtual XBOOL contains(XSceneNode* pSceneNode);

};

#endif

