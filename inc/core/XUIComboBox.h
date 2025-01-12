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
//	Module Name:	XUIComboBox.h
//	Author:			Lynn Duke
//	Creation Date:	5-25-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_COMBO_BOX_H__
#define __X_UI_COMBO_BOX_H__





//*****************************************************************************
//*****************************************************************************
class XUIComboBox : public XUIElement
{
protected:
	XUIListBox					m_listBox;
	XUITextBox					m_textBox;
	XUIButton					m_button;
	XHandle						m_listBoxHandle;
	XHandle						m_textBoxHandle;
	XHandle						m_buttonHandle;
	XS32						m_prevSelectedIndex;
	XBOOL						m_sorted;
	XBOOL						m_ascending;

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUIComboBox();
	~XUIComboBox();
	
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& selectionBoxScreenRect,

						 XRect& comboButtonScreenRect,
		
						XRect& listBoxScreenRect,
						  XRect& scrollBarScreenRect,
						  
						  XRect& backButtonScreenRect,
						  
						  XRect& forwardButtonScreenRect,XS8* pTemplateFile);
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	XStatus addItem(XS8* pItemString);
	XStatus insertItem(XU32 index,XS8* pItemString);
	XS32 findItem(XS8* pItemString);
	XStatus removeItem(XU32 index);
	XS8* getItem(XU32 index);
	XS8* getSelectedItem();
	XStatus clearSelectedItem();
	XU32 getNumItems();
	XStatus clear();
	XStatus refresh();
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus show();
	XStatus showListBox();
	XStatus hide();
	XStatus hideListBox();
	XBOOL isListBoxVisible();
	XStatus enable();
	XStatus disable();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus setJustification(X_TB_JUST_TYPE justification);
	X_TB_JUST_TYPE getJustification();
	XStatus enableSorting(XBOOL ascendingOrder);
	XStatus disableSorting();
	XBOOL isSortingEnabled();
	XBOOL isPointInside(XVector2D& point);
	XStatus setToolTipText(XS8* pToolTipText);
	XS8* getToolTipText();
	virtual XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,XUIElement* pChildElement);
	virtual XStatus onMouseWheel(XS32 movement,XUIElement* pChildElement);
	XStatus setNativePosition(XVector2D& position);
};

#endif
