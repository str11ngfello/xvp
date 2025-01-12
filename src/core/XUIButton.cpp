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
//	Module Name:	XUIButton.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the user interface button.
*/
//*****************************************************************************
XUIButton::XUIButton()
{
	m_buttonState = X_UIBS_OUT;
	m_prevButtonState = X_UIBS_OUT;
	m_inTextureHandle = X_INVALID_HANDLE;
	m_outTextureHandle = X_INVALID_HANDLE;
	m_overTextureHandle = X_INVALID_HANDLE;
	m_inSoundHandle = X_INVALID_HANDLE;
	m_outSoundHandle = X_INVALID_HANDLE;
	m_overSoundHandle = X_INVALID_HANDLE;
	m_repeatEnabled = false;
	m_preventSound = false;
}



//*****************************************************************************
/**
Destructor for the user interface button.
*/
//*****************************************************************************
XUIButton::~XUIButton()
{
}



//*****************************************************************************
/**
Initializes the user interface button.

@param pParentUI	The user interface to which this button belongs.
@param ID	The element ID used to identify this user interface button within
		a user interface.
@param zOrder	The zOrder of the user interface button with respect to other
		user interface elements. 
@param screenRect	The rectangular area of the screen in which the user interface
			button will be displayed.
@param pOutTexFilename	The image displayed when the button is not depressed.
@param pInTexFilename	The image displayed when the button is depressed.
@param pOverTexFilename	The image displayed while the mouse if over the button,
			but the button is not depressed.
@param pInSoundFilename	The sound played when the button is depressed.
@param pOutSoundFilename	The sound played when the button returns to the
				non-depressed state.
@param pOverSoundFilename	The sound played when the mouse moves over the
				button.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIButton::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,XS8* pTemplateFile)/*XU32 srcWidth,
						XU32 srcHeight,,XS8* pOutTexFilename,
						   XS8* pInTexFilename,XS8* pOverTexFilename,XS8* pInSoundFilename,
						   XS8* pOutSoundFilename,XS8* pOverSoundFilename)*/
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
		TiXmlElement* pButtonElement = NULL;
		
		pNode = file.FirstChild( "button" );
		if (pNode)
		{
			pButtonElement = pNode->ToElement();
			if (pButtonElement)
			{	
				XS8* pFilename = NULL;
				pButtonElement->QueryIntAttribute("OutSrcLeft",&m_outSrcRect.left);
				pButtonElement->QueryIntAttribute("OutSrcTop",&m_outSrcRect.top);
				pButtonElement->QueryIntAttribute("OutSrcRight",&m_outSrcRect.right);
				pButtonElement->QueryIntAttribute("OutSrcBottom",&m_outSrcRect.bottom);
				pButtonElement->QueryIntAttribute("InSrcLeft",&m_inSrcRect.left);
				pButtonElement->QueryIntAttribute("InSrcTop",&m_inSrcRect.top);
				pButtonElement->QueryIntAttribute("InSrcRight",&m_inSrcRect.right);
				pButtonElement->QueryIntAttribute("InSrcBottom",&m_inSrcRect.bottom);
				pButtonElement->QueryIntAttribute("OverSrcLeft",&m_overSrcRect.left);
				pButtonElement->QueryIntAttribute("OverSrcTop",&m_overSrcRect.top);
				pButtonElement->QueryIntAttribute("OverSrcRight",&m_overSrcRect.right);
				pButtonElement->QueryIntAttribute("OverSrcBottom",&m_overSrcRect.bottom);
				
				m_nativeScreenRect = screenRect;
				m_sprite.setSrcRect(m_outSrcRect);

				pFilename = (XS8*)pButtonElement->Attribute("InTextureFile");
				if (pFilename)
				{
					if (XVP::GraphicsMgr.createTextureFromFile(pFilename,m_inTextureHandle) != X_OK)
						return X_ERR_OPERATION_FAILED;
				}
				pFilename = (XS8*)pButtonElement->Attribute("OverTextureFile");
				if (pFilename)
					if (XVP::GraphicsMgr.createTextureFromFile(pFilename,m_overTextureHandle) != X_OK)
						return X_ERR_OPERATION_FAILED;
				pFilename = (XS8*)pButtonElement->Attribute("OutTextureFile");
				if (pFilename)
					if (XVP::GraphicsMgr.createTextureFromFile(pFilename,m_outTextureHandle) != X_OK)
						return X_ERR_OPERATION_FAILED;
				pFilename = (XS8*)pButtonElement->Attribute("InSoundFile");
				if (pFilename)
					XVP::AudioMgr.createSoundFromFile(pFilename,m_inSoundHandle);
				pFilename = (XS8*)pButtonElement->Attribute("OutSoundFile");
				if (pFilename)
					XVP::AudioMgr.createSoundFromFile(pFilename,m_outSoundHandle);
				pFilename = (XS8*)pButtonElement->Attribute("OverSoundFile");
				if (pFilename)
					XVP::AudioMgr.createSoundFromFile(pFilename,m_overSoundHandle);

				//Start the button un-pressed.
				m_sprite.setTextureHandle(m_outTextureHandle);
				m_sprite.setSrcRect(m_outSrcRect);
			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;

		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);

		return X_OK;

	}


	
/*
	if (XUISprite::init(pParentUI,zOrder,srcWidth,srcHeight,screenRect,pOutTexFilename) != X_OK)
		return X_ERR_OPERATION_FAILED;
	
	m_outTextureHandle = m_sprite.getTextureHandle();

	if (pInTexFilename)
	{
		if (XVP::GraphicsMgr.createTexture(pInTexFilename,X_TT_2D,m_inTextureHandle) != X_OK)
			return X_ERR_OPERATION_FAILED;
	}

	if (pOverTexFilename)
	{
		if (XVP::GraphicsMgr.createTexture(pOverTexFilename,X_TT_2D,m_overTextureHandle) != X_OK)
		{
			XVP::GraphicsMgr.destroyTexture(m_inTextureHandle);
			return X_ERR_OPERATION_FAILED;
		}
	}
	
	if (pInSoundFilename)
		XVP::AudioMgr.createSoundFromFile(pInSoundFilename,m_inSoundHandle);
	if (pOutSoundFilename)
		XVP::AudioMgr.createSoundFromFile(pOutSoundFilename,m_outSoundHandle);
	if (pOverSoundFilename)
		XVP::AudioMgr.createSoundFromFile(pOverSoundFilename,m_overSoundHandle);

	m_zOrderOffset = zOrder;
	m_pParentUI = pParentUI;
	XUIElement::setOwner(pParentUI,this,zOrder);
*/
}

	
	
//*****************************************************************************
/**
Updates the user interface button.  This function is called by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface button.
@param yScale	The vertical scale factor relative to the original
				height of the user interface button.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIButton::update(XF32 dt)
{
	//Update the sprite.
	m_sprite.update(dt);

	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface button.  This function is called
automatically by the engine once per frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIButton::render()
{
	if (!m_visible)
		return X_OK;

	m_sprite.render();
	return X_OK;
}



//*****************************************************************************
/**
Destroys the user interface button and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIButton::destroy()
{
	if (m_inTextureHandle != X_INVALID_HANDLE)
	{
		XVP::GraphicsMgr.destroyTexture(m_inTextureHandle);
		m_inTextureHandle = X_INVALID_HANDLE;
	}
	if (m_overTextureHandle != X_INVALID_HANDLE)
	{
		XVP::GraphicsMgr.destroyTexture(m_overTextureHandle);
		m_overTextureHandle = X_INVALID_HANDLE;
	}
	if (m_inSoundHandle != X_INVALID_HANDLE)
	{
		XVP::AudioMgr.destroySound(m_inSoundHandle);
		m_inSoundHandle = X_INVALID_HANDLE;
	}
	if (m_outSoundHandle != X_INVALID_HANDLE)
	{
		XVP::AudioMgr.destroySound(m_outSoundHandle);
		m_outSoundHandle = X_INVALID_HANDLE;
	}
	if (m_overSoundHandle != X_INVALID_HANDLE)
	{
		XVP::AudioMgr.destroySound(m_overSoundHandle);
		m_overSoundHandle = X_INVALID_HANDLE;
	}

	//We don't want the sprite to destroy it's currently set texture.  We want to 
	//manage it manually so set an invalid handle before calling destroy.
	//m_sprite.setTextureHandle(X_INVALID_HANDLE);
	m_sprite.destroy();

	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}

						   
						   
//*****************************************************************************
/**
Determines if the user interface button is pressed or unpressed.

@return True if the buttons is depressed, false otherwise.
*/
//*****************************************************************************
XBOOL XUIButton::isPressed()
{
	if (m_buttonState == X_UIBS_IN)
		return true;
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIButton::enableRepeatMode(XBOOL enable)
{
	m_repeatEnabled = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIButton::isRepeatModeEnabled()
{
	return m_repeatEnabled;	
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIButton::setOpacity(XF32 opacity)
{
	m_sprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIButton::move(XVector2D& moveVector)
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



//*****************************************************************************
//*****************************************************************************
XStatus XUIButton::onResize(XF32 xScale,XF32 yScale)
{
	//Adjust the size of the button if necessary.
	m_scaledScreenRect.left = (int)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (int)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (int)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (int)(m_nativeScreenRect.bottom * yScale);
	m_sprite.setDestRect(m_scaledScreenRect);

	return X_OK;
}



XStatus XUIButton::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	//Only process the event if the button is NOT in repeat mode.
	if (m_repeatEnabled)
		return X_OK;

	//Save the previous button states
	m_prevButtonState = m_buttonState;

	//Set new state
	m_buttonState = X_UIBS_IN;

	if (m_prevButtonState != X_UIBS_IN)
		onButtonDown();

	XUIElement::onMouseDown(hotSpot,button,pChildElement);

	return X_OK;
}


XStatus XUIButton::onMouseDownRepeat(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	//Only process the event if the button IS in repeat mode.
	if (!m_repeatEnabled)
		return X_OK;

	//Save the previous button states
	m_prevButtonState = m_buttonState;

	//Set new state
	m_buttonState = X_UIBS_IN;

	onButtonDown();

	XUIElement::onMouseDownRepeat(hotSpot,button,pChildElement);
	return X_OK;
}



XStatus XUIButton::onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	//Save the previous button states
	m_prevButtonState = m_buttonState;

	//Set new state
	m_buttonState = X_UIBS_OUT;
	
	if (m_prevButtonState != X_UIBS_OUT)
		onButtonUp();

	XUIElement::onMouseUp(hotSpot,button,pChildElement);
	return X_OK;
}


XStatus XUIButton::onMouseEnter(XUIElement* pChildElement)
{
	//Save the previous button states
	m_prevButtonState = m_buttonState;


	if (m_buttonState != X_UIBS_IN)
				if (m_overTextureHandle != X_INVALID_HANDLE)
				{
					m_sprite.setTextureHandle(m_overTextureHandle);
					m_sprite.setSrcRect(m_overSrcRect);
				}
			if (m_overSoundHandle != X_INVALID_HANDLE)
				XVP::AudioMgr.playSound(m_overSoundHandle);

			XUIElement::onMouseEnter(pChildElement);

	return X_OK;
}


XStatus XUIButton::onMouseLeave(XUIElement* pChildElement)
{
	m_preventSound = false;
	//Save the previous button states
	m_prevButtonState = m_buttonState;

	if (m_outTextureHandle != X_INVALID_HANDLE)
	{
		m_sprite.setTextureHandle(m_outTextureHandle);
		m_sprite.setSrcRect(m_outSrcRect);
	}
			m_buttonState = X_UIBS_OUT;

			XUIElement::onMouseLeave(pChildElement);
	return X_OK;
}


XStatus XUIButton::onButtonDown()
{
	if (m_inTextureHandle != X_INVALID_HANDLE)
	{
		m_sprite.setTextureHandle(m_inTextureHandle);
		m_sprite.setSrcRect(m_inSrcRect);
	}
	if (m_inSoundHandle != X_INVALID_HANDLE && !m_preventSound)
	{
		XVP::AudioMgr.playSound(m_inSoundHandle);
		m_preventSound = true;
	}
	
	//Send Lua Event
	m_pParentUI->sendLuaEvent(this,X_UILE_CHANGE);

	return X_OK;
}

XStatus XUIButton::onButtonUp()
{
	m_preventSound = false;
	if (m_overTextureHandle != X_INVALID_HANDLE)
	{
		m_sprite.setTextureHandle(m_overTextureHandle);
		m_sprite.setSrcRect(m_overSrcRect);
	}
	if (m_outSoundHandle != X_INVALID_HANDLE)
		XVP::AudioMgr.playSound(m_outSoundHandle);

	//Send Lua Event
	m_pParentUI->sendLuaEvent(this,X_UILE_CHANGE);

	return X_OK;
}

