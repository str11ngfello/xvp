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
//	Module Name:	XRect.h
//	Author:			Lynn Duke
//	Creation Date:	1-17-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RECT_H__
#define __X_RECT_H__




//*****************************************************************************
//! Represents a 2D rectangle.
/** 
The XRect object represents a 2 dimensional rectangle.  It has left, top, right,
and bottom member variables that specify the position and size of the rectangle
in screen space. 

The sprite objects use this class heavily to identify subsections of their
source image to blit to the screen as well as the destination area of the
screen to perform the blit.  See below for more information.
*/
//*****************************************************************************
class XRect 
{
public:
	XS32 left;
	XS32 top;
	XS32 right;
	XS32 bottom;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XRect();
	XRect(XS32 l,XS32 t,XS32 r,XS32 b);
	~XRect();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	void set(XS32 l,XS32 t,XS32 r,XS32 b);
	XRect& operator = (XRect& r);
	XS32 getWidth();
	XS32 getHeight();
	XBOOL intersects(XRect& r);
	XBOOL isPointInside(XVector2D& point);
};


#endif