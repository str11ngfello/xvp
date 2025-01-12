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
//	Module Name:	XUIToggle.cpp
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
Constructor for the user interface toggle.
*/
//*****************************************************************************
XUIToggle::XUIToggle()
{
}



//*****************************************************************************
/**
Destructor for the user interface toggle.
*/
//*****************************************************************************
XUIToggle::~XUIToggle()
{
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIToggle::setState(XBOOL isPressed)
{
	//Save the previous button states
	m_prevButtonState = m_buttonState;

	if (isPressed)
	{
		m_buttonState = X_UIBS_IN;
		if (m_inTextureHandle != X_INVALID_HANDLE)
		{
			m_sprite.setTextureHandle(m_inTextureHandle);
			m_sprite.setSrcRect(m_inSrcRect);
		}
	}
	else
	{
		m_buttonState = X_UIBS_OUT;
		if (m_overTextureHandle != X_INVALID_HANDLE)
		{
			m_sprite.setTextureHandle(m_overTextureHandle);
			m_sprite.setSrcRect(m_overSrcRect);
		}
	}
	return X_OK;
}



XStatus XUIToggle::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	//Save the previous button states
	m_prevButtonState = m_buttonState;

	if (m_buttonState == X_UIBS_OUT)
	{
		m_buttonState = X_UIBS_IN;
		onButtonDown();
	}
	else
	{
		m_buttonState = X_UIBS_OUT;
		onButtonUp();
	}


	XUIElement::onMouseDown(hotSpot,button,pChildElement);

	return X_OK;
}


XStatus XUIToggle::onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	//Do nothing but call base class event handler
	XUIElement::onMouseUp(hotSpot,button,pChildElement);
	return X_OK;
}

XStatus XUIToggle::onMouseEnter(XUIElement* pChildElement)
{
	//Save the previous button states
	m_prevButtonState = m_buttonState;

			if ((m_buttonState != X_UIBS_IN) && (m_overTextureHandle != X_INVALID_HANDLE))
			{
				m_sprite.setTextureHandle(m_overTextureHandle);
				m_sprite.setSrcRect(m_overSrcRect);
			}
			if ((m_buttonState != X_UIBS_IN) && (m_overSoundHandle != X_INVALID_HANDLE))
				XVP::AudioMgr.playSound(m_overSoundHandle);

			
			XUIElement::onMouseEnter(pChildElement);
	

	return X_OK;
}


XStatus XUIToggle::onMouseLeave(XUIElement* pChildElement)
{
	//Save the previous button states
	m_prevButtonState = m_buttonState;

	if ((m_buttonState != X_UIBS_IN) && (m_outTextureHandle != X_INVALID_HANDLE))
	{
		m_sprite.setTextureHandle(m_outTextureHandle);
		m_sprite.setSrcRect(m_outSrcRect);
	}

	XUIElement::onMouseLeave(pChildElement);

	return X_OK;
}

