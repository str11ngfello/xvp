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
//	Module Name:	XUI2DMovie.h
//	Author:			Lynn Duke
//	Creation Date:	7-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_2D_MOVIE_H__
#define __X_UI_2D_MOVIE_H__





//*****************************************************************************
//! 2D movie control designed to be used with the XUserInterface object.
/**
The XUI2DMovie object is designed to be used with the XUserInterface object.  
Functionally, it is very similar to the X2DMovie object.  In fact, it
serves as a wrapper around an X2DMovie object so that it can easily be inserted
into an XUserInterface object.   

See the member functions below for more information.
*/
//*****************************************************************************
class XUI2DMovie : public XUIElement
{
protected:
	X2DMovie		m_movie;
  
public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUI2DMovie();
	~XUI2DMovie();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,XS8* pMovieFilename);
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus render();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus onResize(XF32 xScale,XF32 yScale);
	X2DMovie* getMovie();
};

#endif