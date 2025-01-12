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
//	Module Name:	XRect.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-17-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructs an XRect object by initializing left, top, right and bottom member
variables to 0.
*/
//*****************************************************************************
XRect::XRect()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}



//*****************************************************************************
/**
Constructs an XRect object by initializing left, top, right and bottom member
variables to the passed in parameters.

@param l	The left side of the rectangle in screen space (pixel space). 
@param t	The top side of the rectangle in screen space (pixel space). 
@param r	The right side of the rectangle in screen space (pixel space). 
@param b	The bottom side of the rectangle in screen space (pixel space). 
*/
//*****************************************************************************
XRect::XRect(XS32 l,XS32 t,XS32 r,XS32 b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}



//*****************************************************************************
/**
Destructor for the rectangle.
*/
//*****************************************************************************
XRect::~XRect()
{
}



//*****************************************************************************
/**
Sets the rectangles left, top, right and bottom member variables equal to the
passed in parameters.

@param l	The left side of the rectangle in screen space (pixel space). 
@param t	The top side of the rectangle in screen space (pixel space). 
@param r	The right side of the rectangle in screen space (pixel space). 
@param b	The bottom side of the rectangle in screen space (pixel space). 
@return None.
*/
//*****************************************************************************
void XRect::set(XS32 l,XS32 t,XS32 r,XS32 b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}



//*****************************************************************************
/**
Gets the width of the rectangle.

@return The width of the rectangle.
*/
//*****************************************************************************
XS32 XRect::getWidth()
{
	return right-left;
}



//*****************************************************************************
/**
Gets the height of the rectangle.

@return The height of the rectangle.
*/
//*****************************************************************************
XS32 XRect::getHeight()
{
	return bottom-top;
}



//*****************************************************************************
/**
Copy constructor for the XRect object.  It will copy the left, top, right and
bottom member variables into this XRect object.

@param r	The XRect object to copy.
@return		The XRect object assigned.
*/
//*****************************************************************************
XRect& XRect::operator = (XRect& r)
{
	left = r.left;
	top = r.top;
	right = r.right;
	bottom = r.bottom;

	return (*this);
}



//*****************************************************************************
/**
Determines if two XRect objects intersect.

@param r	The XRect object to use for the intersection test.
@return True if the two rectangles intersect, false otherwise.
*/
//*****************************************************************************
XBOOL XRect::intersects(XRect& r)
{
	if (r.bottom < top) 
		return false;
	if (r.top > bottom) 
		return false;
	if (r.right < left)
		return false;
	if (r.left > right)
		return false;
 
	return true;
}



//*****************************************************************************
/**
Determines if a 2D point is contained within the bounds of the rectangle.

@param point	The 2D point to check for containment.
@return True if the point is contained within the bounds of the rectangle,
		false otherwise.
*/
//*****************************************************************************
XBOOL XRect::isPointInside(XVector2D& point)
{
	if ((point.x >= left) && (point.x <= right) 
		&& (point.y >= top) && (point.y <= bottom))
		return true;
	else
		return false;
}

