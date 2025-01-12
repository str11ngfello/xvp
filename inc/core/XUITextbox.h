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
//	Module Name:	XUITextBox.h
//	Author:			Lynn Duke
//	Creation Date:	8-3-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_UI_TEXT_BOX_H__
#define __X_UI_TEXT_BOX_H__





//*****************************************************************************
//! Text box control that processes keyboard input.													  
/**
The XUITextBox object is used to specify a rectangular portion of the screen
where text can be rendered.  Text boxes can also process keyboard input.

Word wrapping is performed for multi-line textboxes. Text can be centered as
well as left or right justified.  A textbox must have the focus to receive
keyboard input and the correct filter flags must be set to accept certain types
of keys.  For example, you can filter out lower case, upper case, numbers,
punctuation, etc, by setting a combination of input filter flags for the text
box.  

See below for more details.
*/
//*****************************************************************************
class XUITextBox : public XUIElement
{
private:
	XHandle					m_charSetHandle;		//index of the charset for this text box
	XU32					m_maxChars;				//number of characters allowed in text box
	XU32					m_numCurrChars;			//number of chars currently in the buffer
	XU8					m_lineSpacing;			//number of pixels between lines including char height 
	XU8					m_charPadding;			//number of pixels to be skipped between chars
	XU8					m_spaceChar;			//number of pixels in a space character
	XS8*					m_pCharBuf;				//the one and only character buffer
	XU8					m_type;					//type flags describing what chars are acceptable
	X_TB_JUST_TYPE			m_justification;		//justification type - left, right, center
	XF32					m_xScale;				//scaling factor in the x direction
	XF32					m_yScale;				//scaling factor in the y direction
	vector<XCharCoords*>		m_lineOfText;			//used to hold a string of chars
	XU32					m_currentCharIndex;		//used to hold current string position
	XS32					m_cachedCharIndex;		//used to speed up scrolling text boxes
	XU32					m_currentLineWidth;		//used to hold width of current line
	XRect					m_srcRect;				//used to hold src image
	XRect					m_destRect;				//used to specify destination screen position
	XBOOL					m_cursorLoaded;			//determines whether or not a cursor sprite is loaded
	XSprite					m_cursor;				//represents the insertion point
	XU32					m_nativeCursorWidth;	//cursor width in pixels
	XU32					m_nativeCursorHeight;	//cursor height in pixels
	XS32					m_cursorXPos;			//cursor x position
	XS32					m_cursorYPos;			//cursor y position
	XTimer					m_blinkTimer;			//controls cursor blink rate
	XBOOL					m_blinkOn;				//controls cursor blink states
	XF32					m_cursorBlinkRate;		//cursor blink rate
	XBOOL					m_showCursor;			//determines if the cursor is visible
	XU32					m_scrollStep;			//The line number to begin drawing from
	XU32					m_readOnlyMarker;		//Everything before this position is read only
	XBOOL					m_autoScroll;			//Automatically scroll text when needed
	XUISprite				m_bgSprite;				//Optional background sprite
	XU32					m_textPadding;			//Padding between text and background image
	XRect					m_textRect;				//Rect used for drawing text on bg image
	XHandle					m_highlightCharSet;		//CharSet used for hightlighting
	XS32					m_hoverIndex;			//The line index the mouse if hovering over (-1 if not hovering)
	XHandle					m_highlightTexture;		//Texture handle for optional line highlight
	XBOOL					m_stickyHighlightMode;	//Highlight mode (true for sticky, false for hover)
	XBOOL					m_wordWrapEnabled;		//Determines if word wrap mode is enabled/disabled
	XBOOL					m_editable;				//Can textbox receive keyboard input
	XBOOL					m_hasBGSprite;			//Determines if the text box has a background sprite.
	XU16					m_scaledSpaceChar;		//Space pixels scaled
	XS32					m_numHardLineBreaks;	//Line breaks due to '\n'

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus renderCursor();
	XStatus justifyCursor();
	XBOOL getNextLine(XU32& numCharsSkipped);

public:

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XUITextBox();
	virtual ~XUITextBox();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,XS8* pTemplateFile);
	XStatus destroy();
	XStatus appendText(XS8* pCharsToAdd);
	XStatus removeTextFromEnd(XU32 numCharsToRemove);
	XStatus setTextBufferSize(XU32 numChars);
	XU32 getTextBufferSize();
	XStatus setText(XS8* pBuffer);
	XU32 getTextLength();
	XS8* getText();
	XStatus setType(XU8 flags);
	XU8 getType();
	XStatus removeType(XU8 flags);
	XStatus setScrollStep(XU32 step);
	XU32 getScrollStep();
	XStatus setReadOnlyMarker(XU32 charPosition);
	XU32 getReadOnlyMarker();
	XStatus setJustification(X_TB_JUST_TYPE justification);
	X_TB_JUST_TYPE getJustification();
	XStatus setCursorBlinkRate(XF32 blinkRate);
	XF32 getCursorBlinkRate();
	XStatus showCursor(XBOOL show);
	XStatus clear();
	XU32 getLineHeight();
	XU32 getNativeTextBoxHeight();
	XU32 getLongestLineInNativePixels();
	XU32 getNativeTextBoxWidth();
	XU32 getNumHardLineBreaks();
	XStatus setNativePosition(XVector2D& position);
	XU32 getNumLinesVisible();
	XStatus setCharSet(XHandle charSetHandle);
	XHandle getCharSet();
	XStatus enableAutoScroll(XBOOL enable);
	XBOOL isAutoScrollEnabled();
	XStatus enableWordWrapping(XBOOL enable);
	XBOOL isWordWrappingEnabled();
	XStatus enableStickyHighlightMode(XBOOL enable);
	XBOOL isStickyHighlightModeEnabled();
	XStatus setEditable(XBOOL editable);
	XBOOL isEditable();
	XStatus setZOrderOffset(XU32 zOrder);
	XStatus enable();
	XStatus disable();
	XStatus setOpacity(XF32 opacity);
	XStatus move(XVector2D& moveVector);
	XStatus show();
	XStatus hide();
	XStatus setHoverIndex(XS32 index);
	XS32 getHoverIndex();
	XStatus setTextPadding(XU32 textPadding);
	XU32 getTextPadding();
	//XBOOL isPointInside(XVector2D& point);
	XStatus getScaledTextBoxRect(XRect& rect);
	XStatus setNativeRect(XRect& rect);
	XStatus getScaledRect(XRect& rect);
	virtual XStatus render();
	virtual XStatus update(XF32 dt);
	XStatus onResize(XF32 xScale,XF32 yScale);
	virtual XStatus onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement);
	virtual XStatus onMouseLeave(XUIElement* pChildElement);
	virtual XStatus onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,
								XUIElement* pChildElement);

};

#endif