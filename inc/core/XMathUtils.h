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
//	Module Name:	XMathLib.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MATHLIB_H__
#define __X_MATHLIB_H__




//*****************************************************************************
//Floating Point Robustness
//*****************************************************************************
#define X_MATH_EPSILON			(.00001f)
#define X_MATH_IS_XF32_ZERO(x)	((fabs(x) <= X_MATH_EPSILON) ? true : false)
#define X_MATH_XF64_EPSILON		(2.220446049250313e-16)
#define X_MATH_IS_XF64_ZERO(x)	((fabs(x) <= X_MATH_XF64_EPSILON) ? true : false)



//*****************************************************************************
//Random Number Generation Defines
//*****************************************************************************
#define X_MATH_RAND_SIGN			((XGenRandFloat01() > .5f)?-1:1)
#define X_MATH_RAND_F_0_P1			(XGenRandFloat01())
#define X_MATH_RAND_F_N1_P1			(XGenRandFloat01() *  (X_MATH_RAND_SIGN))
#define X_MATH_RAND_NORM_VEC		(XVector3D(X_MATH_RAND_F_N1_P1,X_MATH_RAND_F_N1_P1,X_MATH_RAND_F_N1_P1))
#define X_MATH_RAND_PERCENT(x)		(x * X_MATH_RAND_F_0_P1)



//*****************************************************************************
//Conversion Defines
//*****************************************************************************
#define X_MATH_RAD_PER_DEG		(.017453f)
#define X_MATH_DEG_PER_RAD		(6.28319f / 360.0f)
#define X_MATH_DEG_TO_RAD(d)	(d * 0.0174532925199f)
#define X_MATH_RAD_TO_DEG(r)	(r * 57.2957795132f)
#define X_MATH_ABS(x)			(((x)<0) ? -(x) : (x))
#define X_MATH_FLOOR(x)			((x)>0 ? (XS32)(x) : -(XS32)(-x))
#define X_MATH_CEILING(x)		((x)==(XS32)(x) ? (x) : (x)>0 ? 1+(XS32)(x) : -(1+(XS32)(-x)))
#define X_MATH_ROUND(x)			((x)>0 ? (XS32)(x+0.5f) : -(XS32)(0.5f-x))		
#define X_MATH_SQUARE(x)		((x)*(x))


//*****************************************************************************
//Standard Math Value Defines
//*****************************************************************************
#define X_MATH_HALFPI			(1.57079f)
#define X_MATH_PI				((XF32)3.1415926535897932385f)
#define X_MATH_2PI				(6.28319f)
#define X_MATH_PIDIV2			(X_MATH_PI/2.0f)
#define X_MATH_E				((XF32)2.7182818284590452354f)



//*****************************************************************************
//Miscellaneous Math Defines
//*****************************************************************************
#define X_MATH_MIN(a,b)			(((a)<(b))?(a):(b))	
#define X_MATH_MAX(a,b)			(((a)>(b))?(a):(b))
#define X_MATH_MIN3(a,b,c)		((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define X_MATH_MAX3(a,b,c)		((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))
#define X_MATH_SWAP(a,b)		{a^=b; b^=a; a^=b;}
#define X_MATH_LERP(a,l,h)		((l)+(((h)-(l))*(a)))
#define X_MATH_CLAMP(x,min,max)	{x = (x < min ? min : x < max ? x : max);}
#define X_MATH_IS_ODD(a)		(((a)&1) ? 1 : 0) 



//*****************************************************************************
//Enums
//*****************************************************************************
enum X_MATH_DIRECTION
{
	X_MD_X,
	X_MD_Y,
	X_MD_Z
};

enum X_MATH_PLANE
{
	X_MP_XY,
	X_MP_YZ,
	X_MP_XZ
};



//*****************************************************************************
//Forward Declarations
//*****************************************************************************
class XVector2D;
class XVector3D;
class XVector4D;
class XMatrix3x3;
class XMatrix4x4;
class XQuaternion;
class XPolygon;
class XRay3D;
class XPlane;



//*****************************************************************************
//Non-Member Functions
//*****************************************************************************
XVector2D operator * (XF32 scalar,XVector2D& V);
XVector3D operator * (XF32 scalar,XVector3D& V);
XVector4D operator * (XF32 scalar,XVector4D& V);
XVector3D operator * (XVector3D& V,XMatrix3x3& M);
XVector4D operator * (XVector4D& V,XMatrix4x4& M);



//*****************************************************************************
//NAN and Infinity 
//*****************************************************************************
static XS32 ExponentMask = 0x7f800000;
static XS32 MantissaMask = 0x007fffff;

bool IsNAN(XF32 f);
bool IsInfinity(XF32 f);
bool IsInfinityOrNAN(XF32 f);
bool IsNAN(XVector2D& vec);
bool IsInfinity(XVector2D& vec);
bool IsInfinityOrNAN(XVector2D& vec);
bool IsNAN(XVector3D& vec);
bool IsInfinity(XVector3D& vec);
bool IsInfinityOrNAN(XVector3D& vec);
bool IsNAN(XVector4D& vec);
bool IsInfinity(XVector4D& vec);
bool IsInfinityOrNAN(XVector4D& vec);


//*****************************************************************************
// Miscellaneous Functions
//*****************************************************************************
XF32 X_ArcTan(XF32 y,XF32 x);

#endif