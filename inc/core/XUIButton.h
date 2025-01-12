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
//	Module Name:	XUIButton.h
//	Author:			Lynn Duke
//	Creation Date:	6-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_BUTTON_H__
#define __X_UI_BUTTON_H__





//*****************************************************************************
//! Push button control.
/**
The XUIButton object represents a standard 2 state button.  The button can
either be depressed or not.  

See the member functions below for more information.
*/
//*****************************************************************************
class XUIButton : public XUIElement
{
protected:

	//*****************************************************************************
	//Button States
	//*****************************************************************************
	enum 
	{
		X_UIBS_OUT,
		X_UIBS_IN
	};

	XHandle			m_inTextureHandle;
	XHandle			m_outTextureHandle;
	XHandle			m_overTextureHandle;
	XHandle			m_inSoundHandle;
	XHandle			m_outSoundHandle;
	XHandle			m_overSoundHandle;
	XU8				m_buttonState;
	XU8				m_prevButtonState;
	XSprite			m_sprite;
	XBOOL			m_repeatEnabled;
	XBOOL			m_preventSound;
	XRect			m_outSrcRect;
	XRect			m_inSrcRect;
	XRect			m_overSrcRect;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus handleOverState();

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIButton();
	virtual ~XUIButton();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,XS8* pTemplateFile);
	XStatus update(XF32 dt);
	XStatus render();
	virtual XStatus destroy();
	virtual XBOOL isPressed();
	XStatus enableRepeatMode(XBOOL enable);
	XBOOL isRepeatModeEnabled();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus onResize(XF32 xScale,XF32 yScale);
	virtual XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseDownRepeat(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseEnter(XUIElement* pChildElement);
	virtual XStatus onMouseLeave(XUIElement* pChildElement);
	virtual XStatus onButtonDown();
	virtual XStatus onButtonUp();
};

#endif