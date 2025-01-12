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
//	Module Name:	XTexSeqSprite.h
//	Author:			Lynn Duke
//	Creation Date:	5-5-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TEX_SEQ_SPRITE_H__
#define __X_TEX_SEQ_SPRITE_H__





//*****************************************************************************
//! Represents a sprite object that can play a texture sequence.
/**
The XTexSeqSprite object is used to render texture sequences.  The sprite's
fundamental operation is to play a texture sequence onto the screen.  
The sprite can be moved around on the screen by modifying the destination
rectangle appropriately.
*/
//*****************************************************************************
class XTexSeqSprite : public XBaseSprite
{
protected:
	XVector2D			m_position;
	XRect				m_srcRect;
	XRect				m_destRect;
	XBOOL				m_visible;
	XU32				m_width;
	XU32				m_height;
	XTextureSequence	m_texSeq;
	XF32				m_opacity;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTexSeqSprite();
	virtual ~XTexSeqSprite();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XS8* pFilenameBase,XS8* pExtension,XU32 numFiles);
	virtual XStatus destroy();
	void setSrcRect(XRect& srcRect);
	void getSrcRect(XRect& srcRect);
	void setDestRect(XRect& destRect);
	void getDestRect(XRect& destRect);
	void enableRender();
	void disableRender();
	void setPosition(XVector2D& position);
	XVector2D getPosition();
	XBOOL intersects(XRect& rect);
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XTextureSequence* getTextureSequence();
	XStatus setOpacity(XF32 opacity);
	XF32 getOpacity();
};


#endif