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
//	Module Name:	XRay3D.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XRay3D::XRay3D()
{
	origin = XVector3D(0,0,0);
	vector = XVector3D(0,0,0);
	end = XVector3D(0,0,0);
	normal = XVector3D(0,0,0);
	length = 0;
};



//*****************************************************************************
/**
Constructs an XRay3D object from a point and vector.

@param org	The origin of the ray.
@param vec	The vector that describes the length and direction of the ray.
*/
//*****************************************************************************
XRay3D::XRay3D(XVector3D org,XVector3D vec)
{
	origin = org;
	vector = vec;
	end = origin + vector;
	length = vector.mag();
	normal = XVector3D(0,0,0);
	if (vector != XVector3D(0,0,0))
	{
		normal = vector;
		normal.normalize();
	}
}



//*****************************************************************************
/**
Reconstructs the XRay3D object from the passed in parameters.

@param org	The origin of the ray.
@param vec	The vector that describes the length and direction of the ray.
@return None.
*/
//*****************************************************************************
void XRay3D::build(XVector3D org,XVector3D vec)
{
	origin = org;
	vector = vec;
	end = origin + vector;
	length = vector.mag();
	normal = XVector3D(0,0,0);
	if (vector != XVector3D(0,0,0))
	{
		normal = vector;
		normal.normalize();
	}
}



//*****************************************************************************
/**
Scales the XRay3D object by a 3x3 matrix.

@param m	The 3x3 matrix to scale the ray with.
@return none.
*/
//*****************************************************************************
void XRay3D::scale(XMatrix3x3& m)
{
	vector = m * vector;
	origin = m * origin;
	end = m * end;
	length = vector.mag();
}



//*****************************************************************************
/**
Calculates the end point of the XRay3D object.

@param len	The length to add from the origin of the ray.
@return A 3D point that represents the end point of the ray.
*/
//*****************************************************************************
XVector3D XRay3D::calcEnd(XF32 len)
{
	return origin + (normal * len);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XRay3D::intersect(XRay3D &r,XF32 &time)
{
	time = 0;
	XF32 denom = normal.dot(r.normal);

	if (denom == 0)
		return false;

	XF32 numer = normal.dot(r.origin);

	time = -((numer + (-(origin.dot(normal)))) / denom);

	return true;
}



//*****************************************************************************
//*****************************************************************************
XVector3D XRay3D::closest(XVector3D &point)
{
	XF32 t = distance(point);
	return point - (normal * t);
}



//*****************************************************************************
//*****************************************************************************
XF32 XRay3D::distance(XVector3D point)
{
	XF32 t;
	intersect(XRay3D(point,-normal),t);
	return t;
}
