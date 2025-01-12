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
//	Module Name:	XUIHorizontalSlider.h
//	Author:			Lynn Duke
//	Creation Date:	5-21-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_HORIZONTAL_SLIDER_H__
#define __X_UI_HORIZONTAL_SLIDER_H__




//*****************************************************************************
//! Horizontal slider control representing a numerical range from 0 to 100%.
/**
The XUIHorizontalSlider object provides a graphical representation of a numerical range
similar to slider controls in Microsoft Windows.  The slider has a knob which
can be moved along the slider track by the user.   

See the member functions below for more information.
*/
//*****************************************************************************
class XUIHorizontalSlider : public XUIElement
{
protected:

	enum 
	{
		X_UISS_SLIDING,
		X_UISS_NOT_SLIDING
	};

	XSprite		m_knobSprite;
	XSprite		m_trackSprite;
	XSprite		m_fillSprite;
	XBOOL		m_fill;
	XF32		m_percentage;
	XF32		m_prevPercentage;
	XU8			m_sliderState;
	XU32		m_nativeKnobWidth;
	XU32		m_scaledKnobWidth;
	XRect		m_scaledKnobRect;
	XHandle		m_inSoundHandle;
	XHandle		m_outSoundHandle;


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIHorizontalSlider();
	virtual ~XUIHorizontalSlider();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
						XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XStatus setPercentage(XF32 percent);
	XF32 getPercentage();
	XBOOL isSliding();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus onMouseDown(XVector2D &hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus onMouseUp(XVector2D &hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus onResize(XF32 xScale,XF32 yScale);
	virtual XStatus onBeginSlide();
	virtual XStatus onEndSlide(XBOOL forced);
};

#endif
