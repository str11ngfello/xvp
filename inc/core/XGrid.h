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
//	Module Name:	XGrid.h
//	Author:			Lynn Duke
//	Creation Date:	7-31-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GRID_H__
#define __X_GRID_H__



#define MAX_GRID_LEVELS	10

//*****************************************************************************
//*****************************************************************************
class XGrid : public XSceneNode
{
private:
	XHandle	m_vbHandle[MAX_GRID_LEVELS];
	XHandle	m_centerLinesVB;
	XHandle m_matHandle;
	XU32 m_majorLine;
	XU32 m_numCellsAcross;
	XF32 m_gridCellWidth;
	XS32 m_cameraID;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XHandle generateGrid(XF32 unitSize,XU32 widthInUnits);

	
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGrid();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XF32 gridCellWidth,XU32 numCellsAcross,XU32 majorLine);
	XStatus destroy();
	XS8* getTypeDesc();
	XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus prepForRender(XF32 dt,XScene* pScene);
	XStatus render(XRenderEvent* pRenderEvent);
	XS32 getCameraID();
	void setCameraID(XS32 cameraID);
	XF32 getGridCellWidth();
	XU32 getNumCellsAcross();
	XU32 getMajorLine();


};

#endif

