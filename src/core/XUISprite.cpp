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
//	Module Name:	XUISprite.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the user interface sprite.
*/
//*****************************************************************************
XUISprite::XUISprite()
{
}



//*****************************************************************************
/**
Destructor for the user interface sprite.
*/
//*****************************************************************************
XUISprite::~XUISprite()
{
}



//*****************************************************************************
/**
Initializes the user interface sprite.

@param pParentUI	The user interface to which this sprite belongs.
@param ID	The element ID used to identify this user interface sprite 
			within a user interface.
@param zOrder	The zOrder of the user interface sprite with respect to other
				user interface elements. 
@param screenRect	The rectangular area of the screen in which the sprite 
					will be displayed.
@param pTextureFilename	The texture that contains the sprite image.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUISprite::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
						XS8* pTemplateFile)
{
	//Validate parameters.
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
		TiXmlElement* pSpriteElement = NULL;
		
		pNode = file.FirstChild( "sprite" );
		if (pNode)
		{
			pSpriteElement = pNode->ToElement();
			if (pSpriteElement)
			{	
				XRect r;
				XS8* pFilename = NULL;
				pSpriteElement->QueryIntAttribute("SrcLeft",&r.left);
				pSpriteElement->QueryIntAttribute("SrcTop",&r.top);
				pSpriteElement->QueryIntAttribute("SrcRight",&r.right);
				pSpriteElement->QueryIntAttribute("SrcBottom",&r.bottom);

				pFilename = (XS8*)pSpriteElement->Attribute("TextureFile");
				if (pFilename)
				{
					if (m_sprite.init(pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_nativeScreenRect = screenRect;
					m_sprite.setSrcRect(r);
				}
				else
					return X_ERR_OPERATION_FAILED;
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
Destroys the user interface sprite and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUISprite::destroy()
{
	m_sprite.destroy();

	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
/**
Updates the user interface sprite.  This function is called by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface sprite.
@param yScale	The vertical scale factor relative to the original
				height of the user interface sprite.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUISprite::update(XF32 dt)
{
	//Update the sprite.
	m_sprite.update(dt);

	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface sprite.  This function is called
automatically by the engine once per frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUISprite::render()
{
	if (!m_visible)
		return X_OK;

	m_sprite.render();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISprite::setOpacity(XF32 opacity)
{
	m_sprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISprite::onResize(XF32 xScale,XF32 yScale)
{
	//Adjust the size of the sprite if necessary.
	m_scaledScreenRect.left = (XS32)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (XS32)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (XS32)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (XS32)(m_nativeScreenRect.bottom * yScale);
	m_sprite.setDestRect(m_scaledScreenRect);

	return X_OK;
}



//*****************************************************************************
/**
Returns the XSprite object associated with this user interface sprite.

@return The XSprite object associated with this user interface sprite.
*/
//*****************************************************************************
XSprite* XUISprite::getSprite()
{
	return &m_sprite;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUISprite::move(XVector2D& moveVector)
{
	XRect r;
	m_sprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_sprite.setDestRect(r);

	m_nativeScreenRect.left += (XU32)moveVector.x;
	m_nativeScreenRect.right += (XU32)moveVector.x;
	m_nativeScreenRect.top += (XU32)moveVector.y;
	m_nativeScreenRect.bottom += (XU32)moveVector.y;

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}