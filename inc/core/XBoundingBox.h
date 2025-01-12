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
//	Module Name:	XBoundingBox.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BOUNDING_BOX_H__
#define __X_BOUNDING_BOX_H__




//*****************************************************************************
//*****************************************************************************
class XBoundingBox
{
public:
	XVector3D Min;
	XVector3D Max;

	XBoundingBox();
	XBoundingBox(XVector3D vert1,XVector3D vert2,XVector3D vert3);
};



//*****************************************************************************
//*****************************************************************************
class XBoundingBoxSide
{
protected:
	XVector3D	Points[4];
	XVector3D	Normal;
	XF32		D;
	XF32		Size;
	int			CurSide;

	XVector3D GetBoxPt(XBoundingBox box, int index);
	bool CoPlanar(XBoundingBoxSide test_side);
	bool PointsInside(XBoundingBoxSide test_side);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBoundingBoxSide();
	~XBoundingBoxSide();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	void SetFromBBox(XBoundingBox box,int side);
	bool Neighbors(XBoundingBoxSide test_side);
	XF32 GetSize();
};

#endif