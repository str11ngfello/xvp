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
//	Module Name:	XSprite.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SPRITE_H__
#define __X_SPRITE_H__





//*****************************************************************************
//! Represents a sprite object that can render static images to the screen.
/**
The XSprite object is used to render static images to the screen.  The sprite's
fundamental operation is to take a rectangle of image data from a source image
and blit it to a destination rectangle on the screen.  The sprite can be moved
around on the screen by modifying the destination rectangle appropriately.

For sprites with animation capabilites, see the XAnimSprite object.
*/
//*****************************************************************************
class XSprite : public XBaseSprite
{
protected:
	XVector2D		m_position;
	XRect			m_srcRect;
	XRect			m_destRect;
	XHandle			m_textureHandle;
	XBOOL			m_visible;
	XU32			m_width;
	XU32			m_height;
	XF32			m_opacity;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSprite();
	virtual ~XSprite();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XS8* pTextureFilename);
	virtual XStatus destroy();
	XStatus setTextureHandle(XHandle texHandle);
	XHandle getTextureHandle();
	void setSrcRect(XRect& srcRect);
	void getSrcRect(XRect& srcRect);
	void setDestRect(XRect& destRect);
	void getDestRect(XRect& destRect);
	void enableRender();
	void disableRender();
	XStatus setOpacity(XF32 opacity);
	XF32 getOpacity();
	void setPosition(XVector2D& position);
	XVector2D getPosition();
	XBOOL intersects(XRect& rect);
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
};


#endif