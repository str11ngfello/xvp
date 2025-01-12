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
//	Module Name:	XUIAnimSprite.h
//	Author:			Lynn Duke
//	Creation Date:	7-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_ANIM_SPRITE_H__
#define __X_UI_ANIM_SPRITE_H__





//*****************************************************************************
//! Anim Sprite specifically designed for use with the XUserInterface object.
/**
The XUIAnimSprite object is designed to be used within the context of an
XUserInterface.  Functionally, it is very similar to the XAnimSprite object.   
In fact, it serves as a wrapper around an XAnimSprite object so that it can
easily be inserted into an XUserInterface object.   

See the member functions below for more information.
*/
//*****************************************************************************
class XUIAnimSprite : public XUIElement
{
protected:
	XAnimSprite		m_animSprite;
	

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIAnimSprite();
	virtual ~XUIAnimSprite();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
				XS8* pTemplateFile);
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus render();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus onResize(XF32 xScale,XF32 yScale);
	XAnimSprite* getAnimSprite();
};


#endif