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
//	penalties,and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	XMathLib.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


bool IsNAN(XF32 f)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&f;
	return ((i & ExponentMask) == ExponentMask) && ((i & MantissaMask) != 0);	// mantissa of 0 is infinity
}

bool IsInfinity(XF32 f)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&f;
	return (i & (ExponentMask | MantissaMask)) == ExponentMask;
}

bool IsInfinityOrNAN(XF32 f)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32 *)&f;
	return (i & ExponentMask) == ExponentMask;
}

bool IsNAN(XVector2D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&vec.x;
	XS32 j = *(XS32*)&vec.y;
	return ((((i & ExponentMask) == ExponentMask) && ((i & MantissaMask) != 0)) ||
			(((j & ExponentMask) == ExponentMask) && ((j & MantissaMask) != 0)));	// mantissa of 0 is infinity
}

bool IsInfinity(XVector2D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&vec.x;
	XS32 j = *(XS32*)&vec.y;
	return (((i & (ExponentMask | MantissaMask)) == ExponentMask) ||
			((j & (ExponentMask | MantissaMask)) == ExponentMask));
}

bool IsInfinityOrNAN(XVector2D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32 *)&vec.x;
	XS32 j = *(XS32 *)&vec.y;
	return (((i & ExponentMask) == ExponentMask) ||
			((j & ExponentMask) == ExponentMask));
}

bool IsNAN(XVector3D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&vec.x;
	XS32 j = *(XS32*)&vec.y;
	XS32 k = *(XS32*)&vec.z;
	return ((((i & ExponentMask) == ExponentMask) && ((i & MantissaMask) != 0)) ||
			(((j & ExponentMask) == ExponentMask) && ((j & MantissaMask) != 0)) ||
			(((k & ExponentMask) == ExponentMask) && ((k & MantissaMask) != 0)));	// mantissa of 0 is infinity
}

bool IsInfinity(XVector3D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&vec.x;
	XS32 j = *(XS32*)&vec.y;
	XS32 k = *(XS32*)&vec.z;
	return (((i & (ExponentMask | MantissaMask)) == ExponentMask) ||
			((j & (ExponentMask | MantissaMask)) == ExponentMask) ||
			((k & (ExponentMask | MantissaMask)) == ExponentMask));
}

bool IsInfinityOrNAN(XVector3D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32 *)&vec.x;
	XS32 j = *(XS32 *)&vec.y;
	XS32 k = *(XS32 *)&vec.z;
	return (((i & ExponentMask) == ExponentMask) ||
			((j & ExponentMask) == ExponentMask) ||
			((k & ExponentMask) == ExponentMask));
}

bool IsNAN(XVector4D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&vec.x;
	XS32 j = *(XS32*)&vec.y;
	XS32 k = *(XS32*)&vec.z;
	XS32 l = *(XS32*)&vec.w;
	return ((((i & ExponentMask) == ExponentMask) && ((i & MantissaMask) != 0)) ||
			(((j & ExponentMask) == ExponentMask) && ((j & MantissaMask) != 0)) ||
			(((k & ExponentMask) == ExponentMask) && ((k & MantissaMask) != 0)) ||
			(((l & ExponentMask) == ExponentMask) && ((l & MantissaMask) != 0)));	// mantissa of 0 is infinity
}

bool IsInfinity(XVector4D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32*)&vec.x;
	XS32 j = *(XS32*)&vec.y;
	XS32 k = *(XS32*)&vec.z;
	XS32 l = *(XS32*)&vec.w;
	return (((i & (ExponentMask | MantissaMask)) == ExponentMask) ||
			((j & (ExponentMask | MantissaMask)) == ExponentMask) ||
			((k & (ExponentMask | MantissaMask)) == ExponentMask) ||
			((l & (ExponentMask | MantissaMask)) == ExponentMask));
}

bool IsInfinityOrNAN(XVector4D& vec)
{
	X_ASSERTM(sizeof(XF32) == sizeof(XS32),"size of XF32 != size of XS32)");
	XS32 i = *(XS32 *)&vec.x;
	XS32 j = *(XS32 *)&vec.y;
	XS32 k = *(XS32 *)&vec.z;
	XS32 l = *(XS32 *)&vec.w;
	return (((i & ExponentMask) == ExponentMask) ||
			((j & ExponentMask) == ExponentMask) ||
			((k & ExponentMask) == ExponentMask) ||
			((l & ExponentMask) == ExponentMask));
}

//*****************************************************************************
//*****************************************************************************
XF32 X_ArcTan (XF32 y,XF32 x) 
{
	XF32 rad = (XF32)atan2(y,x);
	if (rad >= (XF32) 0.0)
		return rad;
	else
		return X_MATH_PI - rad;
}
