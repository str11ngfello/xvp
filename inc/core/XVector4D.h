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
//	Module Name:	XVector4D.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VECTOR4D_H__
#define __X_VECTOR4D_H__






//*****************************************************************************
//! A 4 dimensional vector with x, y, z and w components.
/**
The XVector4D object is used to represent a 4 dimensional vector with x, y, z
and w components.  You can access each component using the public members of
the XVector4D object. See below for details on the supported mathematical
operations.
*/
//*****************************************************************************
class XVector4D
{
public:
	union
	{
		struct
		{
			XF32 x, y, z, w;
		};
		XF32 array[4];
	};

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XVector4D();
	XVector4D(XF32 ix,XF32 iy,XF32 iz,XF32 iw);
	XVector4D(XColor& color);
	//~XVector4D();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XVector4D operator + (XVector4D& V);
	XVector4D& operator += (XVector4D& V);
	XVector4D operator - (XVector4D& V);      
	XVector4D& operator -= (XVector4D& V);
	XVector4D operator * (XF32 scalar);      
	XVector4D& operator *= (XF32 scalar);
	XVector4D operator / (XF32 scalar);      
	XVector4D& operator /= (XF32 scalar);
	XVector4D operator * (XVector4D& V);
	XVector4D operator / (XVector4D& V);
	XVector4D operator -();
	XBOOL operator == (XVector4D& V);
	XBOOL operator != (XVector4D& V);
	XF32 dot(const XVector4D& V);
	XVector4D projectOnto(XVector4D& V);
	XVector4D lerp(XVector4D& V,XF32 t);
	XVector4D& normalize();
	XVector4D& invert();
	void abs();
	void setZero();
	XBOOL isZero();
	XF32 angle(XVector4D& V);
	XF32 distance(XVector4D& V);
	XF32 distanceSquared(XVector4D& V);
	XVector4D& homogenize();
	XF32 mag();
	void setLength(XF32 length);
	void min(XVector4D& V);
	void max(XVector4D& V);
	operator XVector3D();
	friend XVector4D operator * (XF32 scalar,XVector4D& V);
	static XVector4D XAxis;
	static XVector4D YAxis;
	static XVector4D ZAxis;
	static XVector4D WAxis;
	static XVector4D Zero;
	static XVector4D Identity;
};


#endif
