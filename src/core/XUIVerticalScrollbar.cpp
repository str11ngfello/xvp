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
//	Module Name:	XUIVerticalScrollbar.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-21-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XUIVerticalScrollbar::XUIVerticalScrollbar()
{
	m_maxSteps = 1;
	m_currentStep = 0;
	m_prevStep = 0;
}



//*****************************************************************************
//*****************************************************************************
XUIVerticalScrollbar::~XUIVerticalScrollbar()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::init(XUserInterface* pParentUI,XU32 zOrder,XRect& sliderScreenRect,
								   			  
						  XRect& backButtonScreenRect,
						  
						  XRect& forwardButtonScreenRect,XS8* pTemplateFile)
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
		TiXmlElement* pScrollbarElement = NULL;
		
		pNode = file.FirstChild( "scrollbar" );
		if (pNode)
		{
			pScrollbarElement = pNode->ToElement();
			if (pScrollbarElement)
			{	
				XS8* pFilename = NULL;
				pFilename = (XS8*)pScrollbarElement->Attribute("BackScrollButtonTemplateFile");
				if (pFilename)
				{
					if (m_scrollBackButton.init(pParentUI,zOrder,backButtonScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_scrollBackButton.setParentElement(this);
					m_scrollBackButton.enableRepeatMode(true);
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFilename = (XS8*)pScrollbarElement->Attribute("ForwardScrollButtonTemplateFile");
				if (pFilename)
				{
					if (m_scrollForwardButton.init(pParentUI,zOrder,forwardButtonScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_scrollForwardButton.setParentElement(this);
					m_scrollForwardButton.enableRepeatMode(true);
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFilename = (XS8*)pScrollbarElement->Attribute("SliderTemplateFile");
				if (pFilename)
				{
					if (m_slider.init(pParentUI,zOrder+1,sliderScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_slider.setParentElement(this);
				}
				else
					return X_ERR_OPERATION_FAILED;
			
				

			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;

		m_slider.setPercentage(1 - ((float)m_currentStep/(float)m_maxSteps));
		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);
		m_nativeScreenRect = XRect(-100000,-100000,-100000,-100000);
		m_scaledScreenRect = XRect(-100000,-100000,-100000,-100000);

		return X_OK;
	}

	/*
	//Initialize sub controls. Place the scroll bar on top of the buttons by increasing
	//the Z order by 1.
	m_slider.init(pParentUI,zOrder+1,scrollBarScreenRect,pSliderPropertyFile);
	m_scrollBackButton.init(pParentUI,zOrder,backScreenRect,pBackButtonPropertyFile);
	m_scrollBackButton.setParentScrollbar(this,0);
	m_scrollForwardButton.init(pParentUI,zOrder,forwardScreenRect,pForwardButtonPropertyFile);
	m_scrollForwardButton.setParentScrollbar(this,1);

	//Start the scrollbar at the top.
	m_slider.setPercentage(1 - ((float)m_currentStep/(float)m_maxSteps));

	//m_slider->registerEventCallback(verticalScrollBarCB);
	//m_slider->setUserData(this);

	m_zOrderOffset = zOrder;
	m_pParentUI = pParentUI;
	XUIElement::setOwner(pParentUI,this,zOrder);
	m_nativeScreenRect = XRect(-100000,-100000,-100000,-100000);
	m_scaledScreenRect = XRect(-100000,-100000,-100000,-100000);
*/
		
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::destroy()
{
	m_slider.destroy();
	m_scrollBackButton.destroy();
	m_scrollForwardButton.destroy();
	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::update(XF32 dt)
{
	//Only if the scrollbar is sliding do we want to modify the current step.
	if (m_slider.isSliding())
	{
		m_currentStep = (XU32)((1 - m_slider.getPercentage()) * (float)m_maxSteps);
		m_scrollBackButton.disable();
		m_scrollForwardButton.disable();
			
		
	}
	else
	{
		m_scrollBackButton.enable();
		m_scrollForwardButton.enable();
	}

	//If the step changed notify the appropriate event.
	if (m_currentStep != m_prevStep)
	{
		m_prevStep = m_currentStep;
           onChange();
	}
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::render()
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
/*XStatus XUIVerticalScrollbar::eventNotify(X_UI_EVENT_TYPE eventType,void* pEventData)
{
	//Call the base object's eventNotify to handle custom user callbacks.
	XUIElement::eventNotify(eventType,pEventData);

	return X_OK;
}
*/


//*****************************************************************************
//*****************************************************************************
XU32 XUIVerticalScrollbar::getStep()
{

	return m_currentStep;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::setStep(XU32 step)
{
	if (step <= m_maxSteps)
	{
		m_currentStep = step;
		m_slider.setPercentage(1 - ((float)step/(float)m_maxSteps));
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::setMaxSteps(XU32 maxSteps)
{
	if (maxSteps != 0)
	{
		m_maxSteps = maxSteps;
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIVerticalScrollbar::getMaxSteps()
{
	return m_maxSteps;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::incrementStep()
{
	++m_currentStep;
	if (m_currentStep > m_maxSteps)
	{
		m_currentStep = m_maxSteps;
		return X_ERR_LIMIT_REACHED;
	}
	else
	{
		m_slider.setPercentage(1 - ((float)m_currentStep/(float)m_maxSteps));
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::decrementStep()
{
	if (m_currentStep != 0)
	{
		--m_currentStep;
		m_slider.setPercentage(1 - ((float)m_currentStep/(float)m_maxSteps));
		return X_OK;
	}
	else
		return X_ERR_LIMIT_REACHED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::setZOrderOffset(XU32 zOrder)
{
	m_scrollBackButton.setZOrderOffset(zOrder);
	m_scrollForwardButton.setZOrderOffset(zOrder);
	m_slider.setZOrderOffset(zOrder);
	XUIElement::setZOrderOffset(zOrder);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::show()
{
	//Show all sub controls
	m_scrollBackButton.show();
	m_scrollForwardButton.show();
	m_slider.show();
	XUIElement::show();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::hide()
{
	m_scrollBackButton.hide();
	m_scrollForwardButton.hide();
	m_slider.hide();
	XUIElement::hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::enable()
{
	m_scrollBackButton.enable();
	m_scrollForwardButton.enable();
	m_slider.enable();
	XUIElement::enable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::disable()
{
	m_scrollBackButton.disable();
	m_scrollForwardButton.disable();
	m_slider.disable();
	XUIElement::disable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::setOpacity(XF32 opacity)
{
	m_scrollBackButton.setOpacity(opacity);
	m_scrollForwardButton.setOpacity(opacity);
	m_slider.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::move(XVector2D& moveVector)
{
	XRect r;
	m_scrollBackButton.getScaledRect(r);
	XVector2D v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_scrollBackButton.setScaledPosition(v);

	m_scrollForwardButton.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_scrollForwardButton.setScaledPosition(v);

	m_slider.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_slider.setScaledPosition(v);

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIVerticalScrollbar::isPointInside(XVector2D& point)
{
	return (m_scrollBackButton.isPointInside(point) ||
			m_scrollForwardButton.isPointInside(point) ||
			m_slider.isPointInside(point));
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::onMouseDownRepeat(XVector2D& hotSpot,X_MOUSE_BUTTON button,
										  XUIElement* pChildElement)
{
	//Depending on which button was pressed, increment or decrement the step.
	if (pChildElement == &m_scrollBackButton && !m_slider.isSliding())
		decrementStep();
	else if (pChildElement == &m_scrollForwardButton && !m_slider.isSliding())
		incrementStep();

	XUIElement::onMouseDownRepeat(hotSpot,button,pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::setNativePosition(XVector2D& position)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIVerticalScrollbar::setToolTipText(XS8* pToolTipText)
{
	m_scrollForwardButton.setToolTipText(pToolTipText);
	m_scrollBackButton.setToolTipText(pToolTipText);
	m_slider.setToolTipText(pToolTipText);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIVerticalScrollbar::getToolTipText()
{
	return m_scrollForwardButton.getToolTipText();
}

