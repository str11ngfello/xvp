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
//	Module Name:	XTexSeqSprite.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-5-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructs an XTexSeqSprite object by initializing the sprite properties to
the following default values:<br><br>

<ul>
<li> Visible = TRUE </li>
</ul>
*///*****************************************************************************
XTexSeqSprite::XTexSeqSprite()
{
	m_visible = true;
	m_width = 0;
	m_height = 0;
	m_opacity = 1.0f;
}



//*****************************************************************************
/**
Destructor for the sprite.  
*/
//*****************************************************************************
XTexSeqSprite::~XTexSeqSprite()
{
}



//*****************************************************************************
/**
Updates the sprite.  This function is called to allow the sprite to update
any internal, time-dependent data.  Once the sprite is added to the scene
graph, X-VP will call this function at the appropriate times.  Normally,
you do not need to call this routine manually.

You can override this function to provide custom functionality for new types
of sprites.

@param dt	The amount of time, in seconds, that has passes since the
					last update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTexSeqSprite::update(XF32 dt)
{
	m_texSeq.update(dt);
	return X_OK;
}



//*****************************************************************************
/**
Renders the sprite to the screen.  Once the sprite is added to the scene
graph, X-VP will call this function at the appropriate times.  Normally,
you do not need to call this routine manually.

You can override this function to provide custom functionality for new types
of sprites.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTexSeqSprite::render()
{
	if (m_visible)
	{
		XVP::GraphicsMgr.setTexture(m_texSeq.getCurrentHandle(),0);
		return XVP::GraphicsMgr.renderTexture(m_width,m_height,m_srcRect,m_destRect,
											m_opacity);
	}
	else
		return X_OK;
}



//*****************************************************************************
/**
Initializes the sprite.  This function must be called before the sprite is
added to scene graph since it associates a texture sequence with the sprite.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTexSeqSprite::init(XS8* pFilenameBase,XS8* pExtension,XU32 numFiles)
{
	//Destroy any current resources in case the user didn't do it.
	destroy();
	XTexSeqSprite();

	//Create the texture sequence
	if (m_texSeq.init(pFilenameBase,pExtension,numFiles) == X_OK)
	{
		//Set the source and destination rectange to match the dimensions
		//of the texture sequence.
		m_width = m_texSeq.getWidth();
		m_height = m_texSeq.getHeight();
		m_position.x = 0;
		m_position.y = 0;
		m_srcRect.set(0,0,m_width,m_height);
		m_destRect = m_srcRect;
	
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Destroy the sprite by freeing all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XTexSeqSprite::destroy()
{
	m_texSeq.destroy();
	return X_OK;
}



//*****************************************************************************
/**
Sets the source rectangle.  This is the retangular region in the source image
that will be blitted to the screen.

@param srcRect	The rectangle representing the area to blit from the source image.
@return None.
*/
//*****************************************************************************
void XTexSeqSprite::setSrcRect(XRect& srcRect)
{
	m_srcRect = srcRect;
}



//*****************************************************************************
/**
Gets the source rectangle.  This is the retangular region in the source image
that will be blitted to the screen. 

@param srcRect	A variable to receive the source rectangle.
@return None.
*/
//*****************************************************************************
void XTexSeqSprite::getSrcRect(XRect& srcRect)
{
	srcRect = m_srcRect;
}



//*****************************************************************************
/**
Sets the destination rectangle.  This is the retangular region on the screen
where the sprite will be blitted.  

@param destRect	The rectangle representing the area to blit to on the screen.
@return None.
*/
//*****************************************************************************
void XTexSeqSprite::setDestRect(XRect& destRect)
{
	m_destRect = destRect;
}



//*****************************************************************************
/**
Gets the destination rectangle.  This is the retangular region on the screen
where the sprite will be blitted. 

@param destRect	A variable to receive the destination rectangle.
@return None.
*/
//*****************************************************************************
void XTexSeqSprite::getDestRect(XRect& destRect)
{
	destRect = m_destRect;
}



//*****************************************************************************
/**
Sets the position of the sprite.  The sprites current width and height are
maintained.

@param position	The new screen position of the sprite.
@return None.
*/
//*****************************************************************************
void XTexSeqSprite::setPosition(XVector2D& position)
{
	m_position = position;
	XU32 width = m_destRect.right - m_destRect.left;
	XU32 height = m_destRect.bottom - m_destRect.top;
	m_destRect.left = (int)m_position.x;
	m_destRect.top = (int)m_position.y;
	m_destRect.right = m_destRect.left + width;
	m_destRect.bottom = m_destRect.top + height;
}



//*****************************************************************************
/**
Returns the upper-left coordinate of the destination rectangle.

@return The upper-left coordinate of the destination rectangle.
*/
//*****************************************************************************
XVector2D XTexSeqSprite::getPosition()
{
	return m_position;
}



//*****************************************************************************
/**
Determines if a rectangle intersects with the sprite's destination rectangle
on the screen.

@param rect	The rectangle to intersect the sprite with.
@return True if the sprite's destination rectangle and the passed in rectangle
		intersect, false otherwise.
*/
//*****************************************************************************
XBOOL XTexSeqSprite::intersects(XRect& rect) 
{
	return m_destRect.intersects(rect);
}



//*****************************************************************************
/**
Enables the sprite to process render messages from the engine, effectively
allowing the sprite to render itself to the screen.  Use this function to
turn a sprite "on" after it has been disabled with a call to disableRender().

@return None.
*/
//*****************************************************************************
void XTexSeqSprite::enableRender()
{
	m_visible = true;
}



//*****************************************************************************
/**
Disables the sprite from processing render messages from the engine, effectively
keeping the sprite from rendering itself to the screen.  Use this function to
"turn off" the sprite temporarily without removing it from the scene graph.

@return None.
*/
//*****************************************************************************
void XTexSeqSprite::disableRender()
{
	m_visible = false;
}



//*****************************************************************************
//*****************************************************************************
XTextureSequence* XTexSeqSprite::getTextureSequence()
{
	return &m_texSeq;
}


//*****************************************************************************
//*****************************************************************************
XStatus XTexSeqSprite::setOpacity(XF32 opacity)
{
	m_opacity = opacity;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XTexSeqSprite::getOpacity()
{
	return m_opacity;
}
