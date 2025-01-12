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
//	Module Name:	XQuaternionD.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_QUATERNION_H__
#define __X_QUATERNION_H__






//*****************************************************************************
//! A quaternion object with x, y, z and w components.
/**
The XQuaternion object is used to represent a quaternion with x, y, z
and w components.  The components are also sometimes referred to as an
axis (x,y,z) and angle (w) pair.  

You can access the quaternion's components by using the public members of the
XQuaternion object.  The layout of the quaternion looks like the following:

[x y z w]

Where, x, y, and z make up the axis, and w represents an angle about that axis.
See below for details on the supported mathematical operations.
*/
//*****************************************************************************
class XQuaternion 
{
public:
	XF32 x,y,z,w;  
	
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XQuaternion();
	XQuaternion(XF32 ix,XF32 iy,XF32 iz,XF32 iw);
	XQuaternion(XVector3D& v,XF32 angle);
	//~XQuaternion();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XBOOL operator==(XQuaternion& q);
	XBOOL operator!=(XQuaternion& q);
	XQuaternion operator*(XQuaternion& q);
	XQuaternion& operator*=(XQuaternion& q);
	XQuaternion operator+(XQuaternion& q);
	XQuaternion& operator+=(XQuaternion& q);
	XQuaternion operator-(XQuaternion& q);
	XQuaternion& operator-=(XQuaternion& q);
	XMatrix4x4 toMatrix4x4();
	void setIdentity();
	XQuaternion slerp(XQuaternion& q,XF32 t);
	void normalize();
	void conjugate();
};

#endif
