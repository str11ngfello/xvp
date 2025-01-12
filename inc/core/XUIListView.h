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
//	Module Name:	XUIListView.h
//	Author:			Lynn Duke
//	Creation Date:	6-23-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_LIST_VIEW_H__
#define __X_UI_LIST_VIEW_H__





//*****************************************************************************
//*****************************************************************************
class XUIListView : public XUIElement
{
private:

	class XUIListButton : public XUIButton
	{
	private:
		XU32			m_ID;
		XUIListView*	m_pParentListView;
		XBOOL			m_ascending;
	public:
		XUIListButton(XU32 id,XUIListView* pParentListView)
		{
			m_ascending = true;
			m_ID = id;
			m_pParentListView = pParentListView;
		}
		
		XStatus onButtonUp()
		{
			XUIButton::onButtonUp();
			m_pParentListView->sortByColumn(m_ID,m_ascending);
			m_ascending = !m_ascending;
			return X_OK;
		}
	};


	class XUIListSeparator : public XUISprite
	{
	private:
		bool			m_dragging;
		XU32			m_ID;
		XUIListView*	m_pParentListView;

	public:

		XUIListSeparator(XU32 id,XUIListView* pParentListView)
		{
			m_dragging = false;
			m_ID = id;
			m_pParentListView = pParentListView;
		}


		XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement)
		{
			//If parent list view pointer is null, then we aren't supposed to respond
			//to mouse clicks.
			if (!m_pParentListView)
				return X_OK;

			if (button == X_MB_LEFT)
			{
				m_dragging = true;
				m_pParentUI->enterDragMode(this);
			}

			return X_OK;
		}

		XStatus onDrag(XVector2D& drag)
		{
			//First, check to make sure resize is allowed
			if (!m_pParentListView->columnResizeAllowed(m_ID))
				return X_OK;

			XRect scaledRect;
			getScaledRect(scaledRect);

			XVector2D v = XVector2D((XF32)scaledRect.left + drag.x,(XF32)scaledRect.top);
			m_pParentListView->constrainMovement(m_ID,v);			

			setScaledPosition(v);
			return m_pParentListView->resizeLists(m_ID);
		}
	};

	struct SortInfo
	{
		string s;
		XU32 index;

		bool operator < (SortInfo& si)
		{
			return s < si.s;
			return true;
		}
	};

	vector<XUIListBox*>			m_listBoxes;
	vector<XUIListButton*>		m_columnHeaders;
	vector<XUIListSeparator*>	m_columnSeperators;
	vector<XUITextBox*>			m_columnHeaderTextBoxes;
	XBOOL						m_lockColumnSizes;
	XS32						m_prevSelectedIndex;

	XStatus resizeLists(XU32 sepID);
	XStatus constrainMovement(XU32 sepID,XVector2D& v);
	XBOOL columnResizeAllowed(XU32 sepID);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIListView();
	virtual ~XUIListView();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XUserInterface* pParentUI,XU32 zOrder,XU32 numColumns,
				  XRect* pColumnRects,XRect& scrollBarScreenRect,					  
				  XRect& backButtonScreenRect,XRect& forwardButtonScreenRect,
				  XS8* pTemplateFile);
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus render();
	XStatus addItem(XS8* pItemString);
	XStatus insertItem(XU32 rowIndex,XS8* pItemString);
	XS8* getItem(XU32 rowIndex);
	XS32 findItem(XS8* pItemString);
	XStatus removeItem(XU32 rowIndex);
	XStatus setSubItem(XU32 rowIndex,XU32 columnIndex,XS8* pSubItemString);
	XS8* getSubItem(XU32 rowIndex,XU32 columnIndex);
	XStatus clearSelectedIndex();
	XS32 getSelectedIndex();
	XU32 getNumItems();
	XStatus setColumnHeading(XU32 columnIndex,XS8* pHeadingText);
	XS8* getColumnHeading(XU32 columnIndex);
	XStatus setColumnJustification(XU32 columnIndex,X_TB_JUST_TYPE justification);
	X_TB_JUST_TYPE getColumnJustification(XU32 columnIndex);
	XStatus setColumnHeadingJustification(XU32 columnIndex,X_TB_JUST_TYPE justification);
	X_TB_JUST_TYPE getColumnHeadingJustification(XU32 columnIndex);
	XStatus setColumnHeadingToolTipText(XU32 columnIndex,XS8* pText);
	XS8* getColumnHeadingToolTipText(XU32 columnIndex);
	XStatus lockColumnSizes(XBOOL lock);
	XStatus sortByColumn(XU32 columnIndex,XBOOL ascending);
	XStatus clear();
	XStatus refresh();
	XStatus enable();
	XStatus disable();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus show();
	XStatus hide();
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement);
	XStatus onMouseLeave(XUIElement* pChildElement);
	XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	XStatus onMouseWheel(XS32 movement,XUIElement* pChildElement);
	XStatus setNativePosition(XVector2D& position);
	friend XUIListSeparator;
};

#endif

