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
//	Module Name:	XUIListView.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-23-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>
	


//*****************************************************************************
//*****************************************************************************
XUIListView::XUIListView()
{
	m_lockColumnSizes = false;
	m_prevSelectedIndex = -1;
}



//*****************************************************************************
//*****************************************************************************
XUIListView::~XUIListView()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::init(XUserInterface* pParentUI,XU32 zOrder,XU32 numColumns,
						  XRect* pColumnRects,XRect& scrollBarScreenRect,					  
						  XRect& backButtonScreenRect,
						  XRect& forwardButtonScreenRect,XS8* pTemplateFile)
{
	if ((!pTemplateFile) || (!pColumnRects) || (numColumns < 2))
		return X_ERR_INVALID_PARAMS;

	char path[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_UI_TEMPLATE,pTemplateFile,path);

	TiXmlDocument file(path);
	if (!file.LoadFile())
		return X_ERR_OPERATION_FAILED;
	else
	{
		TiXmlNode* pNode = NULL;
		TiXmlElement* pListViewElement = NULL;
		
		pNode = file.FirstChild( "listview" );
		if (pNode)
		{
			pListViewElement = pNode->ToElement();
			if (pListViewElement)
			{	
				XS8* pFilename = NULL;
				XS8* pFilename2 = NULL;
				XS8* pFilename3 = NULL;
				XS8* pFilename4 = NULL;
				XS32 colHeaderHeight = 0;
				XS32 colHeaderSepWidth = 0;
				
				pListViewElement->QueryIntAttribute("ColumnHeaderHeight",&colHeaderHeight);
				pListViewElement->QueryIntAttribute("ColumnHeaderSepWidth",&colHeaderSepWidth);
			
				pFilename = (XS8*)pListViewElement->Attribute("ListBoxTemplateFile");
				pFilename2 = (XS8*)pListViewElement->Attribute("ColHeaderImageTemplateFile");
				pFilename3 = (XS8*)pListViewElement->Attribute("ColHeaderSepTemplateFile");
				pFilename4 = (XS8*)pListViewElement->Attribute("ColHeaderTextBoxTemplateFile");
				
				if (pFilename)
				{
					for (XU32 i = 0;i < numColumns;++i)
					{
						//Increment the zorder for each column.
						XUIListBox* pLB = NULL;
						XNew(pLB,XUIListBox,XOSMgr::getDefaultHeap());
						if  (pLB->init(pParentUI,(zOrder+i+1),pColumnRects[i],scrollBarScreenRect,backButtonScreenRect,
								forwardButtonScreenRect,pFilename) != X_OK)								  
							return X_ERR_OPERATION_FAILED;
						else
						{
							pLB->setParentElement(this);
							pLB->enableStickyHighlightMode(true);
							//pLB->enableWordWrapping(false);
							m_listBoxes.push_back(pLB);
							if (i != 0)
								pLB->disableScrollbar();

							//Create the header for this column
							if (pFilename2)
							{
								XUIListButton* pButton = NULL;//new XUIListButton(i,this);
								XNew(pButton,XUIListButton(i,this),XOSMgr::getDefaultHeap());
								XRect colHeaderRect(pColumnRects[i].left,pColumnRects[i].top - colHeaderHeight,
												pColumnRects[i].right - colHeaderSepWidth,pColumnRects[i].top);
								if (pButton->init(pParentUI,zOrder+i+1,colHeaderRect,pFilename2) != X_OK)
									return X_ERR_OPERATION_FAILED;
								else
								{
									m_columnHeaders.push_back(pButton);

									//Create the seperator for the column header
									XUIListSeparator* pSep;
									if (i == numColumns-1)
										XNew(pSep,XUIListSeparator(i,NULL),XOSMgr::getDefaultHeap())
									else
										XNew(pSep,XUIListSeparator(i,this),XOSMgr::getDefaultHeap())
									XRect sepRect(pColumnRects[i].right - colHeaderSepWidth,
												  colHeaderRect.top,pColumnRects[i].right,
												  pColumnRects[i].top);
									if (pSep->init(pParentUI,zOrder+i+1,sepRect,pFilename3) != X_OK)
										return X_ERR_OPERATION_FAILED;
									else
									{
										m_columnSeperators.push_back(pSep);

										//All but the last seperator is resizeable
										if (i != numColumns-1)
											pSep->setPointerState(X_PS_HORIZONTAL_RESIZE);

										if (pFilename4)
										{
											//Create a textbox for the column heading.
											XUITextBox* pTextBox = NULL;
											XNew(pTextBox,XUITextBox(),XOSMgr::getDefaultHeap());
											XRect colHeaderRect(pColumnRects[i].left,pColumnRects[i].top - colHeaderHeight,
												pColumnRects[i].right - colHeaderSepWidth,pColumnRects[i].top);
											if (pTextBox->init(pParentUI,(zOrder+i+1),colHeaderRect,pFilename4) != X_OK)
												return X_ERR_OPERATION_FAILED;
											else
											{
												m_columnHeaderTextBoxes.push_back(pTextBox);
												pTextBox->setParentElement(m_columnHeaders[i]);
											}
										}
									}
								}
							}
						}
					}
				}
				else
					return X_ERR_OPERATION_FAILED;
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
XStatus XUIListView::destroy()
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
	{
		m_listBoxes[i]->destroy();
		XDelete(m_listBoxes[i]);
	}
	m_listBoxes.clear();
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
	{
		m_columnHeaders[i]->destroy();
		XDelete(m_columnHeaders[i]);
	}
	m_columnHeaders.clear();
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
	{
		m_columnSeperators[i]->destroy();
		XDelete(m_columnSeperators[i]);
	}
	m_columnSeperators.clear();
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
	{
		m_columnHeaderTextBoxes[i]->destroy();
		XDelete(m_columnHeaderTextBoxes[i]);
	}
	m_columnHeaderTextBoxes.clear();
	
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::update(XF32 dt)
{
	//Apply the updated scroll steps to all list boxes
	XU32 step = m_listBoxes[0]->getScrollStep();
	for (XU32 i = 1;i < m_listBoxes.size();++i)
	{
		m_listBoxes[i]->setScrollStep(step);
	}

	//Should we fire a change event?
	XS32 si = getSelectedIndex();
	if (si != m_prevSelectedIndex)
	{
		m_prevSelectedIndex = si;
		onChange();
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::render()
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::addItem(XS8* pItemString)
{
	if (m_listBoxes.size() != 0)
	{
		//Add the item and uses spaces as placeholders for the sub items for
		//now.
		for (XU32 i = 0;i < m_listBoxes.size();++i)
		{
			if (i == 0)
				m_listBoxes[i]->addItem(pItemString);
			else
				m_listBoxes[i]->addItem(" ");
		}

		return X_OK;
	}
	else
		return X_ERR_NOT_READY;
}




//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::insertItem(XU32 rowIndex,XS8* pItemString)
{
	if (m_listBoxes.size() != 0)
	{
		//Insert the item and uses spaces as placeholders for the sub items for
		//now.
		for (XU32 i = 0;i < m_listBoxes.size();++i)
		{
			if (i == 0)
			{
				if (m_listBoxes[i]->insertItem(rowIndex,pItemString) != X_OK)
					return X_ERR_NO_SUCH_OBJECT;
			}
			else
				m_listBoxes[i]->insertItem(rowIndex," ");
		}

		return X_OK;
	}
	else
		return X_ERR_NOT_READY;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListView::getItem(XU32 rowIndex)
{
	if (m_listBoxes.size() == 0)
		return NULL;
	else
		return m_listBoxes[0]->getItem(rowIndex);
}



//*****************************************************************************
//*****************************************************************************
XS32 XUIListView::findItem(XS8* pItemString)
{
	if (m_listBoxes.size() == 0)
		return NULL;
	else
	{
		return m_listBoxes[0]->findItem(pItemString);
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::removeItem(XU32 rowIndex)
{
	if (m_listBoxes.size() == 0)
		return X_ERR_NOT_READY;
	
	if (rowIndex > m_listBoxes[0]->getNumItems()-1)
		return X_ERR_NO_SUCH_OBJECT;

	//Remove item and all sub items.
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->removeItem(rowIndex);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setSubItem(XU32 rowIndex,XU32 columnIndex,XS8* pSubItemString)
{
	if (m_listBoxes.size() == 0)
		return X_ERR_NOT_READY;

	if (columnIndex < m_listBoxes.size() && columnIndex != 0)
	{
		if (m_listBoxes[columnIndex]->removeItem(rowIndex) == X_OK)
		{
			m_listBoxes[columnIndex]->insertItem(rowIndex,pSubItemString);
			return X_OK;
		}
	}
	
	return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListView::getSubItem(XU32 rowIndex,XU32 columnIndex)
{
	if (m_listBoxes.size() == 0)
		return NULL;

	if (columnIndex < m_listBoxes.size() && columnIndex != 0)
	{
		//Remember, a single space denotes a place holder and the item
		//is really empty.
		XS8* pTemp = m_listBoxes[columnIndex]->getItem(rowIndex);
		if (strcmp(pTemp," ") == 0)
			return NULL;
		else
			return pTemp;
	}
	else
		return NULL;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::clearSelectedIndex()
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->clearSelectedIndex();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XUIListView::getSelectedIndex()
{
	//Because of the way list boxes report events to the list view
	//we need to find out the selected index and return one when we
	//find one.  If we don't find a non -1 index, that means nothing
	//is selected.
	XS32 index = -1;
	for (XU32 i = 0;i < m_listBoxes.size();++i)
	{
		index = m_listBoxes[i]->getSelectedIndex();
		if (index != -1)
			break;
	}
	
    return index;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUIListView::getNumItems()
{
	if (m_listBoxes.size() == 0)
		return X_ERR_NOT_READY;
	else
		return m_listBoxes[0]->getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setColumnHeading(XU32 columnIndex,XS8* pHeadingText)
{
	if (columnIndex >= m_columnHeaderTextBoxes.size())
		return X_ERR_NO_SUCH_OBJECT;
	else
		return m_columnHeaderTextBoxes[columnIndex]->setText(pHeadingText);
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListView::getColumnHeading(XU32 columnIndex)
{
	if (columnIndex >= m_columnHeaderTextBoxes.size())
		return NULL;
	else
		return m_columnHeaderTextBoxes[columnIndex]->getText();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setColumnJustification(XU32 columnIndex,X_TB_JUST_TYPE justification)
{
	if (columnIndex >= m_listBoxes.size())
		return X_ERR_NO_SUCH_OBJECT;
	else
		return m_listBoxes[columnIndex]->setJustification(justification);
}



//*****************************************************************************
//*****************************************************************************
X_TB_JUST_TYPE XUIListView::getColumnJustification(XU32 columnIndex)
{
	if (columnIndex >= m_listBoxes.size())
		return X_TBJ_NONE;
	else
		return m_listBoxes[columnIndex]->getJustification();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setColumnHeadingJustification(XU32 columnIndex,
												   X_TB_JUST_TYPE justification)
{
	if (columnIndex >= m_columnHeaderTextBoxes.size())
		return X_ERR_NO_SUCH_OBJECT;
	else
		return m_columnHeaderTextBoxes[columnIndex]->setJustification(justification);
}



//*****************************************************************************
//*****************************************************************************
X_TB_JUST_TYPE XUIListView::getColumnHeadingJustification(XU32 columnIndex)
{
	if (columnIndex >= m_columnHeaderTextBoxes.size())
		return X_TBJ_NONE;
	else
		return m_columnHeaderTextBoxes[columnIndex]->getJustification();
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setColumnHeadingToolTipText(XU32 columnIndex,XS8* pText)
{
	if (columnIndex >= m_columnHeaderTextBoxes.size())
		return X_ERR_NO_SUCH_OBJECT;
	else
		return m_columnHeaderTextBoxes[columnIndex]->setToolTipText(pText);
}



//*****************************************************************************
//*****************************************************************************
XS8* XUIListView::getColumnHeadingToolTipText(XU32 columnIndex)
{
	if (columnIndex >= m_columnHeaderTextBoxes.size())
		return NULL;
	else
		return m_columnHeaderTextBoxes[columnIndex]->getToolTipText();
}
	


//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::refresh()
{
	if (m_listBoxes.size() == 0)
		return X_ERR_NOT_READY;

	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->refresh();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::clear()
{
	if (m_listBoxes.size() == 0)
		return X_ERR_NOT_READY;

	while (getNumItems() != 0)
		removeItem(0);
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::enable()
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->enable();
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
		m_columnHeaders[i]->enable();
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
		m_columnSeperators[i]->enable();
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
		m_columnHeaderTextBoxes[i]->enable();
	XUIElement::enable();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::disable()
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->disable();
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
		m_columnHeaders[i]->disable();
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
		m_columnSeperators[i]->disable();
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
		m_columnHeaderTextBoxes[i]->disable();
	XUIElement::disable();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setOpacity(XF32 opacity)
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->setOpacity(opacity);
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
		m_columnHeaders[i]->setOpacity(opacity);
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
		m_columnSeperators[i]->setOpacity(opacity);
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
		m_columnHeaderTextBoxes[i]->setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::move(XVector2D& moveVector)
{
	XRect r;
	XVector2D v;

	for (XU32 i = 0;i < m_listBoxes.size();++i)
	{
		m_listBoxes[i]->getScaledRect(r);
		v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
		m_listBoxes[i]->setScaledPosition(v);
	}
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
	{
		m_columnHeaders[i]->getScaledRect(r);
		v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
		m_columnHeaders[i]->setScaledPosition(v);
	}
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
	{
		m_columnSeperators[i]->getScaledRect(r);
		v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
		m_columnSeperators[i]->setScaledPosition(v);
	}
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
	{
		m_columnHeaderTextBoxes[i]->getScaledRect(r);
		v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
		m_columnHeaderTextBoxes[i]->setScaledPosition(v);
	}

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::show()
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->show();
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
		m_columnHeaders[i]->show();
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
		m_columnSeperators[i]->show();
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
		m_columnHeaderTextBoxes[i]->show();
	XUIElement::show();

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::hide()
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->hide();
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
		m_columnHeaders[i]->hide();
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
		m_columnSeperators[i]->hide();
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
		m_columnHeaderTextBoxes[i]->hide();
	XUIElement::hide();

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setZOrderOffset(XU32 zOrder)
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		m_listBoxes[i]->setZOrderOffset(zOrder+i+1);
	for (XU32 i = 0;i < m_columnHeaders.size();++i)
		m_columnHeaders[i]->setZOrderOffset(zOrder+i+1);
	for (XU32 i = 0;i < m_columnSeperators.size();++i)
		m_columnSeperators[i]->setZOrderOffset(zOrder+i+1);
	for (XU32 i = 0;i < m_columnHeaderTextBoxes.size();++i)
		m_columnHeaderTextBoxes[i]->setZOrderOffset(zOrder+i+1);
	XUIElement::setZOrderOffset(zOrder);

	return X_OK;
}
	
	

//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement)
{
	XUIListBox* pLB = NULL;
	for (XU32 i = 0;i < m_listBoxes.size();++i)
	{
		if (m_listBoxes[i]->isPointInsideSelectionBox(hotSpot))
		{
			pLB = m_listBoxes[i];
			break;
		}
	}

	if (pLB)
	{
		//Run through the list boxes again and apply the hover index to the other
		//boxes
		for (XU32 i = 0;i < m_listBoxes.size();++i)
			m_listBoxes[i]->setHoverIndex(pLB->getHoverIndex());
	}

	XUIElement::onMouseOver(hotSpot,pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::onMouseLeave(XUIElement* pChildElement)
{
	for (XU32 i = 0;i < m_listBoxes.size();++i)
		if (!m_listBoxes[i]->isStickyHighlightModeEnabled())
			m_listBoxes[i]->setHoverIndex(-1);

	XUIElement::onMouseLeave(pChildElement);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,
								 XUIElement* pChildElement)
{
	//Anytime we get a mouse button down even, let's sync all scroll steps for
	//all text boxes to match the last text box since that's the scroll bar that
	//might have been changed.
	/*XU32 step = m_listBoxes[m_listBoxes.size()-1]->getScrollStep();
	for (XU32 i = 0;i < m_listBoxes.size()-1;++i)
		m_listBoxes[i]->setScrollStep(step);
*/
	XUIElement::onMouseDown(hotSpot,button,pChildElement);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::setNativePosition(XVector2D& position)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::resizeLists(XU32 sepID)
{
	//First and last separators cannot resize anything
	if (sepID < m_columnSeperators.size()-1)
	{
		XRect r,r2;
		m_columnSeperators[sepID]->getScaledRect(r);

		m_columnHeaders[sepID]->getScaledRect(r2);
		r2.right = r.left;
		m_columnHeaders[sepID]->setScaledRect(r2);

		m_columnHeaders[sepID+1]->getScaledRect(r2);
		r2.left = r.right;
		m_columnHeaders[sepID+1]->setScaledRect(r2);

		m_columnHeaderTextBoxes[sepID]->getScaledRect(r2);
		r2.right = r.left;
		m_columnHeaderTextBoxes[sepID]->setScaledRect(r2);

		m_columnHeaderTextBoxes[sepID+1]->getScaledRect(r2);
		r2.left = r.right;
		m_columnHeaderTextBoxes[sepID+1]->setScaledRect(r2);
		
		m_listBoxes[sepID]->getScaledTextBoxRect(r2);
		r2.right = r.right;
		m_listBoxes[sepID]->setScaledTextBoxRect(r2);

		m_listBoxes[sepID+1]->getScaledTextBoxRect(r2);
		r2.left = r.right;
		m_listBoxes[sepID+1]->setScaledTextBoxRect(r2);

		/*m_columnHeaders[sepID]->getScaledRect(r);
		r.right += (XS32)drag.x;
		m_columnHeaders[sepID]->setScaledRect(r);

		m_columnHeaders[sepID+1]->getScaledRect(r);
		r.left += (XS32)drag.x;
		m_columnHeaders[sepID+1]->setScaledRect(r);

		m_columnHeaderTextBoxes[sepID]->getScaledRect(r);
		r.right += (XS32)drag.x;
		m_columnHeaderTextBoxes[sepID]->setScaledRect(r);

		m_columnHeaderTextBoxes[sepID+1]->getScaledRect(r);
		r.left += (XS32)drag.x;
		m_columnHeaderTextBoxes[sepID+1]->setScaledRect(r);

		m_listBoxes[sepID]->getScaledTextBoxRect(r);
		r.right += (XS32)drag.x;
		m_listBoxes[sepID]->setScaledTextBoxRect(r);

		m_listBoxes[sepID+1]->getScaledTextBoxRect(r);
		r.left += (XS32)drag.x;
		m_listBoxes[sepID+1]->setScaledTextBoxRect(r);*/

	}
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::constrainMovement(XU32 sepID,XVector2D& v)
{
	XRect r;
	XVector2D hotSpot = XUserInterface::getCursor()->getHotSpot();
	//if (sepID == 0)
	//{
		m_columnHeaders[sepID]->getScaledRect(r);
		if (v.x < r.left || hotSpot.x < r.left) 
			v.x = (XF32)r.left;	
		m_columnSeperators[sepID+1]->getScaledRect(r);
		if ((((v.x + (r.right - r.left)) > r.left)) || (hotSpot.x + (r.right - r.left)) > r.left)
			v.x = (XF32)r.left - (r.right - r.left);
	/*}
	else
	{
		m_columnHeaders[sepID]->getScaledRect(r);
		if (v.x < r.left)
			v.x = (XF32)r.left;	
		m_columnSeparators[1]->getScaledRect(r);
		if (v.x > r.left)
			v.x = (XF32)r.left - (r.right - r.left);
	}
	*/
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIListView::columnResizeAllowed(XU32 sepID)
{
	return !m_lockColumnSizes;	
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::lockColumnSizes(XBOOL lock)
{
	m_lockColumnSizes = lock;
	
	//Set the appropriate mouse pointer states.
	X_POINTER_STATE ps;
	if (lock)
		ps = X_PS_NORMAL;
	else
		ps = X_PS_HORIZONTAL_RESIZE;
	for (XU32 i = 0;i < m_columnSeperators.size()-1;++i)
		m_columnSeperators[i]->setPointerState(ps);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::sortByColumn(XU32 columnIndex,XBOOL ascending)
{
	if (columnIndex >= m_listBoxes.size())
		return X_ERR_NO_SUCH_OBJECT;

	//First, sort a copy of the column's items and track their position
	//changes.
	clearSelectedIndex();
	XMemBlockList<SortInfo> sortedList;
	for (XU32 i = 0;i < m_listBoxes[columnIndex]->getNumItems();++i)
	{	
		SortInfo si;
		si.s = m_listBoxes[columnIndex]->getItem(i);
		si.index = i;
		sortedList.insertBack(si);
	}
	X_WARNING("sort needs to be implemented")
	#pragma message ("@@@@@@@@@@@@@@@@@@ listview sort needs implementation")
	//sortedList.sort();
	if (!ascending)
	{
		X_WARNING("reverse needs to be implemented")
		//sortedList.reverse();
	}
	
	//Append sorted items to the end of the list view
	XMemBlockList<SortInfo>::iterator itr(sortedList.begin());
	while (itr.isValid())
	{
		XU32 index = (itr.getValue()).index;
		for (XU32 i = 0;i < m_listBoxes.size();++i)
		{
			m_listBoxes[i]->addItem(m_listBoxes[i]->getItem(index));
		}
		itr.getNext();
	}

	//Now remove the old entries from above to leave the sorted ones in perfect order
	for (XU32 i = 0;i < m_listBoxes.size();++i)
	{
		for (XU32 j = 0;j < sortedList.getNumItems();++j)
		m_listBoxes[i]->removeItem(0);
	}

	sortedList.clear(); 
	refresh();

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIListView::onMouseWheel(XS32 movement,XUIElement* pChildElement)
{
	//First, normalize and invert movement to be either -1 or 1.
	if (movement > 0)
		movement = -1;
	else
		movement = 1;

	XS32 step = m_listBoxes[0]->getScrollStep();
	step += movement;
	if ((step >= 0) && (step < (XS32)m_listBoxes[0]->getNumItems() - (XS32)m_listBoxes[0]->getNumLinesVisible()+1))
	{	
		for (XU32 i = 0;i < m_listBoxes.size();++i)
		{
			m_listBoxes[i]->setScrollStep(step);
		}
	}

	return X_OK;
}