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
//	Module Name:	XRay3D.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RAY3D_H__
#define __X_RAY3D_H__






//*****************************************************************************
//! 3D ray object.
/**
The XRay3D object represents a ray in 3D space.  It provides common methods
associated with the use of rays.       

See the member functions below for more information.
*/
//*****************************************************************************
class XRay3D
{
public:
	XVector3D origin;
	XVector3D end;
	XVector3D vector;
	XVector3D normal;
	XF32 length;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XRay3D();
	XRay3D(XVector3D org, XVector3D vec);
	//~XRay3D();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	void build(XVector3D org,XVector3D vec);
	void scale(XMatrix3x3& m);
	XVector3D calcEnd(XF32 len);
	XBOOL intersect(XRay3D& r,XF32& time);
	XVector3D closest(XVector3D& point);
	XF32 distance(XVector3D point);
};

#endif
