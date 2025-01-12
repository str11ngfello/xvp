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
//	Module Name:	XUIListBox.h
//	Author:			Lynn Duke
//	Creation Date:	5-22-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_LIST_BOX_H__
#define __X_UI_LIST_BOX_H__




//*****************************************************************************
//*****************************************************************************
class XUIListBox : public XUIElement
{
protected:
	XUIVerticalScrollbar		m_scrollbar;
	XUITextBox					m_textBox;
	XHandle						m_scrollbarHandle;
	XHandle						m_textBoxHandle;
	XMemBlockList<string>		m_items;
	string						m_finalTextBuffer;
	XS32						m_selectedIndex;
	XS32						m_prevSelectedIndex;
	XS32						m_hoverIndex;
	XBOOL						m_sorted;
	XBOOL						m_ascending;
	XBOOL						m_scrollEnabled;
	XUIElement*					m_pParent;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	void configureScrollbar();
	void buildTextBuffer();
	void updateHoverIndex();


public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIListBox();
	virtual ~XUIListBox();
		
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& textboxRect,
						 XRect& scrollBarScreenRect,
						  
						  XRect& backButtonScreenRect,
						  
						  XRect& forwardButtonScreenRect,XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
	XStatus addItem(XS8* pItemString);
	XStatus insertItem(XU32 index,XS8* pItemString);
	XStatus removeItem(XU32 index);
	XS32 findItem(XS8* pItemString);
	XS8* getItem(XU32 index);
	XS32 getSelectedIndex();
	XStatus clearSelectedIndex();
	XS8* getSelectedItem();
	XU32 getNumItems();
	XU32 getNumLinesVisible();
	XStatus clear();
	XStatus refresh();
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus show();
	XStatus hide();
	XStatus enable();
	XStatus disable();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus enableScrollbar();
	XStatus disableScrollbar();
	XStatus enableStickyHighlightMode(XBOOL enable);
	XBOOL isStickyHighlightModeEnabled();
	XS32 getHoverIndex();
	XStatus setHoverIndex(XS32 hoverIndex);
	XBOOL isPointInside(XVector2D& point);
	XBOOL isPointInsideSelectionBox(XVector2D& point);
	XStatus setToolTipText(XS8* pToolTipText);
	XS8* getToolTipText();
	XStatus setScrollStep(XU32 step);
	XU32 getScrollStep();
	XStatus setJustification(X_TB_JUST_TYPE justification);
	X_TB_JUST_TYPE getJustification();
	XStatus enableSorting(XBOOL ascendingOrder);
	XStatus disableSorting();
	XBOOL isSortingEnabled();
	XU32 getNativeTextBoxHeight();
	XU32 getNativeTextBoxWidth();
	virtual XStatus onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement);
	virtual XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseWheel(XS32 movement,XUIElement* pChildElement);
	virtual XStatus onMouseLeave(XUIElement* pChildElement);
	XStatus setNativePosition(XVector2D& position);
	XStatus setScaledTextBoxRect(XRect& rect);
	XStatus getScaledTextBoxRect(XRect& rect);
};

#endif
