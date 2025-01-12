//*****************************************************************************
//*****************************************************************************
//
//	##   ##    ###### ##  ##  #####  ######  ##  ## ###### 
//   ## ##     ##     ### ## ##   ##   ##    ### ## ##     
//    ###      #####  ## ### ##        ##    ## ### #####  
//   ## ##     ##     ##  ## ##  ###   ##    ##  ## ##     
//  ##   ##    ##     ##  ## ##   ##   ##    ##  ## ##     
//  ##   ##    ###### ##  ##  ###### ######  ##  ## ###### 
//
//	X Engine (c) 
//	Copyright 2001. All Rights Reserved.
// 
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	x_3DTileMap.h
//	Author:			Lynn Duke
//	Creation Date:	5-16-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_3D_TILE_MAP_H__
#define __X_3D_TILE_MAP_H__

#include "XSceneNode.h"



struct X_3D_TILE_INFO
{
	char* pModelFilename;
	char* pHeightMapFilename;
	ULONG heightStrength;
};



//*****************************************************************************
//*****************************************************************************
class X_3D_TILE
{
public:
	XMatrix4x4 worldTransform;
	XMatrix4x4 invWorldTransform;
	ULONG rsIndex;
	ULONG vbIndex;
	ULONG ibIndex;
	float heightMap[100][100];
	std::vector<XVector4D> positionList;

	X_3D_TILE();
	~X_3D_TILE();
};



//*****************************************************************************
//*****************************************************************************
class X_3D_TILE_MAP : public XSceneNode
{
private:
	float m_tileWidth;
	float m_tileHeight;
	ULONG m_mapWidthInTiles;
	ULONG m_mapHeightInTiles;
	WORD m_heightMapWidthInPixels;
	WORD m_heightMapHeightInPixels;
	std::vector<X_3D_TILE*> m_tileSet;
	std::list<BYTE> m_renderList;
	XVector3D m_focalPoint;
	ULONG m_rangeInTiles;
	
public:
	X_3D_TILE_MAP();
	~X_3D_TILE_MAP();
	XStatus setFocalPoint(XVector3D focalPoint);
	XStatus loadTileSet();
	float getYValue(float x,float z);
	XStatus update(float timeScale,list<XMatrix4x4>* pMatrixStack);
	XStatus render();
};

#endif