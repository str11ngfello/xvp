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
//	Module Name:	XVector4D.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XVector4D XVector4D::XAxis(1.0f,0.0f,0.0f,0.0f);
XVector4D XVector4D::YAxis(0.0f,1.0f,0.0f,0.0f);
XVector4D XVector4D::ZAxis(0.0f,0.0f,1.0f,0.0f);
XVector4D XVector4D::WAxis(0.0f,0.0f,0.0f,1.0f);
XVector4D XVector4D::Zero(0.0f,0.0f,0.0f,0.0f);
XVector4D XVector4D::Identity(1.0f,1.0f,1.0f,1.0f);


//*****************************************************************************
/**
Constructs an XVector4D object by initializing the x, y, z and w components to
0.
*/
//*****************************************************************************
XVector4D::XVector4D()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}



//*****************************************************************************
/**
Constructs an XVector4D object by initializing the x, y, z and w components to
the passed in parameters.

@param ix	Initial x value.
@param iy	Initial y value.
@param iz	Initial z value.
@param iw	Initial w value.
*/
//*****************************************************************************
XVector4D::XVector4D(XF32 ix,XF32 iy,XF32 iz,XF32 iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}



XVector4D::XVector4D(XColor& color)
{
	x = color.m_R;
	y = color.m_G;
	z = color.m_B;
	w = color.m_A;
}

//*****************************************************************************
/**
Destructor for the 4D vector.
*/
//*****************************************************************************
/*XVector4D::~XVector4D()
{
}*/


//*****************************************************************************
/**
Adds two XVector4D objects.  The vectors are added component wise
by the x, y, z and w components according to the following formula where V1 and V2
are the vectors to add, and R is the resulting vector.

R.x = V1.x + V2.x <br>
R.y = V1.y + V2.y <br>
R.z = V1.z + V2.z <br>
R.w = V1.w + V2.w

@param	V The XVector4D object to add.  
@return	The sum of the two vectors.  
*/
//*****************************************************************************
XVector4D XVector4D::operator + (XVector4D& V) 
{
	return XVector4D(x + V.x,y + V.y,z + V.z,w + V.w);
}



//*****************************************************************************
/**
Adds two XVector4D objects.  The vectors are added component wise
by the x, y, z and w components according to the following formula where V1 and V2
are the vectors to add, and R is the resulting vector.  This XVector4D
object is assigned the result.

R.x = V1.x + V2.x <br>
R.y = V1.y + V2.y <br>
R.z = V1.z + V2.z <br>
R.w = V1.w + V2.w

@param	V The XVector4D object to add.  
@return	The sum of the two vectors.  
*/
//*****************************************************************************
XVector4D &XVector4D::operator += (XVector4D& V) 
{
	x += V.x;
	y += V.y;
	z += V.z;
	w += V.w;

	return *this;
}



//*****************************************************************************
/**
Subtracts two XVector4D objects.  The vectors are subracted component wise
by the x, y, z and w components according to the following formula where V1 and V2
are the vectors to subtract, and R is the resulting vector.

R.x = V1.x - V2.x <br>
R.y = V1.y - V2.y <br>
R.z = V1.z - V2.z <br>
R.w = V1.w - V2.w

@param	V The XVector4D object to subtract.  
@return	The difference of the two vectors.  
*/
//*****************************************************************************
XVector4D XVector4D::operator - (XVector4D& V) 
{
	return XVector4D(x - V.x,y - V.y,z - V.z,w - V.w);
}



//*****************************************************************************
/**
Subtracts two XVector4D objects.  The vectors are subracted component wise
by the x, y, z and w components according to the following formula where V1 and V2
are the vectors to subtract, and R is the resulting vector.  This XVector4D
object is assigned the result.

R.x = V1.x - V2.x <br>
R.y = V1.y - V2.y <br>
R.z = V1.z - V2.z <br>
R.w = V1.w - V2.w

@param	V The XVector4D object to subtract.  
@return	The difference of the two vectors.  
*/
//*****************************************************************************
XVector4D &XVector4D::operator -= (XVector4D& V) 
{
	x -= V.x;
	y -= V.y;
	z -= V.z;
	w -= V.w;

	return *this;
}



//*****************************************************************************
/**
Multiplies an XVector4D object by a scalar.  The scalar is multiplied 
component wise by the x, y, z and w components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  

R.x = V1.x * S <br>
R.y = V1.y * S <br>
R.z = V1.z * S <br>
R.w = V1.w * S

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector4D XVector4D::operator * (XF32 scalar) 
{
	return XVector4D(scalar * x,scalar * y,scalar * z,scalar * w);
}



//*****************************************************************************
/**
Multiplies an XVector4D object by a scalar.  The scalar is multiplied 
component wise by the x, y, z and w components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  This XVector4D object is assigned the result.

R.x = V1.x * S <br>
R.y = V1.y * S <br>
R.z = V1.z * S <br>
R.w = V1.w * S

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector4D &XVector4D::operator *= (XF32 scalar) 
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;

	return *this;
}



//*****************************************************************************
/**
Divides an XVector4D object by a scalar.  The x, y, z and w components of the
vector are divided component wise by the scalar according to the
following formula where V1 is the vector, S is the scalar to divide by, and
R is the resulting vector.  

R.x = V1.x / S <br>
R.y = V1.y / S <br>
R.z = V1.z / S <br>
R.w = V1.w / S

@param	scalar The scalar to divide by.  
@return	The quotient of the vector and scalar.  
*/
//*****************************************************************************
XVector4D XVector4D::operator / (XF32 scalar) 
{
	return XVector4D(x / scalar,y / scalar,z / scalar,w / scalar);
}



//*****************************************************************************
/**
Divides an XVector4D object by a scalar.  The x, y, z and w components of the
vector are divided component wise by the scalar according to the
following formula where V1 is the vector, S is the scalar to divide by, and
R is the resulting vector.  This XVector4D object is assigned the result.

R.x = V1.x / S <br>
R.y = V1.y / S <br>
R.z = V1.z / S <br>
R.w = V1.w / S

@param	scalar The scalar to divide by.  
@return	The quotient of the vector and scalar.  
*/
//*****************************************************************************
XVector4D &XVector4D::operator /= (XF32 scalar) 
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;

	return *this;
}



//*****************************************************************************
/**
Multiplies two XVector4D objects.  The XVector4D objects are multiplied 
component wise by the x, y, z and w components of each vector according to the
following formula where V1 and V2 are the vectors to be multiplied, and
R is the resulting vector.  

R.x = V1.x * V2.x <br>
R.y = V1.y * V2.y <br>
R.z = V1.z * V2.z <br>
R.w = V1.w * V2.w 

@param	V The vector to multiply with.  
@return	The product of the two vectors.  
@remark This function is not the same as calculating the dot product.  Refer to
		the dot() function for more information.
*/
//*****************************************************************************
XVector4D XVector4D::operator * (XVector4D& V) 
{
	return XVector4D(x * V.x,y * V.y,z * V.z,w * V.w);
}



//*****************************************************************************
/**
Divides two XVector4D objects.  The x, y, z and w components of the two
vectors are divided component wise according to the following formula where V1
and V2 are the vectors being divided, and R is the resulting vector.  

R.x = V1.x / V2.x <br>
R.y = V1.y / V2.y <br>
R.z = V1.z / V2.z <br>
R.w = V1.w / V2.w 

@param	V The vector to divide by.  
@return	The quotient of the two vectors.  
*/
//*****************************************************************************
XVector4D XVector4D::operator / (XVector4D& V) 
{
	if ((V.x == 0) || (V.y == 0))
		return XVector4D(0,0,0,0);
	else
		return XVector4D(x / V.x,y / V.y,z / V.z,w / V.w);
}



//*****************************************************************************
/**
Returns an inverted version of an XVector4D object.

@return An inverted version of the XVector4D object where the x, y, z and w
		components have been negated.
*/
//*****************************************************************************
XVector4D XVector4D::operator - () 
{
	return XVector4D(-x,-y,-z,-w);
}



//*****************************************************************************
/**
Determines if two XVector4D objects are equal.  For the two vectors to be
equal, the following conditions must be true where V1 and V2 are the vectors being
compared:

V1.x = V2.x <br>
V1.y = V2.y <br>
V1.z = V2.z <br>
V1.w = V2.w

@param V	The vector to test with.
@return True if the two vectors are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XVector4D::operator == (XVector4D& V)
{
	if ((fabs(x - V.x) > X_MATH_EPSILON) ||
		(fabs(y - V.y) > X_MATH_EPSILON) ||
		(fabs(z - V.z) > X_MATH_EPSILON) ||
		(fabs(w - V.w) > X_MATH_EPSILON))
		return false;
	else
		return true;
}



//*****************************************************************************
/**
Determines if two XVector4D objects are unequal.  For the two vectors to be
unequal, at least one of the following conditions must be false where V1 and V2
are the vectors being compared:

V1.x = V2.x <br>
V1.y = V2.y <br>
V1.z = V2.z <br>
V1.w = V2.w

@param V	The vector to test with.
@return True if the two vectors are unequal, false otherwise.
*/
//*****************************************************************************
XBOOL XVector4D::operator != (XVector4D& V)
{
	if ((fabs(x - V.x) > X_MATH_EPSILON) ||
		(fabs(y - V.y) > X_MATH_EPSILON) ||
		(fabs(z - V.z) > X_MATH_EPSILON) ||
		(fabs(w - V.w) > X_MATH_EPSILON))
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Calculates the dot product of two XVector4D objects according to the following
formula where V1 and V2 are the vectors and R is the return value.

R = (V1.x * V2.x) + (V1.y * V2.y) + (V1.z * V2.z) + (V1.w * V2.w)

@param The XVector4D object to use in the dot product calculation.
@return The dot product of the two vectors.
*/
//*****************************************************************************
XF32 XVector4D::dot(const XVector4D& V) 
{
	return ((x * V.x) + (y * V.y) + (z * V.z) + (w * V.w));
}



//*****************************************************************************
/**
Projects this XVector4D object onto another XVector4D object. 

@param V	The vector to project onto.
@return The projected vector.
*/
//*****************************************************************************
XVector4D XVector4D::projectOnto(XVector4D& V) 
{
	return (this->dot(V) / (V.dot(V))) * V;
}



//*****************************************************************************
/**
Linearly interpolates between two XVector4D objects.

@param V	The vector to interpolate to.
@param t	The interpolation factor.  This value must be between 0 and 1.0. 
*/
//*****************************************************************************
XVector4D XVector4D::lerp(XVector4D& V,XF32 t)
{
	return XVector4D(X_MATH_LERP(t,x,V.x),
					  X_MATH_LERP(t,y,V.y),
					  X_MATH_LERP(t,z,V.z),
					  X_MATH_LERP(t,w,V.w));
}



//*****************************************************************************
/**
Normalizes an XVector4D object.  

@return The normalized XVector4D object.
@remark This call will modify the XVector4D object calling this routine.
*/
//*****************************************************************************
XVector4D &XVector4D::normalize() 
{
	XF32 m = mag();
   
	if (m == 0)
		return *this;
	else
		return ((*this) /= m);
}



//*****************************************************************************
/**
Inverts the XVector4D object by negating the x, y, z and w components.  

@return The inverted version of the XVector4D object.
@remark This call will modify the XVector4D object permanently.
*/
//*****************************************************************************
XVector4D& XVector4D::invert()
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return (*this);
}



//*****************************************************************************
/**
Takes the absolute value of the XVector4D object.  

@return None.
@remark This call will modify the XVector4D object calling this routine.
*/
//*****************************************************************************
void XVector4D::abs()
{
	x = fabs(x);
	y = fabs(y);
	z = fabs(z);
	w = fabs(w);
}

void XVector4D::setZero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

XBOOL XVector4D::isZero()
{
	return (X_MATH_IS_XF32_ZERO(x) &&
			X_MATH_IS_XF32_ZERO(y) &&
			X_MATH_IS_XF32_ZERO(z) &&
			X_MATH_IS_XF32_ZERO(w));
}

//*****************************************************************************
/**
Calculates the angle between two XVector4D objects.

@param V The XVector4D object to calculate the angle with.
@return The angle between the two vectors in degrees.
*/
//*****************************************************************************
XF32 XVector4D::angle(XVector4D& V) 
{
	XF32 m = this->mag() * V.mag();
	if (m == 0)
		return 0;

	return (XF32) acos(this->dot(V) / m);
}



//*****************************************************************************
/**
Homogenizes the XVector4D object.

@return The homogenized vector.
@remark This call will modify the XVector4D object.
*/
//*****************************************************************************
XVector4D &XVector4D::homogenize() 
{
	XF32 OneOverW = (XF32)(1.0)/w;

	x *= OneOverW;
	y *= OneOverW;
	z *= OneOverW;
	w  = (XF32) 1.0;

	return *this;
}



//*****************************************************************************
/**
Calculates the magnitude of the XVector4D object according to the following
formula.

sqrt((x * x) + (y * y) + (z * z) + (w * w))

@return The magnitude/length of the vector.
*/
//*****************************************************************************
XF32 XVector4D::mag() 
{
	return (XF32)sqrt((x * x) + (y * y) + (z * z) + (w * w));
}



//*****************************************************************************
/**
Sets the length of the XVector4D object.  The vector will maintain its
direction but the magnitude will change.

@param length	The length to set the vector to.
@return None.
*/
//*****************************************************************************
void XVector4D::setLength(XF32 length) 
{
	XF32 len = mag();	
	x *= length / len;
	y *= length / len;
	z *= length / len;
	w *= length / len;
}


void XVector4D::min(XVector4D& V)
{
	if (V.x < x)
		x = V.x;
	if (V.y < y)
		y = V.y;
	if (V.z < z)
		z = V.z;
	if (V.w < w)
		w = V.w;
}

void XVector4D::max(XVector4D& V)
{
	if (V.x > x)
		x = V.x;
	if (V.y > y)
		y = V.y;
	if (V.z > z)
		z = V.z;
	if (V.w > w)
		w = V.w;
}


//*****************************************************************************
/**
Determines the distance between two XVector4D objects.

@param V The XVector4D object to find the distance to.
@return The distance between the two vectors.
*/
//*****************************************************************************
XF32 XVector4D::distance(XVector4D& V)
{
	XVector4D t = *this;
	t = t - V;

	return t.mag();
}



//*****************************************************************************
//*****************************************************************************
XF32 XVector4D::distanceSquared(XVector4D& V)
{
	XVector4D t = *this;
	t = t - V;

	return (XF32) (x * x) + (y * y) + (z * z) + (w * w);
}



//*****************************************************************************
/**

*/
//*****************************************************************************
XVector4D::operator XVector3D() 
{
	return XVector3D(x,y,z);
}



//*****************************************************************************
/**
Multiplies a scalar by an XVector4D object.  The scalar is multiplied 
component wise by the x, y, z and w components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  

R.x = S * V1.x <br>
R.y = S * V1.y <br> 
R.z = S * V1.z <br> 
R.w = S * V1.w <br>

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector4D operator * (XF32 scalar,XVector4D& V) 
{
	return XVector4D(scalar * V.x,scalar * V.y,scalar * V.z,scalar * V.w);
}



//*****************************************************************************
/**
Transforms an XVector4D object by an XMatrix4x4 object according to the
following formula where V is the XVector4D object, M is the XMatrix4x4
object, and R is the resulting XVector4D object.

R.x = M._11 * V.x + M._21 * V.y + M._31 * V.z + M._41 * V.w <br>
R.y = M._12 * V.x + M._22 * V.y + M._32 * V.z + M._42 * V.w <br>
R.z = M._13 * V.x + M._23 * V.y + M._33 * V.z + M._43 * V.w <br>
R.w = M._14 * V.x + M._24 * V.y + M._34 * V.z + M._44 * V.w <br>

@param V	The vector to transform.
@param M	The matrix to transform by.
@return The transformed vector.
*/
//*****************************************************************************
XVector4D operator * (XVector4D& V,XMatrix4x4& M) 
{
	//For row vectors:
	return XVector4D(M._11 * V.x + M._21 * V.y + M._31 * V.z + M._41 * V.w,
					  M._12 * V.x + M._22 * V.y + M._32 * V.z + M._42 * V.w,
                      M._13 * V.x + M._23 * V.y + M._33 * V.z + M._43 * V.w,
                      M._14 * V.x + M._24 * V.y + M._34 * V.z + M._44 * V.w);
}