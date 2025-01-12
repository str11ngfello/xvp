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
//	Module Name:	XVector2D.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XVector2D XVector2D::XAxis(1.0f,0.0f);
XVector2D XVector2D::YAxis(0.0f,1.0f);
XVector2D XVector2D::Zero(0.0f,0.0f);
XVector2D XVector2D::Identity(1.0f,1.0f);

//*****************************************************************************
/**
Constructs an XVector2D object by initializing the x and y components to 0.
*/
//*****************************************************************************
XVector2D::XVector2D()
{
	x = 0;
	y = 0;
}



//*****************************************************************************
/**
Constructs an XVector2D object by initializing the x and y components to the
passed in parameters.

@param ix	Initial x value.
@param iy	Initial y value.
*/
//*****************************************************************************
XVector2D::XVector2D(XF32 ix,XF32 iy)
{
	x = ix;
	y = iy;
}



//*****************************************************************************
/**
Destructor for the 2D vector.
*/
//*****************************************************************************
/*XVector2D::~XVector2D()
{
}*/



//*****************************************************************************
/**
Adds two XVector2D objects.  The vectors are added component wise
by the x and y components according to the following formula where V1 and V2
are the vectors to add, and R is the resulting vector.

R.x = V1.x + V2.x <br>
R.y = V1.y + V2.y

@param	V The XVector2D object to add.  
@return	The sum of the two vectors.  
*/
//*****************************************************************************
XVector2D XVector2D::operator + (XVector2D& V) 
{
	return XVector2D(x + V.x,y + V.y);
}



//*****************************************************************************
/**
Adds two XVector2D objects.  The vectors are added component wise
by the x and y components according to the following formula where V1 and V2
are the vectors to add, and R is the resulting vector.  This XVector2D
object is assigned the result.

R.x = V1.x + V2.x <br>
R.y = V1.y + V2.y

@param	V The XVector2D object to add.  
@return	The sum of the two vectors.  
*/
//*****************************************************************************
XVector2D& XVector2D::operator += (XVector2D& V) 
{
	x += V.x;
	y += V.y;

	return *this;
}



//*****************************************************************************
/**
Subtracts two XVector2D objects.  The vectors are subracted component wise
by the x and y components according to the following formula where V1 and V2
are the vectors to subtract, and R is the resulting vector.

R.x = V1.x - V2.x <br>
R.y = V1.y - V2.y

@param	V The XVector2D object to subtract.  
@return	The difference of the two vectors.  
*/
//*****************************************************************************
XVector2D XVector2D::operator - (XVector2D& V) 
{
	return XVector2D(x - V.x,y - V.y);
}



//*****************************************************************************
/**
Subtracts two XVector2D objects.  The vectors are subracted component wise
by the x and y components according to the following formula where V1 and V2
are the vectors to subtract, and R is the resulting vector.  This XVector2D
object is assigned the result.

R.x = V1.x - V2.x <br>
R.y = V1.y - V2.y

@param	V The XVector2D object to subtract.  
@return	The difference of the two vectors.  
*/
//*****************************************************************************
XVector2D& XVector2D::operator -= (XVector2D& V) 
{
	x -= V.x;
	y -= V.y;

	return *this;
}



//*****************************************************************************
/**
Multiplies an XVector2D object by a scalar.  The scalar is multiplied 
component wise by the x and y components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  

R.x = V1.x * S <br>
R.y = V1.y * S

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector2D XVector2D::operator * (XF32 scalar) 
{
	return XVector2D(scalar * x,scalar * y);
}



//*****************************************************************************
/**
Multiplies an XVector2D object by a scalar.  The scalar is multiplied 
component wise by the x and y components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  This XVector2D object is assigned the result.

R.x = V1.x * S <br>
R.y = V1.y * S

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector2D& XVector2D::operator *= (XF32 scalar) 
{
	x *= scalar;
	y *= scalar;

	return *this;
}



//*****************************************************************************
/**
Divides an XVector2D object by a scalar.  The x and y components of the
vector are divided component wise by the scalar according to the
following formula where V1 is the vector, S is the scalar to divide by, and
R is the resulting vector.  

R.x = V1.x / S <br>
R.y = V1.y / S

@param	scalar The scalar to divide by.  
@return	The quotient of the vector and scalar.  
*/
//*****************************************************************************
XVector2D XVector2D::operator / (XF32 scalar) 
{
	return XVector2D(x / scalar,y / scalar);
}



//*****************************************************************************
/**
Divides an XVector2D object by a scalar.  The x and y components of the
vector are divided component wise by the scalar according to the
following formula where V1 is the vector, S is the scalar to divide by, and
R is the resulting vector.  This XVector2D object is assigned the result.

R.x = V1.x / S <br>
R.y = V1.y / S

@param	scalar The scalar to divide by.  
@return	The quotient of the vector and scalar.  
*/
//*****************************************************************************
XVector2D& XVector2D::operator /= (XF32 scalar) 
{
	x /= scalar;
	y /= scalar;

	return *this;
}



//*****************************************************************************
/**
Multiplies two XVector2D objects.  The XVector2D objects are multiplied 
component wise by the x and y components of each vector according to the
following formula where V1 and V2 are the vectors to be multiplied, and
R is the resulting vector.  

R.x = V1.x * V2.x <br>
R.y = V1.y * V2.y 

@param	V The vector to multiply with.  
@return	The product of the two vectors.  
@remark This function is not the same as calculating the dot product.  Refer to
		the dot() function for more information.
*/
//*****************************************************************************
XVector2D XVector2D::operator * (XVector2D& V) 
{
	return XVector2D(x * V.x,y * V.y);
}



//*****************************************************************************
/**
Divides two XVector2D objects.  The x and y components of the two
vectors are divided component wise according to the following formula where V1
and V2 are the vectors being divided, and R is the resulting vector.  

R.x = V1.x / V2.x <br>
R.y = V1.y / V2.y 

@param	V The vector to divide by.  
@return	The quotient of the two vectors.  
*/
//*****************************************************************************
XVector2D XVector2D::operator / (XVector2D& V) 
{
	if ((V.x == 0) || (V.y == 0))
		return XVector2D(0,0);
	else
		return XVector2D(x / V.x,y / V.y);
}



//*****************************************************************************
/**
Returns an inverted version of an XVector2D object.

@return An inverted version of the XVector2D object where the x and y
		components have been negated.
*/
//*****************************************************************************
XVector2D XVector2D::operator - () 
{
	return XVector2D(-x,-y);
}



//*****************************************************************************
/**
Determines if two XVector2D objects are equal.  For the two vectors to be
equal, the following conditions must be true where V1 and V2 are the vectors being
compared:

V1.x = V2.x <br>
V1.y = V2.y

@param V	The vector to test with.
@return True if the two vectors are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XVector2D::operator == (XVector2D& V)
{
	if ((fabs(x - V.x) > X_MATH_EPSILON) ||
		(fabs(y - V.y) > X_MATH_EPSILON))
		return false;
	else
		return true;
}



//*****************************************************************************
/**
Determines if two XVector2D objects are unequal.  For the two vectors to be
unequal, at least one of the following conditions must be false where V1 and V2
are the vectors being compared:

V1.x = V2.x <br>
V1.y = V2.y

@param V	The vector to test with.
@return True if the two vectors are unequal, false otherwise.
*/
//*****************************************************************************
XBOOL XVector2D::operator != (XVector2D& V)
{
	if ((fabs(x - V.x) > X_MATH_EPSILON) ||
		(fabs(y - V.y) > X_MATH_EPSILON))
		return true;
	else
		return false;
}



//*****************************************************************************
/**
Calculates the dot product of two XVector2D objects according to the following
formula where V1 and V2 are the vectors and R is the return value.

R = (V1.x * V2.x) + (V1.y * V2.y)

@param The XVector2D object to use in the dot product calculation.
@return The dot product of the two vectors.
*/
//*****************************************************************************
XF32 XVector2D::dot(XVector2D& V) 
{
	return ((x * V.x) + (y * V.y));
}

XF32 XVector2D::dot(XVector2D* pV) 
{
	return ((x * pV->x) + (y * pV->y));
}


//*****************************************************************************
/**
Projects this XVector2D object onto another XVector2D object. 

@param V	The vector to project onto.
@return The projected vector.
*/
//*****************************************************************************
XVector2D XVector2D::projectOnto(XVector2D& V) 
{
	return (this->dot (V) / (V.dot (V))) * V;
}



//*****************************************************************************
/**
Linearly interpolates between two XVector2D objects.

@param V	The vector to interpolate to.
@param t	The interpolation factor.  This value must be between 0 and 1.0. 
*/
//*****************************************************************************
XVector2D XVector2D::lerp(XVector2D& V,XF32 t)
{
	return XVector2D(X_MATH_LERP(t,x,V.x),
					  X_MATH_LERP(t,y,V.y));
}



//*****************************************************************************
/**
Normalizes an XVector2D object.  

@return The normalized XVector2D object.
@remark This call will modify the XVector2D object calling this routine.
*/
//*****************************************************************************
XVector2D& XVector2D::normalize() 
{
	XF32 m = mag();
   
	if (m == 0)
		return *this;
	else
		return ((*this) /= m);
}



//*****************************************************************************
/**
Inverts the XVector2D object by negating the x and y components.  

@return The inverted version of the XVector2D object.
@remark This call will modify the XVector2D object permanently.
*/
//*****************************************************************************
XVector2D& XVector2D::invert()
{
	x = -x;
	y = -y;
	return (*this);
}



//*****************************************************************************
/**
Takes the absolute value of the XVector2D object.  

@return None.
@remark This call will modify the XVector2D object calling this routine.
*/
//*****************************************************************************
void XVector2D::abs()
{
	x = fabs(x);
	y = fabs(y);
}


void XVector2D::setZero()
{
	x = 0.0f;
	y = 0.0f;
}

XBOOL XVector2D::isZero()
{
	return (X_MATH_IS_XF32_ZERO(x) &&
			X_MATH_IS_XF32_ZERO(y));
}

//*****************************************************************************
/**
Calculates the angle between two XVector2D objects.

@param V The XVector2D object to calculate the angle with.
@return The angle between the two vectors in degrees.
*/
//*****************************************************************************
XF32 XVector2D::angle(XVector2D& V) 
{
	XF32 m = this->mag() * V.mag();
	if (m == 0)
		return 0;

	return (XF32) acos(this->dot(V) / m);
}



//*****************************************************************************
/**
Calculates the magnitude of the XVector2D object according to the following
formula.

sqrt((x * x) + (y * y))

@return The magnitude/length of the vector.
*/
//*****************************************************************************
XF32 XVector2D::mag() 
{
	return (XF32)sqrt((x * x) + (y * y));
}



//*****************************************************************************
/**
Sets the length of the XVector2D object.  The vector will maintain its
direction but the magnitude will change.

@param length	The length to set the vector to.
@return None.
*/
//*****************************************************************************
void XVector2D::setLength(XF32 length) 
{
	XF32 len = mag();	
	x *= length / len;
	y *= length / len;
}


void XVector2D::min(XVector2D& V)
{
	if (V.x < x)
		x = V.x;
	if (V.y < y)
		y = V.y;
}

void XVector2D::max(XVector2D& V)
{
	if (V.x > x)
		x = V.x;
	if (V.y > y)
		y = V.y;
}

//*****************************************************************************
/**
Determines the distance between two XVector2D objects.

@param V The XVector2D object to find the distance to.
@return The distance between the two vectors.
*/
//*****************************************************************************
XF32 XVector2D::distance(XVector2D& V)
{
	XVector2D t = *this;
	t = t - V;

	return t.mag();
}



//*****************************************************************************
//*****************************************************************************
XF32 XVector2D::distanceSquared(XVector2D& V)
{
	XVector2D t = *this;
	t = t - V;

	return (XF32) (t.x * t.x) + (t.y * t.y);
}



//*****************************************************************************
/**
Converts an XVector2D object into an XVector3D object.

@return An XVector3D object where x and y are equal to this XVector2D object
		and the z component is equal to 1.
*/
//*****************************************************************************
XVector2D::operator XVector3D() 
{
   return XVector3D(x,y,1); 
}



//*****************************************************************************
/**
Calculates an XVector2D object in terms of polar coordinate space.

@param m	A variable to store the length of the vector pointing from the 
			coordinate origin to a point in 2D space.
@param angle	A variable to store the polar (phase angle) in radians.
@return None.
*/
//*****************************************************************************
void XVector2D::toPolar(XF32& m,XF32& angle) 
{
	m = mag();
	angle = X_ArcTan(y,x);
}



//*****************************************************************************
/**
Multiplies a scalar by an XVector2D object.  The scalar is multiplied 
component wise by the x and y components of the vector according to the
following formula where V1 is the vector, S is the scalar to multiply with, and
R is the resulting vector.  

R.x = S * V1.x   <br>
R.y = S * V1.y  

@param	scalar The scalar to multiply with.  
@return	The product of the vector and scalar.  
*/
//*****************************************************************************
XVector2D operator * (XF32 scalar,XVector2D& V) 
{
	return XVector2D(scalar * V.x,scalar * V.y);
}