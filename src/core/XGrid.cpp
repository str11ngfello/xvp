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
//	Module Name:	XGrid.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-31-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
//*****************************************************************************
XGrid::XGrid()
{
	for (XU32 i = 0;i < MAX_GRID_LEVELS;++i)
		m_vbHandle[i] = X_INVALID_HANDLE;
	m_centerLinesVB = X_INVALID_HANDLE;
	m_matHandle = X_INVALID_HANDLE;
	m_majorLine = 10;
	m_numCellsAcross = 10;
	m_gridCellWidth = 1.0f;
	m_cameraID = 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGrid::init(XF32 gridCellWidth,XU32 numCellsAcross,XU32 majorLine)
{
	destroy();

	//X_ASSERTM(gridCellWidth != 0,"Unit size cannot be 0");
	X_ASSERTM(numCellsAcross >= 2,"Cannot create a grid less that 2 base units wide");
	X_ASSERTM(numCellsAcross % 2 == 0,"Grid width must be an even number");
	X_ASSERTM(majorLine != 0,"Major line cannot be 0");

	m_majorLine = majorLine;
	m_numCellsAcross = numCellsAcross;
	m_gridCellWidth = gridCellWidth;
	m_vbHandle[0] = generateGrid(gridCellWidth,numCellsAcross);

	XVertexDesc gridVertDesc;
	gridVertDesc.numColors = 1;
	XVP::GraphicsMgr.createVertexBuffer(gridVertDesc,X_PT_LINE_LIST,4,true,m_centerLinesVB);
	XVertexBuffer* pVB = XVP::GraphicsMgr.lockVertexBuffer(m_centerLinesVB,X_LT_DISCARD);
	pVB->setPosition(0,XVector3D((XF32)gridCellWidth*numCellsAcross/2.0f,0.0f,0));
	pVB->setPosition(1,XVector3D((XF32)-gridCellWidth*numCellsAcross/2.0f,0.0f,0));
	pVB->setPosition(2,XVector3D(0,0.0f,(XF32)gridCellWidth*numCellsAcross/2.0f));
	pVB->setPosition(3,XVector3D(0,0.0f,(XF32)-gridCellWidth*numCellsAcross/2.0f));
	pVB->setColor(0,0,0xFF000000);
	pVB->setColor(1,0,0xFF000000);
	pVB->setColor(2,0,0xFF000000);
	pVB->setColor(3,0,0xFF000000);
	XVP::GraphicsMgr.unlockVertexBuffer(m_centerLinesVB);


	XVP::GraphicsMgr.createMaterial(NULL,m_matHandle);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGrid::destroy()
{
	for (XU32 i = 0;i < MAX_GRID_LEVELS;++i)
		if (m_vbHandle[i] != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.destroyVertexBuffer(m_vbHandle[i]);
			m_vbHandle[i] = X_INVALID_HANDLE;
		}
	if (m_centerLinesVB != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyVertexBuffer(m_centerLinesVB);
	if (m_matHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyMaterial(m_matHandle);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XGrid::getTypeDesc()
{
	//Declare a static description string for this node.  The base
	//node class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XGrid";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XGrid::attachShaderController(XS8* pMaterialName,XShaderController* pMatController)
{
	XVP::GraphicsMgr.getMaterialInterface(m_matHandle)->attachShaderController(pMatController);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGrid::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//pScene->addPreRenderEvent(this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGrid::prepForRender(XF32 dt,XScene* pScene)
{
	//Render only if render is allowed
	if (X_SNP_RENDER & m_propertyFlags)
	{
		if (pScene->getActiveCamera()->getID() != m_cameraID)
			return X_OK;

		pScene->addRenderEvent(this,NULL,m_matHandle);
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGrid::render(XRenderEvent* pRenderEvent)
{
	XVP::GraphicsMgr.renderVertexBuffer(m_vbHandle[0]);
	XVP::GraphicsMgr.renderVertexBuffer(m_centerLinesVB);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XHandle XGrid::generateGrid(XF32 unitSize,XU32 widthInUnits)
{
	X_ASSERTM(widthInUnits >= 2,"Cannot create a grid less that 2 base units wide");
	X_ASSERTM(widthInUnits % 2 == 0,"Grid width must be an even number");

	XS32 signedWidth = (XS32)widthInUnits;
	XHandle vbHandle = X_INVALID_HANDLE;

	XVertexDesc gridVertDesc;
	gridVertDesc.numColors = 1;
	XVP::GraphicsMgr.createVertexBuffer(gridVertDesc,X_PT_LINE_LIST,(signedWidth+1)*4,true,vbHandle);
	XVertexBuffer* pVB = XVP::GraphicsMgr.lockVertexBuffer(vbHandle,X_LT_DISCARD);
	XF32 halfUnit = unitSize * .5f;
	for (XS32 i = -signedWidth;i <= signedWidth;i+=2)
	{
		pVB->setPosition(i+signedWidth,XVector3D((XF32)(i*halfUnit),0.0f,(XF32)signedWidth*halfUnit));
		pVB->setPosition(i+signedWidth+1,XVector3D((XF32)(i*halfUnit),0.0f,(XF32)-signedWidth*halfUnit));

		if (i == 0)
		{
			pVB->setColor(i+signedWidth,0,0xFF000000);
			pVB->setColor(i+signedWidth+1,0,0xFF000000);
		}
		else
		{
			if (abs(i) % (m_majorLine*2) == 0)
			{
				pVB->setColor(i+signedWidth,0,0xFF727272);
				pVB->setColor(i+signedWidth+1,0,0xFF727272);
			}
			else
			{
				pVB->setColor(i+signedWidth,0,0xFFC2C2C2);
				pVB->setColor(i+signedWidth+1,0,0xFFC2C2C2);
			}
		}
		
	}
	for (XS32 i = -signedWidth;i <= signedWidth;i+=2)
	{
		pVB->setPosition(i+signedWidth+(signedWidth*2+2),XVector3D((XF32)signedWidth*halfUnit,0.0f,(XF32)(i*halfUnit)));
		pVB->setPosition(i+signedWidth+(signedWidth*2+2)+1,XVector3D((XF32)-signedWidth*halfUnit,0.0f,(XF32)(i*halfUnit)));

		if (i == 0)
		{
			pVB->setColor(i+signedWidth+(signedWidth*2+2),0,0xFF000000);
			pVB->setColor(i+signedWidth+(signedWidth*2+2)+1,0,0xFF000000);
		}
		else
		{
			if (abs(i) % (m_majorLine*2) == 0)
			{
				pVB->setColor(i+signedWidth+(signedWidth*2+2),0,0xFF727272);
				pVB->setColor(i+signedWidth+(signedWidth*2+2)+1,0,0xFF727272);
			}
			else
			{
				pVB->setColor(i+signedWidth+(signedWidth*2+2),0,0xFFC2C2C2);
				pVB->setColor(i+signedWidth+(signedWidth*2+2)+1,0,0xFFC2C2C2);
			}
			
		}
	}

	XVP::GraphicsMgr.unlockVertexBuffer(vbHandle);

	return vbHandle;
}



//*****************************************************************************
//*****************************************************************************
XS32 XGrid::getCameraID()
{
	return m_cameraID;
}
	


//*****************************************************************************
//*****************************************************************************
void XGrid::setCameraID(XS32 cameraID)
{
	m_cameraID = cameraID;
}


XF32 XGrid::getGridCellWidth()
{
	return m_gridCellWidth;
}

XU32 XGrid::getNumCellsAcross()
{
	return m_numCellsAcross;
}

XU32 XGrid::getMajorLine()
{
	return m_majorLine;
}