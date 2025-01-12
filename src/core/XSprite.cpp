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
//	Module Name:	XSprite.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructs an XSprite object by initializing the sprite properties to
the following default values:<br><br>

<ul>
<li> Texture Handle = X_INVALID_HANDLE </li>
<li> Visible = TRUE </li>
</ul>
*///*****************************************************************************
XSprite::XSprite()
{
	m_textureHandle = X_INVALID_HANDLE;
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
XSprite::~XSprite()
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
XStatus XSprite::update(XF32 dt)
{
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
XStatus XSprite::render()
{
	if ((m_visible) && (m_textureHandle != X_INVALID_HANDLE))
	{
		XVP::GraphicsMgr.setTexture(m_textureHandle,0);
		return XVP::GraphicsMgr.renderTexture(m_width,m_height,m_srcRect,m_destRect,
											m_opacity);
	}
	else
		return X_OK;
}



//*****************************************************************************
/**
Initializes the sprite.  This function must be called before the sprite is
added to scene graph since it associates an image with the sprite.

@param pTextureFilename	The path to an image to be used as the sprite.  If this
						parameter is NULL, then no image will be associated
						with this sprite.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSprite::init(XS8* pTextureFilename)
{
	//If the texure filename is NULL, then no texture should be used
	//for this sprite.
	if (!pTextureFilename)
		return X_OK;

	//Destroy any current resources in case the user didn't do it.
	destroy();
	XSprite();

	//Create the new texture for use by the sprite.  
	if (XVP::GraphicsMgr.createTextureFromFile(pTextureFilename,m_textureHandle) == X_OK)
	{
		//Set the source and destination rectange to match the dimensions
		//of the texture.
		m_width = XVP::GraphicsMgr.getTextureWidth(m_textureHandle);
		m_height = XVP::GraphicsMgr.getTextureHeight(m_textureHandle);
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
XStatus XSprite::destroy()
{
	if (m_textureHandle != X_INVALID_HANDLE)
	{
		XStatus status;
		status = XVP::GraphicsMgr.destroyTexture(m_textureHandle);
		m_textureHandle = X_INVALID_HANDLE;
		return status;
	}
	
	m_textureHandle = X_INVALID_HANDLE;

	return X_OK;
}



//*****************************************************************************
/**
Sets an initialized texture to be used for the sprite.  This method will
not destroy the sprite's current texture.  You can get the current handle, destroy
it, then call this method to set a new handle.

@param texHandle	The handle to an initialized texture that will become the
				sprite's new texture.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSprite::setTextureHandle(XHandle texHandle)
{
	//Destroy the old texture handle and bump the reference count of the new one
	if (m_textureHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyTexture(m_textureHandle);

	m_textureHandle = texHandle;
	XVP::GraphicsMgr.addTextureRef(texHandle);

	//Set the new width and height if not an invalid handle
	if (texHandle != X_INVALID_HANDLE)
	{
		m_width = XVP::GraphicsMgr.getTextureWidth(m_textureHandle);
		m_height = XVP::GraphicsMgr.getTextureHeight(m_textureHandle);
	}

	return X_OK;
}

	
	
//*****************************************************************************
/**
Gets the sprite's texture handle.

@return The sprite's texture handle.
*/
//*****************************************************************************
XHandle XSprite::getTextureHandle()
{
	return m_textureHandle;
}



//*****************************************************************************
/**
Sets the source rectangle.  This is the retangular region in the source image
that will be blitted to the screen.

@param srcRect	The rectangle representing the area to blit from the source image.
@return None.
*/
//*****************************************************************************
void XSprite::setSrcRect(XRect& srcRect)
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
void XSprite::getSrcRect(XRect& srcRect)
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
void XSprite::setDestRect(XRect& destRect)
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
void XSprite::getDestRect(XRect& destRect)
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
void XSprite::setPosition(XVector2D& position)
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
XVector2D XSprite::getPosition()
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
XBOOL XSprite::intersects(XRect& rect) 
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
void XSprite::enableRender()
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
void XSprite::disableRender()
{
	m_visible = false;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSprite::setOpacity(XF32 opacity)
{
	m_opacity = opacity;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XSprite::getOpacity()
{
	return m_opacity;
}