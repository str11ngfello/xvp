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
//	Module Name:	XPlane.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PLANE_H__
#define __X_PLANE_H__






//*****************************************************************************
//!Represents a plane in 3D space infinitely extending in all directions.
/**
The XPlane object is used to represent a plane in 3D space which extends
infinitely in all directions.  The XPlane object contains the following data.

<ul>
<li> Normal - this is the normalized vector representing the plane's normal.</li>
<li> Reference Point - this is a point that lies on the 3D plane. </li> 
<li> Distance - the distance from the world origin (0,0,0) to the plane. </li> 
</ul>

See below for supported operations.
*/
//*****************************************************************************
class XPlane
{
public:
	XVector3D normal;
	XVector3D refPoint;
	XF32 distance;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XPlane();
	XPlane(XVector3D& point1,XVector3D& point2,XVector3D& point3);
	XPlane(XVector3D& point,XVector3D& normal);

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XF32 getPlaneSide(XVector3D& point);
	X_MATH_PLANE getMaxPlane();
	XVector3D getClosestPoint(XVector3D& point);
};



#endif
