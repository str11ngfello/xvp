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
//	Module Name:	XUISpinner.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-12-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


#define	X_MAX_SPINNER_CHARS  64


//*****************************************************************************
/**
Constructor for the user interface spinner.
*/
//*****************************************************************************
XUISpinner::XUISpinner()
{
	m_stepSize = 0;
	m_maxValue = 0;
	m_minValue = 0;
	m_precision = 0;
	m_dirty = true;
	m_pFormatString = NULL;
}



//*****************************************************************************
/**
Destructor for the user interface spinner.
*/
//*****************************************************************************
XUISpinner::~XUISpinner()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::init(XUserInterface* pParentUI,XU32 zOrder,XRect& upButtonScreenRect,
						 XRect& downButtonScreenRect,XRect& textBoxScreenRect,
						XS8* pTemplateFile)
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
		TiXmlElement* pSpinnerElement = NULL;
		
		pNode = file.FirstChild("spinner");
		if (pNode)
		{
			pSpinnerElement = pNode->ToElement();
			if (pSpinnerElement)
			{	
				XF64 doubleValue = 0;
				XS8* pFilename = NULL;
				XS32 intValue = 0;
				pFilename = (XS8*)pSpinnerElement->Attribute("UpButtonTemplateFile");
				if (pFilename)
				{
					if (m_upButton.init(pParentUI,zOrder,upButtonScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_upButton.setParentScrollbar(this,0);
					m_upButton.enableRepeatMode(true);
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFilename = (XS8*)pSpinnerElement->Attribute("DownButtonTemplateFile");
				if (pFilename)
				{
					if (m_downButton.init(pParentUI,zOrder,downButtonScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_downButton.setParentScrollbar(this,1);
					m_downButton.enableRepeatMode(true);
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFilename = (XS8*)pSpinnerElement->Attribute("TextboxTemplateFile");
				if (pFilename)
				{
					if (m_textBox.init(pParentUI,zOrder+1,textBoxScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
				}
				else
					return X_ERR_OPERATION_FAILED;

				pSpinnerElement->QueryDoubleAttribute("StepSize",&doubleValue);
				m_stepSize = (XF32)doubleValue;
				pSpinnerElement->QueryDoubleAttribute("MaxValue",&doubleValue);
				m_maxValue = (XF32)doubleValue;
				pSpinnerElement->QueryDoubleAttribute("MinValue",&doubleValue);
				m_minValue = (XF32)doubleValue;
				pSpinnerElement->QueryDoubleAttribute("StartValue",&doubleValue);
				m_value = (XF32)doubleValue;

				//Get the precision...max out at 9
				pSpinnerElement->QueryIntAttribute("Precision",&intValue);
				m_precision = (XS8)intValue;
				if ((intValue > 9) || (intValue < 0))
					intValue = 9;
				m_pFormatString = XOSMgr::createString("%.Xf");
				char buf[5];
				_itoa(intValue,buf,10);
				m_pFormatString[2] = buf[0];
			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;

		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);
		m_nativeScreenRect = XRect(-100000,-100000,-100000,-100000);
		m_scaledScreenRect = XRect(-100000,-100000,-100000,-100000);
		
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::destroy()
{
	m_textBox.destroy();
	m_upButton.destroy();
	m_downButton.destroy();
	XUIElement::destroy();
	XOSMgr::destroyString(&m_pFormatString);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::update(XF32 dt)
{
	if (m_dirty)
	{
		char buf[X_MAX_SPINNER_CHARS];
		bool skipSign = false;
		float compareValue = 0.0f;
		sprintf(buf,m_pFormatString,m_value);
		
		//Get rid of a negative sign when 0
		switch (m_precision)
		{
		case 0:
			break;
		case 1:
			compareValue = .09f;
			break;
		case 2:
			compareValue = .009f;
			break;
		case 3:
			compareValue = .0009f;
			break;
		case 4:
			compareValue = .00009f;
			break;
		case 5:
			compareValue = .000009f;
			break;
		case 6:
			compareValue = .0000009f;
			break;
		case 7:
			compareValue = .00000009f;
			break;
		case 8:
			compareValue = .000000009f;
			break;
		case 9:
			compareValue = .0000000009f;
			break;
		default:
			break;
		}	
		
		m_dirty = false;
		
		//Skip 0
		if ((compareValue != 0.0f) && (m_value < compareValue) && (m_value > -compareValue) && buf[0] == '-')
			return m_textBox.setText(buf+1);
		else
			return m_textBox.setText(buf);
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::render()
{
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::incrementStep()
{
	m_value += m_stepSize;
	if (m_value > m_maxValue)
		m_value = m_maxValue;
	m_dirty = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::decrementStep()
{
	m_value -= m_stepSize;
	if (m_value < m_minValue)
		m_value = m_minValue;
	m_dirty = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::setStepSize(XF32 stepSize)
{
	if (stepSize < 0)
		return X_ERR_INVALID_PARAMS;
	else
	{
		m_stepSize = stepSize;
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
XF32 XUISpinner::getStepSize()
{
	return m_stepSize;
}


//*****************************************************************************
//*****************************************************************************
XF32 XUISpinner::getValue()
{
	return m_value;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::setValue(XF32 value)
{
	m_value = value;
	m_dirty = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::setZOrderOffset(XU32 zOrder)
{
	m_upButton.setZOrderOffset(zOrder);
	m_downButton.setZOrderOffset(zOrder);
	m_textBox.setZOrderOffset(zOrder);
	XUIElement::setZOrderOffset(zOrder);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::show()
{
	m_upButton.show();
	m_downButton.show();
	m_textBox.show();
	XUIElement::show();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::hide()
{
	m_upButton.hide();
	m_downButton.hide();
	m_textBox.hide();
	XUIElement::hide();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::enable()
{
	m_upButton.enable();
	m_downButton.enable();
	m_textBox.enable();
	XUIElement::enable();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::disable()
{
	m_upButton.disable();
	m_downButton.disable();
	m_textBox.disable();
	XUIElement::disable();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::setOpacity(XF32 opacity)
{
	m_upButton.setOpacity(opacity);
	m_downButton.setOpacity(opacity);
	m_textBox.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::move(XVector2D& moveVector)
{
	XRect r;
	m_upButton.getScaledRect(r);
	XVector2D v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_upButton.setScaledPosition(v);

	m_downButton.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_downButton.setScaledPosition(v);

	m_textBox.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_textBox.setScaledPosition(v);

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::setNativePosition(XVector2D& position)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUISpinner::isPointInside(XVector2D& point)
{
	return (m_upButton.isPointInside(point) ||
			m_downButton.isPointInside(point) ||
			m_textBox.isPointInside(point));
}



//*****************************************************************************
//*****************************************************************************
XStatus XUISpinner::setToolTipText(XS8* pToolTipText)
{
	m_upButton.setToolTipText(pToolTipText);
	m_downButton.setToolTipText(pToolTipText);
	m_textBox.setToolTipText(pToolTipText);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUISpinner::getToolTipText()
{
	return m_upButton.getToolTipText();
}