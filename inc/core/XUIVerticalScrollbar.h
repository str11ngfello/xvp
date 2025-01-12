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
//	Module Name:	XUIVerticalScrollbar.h
//	Author:			Lynn Duke
//	Creation Date:	5-21-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_VERTICAL_SCROLLBAR_H__
#define __X_UI_VERTICAL_SCROLLBAR_H__





//*****************************************************************************
//*****************************************************************************
class XUIVerticalScrollbar : public XUIElement
{
protected:
	XUIButton				m_scrollForwardButton;
	XUIButton				m_scrollBackButton;
	XUIVerticalSlider		m_slider;
	XU32					m_maxSteps;
	XU32					m_currentStep;
	XU32					m_prevStep;

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIVerticalScrollbar();
	virtual ~XUIVerticalScrollbar();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& sliderScreenRect,
								   			  
						  XRect& backButtonScreenRect,
						  
						  XRect& forwardButtonScreenRect,XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XStatus setStep(XU32 step);
	XU32 getStep();
	XStatus setMaxSteps(XU32 maxSteps);
	XU32 getMaxSteps();
	XStatus incrementStep();
	XStatus decrementStep();
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus show();
	XStatus hide();
	XStatus enable();
	XStatus disable();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XBOOL isPointInside(XVector2D& point);
	virtual XStatus onMouseDownRepeat(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus setNativePosition(XVector2D& position);
	XStatus setToolTipText(XS8* pToolTipText);
	XS8* getToolTipText();

};

#endif
