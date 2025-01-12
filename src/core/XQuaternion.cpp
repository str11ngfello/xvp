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
//	Module Name:	XQuaternion.cpp
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
Constructs an XQuaternion object by initializing the components to the identity
quaternion as seen below.

[0 0 0 1]
*/
//*****************************************************************************
XQuaternion::XQuaternion()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}



//*****************************************************************************
/**
Constructs an XQuaternion object by initializing the components to the passed
in parameters.

@param ix The initial x value.
@param iy The initial y value.
@param iz The initial z value.
@param iw The initial w value.
*/
//*****************************************************************************
XQuaternion::XQuaternion(XF32 ix,XF32 iy,XF32 iz,XF32 iw)
{
	x = ix;
	y = iy;
	z = iz;
	w = iw;
}



//*****************************************************************************
/**
Constructs an XQuaternion object by initializing the components to the passed
in parameters.

@param v The initial axis.
@param angle The initial angle.
*/
//*****************************************************************************
XQuaternion::XQuaternion(XVector3D& v,XF32 angle)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = angle;
}



//*****************************************************************************
/**
Destructor for the quaternion.
*/
//*****************************************************************************
/*XQuaternion::~XQuaternion()
{

}*/



//*****************************************************************************
/**
Determines if two quaternions are equal to each other.

@param Q	The quaternion against which to check for equality.
@return True if the quaternions are equal, false otherwise.
*/
//*****************************************************************************
XBOOL XQuaternion::operator==(XQuaternion& q)
{
	if ((fabs(x - q.x) > X_MATH_EPSILON) ||
		(fabs(y - q.y) > X_MATH_EPSILON) ||
		(fabs(z - q.z) > X_MATH_EPSILON) ||
		(fabs(w - q.w) > X_MATH_EPSILON))
		return false;
	else
		return true;
}



//*****************************************************************************
/**
Determines if two quaternions are not equal to each other.

@param Q	The quaternion against which to check for inequality.
@return True if the quaternions are unequal, false otherwise.
*/
//*****************************************************************************
XBOOL XQuaternion::operator!=(XQuaternion& q)
{
	if ((fabs(x - q.x) > X_MATH_EPSILON) ||
		(fabs(y - q.y) > X_MATH_EPSILON) ||
		(fabs(z - q.z) > X_MATH_EPSILON) ||
		(fabs(w - q.w) > X_MATH_EPSILON))
		return true;
	else
		return true;
}



//*****************************************************************************
/**
Multiplies two XQuaternion objects according to the following formula where
Q1 and Q2 are the quaternions to multiply, and R is the resulting quaternion.

R.x = Q1.w * Q2.x + Q1.x * Q2.w + Q1.y * Q2.z - Q1.z * Q2.y <br>
R.y = Q1.w * Q2.y + Q1.y * Q2.w + Q1.z * Q2.x - Q1.x * Q2.z <br>
R.z = Q1.w * Q2.z + Q1.z * Q2.w + Q1.x * Q2.y - Q1.y * Q2.x <br>
R.w = Q1.w * Q2.w - Q1.x * Q2.x - Q1.y * Q2.y - Q1.z * Q2.z <br>

@param Q	The quaternion to multiply by.
@return The product of the two quaternions.
*/
//*****************************************************************************
XQuaternion XQuaternion::operator*(XQuaternion& Q)
{
	return XQuaternion(w*Q.x + x*Q.w + y*Q.z - z*Q.y,
						w*Q.y + y*Q.w + z*Q.x - x*Q.z,
						w*Q.z + z*Q.w + x*Q.y - y*Q.x,
						w*Q.w - x*Q.x - y*Q.y - z*Q.z);
}



//*****************************************************************************
/**
Multiplies two XQuaternion objects according to the following formula where
Q1 and Q2 are the quaternions to multiply, and R is the resulting quaternion.
This XQuaternion object is assigned the result.

R.x = Q1.w * Q2.x + Q1.x * Q2.w + Q1.y * Q2.z - Q1.z * Q2.y <br>
R.y = Q1.w * Q2.y + Q1.y * Q2.w + Q1.z * Q2.x - Q1.x * Q2.z <br>
R.z = Q1.w * Q2.z + Q1.z * Q2.w + Q1.x * Q2.y - Q1.y * Q2.x <br>
R.w = Q1.w * Q2.w - Q1.x * Q2.x - Q1.y * Q2.y - Q1.z * Q2.z <br>

@param Q	The quaternion to multiply by.
@return The product of the two quaternions.
*/
//*****************************************************************************
XQuaternion& XQuaternion::operator*=(XQuaternion& Q)
{
	x = w * Q.x + x * Q.w + y * Q.z - z * Q.y;
	y = w * Q.y - x * Q.z + y * Q.w + z * Q.x;
	z = w * Q.z + x * Q.y - y * Q.x + z * Q.w;
	w = w * Q.w - x * Q.x - y * Q.y - z * Q.z;

	return *this;
}



//*****************************************************************************
/**
Adds two XQuaternion objects according to the following formula where Q1 and
Q2 are the XQuaternion objects to add, and R is the resulting quaternion.

R.x = Q1.x + Q2.x <br>
R.y = Q1.y + Q2.y <br>
R.z = Q1.z + Q2.z <br>
R.w = Q1.w + Q2.w <br>

@param Q	The quaternion to add.
@return The sum of the two quaternions.
*/
//*****************************************************************************
XQuaternion XQuaternion::operator+(XQuaternion& Q)
{
	return XQuaternion(x + Q.x,y + Q.y,z + Q.z,w + Q.w);
}



//*****************************************************************************
/**
Adds two XQuaternion objects according to the following formula where Q1 and
Q2 are the XQuaternion objects to add, and R is the resulting quaternion.  This
XQuaternion object is assigned the result.

R.x = Q1.x + Q2.x <br>
R.y = Q1.y + Q2.y <br>
R.z = Q1.z + Q2.z <br>
R.w = Q1.w + Q2.w <br>

@param Q	The quaternion to add.
@return The sum of the two quaternions.
*/
//*****************************************************************************
XQuaternion& XQuaternion::operator+=(XQuaternion& Q)
{
	x += Q.x;
	y += Q.y;
	z += Q.z;
	w += Q.w;
	
	return *this;
}



//*****************************************************************************
/**
Subtracts two XQuaternion objects according to the following formula where Q1 and
Q2 are the XQuaternion objects to subtract, and R is the resulting quaternion.

R.x = Q1.x - Q2.x <br>
R.y = Q1.y - Q2.y <br>
R.z = Q1.z - Q2.z <br>
R.w = Q1.w - Q2.w <br>

@param Q	The quaternion to subtract.
@return The difference of the two quaternions.
*/
//*****************************************************************************
XQuaternion XQuaternion::operator-(XQuaternion& Q)
{
	return XQuaternion(x - Q.x,y - Q.y,z - Q.z,w - Q.w);
}



//*****************************************************************************
/**
Subtracts two XQuaternion objects according to the following formula where Q1 and
Q2 are the XQuaternion objects to subtract, and R is the resulting quaternion.
This XQuaternion object is assigned the result.

R.x = Q1.x - Q2.x <br>
R.y = Q1.y - Q2.y <br>
R.z = Q1.z - Q2.z <br>
R.w = Q1.w - Q2.w <br>

@param Q	The quaternion to subtract.
@return The difference of the two quaternions.
*/
//*****************************************************************************
XQuaternion& XQuaternion::operator-=(XQuaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	
	return *this;
}



//*****************************************************************************
/**
Returns the XMatrix4x4 object representation of this XQuaternion object.

@return The 4x4 matrix that represents this quaternion.
*/
//*****************************************************************************
XMatrix4x4 XQuaternion::toMatrix4x4()
{
	XMatrix4x4 result;
 
	XF32 xsq = x * x;
	XF32 ysq = y * y;
	XF32 zsq = z * z;

	result._11 = 1.0f - 2*ysq - 2*zsq; result._21 = 2*x*y - 2*w*z; result._31 = 2*x*z + 2*w*y; result._41 = 0.0f;
 	result._12 = 2*x*y + 2*w*z; result._22 = 1.0f - 2*xsq - 2*zsq; result._32 = 2*y*z - 2*w*x; result._42 = 0.0f;
 	result._13 = 2*x*z - 2*w*y; result._23 = 2*y*z + 2*w*x; result._33 = 1.0f - 2*xsq - 2*ysq; result._43 = 0.0f;
 	result._14 = 0.0f; result._24 = 0.0f; result._34 = 0.0f; result._44 = 1.0f;

	return result;
}



//*****************************************************************************
/**
Spherically interpolates between two XQuaternion objects.

@param Q The XQuaternion object to interplate to.
@param t The interpolation factor.  This value must be in the range of 0 to 1.0.
*/
//*****************************************************************************
XQuaternion XQuaternion::slerp(XQuaternion& Q,XF32 t)
{
  XF32 norm;
  norm = x * Q.x + y * Q.y + z * Q.z + w * Q.w;

  XBOOL bFlip = false;

  if(norm < 0.0f)
  {
    norm = -norm;
    bFlip = true;
  }

  XF32 inv_d;
  if(1.0f - norm < 0.000001f)
  {
    inv_d = 1.0f - t;
  }
  else
  {
    XF32 theta;
    theta = (XF32) acos(norm);

    XF32 s;
    s = (XF32) (1.0f / sin(theta));

    inv_d = (XF32) sin((1.0f - t) * theta) * s;
    t = (XF32) sin(t * theta) * s;
  }

  if(bFlip)
  {
    t = -t;
  }

  return XQuaternion(inv_d * x + t * Q.x,
					  inv_d * y + t * Q.y,
                      inv_d * z + t * Q.z,
                      inv_d * w + t * Q.w);
}



//*****************************************************************************
/**
Normalizes an XQuaternion object.

@return None.
@remark This function will modify this XQuaternion object.
*/
//*****************************************************************************
void XQuaternion::normalize()
{
    XF32 s = (x*x)+(y*y)+(z*z)+(w*w);

    if (s > 0.0f)
    {
        s = (XF32)(1.0 / sqrt(s));
        x *= s;
        y *= s;
		z *= s;
		w *= s;
    }
    else
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1;
    }
}



//*****************************************************************************
/**
Inverts an XQuaternion object by negating the x, y, and z components.

@return None.
@remark This function will modify the XQuaternion object permanently.
*/
//*****************************************************************************
void XQuaternion::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}



//*****************************************************************************
/**
Sets this XQuaternion object to the identity quaternion where the x, y and z
components are equal to 0, and the w component is equal to 1 as seen below.

[0 0 0 1]

@return None.
*/
//*****************************************************************************
void XQuaternion::setIdentity()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}