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
//	Module Name:	XGizmoMgr.h
//	Author:			Lynn Duke
//	Creation Date:	4-18-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GIZMO_MGR_H__
#define __X_GIZMO_MGR_H__




enum X_GIZMO_MODE
{
	X_GM_NONE,
	X_GM_MOVE,
	X_GM_ROTATE,
	X_GM_SCALE
};


//*****************************************************************************
//*****************************************************************************
class XGizmoMgr
{
private:

	#define MAX_VIEWPORTS	4

	X_GIZMO_MODE	m_gizmoMode;
	XGizmo*			m_pCurrentGizmos[MAX_VIEWPORTS];
	XScene*			m_pScene;
	
	//Gizmos
	static XModelTemplate			m_moveModelTemplate;
	static XModelTemplate			m_rotateModelTemplate;
	static XModelTemplate			m_scaleModelTemplate;
	XGizmoMove		m_gizmoMove[MAX_VIEWPORTS];
	XGizmoRotate	m_gizmoRotate[MAX_VIEWPORTS];
	XGizmoScale		m_gizmoScale[MAX_VIEWPORTS];

	XGizmo*			m_pActiveGizmo;

	
	XBOOL			m_moveSnapEnabled;
	XBOOL			m_rotateSnapEnabled;
	XBOOL			m_scaleSnapEnabled;
	XF32			m_moveSnapAmount;
	XF32			m_rotateSnapAmount;
	XF32			m_scaleSnapAmount;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGizmoMgr();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus initStaticData();
	XStatus init(XScene* pScene);
	static XStatus destroyStaticData();
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus updateHover(XS32 x,XS32 y);
	XStatus updateDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY);
	XStatus onMouseDown(XSceneNode* pGizmoNode,XS32 screenX,XS32 screenY);
	XStatus onMouseUp();
	XStatus onMouseOver(XF32 x,XF32 y);
	XStatus setMode(X_GIZMO_MODE mode);
	XStatus setActiveGizmo(XGizmo* pGizmo);
	XBOOL isGizmo(XSceneNode* pSceneNode);
	XStatus onSelectionChanged();

	XBOOL isMoveSnapEnabled();
	XStatus enableMoveSnap(XBOOL enable);
	XF32 getMoveSnapAmount();
	XStatus setMoveSnapAmount(XF32 amount);

	XBOOL isRotateSnapEnabled();
	XStatus enableRotateSnap(XBOOL enable);
	XF32 getRotateSnapAmount();
	XStatus setRotateSnapAmount(XF32 amount);

	XBOOL isScaleSnapEnabled();
	XStatus enableScaleSnap(XBOOL enable);
	XF32 getScaleSnapAmount();
	XStatus setScaleSnapAmount(XF32 amount);

	XModelTemplate* getMoveModelTemplate();
	XModelTemplate* getRotateModelTemplate();
	XModelTemplate* getScaleModelTemplate();
};

#endif

