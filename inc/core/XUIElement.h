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
//	Module Name:	XUIElement.h
//	Author:			Lynn Duke
//	Creation Date:	6-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UIELEMENT_H__
#define __X_UIELEMENT_H__

class XUIFrame;

#define UNUSED_RECT		-10000


//*****************************************************************************
//! Base object for user interface elements such as buttons, text boxes, etc.
/**
The XUIElement object is the basic building block for all user interface
elements that belong to a XUserInterface.  It provides functionality
that is common to all elements of a user interface such as z order, screen
position and size, etc.

Using this object, you can derive new types of user interface elements that
are compatible with the XUserInterface object.  

See the member functions below for more information.
*/
//*****************************************************************************
class XUIElement
{
protected:
	XUserInterface*				m_pParentUI;
	XUIFrame*					m_pParentFrame;
	XBOOL						m_focus;
	XRect						m_nativeScreenRect;
	XRect						m_scaledScreenRect;
	XBOOL						m_visible;
	XBOOL						m_enabled;
	XU32						m_zOrderOffset;
	XS8*						m_pToolTipText;
	X_POINTER_STATE				m_pointerState;
	XUIElement*					m_pParentElement;
	XF32						m_opacity;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIElement();
	virtual ~XUIElement();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	static XStatus setOwner(XUserInterface* pParentUI,XUIElement* pElement,XU32 zOrder);
	static XStatus removeOwner(XUserInterface* pParentUI,XUIElement* pElement);
	virtual XStatus setNativeRect(XRect& rect);
	virtual XStatus getNativeRect(XRect& rect);
	virtual XStatus setScaledRect(XRect& rect);
	virtual XStatus getScaledRect(XRect& rect);
	virtual XStatus setNativePosition(XVector2D& position);
	virtual XStatus setScaledPosition(XVector2D& position);
	XStatus setParentElement(XUIElement* pParentElement);
	XUIElement* getParentElement();
	XStatus setParentFrame(XUIFrame* pParentFrame);
	XUIFrame* getParentFrame();
	virtual XStatus setZOrderOffset(XU32 zOrder);
	virtual XU32 getZOrderOffset();
	virtual XStatus setToolTipText(XS8* pToolTipText);
	virtual XS8* getToolTipText();
	virtual XStatus enable();
	virtual XStatus disable();
	virtual XBOOL isEnabled();
	virtual XStatus setOpacity(XF32 opacity);
	virtual XF32 getOpacity();
	virtual XStatus show();
	virtual XStatus hide();
	virtual XBOOL isVisible();
	virtual XBOOL hasFocus();
	virtual XStatus move(XVector2D& moveVector);
	virtual XBOOL isPointInside(XVector2D& point);
	virtual XStatus setPointerState(X_POINTER_STATE pointerState);
	virtual X_POINTER_STATE getPointerState();
	virtual XStatus onEnableChanged();
	virtual XStatus onVisibleChanged();
	virtual XStatus onResize(XF32 xScale,XF32 yScale);
	virtual XStatus onGotFocus();
	virtual XStatus onLostFocus();
	virtual XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseDownRepeat(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement);
	virtual XStatus onMouseEnter(XUIElement* pChildElement);
	virtual XStatus onMouseLeave(XUIElement* pChildElement);
	virtual XStatus onMouseMove(XUIElement* pChildElement);
	virtual XStatus onMouseWheel(XS32 movement,XUIElement* pChildElement);
	virtual XStatus onMouseDblClick(XUIElement* pChildElement);
	virtual XStatus onDrag(XVector2D& drag);
	virtual XStatus onChange();
};

#endif