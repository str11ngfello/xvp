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
//	Module Name:	XPolygon.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the polygon.  
*/
//*****************************************************************************
XPolygon::XPolygon()
{
}



//*****************************************************************************
/**
Constructor for the polygon.  

@param v1	The first vertex in the polygon.
@param v2	The second vertex in the polygon.
@param v3	The third vertex in the polygon.
*/
//*****************************************************************************
XPolygon::XPolygon(XVector3D v1,XVector3D v2,XVector3D v3)
{
	vert1 = v1;
	vert2 = v2;
	vert3 = v3;
}



//*****************************************************************************
/**
Destructor for the polygon.
*/
//*****************************************************************************
XPolygon::~XPolygon()
{
}



//*****************************************************************************
/**
Calculates the plane in which the polygon lies.  The plane information is stored
within the polygon object.

@param clockwise	True to specify clockwise winding order, false to specify
					counter-clockwise winding order.
@return None.
*/
//*****************************************************************************
void XPolygon::calcPlane(XBOOL clockwise)
{
	//plane.origin = vert1;	
	XVector3D v0 = vert1 - vert2;
	XVector3D v1 = vert3 - vert2;
	plane.build(vert1,v1.cross(v0));
	if (!clockwise)
	{
		plane.vector = -plane.vector;

		//Must rebuild the ray/plane normal since we flipped the vector
		plane.build(plane.origin,plane.vector);
	}
}



//*****************************************************************************
/**
Determines if a point is inside the polygon within the given tolerance.

@param p	The point to check for inclusion.
@param epsilon	The tolerance factor used for determining inclusion.
@return True if the point lies in the polygon, false otherwise.
*/
//*****************************************************************************
XBOOL XPolygon::inside(XVector3D p,XF32 epsilon)
{
	int	pos = 0;
	int	neg = 0;
	XVector3D center = calcCenterOfMass();

	XU32 j = 0;
	++j;
	if (j == 3) 
		j = 0;
		
	for (XU32 i = 0;i != 3;++i,++j)
	{
			if (j == 3) 
				j = 0;

			//The current edge
			XVector3D	&p0 = vertArray[i];
			XVector3D	&p1 = vertArray[j];

			//Generate a Normal for this edge
			XVector3D n = (p1 - p0).cross(plane.normal);

			//Which side of this edge-plane is the point?
			XF32	halfPlane = (p.dot(n)) - (p0.dot(n));

			//Keep track of positives & negatives (but not zeros -- which means it's on the edge)
			if (halfPlane > epsilon) pos++;
				else if (halfPlane < -epsilon) neg++;
		}

		// If they're ALL positive,or ALL negative,then it's inside
		if (!pos || !neg)
			return true;

		// Must not be inside,because some were pos and some were neg
		return false;
}



//*****************************************************************************
/**
Calculates the center of mass for the polygon.
*/
//*****************************************************************************
XVector3D XPolygon::calcCenterOfMass()
{
	XVector3D center = vert1 + vert2 + vert3;
	return center * ((XF32) 1 / (XF32) 3);
}
