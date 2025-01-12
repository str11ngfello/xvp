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
//	Module Name:	XUISpinner.h
//	Author:			Lynn Duke
//	Creation Date:	6-14-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_SPINNER_H__
#define __X_UI_SPINNER_H__




//*****************************************************************************
//! 
/**

*/
//*****************************************************************************
class XUISpinner : public XUIElement
{
private:
	
	class XUISpinnerButton : public XUIButton
	{
	private:
		XU8					m_opType;
		XUISpinner*				m_pParentSpinner;

	public:
		XUISpinnerButton()
		{
			m_pParentSpinner = NULL;
		}

		XStatus setParentScrollbar(XUISpinner* pParentScrollbar,XU8 opType)
		{
			m_pParentSpinner = pParentScrollbar;
			m_opType = opType;
			return X_OK;
		}

		XStatus onButtonDown()
		{
			if (m_pParentSpinner)
			{
				if (m_opType == 0)
					m_pParentSpinner->incrementStep();
				else
					m_pParentSpinner->decrementStep();
			}
			XUIButton::onButtonDown();
			return X_OK;
		}
	};


	XUISpinnerButton			m_upButton;
	XUISpinnerButton			m_downButton;
	XUITextBox					m_textBox;
	XF32						m_stepSize;
	XF32						m_value;
	XF32						m_maxValue;
	XF32						m_minValue;
	XBOOL						m_dirty;
	XS8*						m_pFormatString;
	XS8						m_precision;


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUISpinner();
	virtual ~XUISpinner();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& upButtonScreenRect,
						 XRect& downButtonScreenRect,XRect& textBoxScreenRect,
						XS8* pTemplateFile);
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus render();
	XF32 getValue();
	XStatus setValue(XF32 value);
	XStatus incrementStep();
	XStatus decrementStep();
	XStatus setStepSize(XF32 stepSize);
	XF32 getStepSize();
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus show();
	XStatus hide();
	XStatus enable();
	XStatus disable();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XBOOL isPointInside(XVector2D& point);
	XStatus setToolTipText(XS8* pToolTipText);
	XS8* getToolTipText();
	XStatus setNativePosition(XVector2D& position);
};

#endif