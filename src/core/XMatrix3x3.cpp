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
//	Module Name:	XMatrix3x3.cpp
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
Constructs an XMatrix3x3 object by initializing the matrix to the 3x3 identity
below.

[1 0 0] <br>
[0 1 0] <br>
[0 0 1] <br>
*/
//*****************************************************************************
XMatrix3x3::XMatrix3x3()
{
	_11 = 1; _12 = 0; _13 = 0;
	_21 = 0; _22 = 1; _23 = 0;
	_31 = 0; _32 = 0; _33 = 1;
}



//*****************************************************************************
/**
Initializes an XMatrix3x3 object by intializing the matrix elements to the
passed in parameters.  

@param i11	Inital value of the element in Row 1 - Column 1.
@param i12	Inital value of the element in Row 1 - Column 2.
@param i13	Inital value of the element in Row 1 - Column 3.
@param i21	Inital value of the element in Row 2 - Column 1.
@param i22	Inital value of the element in Row 2 - Column 2.
@param i23	Inital value of the element in Row 2 - Column 3.
@param i31	Inital value of the element in Row 3 - Column 1.
@param i32	Inital value of the element in Row 3 - Column 2.
@param i33	Inital value of the element in Row 3 - Column 3.
*/
//*****************************************************************************
XMatrix3x3::XMatrix3x3(XF32 i11,XF32 i12,XF32 i13,
						 XF32 i21,XF32 i22,XF32 i23,
						 XF32 i31,XF32 i32,XF32 i33)
{
	_11 = i11; _12 = i12; _13 = i13;
	_21 = i21; _22 = i22; _23 = i23;
	_31 = i31; _32 = i32; _33 = i33;
}



//*****************************************************************************
/**
Determines if two XMatrix3x3 objects are equal.  For the XMatrix3x3 objects
to be equal, all elements must match between the matrices.

@param M	The matrix to compare with.
@return True if the matrices are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix3x3::operator == (XMatrix3x3& M) 
{
	XF32* M1 = &_11; 
	XF32* M2 = &M._11;

	for (XU32 i = 0;i < 9;i++) 
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
Determines if two XMatrix3x3 objects are unequal.  For the XMatrix3x3 objects
to be unequal, at least one element must differ between the matrices.

@param M	The matrix to compare with.
@return True if the matrices are unequal, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix3x3::operator != (XMatrix3x3& M) 
{
	XF32* M1 = &_11;
	XF32* M2 = &M._11;

	for (XU32 i = 0;i < 9;i++) 
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
Adds two XMatrix3x3 objects.  The matrices are added component wise
according to the following formula where M1 and M2 are the matrices to add,
and R is the resulting matrix.

R._11 = M1._11 + M2._11 <br>
R._12 = M1._12 + M2._12 <br>
R._13 = M1._13 + M2._13 <br>
R._21 = M1._21 + M2._21 <br>
R._22 = M1._22 + M2._22 <br>
R._23 = M1._23 + M2._23 <br>
R._31 = M1._31 + M2._31 <br>
R._32 = M1._32 + M2._32 <br>
R._33 = M1._33 + M2._33 <br>

@param	M The XMatrix3x3 object to add.  
@return	The sum of the two matrices.  
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::operator + (XMatrix3x3& M) 
{
	return XMatrix3x3(_11 + M._11,
					   _12 + M._12,
					   _13 + M._13,
					   _21 + M._21,
					   _22 + M._22,
					   _23 + M._23,
					   _31 + M._31,
					   _32 + M._32,
					   _33 + M._33);
}



//*****************************************************************************
/**
Adds two XMatrix3x3 objects.  The matrices are added component wise
according to the following formula where M1 and M2 are the matrices to add,
and R is the resulting matrix.  This XMatrix3x3 object is assigned the result.

R._11 = M1._11 + M2._11 <br>
R._12 = M1._12 + M2._12 <br>
R._13 = M1._13 + M2._13 <br>
R._21 = M1._21 + M2._21 <br>
R._22 = M1._22 + M2._22 <br>
R._23 = M1._23 + M2._23 <br>
R._31 = M1._31 + M2._31 <br>
R._32 = M1._32 + M2._32 <br>
R._33 = M1._33 + M2._33 <br>

@param	M The XMatrix3x3 object to add.  
@return	The sum of the two matrices.  
*/
//*****************************************************************************
XMatrix3x3 &XMatrix3x3::operator += (XMatrix3x3& M) 
{
	_11 += M._11;
	_12 += M._12;
	_13 += M._13;
	_21 += M._21;
	_22 += M._22;
	_23 += M._23;
	_31 += M._31;
	_32 += M._32;
	_33 += M._33;
	
	return *this;
}



//*****************************************************************************
/**
Subtracts two XMatrix3x3 objects.  The matrices are subtracted component wise
according to the following formula where M1 and M2 are the matrices to subtract,
and R is the resulting matrix.

R._11 = M1._11 - M2._11 <br>
R._12 = M1._12 - M2._12 <br>
R._13 = M1._13 - M2._13 <br>
R._21 = M1._21 - M2._21 <br>
R._22 = M1._22 - M2._22 <br>
R._23 = M1._23 - M2._23 <br>
R._31 = M1._31 - M2._31 <br>
R._32 = M1._32 - M2._32 <br>
R._33 = M1._33 - M2._33 <br>

@param	M The XMatrix3x3 object to subtract.  
@return	The difference of the two matrices.  
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::operator - (XMatrix3x3& M) 
{
	return XMatrix3x3(_11 - M._11,
					   _12 - M._12,
					   _13 - M._13,
					   _21 - M._21,
					   _22 - M._22,
					   _23 - M._23,
					   _31 - M._31,
					   _32 - M._32,
					   _33 - M._33);
}



//*****************************************************************************
/**
Subtracts two XMatrix3x3 objects.  The matrices are subtracted component wise
according to the following formula where M1 and M2 are the matrices to subtract,
and R is the resulting matrix.  This XMatrix3x3 object is assigned the result.

R._11 = M1._11 - M2._11 <br>
R._12 = M1._12 - M2._12 <br>
R._13 = M1._13 - M2._13 <br>
R._21 = M1._21 - M2._21 <br>
R._22 = M1._22 - M2._22 <br>
R._23 = M1._23 - M2._23 <br>
R._31 = M1._31 - M2._31 <br>
R._32 = M1._32 - M2._32 <br>
R._33 = M1._33 - M2._33 <br>

@param	M The XMatrix3x3 object to subtract.  
@return	The difference of the two matrices.  
*/
//*****************************************************************************
XMatrix3x3 &XMatrix3x3::operator -= (XMatrix3x3& M) 
{
	_11 -= M._11;
	_12 -= M._12;
	_13 -= M._13;
	_21 -= M._21;
	_22 -= M._22;
	_23 -= M._23;
	_31 -= M._31;
	_32 -= M._32;
	_33 -= M._33;
	
	return *this;
}



//*****************************************************************************
/**
Multiplies two XMatrix3x3 objects.  The matrices are multiplied according to
the following formula where M1 and M2 are the matrices to multiply, and R is
the resulting matrix.

R._11 = M1._11 * M2._11 + M1._12 * M2._21 + M1._13 * M2._31 <br>
R._12 = M1._11 * M2._12 + M1._12 * M2._22 + M1._13 * M2._32 <br>
R._13 = M1._11 * M2._13 + M1._12 * M2._23 + M1._13 * M2._33 <br>
R._21 = M1._21 * M2._11 + M1._22 * M2._21 + M1._23 * M2._31 <br>
R._22 = M1._21 * M2._12 + M1._22 * M2._22 + M1._23 * M2._32 <br>
R._23 = M1._21 * M2._13 + M1._22 * M2._23 + M1._23 * M2._33 <br>
R._31 = M1._31 * M2._11 + M1._32 * M2._21 + M1._33 * M2._31 <br>
R._32 = M1._31 * M2._12 + M1._32 * M2._22 + M1._33 * M2._32 <br>
R._33 = M1._31 * M2._13 + M1._32 * M2._23 + M1._33 * M2._33 <br>

@param	M The XMatrix3x3 object to multiply by.  
@return	The product of the two matrices.  
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::operator * (XMatrix3x3& M) 
{
	return XMatrix3x3(_11 * M._11 + _12 * M._21 + _13 * M._31,
			   		   _11 * M._12 + _12 * M._22 + _13 * M._32,
					   _11 * M._13 + _12 * M._23 + _13 * M._33,
					   _21 * M._11 + _22 * M._21 + _23 * M._31,
					   _21 * M._12 + _22 * M._22 + _23 * M._32,
					   _21 * M._13 + _22 * M._23 + _23 * M._33,
					   _31 * M._11 + _32 * M._21 + _33 * M._31,
					   _31 * M._12 + _32 * M._22 + _33 * M._32,
					   _31 * M._13 + _32 * M._23 + _33 * M._33);
}



//*****************************************************************************
/**
Multiplies two XMatrix3x3 objects.  The matrices are multiplied according to
the following formula where M1 and M2 are the matrices to multiply, and R is
the resulting matrix.  This XMatrix3x3 object is assigned the result.

R._11 = M1._11 * M2._11 + M1._12 * M2._21 + M1._13 * M2._31 <br>
R._12 = M1._11 * M2._12 + M1._12 * M2._22 + M1._13 * M2._32 <br>
R._13 = M1._11 * M2._13 + M1._12 * M2._23 + M1._13 * M2._33 <br>
R._21 = M1._21 * M2._11 + M1._22 * M2._21 + M1._23 * M2._31 <br>
R._22 = M1._21 * M2._12 + M1._22 * M2._22 + M1._23 * M2._32 <br>
R._23 = M1._21 * M2._13 + M1._22 * M2._23 + M1._23 * M2._33 <br>
R._31 = M1._31 * M2._11 + M1._32 * M2._21 + M1._33 * M2._31 <br>
R._32 = M1._31 * M2._12 + M1._32 * M2._22 + M1._33 * M2._32 <br>
R._33 = M1._31 * M2._13 + M1._32 * M2._23 + M1._33 * M2._33 <br>

@param	M The XMatrix3x3 object to multiply by.  
@return	The product of the two matrices.  
*/
//*****************************************************************************
XMatrix3x3 &XMatrix3x3::operator *= (XMatrix3x3& M) 
{
	XMatrix3x3 R;

	R._11 = _11 * M._11 + _12 * M._21 + _13 * M._31;
	R._12 = _11 * M._12 + _12 * M._22 + _13 * M._32;
	R._13 = _11 * M._13 + _12 * M._23 + _13 * M._33;

	R._21 = _21 * M._11 + _22 * M._21 + _23 * M._31;
	R._22 = _21 * M._12 + _22 * M._22 + _23 * M._32;
	R._23 = _21 * M._13 + _22 * M._23 + _23 * M._33;

	R._31 = _31 * M._11 + _32 * M._21 + _33 * M._31;
	R._32 = _31 * M._12 + _32 * M._22 + _33 * M._32;
	R._33 = _31 * M._13 + _32 * M._23 + _33 * M._33;

	(*this) = R;

	return *this;
}



//*****************************************************************************
/**
Multiplies an XMatrix3x3 object by a scalar.  The matrix and scalar are
multiplied according to the following formula where M is the matrix, S is the
scalar to multiply by, and R is the resulting matrix.

R._11 = M._11 * S <br>
R._12 = M._12 * S <br>
R._13 = M._13 * S <br>
R._21 = M._21 * S <br>
R._22 = M._22 * S <br>
R._23 = M._23 * S <br>
R._31 = M._31 * S <br>
R._32 = M._32 * S <br>
R._33 = M._33 * S <br>

@param	scalar The scalar to multiply by.  
@return	The product of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::operator * (XF32 scalar) 
{
	return XMatrix3x3(_11 * scalar,_12 * scalar,_13 * scalar,
					   _21 * scalar,_22 * scalar,_23 * scalar,
                       _31 * scalar,_32 * scalar,_33 * scalar);
}



//*****************************************************************************
/**
Multiplies an XMatrix3x3 object by a scalar.  The matrix and scalar are
multiplied according to the following formula where M is the matrix, S is the
scalar to multiply by, and R is the resulting matrix.  This XMatrix3x3 object
is assigned the result.

R._11 = M._11 * S <br>
R._12 = M._12 * S <br>
R._13 = M._13 * S <br>
R._21 = M._21 * S <br>
R._22 = M._22 * S <br>
R._23 = M._23 * S <br>
R._31 = M._31 * S <br>
R._32 = M._32 * S <br>
R._33 = M._33 * S <br>

@param	scalar The scalar to multiply by.  
@return	The product of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix3x3 &XMatrix3x3::operator *= (XF32 scalar) 
{
	_11 *= scalar;
	_12 *= scalar;
	_13 *= scalar;
    _21 *= scalar;
	_22 *= scalar;
	_23 *= scalar;
    _31 *= scalar;
	_32 *= scalar;
	_33 *= scalar;

	return *this;
}



//*****************************************************************************
/**
Divides an XMatrix3x3 object by a scalar.  The matrix and scalar are
divided according to the following formula where M is the matrix, S is the
scalar to divide by, and R is the resulting matrix.

R._11 = M._11 / S <br>
R._12 = M._12 / S <br>
R._13 = M._13 / S <br>
R._21 = M._21 / S <br>
R._22 = M._22 / S <br>
R._23 = M._23 / S <br>
R._31 = M._31 / S <br>
R._32 = M._32 / S <br>
R._33 = M._33 / S <br>

@param	scalar The scalar to divide by.  
@return	The quotient of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::operator / (XF32 scalar) 
{
	return XMatrix3x3(_11 / scalar,_12 / scalar,_13 / scalar,
					   _21 / scalar,_22 / scalar,_23 / scalar,
                       _31 / scalar,_32 / scalar,_33 / scalar);
}



//*****************************************************************************
/**
Divides an XMatrix3x3 object by a scalar.  The matrix and scalar are
divided according to the following formula where M is the matrix, S is the
scalar to divide by, and R is the resulting matrix.  This XMatrix3x3 object
is assigned the result.

R._11 = M._11 / S <br>
R._12 = M._12 / S <br>
R._13 = M._13 / S <br>
R._21 = M._21 / S <br>
R._22 = M._22 / S <br>
R._23 = M._23 / S <br>
R._31 = M._31 / S <br>
R._32 = M._32 / S <br>
R._33 = M._33 / S <br>

@param	scalar The scalar to divide by.  
@return	The quotient of the matrix and scalar.  
*/
//*****************************************************************************
XMatrix3x3 &XMatrix3x3::operator /= (XF32 scalar) 
{
	_11 /= scalar;
	_12 /= scalar;
	_13 /= scalar;
    _21 /= scalar;
	_22 /= scalar;
	_23 /= scalar;
    _31 /= scalar;
	_32 /= scalar;
	_33 /= scalar;

	return *this;
}



//*****************************************************************************
/**
Transforms an XVector3D object by an XMatrix3x3 object according to the
following formula where M is the XMatrix3x3 object, V is the XVector3D object
to transform, and R is the resulting XVector3D object.  This function assumes
the matrix is made up of column vectors.

R.x = M._11 * V.x + M._12 * V.y + M._13 * V.z <br>
R.y = M._21 * V.x + M._22 * V.y + M._23 * V.z <br>
R.z = M._31 * V.x + M._32 * V.y + M._33 * V.z <br>

@param V	The vector to transform.
@return The transformed vector.
*/
//*****************************************************************************
XVector3D XMatrix3x3::operator * (XVector3D& V) 
{
	//For column vectors
	return XVector3D(_11 * V.x + _12 * V.y + _13 * V.z,
					  _21 * V.x + _22 * V.y + _23 * V.z,
					  _31 * V.x + _32 * V.y + _33 * V.z);
}



//*****************************************************************************
/**
Scales an XMatrix3x3 object by an XVector3D object according to the following
formula where M is the matrix to scale, and V is the vector to scale with.

M._11 *= V.x; <br>
M._22 *= V.y; <br>
M._23 *= V.z; <br>

@param vec	The vector to scale with.
@return None.
@remark This function will modify the XMatrix3x3 object.
*/
//*****************************************************************************
void XMatrix3x3::scale(XVector3D& vec)
{
	_11 *= vec.x;
	_22 *= vec.y;
	_33 *= vec.z;
}



//*****************************************************************************
/**
Calculates the determinant of an XMatrix3x3 object according to the following
formula where M is the matrix to calculate the determinant for, and R is the
resulting determinant.

R = 
-M._13 * M._22 * M._31 + M._12 * M._23 * M._31 + M._13 * M._21 * M._32 - <br>
M._11 * M._23 * M._32 - M._12 * M._21 * M._33 + M._11 * M._22 * M._33 <br>

@return The determinant of the matrix.
*/
//*****************************************************************************
XF32 XMatrix3x3::getDeterminant() 
{
	return -_13 * _22 * _31 + 
			_12 * _23 * _31 + 
			_13 * _21 * _32 - 
			_11 * _23 * _32 - 
			_12 * _21 * _33 + 
			_11 * _22 * _33;
}

void XMatrix3x3::setZero()
{
	_11 = 0.0f;_12 = 0.0f;_13 = 0.0f;
	_21 = 0.0f;_22 = 0.0f;_23 = 0.0f;
	_31 = 0.0f;_32 = 0.0f;_33 = 0.0f;
}

XBOOL XMatrix3x3::isZero()
{
	return (X_MATH_IS_XF32_ZERO(_11) &&
			X_MATH_IS_XF32_ZERO(_12) &&
			X_MATH_IS_XF32_ZERO(_13) &&
			X_MATH_IS_XF32_ZERO(_21) &&
			X_MATH_IS_XF32_ZERO(_22) &&
			X_MATH_IS_XF32_ZERO(_23) &&
			X_MATH_IS_XF32_ZERO(_31) &&
			X_MATH_IS_XF32_ZERO(_32) &&
			X_MATH_IS_XF32_ZERO(_33));
}


//*****************************************************************************
/**
Gets the inverse of an XMatrix3x3 object. The inverse can only be 
calculated if the determinant of the matrix in not 0.  If the determinant is
0, this function will fail.  The following formula is used where M is the 
matrix to invert, and R is the resulting matrix.  

d = determinant of matrix M <br>
R._11 = (-M._23 * M._32 + M._22 * M._33) / d <br>
R._12 = (M._13 * M._32 - M._12 * M._33) / d <br>
R._13 = (-M._13 * M._22 + M._12 * M._23) / d <br>
R._21 = (M._23 * M._31 - M._21 * M._33) / d <br>
R._22 = (-M._13 * M._31 + M._11 * M._33) / d <br>
R._23 = (M._13 * M._21 - M._11 * M._23) / d <br>
R._31 = (-M._22 * M._31 + M._21 * M._32) / d <br>
R._32 = (M._12 * M._31 - M._11 * M._32) / d <br>
R._33 = (-M._12 * M._21 + M._11 * M._22) / d <br>

@return The inverted matrix.
@remark This function will not modify the XMatrix3x3 object.
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::getInverse() 
{
	XMatrix3x3 R;
	XF32 d = getDeterminant();

	if (d == 0) 
		throw logic_error("Determinant = 0!");

	R._11 = (-_23 * _32 + _22 * _33) / d;
	R._12 = (_13 * _32 - _12 * _33) / d;
	R._13 = (-_13 * _22 + _12 * _23) / d;
	R._21 = (_23 * _31 - _21 * _33) / d;
	R._22 = (-_13 * _31 + _11 * _33) / d;
	R._23 = (_13 * _21 - _11 * _23) / d;
	R._31 = (-_22 * _31 + _21 * _32) / d;
	R._32 = (_12 * _31 - _11 * _32) / d;
	R._33 = (-_12 * _21 + _11 * _22) / d;

	return R;
}



//*****************************************************************************
//*****************************************************************************
XMatrix3x3& XMatrix3x3::invert()
{
	(*this) = this->getInverse();
	return (*this);
}



//*****************************************************************************
/**
Gets a transposed version of an XMatrix3x3 object.  

@return A transposed version of the matrix.
@remark This function will not modify the XMatrix3x3 object.
*/
//*****************************************************************************
XMatrix3x3 XMatrix3x3::getTranspose() 
{
	XMatrix3x3 R;

	for (XU32 i = 0;i < 3;i++) 
		for (XU32 j = 0;j < 3;j++) 
			R.array[j][i] = array[i][j];

	return R;
}



//*****************************************************************************
//*****************************************************************************
XMatrix3x3& XMatrix3x3::transpose()
{
	(*this) = this->getTranspose();
	return (*this);
}



//*****************************************************************************
/**
Orthogonalizes the XMatrix3x3 object. 

@return None.
@remark This function will modify this XMatrix3x3 object.
*/
//*****************************************************************************
void XMatrix3x3::orthogonalize() 
{
	XMatrix3x3 R;
	XVector3D V,PrevVector,VPrime;

	getRow(0,V);
	R.setRow(0,V / V.mag());

	for (XU32 Row = 1;Row < 3;Row++) 
	{
		getRow(Row,V);
		VPrime = V;

		for (XU32 Term = 0;Term < Row;Term++) 
		{
			R.getRow(Term,PrevVector);
			VPrime -= V.projectOnto(PrevVector);
		}

		R.setRow(Row,VPrime);
	}

	(*this) = R;
}



//*****************************************************************************
/**
Sets the XMatrix3x3 object to the 3x3 identity as shown below.

[1 0 0] <br>
[0 1 0] <br>
[0 0 1] <br>

@return None.
*/
//*****************************************************************************
void XMatrix3x3::setIdentity()
{
	_11 = 1; _12 = 0; _13 = 0;
	_21 = 0; _22 = 1; _23 = 0;
	_31 = 0; _32 = 0; _33 = 1;
}

XBOOL XMatrix3x3::isIdentity()
{
	return (_11 == 1.0f &&
			X_MATH_IS_XF32_ZERO(_12) &&
			X_MATH_IS_XF32_ZERO(_13) &&
			X_MATH_IS_XF32_ZERO(_21) &&
			_22 == 1.0f &&
			X_MATH_IS_XF32_ZERO(_23) &&
			X_MATH_IS_XF32_ZERO(_31) &&
			X_MATH_IS_XF32_ZERO(_32) &&
			_33 == 1.0f);
}

//*****************************************************************************
/**
Orthonormalizes the XMatrix3x3 object. 

@return None.
@remark This function will modify this XMatrix3x3 object.
*/
//*****************************************************************************
void XMatrix3x3::orthonormalize() 
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
Sets a single row in the XMatrix3x3 object.  

@param row	The 0-based row number to set.  This value ranges from 0-2.
@param V	The XVector3D that specifies the component values for the row
			being set.
@return True if the operation was successful, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix3x3::setRow(XU32 row,XVector3D& V) 
{
	if (row < 0 || row >= 3)
		return false;

	for (XU32 i = 0;i < 3;i++)
		array[row][i] = V.array[i];

	return true;
}



//*****************************************************************************
/**
Gets a single row in the XMatrix3x3 object.  

@param row	The 0-based row number to get.  This value ranges from 0-2.
@param V	The XVector3D that will hold the component values for the
			requested row.
@return True if the operation was successful, false otherwise.
*/
//*****************************************************************************
XBOOL XMatrix3x3::getRow(XU32 row,XVector3D& V) 
{
	if (row < 0 || row >= 3)
		return false;

	for (XU32 i = 0;i < 3;i++)
		V.array[i] = array[row][i];

	return true;
}



//*****************************************************************************
/**
Transforms an XVector3D object by an XMatrix3x3 object according to the
following formula where M is the XMatrix3x3 object, V is the XVector3D object
to transform, and R is the resulting XVector3D object.  This function assumes
the matrix is made up of row vectors.

R.x = M._11 * V.x + M._21 * V.y + M._31 * V.z <br>
R.y = M._12 * V.x + M._22 * V.y + M._32 * V.z <br>
R.z = M._13 * V.x + M._23 * V.y + M._33 * V.z <br>

@param V	The vector to transform.
@param M	The matrix to transform the vector by.
@return The transformed vector.
*/
//*****************************************************************************
XVector3D operator * (XVector3D& V,XMatrix3x3& M) 
{
	//For row vectors
	return XVector3D(M._11 * V.x + M._21 * V.y + M._31 * V.z,
					  M._12 * V.x + M._22 * V.y + M._32 * V.z,
					  M._13 * V.x + M._23 * V.y + M._33 * V.z);   
}