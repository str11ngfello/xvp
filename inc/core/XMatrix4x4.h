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
//	Module Name:	XMatrix4x4.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MATRIX4x4_H__
#define __X_MATRIX4x4_H__






//*****************************************************************************
//! A 4x4 matrix object with 16 accessible elements.
/**
The XMatrix4x4 object is used to represent a 4x4 matrix.  The matrix has 16 
elements that may be accessed by the element's row and column numbers. The
layout of the matrix looks like the following:

[i11  i12  i13 i14] <br>
[i21  i22  i23 i24] <br>
[i31  i32  i33 i34] <br>
[i41  i42  i43 i44] <br>

where,

"i11" is the element for row 1 - column 1 <br>
"i12" is the element for row 1 - column 2 <br>
"i13" is the element for row 1 - column 3 <br>
"i14" is the element for row 1 - column 4 <br>
"i21" is the element for row 2 - column 1 <br>
"i22" is the element for row 2 - column 2 <br>
"i23" is the element for row 2 - column 3 <br>
"i24" is the element for row 2 - column 4 <br>
"i31" is the element for row 3 - column 1 <br>
"i32" is the element for row 3 - column 2 <br>
"i33" is the element for row 3 - column 3 <br>
"i34" is the element for row 3 - column 4 <br>
"i41" is the element for row 4 - column 1 <br>
"i42" is the element for row 4 - column 2 <br>
"i43" is the element for row 4 - column 3 <br>
"i44" is the element for row 4 - column 4 <br>

You can access each element by using the public members of the XMatrix4x4 object.
See below for details on the supported mathematical operations.
*/
//*****************************************************************************
class XMatrix4x4 
{
public:
	union
	{
		struct
		{
			XF32 _11, _12, _13, _14;
			XF32 _21, _22, _23, _24;
			XF32 _31, _32, _33, _34;
			XF32 _41, _42, _43, _44;
		};
        XF32 array[4][4];
		struct
		{
			XVector4D row1, row2, row3, row4;
		};
	};

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMatrix4x4();
	XMatrix4x4(XF32 i11,XF32 i12,XF32 i13,XF32 i14,
				XF32 i21,XF32 i22,XF32 i23,XF32 i24,
				XF32 i31,XF32 i32,XF32 i33,XF32 i34,
				XF32 i41,XF32 i42,XF32 i43,XF32 i44);
	//~XMatrix4x4(); 

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XBOOL operator == (XMatrix4x4& M);
	XBOOL operator != (XMatrix4x4& M);
	XMatrix4x4 operator + (XMatrix4x4& M);
	XMatrix4x4& operator += (XMatrix4x4& M);
	XMatrix4x4 operator - (XMatrix4x4& M);
	XMatrix4x4& operator -= (XMatrix4x4& M);
	XMatrix4x4 operator * (XMatrix4x4& M);
	XMatrix4x4& operator *= (XMatrix4x4& M);
	XMatrix4x4 operator * (XF32 scalar);
	XMatrix4x4& operator *= (XF32 scalar);
	XMatrix4x4 operator / (XF32 scalar);
	XMatrix4x4& operator /= (XF32 scalar);
	XVector3D  operator * (XVector3D& V); 
	XVector4D  operator * (XVector4D& V);
	XF32 getDeterminant();
	void setZero();
	XBOOL isZero();
	XMatrix4x4 getInverse();
	XMatrix4x4& invert();
	XMatrix4x4 getTranspose();
	XMatrix4x4& transpose();
	XQuaternion toQuat();
	void orthogonalize();
	void orthonormalize();
	void setIdentity();
	XBOOL isIdentity();
	XVector3D transformVector3D(XVector3D& v);
	XVector4D transformVector4D(XVector4D& v);
	void createXRotation(XF32 radians);
	void createYRotation(XF32 radians);
	void createZRotation(XF32 radians);
	void createTranslation(XVector3D& translationVec);
	void createScale(XVector3D& scaleVec);
	XF32 getXScale();
	void setXScale(XF32 scale);
	XF32 getYScale();
	void setYScale(XF32 scale);
	XF32 getZScale();
	void setZScale(XF32 scale);
	void createArbRotation(XVector3D& axis,XF32 radians);
	void createProjLH(XF32 zNearClip,XF32 zFarClip,XF32 fov,XF32 aspect);
	void createProjRH(XF32 zNearClip,XF32 zFarClip,XF32 fov,XF32 aspect);
	void setOrthoProjectionLH(XF32 zNearClip,XF32 zFarClip,XF32 viewportWidth,XF32 viewportHeight);
	void createView(XVector3D& right,XVector3D& up,XVector3D& look,XVector3D& pos);
	void createView(XMatrix4x4& matrix);
	static XMatrix4x4 getObjectLookat(XVector3D& from,XVector3D& at);
	XBOOL setRow(XU32 row,XVector4D& V);
	XBOOL getRow(XU32 row,XVector4D& V);
	friend XVector4D operator * (XVector4D& V,XMatrix4x4& M);
};

#endif
