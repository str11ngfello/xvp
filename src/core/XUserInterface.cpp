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
//	Module Name:	XUserInterface.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-12-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//Statics
//*****************************************************************************
XCursor			XUserInterface::m_cursor;
XBOOL			XUserInterface::m_cursorLoaded = false;
XBOOL			XUserInterface::m_cursorVisible = false;
XBOOL			XUserInterface::m_cursorEnabled = false;
XUserInterface*	XUserInterface::m_currentWorkingUI = NULL;


#define X_USER_INTERFACE_LUA_LIB_NAME	"UI"

static const luaL_reg XUserInterfaceLuaLib[] = 
{
	{"setNativeResolution",LAPI_UISetNativeResolution},
	{"registerCreationHandler",LAPI_UIRegisterCreationHandler},
	{"registerDestructionHandler",LAPI_UIRegisterDestructionHandler},
	{"registerUpdateHandler",LAPI_UIRegisterUpdateHandler},
	{"registerEventHandler",LAPI_UIRegisterEventHandler},

	{"enableScaling",LAPI_UIEnableScaling},
	{"isScalingEnabled",LAPI_UIIsScalingEnabled},
	{"enableToolTips",LAPI_UIEnableToolTips},
	{"disableToolTips",LAPI_UIDisableToolTips},
	{"getXScale",LAPI_UIGetXScale},
	{"getYScale",LAPI_UIGetYScale},
	{"setToolTipTimeout",LAPI_UISetToolTipTimeout},
	{"getToolTipTimeout",LAPI_UIGetToolTipTimeout},
	{"setOpacity",LAPI_UISetOpacity},
	{"createCursor",LAPI_UICreateCursor},
	{"destroyCursor",LAPI_UIDestroyCursor},
	{"getCursorHotSpot",LAPI_UIGetCursorHotSpot},
	{"getCursorRelXMov",LAPI_UIGetCursorRelXMov},
	{"getCursorRelYMov",LAPI_UIGetCursorRelYMov},
	{"setCursorState",LAPI_UISetCursorState},
	{"getCursorState",LAPI_UIGetCursorState},
	{"showCursor",LAPI_UIShowCursor},
	{"isCursorVisible",LAPI_UIIsCursorVisible},
	{"lockCursorState",LAPI_UILockCursorState},
	{"isCursorStateLocked",LAPI_UIIsCursorStateLocked},
	{"enableCursor",LAPI_UIEnableCursor},
	{"enabledCursor",LAPI_UIIsCursorEnabled},


	
	{"createSprite",LAPI_UICreateSprite},
	{"createAnimSprite",LAPI_UICreateAnimSprite},
	{"setAnimSpriteBeginRow",LAPI_UISetAnimSpriteBeginRow},
	{"getAnimSpriteBeginRow",LAPI_UIGetAnimSpriteBeginRow},
	{"setAnimSpriteEndRow",LAPI_UISetAnimSpriteEndRow},
	{"getAnimSpriteEndRow",LAPI_UIGetAnimSpriteEndRow},
	{"getAnimSpriteCurrentRow",LAPI_UIGetAnimSpriteCurrentRow},
	{"getAnimSpriteCurrentColumn",LAPI_UIGetAnimSpriteCurrentColumn},
	{"getAnimSpriteFrameWidth",LAPI_UIGetAnimSpriteFrameWidth},
	{"getAnimSpriteFrameHeight",LAPI_UIGetAnimSpriteFrameHeight},
	{"setAnimSpriteFrameDelay",LAPI_UISetAnimSpriteFrameDelay},
	{"getAnimSpriteFrameDelay",LAPI_UIGetAnimSpriteFrameDelay},
	{"setAnimSpritePlayCount",LAPI_UISetAnimSpritePlayCount},
	{"getAnimSpritePlayCount",LAPI_UIGetAnimSpritePlayCount},
	{"playAnimSprite",LAPI_UIPlayAnimSprite},
	{"stopAnimSprite",LAPI_UIStopAnimSprite},
	{"isAnimSpritePlaying",LAPI_UIIsAnimSpritePlaying},

	{"createTextBox",LAPI_UICreateTextBox},
	{"setTextBoxText",LAPI_UISetTextBoxText},
	{"getTextBoxText",LAPI_UIGetTextBoxText},
	{"appendTextBoxText",LAPI_UIAppendTextBoxText},
	{"removeTextBoxTextFromEnd",LAPI_UIRemoveTextBoxTextFromEnd},
	{"setTextBoxBufferSize",LAPI_UISetTextBoxBufferSize},
	{"getTextBoxBufferSize",LAPI_UIGetTextBoxBufferSize},
	{"getTextBoxTextLength",LAPI_UIGetTextBoxTextLength},
	{"setTextBoxScrollStep",LAPI_UISetTextBoxScrollStep},
	{"getTextBoxScrollStep",LAPI_UIGetTextBoxScrollStep},
	{"setTextBoxReadOnlyMarker",LAPI_UISetTextBoxReadOnlyMarker},
	{"getTextBoxReadOnlyMarker",LAPI_UIGetTextBoxReadOnlyMarker},
	{"setTextBoxJustification",LAPI_UISetTextBoxJustification},
	{"getTextBoxJustification",LAPI_UIGetTextBoxJustification},
	{"setTextBoxCursorBlinkRate",LAPI_UISetTextBoxCursorBlinkRate},
	{"getTextBoxCursorBlinkRate",LAPI_UIGetTextBoxCursorBlinkRate},
	{"showTextBoxCursor",LAPI_UIShowTextBoxCursor},
	{"clearTextBox",LAPI_UIClearTextBox},
	{"getTextBoxLineHeight",LAPI_UIGetTextBoxLineHeight},
	{"getNativeTextBoxHeight",LAPI_UIGetNativeTextBoxHeight},
	{"getNativeTextBoxWidth",LAPI_UIGetNativeTextBoxWidth},
	{"getNumTextBoxLinesVisible",LAPI_UIGetNumTextBoxLinesVisible},
	{"setTextBoxCharSet",LAPI_UISetTextBoxCharSet},
	{"getTextBoxCharSet",LAPI_UIGetTextBoxCharSet},
	{"enableTextBoxAutoScroll",LAPI_UIEnableTextBoxAutoScroll},
	{"isTextBoxAutoScrollEnabled",LAPI_UIIsTextBoxAutoScrollEnabled},
	{"enableTextBoxWordWrapping",LAPI_UIEnableTextBoxWordWrapping},
	{"isTextBoxWordWrappingEnabled",LAPI_UIIsTextBoxWordWrappingEnabled},
	{"enableTextBoxStickyHighlightMode",LAPI_UIEnableTextBoxStickyHighlightMode},
	{"isTextBoxStickyHighlightModeEnabled",LAPI_UIIsTextBoxStickyHighlightModeEnabled},
	{"setTextBoxEditable",LAPI_UISetTextBoxEditable},
	{"isTextBoxEditable",LAPI_UIIsTextBoxEditable},
	{"setTextBoxHoverIndex",LAPI_UISetTextBoxHoverIndex},
	{"getTextBoxHoverIndex",LAPI_UIGetTextBoxHoverIndex},
	{"getScaledTextBoxRect",LAPI_UIGetScaledTextBoxRect},


	{"createMovie",LAPI_UICreateMovie},
	{"playMovie",LAPI_UIPlayMovie},
	{"stopMovie",LAPI_UIStopMovie},
	{"pauseMovie",LAPI_UIPauseMovie},
	{"resumeMovie",LAPI_UIResumeMovie},
	{"setMovieTime",LAPI_UISetMovieTime},
	{"getMovieTime",LAPI_UIGetMovieTime},
	{"setMoviePlayCount",LAPI_UISetMoviePlayCount},
	{"getMoviePlayCount",LAPI_UIGetMoviePlayCount},
	{"getMovieDuration",LAPI_UIGetMovieDuration},
	{"isMoviePlaying",LAPI_UIIsMoviePlaying},

	{"createVerticalSlider",LAPI_UICreateVerticalSlider},
	{"setVerticalSliderPercentage",LAPI_UISetVerticalSliderPercentage},
	{"getVerticalSliderPercentage",LAPI_UIGetVerticalSliderPercentage},
	{"isVerticalSliderSliding",LAPI_UIIsVerticalSliderSliding},

	{"createHorizontalSlider",LAPI_UICreateHorizontalSlider},
	{"setHorizontalSliderPercentage",LAPI_UISetHorizontalSliderPercentage},
	{"getHorizontalSliderPercentage",LAPI_UIGetHorizontalSliderPercentage},
	{"isHorizontalSliderSliding",LAPI_UIIsHorizontalSliderSliding},

	{"createVerticalScrollbar",LAPI_UICreateVerticalScrollbar},
	{"setVerticalScrollbarStep",LAPI_UISetVerticalScrollbarStep},
	{"getVerticalScrollbarStep",LAPI_UIGetVerticalScrollbarStep},
	{"setVerticalScrollbarMaxSteps",LAPI_UISetVerticalScrollbarMaxSteps},
	{"getVerticalScrollbarMaxSteps",LAPI_UIGetVerticalScrollbarMaxSteps},
	{"incrementVerticalScrollbarStep",LAPI_UIIncrementVerticalScrollbarStep},
	{"decrementVerticalScrollbarStep",LAPI_UIDecrementVerticalScrollbarStep},

	{"createHorizontalScrollbar",LAPI_UICreateHorizontalScrollbar},
	{"setHorizontalScrollbarStep",LAPI_UISetHorizontalScrollbarStep},
	{"getHorizontalScrollbarStep",LAPI_UIGetHorizontalScrollbarStep},
	{"setHorizontalScrollbarMaxSteps",LAPI_UISetHorizontalScrollbarMaxSteps},
	{"getHorizontalScrollbarMaxSteps",LAPI_UIGetHorizontalScrollbarMaxSteps},
	{"incrementHorizontalScrollbarStep",LAPI_UIIncrementHorizontalScrollbarStep},
	{"decrementHorizontalScrollbarStep",LAPI_UIDecrementHorizontalScrollbarStep},

	{"createButton",LAPI_UICreateButton},
	{"isButtonPressed",LAPI_UIIsButtonPressed},
	{"enableButtonRepeatMode",LAPI_UIEnableButtonRepeatMode},
	{"isButtonRepeatModeEnabled",LAPI_UIIsButtonRepeatModeEnabled},

	{"createToggle",LAPI_UICreateToggle},
	{"setToggleState",LAPI_UISetToggleState},


	{"createProgressBar",LAPI_UICreateProgressBar},
	{"renderProgressBarCoverFirst",LAPI_UIRenderProgressBarCoverFirst},
	{"isProgressBarCoverFirst",LAPI_UIIsProgressBarCoverFirst},
	{"setProgressBarPercentage",LAPI_UISetProgressBarPercentage},
	{"getProgressBarPercentage",LAPI_GetProgressBarPercentage},

	{"createSpinner",LAPI_UICreateSpinner},
	{"getSpinnerValue",LAPI_UIGetSpinnerValue},
	{"setSpinnerValue",LAPI_UISetSpinnerValue},
	{"incrementSpinnerStep",LAPI_UIIncrementSpinnerStep},
	{"decrementSpinnerStep",LAPI_UIDecrementSpinnerStep},
	{"setSpinnerStepSize",LAPI_UISetSpinnerStepSize},


	{"createComboBox",LAPI_UICreateComboBox},
	{"addComboBoxItem",LAPI_UIAddComboBoxItem},
	{"insertComboBoxItem",LAPI_UIInsertComboBoxItem},
	{"findComboBoxItem",LAPI_UIFindComboBoxItem},
	{"removeComboBoxItem",LAPI_UIRemoveComboBoxItem},
	{"getComboBoxItem",LAPI_UIGetComboBoxItem},
	{"getSelectedComboBoxItem",LAPI_UIGetSelectedComboBoxItem},
	{"getNumComboBoxItems",LAPI_UIGetNumComboBoxItems},
	{"clearComboBox",LAPI_UIClearComboBox},
	{"refreshComboBox",LAPI_UIRefreshComboBox},
	{"showComboBoxListBox",LAPI_UIShowComboBoxListBox},
	{"hideComboBoxListBox",LAPI_UIHideComboBoxListBox},
	{"isComboBoxListBoxVisible",LAPI_UIIsComboBoxListBoxVisible},
	{"setComboBoxJustification",LAPI_UISetComboBoxJustification},
	{"getComboBoxJustification",LAPI_UIGetComboBoxJustification},
	{"enableComboBoxSorting",LAPI_UIEnableComboBoxSorting},
	{"disableComboBoxSorting",LAPI_UIDisableComboBoxSorting},
	{"isComboBoxSortingEnabled",LAPI_UIIsComboBoxSortingEnabled},
	{"clearComboBoxSelectedItem",LAPI_UIClearComboBoxSelectedItem},


	{"createListBox",LAPI_UICreateListBox},
	{"addListBoxItem",LAPI_UIAddListBoxItem},
	{"insertListBoxItem",LAPI_UIInsertListBoxItem},
	{"removeListBoxItem",LAPI_UIRemoveListBoxItem},
	{"findListBoxItem",LAPI_UIFindListBoxItem},
	{"getListBoxItem",LAPI_UIGetListBoxItem},
	{"clearSelectedListBoxIndex",LAPI_UIClearSelectedListBoxIndex},
	{"getSelectedListBoxIndex",LAPI_UIGetSelectedListBoxIndex},
	{"getSelectedListBoxItem",LAPI_UIGetSelectedListBoxItem},
	{"getNumListBoxItems",LAPI_UIGetNumListBoxItems},
	{"clearListBox",LAPI_UIClearListBox},
	{"refreshListBox",LAPI_UIRefreshListBox},
	{"enableListBoxStickyHighlightMode",LAPI_UIEnableListBoxStickyHighlightMode},
	{"isListBoxStickyHighlightModeEnabled",LAPI_UIIsListBoxStickyHighlightModeEnabled},
	{"setListBoxHoverIndex",LAPI_UISetListBoxHoverIndex},
	{"getListBoxHoverIndex",LAPI_UIGetListBoxHoverIndex},
	{"setListBoxScrollStep",LAPI_UISetListBoxScrollStep},
	{"getListBoxScrollStep",LAPI_UIGetListBoxScrollStep},
	{"setListBoxJustification",LAPI_UISetListBoxJustification},
	{"getListBoxJustification",LAPI_UIGetListBoxJustification},
	{"enableListBoxSorting",LAPI_UIEnableListBoxSorting},
	{"disableListBoxSorting",LAPI_UIDisableListBoxSorting},
	{"isListBoxSortingEnabled",LAPI_UIIsListBoxSortingEnabled},


	{"createListView",LAPI_UICreateListView},
	{"addListViewItem",LAPI_UIAddListViewItem},
	{"insertListViewItem",LAPI_UIInsertListViewItem},
	{"findListViewItem",LAPI_UIFindListViewItem},
	{"removeListViewItem",LAPI_UIRemoveListViewItem},
	{"getListViewItem",LAPI_UIGetListViewItem},
	{"setListViewSubItem",LAPI_UISetListViewSubItem},
	{"getListViewSubItem",LAPI_UIGetListViewSubItem},
	{"clearSelectedListViewIndex",LAPI_UIClearSelectedListViewIndex},
	{"getSelectedListViewIndex",LAPI_UIGetSelectedListViewIndex},
	{"getNumListViewItems",LAPI_UIGetNumListViewItems},
	{"setListViewColumnHeading",LAPI_UISetListViewColumnHeading},
	{"getListViewColumnHeading",LAPI_UIGetListViewColumnHeading},
	{"setListViewColumnJustification",LAPI_UISetListViewColumnJustification},
	{"getListViewColumnJustification",LAPI_UIGetListViewColumnJustification},
	{"setListViewColumnHeadingJustification",LAPI_UISetListViewColumnHeadingJustification},
	{"getListViewColumnHeadingJustification",LAPI_UIGetListViewColumnHeadingJustification},
	{"setListViewColumnHeadingToolTipText",LAPI_UISetListViewColumnHeadingToolTipText},
	{"getListViewColumnHeadingToolTipText",LAPI_UIGetListViewColumnHeadingToolTipText},
	{"lockListViewColumnSizes",LAPI_UILockListViewColumnSizes},
	{"clearListView",LAPI_UIClearListView},
	{"refreshListView",LAPI_UIRefreshListView},

	{"createFrame",LAPI_UICreateFrame},
	{"addElementToFrame",LAPI_UIAddElementToFrame},

	{"destroyElement",LAPI_UIDestroyElement},
	{"setElementZOrderOffset",LAPI_UISetElementZOrderOffset},
	{"getElementZOrderOffset",LAPI_UIGetElementZOrderOffset},
	{"setElementToolTipText",LAPI_UISetElementToolTipText},
	{"getElementToolTipText",LAPI_UIGetElementToolTipText},
	{"enableElement",LAPI_UIEnableElement},
	{"disableElement",LAPI_UIDisableElement},
	{"isElementEnabled",LAPI_UIIsElementEnabled},
	{"showElement",LAPI_UIShowElement},
	{"hideElement",LAPI_UIHideElement},
	{"isElementVisible",LAPI_UIIsElementVisible},
	{"elementHasFocus",LAPI_UIElementHasFocus},
	{"isPointInsideElement",LAPI_UIIsPointInsideElement},
	{"setElementPointerState",LAPI_UISetElementPointerState},
	{"getElementPointerState",LAPI_UIGetElementPointerState},
	{"setElementOpacity",LAPI_UISetElementOpacity},
	{"getElementOpacity",LAPI_UIGetElementOpacity},

	{NULL,NULL}
};



//*****************************************************************************
/**
Constructor for the user interface.
*/
//*****************************************************************************
XUserInterface::XUserInterface()
{
	m_nativeXRes = 0;
	m_nativeYRes = 0;
	m_xScale = 1;
	m_yScale = 1;
	m_scale = true;
	m_pLastElementEntered = NULL;
	m_toolTipTimer.start();
	m_toolTipTimeout = 1;
	m_toolTipsEnabled = false;
	m_pElementWithFocus = NULL;
	m_pDragElement = NULL;
	m_pCreationHandler = NULL;
	m_pDestructionHandler = NULL;
	m_pEventHandler = NULL;
	m_pUpdateHandler = NULL;
	m_queueElementCmds = false;
}



//*****************************************************************************
/**
Destructor for the user interface.
*/
//*****************************************************************************
XUserInterface::~XUserInterface()
{
}



//*****************************************************************************
/**
Sets the native resolution of the user interface.  The native resolution is
used to scale coordinates when the user interface is displayed in other
resolutions.

@param nativeXRes	The native horizontal resolution of the user interface in
					pixels.  This value is used to determine how to scale the
					interface's elements when the screen width changes.  All
					user interface elements should be defines relative to
					this screen width.
@param nativeYRes	The native vertical resolution of the user interface in
					pixels.  This value is used to determine how to scale the
					interface's elements when the screen height changes.  All
					user interface elements should be defines relative to
					this screen height.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::setNativeResolution(XU32 nativeXRes,XU32 nativeYRes)
{
	m_nativeXRes = nativeXRes;
	m_nativeYRes = nativeYRes;

	//Update the scaling ratios.
	updateScalingRatios();

	return X_OK;
}




//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::notify(X_UI_LUA_EVENT event)
{
	if (m_pEventHandler)
	{
		XLuaArgument args[2];
		args[0].luaNumber = 0;
		args[0].type = X_LAT_NUMBER;
		
		switch(event)
		{
		case X_UILE_ADDED_TO_SCENE:
			args[1].luaNumber = (XF64)X_UILE_ADDED_TO_SCENE;
			args[1].type = X_LAT_NUMBER;
			break;
		case X_UILE_REM_FROM_SCENE:
			args[1].luaNumber = (XF64)X_UILE_REM_FROM_SCENE;
			args[1].type = X_LAT_NUMBER;
			break;
		default:
			break;
		}

		return XLua::executeFunction(m_pEventHandler,args,2,0);
	}
	else
		return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::load(XS8* pFilename)
{
	m_currentWorkingUI = this;

	//Destroy and previous data
	destroy();

	//Now, try to run the script
	if (XLua::executeScript(pFilename) != X_OK)
		return X_ERR_OPERATION_FAILED;
	
	//Script was valid, so let's try to call the creation handler.
	if (m_pCreationHandler)
		return XLua::executeFunction(m_pCreationHandler,NULL,0,0);
	else
		return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::destroy()
{
	//If there is a script destroy handler, call it.
	if (m_pDestructionHandler)
		XLua::executeFunction(m_pDestructionHandler,NULL,0,0);

	if (m_toolTipsEnabled)
	{
		m_pToolTipBox->destroy();
		XDelete(m_pToolTipBox);
	}
	m_elementList.clear();
	m_frameList.clear();

	XOSMgr::destroyString(&m_pCreationHandler);
	XOSMgr::destroyString(&m_pDestructionHandler);
	XOSMgr::destroyString(&m_pEventHandler);
	XOSMgr::destroyString(&m_pUpdateHandler);

	m_pLastElementEntered = NULL;
	m_pElementWithFocus = NULL;
	m_pDragElement = NULL;

	return X_OK;
}



//*****************************************************************************
/**
Allows the user interface to update any internal, time-dependent data.  This
method is called once per frame by the engine and allows the interface
to trigger and process events such as mouse up, mouse down, etc.

@param dt	Amount of time, in seconds, that has passed since the last
					update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::update(XF32 dt)
{
	m_currentWorkingUI = this;

	//If there is a script update handler, call it.  It expects the dt
	//as a parameter
	if (m_pUpdateHandler)
	{
		XLuaArgument arg;
		arg.type = X_LAT_NUMBER;
		arg.luaNumber = dt;
		XLua::executeFunction(m_pUpdateHandler,&arg,1,0);
	}

	XMemBlockList<XUIElement*>::iterator i;
	XUIElement* pSavedElement = NULL;
	
	//Update the scaling ratios.
	updateScalingRatios();
	
	//First, update each element.
	i.set(m_elementList.begin());
	while (i.isValid())
	{
		i.getValue()->update(dt);
		i.getNext();
	}

	//Now go through the element list and send the mouse up,down and over messages
	//to the closest element z order wise.  We can only do this if a cursor has
	//been added to the user interface.
	if (!m_cursorLoaded || !m_cursorVisible || !m_cursorEnabled)
	{
		if (m_toolTipsEnabled)
			m_pToolTipBox->hide();
		return X_OK;
	}

	XVector2D cursorHotSpot = m_cursor.getHotSpot();

	//If the mouse cursor moved...reset the tooltip timer and hide the tool
	//tip box
	if (m_toolTipsEnabled)
	{
		if (cursorHotSpot != m_lastCursorHotSpot)
		{
			m_toolTipTimer.reset();
			m_pToolTipBox->hide();
		}
	}

	//Are we in drag mode?  If so we don't send any messages except drag
	//messages
	if (m_pDragElement)
	{
		XVector2D delta = cursorHotSpot - m_lastCursorHotSpot;
		if ((delta.x != 0) || (delta.y != 0))
			m_pDragElement->onDrag(delta);
		
		//Check for a mouse up event to exit drag mode
		if (XVP::InputMgr.mouseButtonUpOnce(X_MB_LEFT))
			m_pDragElement = NULL;

		m_lastCursorHotSpot = cursorHotSpot;

		return X_OK;
	}

	//Queue element insertion/deletion commands until we're done processing the UI
	m_queueElementCmds = true;

	//XMemBlockList<XUIElement*>::iterator i(m_elementList.end());
	i.set(m_elementList.end());
	while (i.isValid())
	{		
		//First, always skip the tool tip box.  It doesn't receive any
		//events.
		if (i.getValue() == m_pToolTipBox)
		{
			i.getPrev();
			continue;
		}

		//Send the mouse messages if the cursor is inside
		//the element and the element is enabled.
		if (i.getValue()->isPointInside(cursorHotSpot) && i.getValue()->isEnabled() && i.getValue()->isVisible())
		{
			//If this is the first time we've entered this element with the
			//mouse cursor, tell it so.  Also, tell the last element that
			//the mouse has left it.
			if (m_pLastElementEntered != i.getValue())
			{
				if (m_pLastElementEntered)
				{
					m_pLastElementEntered->onMouseLeave(NULL);

					if (m_toolTipsEnabled)
					{
						m_pToolTipBox->clear();
						m_pToolTipBox->hide();
					}
				}
				
				//Send an "entered" message to the element and start the tool tip
				//timer
				i.getValue()->onMouseEnter(NULL);
				m_toolTipTimer.reset();

				//Remember that we just sent the "mouse enter" message
				m_pLastElementEntered = i.getValue();
			}
	
			//Check to see if the tool tip timer has expired, if it has,
			//then display the tool tip.
			if ((m_toolTipTimer.getDuration() >= m_toolTipTimeout) && m_toolTipsEnabled)
			{
				//If the control has tool text associated with it, display it.
				XS8* pText = i.getValue()->getToolTipText();
				if (pText)
				{	
					//Let's figure out how big the tool tip text box
					//should be.  Set the text first.
					m_pToolTipBox->setText(pText);

					XRect cursorRect;
					XRect destRect;
					m_cursor.getDestRect(cursorRect);
					
					XU32 numNewLines = m_pToolTipBox->getNumHardLineBreaks();
					XU32 lineHeight = (XU32)(m_pToolTipBox->getLineHeight());
					
					//Calculate the dimensions of the tool tip box in scaled space
					destRect.left = (XS32)cursorHotSpot.x;
					destRect.top = (XS32)cursorHotSpot.y + (cursorRect.bottom - cursorRect.top);

					XU32 nativeWidth = (XS32)(m_pToolTipBox->getLongestLineInNativePixels() + (m_pToolTipBox->getTextPadding()*2));
					XU32 nativeHeight = (XS32)((lineHeight + (m_pToolTipBox->getTextPadding()*2) + (numNewLines * lineHeight)));
					destRect.right = (XS32)(destRect.left + (nativeWidth * m_xScale));
					destRect.bottom = (XS32)(destRect.top + (nativeHeight * m_yScale));

					//Because of possible floating point loss, we need to pad the width just a little bit
					//to take account for it.  It shouldn't be too noticeable but will keep the 
					//width at the minimum needed to display the tool tip text
					destRect.right += 1;
 
					//Now, set the scaled rect and show the box
					m_pToolTipBox->setScaledRect(destRect);
					m_pToolTipBox->show();
				}				
			}

			if (XVP::InputMgr.mouseButtonDownOnce(X_MB_LEFT))
			{
				i.getValue()->onMouseDown(cursorHotSpot,X_MB_LEFT,NULL);
				i.getValue()->onGotFocus();
			}
			if (XVP::InputMgr.mouseButtonDownOnceRepeat(X_MB_LEFT))
				i.getValue()->onMouseDownRepeat(cursorHotSpot,X_MB_LEFT,NULL);
			if (XVP::InputMgr.mouseButtonUpOnce(X_MB_LEFT))
				i.getValue()->onMouseUp(cursorHotSpot,X_MB_LEFT,NULL);

			if (XVP::InputMgr.mouseButtonDownOnce(X_MB_RIGHT))
				i.getValue()->onMouseDown(cursorHotSpot,X_MB_RIGHT,NULL);
			if (XVP::InputMgr.mouseButtonDownOnceRepeat(X_MB_RIGHT))
				i.getValue()->onMouseDownRepeat(cursorHotSpot,X_MB_RIGHT,NULL);
			if (XVP::InputMgr.mouseButtonUpOnce(X_MB_RIGHT))
				i.getValue()->onMouseUp(cursorHotSpot,X_MB_RIGHT,NULL);

			if (XVP::InputMgr.mouseButtonDownOnce(X_MB_MIDDLE))
				i.getValue()->onMouseDown(cursorHotSpot,X_MB_MIDDLE,NULL);
			if (XVP::InputMgr.mouseButtonDownOnceRepeat(X_MB_MIDDLE))
				i.getValue()->onMouseDownRepeat(cursorHotSpot,X_MB_MIDDLE,NULL);
			if (XVP::InputMgr.mouseButtonUpOnce(X_MB_MIDDLE))
				i.getValue()->onMouseUp(cursorHotSpot,X_MB_MIDDLE,NULL);

			i.getValue()->onMouseOver(cursorHotSpot,NULL);

			if (m_pElementWithFocus)
			{
				XS32 wheelMovement = XVP::InputMgr.getMouseRelZMovement();
				if ((wheelMovement != 0) && (m_pElementWithFocus))
					m_pElementWithFocus->onMouseWheel(wheelMovement,NULL);
			}
			//(*ri)->eventNotify(X_UIE_MOUSE_OVER,&cursorHotSpot);
			
			//Save this element so we know exactly who received the messages
			//and therefore will not recieve the "mouse not over" message
			//below.  Also, break from the loop to ensure that only one
			//interface element receives these messages.
			pSavedElement = i.getValue();
			break;
		}
		i.getPrev();
	}
	
	//Was the mouse over any elements?  If not, then we need to make sure that the
	//last element entered, if there was one, is sent a "mouse leave" message.
	if ((!pSavedElement) && (m_pLastElementEntered))
	{
		m_pLastElementEntered->onMouseLeave(NULL);
		m_pLastElementEntered = NULL;

		if (m_toolTipsEnabled)
		{
			m_toolTipTimer.reset();
			m_pToolTipBox->hide();
			m_pToolTipBox->clear();
		}

		//Return cursor to normal state if not over any elements
		if (m_cursorLoaded)
			m_cursor.setState(X_PS_NORMAL);
	}

	//Now go through the element list and send the "mouse not over"
	//message to everyone except the saved element from above.
	i.set(m_elementList.begin());
	while (i.isValid())
	{
		//If the element is not visible or enabled, skip it...
		if (!i.getValue()->isEnabled() || !i.getValue()->isVisible())
		{
			i.getNext();
			continue;
		}

		/*if (pSavedElement)
		{
			if (pSavedElement != (*i))
				(*i)->eventNotify(X_UIE_MOUSE_NOT_OVER,&cursorHotSpot);
		}
		else
			(*i)->eventNotify(X_UIE_MOUSE_NOT_OVER,&cursorHotSpot);
		
		//Send the global mouse up and mouse down events to every
		//single element.
		if (XVP::InputMgr.mouseButtonDownOnce(X_MB_LEFT))
			(*i)->eventNotify(X_UIE_GLOBAL_MOUSE_DOWN,&cursorHotSpot);
		if (XVP::InputMgr.mouseButtonUpOnce(X_MB_LEFT))
			(*i)->eventNotify(X_UIE_GLOBAL_MOUSE_UP,&cursorHotSpot);
			*/
		i.getNext();
	}

	m_lastCursorHotSpot = cursorHotSpot;


	//Now process element commands that were queued
	m_queueElementCmds = false;
	processElementCmds();

	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface by rendering all elements within the interface.  
For example, if the interface has four buttons, 2 text boxes,
and a sprite, calling this method will force each interface element to
render itself.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::render()
{
	//Update the elements
	XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());
	while (i.isValid())
	{
		i.getValue()->render();
		i.getNext();
	}
	
	return X_OK;
}																																   



//*****************************************************************************
/**
Updates the cursor.  

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::updateCursor(XF32 dt)
{
	if (m_cursorLoaded)
		return m_cursor.update(dt);
	return X_OK;
}



//*****************************************************************************
/**
Renders the cursor.  

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::renderCursor()
{
	if ((m_cursorLoaded) && (m_cursorVisible))
		return m_cursor.render();
	else
		return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XUserInterface::updateScalingRatios()
{
	//Update the scaling ratios if scaling is enabled.
	if (m_scale && (XVP::GraphicsMgr.getRenderContext() != X_INVALID_HANDLE))
	{
		if ((m_nativeXRes != 0) && (m_nativeYRes != 0))
		{
#pragma message("Again, UI won't work until these lines are fixed")
			float tx = (XF32)((XF32)XVP::GraphicsMgr.getRenderContextWidth(XVP::GraphicsMgr.getRenderContext()) / (XF32)m_nativeXRes);
			float ty = (XF32)((XF32)XVP::GraphicsMgr.getRenderContextHeight(XVP::GraphicsMgr.getRenderContext()) / (XF32)m_nativeYRes);
			
			//Notify each element if scaling ratios have changed from their previous values
			if ((ty != m_yScale) || (tx != m_xScale))
			{
				m_xScale = tx;
				m_yScale = ty;
			
				XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());
				while (i.isValid())
				{
					i.getValue()->onResize(tx,ty);	
					i.getNext();
				}
			}
		}
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::addFrame(XUIFrame* pFrame)
{
	if (!pFrame)
		return X_ERR_INVALID_PARAMS;

	XMemBlockList<XUIFrame*>::iterator i(m_frameList.begin());

	//Cycle through the list looking for the correct insertion point
	XS32 zOrder = pFrame->getZOrderOffset();
	while (i.isValid())
	{
		if (zOrder <= i.getValue()->getZOrderOffset())
			break;
		i.getNext();
	}

	m_frameList.insert(i,pFrame);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::removeFrame(XUIFrame* pFrame)
{
	m_frameList.remove(pFrame);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::bringFrameToFront(XUIFrame* pFrame)
{
	if (!pFrame)
		return X_ERR_INVALID_PARAMS;

	//If this frame is already on in front, or there is only
	//one frame in the UI, then return.
	if ((pFrame == (m_frameList.getBack())) || (m_frameList.getNumItems() == 1))
		return X_OK;

	//First, find the frame in the list.
	XMemBlockList<XUIFrame*>::iterator i(m_frameList.begin());
	XMemBlockList<XUIFrame*>::iterator j(m_frameList.begin());
	XS32 zOrder = pFrame->getZOrderOffset();
	while (i.isValid())
	{
		if (pFrame == i.getValue())
		{
			j.getNext();
			break;
		}
		i.getNext();
		j.getNext();
	}

	//Now, we know that all frames above this one need to be pulled back by one in
	//this list.  We are going to assume we can shuffle all the zorders down by one.
	//Remove the frame from the list then move all higher frames down by using shuffling
	//the zorders down.
	m_frameList.remove(pFrame);
	XS32 tempZOrder = 0;
	while (j.isValid())
	{
		tempZOrder = j.getValue()->getZOrderOffset();
		j.getValue()->changeZOrder(zOrder);
		zOrder = tempZOrder;
		j.getNext();
	}

	//Now, zOrder holds the new zorder of the highest frame which we'll assign
	//to the frame which we are bring to the front of all other frames.
	pFrame->changeZOrder(zOrder);
	m_frameList.insertBack(pFrame);

	return X_OK;
}


//*****************************************************************************
/**
Processes all node commands on the command queue.

@return None.
*/
//*****************************************************************************
XStatus XUserInterface::processElementCmds()
{
	//Let's process and empty the command queues.  
	while (m_elementCmdQueue.getNumItems() != 0)
	{
		XElementCmd* pCmd = &m_elementCmdQueue.getFront();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			addElement(pCmd->pElement,pCmd->zOrder);
			break;
		case X_CMDT_REMOVE:
			removeElement(pCmd->pElement);
			break;
		}
		m_elementCmdQueue.removeFront();		
	}

	return X_OK;
}



//*****************************************************************************
/**
Gets a pointer to the cursor.

@return A pointer to the cursor.  If no cursor has been loaded, the
		return value is NULL.
*/
//*****************************************************************************
XCursor* XUserInterface::getCursor()
{
	//If no cursor has been created yet, return NULL.
	if (!m_cursorLoaded)
		return NULL;
	else
		return &m_cursor;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::enableScaling(XBOOL enable)
{
	m_scale = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUserInterface::isScalingEnabled()
{
	return m_scale;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::enableToolTips(XS8* pTextBoxTemplateFile)
{
	if (!pTextBoxTemplateFile)
		return X_ERR_INVALID_PARAMS;

	//Destroy any previous settings and re-create the tool tip box.
	//Destroy the tool tip box if necessary and 
	if (m_toolTipsEnabled)
	{
		m_pToolTipBox->destroy();
		XDelete(m_pToolTipBox);
	}

	XNew(m_pToolTipBox,XUITextBox,XOSMgr::getDefaultHeap());
	m_pToolTipBox->init(this,X_UI_TOOL_TIP_Z_ORDER,XRect(0,0,128,32),pTextBoxTemplateFile);
	m_toolTipsEnabled = true;
	m_pToolTipBox->hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::disableToolTips()
{
	m_toolTipsEnabled = false;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUserInterface::isToolTipsEnabled()
{
	return m_toolTipsEnabled;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::setToolTipTimeout(float seconds)
{
	if (seconds > 0)
	{
		m_toolTipTimeout = seconds;
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
float XUserInterface::getToolTipTimeout()
{
	return m_toolTipTimeout;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::setOpacity(XF32 opacity)
{
	//Set the opacity for all elements contained within the user interface.
	XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());
	while (i.isValid())
	{
		i.getValue()->setOpacity(opacity);
		i.getNext();
	}
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::sendLuaEvent(XUIElement* pElement,X_UI_LUA_EVENT event)
{
	if ((pElement) && (m_pEventHandler))
	{
		XLuaArgument args[2];
		args[0].luaNumber = (XF64)(XU64)pElement;
		args[0].type = X_LAT_NUMBER;
		args[1].luaNumber = (XF64)event;
		args[1].type = X_LAT_NUMBER;
		return XLua::executeFunction(m_pEventHandler,args,2,0);
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::enterDragMode(XUIElement* pElement)
{
	if (!pElement)
		return X_ERR_INVALID_PARAMS;

	//Can't enter drag mode if we're already in drag mode
	if (m_pDragElement)
		return X_ERR_NOT_READY;
	else
	{
		m_pDragElement = pElement;
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
float XUserInterface::getXScale()
{
	return m_xScale;
}



//*****************************************************************************
//*****************************************************************************
float XUserInterface::getYScale()
{
	return m_yScale;
}

	
	
//*****************************************************************************
/**
Returns the number of elements within the user interface.

@return The number of elements within the user interface.
*/
//*****************************************************************************
XU32 XUserInterface::getNumElements()
{
	return m_elementList.getNumItems();
}



//*****************************************************************************
/**
Creates a cursor for all user interfaces to use.  

@param pTextureFilename	The texture to use for the cursor.
@param desc	An anim sprite description structure.
@param hotSpot	The coordinates, relative to the cursor texture's upper left
				corner, that represent the hot spot of the cursor.
@param speed	The speed of the cursor.  This value should be non-zero and
				positive.
@return	A return code of type XStatus.	
*/
//*****************************************************************************
XStatus XUserInterface::createCursor(XS8* pTemplateFile)
{
	XStatus status;

	//Make sure the file name is valid.
	if (!pTemplateFile)
		return X_ERR_INVALID_PARAMS;

	//Destroy any previous cursor.
	m_cursor.destroy();

	char path[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_UI_TEMPLATE,pTemplateFile,path);

	//Initialize the new cursor.
	status = m_cursor.init(path);

	if (status == X_OK)
	{
		m_cursorLoaded = true;
		m_cursorVisible = true;
		m_cursorEnabled = true;
	}
	
	return status;
}

	
	
//*****************************************************************************
/**
Destroys the cursor for the user interface.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::destroyCursor()
{
	//If there is no cursor, just return X_OK.
	if (m_cursorLoaded)
	{	
		m_cursor.destroy();
		m_cursorLoaded = false;
		m_cursorVisible = false;
		m_cursorEnabled = false;
	}

	return X_OK;
}



//*****************************************************************************
/**
Shows the user interface cursor.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUserInterface::showCursor(XBOOL show)
{
	m_cursorVisible = show;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUserInterface::isCursorVisible()
{
	return m_cursorVisible;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::enableCursor(XBOOL enable)
{
	m_cursorEnabled = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUserInterface::isCursorEnabled()
{
	return m_cursorEnabled;
}



//*****************************************************************************
/**
Adds a user interface element to the user interface.

@param pElement	A pointer to the element to add.
@param zOrder	The zOrder of the user interface element with respect to other
				user interface elements.
*/
//*****************************************************************************
XStatus XUserInterface::addElement(XUIElement* pElement,XU32 zOrder)
{
	if (!pElement)
		return X_ERR_INVALID_PARAMS;

	//Check to make sure we can add the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_queueElementCmds)
	{
		XElementCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.pElement = pElement;
		cmd.zOrder = zOrder;
		m_elementCmdQueue.insertBack(cmd);
		return X_OK;
	}

	XMemBlockList<XUIElement*>::iterator i(m_elementList.begin());

	//Cycle through the list looking for the correct insertion point
	while (i.isValid())
	{
		if (zOrder <= i.getValue()->getZOrderOffset())
			break;
		i.getNext();
	}

	m_elementList.insert(i,pElement);

	//Call an initial resize event if scaling is enabled
	if (m_scale)
		pElement->onResize(m_xScale,m_yScale);
	else
		pElement->onResize(1.0f,1.0f);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::removeElement(XUIElement* pElement)
{
	if (!pElement)
		return X_ERR_INVALID_PARAMS;
	else
	{
		//Check to make sure we can remove the node right now.  If we can't, then
		//queue the request for later processing.
		if (m_queueElementCmds)
		{
			XElementCmd cmd;
			cmd.cmdType = X_CMDT_REMOVE;
			cmd.pElement = pElement;
			m_elementCmdQueue.insertBack(cmd);
			return X_OK;
		}

		m_elementList.remove(pElement);
		return X_OK;
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::initLuaGlue()
{
	static XBOOL initialized = false;
	if (initialized)
		return X_ERR_REDUNDANT_OPERATION;
	else
	{
		initialized = true;
		return XLua::registerLibrary(X_USER_INTERFACE_LUA_LIB_NAME,XUserInterfaceLuaLib);
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::setCreationHandler(XS8* pHandlerName)
{
	if (pHandlerName)
	{
		m_pCreationHandler = XOSMgr::createString(pHandlerName);
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::setDestructionHandler(XS8* pHandlerName)
{
	if (pHandlerName)
	{
		m_pDestructionHandler = XOSMgr::createString(pHandlerName);
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::setEventHandler(XS8* pHandlerName)
{
	if (pHandlerName)
	{
		m_pEventHandler = XOSMgr::createString(pHandlerName);
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUserInterface::setUpdateHandler(XS8* pHandlerName)
{
	if (pHandlerName)
	{
		m_pUpdateHandler = XOSMgr::createString(pHandlerName);
		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}
