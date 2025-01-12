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
//	Module Name:	XGizmoScale.h
//	Author:			Lynn Duke
//	Creation Date:	4-18-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GIZMO_SCALE_H__
#define __X_GIZMO_SCALE_H__




//*****************************************************************************
//*****************************************************************************
class XGizmoScale : public XGizmo
{
private:

	enum MOVE_STATE
	{
		NONE,
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		ALL_AXIS,
	};

	MOVE_STATE				m_moveState;

	XModel					m_model;

	XGizmoController		m_gizmoController;

	XMaterialColorSC	m_xAxisSC;
	XMaterialColorSC	m_yAxisSC;
	XMaterialColorSC	m_zAxisSC;

	XSceneNode*				m_pXAxisSN;
	XSceneNode*				m_pYAxisSN;
	XSceneNode*				m_pZAxisSN;
	XSceneNode*				m_pAllAxisSN;
	XF32					m_initialXScale;
	XF32					m_initialYScale;
	XF32					m_initialZScale;
	XF32					m_initialMouseX;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGizmoScale();

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

