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
//	Module Name:	XUIFrame.h
//	Author:			Lynn Duke
//	Creation Date:	12-10-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_FRAME_H__
#define __X_UI_FRAME_H__




//*****************************************************************************
//! Sprite object for use with user interfaces.
/**
The XUIFrame is a specific type of 2D sprite that is compatible with the
XUserInterface object.  The XUIFrame object is very similar to the XSprite
object feature-wise, but is specifically suited for user interfaces.   

See the member functions below for more information.
*/
//*****************************************************************************
class XUIFrame : public XUIElement
{
protected:
	XSprite				m_sprite;
	XBOOL				m_dragging;
	XRect				m_nativeMoveArea;
	XRect				m_scaledMoveArea;
	XMemBlockList<XUIElement*>	m_elementList;

	XStatus changeZOrder(XU32 zOrder);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIFrame();
	virtual ~XUIFrame();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
						XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XStatus setOpacity(XF32 opacity);
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus onResize(XF32 xScale,XF32 yScale);
	XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement);
	XStatus onDrag(XVector2D& drag);
	XSprite* getSprite();
	XStatus addElement(XUIElement* pElement);
	friend XUserInterface;
};


#endif
