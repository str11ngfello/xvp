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
//	Module Name:	XUIAnimSprite.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the user interface animated sprite.
*/
//*****************************************************************************
XUIAnimSprite::XUIAnimSprite()
{
}



//*****************************************************************************
/**
Destructor for the user interface animated sprite.
*/
//*****************************************************************************
XUIAnimSprite::~XUIAnimSprite()
{
}



//*****************************************************************************
/**
Initializes the user interface animated sprite.

@param pParentUI	The user interface to which this animated sprite belongs.
@param ID	The element ID used to identify this user interface animated sprite
		within a user interface.
@param zOrder	The zOrder of the user interface animated sprite with respect to other
		user interface elements. 
@param screenRect	The rectangular area of the screen in which the user interface
			animated sprite will be displayed.
@param pTextureFilename The texture that contains the frames of the animated sprite.
@param desc	An animated sprite description structure that describes the animated sprite.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIAnimSprite::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
								XS8* pTemplateFile)
{
	if (!pTemplateFile)
		return X_ERR_INVALID_PARAMS;

	char path[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_UI_TEMPLATE,pTemplateFile,path);

	TiXmlDocument file(path);
	if (!file.LoadFile())
		return X_ERR_OPERATION_FAILED;
	else
	{
		TiXmlNode* pNode = NULL;
		TiXmlElement* pAnimSpriteElement = NULL;
		
		pNode = file.FirstChild( "animsprite" );
		if (pNode)
		{
			pAnimSpriteElement = pNode->ToElement();
			if (pAnimSpriteElement)
			{	
				XS8* pFilename = NULL;
				XS32 intValue = 0;
				XF64 doubleValue = 0;
				XAnimSpriteDesc desc;
				pFilename = (XS8*)pAnimSpriteElement->Attribute("TextureFile");
				pAnimSpriteElement->QueryIntAttribute("BeginRow",&intValue);
				desc.beginRow = intValue;
				pAnimSpriteElement->QueryIntAttribute("EndRow",&intValue);
				desc.endRow = intValue;
				pAnimSpriteElement->QueryIntAttribute("EndCol",&intValue);
				desc.endCol = intValue;
				pAnimSpriteElement->QueryIntAttribute("FrameWidth",&intValue);
				desc.frameWidth = intValue;
				pAnimSpriteElement->QueryIntAttribute("FrameHeight",&intValue);
				desc.frameHeight = intValue;
				pAnimSpriteElement->QueryIntAttribute("Visible",&intValue);
				desc.visible = (XBOOL)intValue;
				pAnimSpriteElement->QueryDoubleAttribute("DelayFrameTime",&doubleValue);
				desc.delayFrameTime = (float)doubleValue;

				if (pFilename)
				{
					XStatus status = m_animSprite.init(pFilename,desc);
					if (status == X_OK)
						m_nativeScreenRect = screenRect;
					else
						return X_ERR_OPERATION_FAILED;
				}
			}
		}
		else
			return X_ERR_OPERATION_FAILED;
	
		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);
	}

	return X_OK;
}



//*****************************************************************************
/**
Destroys the user interface animated sprite and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIAnimSprite::destroy()
{
	m_animSprite.destroy();

	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
/**
Updates the user interface animated sprite.  This function is called automatically by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
			the last update.
@param xScale		The horizontal scale factor relative to the original
			width of the user interface animated sprite.
@param yScale		The vertical scale factor relative to the original
			height of the user interface animated sprite.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIAnimSprite::update(XF32 dt)
{
	//Update the sprite if enabled
	if (m_enabled)
		m_animSprite.update(dt);

	return X_OK;
}



//*****************************************************************************
/**
Renders the animated sprite to the screen.  This function is called
automatically by the engine once per frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIAnimSprite::render()
{
	if (!m_visible)
		return X_OK;

	m_animSprite.render();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIAnimSprite::onResize(XF32 xScale,XF32 yScale)
{
	//Adjust the size of the sprite if necessary.
	m_scaledScreenRect.left = (int)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (int)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (int)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (int)(m_nativeScreenRect.bottom * yScale);
	m_animSprite.setDestRect(m_scaledScreenRect);

	return X_OK;
}

	
	
//*****************************************************************************
/**
Returns the XAnimSprite object associated with the user interface animated sprite.

@return The XAnimSprite object associated with the user interface animated sprite.
*/
//*****************************************************************************
XAnimSprite* XUIAnimSprite::getAnimSprite()
{
	return &m_animSprite;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIAnimSprite::setOpacity(XF32 opacity)
{
	m_animSprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIAnimSprite::move(XVector2D& moveVector)
{
	XRect r;
	m_animSprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_animSprite.setDestRect(r);

	m_nativeScreenRect.left += (XU32)moveVector.x;
	m_nativeScreenRect.right += (XU32)moveVector.x;
	m_nativeScreenRect.top += (XU32)moveVector.y;
	m_nativeScreenRect.bottom += (XU32)moveVector.y;

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}