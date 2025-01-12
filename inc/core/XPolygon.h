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
//	Module Name:	XPolygon.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_POLYGON_H__
#define __X_POLYGON_H__






//*****************************************************************************
//! Represents a polygon made of 3 vertices.
/**
The XPolygon object represents a triangle with 3 vertices.       

See the member functions below for more information.
*/
//*****************************************************************************
class XPolygon
{
public:
	union
	{
		struct
		{
			XVector3D vert1,vert2,vert3;
		};
        
		struct
		{
			XVector3D vertArray[3];
		};
    };

	XRay3D plane;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XPolygon();
	XPolygon(XVector3D vert1,XVector3D vert2,XVector3D vert3);
	~XPolygon();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	void calcPlane(XBOOL clockwise = true);
	XBOOL inside(XVector3D p, const XF32 epsilon = (XF32) 0);
	XVector3D calcCenterOfMass();
};



#endif
