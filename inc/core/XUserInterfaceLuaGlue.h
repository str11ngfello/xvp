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
//	Module Name:	XUserInterfaceLuaGlue.h
//	Author:			Lynn Duke
//	Creation Date:	8-27-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_USER_INTERFACE_LUA_GLUE_H__
#define __X_USER_INTERFACE_LUA_GLUE_H__




int LAPI_UISetNativeResolution(lua_State* pLuaState);
int LAPI_UIRegisterCreationHandler(lua_State* pLuaState);
int LAPI_UIRegisterDestructionHandler(lua_State* pLuaState);
int LAPI_UIRegisterUpdateHandler(lua_State* pLuaState);
int LAPI_UIRegisterEventHandler(lua_State* pLuaState);

int LAPI_UIEnableScaling(lua_State* pLuaState);
int LAPI_UIIsScalingEnabled(lua_State* pLuaState);
int LAPI_UIEnableToolTips(lua_State* pLuaState);
int LAPI_UIDisableToolTips(lua_State* pLuaState);
int LAPI_UIIsToolTipsEnabled(lua_State* pLuaState);
int LAPI_UIGetXScale(lua_State* pLuaState);
int LAPI_UIGetYScale(lua_State* pLuaState);
int LAPI_UISetToolTipTimeout(lua_State* pLuaState);
int LAPI_UIGetToolTipTimeout(lua_State* pLuaState);
int LAPI_UISetOpacity(lua_State* pLuaState);
int LAPI_UICreateCursor(lua_State* pLuaState);
int LAPI_UIDestroyCursor(lua_State* pLuaState);
int LAPI_UIGetCursorHotSpot(lua_State* pLuaState);
int LAPI_UIGetCursorRelXMov(lua_State* pLuaState);
int LAPI_UIGetCursorRelYMov(lua_State* pLuaState);
int LAPI_UISetCursorState(lua_State* pLuaState);
int LAPI_UIGetCursorState(lua_State* pLuaState);
int LAPI_UIShowCursor(lua_State* pLuaState);
int LAPI_UIIsCursorVisible(lua_State* pLuaState);
int LAPI_UILockCursorState(lua_State* pLuaState);
int LAPI_UIIsCursorStateLocked(lua_State* pLuaState);
int LAPI_UIEnableCursor(lua_State* pLuaState);
int LAPI_UIIsCursorEnabled(lua_State* pLuaState);


int LAPI_UICreateSprite(lua_State* pLuaState);
int LAPI_UICreateAnimSprite(lua_State* pLuaState);
int LAPI_UISetAnimSpriteBeginRow(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteBeginRow(lua_State* pLuaState);
int LAPI_UISetAnimSpriteEndRow(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteEndRow(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteCurrentRow(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteCurrentColumn(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteFrameWidth(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteFrameHeight(lua_State* pLuaState);
int LAPI_UISetAnimSpriteFrameDelay(lua_State* pLuaState);
int LAPI_UIGetAnimSpriteFrameDelay(lua_State* pLuaState);
int LAPI_UISetAnimSpritePlayCount(lua_State* pLuaState);
int LAPI_UIGetAnimSpritePlayCount(lua_State* pLuaState);
int LAPI_UIPlayAnimSprite(lua_State* pLuaState);
int LAPI_UIStopAnimSprite(lua_State* pLuaState);
int LAPI_UIIsAnimSpritePlaying(lua_State* pLuaState);



int LAPI_UICreateTextBox(lua_State* pLuaState);
int LAPI_UISetTextBoxText(lua_State* pLuaState);
int LAPI_UIAppendTextBoxText(lua_State* pLuaState);
int LAPI_UIRemoveTextBoxTextFromEnd(lua_State* pLuaState);
int LAPI_UISetTextBoxBufferSize(lua_State* pLuaState);
int LAPI_UIGetTextBoxBufferSize(lua_State* pLuaState);
int LAPI_UIGetTextBoxTextLength(lua_State* pLuaState);
int LAPI_UIGetTextBoxText(lua_State* pLuaState);
int LAPI_UISetTextBoxScrollStep(lua_State* pLuaState);
int LAPI_UIGetTextBoxScrollStep(lua_State* pLuaState);
int LAPI_UISetTextBoxReadOnlyMarker(lua_State* pLuaState);
int LAPI_UIGetTextBoxReadOnlyMarker(lua_State* pLuaState);
int LAPI_UISetTextBoxJustification(lua_State* pLuaState);
int LAPI_UIGetTextBoxJustification(lua_State* pLuaState);
int LAPI_UISetTextBoxCursorBlinkRate(lua_State* pLuaState);
int LAPI_UIGetTextBoxCursorBlinkRate(lua_State* pLuaState);
int LAPI_UIShowTextBoxCursor(lua_State* pLuaState);
int LAPI_UIClearTextBox(lua_State* pLuaState);
int LAPI_UIGetTextBoxLineHeight(lua_State* pLuaState);
int LAPI_UIGetNativeTextBoxHeight(lua_State* pLuaState);
int LAPI_UIGetNativeTextBoxWidth(lua_State* pLuaState);
int LAPI_UIGetNumTextBoxLinesVisible(lua_State* pLuaState);
int LAPI_UISetTextBoxCharSet(lua_State* pLuaState);
int LAPI_UIGetTextBoxCharSet(lua_State* pLuaState);
int LAPI_UIEnableTextBoxAutoScroll(lua_State* pLuaState);
int LAPI_UIIsTextBoxAutoScrollEnabled(lua_State* pLuaState);
int LAPI_UIEnableTextBoxWordWrapping(lua_State* pLuaState);
int LAPI_UIIsTextBoxWordWrappingEnabled(lua_State* pLuaState);
int LAPI_UIEnableTextBoxStickyHighlightMode(lua_State* pLuaState);
int LAPI_UIIsTextBoxStickyHighlightModeEnabled(lua_State* pLuaState);
int LAPI_UISetTextBoxEditable(lua_State* pLuaState);
int LAPI_UIIsTextBoxEditable(lua_State* pLuaState);
int LAPI_UISetTextBoxHoverIndex(lua_State* pLuaState);
int LAPI_UIGetTextBoxHoverIndex(lua_State* pLuaState);
int LAPI_UIGetScaledTextBoxRect(lua_State* pLuaState);

int LAPI_UICreateButton(lua_State* pLuaState);
int LAPI_UIIsButtonPressed(lua_State* pLuaState);
int LAPI_UIEnableButtonRepeatMode(lua_State* pLuaState);
int LAPI_UIIsButtonRepeatModeEnabled(lua_State* pLuaState);

int LAPI_UICreateToggle(lua_State* pLuaState);
int LAPI_UISetToggleState(lua_State* pLuaState);


int LAPI_UICreateMovie(lua_State* pLuaState);
int LAPI_UIPlayMovie(lua_State* pLuaState);
int LAPI_UIPauseMovie(lua_State* pLuaState);
int LAPI_UIResumeMovie(lua_State* pLuaState);
int LAPI_UIStopMovie(lua_State* pLuaState);
int LAPI_UISetMovieTime(lua_State* pLuaState);
int LAPI_UIGetMovieTime(lua_State* pLuaState);
int LAPI_UISetMoviePlayCount(lua_State* pLuaState);
int LAPI_UIGetMoviePlayCount(lua_State* pLuaState);
int LAPI_UIGetMovieDuration(lua_State* pLuaState);
int LAPI_UIIsMoviePlaying(lua_State* pLuaState);

int LAPI_UICreateVerticalSlider(lua_State* pLuaState);
int LAPI_UISetVerticalSliderPercentage(lua_State* pLuaState);
int LAPI_UIGetVerticalSliderPercentage(lua_State* pLuaState);
int LAPI_UIIsVerticalSliderSliding(lua_State* pLuaState);


int LAPI_UICreateHorizontalSlider(lua_State* pLuaState);
int LAPI_UISetHorizontalSliderPercentage(lua_State* pLuaState);
int LAPI_UIGetHorizontalSliderPercentage(lua_State* pLuaState);
int LAPI_UIIsHorizontalSliderSliding(lua_State* pLuaState);

int LAPI_UICreateHorizontalScrollbar(lua_State* pLuaState);
int LAPI_UISetHorizontalScrollbarStep(lua_State* pLuaState);
int LAPI_UIGetHorizontalScrollbarStep(lua_State* pLuaState);
int LAPI_UISetHorizontalScrollbarMaxSteps(lua_State* pLuaState);
int LAPI_UIGetHorizontalScrollbarMaxSteps(lua_State* pLuaState);
int LAPI_UIIncrementHorizontalScrollbarStep(lua_State* pLuaState);
int LAPI_UIDecrementHorizontalScrollbarStep(lua_State* pLuaState);


int LAPI_UICreateVerticalScrollbar(lua_State* pLuaState);
int LAPI_UISetVerticalScrollbarStep(lua_State* pLuaState);
int LAPI_UIGetVerticalScrollbarStep(lua_State* pLuaState);
int LAPI_UISetVerticalScrollbarMaxSteps(lua_State* pLuaState);
int LAPI_UIGetVerticalScrollbarMaxSteps(lua_State* pLuaState);
int LAPI_UIIncrementVerticalScrollbarStep(lua_State* pLuaState);
int LAPI_UIDecrementVerticalScrollbarStep(lua_State* pLuaState);

int LAPI_UICreateProgressBar(lua_State* pLuaState);
int LAPI_UIRenderProgressBarCoverFirst(lua_State* pLuaState);
int LAPI_UIIsProgressBarCoverFirst(lua_State* pLuaState);
int LAPI_UISetProgressBarPercentage(lua_State* pLuaState);
int LAPI_GetProgressBarPercentage(lua_State* pLuaState);

int LAPI_UICreateSpinner(lua_State* pLuaState);
int LAPI_UIGetSpinnerValue(lua_State* pLuaState);
int LAPI_UISetSpinnerValue(lua_State* pLuaState);
int LAPI_UIIncrementSpinnerStep(lua_State* pLuaState);
int LAPI_UIDecrementSpinnerStep(lua_State* pLuaState);
int LAPI_UISetSpinnerStepSize(lua_State* pLuaState);


int LAPI_UICreateComboBox(lua_State* pLuaState);
int LAPI_UIAddComboBoxItem(lua_State* pLuaState);
int LAPI_UIInsertComboBoxItem(lua_State* pLuaState);
int LAPI_UIFindComboBoxItem(lua_State* pLuaState);
int LAPI_UIRemoveComboBoxItem(lua_State* pLuaState);
int LAPI_UIGetComboBoxItem(lua_State* pLuaState);
int LAPI_UIGetSelectedComboBoxItem(lua_State* pLuaState);
int LAPI_UIGetNumComboBoxItems(lua_State* pLuaState);
int LAPI_UIClearComboBox(lua_State* pLuaState);
int LAPI_UIRefreshComboBox(lua_State* pLuaState);
int LAPI_UIShowComboBoxListBox(lua_State* pLuaState);
int LAPI_UIHideComboBoxListBox(lua_State* pLuaState);
int LAPI_UIIsComboBoxListBoxVisible(lua_State* pLuaState);
int LAPI_UISetComboBoxJustification(lua_State* pLuaState);
int LAPI_UIGetComboBoxJustification(lua_State* pLuaState);
int LAPI_UIEnableComboBoxSorting(lua_State* pLuaState);
int LAPI_UIDisableComboBoxSorting(lua_State* pLuaState);
int LAPI_UIIsComboBoxSortingEnabled(lua_State* pLuaState);
int LAPI_UIClearComboBoxSelectedItem(lua_State* pLuaState);



int LAPI_UICreateListBox(lua_State* pLuaState);
int LAPI_UIAddListBoxItem(lua_State* pLuaState);
int LAPI_UIInsertListBoxItem(lua_State* pLuaState);
int LAPI_UIRemoveListBoxItem(lua_State* pLuaState);
int LAPI_UIFindListBoxItem(lua_State* pLuaState);
int LAPI_UIGetListBoxItem(lua_State* pLuaState);
int LAPI_UIClearSelectedListBoxIndex(lua_State* pLuaState);
int LAPI_UIGetSelectedListBoxIndex(lua_State* pLuaState);
int LAPI_UIGetSelectedListBoxItem(lua_State* pLuaState);
int LAPI_UIGetNumListBoxItems(lua_State* pLuaState);
int LAPI_UIClearListBox(lua_State* pLuaState);
int LAPI_UIRefreshListBox(lua_State* pLuaState);
int LAPI_UIEnableListBoxStickyHighlightMode(lua_State* pLuaState);
int LAPI_UIIsListBoxStickyHighlightModeEnabled(lua_State* pLuaState);
int LAPI_UISetListBoxHoverIndex(lua_State* pLuaState);
int LAPI_UIGetListBoxHoverIndex(lua_State* pLuaState);
int LAPI_UISetListBoxScrollStep(lua_State* pLuaState);
int LAPI_UIGetListBoxScrollStep(lua_State* pLuaState);
int LAPI_UISetListBoxJustification(lua_State* pLuaState);
int LAPI_UIGetListBoxJustification(lua_State* pLuaState);
int LAPI_UIEnableListBoxSorting(lua_State* pLuaState);
int LAPI_UIDisableListBoxSorting(lua_State* pLuaState);
int LAPI_UIIsListBoxSortingEnabled(lua_State* pLuaState);




int LAPI_UICreateListView(lua_State* pLuaState);
int LAPI_UIAddListViewItem(lua_State* pLuaState);
int LAPI_UIInsertListViewItem(lua_State* pLuaState);
int LAPI_UIGetListViewItem(lua_State* pLuaState);
int LAPI_UIFindListViewItem(lua_State* pLuaState);
int LAPI_UIRemoveListViewItem(lua_State* pLuaState);
int LAPI_UISetListViewSubItem(lua_State* pLuaState);
int LAPI_UIGetListViewSubItem(lua_State* pLuaState);
int LAPI_UIClearSelectedListViewIndex(lua_State* pLuaState);
int LAPI_UIGetSelectedListViewIndex(lua_State* pLuaState);
int LAPI_UIGetNumListViewItems(lua_State* pLuaState);
int LAPI_UISetListViewColumnHeading(lua_State* pLuaState);
int LAPI_UIGetListViewColumnHeading(lua_State* pLuaState);
int LAPI_UISetListViewColumnJustification(lua_State* pLuaState);
int LAPI_UIGetListViewColumnJustification(lua_State* pLuaState);
int LAPI_UISetListViewColumnHeadingJustification(lua_State* pLuaState);
int LAPI_UIGetListViewColumnHeadingJustification(lua_State* pLuaState);
int LAPI_UISetListViewColumnHeadingToolTipText(lua_State* pLuaState);
int LAPI_UIGetListViewColumnHeadingToolTipText(lua_State* pLuaState);
int LAPI_UILockListViewColumnSizes(lua_State* pLuaState);
int LAPI_UIClearListView(lua_State* pLuaState);
int LAPI_UIRefreshListView(lua_State* pLuaState);

int LAPI_UICreateFrame(lua_State* pLuaState);
int LAPI_UIAddElementToFrame(lua_State* pLuaState);

int LAPI_UIDestroyElement(lua_State* pLuaState);
int LAPI_UISetElementZOrderOffset(lua_State* pLuaState);
int LAPI_UIGetElementZOrderOffset(lua_State* pLuaState);
int LAPI_UISetElementToolTipText(lua_State* pLuaState);
int LAPI_UIGetElementToolTipText(lua_State* pLuaState);
int LAPI_UIEnableElement(lua_State* pLuaState);
int LAPI_UIDisableElement(lua_State* pLuaState);
int LAPI_UIIsElementEnabled(lua_State* pLuaState);
int LAPI_UIShowElement(lua_State* pLuaState);
int LAPI_UIHideElement(lua_State* pLuaState);
int LAPI_UIIsElementVisible(lua_State* pLuaState);
int LAPI_UIElementHasFocus(lua_State* pLuaState);
int LAPI_UIIsPointInsideElement(lua_State* pLuaState);
int LAPI_UISetElementPointerState(lua_State* pLuaState);
int LAPI_UIGetElementPointerState(lua_State* pLuaState);
int LAPI_UISetElementOpacity(lua_State* pLuaState);
int LAPI_UIGetElementOpacity(lua_State* pLuaState);



#endif