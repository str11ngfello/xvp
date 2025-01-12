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
//	Module Name:	XPlane.cpp
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
Contructs an XPlane object by intializing the normal to (0,0,0), reference
point to (0,0,0) and the distance to 0.  This data must be given valid
values after construction for the member functions to work correctly.
*/
//*****************************************************************************
XPlane::XPlane()
{ 
	normal = XVector3D(0,0,0);
	refPoint = XVector3D(0,0,0);
	distance = 0;
}



//*****************************************************************************
/**
Constructs an XPlane object by determining the plane of three points in space.
By definition, three points must lie on the same plane.  The "front" of the
plane is defined by the clockwise ordering of the points where as the "back"
of the plane is defined by the counter-clockwise ordering of the points.

@param v1	Point 1.
@param v2	Point 2.
@param v3	Point 3.
*/
//*****************************************************************************
XPlane::XPlane(XVector3D& v1,XVector3D& v2,XVector3D& v3)
{
	refPoint = v1;
	
	XVector3D av,bv;
	av = v3 - v2;
	bv = v1 - v2;

    normal = av.cross(bv);
	normal.normalize();
	distance = -(normal.dot(v1));
}
       	   


//*****************************************************************************
/**
Constructs an XPlane object from the passed in point and normal.
The "front" of the plane is defined by the normal's direction.

@param point	A point that lies on the plane.
@param norm		The normal of the plane.
*/
//*****************************************************************************
XPlane::XPlane(XVector3D& point,XVector3D& norm)
{
	normal   = norm;
	refPoint = point;
	distance = -(normal.dot(point));
}



//*****************************************************************************
/**
Determines where a point lies with respect to the XPlane object by returning
the distance from the point to the plane.  

@param v	The point to compare with the plane.
@return The distance between the point and the plane.  This value can be positive or
		negative.  If the value is positive, the point lies in front of the
		plane.  If the value is negative, the point lies behind the plane.
*/
//*****************************************************************************
XF32 XPlane::getPlaneSide(XVector3D& v)
{
	return (normal.dot(v) + distance);
}



//*****************************************************************************
/**
Determines whether the plane projects more heavily onto the XY,YZ or XZ world
plane.

@return The world plane onto which this XPlane object most heavily projects.
*/
//*****************************************************************************
X_MATH_PLANE XPlane::getMaxPlane()
{
	X_MATH_PLANE p;

	switch(normal.getMaxDirection())
	{
	   case X_MD_X:
		    p = X_MP_YZ;
			break;

	   case X_MD_Y:
		    p = X_MP_XZ;
			break;
	   
	   case X_MD_Z:
		    p = X_MP_XY;
			break;
	}

	return p;
}



//*****************************************************************************
/**
Determines the point on the XPlane object that is the closest to the passed
in point.

@param point	The point to compare with.
@return The point on the plane that is the closest to the passed in point.
*/
//*****************************************************************************
XVector3D XPlane::getClosestPoint(XVector3D &point)
{
	XF32 t = getPlaneSide(point);
	return point - (normal * t);
}