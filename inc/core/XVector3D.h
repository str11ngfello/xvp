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
//	Module Name:	XVector3D.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VECTOR3D_H__
#define __X_VECTOR3D_H__



//*****************************************************************************
//! A 3 dimensional vector with x, y and z components.
/**
The XVector3D object is used to represent a 3 dimensional vector with x, y and
z components.  You can access each component using the public members of
the XVector3D object.  See below for details on the supported mathematical
operations.
*/
//*****************************************************************************
class XVector3D
{
public:
	union
	{
		struct
		{
			XF32 x, y, z;
		};
		XF32 array[3];
	};

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XVector3D();
	XVector3D(XF32 ix,XF32 iy,XF32 iz);
	 //~XVector3D();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XVector3D operator + (XVector3D& V);
	XVector3D& operator += (XVector3D& V);
	XVector3D operator - (XVector3D& V);      
	XVector3D& operator -= (XVector3D& V);
	XVector3D operator * (XF32 scalar);      
	XVector3D& operator *= (XF32 scalar);
	XVector3D operator / (XF32 scalar); 
	XVector3D& operator /= (XF32 scalar);
	XVector3D operator * (XVector3D& V);
	XVector3D operator / (XVector3D& V);
	XVector3D operator -();
	XBOOL operator == (XVector3D& V);
	XBOOL operator != (XVector3D& V);
	XBOOL operator > (XVector3D& V);
	XBOOL operator >= (XVector3D& V);
	XBOOL operator < (XVector3D& V);
	XBOOL operator <= (XVector3D& V);
	XF32 dot(XVector3D& V);
	XVector3D cross(XVector3D& V);
	XVector3D projectOnto(XVector3D& V);
	XVector3D lerp(XVector3D& V,XF32 t);
	XVector3D& normalize();
	XVector3D& invert();
	void abs();
	void setZero();
	XBOOL isZero();
	XF32 angle(XVector3D& V);
	XF32 mag();
	XF32 distance(XVector3D& V);
	XF32 distanceSquared(XVector3D& V);
	void setLength(XF32 length);
	void min(XVector3D& V);
	void max(XVector3D& V);
	X_MATH_DIRECTION getMaxDirection();
	operator XVector4D();
	XStatus toSpherical(XF32& m,XF32& angle1,XF32& angle2);
	friend XVector3D operator * (XF32 scalar,XVector3D& V);
	static XVector3D XAxis;
	static XVector3D YAxis;
	static XVector3D ZAxis;
	static XVector3D Zero;
	static XVector3D Identity;

};

#endif