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
//	Module Name:	XUIListBox.cpp
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
XUIListBox::XUIListBox()
{
	m_selectedIndex = -1;
	m_prevSelectedIndex = -1;
	m_hoverIndex = -1;
	m_sorted = false;
	m_ascending = true;
	m_pParent = NULL;
	m_scrollEnabled = true;
}



//*****************************************************************************
//*****************************************************************************
XUIListBox::~XUIListBox()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::init(XUserInterface* pParentUI,XU32 zOrder,XRect& textboxRect,
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
		TiXmlElement* pListBoxElement = NULL;
		
		pNode = file.FirstChild( "listbox" );
		if (pNode)
		{
			pListBoxElement = pNode->ToElement();
			if (pListBoxElement)
			{	
				XS8* pFilename = NULL;
				pFilename = (XS8*)pListBoxElement->Attribute("TextBoxTemplateFile");
				if (pFilename)
				{
					if (m_textBox.init(pParentUI,zOrder,textboxRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					m_textBox.setParentElement(this);
					m_textBox.enableWordWrapping(false);
				}
				else
					return X_ERR_OPERATION_FAILED;


				pFilename = (XS8*)pListBoxElement->Attribute("VerticalScrollbarTemplateFile");
				if (pFilename)
				{
					if (m_scrollbar.init(pParentUI,zOrder,scrollBarScreenRect,			  
						  backButtonScreenRect,forwardButtonScreenRect,pFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;	
					m_scrollbar.setParentElement(this);
				}
				else
					return X_ERR_OPERATION_FAILED;
			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;

		configureScrollbar();
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
XStatus XUIListBox::destroy()
{
	clear();
	m_scrollbar.destroy();
	m_textBox.destroy();
	
	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::update(XF32 dt)
{
	if (m_scrollEnabled)
		m_textBox.setScrollStep(m_scrollbar.getStep());
	setHoverIndex(m_hoverIndex);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::render()
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XUIListBox::configureScrollbar()
{
	UINT lineHeight = m_textBox.getLineHeight();
	if (lineHeight != 0)
	{	
		XU32 numLinesPerBox = m_textBox.getNumLinesVisible();
		if (numLinesPerBox > m_items.getNumItems())
		{
			m_scrollbar.hide();
		}
		else 
		{
			if ((m_visible) && (m_scrollEnabled))
				m_scrollbar.show();
			m_scrollbar.setMaxSteps(m_items.getNumItems() - numLinesPerBox);
		}
	}
}


//*****************************************************************************
//*****************************************************************************
void XUIListBox::buildTextBuffer()
{
	m_finalTextBuffer.clear();
	XMemBlockList<string>::iterator i(m_items.begin());
	while (i.isValid())
	{
		m_finalTextBuffer += i.getValue();
		m_finalTextBuffer += '\n';
		i.getNext();
	}

	m_textBox.setText((XS8*)m_finalTextBuffer.c_str());
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::addItem(XS8* pItemString)
{
	if (!pItemString)
		return X_ERR_INVALID_PARAMS;

	string s = pItemString;
	m_items.insertBack(s);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::insertItem(XU32 index,XS8* pItemString)
{
	if (!pItemString)
		return X_ERR_INVALID_PARAMS;

	//First, are we really just appending?  If so, append and return.
	if (index == m_items.getNumItems())
		return addItem(pItemString);

	string s = pItemString;
	XMemBlockList<string>::iterator i(m_items.begin());
	XU32 ctr = 0;
	while (i.isValid())
	{
		if (ctr == index)
		{
			m_items.insert(i,s);
			return X_OK;
		}
		++ctr;
	}
	
	return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XS32 XUIListBox::findItem(XS8* pItemString)
{
	if (!pItemString)
		return -1;
	else
	{
		XMemBlockList<string>::iterator i(m_items.begin());
		XS32 ctr = 0;
		while (i.isValid())
		{
			if (strcmp(i.getValue().c_str(),pItemString) == 0)
				return ctr;
			++ctr;
			i.getNext();
		}
	}

	return -1;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::removeItem(XU32 index)
{
	XMemBlockList<string>::iterator i(m_items.begin());
	if (index < m_items.getNumItems())
	{
		for (XU32 ctr = 0;ctr < index;++ctr)
			i.getNext();
		
		m_items.remove(i.getValue());
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListBox::getItem(XU32 index)
{
	XMemBlockList<string>::iterator i(m_items.begin());
	XU32 ctr = 0;
	while (i.isValid())
	{
		if (ctr == index)
			return (XS8*)i.getValue().c_str();
		++ctr;
		i.getNext();
	}

	return NULL;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListBox::getSelectedItem()
{
	XMemBlockList<string>::iterator i(m_items.begin());

	//If there is not selected item...
	if (m_selectedIndex == -1)
		return NULL;

	if (m_selectedIndex < m_items.getNumItems())
	{
		for (XU32 ctr = 0;ctr < m_selectedIndex;++ctr)
			i.getNext();
		
		return (XS8*)i.getValue().c_str();
	}
	else
		return NULL;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::clear()
{
	m_items.clear();
	m_finalTextBuffer.clear();
	m_textBox.clear();
	m_selectedIndex = -1;
	m_hoverIndex = -1;
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::refresh()
{
#pragma message ("@@@@@@@@@@@@@@@@@@ listbox sort needs implementation")
	/*
	//Do we need to sort?  If so, what order?
	if (m_sorted)
	{
		m_items.sort();
		if (!m_ascending)
			m_items.reverse();
	}*/

	buildTextBuffer();
	configureScrollbar();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setZOrderOffset(XU32 zOrder)
{
	m_textBox.setZOrderOffset(zOrder);
	m_scrollbar.setZOrderOffset(zOrder);
	XUIElement::setZOrderOffset(zOrder);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::show()
{
	m_textBox.show();
	XUIElement::show();

	//Must configure scrollbar last.
	configureScrollbar();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::hide()
{
	m_scrollbar.hide();
	m_textBox.hide();
	XUIElement::hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::enable()
{
	m_scrollbar.enable();
	m_textBox.enable();
	XUIElement::enable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::disable()
{
	m_scrollbar.disable();
	m_textBox.disable();
	XUIElement::disable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setOpacity(XF32 opacity)
{
	m_scrollbar.setOpacity(opacity);
	m_textBox.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::move(XVector2D& moveVector)
{
	XRect r;
	m_scrollbar.getScaledRect(r);
	XVector2D v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_scrollbar.setScaledPosition(v);

	m_textBox.getScaledRect(r);
	v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
	m_textBox.setScaledPosition(v);

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::enableScrollbar()
{
	m_scrollEnabled = true;
	configureScrollbar();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::disableScrollbar()
{
	m_scrollEnabled = false;
	m_scrollbar.hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XUIListBox::getHoverIndex()
{
	return m_hoverIndex;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setHoverIndex(XS32 hoverIndex)
{
	m_hoverIndex = hoverIndex;
	if (m_items.getNumItems() != 0)
	{
		if (m_hoverIndex >= (XS32)(m_items.getNumItems()-1))
			m_hoverIndex = m_items.getNumItems()-1;
		
		//Set the hover index for the child textbox
		if ((m_hoverIndex - (XS32)(m_textBox.getScrollStep())) >= 0)
		{
			m_textBox.setHoverIndex(m_hoverIndex - m_textBox.getScrollStep());
			return X_OK;
		}
	}
	
	//m_hoverIndex = -1;
	m_textBox.setHoverIndex(-1);
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::enableStickyHighlightMode(XBOOL enable)
{
	return m_textBox.enableStickyHighlightMode(enable);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIListBox::isStickyHighlightModeEnabled()
{
	return m_textBox.isStickyHighlightModeEnabled();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIListBox::isPointInside(XVector2D& point)
{
	return ((m_scrollbar.isPointInside(point) && m_scrollbar.isVisible())||
			m_textBox.isPointInside(point));
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIListBox::isPointInsideSelectionBox(XVector2D& point)
{
	return (m_textBox.isPointInside(point));
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setToolTipText(XS8* pToolTipText)
{
	m_textBox.setToolTipText(pToolTipText);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListBox::getToolTipText()
{
	return m_textBox.getToolTipText();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::clearSelectedIndex()
{
	m_selectedIndex = -1;
	m_hoverIndex = -1;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XUIListBox::getSelectedIndex()
{
	return m_selectedIndex;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIListBox::getNumItems()
{
	return m_items.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIListBox::getNumLinesVisible()
{
	return m_textBox.getNumLinesVisible();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setScrollStep(XU32 step)
{
	m_textBox.setScrollStep(step);
	m_scrollbar.setStep(step);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIListBox::getScrollStep()
{
	return m_textBox.getScrollStep();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setJustification(X_TB_JUST_TYPE justification)
{
	return m_textBox.setJustification(justification);
}



//*****************************************************************************
//*****************************************************************************
X_TB_JUST_TYPE XUIListBox::getJustification()
{
	return m_textBox.getJustification();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::enableSorting(XBOOL ascendingOrder)
{
	m_sorted = true;
	m_ascending = ascendingOrder;
	return refresh();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::disableSorting()
{
	m_sorted = false;
	return refresh();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIListBox::isSortingEnabled()
{
	return m_sorted;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIListBox::getNativeTextBoxHeight()
{
	return m_textBox.getNativeTextBoxHeight();
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIListBox::getNativeTextBoxWidth()
{
	return m_textBox.getNativeTextBoxWidth();
}



//*****************************************************************************
//*****************************************************************************
void XUIListBox::updateHoverIndex()
{
	m_hoverIndex = m_textBox.getHoverIndex() + m_textBox.getScrollStep();
	if (m_items.getNumItems() != 0)
	{
		if (m_hoverIndex >= (XS32)(m_items.getNumItems()))
			m_hoverIndex = m_items.getNumItems()-1;
	}
	else
		m_hoverIndex = -1;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement)
{
	if (!m_textBox.isStickyHighlightModeEnabled() && pChildElement == &m_textBox)
		updateHoverIndex();
	XUIElement::onMouseOver(hotSpot,pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
{
	if (pChildElement == &m_textBox)
	{
		updateHoverIndex();
		m_selectedIndex = m_hoverIndex;

		//Should we fire a change event?
		if (m_selectedIndex != m_prevSelectedIndex)
		{
			m_prevSelectedIndex = m_selectedIndex;
			onChange();
		}
	}

	XUIElement::onMouseDown(hotSpot,button,pChildElement);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::onMouseWheel(XS32 movement,XUIElement* pChildElement)
{
	//First, normalize and invert movement to be either -1 or 1.
	if (movement > 0)
		movement = -1;
	else
		movement = 1;

	XS32 step = getScrollStep();
	step += movement;
	if ((step >= 0) && (step < ((XS32)getNumItems() - (XS32)m_textBox.getNumLinesVisible()+1)))
		setScrollStep(step);

	XUIElement::onMouseWheel(movement,this);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::onMouseLeave(XUIElement* pChildElement)
{
	if (!m_textBox.isStickyHighlightModeEnabled())
	{
		setHoverIndex(-1);
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setNativePosition(XVector2D& position)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::setScaledTextBoxRect(XRect& rect)
{
	return m_textBox.setScaledRect(rect);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListBox::getScaledTextBoxRect(XRect& rect)
{
	return m_textBox.getScaledRect(rect);
}