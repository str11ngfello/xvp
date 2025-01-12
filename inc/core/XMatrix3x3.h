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
//	Module Name:	XMatrix3x3.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MATRIX3x3_H__
#define __X_MATRIX3x3_H__






//*****************************************************************************
//! A 3x3 matrix object with 9 accessible elements.
/**
The XMatrix3x3 object is used to represent a 3x3 matrix.  The matrix has 9 
elements that may be accessed by the element's row and column numbers. The
layout of the matrix looks like the following:

[i11  i12  i13] <br>
[i21  i22  i23] <br>
[i31  i32  i33] <br>

where,

"i11" is the element for row 1 - column 1 <br>
"i12" is the element for row 1 - column 2 <br>
"i13" is the element for row 1 - column 3 <br>
"i21" is the element for row 2 - column 1 <br>
"i22" is the element for row 2 - column 2 <br>
"i23" is the element for row 2 - column 3 <br>
"i31" is the element for row 3 - column 1 <br>
"i32" is the element for row 3 - column 2 <br>
"i33" is the element for row 3 - column 3 <br>

You can access each element by using the public members of the XMatrix3x3 object.
See below for details on the supported mathematical operations.
*/
//*****************************************************************************
class XMatrix3x3 
{
public:
	union
	{
		struct
		{
			XF32 _11, _12, _13;
			XF32 _21, _22, _23;
			XF32 _31, _32, _33;
		};
        XF32 array[3][3];
        struct 
		{
           XVector3D row1, row2, row3;
		};
	};
	
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMatrix3x3();
	XMatrix3x3(XF32 i11,XF32 i12,XF32 i13,
				XF32 i21,XF32 i22,XF32 i23,
				XF32 i31,XF32 i32,XF32 i33);
	//~XMatrix3x3();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XBOOL operator == (XMatrix3x3& M);
	XBOOL operator != (XMatrix3x3& M);
	XMatrix3x3 operator + (XMatrix3x3& M);
	XMatrix3x3& operator += (XMatrix3x3& M);
	XMatrix3x3 operator - (XMatrix3x3& M);
	XMatrix3x3& operator -= (XMatrix3x3& M);
	XMatrix3x3 operator * (XMatrix3x3& M);
	XMatrix3x3& operator *= (XMatrix3x3& M);
	XMatrix3x3 operator * (XF32 scalar);
	XMatrix3x3& operator *= (XF32 scalar);
	XMatrix3x3 operator / (XF32 scalar);
	XMatrix3x3& operator /= (XF32 scalar);
	XVector3D  operator * (XVector3D& V);
	void scale(XVector3D& vec);
	XF32 getDeterminant();
	void setZero();
	XBOOL isZero();
	XMatrix3x3 getInverse();
	XMatrix3x3& invert();
	XMatrix3x3 getTranspose();
	XMatrix3x3& transpose();
	void orthogonalize();
	void orthonormalize();
	void setIdentity();
	XBOOL isIdentity();
	XBOOL setRow(XU32 row,XVector3D& V);
	XBOOL getRow (XU32 row,XVector3D& V);
	friend XVector3D operator * (XVector3D& V,XMatrix3x3& M);
};


#endif
