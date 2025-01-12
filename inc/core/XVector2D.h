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
//	Module Name:	XVector2D.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VECTOR2D_H__
#define __X_VECTOR2D_H__





//*****************************************************************************
//! A 2 dimensional vector with x and y components.
/**
The XVector2D object is used to represent a 2 dimensional vector with x and
y components.  You can access each component using the public members of
the XVector2D object. See below for details on the supported mathematical
operations.
*/
//*****************************************************************************
SCRIPT_EXPORT_CLASS class XVector2D
{
public:
	union
	{
		struct
		{
			SCRIPT_EXPORT_MEMBER XF32 x;
			SCRIPT_EXPORT_MEMBER XF32 y;
		};
		XF32 array[2];
	};

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	DECLARE_SCRIPTED_CLASS_REF_ID;
	SCRIPT_EXPORT_CONSTRUCTOR XVector2D();
	XVector2D(XF32 ix,XF32 iy);
	//~XVector2D();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XVector2D operator + (XVector2D& V);
	XVector2D& operator += (XVector2D& V);
	XVector2D operator - (XVector2D& V);      
	XVector2D& operator -= (XVector2D& V);
	XVector2D operator * (XF32 scalar);      
	XVector2D& operator *= (XF32 scalar);
	XVector2D operator / (XF32 scalar);      
	XVector2D& operator /= (XF32 scalar);
	XVector2D operator * (XVector2D& V);
	XVector2D operator / (XVector2D& V);
	XVector2D operator - (); 
	XBOOL operator == (XVector2D& V);
	XBOOL operator != (XVector2D& V);
	XF32 dot(XVector2D& V);
	SCRIPT_EXPORT_FUNCTION XF32 dot(XVector2D* pV);
	XVector2D projectOnto(XVector2D& V);
	XVector2D lerp(XVector2D& V,XF32 t);
	XVector2D& normalize();
	 XVector2D& invert();
	void abs();
	void setZero();
	XBOOL isZero();
	XF32 angle(XVector2D& V);
	XF32 mag();
	XF32 distance(XVector2D& V);
	XF32 distanceSquared(XVector2D& V);
	void setLength(XF32 length);
	void min(XVector2D& V);
	void max(XVector2D& V);
	operator XVector3D();
	void toPolar(XF32& m,XF32& angle); 
	friend XVector2D operator * (XF32 scalar,XVector2D& V);
	static XVector2D XAxis;
	static XVector2D YAxis;
	static XVector2D Zero;
	static XVector2D Identity;
};


#endif
