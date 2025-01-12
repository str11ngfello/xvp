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
//	Module Name:	XUIComboBox.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-25-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XUIComboBox::XUIComboBox()
{
	m_prevSelectedIndex = -1;
	m_sorted = false;
	m_ascending = true;
}



//*****************************************************************************
//*****************************************************************************
XUIComboBox::~XUIComboBox()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::init(XUserInterface* pParentUI,XU32 zOrder,XRect& selectionBoxScreenRect,

						 XRect& comboButtonScreenRect,
		
						XRect& listBoxScreenRect,
						  XRect& scrollBarScreenRect,
						  
						  XRect& backButtonScreenRect,
						  
						  XRect& forwardButtonScreenRect,XS8* pTemplateFile)
{
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
		TiXmlElement* pComboBoxElement = NULL;
		
		pNode = file.FirstChild( "combobox" );
		if (pNode)
		{
			pComboBoxElement = pNode->ToElement();
			if (pComboBoxElement)
			{	
				XS8* pFilename = NULL;
				pFilename = (XS8*)pComboBoxElement->Attribute("SelectionBoxTemplateFile");
				if (pFilename)
				{
					if (m_textBox.init(pParentUI,zOrder,selectionBoxScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_textBox.setParentElement(this);
				}
				else
					return X_ERR_OPERATION_FAILED;
				pFilename = (XS8*)pComboBoxElement->Attribute("ComboButtonTemplateFile");
				if (pFilename)
				{
					if (m_button.init(pParentUI,zOrder,comboButtonScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_button.setParentElement(this);
				}
				else
					return X_ERR_OPERATION_FAILED;
				pFilename = (XS8*)pComboBoxElement->Attribute("ListBoxTemplateFile");
				if (pFilename)
				{
					if (m_listBox.init(pParentUI,zOrder,listBoxScreenRect,scrollBarScreenRect, 
						  
						  backButtonScreenRect, 
						  
						  forwardButtonScreenRect,pFilename ) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_listBox.setParentElement(this);
				}
				else
					return X_ERR_OPERATION_FAILED;
			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;

		hideListBox();

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
XStatus XUIComboBox::destroy()
{
	m_listBox.destroy();
	m_textBox.destroy();
	m_button.destroy();

	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::update(XF32 dt)
{
	//Check for a mouse click somewhere outside of the combo box area so
	//we can close the drop down.
	if (XVP::InputMgr.mouseButtonDownOnce(X_MB_LEFT) ||
		XVP::InputMgr.mouseButtonDownOnce(X_MB_RIGHT))
	{
		XCursor* pC = XUserInterface::getCursor();
		if (pC)
		{
			if (!isPointInside(pC->getHotSpot()))
				hideListBox();
		}
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::addItem(XS8* pItemString)
{
	return m_listBox.addItem(pItemString);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::insertItem(XU32 index,XS8* pItemString)
{
	return m_listBox.insertItem(index,pItemString);
}



//*****************************************************************************
//*****************************************************************************
XS32 XUIComboBox::findItem(XS8* pItemString)
{
	return m_listBox.findItem(pItemString);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::removeItem(XU32 index)
{
	return m_listBox.removeItem(index);
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIComboBox::getItem(XU32 index)
{
	return m_listBox.getItem(index);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::clear()
{
	m_textBox.clear();
	m_listBox.clear();
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::refresh()
{
	return m_listBox.refresh();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::show()
{
	m_textBox.show();
	m_button.show();
	XUIElement::show();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::setZOrderOffset(XU32 zOrder)
{
	m_textBox.setZOrderOffset(zOrder);
	m_button.setZOrderOffset(zOrder);
	XUIElement::setZOrderOffset(zOrder);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::showListBox()
{
	m_listBox.show();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::hide()
{
	m_listBox.hide();
	m_textBox.hide();
	m_button.hide();
	XUIElement::hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::hideListBox()
{
	m_listBox.hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIComboBox::isListBoxVisible()
{
	return m_listBox.isVisible();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::enable()
{
	m_listBox.enable();
	m_textBox.enable();
	m_button.enable();
	XUIElement::enable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::disable()
{
	m_listBox.disable();
	m_textBox.disable();
	m_button.disable();
	XUIElement::disable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::setOpacity(XF32 opacity)
{
	m_listBox.setOpacity(opacity);
	m_textBox.setOpacity(opacity);
	m_button.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::move(XVector2D& moveVector)
{
	XRect r;
	m_listBox.getScaledRect(r);
	XVector2D v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_listBox.setScaledPosition(v);

	m_textBox.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_textBox.setScaledPosition(v);

	m_button.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_button.setScaledPosition(v);

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::setJustification(X_TB_JUST_TYPE justification)
{
	return m_listBox.setJustification(justification);
}



//*****************************************************************************
//*****************************************************************************
X_TB_JUST_TYPE XUIComboBox::getJustification()
{
	return m_listBox.getJustification();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::enableSorting(XBOOL ascendingOrder)
{
	return m_listBox.enableSorting(ascendingOrder);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::disableSorting()
{
	return m_listBox.disableSorting();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIComboBox::isSortingEnabled()
{
	return m_listBox.isSortingEnabled();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIComboBox::isPointInside(XVector2D& point)
{
	//Is the list box visible?
	if (isListBoxVisible())
		return (m_listBox.isPointInside(point) ||
				m_textBox.isPointInside(point) ||
				m_button.isPointInside(point));
	else
		return (m_textBox.isPointInside(point) ||
				m_button.isPointInside(point));
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::setToolTipText(XS8* pToolTipText)
{
	m_textBox.setToolTipText(pToolTipText);
	m_button.setToolTipText(pToolTipText);
	m_listBox.setToolTipText(pToolTipText);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIComboBox::getToolTipText()
{
	return m_textBox.getToolTipText();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::clearSelectedItem()
{
	m_listBox.clearSelectedIndex();
	m_textBox.clear();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIComboBox::getSelectedItem()
{
	return m_textBox.getText();
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIComboBox::getNumItems()
{
	return m_listBox.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	//Hide the list box and set the selection if we got a mouse down over
	//the selection box
	if (m_listBox.isPointInsideSelectionBox(hotSpot))
	{
		hideListBox();
		m_textBox.setText(m_listBox.getSelectedItem());
		
		//Should we fire a change event?
		XS32 index = m_listBox.getSelectedIndex();
		if (index != m_prevSelectedIndex)
		{
			m_prevSelectedIndex = index;
			onChange();
		}
	}
	
	if ((pChildElement == &m_button) || (pChildElement == &m_textBox))
	{
		if (isListBoxVisible())
			hideListBox();			
		else
			showListBox();
	}

	XUIElement::onMouseDown(hotSpot,button,pChildElement);
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::onMouseWheel(XS32 movement,XUIElement* pChildElement)
{
	//First, normalize and invert movement to be either -1 or 1.
	if (movement > 0)
		movement = -1;
	else
		movement = 1;

	XS32 step = m_listBox.getScrollStep();
	step += movement;
	if ((step >= 0) && (step < ((XS32)m_listBox.getNumItems() - (XS32)m_listBox.getNumLinesVisible()+1)))
		m_listBox.setScrollStep(step);

	XUIElement::onMouseWheel(movement,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIComboBox::setNativePosition(XVector2D& position)
{
	return X_ERR_NOT_SUPPORTED;
}