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
//	Module Name:	XUIElement.cpp
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
Constructor for the user interface element.
*/
//*****************************************************************************
XUIElement::XUIElement()
{
	m_pParentUI = NULL;
	m_pParentFrame = NULL;
	m_zOrderOffset = 0;
	m_visible = true;
	m_enabled = true;
	m_focus = false;
	m_pToolTipText = NULL;
	m_pointerState = X_PS_NORMAL;
	m_pParentElement = NULL;
	m_opacity = 1.0f;
}



//*****************************************************************************
/**
Destructor for the user interface element.
*/
//*****************************************************************************
XUIElement::~XUIElement()
{
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XStatus XUIElement::destroy()
{
	XUIElement::removeOwner(m_pParentUI,this);
	XOSMgr::destroyString(&m_pToolTipText);
	return X_OK;
}



//*****************************************************************************
/**
Updates the user interface element.  This function is called automatically by the
engine once per frame.  You should override this function to provide custom
update functionality for derived elements.

@param dt	The amount of time, in seconds, that has elapsed since
			the last update.
@param xScale		The horizontal scale factor relative to the original
			width of the user interface element.  For example, a value of
			1.5 means one and a half times the original width.
@param yScale		The vertical scale factor relative to the original
			height of the user interface element.  For example, a value of
			.5 means one half times the original height.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::update(XF32 dt)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setOwner(XUserInterface* pParentUI,XUIElement* pElement,XU32 zOrder)
{
	pParentUI->addElement(pElement,zOrder);
	pElement->m_zOrderOffset = zOrder;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::removeOwner(XUserInterface* pParentUI,XUIElement* pElement)
{
	if (pParentUI)
		pParentUI->removeElement(pElement);	
	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface element.  This function is called
automatically by the engine once per frame.  You should override this function
to provide rendering code for derived elements.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::render()
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setNativePosition(XVector2D& position)
{
	XU32 width = m_nativeScreenRect.right - m_nativeScreenRect.left;
	XU32 height = m_nativeScreenRect.bottom - m_nativeScreenRect.top;
	m_nativeScreenRect.set((XS32)position.x,(XS32)position.y,(XS32)position.x + width,
							(XS32)position.y + height);

	//Need to send a resize event since we've modified the native rectangle
	return onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setScaledPosition(XVector2D& position)
{
	//Since we're going from scaled to native coordinate systems, we need to
	//take the ceiling of the coordinates in order to regain some lost floating
	//point precision.
	return setNativePosition(XVector2D(ceil(position.x / m_pParentUI->getXScale()),
							ceil(position.y / m_pParentUI->getYScale())));
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setParentElement(XUIElement* pParentElement)
{
	m_pParentElement = pParentElement;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XUIElement* XUIElement::getParentElement()
{
	return m_pParentElement;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setParentFrame(XUIFrame* pParentFrame)
{
	m_pParentFrame = pParentFrame;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XUIFrame* XUIElement::getParentFrame()
{
	return m_pParentFrame;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setToolTipText(XS8* pToolTipText)
{
	if (!pToolTipText)
	{
		XOSMgr::destroyString(&m_pToolTipText);
	}
	else
	{
		if (m_pToolTipText)
			XOSMgr::destroyString(&m_pToolTipText);
		m_pToolTipText = XOSMgr::createString(pToolTipText);
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIElement::getToolTipText()
{
	return m_pToolTipText;
}



//*****************************************************************************
/**
Shows the user interface element by making it visible.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::show()
{
	m_visible = true;
	return X_OK;
}



//*****************************************************************************
/**
Hides the user interface element by making it invisible.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::hide()
{
	m_visible = false;
	return X_OK;
}



//*****************************************************************************
/**
Determines if the user interface element is visible.

@return True if the element is visible, false otherwise.
*/
//*****************************************************************************
XBOOL XUIElement::isVisible()
{
	return m_visible;
}



//*****************************************************************************
/**
Determines if the user interface element has the focus.

@return True if the element has the focus, false otherwise.
*/
//*****************************************************************************
XBOOL XUIElement::hasFocus()
{
	return m_focus;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::move(XVector2D& moveVector)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIElement::isPointInside(XVector2D& point)
{
	return m_scaledScreenRect.isPointInside(point);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setPointerState(X_POINTER_STATE pointerState)
{
	m_pointerState = pointerState;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_POINTER_STATE XUIElement::getPointerState()
{
	return m_pointerState;
}

	
	
//*****************************************************************************
/**
Enables the user interface element.  While an element is enabled it can receive
and process event notifications.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::enable()
{
	m_enabled = true;
	return X_OK;
}



//*****************************************************************************
/**
Disables the user interface element.  While an element is disabled it cannot
receive or process event notifications.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::disable()
{
	m_enabled = false;
	return X_OK;
}



//*****************************************************************************
/**
Determines if the user interface element is enabled or disabled.

@return True if element is enabled, false otherwise.
*/
//*****************************************************************************
XBOOL XUIElement::isEnabled()
{
	if (!m_visible)
		return false;
	else
		return m_enabled;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setOpacity(XF32 opacity)
{
	m_opacity = opacity;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XUIElement::getOpacity()
{
	return m_opacity;
}



//*****************************************************************************
/**
Returns the native rectangle of the user interface element.  The native
retangle is the original, unscaled rectangle of the element.

@param rect	A variable in which the native rectangle will be stored.
@return None.
*/
//*****************************************************************************
XStatus XUIElement::getNativeRect(XRect& rect)
{
	rect = m_nativeScreenRect;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setNativeRect(XRect& rect)
{
	m_nativeScreenRect = rect;
	return onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());
}



//*****************************************************************************
/**
Returns the scaled rectangle of the user interface element.  The scaled
rectangle is the native rectangle scaled by the current screen resolution.

@param rect	A variable in which the native rectangle will be stored.
@return None.
*/
//*****************************************************************************
XStatus XUIElement::getScaledRect(XRect& rect)
{
	rect = m_scaledScreenRect;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::setScaledRect(XRect& rect)
{
	//Since we're going from scaled to native coordinate systems, we need to
	//take the ceiling of the coordinates in order to regain some lost floating
	//point precision.
	XF32 xScale = m_pParentUI->getXScale();
	XF32 yScale = m_pParentUI->getYScale();
	return setNativeRect(XRect(XS32(ceil(rect.left / xScale)),
							   XS32(ceil(rect.top / yScale)),
							   XS32(ceil(rect.right / xScale)),
							   XS32(ceil(rect.bottom / yScale))));
}



//*****************************************************************************
/**
Returns the Z order of the user interface element. This value is treated as an
offset from the z order of the user interface.

@return The Z order offset.
*/
//*****************************************************************************
XU32 XUIElement::getZOrderOffset()
{
	return m_zOrderOffset;
}



//*****************************************************************************
/**
Sets the Z order of the user interface element.  This value is treated as an
offset from the z order of the user interface.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIElement::setZOrderOffset(XU32 zOrder)
{
	if (m_pParentUI)
	{
		m_pParentUI->removeElement(this);
		m_pParentUI->addElement(this,zOrder);
		m_zOrderOffset = zOrder;
	}
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onEnableChanged()
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onVisibleChanged()
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onResize(XF32 xScale,XF32 yScale)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onGotFocus()
{
	//If this element has a parent, pass the focus up to the parent. Otherwise
	//this element gets the focus
	if (m_pParentElement)
		return m_pParentElement->onGotFocus();
	
	if (m_pParentUI->m_pElementWithFocus)
	{
		if (m_pParentUI->m_pElementWithFocus != this)
			m_pParentUI->m_pElementWithFocus->onLostFocus();
	}

	m_pParentUI->m_pElementWithFocus = this;
	m_focus = true;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onLostFocus()
{
	m_focus = false;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	if (m_pParentElement)
		m_pParentElement->onMouseDown(hotSpot,button,this);
	if (m_pParentFrame)
		m_pParentFrame->onMouseDown(hotSpot,button,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseDownRepeat(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	if (m_pParentElement)
		m_pParentElement->onMouseDownRepeat(hotSpot,button,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseUp(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	if (m_pParentElement)
		m_pParentElement->onMouseUp(hotSpot,button,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement)
{
	m_pParentUI->sendLuaEvent(this,X_UILE_MOUSE_OVER);

	if (m_pParentElement)
		m_pParentElement->onMouseOver(hotSpot,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseEnter(XUIElement* pChildElement)
{
	XCursor* pCursor = XUserInterface::getCursor();
	if (pCursor)
		pCursor->setState(m_pointerState);

	m_pParentUI->sendLuaEvent(this,X_UILE_MOUSE_ENTER);

	if (m_pParentElement)
		m_pParentElement->onMouseEnter(this);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseLeave(XUIElement* pChildElement)
{
	m_pParentUI->sendLuaEvent(this,X_UILE_MOUSE_LEAVE);

	if (m_pParentElement)
		m_pParentElement->onMouseLeave(this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseMove(XUIElement* pChildElement)
{
	if (m_pParentElement)
		m_pParentElement->onMouseMove(this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseWheel(XS32 movement,XUIElement* pChildElement)
{
	if (m_pParentElement)
		m_pParentElement->onMouseWheel(movement,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onMouseDblClick(XUIElement* pChildElement)
{
	if (m_pParentElement)
		m_pParentElement->onMouseDblClick(this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onDrag(XVector2D& drag)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIElement::onChange()
{
	m_pParentUI->sendLuaEvent(this,X_UILE_CHANGE);
	return X_OK;
}