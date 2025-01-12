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
//	Module Name:	XUISprite.h
//	Author:			Lynn Duke
//	Creation Date:	6-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_SPRITE_H__
#define __X_UI_SPRITE_H__




//*****************************************************************************
//! Sprite object for use with user interfaces.
/**
The XUISprite is a specific type of 2D sprite that is compatible with the
XUserInterface object.  The XUISprite object is very similar to the XSprite
object feature-wise, but is specifically suited for user interfaces.   

See the member functions below for more information.
*/
//*****************************************************************************
class XUISprite : public XUIElement
{
protected:
	XSprite		m_sprite;


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUISprite();
	virtual ~XUISprite();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
						XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus onResize(XF32 xScale,XF32 yScale);
	XSprite* getSprite();
};


#endif
