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
//	Module Name:	XVector3D.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************


#include <xvpsdk.h>


XVector3D XVector3D::XAxis(1.0f,0.0f,0.0f);
XVector3D XVector3D::YAxis(0.0f,1.0f,0.0f);
XVector3D XVector3D::ZAxis(0.0f,0.0f,1.0f);
XVector3D XVector3D::Zero(0.0f,0.0f,0.0f);
XVector3D XVector3D::Identity(1.0f,1.0f,1.0f);

//*****************************************************************************
/**
Constructs an XVector3D object by initializing the x, y and z components to 0.
*/
//*****************************************************************************
XVector3D::XVector3D()
{
	x = 0;
	y = 0;
	z = 0;
}



//*****************************************************************************
/**
Constructs an XVector3D object by initializing the x, y and z components to the
passed in parameters.

@param ix	Initial x value.
@param iy	Initial y value.
@param iz	Initial z value.
*/
//*****************************************************************************
XVector3D::XVector3D(XF32 ix,XF32 iy,XF32 iz)
{
	x = ix;
	y = iy;
	z = iz;
}



//*****************************************************************************
/**
Adds two XVector3D objects.  The vectors are added component wise
by the x, y and z components according to the following formula where V1 and V2
are the vectors to add, and R is the resulting vector.

R.x = V1.x + V2.x <br>
R.y = V1.y + V2.y <br>
R.z = V1.z + V2.z

@param	V The XVector3D object to add.  
@return	The sum of the two vectors.  
*/
//*****************************************************************************
XVector3D XVector3D::operator + (XVector3D& V) 
{
	return XVector3D(x + V.x,y + V.y,z + V.z);
}



//*****************************************************************************
/**
Adds two XVector3D objects.  The vectors are added component wise
by the x, y and z components according to the following formula where V1 and V2
are the vectors to add, and R is the resulting vector.  This XVector3D
object is assigned the result.

R.x = V1.x + V2.x <br>
R.y = V1.y + V2.y <br>
R.z = V1.z + V2.z

@param	V The XVector3D object to add.  
@return	The sum of the two vectors.  
*/
//*****************************************************************************
XVector3D &XVector3D::operator += (XVector3D& V) 
{
	x += V.x;
	y += V.y;
	z += V.z;

	return *this;
}



//*****************************************************************************
/**
Subtracts two XVector3D objects.  The vectors are subracted component wise
by the x, y and z components according to the following formula where V1 and V2
are the vectors to subtract, and R is the resulting vector.

R.x = V1.x - V2.x <br>
R.y = V1.y - V2.y <br>
R.z = V1.z - V2.z

@param	V The XVector3D object to subtract.  
@return	The difference of the two vectors.  
*/
//*****************************************************************************
XVector3D XVector3D::operator - (XVector3D& V) 
{
	return XVector3D(x - V.x,y - V.y,z - V.z);
}



//*****************************************************************************
/**
Subtracts two XVector3D objects.  The vectors are subracted component wise
by the x, y and z components according to the following formula where V1 and V2
are the vectors to subtract, and R is the resulting vector.  This XVector3D
object is assigned the result.

R.x = V1.x - V2.x <br>
R.y = V1.y - V2.y <br>
R.z = V1.z - V2.z

@param	V The XVector3D object to subtract.  
@return	The difference of the two vectors.  
*/
//*****************************************************************************
XVector3D &XVector3D::operator -= (XVector3D& V) 
{
	x -= V.x;
	y -= V.y;
	z -= V.z;

	return *this;
}



//*****************************************************************************
/**
Multiplies an XVector3D object by a scalar.  The scalar is multiplied 
component wise by the x, y and z components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  

R.x = V1.x * S <br>
R.y = V1.y * S <br>
R.z = V1.z * S

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector3D XVector3D::operator * (XF32 scalar) 
{
	return XVector3D(scalar * x,scalar * y,scalar * z);
}



//*****************************************************************************
/**
Multiplies an XVector3D object by a scalar.  The scalar is multiplied 
component wise by the x, y and z components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  This XVector3D object is assigned the result.

R.x = V1.x * S <br>
R.y = V1.y * S <br>
R.z = V1.z * S

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector3D &XVector3D::operator *= (XF32 scalar) 
{
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}



//*****************************************************************************
/**
Divides an XVector3D object by a scalar.  The x, y and z components of the
vector are divided component wise by the scalar according to the
following formula where V1 is the vector, S is the scalar to divide by, and
R is the resulting vector.  

R.x = V1.x / S <br>
R.y = V1.y / S <br>
R.z = V1.z / S

@param	scalar The scalar to divide by.  
@return	The quotient of the vector and scalar.  
*/
//*****************************************************************************
XVector3D XVector3D::operator / (XF32 scalar) 
{
	return XVector3D(x / scalar,y / scalar,z / scalar);
}



//*****************************************************************************
/**
Divides an XVector3D object by a scalar.  The x, y and z components of the
vector are divided component wise by the scalar according to the
following formula where V1 is the vector, S is the scalar to divide by, and
R is the resulting vector.  This XVector3D object is assigned the result.

R.x = V1.x / S <br>
R.y = V1.y / S <br>
R.z = V1.z / S

@param	scalar The scalar to divide by.  
@return	The quotient of the vector and scalar.  
*/
//*****************************************************************************
XVector3D &XVector3D::operator /= (XF32 scalar) 
{   
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}



//*****************************************************************************
/**
Multiplies two XVector3D objects.  The XVector3D objects are multiplied 
component wise by the x, y and z components of each vector according to the
following formula where V1 and V2 are the vectors to be multiplied, and
R is the resulting vector.  

R.x = V1.x * V2.x <br>
R.y = V1.y * V2.y <br>
R.z = V1.z * V2.z 

@param	V The vector to multiply with.  
@return	The product of the two vectors.  
@remark This function is not the same as calculating the dot product.  Refer to
		the dot() function for more information.
*/
//*****************************************************************************
XVector3D XVector3D::operator * (XVector3D& V) 
{
	return XVector3D(x * V.x,y * V.y,z * V.z);
}



//*****************************************************************************
/**
Divides two XVector3D objects.  The x, y and z components of the two
vectors are divided component wise according to the following formula where V1
and V2 are the vectors being divided, and R is the resulting vector.  

R.x = V1.x / V2.x <br>
R.y = V1.y / V2.y <br>
R.z = V1.z / V2.z 

@param	V The vector to divide by.  
@return	The quotient of the two vectors.  
*/
//*****************************************************************************
XVector3D XVector3D::operator / (XVector3D& V) 
{
	if ((V.x == 0) || (V.y == 0) || (V.z == 0))
		return XVector3D(0,0,0);
	else
		return XVector3D(x / V.x,y / V.y,z / V.z);
}



//*****************************************************************************
/**
Returns an inverted version of an XVector3D object.

@return An inverted version of the XVector3D object where the x, y and z
		components have been negated.
*/
//*****************************************************************************
XVector3D XVector3D::operator - () 
{
	return XVector3D(-x,-y,-z);
}



//*****************************************************************************
/**
Determines if two XVector3D objects are equal.  For the two vectors to be
equal, the following conditions must be true where V1 and V2 are the vectors being
compared:

V1.x = V2.x <br>
V1.y = V2.y <br>
V1.z = V2.z

@param V	The vector to test with.
@return True if the two vectors are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XVector3D::operator == (XVector3D& V)
{
	if ((fabs(x - V.x) > X_MATH_EPSILON) ||
		(fabs(y - V.y) > X_MATH_EPSILON) ||
		(fabs(z - V.z) > X_MATH_EPSILON))
		return false;
	else
		return true;
}



//*****************************************************************************
/**
Determines if two XVector3D objects are unequal.  For the two vectors to be
unequal, at least one of the following conditions must be false where V1 and V2
are the vectors being compared:

V1.x = V2.x <br>
V1.y = V2.y <br>
V1.z = V2.z

@param V	The vector to test with.
@return True if the two vectors are unequal, false otherwise.
*/
//*****************************************************************************
XBOOL XVector3D::operator != (XVector3D& V)
{
	if ((fabs(x - V.x) > X_MATH_EPSILON) ||
		(fabs(y - V.y) > X_MATH_EPSILON) ||
		(fabs(z - V.z) > X_MATH_EPSILON))
		return true;
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XVector3D::operator > (XVector3D& V)
{
	return (x > V.x && y > V.y && z > V.z);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XVector3D::operator >= (XVector3D& V)
{
	return (x >= V.x && y >= V.y && z >= V.z);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XVector3D::operator < (XVector3D& V)
{
	return (x < V.x && y < V.y && z < V.z);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XVector3D::operator <= (XVector3D& V)
{
	return (x <= V.x && y <= V.y && z <= V.z);
}



//*****************************************************************************
/**
Calculates the dot product of two XVector3D objects according to the following
formula where V1 and V2 are the vectors and R is the return value.

R = (V1.x * V2.x) + (V1.y * V2.y) + (V1.z * V2.z)

@param The XVector3D object to use in the dot product calculation.
@return The dot product of the two vectors.
*/
//*****************************************************************************
XF32 XVector3D::dot(XVector3D& V) 
{
	return ((x * V.x) + (y * V.y) + (z * V.z));
}



//*****************************************************************************
/**
Calculates the cross product of two XVector3D objects according to the
following formula where V1 and V2 are the two vectors and R is the resulting
vector.

R.x = (y * V.z) - (z * V.y)
R.y = (z * V.x) - (x * V.z)
R.z = (x * V.y) - (y * V.x)

@param V	The vector to use in the cross product calculation.
@return The vector resulting from the cross product calculation.
@remark The cross product will return a vector that is mutually perpendicular
		to the two vectors used as input to the cross product equation. 
*/
//*****************************************************************************
XVector3D XVector3D::cross(XVector3D& V) 
{
	return XVector3D((y * V.z) - (z * V.y),
					  (z * V.x) - (x * V.z),
					  (x * V.y) - (y * V.x));
}



//*****************************************************************************
/**
Normalizes an XVector3D object.  

@return The normalized XVector3D object.
@remark This call will modify the XVector3D object calling this routine.
*/
//*****************************************************************************
XVector3D& XVector3D::normalize() 
{
	XF32 m = mag();
    X_ASSERTM(m != 0,"Attempting to normalize a zero length vector.  This is not possible.");
	//if(m == 0)
	//	return *this;
	//else
		return ((*this) /= m);
}



//*****************************************************************************
/**
Takes the absolute value of the XVector3D object.  

@return None.
@remark This call will modify the XVector3D object calling this routine.
*/
//*****************************************************************************
void XVector3D::abs()
{
	x = fabs(x);
	y = fabs(y);
	z = fabs(z);
}

void XVector3D::setZero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

XBOOL XVector3D::isZero()
{
	return (X_MATH_IS_XF32_ZERO(x) &&
			X_MATH_IS_XF32_ZERO(y) &&
			X_MATH_IS_XF32_ZERO(z));
}


//*****************************************************************************
/**
Inverts the XVector3D object by negating the x, y and z components.  

@return The inverted version of the XVector3D object.
@remark This call will modify the XVector3D object permanently.
*/
//*****************************************************************************
XVector3D &XVector3D::invert()
{
	x = -x;
	y = -y;
	z = -z;
	return (*this);
}



//*****************************************************************************
/**
Calculates the magnitude of the XVector3D object according to the following
formula.

sqrt((x * x) + (y * y) + (z * z))

@return The magnitude/length of the vector.
*/
//*****************************************************************************
XF32 XVector3D::mag() 
{
	return (XF32) sqrt((x * x) + (y * y) + (z * z));
}



//*****************************************************************************
/**
Sets the length of the XVector3D object.  The vector will maintain its
direction but the magnitude will change.

@param length	The length to set the vector to.
@return None.
*/
//*****************************************************************************
void XVector3D::setLength(XF32 length) 
{
	XF32 len = mag();	
	x *= length / len;
	y *= length / len;
	z *= length / len;
}


void XVector3D::min(XVector3D& V)
{
	if (V.x < x)
		x = V.x;
	if (V.y < y)
		y = V.y;
	if (V.z < z)
		z = V.z;
}

void XVector3D::max(XVector3D& V)
{
	if (V.x > x)
		x = V.x;
	if (V.y > y)
		y = V.y;
	if (V.z > z)
		z = V.z;
}

//*****************************************************************************
/**
Determines the distance between two XVector3D objects.

@param V The XVector3D object to find the distance to.
@return The distance between the two vectors.
*/
//*****************************************************************************
XF32 XVector3D::distance(XVector3D& V)
{
	XVector3D t = *this;
	t = t - V;

	return t.mag();
}



//*****************************************************************************
/**
Determines the squared distance between two XVector3D objects.  This is faster
than the distance() method since it doesn't perform the sqrt() during evaluation.

@param V The XVector3D object to find the distance to.
@return The squared distance between the two vectors.
*/
//*****************************************************************************
XF32 XVector3D::distanceSquared(XVector3D& V)
{
	XVector3D t = *this;
	t = t - V;

	return (XF32)((t.x * t.x) + (t.y * t.y) + (t.z * t.z));
}



//*****************************************************************************
/**
Projects this XVector3D object onto another XVector3D object. 

@param V	The vector to project onto.
@return The projected vector.
*/
//*****************************************************************************
XVector3D XVector3D::projectOnto(XVector3D& V) 
{
	return (this->dot(V) / (V.dot(V))) * V;
}



//*****************************************************************************
/**
Linearly interpolates between two XVector3D objects.

@param V	The vector to interpolate to.
@param t	The interpolation factor.  This value must be between 0 and 1.0. 
*/
//*****************************************************************************
XVector3D XVector3D::lerp(XVector3D& V,XF32 t)
{
	return XVector3D(X_MATH_LERP(t,x,V.x),
					  X_MATH_LERP(t,y,V.y),
					  X_MATH_LERP(t,z,V.z));
}



//*****************************************************************************
/**
Calculates the angle between two XVector3D objects.

@param V The XVector3D object to calculate the angle with.
@return The angle between the two vectors in degrees.
*/
//*****************************************************************************
XF32 XVector3D::angle(XVector3D& V) 
{
	XF32 m = this->mag() * V.mag();

	if (m == 0)
		return 0;

	return (XF32) acos(this->dot(V) / m);
}



//*****************************************************************************
/**
Converts an XVector3D object into an XVector4D object.

@return An XVector4D object where x, y and z are equal to this XVector3D object
		and the z component is equal to 1.
*/
//*****************************************************************************
XVector3D::operator XVector4D() 
{
   return XVector4D(x,y,z,1); 
}



//*****************************************************************************
/**
Calculates an XVector3D object in terms of spherical coordinate space.

@param m	A variable to store the length of the vector pointing from the 
			coordinate origin to a point in 3D space.
@param angle1	A variable to store the x phase angle in radians.
@param angle2	A variable to store the y phase angle in radians.
@return True if the calculation was successful, false otherwise.
*/
//*****************************************************************************
XStatus XVector3D::toSpherical(XF32& m,XF32& angle1,XF32& angle2) 
{
	XF32 p,pSquared,ySquared,r;

	p = mag();
	pSquared  = p * p;
	ySquared = y * y;
	r = (XF32)sqrt(pSquared - ySquared);

	X_ASSERTM(((p != 0.0)&&(r != 0.0f)),"toSpherical() called on zero lenght 3D vector");

	m = p;
	angle1 = (XF32)acos(x / r);
	angle2 = (XF32)acos(y / p);

	return X_OK;
}



//*****************************************************************************
/**
Multiplies a scalar by an XVector3D object.  The scalar is multiplied 
component wise by the x, y and z components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  

R.x = S * V1.x <br>
R.y = S * V1.y <br> 
R.z = S * V1.z  

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector3D operator * (XF32 scalar,XVector3D& V) 
{
	return XVector3D(scalar * V.x,scalar * V.y,scalar * V.z);
}



//*****************************************************************************
/**
Determines if the XVector3D object projects more heavily in the x, y or z
direction. 

@return The direction in which the vector most heavily projects.
*/
//*****************************************************************************
X_MATH_DIRECTION XVector3D::getMaxDirection()
{
	X_MATH_DIRECTION Max = X_MD_X;
	XF32 NMax = (XF32)fabs(x);

	if(fabs(y) > NMax)
	{
		Max = X_MD_Y;
		NMax = (XF32)fabs(y);
	}

	if(fabs(z) > NMax)
	{
		Max = X_MD_Z;
		NMax = (XF32)fabs(z);
	}

	return Max;
}





