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
//	Module Name:	XMatrix4x4.cpp
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
Constructs an XMatrix4x4 object by initializing the matrix to the 4x4 identity
below.

[1 0 0 0] <br>
[0 1 0 0] <br>
[0 0 1 0] <br>
[0 0 0 1] <br>
*/
//*****************************************************************************
XMatrix4x4::XMatrix4x4()
{
	_11 = 1;_12 = 0;_13 = 0;_14 = 0;
	_21 = 0;_22 = 1;_23 = 0;_24 = 0;
	_31 = 0;_32 = 0;_33 = 1;_34 = 0;
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;
}



//*****************************************************************************
/**
Initializes an XMatrix4x4 object by intializing the matrix elements to the
passed in parameters.

@param i11	Inital value of the element in Row 1 - Column 1.
@param i12	Inital value of the element in Row 1 - Column 2.
@param i13	Inital value of the element in Row 1 - Column 3.
@param i14	Inital value of the element in Row 1 - Column 4.
@param i21	Inital value of the element in Row 2 - Column 1.
@param i22	Inital value of the element in Row 2 - Column 2.
@param i23	Inital value of the element in Row 2 - Column 3.
@param i24	Inital value of the element in Row 2 - Column 4.
@param i31	Inital value of the element in Row 3 - Column 1.
@param i32	Inital value of the element in Row 3 - Column 2.
@param i33	Inital value of the element in Row 3 - Column 3.
@param i34	Inital value of the element in Row 3 - Column 4.
@param i41	Inital value of the element in Row 4 - Column 1.
@param i42	Inital value of the element in Row 4 - Column 2.
@param i43	Inital value of the element in Row 4 - Column 3.
@param i44	Inital value of the element in Row 4 - Column 4.
*/
//*****************************************************************************
XMatrix4x4::XMatrix4x4(XF32 i11,XF32 i12,XF32 i13,XF32 i14,
						 XF32 i21,XF32 i22,XF32 i23,XF32 i24,
						 XF32 i31,XF32 i32,XF32 i33,XF32 i34,
						 XF32 i41,XF32 i42,XF32 i43,XF32 i44)
{
	_11 = i11;_12 = i12;_13 = i13;_14 = i14;
	_21 = i21;_22 = i22;_23 = i23;_24 = i24;
	_31 = i31;_32 = i32;_33 = i33;_34 = i34;
	_41 = i41;_42 = i42;_43 = i43;_44 = i44;
}



//*****************************************************************************
/**
Determines if two XMatrix4x4 objects are equal.  For the XMatrix4x4 objects
to be equal, all elements must match between the matrices.

@param M	The matrix to compare with.
@return True if the matrices are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix4x4::operator == (XMatrix4x4& M) 
{
	XF32 *M1,*M2;

	M1 =   &_11;
	M2 = &M._11;

	for (XU32 i = 0;i < 16;i++) 
	{
		if (fabs(*M1 - *M2) > X_MATH_EPSILON)
			return false;
		++M1;
		++M2;
	}

	return true;
}



//*****************************************************************************
/**
Determines if two XMatrix4x4 objects are unequal.  For the XMatrix4x4 objects
to be unequal, at least one element must differ between the matrices.

@param M	The matrix to compare with.
@return True if the matrices are unequal, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix4x4::operator != (XMatrix4x4& M) 
{
	XF32 *M1,*M2;

	M1 = &_11;
	M2 = &M._11;

	for (XU32 i = 0;i < 16;i++) 
	{
		if (fabs(*M1 - *M2) > X_MATH_EPSILON)
			return true;
		++M1;
		++M2;
	}

	return false;
}



//*****************************************************************************
/**
Adds two XMatrix4x4 objects.  The matrices are added component wise
according to the following formula where M1 and M2 are the matrices to add,
and R is the resulting matrix.

R._11 = M1._11 + M2._11 <br>
R._12 = M1._12 + M2._12 <br>
R._13 = M1._13 + M2._13 <br>
R._14 = M1._14 + M2._14 <br>
R._21 = M1._21 + M2._21 <br>
R._22 = M1._22 + M2._22 <br>
R._23 = M1._23 + M2._23 <br>
R._24 = M1._24 + M2._24 <br>
R._31 = M1._31 + M2._31 <br>
R._32 = M1._32 + M2._32 <br>
R._33 = M1._33 + M2._33 <br>
R._34 = M1._34 + M2._34 <br>
R._41 = M1._41 + M2._41 <br>
R._42 = M1._42 + M2._42 <br>
R._43 = M1._43 + M2._43 <br>
R._44 = M1._44 + M2._44 <br>

@param	M The XMatrix4x4 object to add.  
@return	The sum of the two matrices.  
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::operator + (XMatrix4x4& M) 
{
	return XMatrix4x4(_11 + M._11,
					   _12 + M._12,
					   _13 + M._13,
					   _14 + M._14,
					   _21 + M._21,
					   _22 + M._22,
					   _23 + M._23,
					   _24 + M._24,
					   _31 + M._31,
					   _32 + M._32,
					   _33 + M._33,
					   _34 + M._34,
					   _41 + M._41,
					   _42 + M._42,
					   _43 + M._43,
					   _44 + M._44);
}



//*****************************************************************************
/**
Adds two XMatrix4x4 objects.  The matrices are added component wise
according to the following formula where M1 and M2 are the matrices to add,
and R is the resulting matrix.  This XMatrix4x4 object is assigned the result.

R._11 = M1._11 + M2._11 <br>
R._12 = M1._12 + M2._12 <br>
R._13 = M1._13 + M2._13 <br>
R._14 = M1._14 + M2._14 <br>
R._21 = M1._21 + M2._21 <br>
R._22 = M1._22 + M2._22 <br>
R._23 = M1._23 + M2._23 <br>
R._24 = M1._24 + M2._24 <br>
R._31 = M1._31 + M2._31 <br>
R._32 = M1._32 + M2._32 <br>
R._33 = M1._33 + M2._33 <br>
R._34 = M1._34 + M2._34 <br>
R._41 = M1._41 + M2._41 <br>
R._42 = M1._42 + M2._42 <br>
R._43 = M1._43 + M2._43 <br>
R._44 = M1._44 + M2._44 <br>

@param	M The XMatrix4x4 object to add.  
@return	The sum of the two matrices.  
*/
//*****************************************************************************
XMatrix4x4 &XMatrix4x4::operator += (XMatrix4x4& M) 
{
	_11 += M._11;
	_12 += M._12;
	_13 += M._13;
	_14 += M._14;
	_21 += M._21;
	_22 += M._22;
	_23 += M._23;
	_24 += M._24;
	_31 += M._31;
	_32 += M._32;
	_33 += M._33;
	_34 += M._34;
	_41 += M._41;
	_42 += M._42;
	_43 += M._43;
	_44 += M._44;

	return *this;
}



//*****************************************************************************
/**
Subtracts two XMatrix4x4 objects.  The matrices are subtracted component wise
according to the following formula where M1 and M2 are the matrices to subtract,
and R is the resulting matrix.

R._11 = M1._11 - M2._11 <br>
R._12 = M1._12 - M2._12 <br>
R._13 = M1._13 - M2._13 <br>
R._14 = M1._14 - M2._14 <br>
R._21 = M1._21 - M2._21 <br>
R._22 = M1._22 - M2._22 <br>
R._23 = M1._23 - M2._23 <br>
R._24 = M1._24 - M2._24 <br>
R._31 = M1._31 - M2._31 <br>
R._32 = M1._32 - M2._32 <br>
R._33 = M1._33 - M2._33 <br>
R._34 = M1._34 - M2._34 <br>
R._41 = M1._41 - M2._41 <br>
R._42 = M1._42 - M2._42 <br>
R._43 = M1._43 - M2._43 <br>
R._44 = M1._44 - M2._44 <br>

@param	M The XMatrix4x4 object to subtract.  
@return	The difference of the two matrices.  
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::operator - (XMatrix4x4& M) 
{
	return XMatrix4x4(_11 - M._11,
					   _12 - M._12,
					   _13 - M._13,
					   _14 - M._14,
					   _21 - M._21,
					   _22 - M._22,
					   _23 - M._23,
					   _24 - M._24,
					   _31 - M._31,
					   _32 - M._32,
					   _33 - M._33,
					   _34 - M._34,
					   _41 - M._41,
					   _42 - M._42,
					   _43 - M._43,
					   _44 - M._44);
}



//*****************************************************************************
/**
Subtracts two XMatrix4x4 objects.  The matrices are subtracted component wise
according to the following formula where M1 and M2 are the matrices to subtract,
and R is the resulting matrix.  This XMatrix4x4 object is assigned the result.

R._11 = M1._11 - M2._11 <br>
R._12 = M1._12 - M2._12 <br>
R._13 = M1._13 - M2._13 <br>
R._14 = M1._14 - M2._14 <br>
R._21 = M1._21 - M2._21 <br>
R._22 = M1._22 - M2._22 <br>
R._23 = M1._23 - M2._23 <br>
R._24 = M1._24 - M2._24 <br>
R._31 = M1._31 - M2._31 <br>
R._32 = M1._32 - M2._32 <br>
R._33 = M1._33 - M2._33 <br>
R._34 = M1._34 - M2._34 <br>
R._41 = M1._41 - M2._41 <br>
R._42 = M1._42 - M2._42 <br>
R._43 = M1._43 - M2._43 <br>
R._44 = M1._44 - M2._44 <br>

@param	M The XMatrix4x4 object to subtract.  
@return	The difference of the two matrices.  
*/
//*****************************************************************************
XMatrix4x4 &XMatrix4x4::operator -= (XMatrix4x4& M) 
{
	_11 -= M._11;
	_12 -= M._12;
	_13 -= M._13;
	_14 -= M._14;
	_21 -= M._21;
	_22 -= M._22;
	_23 -= M._23;
	_24 -= M._24;
	_31 -= M._31;
	_32 -= M._32;
	_33 -= M._33;
	_34 -= M._34;
	_41 -= M._41;
	_42 -= M._42;
	_43 -= M._43;
	_44 -= M._44;

	return *this;
}



//*****************************************************************************
/**
Multiplies two XMatrix4x4 objects.  The matrices are multiplied according to
the following formula where M1 and M2 are the matrices to multiply, and R is
the resulting matrix.

R._11 = M1._11 * M2._11 + M1._12 * M2._21 + M1._13 * M2._31 + M1._14 * M2._41 <br>
R._12 = M1._11 * M2._12 + M1._12 * M2._22 + M1._13 * M2._32 + M1._14 * M2._42 <br>
R._13 = M1._11 * M2._13 + M1._12 * M2._23 + M1._13 * M2._33 + M1._14 * M2._43 <br>
R._14 = M1._11 * M2._14 + M1._12 * M2._24 + M1._13 * M2._34 + M1._14 * M2._44 <br>
R._21 = M1._21 * M2._11 + M1._22 * M2._21 + M1._23 * M2._31 + M1._24 * M2._41 <br>
R._22 = M1._21 * M2._12 + M1._22 * M2._22 + M1._23 * M2._32 + M1._24 * M2._42 <br>
R._23 = M1._21 * M2._13 + M1._22 * M2._23 + M1._23 * M2._33 + M1._24 * M2._43 <br>
R._24 = M1._21 * M2._14 + M1._22 * M2._24 + M1._23 * M2._34 + M1._24 * M2._44 <br>
R._31 = M1._31 * M2._11 + M1._32 * M2._21 + M1._33 * M2._31 + M1._34 * M2._41 <br>
R._32 = M1._31 * M2._12 + M1._32 * M2._22 + M1._33 * M2._32 + M1._34 * M2._42 <br>
R._33 = M1._31 * M2._13 + M1._32 * M2._23 + M1._33 * M2._33 + M1._34 * M2._43 <br>
R._34 = M1._31 * M2._14 + M1._32 * M2._24 + M1._33 * M2._34 + M1._34 * M2._44 <br>
R._41 = M1._41 * M2._11 + M1._42 * M2._21 + M1._43 * M2._31 + M1._44 * M2._41 <br>
R._42 = M1._41 * M2._12 + M1._42 * M2._22 + M1._43 * M2._32 + M1._44 * M2._42 <br>
R._43 = M1._41 * M2._13 + M1._42 * M2._23 + M1._43 * M2._33 + M1._44 * M2._43 <br>
R._44 = M1._41 * M2._14 + M1._42 * M2._24 + M1._43 * M2._34 + M1._44 * M2._44 <br>

@param	M The XMatrix4x4 object to multiply by.  
@return	The product of the two matrices.  
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::operator * (XMatrix4x4& M) 
{
	return XMatrix4x4(_11 * M._11 + _12 * M._21 + _13 * M._31 + _14 * M._41,
					   _11 * M._12 + _12 * M._22 + _13 * M._32 + _14 * M._42,
					   _11 * M._13 + _12 * M._23 + _13 * M._33 + _14 * M._43,
					   _11 * M._14 + _12 * M._24 + _13 * M._34 + _14 * M._44,
					   _21 * M._11 + _22 * M._21 + _23 * M._31 + _24 * M._41,
					   _21 * M._12 + _22 * M._22 + _23 * M._32 + _24 * M._42,
					   _21 * M._13 + _22 * M._23 + _23 * M._33 + _24 * M._43,
					   _21 * M._14 + _22 * M._24 + _23 * M._34 + _24 * M._44,
					   _31 * M._11 + _32 * M._21 + _33 * M._31 + _34 * M._41,
					   _31 * M._12 + _32 * M._22 + _33 * M._32 + _34 * M._42,
					   _31 * M._13 + _32 * M._23 + _33 * M._33 + _34 * M._43,
					   _31 * M._14 + _32 * M._24 + _33 * M._34 + _34 * M._44,
					   _41 * M._11 + _42 * M._21 + _43 * M._31 + _44 * M._41,
					   _41 * M._12 + _42 * M._22 + _43 * M._32 + _44 * M._42,
					   _41 * M._13 + _42 * M._23 + _43 * M._33 + _44 * M._43,
					   _41 * M._14 + _42 * M._24 + _43 * M._34 + _44 * M._44);
}



//*****************************************************************************
/**
Multiplies two XMatrix4x4 objects.  The matrices are multiplied according to
the following formula where M1 and M2 are the matrices to multiply, and R is
the resulting matrix.  This XMatrix4x4 object is assigned the result.

R._11 = M1._11 * M2._11 + M1._12 * M2._21 + M1._13 * M2._31 + M1._14 * M2._41 <br>
R._12 = M1._11 * M2._12 + M1._12 * M2._22 + M1._13 * M2._32 + M1._14 * M2._42 <br>
R._13 = M1._11 * M2._13 + M1._12 * M2._23 + M1._13 * M2._33 + M1._14 * M2._43 <br>
R._14 = M1._11 * M2._14 + M1._12 * M2._24 + M1._13 * M2._34 + M1._14 * M2._44 <br>
R._21 = M1._21 * M2._11 + M1._22 * M2._21 + M1._23 * M2._31 + M1._24 * M2._41 <br>
R._22 = M1._21 * M2._12 + M1._22 * M2._22 + M1._23 * M2._32 + M1._24 * M2._42 <br>
R._23 = M1._21 * M2._13 + M1._22 * M2._23 + M1._23 * M2._33 + M1._24 * M2._43 <br>
R._24 = M1._21 * M2._14 + M1._22 * M2._24 + M1._23 * M2._34 + M1._24 * M2._44 <br>
R._31 = M1._31 * M2._11 + M1._32 * M2._21 + M1._33 * M2._31 + M1._34 * M2._41 <br>
R._32 = M1._31 * M2._12 + M1._32 * M2._22 + M1._33 * M2._32 + M1._34 * M2._42 <br>
R._33 = M1._31 * M2._13 + M1._32 * M2._23 + M1._33 * M2._33 + M1._34 * M2._43 <br>
R._34 = M1._31 * M2._14 + M1._32 * M2._24 + M1._33 * M2._34 + M1._34 * M2._44 <br>
R._41 = M1._41 * M2._11 + M1._42 * M2._21 + M1._43 * M2._31 + M1._44 * M2._41 <br>
R._42 = M1._41 * M2._12 + M1._42 * M2._22 + M1._43 * M2._32 + M1._44 * M2._42 <br>
R._43 = M1._41 * M2._13 + M1._42 * M2._23 + M1._43 * M2._33 + M1._44 * M2._43 <br>
R._44 = M1._41 * M2._14 + M1._42 * M2._24 + M1._43 * M2._34 + M1._44 * M2._44 <br>

@param	M The XMatrix4x4 object to multiply by.  
@return	The product of the two matrices.  
*/
//*****************************************************************************
XMatrix4x4 &XMatrix4x4::operator *= (XMatrix4x4& M) 
{
	XMatrix4x4 R;

	R._11 = _11 * M._11 + _12 * M._21 + _13 * M._31 + _14 * M._41;
	R._12 = _11 * M._12 + _12 * M._22 + _13 * M._32 + _14 * M._42;
	R._13 = _11 * M._13 + _12 * M._23 + _13 * M._33 + _14 * M._43;
	R._14 = _11 * M._14 + _12 * M._24 + _13 * M._34 + _14 * M._44;

	R._21 = _21 * M._11 + _22 * M._21 + _23 * M._31 + _24 * M._41;
	R._22 = _21 * M._12 + _22 * M._22 + _23 * M._32 + _24 * M._42;
	R._23 = _21 * M._13 + _22 * M._23 + _23 * M._33 + _24 * M._43;
	R._24 = _21 * M._14 + _22 * M._24 + _23 * M._34 + _24 * M._44;

	R._31 = _31 * M._11 + _32 * M._21 + _33 * M._31 + _34 * M._41;
	R._32 = _31 * M._12 + _32 * M._22 + _33 * M._32 + _34 * M._42;
	R._33 = _31 * M._13 + _32 * M._23 + _33 * M._33 + _34 * M._43;
	R._34 = _31 * M._14 + _32 * M._24 + _33 * M._34 + _34 * M._44;

	R._41 = _41 * M._11 + _42 * M._21 + _43 * M._31 + _44 * M._41;
	R._42 = _41 * M._12 + _42 * M._22 + _43 * M._32 + _44 * M._42;
	R._43 = _41 * M._13 + _42 * M._23 + _43 * M._33 + _44 * M._43;
	R._44 = _41 * M._14 + _42 * M._24 + _43 * M._34 + _44 * M._44;

	(*this) = R;

	return *this;
}



//*****************************************************************************
/**
Multiplies an XMatrix4x4 object by a scalar.  The matrix and scalar are
multiplied according to the following formula where M is the matrix, S is the
scalar to multiply by, and R is the resulting matrix.

R._11 = M._11 * S <br>
R._12 = M._12 * S <br>
R._13 = M._13 * S <br>
R._14 = M._14 * S <br>
R._21 = M._21 * S <br>
R._22 = M._22 * S <br>
R._23 = M._23 * S <br>
R._24 = M._24 * S <br>
R._31 = M._31 * S <br>
R._32 = M._32 * S <br>
R._33 = M._33 * S <br>
R._34 = M._34 * S <br>
R._41 = M._41 * S <br>
R._42 = M._42 * S <br>
R._43 = M._43 * S <br>
R._44 = M._44 * S <br>

@param	scalar The scalar to multiply by.  
@return	The product of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::operator * (XF32 scalar) 
{
	return XMatrix4x4(_11 * scalar,_12 * scalar,_13 * scalar,_14 * scalar,
					   _21 * scalar,_22 * scalar,_23 * scalar,_24 * scalar,
                       _31 * scalar,_32 * scalar,_33 * scalar,_34 * scalar,
                       _41 * scalar,_42 * scalar,_43 * scalar,_44 * scalar);
}



//*****************************************************************************
/**
Multiplies an XMatrix4x4 object by a scalar.  The matrix and scalar are
multiplied according to the following formula where M is the matrix, S is the
scalar to multiply by, and R is the resulting matrix.  This XMatrix4x4 object
is assigned the result.

R._11 = M._11 * S <br>
R._12 = M._12 * S <br>
R._13 = M._13 * S <br>
R._14 = M._13 * S <br>
R._21 = M._21 * S <br>
R._22 = M._22 * S <br>
R._23 = M._23 * S <br>
R._24 = M._24 * S <br>
R._31 = M._31 * S <br>
R._32 = M._32 * S <br>
R._33 = M._33 * S <br>
R._34 = M._34 * S <br>
R._41 = M._41 * S <br>
R._42 = M._42 * S <br>
R._43 = M._43 * S <br>
R._44 = M._44 * S <br>

@param	scalar The scalar to multiply by.  
@return	The product of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix4x4 &XMatrix4x4::operator *= (XF32 scalar) 
{
	_11 *= scalar;
	_12 *= scalar;
	_13 *= scalar;
	_14 *= scalar;
    _21 *= scalar;
	_22 *= scalar;
	_23 *= scalar;
	_24 *= scalar;
    _31 *= scalar;
	_32 *= scalar;
	_33 *= scalar;
	_34 *= scalar;
    _41 *= scalar;
	_42 *= scalar;
	_43 *= scalar;
	_44 *= scalar;

	return *this;
}



//*****************************************************************************
/**
Divides an XMatrix4x4 object by a scalar.  The matrix and scalar are
divided according to the following formula where M is the matrix, S is the
scalar to divide by, and R is the resulting matrix.

R._11 = M._11 / S <br>
R._12 = M._12 / S <br>
R._13 = M._13 / S <br>
R._14 = M._14 / S <br>
R._21 = M._21 / S <br>
R._22 = M._22 / S <br>
R._23 = M._23 / S <br>
R._24 = M._24 / S <br>
R._31 = M._31 / S <br>
R._32 = M._32 / S <br>
R._33 = M._33 / S <br>
R._34 = M._34 / S <br>
R._41 = M._41 / S <br>
R._42 = M._42 / S <br>
R._43 = M._43 / S <br>
R._44 = M._44 / S <br>

@param	scalar The scalar to divide by.  
@return	The quotient of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::operator / (XF32 scalar) 
{
	return XMatrix4x4(_11 / scalar,_12 / scalar,_13 / scalar,_14 / scalar,
					   _21 / scalar,_22 / scalar,_23 / scalar,_24 / scalar,
                       _31 / scalar,_32 / scalar,_33 / scalar,_34 / scalar,
                       _41 / scalar,_42 / scalar,_43 / scalar,_44 / scalar);
}



//*****************************************************************************
/**
Divides an XMatrix4x4 object by a scalar.  The matrix and scalar are
divided according to the following formula where M is the matrix, S is the
scalar to divide by, and R is the resulting matrix.  This XMatrix4x4 object
is assigned the result.

R._11 = M._11 / S <br>
R._12 = M._12 / S <br>
R._13 = M._13 / S <br>
R._14 = M._14 / S <br>
R._21 = M._21 / S <br>
R._22 = M._22 / S <br>
R._23 = M._23 / S <br>
R._24 = M._24 / S <br>
R._31 = M._31 / S <br>
R._32 = M._32 / S <br>
R._33 = M._33 / S <br>
R._34 = M._34 / S <br>
R._41 = M._41 / S <br>
R._42 = M._42 / S <br>
R._43 = M._43 / S <br>
R._44 = M._44 / S <br>

@param	scalar The scalar to divide by.  
@return	The quotient of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix4x4 &XMatrix4x4::operator /= (XF32 scalar) 
{
	_11 /= scalar;
	_12 /= scalar;
	_13 /= scalar;
	_14 /= scalar;
    _21 /= scalar;
	_22 /= scalar;
	_23 /= scalar;
	_24 /= scalar;
    _31 /= scalar;
	_32 /= scalar;
	_33 /= scalar;
	_34 /= scalar;
    _41 /= scalar;
	_42 /= scalar;
	_43 /= scalar;
	_44 /= scalar;

	return *this;
}



//*****************************************************************************
/**
Transforms an XVector4D object by an XMatrix4x4 object according to the
following formula where M is the XMatrix4x4 object, V is the XVector4D object
to transform, and R is the resulting XVector4D object.  

R.x = M._11 * V.x + M._21 * V.y + M._31 * V.z + M._41 * V.w <br>
R.y = M._12 * V.x + M._22 * V.y + M._32 * V.z + M._42 * V.w <br>
R.z = M._13 * V.x + M._23 * V.y + M._33 * V.z + M._43 * V.w <br>
R.w = M._14 * V.x + M._24 * V.y + M._34 * V.z + M._44 * V.w <br>

@param V	The vector to transform.
@return The transformed vector.
*/
//*****************************************************************************
XVector4D XMatrix4x4::operator * (XVector4D& V) 
{
	return XVector4D(_11 * V.x + _21 * V.y + _31 * V.z + _41 * V.w,
					  _12 * V.x + _22 * V.y + _32 * V.z + _42 * V.w,
                      _13 * V.x + _23 * V.y + _33 * V.z + _43 * V.w,
                      _14 * V.x + _24 * V.y + _34 * V.z + _44 * V.w);
}



//*****************************************************************************
/**
Transforms an XVector3D object by an XMatrix4x4 object according to the
following formula where M is the XMatrix4x4 object, V is the XVector3D object
to transform, and R is the resulting XVector3D object.  

R.x = v.x * _11 + v.y * _21 + v.z * _31 + _41 <br>
R.y = v.x * _12 + v.y * _22 + v.z * _32 + _42 <br>
R.z = v.x * _13 + v.y * _23 + v.z * _33 + _43 <br>

@param V	The vector to transform.
@return The transformed vector.
*/
//*****************************************************************************
XVector3D XMatrix4x4::operator * (XVector3D& V) 
{
	return XVector3D(V.x * _11 + V.y * _21 + V.z * _31 + _41,
					  V.x * _12 + V.y * _22 + V.z * _32 + _42,
                      V.x * _13 + V.y * _23 + V.z * _33 + _43);
}



//*****************************************************************************
/**
Calculates the determinant of an XMatrix4x4 object according to the following
formula where M is the matrix to calculate the determinant for, and R is the
resulting determinant.

R = 
_12 * _24 * _33 * _41 - _12 * _23 * _34 * _41 -	_11 * _24 * _33 * _42 +  <br>      
_11 * _23 * _34 * _42 - _12 * _24 * _31 * _43 + _11 * _24 * _32 * _43 +  <br>         
_12 * _21 * _34 * _43 -	_11 * _22 * _34 * _43 +  <br>

_14 * (_23 * _32 * _41 - _22 * _33 * _41 - _23 * _31 * _42 + _21 * _33 * _42 +  <br>
_22 * _31 * _43 - _21 * _32 * _43) + <br>

_44 * (_12 * _23 * _31 - _11 * _23 * _32 - _12 * _21 * _33 + _11 * _22 * _33) + <br>

_13 * (-_24 * _32 * _41 + _22 * _34 * _41 +	_24 * _31 * _42 -_21 * _34 * _42 - <br>
_22 * _31 * _44 + _21 * _32 * _44) <br>

@return The determinant of the matrix.
*/
//*****************************************************************************
XF32 XMatrix4x4::getDeterminant() 
{
	return _12 * _24 * _33 * _41 -           
			_12 * _23 * _34 * _41 -           
			_11 * _24 * _33 * _42 +           
			_11 * _23 * _34 * _42 -           
			_12 * _24 * _31 * _43 +           
			_11 * _24 * _32 * _43 +           
			_12 * _21 * _34 * _43 -           
			_11 * _22 * _34 * _43 + 

			_14 * (_23 * _32 * _41 - _22 * _33 * _41 - 
					_23 * _31 * _42 + _21 * _33 * _42 + 
					_22 * _31 * _43 - _21 * _32 * _43) +

			_44 * (_12 * _23 * _31 - 
					_11 * _23 * _32 - 
					_12 * _21 * _33 + 
					_11 * _22 * _33) +

			_13 * (-_24 * _32 * _41 +
					_22 * _34 * _41 +
					_24 * _31 * _42 -
					_21 * _34 * _42 -
					_22 * _31 * _44 + 
					_21 * _32 * _44);  
}

void XMatrix4x4::setZero()
{
	_11 = 0.0f;_12 = 0.0f;_13 = 0.0f;_14 = 0.0f;
	_21 = 0.0f;_22 = 0.0f;_23 = 0.0f;_24 = 0.0f;
	_31 = 0.0f;_32 = 0.0f;_33 = 0.0f;_34 = 0.0f;
	_41 = 0.0f;_42 = 0.0f;_43 = 0.0f;_44 = 0.0f;
}

XBOOL XMatrix4x4::isZero()
{
	return (X_MATH_IS_XF32_ZERO(_11) &&
			X_MATH_IS_XF32_ZERO(_12) &&
			X_MATH_IS_XF32_ZERO(_13) &&
			X_MATH_IS_XF32_ZERO(_14) &&
			X_MATH_IS_XF32_ZERO(_21) &&
			X_MATH_IS_XF32_ZERO(_22) &&
			X_MATH_IS_XF32_ZERO(_23) &&
			X_MATH_IS_XF32_ZERO(_24) &&
			X_MATH_IS_XF32_ZERO(_31) &&
			X_MATH_IS_XF32_ZERO(_32) &&
			X_MATH_IS_XF32_ZERO(_33) &&
			X_MATH_IS_XF32_ZERO(_34) &&
			X_MATH_IS_XF32_ZERO(_41) &&
			X_MATH_IS_XF32_ZERO(_42) &&
			X_MATH_IS_XF32_ZERO(_43) &&
			X_MATH_IS_XF32_ZERO(_44));
}

//*****************************************************************************
/**
Gets the inverse of an XMatrix4x4 object. The inverse can only be 
calculated if the determinant of the matrix in not 0.  If the determinant is
0, this function will fail.  The following formula is used where M is the 
matrix to invert, and R is the resulting matrix.  

d = determinant of matrix M <br>
R._11 = (-M._24 * M._33 * M._42 + M._23 * M._34 * M._42 + M._24 * M._32 * M._43 - M._22 * M._34 * M._43 - M._23 * M._32 * M._44 + M._22 * M._33 * M._44) / d <br>
R._12 = (M._14 * M._33 * M._42 - M._13 * M._34 * M._42 - M._14 * M._32 * M._43 + M._12 * M._34 * M._43 + M._13 * M._32 * M._44 - M._12 * M._33 * M._44) / d <br>
R._13 = (-M._14 * M._23 * M._42 + M._13 * M._24 * M._42 + M._14 * M._22 * M._43 - M._12 * M._24 * M._43 - M._13 * M._22 * M._44 + M._12 * M._23 * M._44) / d <br>
R._14 = (M._14 * M._23 * M._32 - M._13 * M._24 * M._32 - M._14 * M._22 * M._33 + M._12 * M._24 * M._33 + M._13 * M._22 * M._34 - M._12 * M._23 * M._34) / d <br>
R._21 = (M._24 * M._33 * M._41 - M._23 * M._34 * M._41 - M._24 * M._31 * M._43 + M._21 * M._34 * M._43 + M._23 * M._31 * M._44 - M._21 * M._33 * M._44) / d <br>
R._22 = (-M._14 * M._33 * M._41 + M._13 * M._34 * M._41 + M._14 * M._31 * M._43 - M._11 * M._34 * M._43 - M._13 * M._31 * M._44 + M._11 * M._33 * M._44) / d <br>
R._23 = (M._14 * M._23 * M._41 - M._13 * M._24 * M._41 - M._14 * M._21 * M._43 + M._11 * M._24 * M._43 + M._13 * M._21 * M._44 - M._11 * M._23 * M._44) / d <br>
R._24 = (-M._14 * M._23 * M._31 + M._13 * M._24 * M._31 + M._14 * M._21 * M._33 - M._11 * M._24 * M._33 - M._13 * M._21 * M._34 + M._11 * M._23 * M._34) / d <br>
R._31 = (-M._24 * M._32 * M._41 + M._22 * M._34 * M._41 + M._24 * M._31 * M._42 - M._21 * M._34 * M._42 - M._22 * M._31 * M._44 + M._21 * M._32 * M._44) / d <br>
R._32 = (M._14 * M._32 * M._41 - M._12 * M._34 * M._41 - M._14 * M._31 * M._42 + M._11 * M._34 * M._42 + M._12 * M._31 * M._44 - M._11 * M._32 * M._44) / d <br>
R._33 = (-M._14 * M._22 * M._41 + M._12 * M._24 * M._41 + M._14 * M._21 * M._42 - M._11 * M._24 * M._42 - M._12 * M._21 * M._44 + M._11 * M._22 * M._44) / d <br>
R._34 = (M._14 * M._22 * M._31 - M._12 * M._24 * M._31 - M._14 * M._21 * M._32 + M._11 * M._24 * M._32 + M._12 * M._21 * M._34 - M._11 * M._22 * M._34) / d <br>
R._41 = (M._23 * M._32 * M._41 - M._22 * M._33 * M._41 - M._23 * M._31 * M._42 + M._21 * M._33 * M._42 + M._22 * M._31 * M._43 - M._21 * M._32 * M._43) / d <br>
R._42 = (-M._13 * M._32 * M._41 + M._12 * M._33 * M._41 + M._13 * M._31 * M._42 - M._11 * M._33 * M._42 - M._12 * M._31 * M._43 + M._11 * M._32 * M._43) / d <br>
R._43 = (M._13 * M._22 * M._41 - M._12 * M._23 * M._41 - M._13 * M._21 * M._42 + M._11 * M._23 * M._42 + M._12 * M._21 * M._43 - M._11 * M._22 * M._43) / d <br>
R._44 = (-M._13 * M._22 * M._31 + M._12 * M._23 * M._31 + M._13 * M._21 * M._32 - M._11 * M._23 * M._32 - M._12 * M._21 * M._33 + M._11 * M._22 * M._33) / d <br>

@return The inverted matrix.
@remark This function will not modify the XMatrix4x4 object.
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::getInverse() 
{
	XMatrix4x4 R;
	XF32 d = getDeterminant();

	//Can't use a determinant of 0
	if (d == 0.0f)
	{
		X_ASSERT("matrix has no inverse because determinant was 0");
		return R;
	}
	

	R._11 = (-_24 * _33 * _42 + _23 * _34 * _42 + _24 * _32 * _43 - _22 * _34 * _43 - _23 * _32 * _44 + _22 * _33 * _44) / d;
	R._12 = (_14 * _33 * _42 - _13 * _34 * _42 - _14 * _32 * _43 + _12 * _34 * _43 + _13 * _32 * _44 - _12 * _33 * _44) / d;
	R._13 = (-_14 * _23 * _42 + _13 * _24 * _42 + _14 * _22 * _43 - _12 * _24 * _43 - _13 * _22 * _44 + _12 * _23 * _44) / d;
	R._14 = (_14 * _23 * _32 - _13 * _24 * _32 - _14 * _22 * _33 + _12 * _24 * _33 + _13 * _22 * _34 - _12 * _23 * _34) / d;

	R._21 = (_24 * _33 * _41 - _23 * _34 * _41 - _24 * _31 * _43 + _21 * _34 * _43 + _23 * _31 * _44 - _21 * _33 * _44) / d;
	R._22 = (-_14 * _33 * _41 + _13 * _34 * _41 + _14 * _31 * _43 - _11 * _34 * _43 - _13 * _31 * _44 + _11 * _33 * _44) / d;
	R._23 = (_14 * _23 * _41 - _13 * _24 * _41 - _14 * _21 * _43 + _11 * _24 * _43 + _13 * _21 * _44 - _11 * _23 * _44) / d;
	R._24 = (-_14 * _23 * _31 + _13 * _24 * _31 + _14 * _21 * _33 - _11 * _24 * _33 - _13 * _21 * _34 + _11 * _23 * _34) / d;

	R._31 = (-_24 * _32 * _41 + _22 * _34 * _41 + _24 * _31 * _42 - _21 * _34 * _42 - _22 * _31 * _44 + _21 * _32 * _44) / d;
	R._32 = (_14 * _32 * _41 - _12 * _34 * _41 - _14 * _31 * _42 + _11 * _34 * _42 + _12 * _31 * _44 - _11 * _32 * _44) / d;
	R._33 = (-_14 * _22 * _41 + _12 * _24 * _41 + _14 * _21 * _42 - _11 * _24 * _42 - _12 * _21 * _44 + _11 * _22 * _44) / d;
	R._34 = (_14 * _22 * _31 - _12 * _24 * _31 - _14 * _21 * _32 + _11 * _24 * _32 + _12 * _21 * _34 - _11 * _22 * _34) / d;

	R._41 = (_23 * _32 * _41 - _22 * _33 * _41 - _23 * _31 * _42 + _21 * _33 * _42 + _22 * _31 * _43 - _21 * _32 * _43) / d;
	R._42 = (-_13 * _32 * _41 + _12 * _33 * _41 + _13 * _31 * _42 - _11 * _33 * _42 - _12 * _31 * _43 + _11 * _32 * _43) / d;
	R._43 = (_13 * _22 * _41 - _12 * _23 * _41 - _13 * _21 * _42 + _11 * _23 * _42 + _12 * _21 * _43 - _11 * _22 * _43) / d;
	R._44 = (-_13 * _22 * _31 + _12 * _23 * _31 + _13 * _21 * _32 - _11 * _23 * _32 - _12 * _21 * _33 + _11 * _22 * _33) / d;

	return R;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XMatrix4x4::invert()
{
	(*this) = this->getInverse();	
	return (*this);
}



//*****************************************************************************
/**
Returns a quaternion representation of an XMatrix4x4 object.

@return The quaternion representation of the matrix.
*/
//*****************************************************************************
XQuaternion XMatrix4x4::toQuat()
{
	XF32 trace = _11 + _22 + _33 + _44;
	XF32 sqrtTrace = sqrt(trace);
	XF32 tempTrace = 1.0f/(2.0f * sqrtTrace);

	XQuaternion q((_23-_32) * tempTrace,
				   (_31-_13) * tempTrace,
				   (_12-_21) * tempTrace,
				   sqrtTrace/2.0f);

	q.normalize();
	return q;
}



//*****************************************************************************
/**
Gets a transposed version of an XMatrix4x4 object.  

@return A transposed version of the matrix.
@remark This function will not modify the XMatrix4x4 object.
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::getTranspose() 
{
	XMatrix4x4 R;

	for (XU32 i = 0;i < 4;i++) 
		for (XU32 j = 0;j < 4;j++) 
			R.array[i][j] = array[j][i];

	return R;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XMatrix4x4::transpose() 
{
	(*this) = this->getTranspose();
	return (*this);
}



//*****************************************************************************
/**
Transforms an XVector3D object by an XMatrix4x4 object.  This function will
convert the 3D vector into a 4D vector and then call transformVector4D().  It
then converts the return value from transformVector4D() back into a 3D vector
and passes it back. See transformVector4D() for more information.

@param V	The vector to transform.
@return	The transformed vector.
*/
//*****************************************************************************
XVector3D XMatrix4x4::transformVector3D(XVector3D& V)
{
	XVector4D temp4DVec = XVector4D(V);
	temp4DVec = this->transformVector4D(temp4DVec);
	return XVector3D(temp4DVec);
}



//*****************************************************************************
/**
Transforms an XVector4D object by an XMatrix4x4 object according to the
following formula where V is the vector to transform, M is the matrix to
transform by, and R is the resulting transformed vector.

R.x = M._11 * V.x + M._21 * V.y + M._31 * V.z + M._41 * V.w <br>
R.y = M._12 * V.x + M._22 * V.y + M._32 * V.z + M._42 * V.w <br>
R.z = M._13 * V.x + M._23 * V.y + M._33 * V.z + M._43 * V.w <br>
R.w = M._14 * V.x + M._24 * V.y + M._34 * V.z + M._44 * V.w <br>

@param V	The vector to transform.
@return	The transformed vector.
*/
//*****************************************************************************
XVector4D XMatrix4x4::transformVector4D(XVector4D& v)
{
	return XVector4D(_11 * v.x + _21 * v.y + _31 * v.z + _41 * v.w,
					  _12 * v.x + _22 * v.y + _32 * v.z + _42 * v.w,
                      _13 * v.x + _23 * v.y + _33 * v.z + _43 * v.w,
                      _14 * v.x + _24 * v.y + _34 * v.z + _44 * v.w);
}



//*****************************************************************************
/**
Orthogonalizes the XMatrix4x4 object. 

@return None.
@remark This function will modify this XMatrix4x4 object.
*/
//*****************************************************************************
void XMatrix4x4::orthogonalize() 
{
	XMatrix4x4 R;
	XVector4D V,PrevVector,VPrime;
	getRow(0,V);
	R.setRow(0,V / V.mag());

	for (XU32 Row = 1;Row < 4;Row++) 
	{
		getRow(Row,V);
		VPrime = V;

		for (XU32 Term = 0;Term < Row;Term++) 
		{
			R.getRow (Term,PrevVector);
			VPrime -= V.projectOnto(PrevVector);
		}

		R.setRow (Row,VPrime);
	}

	(*this) = R;
}



//*****************************************************************************
/**
Orthonormalizes the XMatrix4x4 object. 

@return None.
@remark This function will modify this XMatrix4x4 object.
*/
//*****************************************************************************
void XMatrix4x4::orthonormalize() 
{
	XVector3D right(_11,_12,_13);
	XVector3D up(_21,_22,_23);
	XVector3D look(_31,_32,_33);
	XVector3D temp;

	temp = (up.cross(look)).normalize();
	_11 = temp.x;
	_12 = temp.y;
	_13 = temp.z;

	temp = (look.cross(right)).normalize();
	_21 = temp.x;
	_22 = temp.y;
	_23 = temp.z;

	temp = look.normalize();
	_31 = temp.x;
	_32 = temp.y;
	_33 = temp.z;
}



//*****************************************************************************
/**
Sets a single row in the XMatrix4x4 object.  

@param row	The 0-based row number to set.  This value ranges from 0-3.
@param V	The XVector4D that specifies the component values for the row
			being set.
@return True if the operation was successful, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix4x4::setRow(XU32 row,XVector4D& V) 
{
	if (row < 0 || row >= 4)
		return false;

	for (XU32 i = 0;i < 4;i++)
		array[row][i] = V.array[i];

	return true;
}



//*****************************************************************************
/**
Gets a single row in the XMatrix4x4 object.  

@param row	The 0-based row number to get.  This value ranges from 0-3.
@param V	The XVector4D that will hold the component values for the
			requested row.
@return True if the operation was successful, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix4x4::getRow(XU32 row,XVector4D& V) 
{
   if (row < 0 || row >= 4)
      return false;

   for (XU32 i = 0;i < 4;i++)
      V.array[i] = array[row][i];

   return true;
}



//*****************************************************************************
/**
Sets the XMatrix4x4 object to the 4x4 identity as shown below.

[1 0 0 0] <br>
[0 1 0 0] <br>
[0 0 1 0] <br>
[0 0 0 1] <br>

@return None.
*/
//*****************************************************************************
void XMatrix4x4::setIdentity()
{
	_11 = 1;_12 = 0;_13 = 0;_14 = 0;
	_21 = 0;_22 = 1;_23 = 0;_24 = 0;
	_31 = 0;_32 = 0;_33 = 1;_34 = 0;
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;
}



XBOOL XMatrix4x4::isIdentity()
{
	return (_11 == 1.0f &&
			X_MATH_IS_XF32_ZERO(_12) &&
			X_MATH_IS_XF32_ZERO(_13) &&
			X_MATH_IS_XF32_ZERO(_14) &&
			X_MATH_IS_XF32_ZERO(_21) &&
			_22 == 1.0f &&
			X_MATH_IS_XF32_ZERO(_23) &&
			X_MATH_IS_XF32_ZERO(_24) &&
			X_MATH_IS_XF32_ZERO(_31) &&
			X_MATH_IS_XF32_ZERO(_32) &&
			_33 == 1.0f &&
			X_MATH_IS_XF32_ZERO(_34) &&
			X_MATH_IS_XF32_ZERO(_41) &&
			X_MATH_IS_XF32_ZERO(_42) &&
			X_MATH_IS_XF32_ZERO(_43) &&
			_44 == 1.0f);
}


//*****************************************************************************
/**
Creates an XMatrix4x4 object that transforms around the X axis by the specified
number of radians.

@param radians	The number of radians to rotate around the X axis.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createXRotation(XF32 radians)
{
	_11 = 1;_12 = 0;_13 = 0;_14 = 0;
	_21 = 0;                _24 = 0;
	_31 = 0;                _34 = 0;
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;

	_22 =  (XF32)cos(radians);
	_23 =  (XF32)sin(radians);
	_32 = -(XF32)sin(radians);
	_33 =  (XF32)cos(radians);
}



//*****************************************************************************
/**
Creates an XMatrix4x4 object that transforms around the Y axis by the specified
number of radians.

@param radians	The number of radians to rotate around the Y axis.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createYRotation(XF32 radians)
{
	        _12 = 0;        _14 = 0;
	_21 = 0;_22 = 1;_23 = 0;_24 = 0;
	        _32 = 0;        _34 = 0;
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;

	_11 =  (XF32)cos(radians);
	_13 = -(XF32)sin(radians);
	_31 =  (XF32)sin(radians);
	_33 =  (XF32)cos(radians);
}



//*****************************************************************************
/**
Creates an XMatrix4x4 object that transforms around the Z axis by the specified
number of radians..

@param radians	The number of radians to rotate around the Z axis.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createZRotation(XF32 radians)
{
	                _13 = 0;_14 = 0;
	                _23 = 0;_24 = 0;
	_31 = 0;_32 = 0;_33 = 1;_34 = 0;
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;

	_11  =  (XF32)cos(radians);
	_12  =  (XF32)sin(radians);
	_21  = -(XF32)sin(radians);
	_22  =  (XF32)cos(radians);
}



//*****************************************************************************
//*****************************************************************************
void XMatrix4x4::createTranslation(XVector3D& translationVec)
{
	_11 = 1;_12 = 0;_13 = 0;_14 = 0;
	_21 = 0;_22 = 1;_23 = 0;_24 = 0;
	_31 = 0;_32 = 0;_33 = 1;_34 = 0;
	                        _44 = 1;

	_41 = translationVec.x;
	_42 = translationVec.y;
	_43 = translationVec.z;
}
	
	

//*****************************************************************************
//*****************************************************************************
void XMatrix4x4::createScale(XVector3D& scaleVec)
{
            _12 = 0;_13 = 0;_14 = 0;
	_21 = 0;        _23 = 0;_24 = 0;
	_31 = 0;_32 = 0;        _34 = 0;
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;

	_11 = scaleVec.x;
	_22 = scaleVec.y;
	_33 = scaleVec.z;
}


XF32 XMatrix4x4::getXScale()
{
	return (XF32) sqrt((_11 * _11) + (_12 * _12) + (_13 * _13));
}

void XMatrix4x4::setXScale(XF32 scale)
{
	XF32 mag = (XF32) sqrt((_11 * _11) + (_12 * _12) + (_13 * _13));	
	_11 *= scale / mag;
	_12 *= scale / mag;
	_13 *= scale / mag;
}

XF32 XMatrix4x4::getYScale()
{
	return (XF32) sqrt((_21 * _21) + (_22 * _22) + (_23 * _23));	
}

void XMatrix4x4::setYScale(XF32 scale)
{
	XF32 mag = (XF32) sqrt((_21 * _21) + (_22 * _22) + (_23 * _23));	
	_21 *= scale / mag;
	_22 *= scale / mag;
	_23 *= scale / mag;
}

XF32 XMatrix4x4::getZScale()
{
	return (XF32) sqrt((_31 * _31) + (_32 * _32) + (_33 * _33));	
}

void XMatrix4x4::setZScale(XF32 scale)
{
	XF32 mag = (XF32) sqrt((_31 * _31) + (_32 * _32) + (_33 * _33));	
	_31 *= scale / mag;
	_32 *= scale / mag;
	_33 *= scale / mag;
}


//*****************************************************************************
/**
Creates an XMatrix4x4 object that transforms around an arbitrary axis by the
specified number of radians.

@param axis	The axis to rotate around.
@param radians	The number of radians to rotate around the arbitrary axis.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createArbRotation(XVector3D& axis,XF32 radians)
{
	XF32 cosValue = (XF32)cos(radians);
	XF32 sinValue = (XF32)sin(radians);
	axis.normalize();

	_11 = (axis.x*axis.x) * (1.0f-cosValue) + cosValue;
	_12 = (axis.y*axis.x) * (1.0f-cosValue) + (axis.z*sinValue);
	_13 = (axis.z*axis.x) * (1.0f-cosValue) - (axis.y*sinValue);
	_14 = 0;
	
	_21 = (axis.x*axis.y) * (1.0f-cosValue) - (axis.z*sinValue);
	_22 = (axis.y*axis.y) * (1.0f-cosValue) + cosValue;
	_23 = (axis.z*axis.y) * (1.0f-cosValue) + (axis.x*sinValue);
	_24 = 0;
	
	_31 = (axis.x*axis.z) * (1.0f-cosValue) + (axis.y*sinValue);
	_32 = (axis.y*axis.z) * (1.0f-cosValue) - (axis.x*sinValue);
	_33 = (axis.z*axis.z) * (1.0f-cosValue) + cosValue;
	_34 = 0;

	_41 = 0;
	_42 = 0;
	_43 = 0;
	_44 = 1;
}



//*****************************************************************************
/**
Creates a left-handed coordinate system projection matrix.

@param zNearClip The near clipping plane's z coordinate.
@param zFarClip The far clipping plane's z coordinate.
@param fov The field of view.
@param aspect The aspect ratio.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createProjLH(XF32 zNearClip,XF32 zFarClip,XF32 fov,
								  XF32 aspect)
{

	//D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)this, fov, aspect, zNearClip, zFarClip );

	/*XF32 h = cos(fov/2) / sin(fov/2);
	XF32 w = h/aspect;
	
	_11 = 2*zNearClip/w;_12 = 0;_13 = 0;_14 = 0;
	_21 = 0;_22 = 2*zNearClip/h;_23 = 0;_24 = 0;
	_31 = 0;_32 = 0;_33 = zFarClip/(zFarClip-zNearClip);_34 = 1;
	_41 = 0;_42 = 0;_43 = zNearClip*zFarClip/(zNearClip-zFarClip);_44 = 0;
*/
	XF32 h = aspect * ( cosf(fov/2.0f)/sinf(fov/2.0f));
	XF32 w = cosf(fov/2.0f)/sinf(fov/2.0f);
	XF32 Q = zFarClip / ( zFarClip - zNearClip );
	
	        _12 = 0;_13 = 0;_14 = 0;
	_21 = 0;        _23 = 0;_24 = 0;
	_31 = 0;_32 = 0;        
	_41 = 0;_42 = 0;        _44 = 1;

	_11 = w;
	_22 = h;
	_33 = Q;
	_34 = 1.0f;
	_43 = -Q*zNearClip;
}



//*****************************************************************************
/**
Creates a right-handed coordinate system projection matrix.

@param zNearClip The near clipping plane's z coordinate.
@param zFarClip The far clipping plane's z coordinate.
@param fov The field of view.
@param aspect The aspect ratio.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createProjRH(XF32 zNearClip,XF32 zFarClip,XF32 fov,
								  XF32 aspect)
{
	XF32 h = cos(fov/2) / sin(fov/2);
	XF32 w = h/aspect;
	
	_11 = 2*zNearClip/w;_12 = 0;_13 = 0;_14 = 0;
	_21 = 0;_22 = 2*zNearClip/h;_23 = 0;_24 = 0;
	_31 = 0;_32 = 0;_33 = zFarClip/(zNearClip-zFarClip);_34 = -1;
	_41 = 0;_42 = 0;_43 = zNearClip*zFarClip/(zNearClip-zFarClip);_44 = 0;
}



//*****************************************************************************
/**
Creates a left-handed coordinate system orthographic projection matrix.

@param zNearClip The near clipping plane's z coordinate.
@param zFarClip The far clipping plane's z coordinate.
@param fov The field of view.
@param aspect The aspect ratio.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::setOrthoProjectionLH(XF32 zNearClip,XF32 zFarClip,XF32 viewportWidth,
								 XF32 viewportHeight)
{	
	        _12 = 0;_13 = 0;_14 = 0;
	_21 = 0;        _23 = 0;_24 = 0;
	_31 = 0;_32 = 0;        
	_41 = 0;_42 = 0;_43 = 0;_44 = 1;

	_11 = 2.0f / viewportWidth;
	_22 = 2.0f / viewportHeight;
	_33 = 1.0f / (zFarClip - zNearClip);
	_34 = -zNearClip / (zFarClip - zNearClip);
}



//*****************************************************************************
/**
Creates a view matrix.

@param right The viewpoint's right vector.
@param up The viewpoint's up vector.
@param look The viewpoint's look vector.
@param pos The viewpoint's position.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createView(XVector3D& right,XVector3D& up,XVector3D& look,XVector3D& pos)
{
	_11 = right.x; 
	_21 = right.y; 
	_31 = right.z; 
	_12 = up.x;
	_22 = up.y;
	_32 = up.z;
	_13 = look.x;
	_23 = look.y;
	_33 = look.z;
	_41 = - pos.dot(right);
	_42 = - pos.dot(up);
	_43 = - pos.dot(look);
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;
}



//*****************************************************************************
/**
Creates a view matrix.

@param matrix The viewpoint's matrix.
@return None.
*/
//*****************************************************************************
void XMatrix4x4::createView(XMatrix4x4& matrix)
{
	/*
	XVector3D right(matrix._11,matrix._12,matrix._13);
	XVector3D up(matrix._21,matrix._22,matrix._23);
	XVector3D look(matrix._31,matrix._32,matrix._33);
	XVector3D pos(matrix._41,matrix._42,matrix._43);
	_11 = right.x; 
	_21 = right.y; 
	_31 = right.z; 
	_12 = up.x;
	_22 = up.y;
	_32 = up.z;
	_13 = look.x;
	_23 = look.y;
	_33 = look.z;
	_41 = - pos.dot(right);
	_42 = - pos.dot(up);
	_43 = - pos.dot(look);
	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;
	*/

	XVector3D right(matrix._11,matrix._12,matrix._13);
	XVector3D up(matrix._21,matrix._22,matrix._23);
	XVector3D look(matrix._31,matrix._32,matrix._33);
	XVector3D pos(matrix._41,matrix._42,matrix._43);
	_11 = matrix._11; 
	_21 = matrix._12; 
	_31 = matrix._13; 
	_12 = matrix._21;
	_22 = matrix._22;
	_32 = matrix._23;
	_13 = matrix._31;
	_23 = matrix._32;
	_33 = matrix._33;

	//Negative dot product
	_41 = - ((matrix._41 * matrix._11) + (matrix._42 * matrix._12) + (matrix._43 * matrix._13));      
	_42 = - ((matrix._41 * matrix._21) + (matrix._42 * matrix._22) + (matrix._43 * matrix._23));  
	_43 = - ((matrix._41 * matrix._31) + (matrix._42 * matrix._32) + (matrix._43 * matrix._33));  

	_14 = 0;
	_24 = 0;
	_34 = 0;
	_44 = 1;


}	



//*****************************************************************************
/**
Creates a left-handed coordinate system look at matrix.

@param from The viewpoint position.
@param at The view target.
@return None.
*/
//*****************************************************************************
XMatrix4x4 XMatrix4x4::getObjectLookat(XVector3D& from,XVector3D& at)
{
 	XVector3D View = at - from;

	//If source and target are identical, we can't really do anything so return the
	//identity matrix
	if (View.mag() == 0.0f)
		return XMatrix4x4();
	//X_ASSERTM(View.mag() != 0.0f,"Can't generate look at matrix when from and at positions are identical.  This creates a 0 length view vector.");

	View.normalize();

	/*XF32 Length = View.mag();

	//We might be dividing by 0 if we generated a zero length vector from above
	//A zero length vector isn't a bad thing by itself, but we don't want
	//to divide by 0. If we determine that we're going to be dividing by 0,
	//don't divide.
	if (Length != 0.0f)
		View /= Length;*/
	

	XVector3D WorldUp = XVector3D(0,1,0);
	/*if (XVector3D(0,1,0).dot(View) == -1.0f)
		WorldUp = XVector3D(0,0,1);
	else
		WorldUp = XVector3D(0,1,0);*/
	XF32 DotProduct = WorldUp.dot(View);

	
	XVector3D Up = WorldUp - (View * DotProduct);

	//Check to see if we are looking straight down.  If so, this will
	//produce a case where we try to normalize a 0 length vector.  To
	//work around this, adjust the world up vector by a tad.  The results
	//aren't totally accurate
	/*if (Up.isZero())
	{
		WorldUp.x = .000001;
		Up = WorldUp - (View * DotProduct);
	}*/
	Up.normalize();

	XVector3D Right = Up.cross(View);

	XMatrix4x4 mat;
	mat._11 = Right.x;mat._21 = Up.x;mat._31 = View.x;
	mat._12 = Right.y;mat._22 = Up.y;mat._32 = View.y;
	mat._13 = Right.z;mat._23 = Up.z;mat._33 = View.z;

	mat._41 = from.x;
	mat._42 = from.y;
	mat._43 = from.z;

	return mat;


}