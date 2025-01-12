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
//	Module Name:	x_textbox.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-3-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
/**
Constructor for the text box.  Initializes the text box properties to
the following default values:<br><br>

<ul>
<li> Character Buffer = NULL (empty) </li>
<li> Max Characters = 0 </li>
<li> Line Spacing = 0 </li>
<li> Character Padding = 0 </li>
<li> Character Set Index = 0 </li>
<li> Type = X_TBT_ALL </li>
<li> Space Pixels = 0 </li>
</ul>
*///*****************************************************************************
XUITextBox::XUITextBox()
{
	m_pCharBuf = NULL;
	m_maxChars = 0;		
	m_lineSpacing = 0;	
	m_charPadding = 0;
	m_charSetHandle = X_INVALID_HANDLE;
	m_numCurrChars = 0;
	m_type = X_TBT_ALL;
	m_spaceChar = 0;
	m_xScale = 0;
	m_yScale = 0;
	m_currentLineWidth = 0;
	m_currentCharIndex = 0;
	m_justification = X_TBJ_LEFT;
	m_cursorLoaded = false;		
	m_nativeCursorWidth = 0;
	m_nativeCursorHeight = 0;
	m_cursorXPos = 0;
	m_cursorYPos = 0;
	m_blinkOn = true;
	m_cursorBlinkRate = .5;
	m_showCursor = false;
	m_scrollStep = 0;
	m_readOnlyMarker = 0;
	m_autoScroll = false;
	m_textPadding = 0;
	m_highlightCharSet = X_INVALID_HANDLE;
	m_hoverIndex = -1;
	m_highlightTexture = X_INVALID_HANDLE;
	m_cachedCharIndex = -1;
	m_stickyHighlightMode = false;
	m_wordWrapEnabled = true;
	m_editable = false;
	m_hasBGSprite = false;
	m_scaledSpaceChar = 0;
	m_numHardLineBreaks = -1;
}



//*****************************************************************************
/**
Destructor for the text box.  
*/
//*****************************************************************************
XUITextBox::~XUITextBox()
{
}



//*****************************************************************************
/**
Initializes an XUITextBox object according to the passed in parameters.  This
function must be called before the text box can be used.

@param pParentUI	The user interface to which this text box belongs.
@param ID	The element ID used to identify this user interface text box 
			within a user interface.
@param zOrder	The zOrder of the user interface text box with respect to other
				user interface elements. 
@param desc	A text box description structure.
@param pCursorTexFilename	The texture file to use for the cursor image.
@param cursorWidth	The width of the cursor.
@param cursorHeight	The height of the cursor.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,XS8* pTemplateFile)
{
	//Validate params
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
		TiXmlElement* pTextboxElement = NULL;
		
		pNode = file.FirstChild( "textbox" );
		if (pNode)
		{
			pTextboxElement = pNode->ToElement();
			if (pTextboxElement)
			{	
				XS8* pFilename = NULL;
				XS8* pFilename2 = NULL;
				XS8* pFilename3 = NULL;
				XS8* pFilename4 = NULL;
				XS8* pFilename5 = NULL;
				XS8* pFilename6 = NULL;
				XS32 intValue = 0;
				XF64 floatValue = 0;
				pTextboxElement->QueryIntAttribute("MaxChars",&intValue);
				m_maxChars = intValue;
				pTextboxElement->QueryIntAttribute("CharPadding",&intValue);
				m_charPadding = intValue;
				pTextboxElement->QueryIntAttribute("Justification",&intValue);
				m_justification = (X_TB_JUST_TYPE)intValue;
				pTextboxElement->QueryIntAttribute("ShowCursor",&intValue);
				m_showCursor = intValue;
				pTextboxElement->QueryIntAttribute("CursorWidth",&intValue);
				m_nativeCursorWidth = intValue;
				pTextboxElement->QueryDoubleAttribute("CursorBlinkRate",&floatValue);
				m_cursorBlinkRate = (float)floatValue;
				pTextboxElement->QueryIntAttribute("TextBoxPadding",&intValue);
				m_textPadding = intValue;
				pTextboxElement->QueryIntAttribute("Editable",&intValue);
				m_editable = (XBOOL)intValue;
				
				//Setup the rect for text drawing
				m_textRect = screenRect;
				m_textRect.left += m_textPadding;
				m_textRect.top += m_textPadding;
				m_textRect.right -= m_textPadding;
				m_textRect.bottom -= m_textPadding;

				pFilename = (XS8*)pTextboxElement->Attribute("CharSetFile");
				pFilename2 = (XS8*)pTextboxElement->Attribute("CharSetDataFile");
				if (pFilename && pFilename2)
				{
					if (XVP::createCharSet(pFilename,pFilename2,m_charSetHandle) != X_OK)
						return X_ERR_OPERATION_FAILED;
					XCharSet* pCS = XVP::getCharSetInterface(m_charSetHandle);
					if (pCS)
					{
						XS32 lineHeightBias = 0;
						pTextboxElement->QueryIntAttribute("LineHeightBias",&lineHeightBias);
						m_lineSpacing = pCS->getLineHeight() + lineHeightBias;
						m_spaceChar = pCS->getSpaceWidth();
					}
					else
						return X_ERR_OPERATION_FAILED;
				}
				else
					return X_ERR_OPERATION_FAILED;

				pFilename3 = (XS8*)pTextboxElement->Attribute("BGSpriteTemplateFile");
				if (pFilename3)
				{
					if (m_bgSprite.init(pParentUI,zOrder,screenRect,pFilename3) == X_OK)
						m_hasBGSprite = true;
				}

				pFilename4 = (XS8*)pTextboxElement->Attribute("HighlightCharSetFile");
				if (pFilename4)
				{
					XVP::createCharSet(pFilename4,pFilename2,m_highlightCharSet);
				}

				pFilename5 = (XS8*)pTextboxElement->Attribute("HighlightTextureFile");
				if (pFilename5)
				{
					XVP::GraphicsMgr.createTextureFromFile(pFilename5,m_highlightTexture);
				}
				pFilename6 = (XS8*)pTextboxElement->Attribute("CursorTextureFile");
				if (pFilename6)
				{
					if (m_cursor.init(pFilename6) == X_OK)
					{
						m_cursorLoaded = true;
						m_blinkTimer.start();
						m_nativeCursorHeight = m_lineSpacing;
					}
				}

			}
			else
				return X_ERR_OPERATION_FAILED;
		}
		else
			return X_ERR_OPERATION_FAILED;


	/*if (pCursorTexFilename)
	{
		if (m_cursor.init(pCursorTexFilename) == X_OK)
		{
			m_cursorLoaded = true;
			m_blinkTimer.start();
			m_nativeCursorWidth = cursorWidth;
			m_nativeCursorHeight = cursorHeight;
		}
		else
			return X_ERR_OPERATION_FAILED;
	}*/
	
		//Now we need to allocate the buffer that will be used to hold the 
		//characters that belong to this text box.  We use strcat to add
		//to the text buffer so place a null character in it by default.
		XNewArray(m_pCharBuf,XS8,m_maxChars+1,XOSMgr::getDefaultHeap());
		m_pCharBuf[0] = '\0';

		m_cachedCharIndex = -1;
		m_numHardLineBreaks = -1;
		m_nativeScreenRect = screenRect;
		m_currentCharIndex = 0;
		m_numCurrChars = 0;
		m_focus = false;
		m_zOrderOffset = zOrder+1; //Add 1 so text will appear on top of background
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder+1);

		return X_OK;

	}
}



//*****************************************************************************
/**
Destroys the user interface text box by freeing all of its allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::destroy()
{
	m_bgSprite.destroy();
	m_cursor.destroy();

	//Get rid of the character buffer
	XDeleteArray(m_pCharBuf);

	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}



//*****************************************************************************
/**
Appends a buffer of characters onto the existing text box buffer if an only
if the operation will not cause the total characters in the text box to exceed
the maximum number of characters allowed.

@param pCharsToAdd	The buffer of characters to append.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::appendText(XS8* pCharsToAdd)
{
	//We need to first check that the number of characters being added
	//will not exceed the buffer size
	XU32 numCharsToAdd = strlen(pCharsToAdd);
	if ((numCharsToAdd + m_numCurrChars) <= m_maxChars) 
	{
		strcat(m_pCharBuf,pCharsToAdd);
		m_numCurrChars += numCharsToAdd;
		m_cachedCharIndex = -1;
		m_numHardLineBreaks = -1;
		return X_OK;
	}
	else 
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Removes a specified number of characters from the end of the text box buffer.

@param numCharsToRemove	The number of chars to remove from the end of the text
						box buffer.  If the amount of characters in the text box
						buffer is less that the number to remove, the operation
						will fail.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::removeTextFromEnd(XU32 numCharsToRemove)
{
	//Make sure that there are enough chars to remove
	if (numCharsToRemove > strlen(m_pCharBuf))
		return X_ERR_OPERATION_FAILED;

	if (numCharsToRemove == 0)
		return X_OK;
	
	//Okay, now perform the removal
	m_pCharBuf[strlen(m_pCharBuf) - numCharsToRemove] = '\0';	
	m_numCurrChars -= numCharsToRemove;
	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;

	return X_OK;
}



//*****************************************************************************
/**
Clears the text box buffer by removing all characters.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::clear()
{
	//Clear the entire text buffer if it is valid
	if (m_pCharBuf)
		m_pCharBuf[0] = '\0';

	//Reset the number of chars and scroll back to the top
	m_numCurrChars = 0;
	m_scrollStep = 0;
	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;

	return X_OK;
}



//*****************************************************************************
/**
Gets a character pointer to the text box buffer.

@return A pointer to the text box buffer of characters.
*/
//*****************************************************************************
XS8* XUITextBox::getText()
{
	return m_pCharBuf;
}



//*****************************************************************************
/**
Gets the number of characters in the text box buffer.

@return The number of characters in the text box buffer.
*/
//*****************************************************************************
XU32 XUITextBox::getTextLength()
{
	return m_numCurrChars;
}



//*****************************************************************************
/**
Sets the size of the text box buffer by specifying the maximum number of
characters the text box can hold. This is a destructive operation.  It destroys
the current text box buffer and allocates a new one, however it will try to
maintain the contents of the text box according to the new size.

@param numChars The new size of the text box character buffer.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::setTextBufferSize(XU32 numChars)
{
	//Store the current buffer for later.
	XS8* pTempBuffer = m_pCharBuf;

	//Now we need to allocate the buffer that will be used to hold the 
	//characters that belong to this text box
	XNewArray(m_pCharBuf,XS8,numChars+1,XOSMgr::getDefaultHeap());
	m_pCharBuf[0] = '\0';
	m_maxChars = numChars;	
	m_numCurrChars = 0;
	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;

	if (pTempBuffer)
	{
		setText(pTempBuffer);
		XDeleteArray(pTempBuffer);
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getTextBufferSize()
{
	return m_maxChars;
}



//*****************************************************************************
/**
Sets the contents of the text box character buffer equal to the passed in 
character buffer.  This is a destructive operation. The current text buffer will
be lost and set to equal the buffer passed in.

@param buffer	The buffer of characters to set the text box buffer equal to.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::setText(XS8* pBuffer)
{
	//Validate parameters
	if (!pBuffer)
		return X_ERR_INVALID_PARAMS;

	//Reset the buffer
	m_pCharBuf[0] = '\0';

	XU32 length = strlen(pBuffer);
	//This is a destructive operation. The current text buffer will be
	//lost and set to equal the buffer passed in.
	if (length <= m_maxChars)
	{
		strcpy(m_pCharBuf,pBuffer);
		m_numCurrChars = strlen(pBuffer);
	}
	else
	{
		strncpy(m_pCharBuf,pBuffer,m_maxChars);
		m_numCurrChars = m_maxChars;
	}

	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;

	return X_OK;
}



//*****************************************************************************
/**
Sets filter flags that specify what type of characters the text box can
accept from keyboard input.

@param flags	Flags that specify what type of characters can be accepted by
				the text box.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::setType(XU8 flags)
{
	m_type |= flags;
	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;
	return X_OK;
}



//*****************************************************************************
/**
Gets the type flags of the text box.  The type flags determine the type of
characters the text box can accept.

@return The text box type flags.
*/
//*****************************************************************************
XU8 XUITextBox::getType()
{
	return m_type;
}



//*****************************************************************************
/**
Sets the justification property of the textbox.

@param justification	The justification type.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::setJustification(X_TB_JUST_TYPE justification)
{
	m_justification = justification;
	m_cachedCharIndex = -1;

	return X_OK;
}



//*****************************************************************************
/**
Gets the justification type of the text box.  

@return The text box justification type.
*/
//*****************************************************************************
X_TB_JUST_TYPE XUITextBox::getJustification()
{
	return m_justification;
}



//*****************************************************************************
/**
Sets the cursor blink rate of the textbox.

@param blinkRate	This rate at which the cursor blinks.  For example, if this
					value is set to 1, the cursor will blink every 1 second.  If
					set to 2.5, the cursor will blink every 2.5 seconds.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::setCursorBlinkRate(XF32 blinkRate)
{
	if (blinkRate >= 0)
		m_cursorBlinkRate = blinkRate;
	return X_OK;
}



//*****************************************************************************
/**
Gets the cursor blink rate of the textbox.

@return	This rate at which the cursor blinks.  For example, if this
		value is set to 1, the cursor will blink every 1 second.  If
		set to 2.5, the cursor will blink every 2.5 seconds.
*/
//*****************************************************************************
XF32 XUITextBox::getCursorBlinkRate()
{
	return m_cursorBlinkRate;
}



//*****************************************************************************
/**
Removes type flags that specify what type of characters the text box can
accept from keyboard input.

@param flags	Character types to remove.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::removeType(XU8 flags)
{
	m_type &= ~flags;
	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setScrollStep(XU32 step)
{
	//If we're at the step already, don't do anything.
	if (step == m_scrollStep)
		return X_OK;

	m_scrollStep = step;
	m_cachedCharIndex = -1;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getScrollStep()
{
	return m_scrollStep;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setReadOnlyMarker(XU32 charPosition)
{
	m_readOnlyMarker = charPosition;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getReadOnlyMarker()
{
	return m_readOnlyMarker;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getLineHeight()
{
	return m_lineSpacing;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getNativeTextBoxHeight()
{
	return m_textRect.bottom - m_textRect.top;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getNativeTextBoxWidth()
{
	return m_textRect.right - m_textRect.left;
}




//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setNativePosition(XVector2D& position)
{
	XU32 width = m_textRect.right - m_textRect.left;
	XU32 height = m_textRect.bottom - m_textRect.top;
	m_textRect.left = (XS32)(position.x + m_textPadding);
	m_textRect.top =(XS32)(position.y + m_textPadding);
	m_textRect.right = m_textRect.left + width;
	m_textRect.bottom = m_textRect.top + height;

	if (m_hasBGSprite)
		m_bgSprite.setNativePosition(position);

	//Call the normal positioning function last since it may call events.
	return XUIElement::setNativePosition(position);
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setNativeRect(XRect& rect)
{
	m_textRect.left = (XS32)(rect.left + m_textPadding);
	m_textRect.top =(XS32)(rect.top + m_textPadding);
	m_textRect.right = rect.right - m_textPadding;
	m_textRect.bottom = rect.bottom - m_textPadding;

	if (m_hasBGSprite)
		m_bgSprite.setNativeRect(rect);

	//Call the normal positioning function last since it may call events.
	return XUIElement::setNativeRect(rect);
}


//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::getScaledRect(XRect& rect)
{
	rect = m_scaledScreenRect;//m_bgSprite.getScaledRect(rect);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getNumLinesVisible()
{
	return (XU32)(getNativeTextBoxHeight()/getLineHeight());
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::getScaledTextBoxRect(XRect& rect)
{
	//Return a scaled text box rect.  This is not the rect used for the background
	//sprite, but rather the box used to contain text
	rect = m_textRect;
	rect.left = (XS32)(m_textRect.left * m_xScale);
	rect.right = (XS32)(m_textRect.right * m_xScale);
	rect.top = (XS32)(m_textRect.top * m_yScale);
	rect.bottom = (XS32)(m_textRect.bottom * m_yScale);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setCharSet(XHandle charSetHandle)
{
	m_charSetHandle = charSetHandle;
	m_cachedCharIndex = -1;
	m_numHardLineBreaks = -1;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XHandle XUITextBox::getCharSet()
{
	return m_charSetHandle;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::enableAutoScroll(XBOOL enable)
{
	m_autoScroll = enable;
	m_cachedCharIndex = -1;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUITextBox::isAutoScrollEnabled()
{
	return m_autoScroll;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::enableWordWrapping(XBOOL enable)
{
	m_wordWrapEnabled = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUITextBox::isWordWrappingEnabled()
{
	return m_wordWrapEnabled;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::enableStickyHighlightMode(XBOOL enable)
{
	m_stickyHighlightMode = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUITextBox::isStickyHighlightModeEnabled()
{
	return m_stickyHighlightMode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setEditable(XBOOL editable)
{
	m_editable = editable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUITextBox::isEditable()
{
	return m_editable;
}




//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setZOrderOffset(XU32 zOrder)
{
	if (m_hasBGSprite)
		m_bgSprite.setZOrderOffset(zOrder);
	XUIElement::setZOrderOffset(zOrder+1);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::enable()
{
	if (m_hasBGSprite)
		m_bgSprite.enable();
	XUIElement::enable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::disable()
{
	if (m_hasBGSprite)
		m_bgSprite.disable();
	XUIElement::disable();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setOpacity(XF32 opacity)
{
	if (m_hasBGSprite)
		m_bgSprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::move(XVector2D& moveVector)
{
	if (m_hasBGSprite)
	{
		XRect r;
		m_bgSprite.getScaledRect(r);
		XVector2D v = XVector2D((XF32)r.left + moveVector.x,(XF32)r.top + moveVector.y);
		m_bgSprite.setScaledPosition(v);

		m_nativeScreenRect.left += (XU32)moveVector.x;
		m_nativeScreenRect.right += (XU32)moveVector.x;
		m_nativeScreenRect.top += (XU32)moveVector.y;
		m_nativeScreenRect.bottom += (XU32)moveVector.y;

		onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
/*XBOOL XUITextBox::isPointInside(XVector2D& point)
{
	if (m_hasBGSprite)
	{
		return (m_bgSprite.isPointInside(point) ||
				XUIElement::isPointInside(point));
	}
	else
	{
		XUIElement
	}
}*/


//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::show()
{
	if (m_hasBGSprite)
		m_bgSprite.show();
	XUIElement::show();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::hide()
{
	if (m_hasBGSprite)
		m_bgSprite.hide();
	XUIElement::hide();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setHoverIndex(XS32 index)
{
	m_hoverIndex = index;
	if (m_hoverIndex > (getNumLinesVisible() - 1))
		m_hoverIndex = -1;

	return X_OK;
}
	


//*****************************************************************************
/**
Displays or hides the cursor.

@param show	True to show the cursor, false to hide it.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::showCursor(XBOOL show)
{
	m_showCursor = show;
	return X_OK;
}



//*****************************************************************************
/**
Justifies the cursor according to the justification setting of the 
textbox.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::justifyCursor()
{
	switch (m_justification)
	{
	case X_TBJ_LEFT:
		m_cursorXPos = (int)(m_textRect.left * m_xScale);
		break;
	case X_TBJ_CENTER:
		m_cursorXPos = (int)((((m_textRect.right - m_textRect.left)/2) + m_textRect.left) * m_xScale);
		break;
	case X_TBJ_RIGHT:
		m_cursorXPos = (int)(m_textRect.right  * m_xScale);
		break;
	default:
		break;
	}

	return X_OK;
}



//*****************************************************************************
/**
Renders the cursor in the appropriate location.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::renderCursor()
{
	//If the cursor is loaded, and the text box has focus, set the rect and render it.
	if ((m_cursorLoaded) && (m_focus))
	{	
		if (m_blinkTimer.getDuration() >= m_cursorBlinkRate)
		{
			m_blinkTimer.reset();
			m_blinkOn = !m_blinkOn;
		}

		if ((m_blinkOn) && (m_showCursor))
		{
			XRect r;
			r.left = m_cursorXPos;
			r.top = m_cursorYPos;
			r.right = r.left + (int)(m_nativeCursorWidth * m_xScale);
			r.bottom = r.top + (int)(m_nativeCursorHeight * m_yScale);
			m_cursor.setDestRect(r);
			m_cursor.render();
		}
	}

	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface textbox.  

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::render()
{
	if (!m_visible)
		return X_OK;

	if (m_cachedCharIndex == -1)
		m_currentCharIndex = 0;
	else
		m_currentCharIndex = m_cachedCharIndex;

	int insertXPosOffset = 0;
	int insertYPosOffset = 0;
	XCharSet* cs = NULL;	
	XCharSet* pHCS = NULL;
	XU32 i = 0;
	XU32 scaledLineSpacing = (XU32)(m_lineSpacing * m_yScale);
	XBOOL reachedBounds = false;
	XU32 currentStep = 0;
	XU32 relativeLineNum = 0;
	XBOOL textureSwapped = false;
	XBOOL setCache = true;
	XU32 numCharsSkipped = 0;

	//Make sure there are chars to render.  If there aren't just render
	//the cursor if needed.
	if (m_numCurrChars <= 0)
	{	
		justifyCursor();
		m_cursorYPos = (int)(m_textRect.top * m_yScale);
		renderCursor();
		return X_OK;
	}
	
	//First grab a pointer to this text box's charset and highlight charset
	//if applicable
	cs = XVP::getCharSetInterface(m_charSetHandle);
	if (!cs)
		return X_ERR_OPERATION_FAILED;
	if (m_highlightCharSet != X_INVALID_HANDLE)
	{
		pHCS = XVP::getCharSetInterface(m_highlightCharSet);
		if (!pHCS)
			return X_ERR_OPERATION_FAILED;
	}

	//Note that the regular charset and highlight charset dimensions
	//must match as well as the font metrics
	XVP::GraphicsMgr.setTexture(cs->m_texHandle,0);
	XU32 texWidth = XVP::GraphicsMgr.getTextureWidth(cs->m_texHandle);
	XU32 texHeight = XVP::GraphicsMgr.getTextureHeight(cs->m_texHandle);

	//While there are still lines to process.
	while (getNextLine(numCharsSkipped))
	{
		if (m_cachedCharIndex == -1)
		{
			if (currentStep++ < m_scrollStep)
			{
				m_lineOfText.clear();
				continue;
			}
		}
		
		/*if (setCache)
		{
			m_cachedCharIndex = m_currentCharIndex - (m_lineOfText.size() + numCharsSkipped) - 1;
			setCache = false;
		}*/

		//Highlight this line if appropriate
		if (textureSwapped)
		{
			if (relativeLineNum == m_hoverIndex+1)
				XVP::GraphicsMgr.setTexture(cs->m_texHandle,0);
			textureSwapped = false;
		}
		if (relativeLineNum == m_hoverIndex)
		{
			
			if (m_highlightTexture != X_INVALID_HANDLE)
			{
				XU32 w,h;
				w = XVP::GraphicsMgr.getTextureWidth(m_highlightTexture);
				h = XVP::GraphicsMgr.getTextureHeight(m_highlightTexture);
				XRect srcRect(0,0,w,h);
				XRect destRect((XS32)(m_textRect.left * m_xScale),
							  (XS32)((insertYPosOffset + m_textRect.top) * m_yScale),
							  (XS32)(m_textRect.right * m_xScale),
							  (XS32)(((insertYPosOffset + m_textRect.top) * m_yScale) + ((float)getLineHeight() * m_yScale)));
				XVP::GraphicsMgr.setTexture(m_highlightTexture,0);
				
				XVP::GraphicsMgr.renderTexture(w,h,srcRect,destRect,m_opacity);
				XVP::GraphicsMgr.setTexture(cs->m_texHandle,0);
			}

			if ((m_highlightCharSet != X_INVALID_HANDLE) && (pHCS))
			{
				XVP::GraphicsMgr.setTexture(pHCS->m_texHandle,0);
				textureSwapped = true;
			}
		}
		
		
			
		switch (m_justification)
		{
		case X_TBJ_LEFT:
			insertXPosOffset = 0;
			break;
		case X_TBJ_CENTER:
			insertXPosOffset = ((m_textRect.right - m_textRect.left) - m_currentLineWidth)/2;
			break;
		case X_TBJ_RIGHT:
			insertXPosOffset = ((m_textRect.right - m_textRect.left) - m_currentLineWidth);
			break;
		default:
			break;
		}
	
		for (i = 0;i < m_lineOfText.size();++i)
		{
			//If the character is not a space...
			if (m_lineOfText[i] != NULL)
			{
				m_srcRect.left = m_lineOfText[i]->m_xPos;
				m_srcRect.top = m_lineOfText[i]->m_yPos;
				m_srcRect.right = m_srcRect.left + m_lineOfText[i]->m_width;
				m_srcRect.bottom = m_srcRect.top + m_lineOfText[i]->m_height;

				m_destRect.left = (int)((insertXPosOffset + m_textRect.left) * m_xScale);
				m_destRect.top = (int)((insertYPosOffset + m_textRect.top) * m_yScale);
				m_destRect.right = (int)(((insertXPosOffset + m_textRect.left) + 
										(m_srcRect.right-m_srcRect.left)) * m_xScale);
				m_destRect.bottom = (int)(((insertYPosOffset + m_textRect.top) + 
										(m_srcRect.bottom-m_srcRect.top)) * m_yScale);

				//Render any non zero width character
				if ((m_srcRect.right - m_srcRect.left) != 0)
				{
					//XVP::GraphicsMgr.setMinFilter(0,X_TFT_NONE);
				//XVP::GraphicsMgr.setMagFilter(0,X_TFT_NONE);
				//XVP::GraphicsMgr.setMipFilter(0,X_TFT_NONE);
					XVP::GraphicsMgr.renderTexture(texWidth,texHeight,m_srcRect,m_destRect,m_opacity);
				}
					
				insertXPosOffset += m_lineOfText[i]->m_width + m_charPadding;
				}
				else
					insertXPosOffset += m_scaledSpaceChar;
			}
	
		//Clear the line of text and move to the next line.  Also, upate the
		//cursor position in case this is the last char in the text box.
		m_cursorXPos = (int)((insertXPosOffset + m_textRect.left) * m_xScale);
		m_cursorYPos = (int)((insertYPosOffset + m_textRect.top) * m_yScale);
		
		m_lineOfText.clear();
		insertYPosOffset += m_lineSpacing;//scaledLineSpacing;

		//If we've reached the bottom of the text box, quit.
		if ((m_textRect.top + insertYPosOffset + m_lineSpacing) > m_textRect.bottom)
		{
			if (m_autoScroll)
			{
				++m_scrollStep;
				m_cachedCharIndex = -1;
			}
			reachedBounds = true;
			break;
		}
		
		++relativeLineNum;
	}

	//Render the cursor.  Adjust for newline if necessary.
	if ((m_pCharBuf[m_numCurrChars-1] == '\n') && (!reachedBounds))
	{
		justifyCursor();
        m_cursorYPos += scaledLineSpacing;
	}
	renderCursor();

	return X_OK;
}	



//*****************************************************************************
/**
Gets the next line of text from the text box.

@return True if a line was received, false othewise.
*/
//*****************************************************************************
XBOOL XUITextBox::getNextLine(XU32& numCharsSkipped)
{
	XCharSet* cs = NULL;			
	XU32 insertXPosOffset = 0;		
	XU32 tempXOffset = 0;			
	XBOOL containsASpace = false;	
	numCharsSkipped = 0;

	//Make sure we aren't at the end of the char buffer
	if (m_currentCharIndex >= (m_numCurrChars))
		return false;

	//First grab a pointer to this text box's charset
	cs = XVP::getCharSetInterface(m_charSetHandle);
	if (!cs)
		return false;

	while (true)
	{
		switch (m_pCharBuf[m_currentCharIndex])
		{
		case 'a':
			m_lineOfText.push_back(&cs->m_a);
			tempXOffset = cs->m_a.m_width + m_charPadding; 
			break;
		case 'b':
			m_lineOfText.push_back(&cs->m_b);
			tempXOffset = cs->m_b.m_width + m_charPadding;
			break;
		case 'c':
			m_lineOfText.push_back(&cs->m_c);
			tempXOffset = cs->m_c.m_width + m_charPadding;
			break;
		case 'd':
			m_lineOfText.push_back(&cs->m_d);
			tempXOffset = cs->m_d.m_width + m_charPadding;
			break;
		case 'e':
			m_lineOfText.push_back(&cs->m_e);
			tempXOffset = cs->m_e.m_width + m_charPadding;
			break;
		case 'f':
			m_lineOfText.push_back(&cs->m_f);
			tempXOffset = cs->m_f.m_width + m_charPadding;
			break;
		case 'g':
			m_lineOfText.push_back(&cs->m_g);
			tempXOffset = cs->m_g.m_width + m_charPadding;
			break;
		case 'h':
			m_lineOfText.push_back(&cs->m_h);
			tempXOffset = cs->m_h.m_width + m_charPadding;
			break;
		case 'i':
			m_lineOfText.push_back(&cs->m_i);
			tempXOffset = cs->m_i.m_width + m_charPadding;
			break;
		case 'j':
			m_lineOfText.push_back(&cs->m_j);
			tempXOffset = cs->m_j.m_width + m_charPadding;
			break;
		case 'k':
			m_lineOfText.push_back(&cs->m_k);
			tempXOffset = cs->m_k.m_width + m_charPadding;
			break;
		case 'l':
			m_lineOfText.push_back(&cs->m_l);
			tempXOffset = cs->m_l.m_width + m_charPadding;
			break;
		case 'm':
			m_lineOfText.push_back(&cs->m_m);
			tempXOffset = cs->m_m.m_width + m_charPadding;
			break;
		case 'n':
			m_lineOfText.push_back(&cs->m_n);
			tempXOffset = cs->m_n.m_width + m_charPadding;
			break;
		case 'o':
			m_lineOfText.push_back(&cs->m_o);
			tempXOffset = cs->m_o.m_width + m_charPadding;
			break;
		case 'p':
			m_lineOfText.push_back(&cs->m_p);
			tempXOffset = cs->m_p.m_width + m_charPadding;
			break;
		case 'q':
			m_lineOfText.push_back(&cs->m_q);
			tempXOffset = cs->m_q.m_width + m_charPadding;
			break;
		case 'r':
			m_lineOfText.push_back(&cs->m_r);
			tempXOffset = cs->m_r.m_width + m_charPadding;
			break;
		case 's':
			m_lineOfText.push_back(&cs->m_s);
			tempXOffset = cs->m_s.m_width + m_charPadding;
			break;
		case 't':
			m_lineOfText.push_back(&cs->m_t);
			tempXOffset = cs->m_t.m_width + m_charPadding;
			break;
		case 'u':
			m_lineOfText.push_back(&cs->m_u);
			tempXOffset = cs->m_u.m_width + m_charPadding;
			break;
		case 'v':
			m_lineOfText.push_back(&cs->m_v);
			tempXOffset = cs->m_v.m_width + m_charPadding;
			break;
		case 'w':
			m_lineOfText.push_back(&cs->m_w);
			tempXOffset = cs->m_w.m_width + m_charPadding;
			break;
		case 'x':
			m_lineOfText.push_back(&cs->m_x);
			tempXOffset = cs->m_x.m_width + m_charPadding;
			break;
		case 'y':
			m_lineOfText.push_back(&cs->m_y);
			tempXOffset = cs->m_y.m_width + m_charPadding;
			break;
		case 'z':
			m_lineOfText.push_back(&cs->m_z);
			tempXOffset = cs->m_z.m_width + m_charPadding;
			break;
		case 'A':
			m_lineOfText.push_back(&cs->m_A);
			tempXOffset = cs->m_A.m_width + m_charPadding;
			break;
		case 'B':
			m_lineOfText.push_back(&cs->m_B);
			tempXOffset = cs->m_B.m_width + m_charPadding;
			break;
		case 'C':
			m_lineOfText.push_back(&cs->m_C);
			tempXOffset = cs->m_C.m_width + m_charPadding;
			break;
		case 'D':
			m_lineOfText.push_back(&cs->m_D);
			tempXOffset = cs->m_D.m_width + m_charPadding;
			break;
		case 'E':
			m_lineOfText.push_back(&cs->m_E);
			tempXOffset = cs->m_E.m_width + m_charPadding;
			break;
		case 'F':
			m_lineOfText.push_back(&cs->m_F);
			tempXOffset = cs->m_F.m_width + m_charPadding;
			break;
		case 'G':
			m_lineOfText.push_back(&cs->m_G);
			tempXOffset = cs->m_G.m_width + m_charPadding;
			break;
		case 'H':
			m_lineOfText.push_back(&cs->m_H);
			tempXOffset = cs->m_H.m_width + m_charPadding;
			break;
		case 'I':
			m_lineOfText.push_back(&cs->m_I);
			tempXOffset = cs->m_I.m_width + m_charPadding;
			break;
		case 'J':
			m_lineOfText.push_back(&cs->m_J);
			tempXOffset = cs->m_J.m_width + m_charPadding;
			break;
		case 'K':
			m_lineOfText.push_back(&cs->m_K);
			tempXOffset = cs->m_K.m_width + m_charPadding;
			break;
		case 'L':
			m_lineOfText.push_back(&cs->m_L);
			tempXOffset = cs->m_L.m_width + m_charPadding;
			break;
		case 'M':
			m_lineOfText.push_back(&cs->m_M);
			tempXOffset = cs->m_M.m_width + m_charPadding;
			break;
		case 'N':
			m_lineOfText.push_back(&cs->m_N);
			tempXOffset = cs->m_N.m_width + m_charPadding;
			break;
		case 'O':
			m_lineOfText.push_back(&cs->m_O);
			tempXOffset = cs->m_O.m_width + m_charPadding;
			break;
		case 'P':
			m_lineOfText.push_back(&cs->m_P);
			tempXOffset = cs->m_P.m_width + m_charPadding;
			break;
		case 'Q':
			m_lineOfText.push_back(&cs->m_Q);
			tempXOffset = cs->m_Q.m_width + m_charPadding;
			break;
		case 'R':
			m_lineOfText.push_back(&cs->m_R);
			tempXOffset = cs->m_R.m_width + m_charPadding;
			break;
		case 'S':
			m_lineOfText.push_back(&cs->m_S);
			tempXOffset = cs->m_S.m_width + m_charPadding;
			break;
		case 'T': 
			m_lineOfText.push_back(&cs->m_T);
			tempXOffset = cs->m_T.m_width + m_charPadding;
			break;
		case 'U':
			m_lineOfText.push_back(&cs->m_U);
			tempXOffset = cs->m_U.m_width + m_charPadding;
			break;
		case 'V':
			m_lineOfText.push_back(&cs->m_V);
			tempXOffset = cs->m_V.m_width + m_charPadding;
			break;
		case 'W':
			m_lineOfText.push_back(&cs->m_W);
			tempXOffset = cs->m_W.m_width + m_charPadding;
			break;
		case 'X':
			m_lineOfText.push_back(&cs->m_X);
			tempXOffset = cs->m_X.m_width + m_charPadding;
			break;
		case 'Y':
			m_lineOfText.push_back(&cs->m_Y);
			tempXOffset = cs->m_Y.m_width + m_charPadding;
			break;
		case 'Z':
			m_lineOfText.push_back(&cs->m_Z);
			tempXOffset = cs->m_Z.m_width + m_charPadding;
			break;
		case '0':
			m_lineOfText.push_back(&cs->m_num0);
			tempXOffset = cs->m_num0.m_width + m_charPadding;
			break;
		case '1':
			m_lineOfText.push_back(&cs->m_num1);
			tempXOffset = cs->m_num1.m_width + m_charPadding;
			break;
		case '2':
			m_lineOfText.push_back(&cs->m_num2);
			tempXOffset = cs->m_num2.m_width + m_charPadding;
			break;
		case '3':
			m_lineOfText.push_back(&cs->m_num3);
			tempXOffset = cs->m_num3.m_width + m_charPadding;
			break;
		case '4':
			m_lineOfText.push_back(&cs->m_num4);
			tempXOffset = cs->m_num4.m_width + m_charPadding;
			break;
		case '5':
			m_lineOfText.push_back(&cs->m_num5);
			tempXOffset = cs->m_num5.m_width + m_charPadding;
			break;
		case '6':
			m_lineOfText.push_back(&cs->m_num6);
			tempXOffset = cs->m_num6.m_width + m_charPadding;
			break;
		case '7':
			m_lineOfText.push_back(&cs->m_num7);
			tempXOffset = cs->m_num7.m_width + m_charPadding;
			break;
		case '8':
			m_lineOfText.push_back(&cs->m_num8);
			tempXOffset = cs->m_num8.m_width + m_charPadding;
			break;
		case '9':
			m_lineOfText.push_back(&cs->m_num9);
			tempXOffset = cs->m_num9.m_width + m_charPadding;
			break;
		case '.':
			m_lineOfText.push_back(&cs->m_period);
			tempXOffset = cs->m_period.m_width + m_charPadding;
			break;
		case ',':
			m_lineOfText.push_back(&cs->m_comma);
			tempXOffset = cs->m_comma.m_width + m_charPadding;
			break;
		case '?':
			m_lineOfText.push_back(&cs->m_questionMark);
			tempXOffset = cs->m_questionMark.m_width + m_charPadding;
			break;
		case ':':
			m_lineOfText.push_back(&cs->m_colon);
			tempXOffset = cs->m_colon.m_width + m_charPadding;
			break;
		case ';':
			m_lineOfText.push_back(&cs->m_semicolon);
			tempXOffset = cs->m_semicolon.m_width + m_charPadding;
			break;
		case '(':
			m_lineOfText.push_back(&cs->m_openParen);
			tempXOffset = cs->m_openParen.m_width + m_charPadding;
			break;
		case ')':
			m_lineOfText.push_back(&cs->m_closeParen);
			tempXOffset = cs->m_closeParen.m_width + m_charPadding;
			break;
		case '!':
			m_lineOfText.push_back(&cs->m_exclamation);
			tempXOffset = cs->m_exclamation.m_width + m_charPadding;
			break;
		case '/':
			m_lineOfText.push_back(&cs->m_forwardSlash);
			tempXOffset = cs->m_forwardSlash.m_width + m_charPadding;
			break;
		case '\\':
			m_lineOfText.push_back(&cs->m_backSlash);
			tempXOffset = cs->m_backSlash.m_width + m_charPadding;
			break;
		case '=':
			m_lineOfText.push_back(&cs->m_equal);
			tempXOffset = cs->m_equal.m_width + m_charPadding;
			break;
		case '+':
			m_lineOfText.push_back(&cs->m_plus);
			tempXOffset = cs->m_plus.m_width + m_charPadding;
			break;
		case '-':
			m_lineOfText.push_back(&cs->m_dash);
			tempXOffset = cs->m_dash.m_width + m_charPadding;
			break;
		case '*':
			m_lineOfText.push_back(&cs->m_asterisk);
			tempXOffset = cs->m_asterisk.m_width + m_charPadding;
			break;
		case '%':
			m_lineOfText.push_back(&cs->m_percent);
			tempXOffset = cs->m_percent.m_width + m_charPadding;
			break;
		case '\'':
			m_lineOfText.push_back(&cs->m_apostrophe);
			tempXOffset = cs->m_apostrophe.m_width + m_charPadding;
			break;
		case '#':
			m_lineOfText.push_back(&cs->m_poundSign);
			tempXOffset = cs->m_poundSign.m_width + m_charPadding;
			break;
		case '$':
			m_lineOfText.push_back(&cs->m_dollarSign);
			tempXOffset = cs->m_dollarSign.m_width + m_charPadding;
			break;
		case '&':
			m_lineOfText.push_back(&cs->m_ampersand);
			tempXOffset = cs->m_ampersand.m_width + m_charPadding;
			break;
		case '\"':
			m_lineOfText.push_back(&cs->m_doubleQuote);
			tempXOffset = cs->m_doubleQuote.m_width + m_charPadding;
			break;
		case '<':
			m_lineOfText.push_back(&cs->m_lessThan);
			tempXOffset = cs->m_lessThan.m_width + m_charPadding;
			break;
		case '>':
			m_lineOfText.push_back(&cs->m_greaterThan);
			tempXOffset = cs->m_greaterThan.m_width + m_charPadding;
			break;
		case '@':
			m_lineOfText.push_back(&cs->m_atSign);
			tempXOffset = cs->m_atSign.m_width + m_charPadding;
			break;
		case '[':
			m_lineOfText.push_back(&cs->m_openSquareBracket);
			tempXOffset = cs->m_openSquareBracket.m_width + m_charPadding;
			break;
		case ']':
			m_lineOfText.push_back(&cs->m_closeSquareBracket);
			tempXOffset = cs->m_closeSquareBracket.m_width + m_charPadding;
			break;
		case '{':
			m_lineOfText.push_back(&cs->m_openCurlyBrace);
			tempXOffset = cs->m_openCurlyBrace.m_width + m_charPadding;
			break;
		case '}':
			m_lineOfText.push_back(&cs->m_closeCurlyBrace);
			tempXOffset = cs->m_closeCurlyBrace.m_width + m_charPadding;
			break;
		case '^':
			m_lineOfText.push_back(&cs->m_caret);
			tempXOffset = cs->m_caret.m_width + m_charPadding;
			break;
		case '_':
			m_lineOfText.push_back(&cs->m_underscore);
			tempXOffset = cs->m_underscore.m_width + m_charPadding;
			break;
		case '|':
			m_lineOfText.push_back(&cs->m_verticalBar);
			tempXOffset = cs->m_verticalBar.m_width + m_charPadding;
			break;
		case '\n':
			m_currentCharIndex++;
			m_currentLineWidth = insertXPosOffset;
			return true;
			break;
		case ' ':
			m_lineOfText.push_back(NULL);
			tempXOffset = m_scaledSpaceChar;
			containsASpace = true;
			break;
		default:
			break;
		}

		//If the next character will extend pass the right of the text box
		//then we need to word wrap.  Back up to the nearest space.
		if ((m_textRect.left + insertXPosOffset + tempXOffset) > m_textRect.right)
		{
			//Are we word wraping or preserving line breaks
			if (m_wordWrapEnabled)
			{
			
				//If the current line contains a space, back up to it.
				if (containsASpace)
				{
					while (m_pCharBuf[m_currentCharIndex] != ' ')
					{
						m_lineOfText.pop_back();
						m_currentCharIndex--;
					}
				}
			/*
			//Increment the char index and return.
			m_currentCharIndex++;
			m_currentLineWidth = insertXPosOffset;
			return true;
			*/
			}
			else
			{

				while (m_pCharBuf[m_currentCharIndex] != '\n')
				{
					++m_currentCharIndex;
					++numCharsSkipped;
					//Make sure we aren't at the end of the char buffer
					if (m_currentCharIndex >= (m_numCurrChars))
						break;
				}
				
				//Let's insert '...' if there are at least 3 chars
				if (m_lineOfText.size() >= 3)
				{
					XCharCoords* pCC = NULL;
					for (XU32 i = 0;i < 3;++i)
					{
						pCC = m_lineOfText.back();
						if (pCC)
							insertXPosOffset = insertXPosOffset - (pCC->m_width + m_charPadding);
						else
							insertXPosOffset = insertXPosOffset - m_scaledSpaceChar;
						m_lineOfText.pop_back();
					}
					for (XU32 i = 0;i < 3;++i)
					{
						insertXPosOffset = insertXPosOffset + cs->m_period.m_width + m_charPadding;
						m_lineOfText.push_back(&cs->m_period);
					}

					//If right justified, adjust to make room for the 3 periods we sticking
					//onto the end of this line.
					if (m_justification == X_TBJ_RIGHT)
						insertXPosOffset += (cs->m_period.m_width + m_charPadding)*2;
				}
			}

			++m_currentCharIndex;
			m_currentLineWidth = insertXPosOffset;

			return true;


		}

		//If the next character will extend pass the bottom of the text box
		//stop the rendering and terminate the char buffer.  Update the size and break.
		/*if ((m_nativeScreenRect.top + insertYPosOffset + tempYOffset) > m_nativeScreenRect.bottom)
		{
			m_pCharBuf[i] = '\0';
			m_numCurrChars = strlen(m_pCharBuf);
			break;
		}*/

		//Increment the character index	
		m_currentCharIndex++;

		//Add the temp offset and continue.
		insertXPosOffset += tempXOffset;
		
		//Make sure we aren't at the end of the char buffer
		if (m_currentCharIndex >= (m_numCurrChars))
		{
			m_currentLineWidth = insertXPosOffset;
			return true;
		}
	} 

	m_currentLineWidth = insertXPosOffset;
	return true;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getLongestLineInNativePixels()
{
	//Do we even have any text?
	if (m_numCurrChars == 0)
		return 0;

	//Grab a pointer to this text box's charset
	XCharSet* cs = XVP::getCharSetInterface(m_charSetHandle);
	if (!cs)
		return false;

	XU32 longestLine = 0;
	XU32 currentLine = 0;
	XU32 currentIndex = 0;
	XU32 charWidth = 0;
	XBOOL breakFromInnerLoop = false;

	//Make sure we aren't at the end of the char buffer
	while (currentIndex < m_numCurrChars)
	{
		currentLine = 0;
		breakFromInnerLoop = false;

		while (true)
		{
			switch (m_pCharBuf[currentIndex])
			{
			case 'a':
				charWidth = cs->m_a.m_width + m_charPadding; 
				break;
			case 'b':
				charWidth = cs->m_b.m_width + m_charPadding;
				break;
			case 'c':
				charWidth = cs->m_c.m_width + m_charPadding;
				break;
			case 'd':
				charWidth = cs->m_d.m_width + m_charPadding;
				break;
			case 'e':
				charWidth = cs->m_e.m_width + m_charPadding;
				break;
			case 'f':
				charWidth = cs->m_f.m_width + m_charPadding;
				break;
			case 'g':
				charWidth = cs->m_g.m_width + m_charPadding;
				break;
			case 'h':
				charWidth = cs->m_h.m_width + m_charPadding;
				break;
			case 'i':
				charWidth = cs->m_i.m_width + m_charPadding;
				break;
			case 'j':
				charWidth = cs->m_j.m_width + m_charPadding;
				break;
			case 'k':
				charWidth = cs->m_k.m_width + m_charPadding;
				break;
			case 'l':
				charWidth = cs->m_l.m_width + m_charPadding;
				break;
			case 'm':
				charWidth = cs->m_m.m_width + m_charPadding;
				break;
			case 'n':
				charWidth = cs->m_n.m_width + m_charPadding;
				break;
			case 'o':
				charWidth = cs->m_o.m_width + m_charPadding;
				break;
			case 'p':
				charWidth = cs->m_p.m_width + m_charPadding;
				break;
			case 'q':
				charWidth = cs->m_q.m_width + m_charPadding;
				break;
			case 'r':
				charWidth = cs->m_r.m_width + m_charPadding;
				break;
			case 's':
				charWidth = cs->m_s.m_width + m_charPadding;
				break;
			case 't':
				charWidth = cs->m_t.m_width + m_charPadding;
				break;
			case 'u':
				charWidth = cs->m_u.m_width + m_charPadding;
				break;
			case 'v':
				charWidth = cs->m_v.m_width + m_charPadding;
				break;
			case 'w':
				charWidth = cs->m_w.m_width + m_charPadding;
				break;
			case 'x':
				charWidth = cs->m_x.m_width + m_charPadding;
				break;
			case 'y':
				charWidth = cs->m_y.m_width + m_charPadding;
				break;
			case 'z':
				charWidth = cs->m_z.m_width + m_charPadding;
				break;
			case 'A':
				charWidth = cs->m_A.m_width + m_charPadding;
				break;
			case 'B':
				charWidth = cs->m_B.m_width + m_charPadding;
				break;
			case 'C':
				charWidth = cs->m_C.m_width + m_charPadding;
				break;
			case 'D':
				charWidth = cs->m_D.m_width + m_charPadding;
				break;
			case 'E':
				charWidth = cs->m_E.m_width + m_charPadding;
				break;
			case 'F':
				charWidth = cs->m_F.m_width + m_charPadding;
				break;
			case 'G':
				charWidth = cs->m_G.m_width + m_charPadding;
				break;
			case 'H':
				charWidth = cs->m_H.m_width + m_charPadding;
				break;
			case 'I':
				charWidth = cs->m_I.m_width + m_charPadding;
				break;
			case 'J':
				charWidth = cs->m_J.m_width + m_charPadding;
				break;
			case 'K':
				charWidth = cs->m_K.m_width + m_charPadding;
				break;
			case 'L':
				charWidth = cs->m_L.m_width + m_charPadding;
				break;
			case 'M':
				charWidth = cs->m_M.m_width + m_charPadding;
				break;
			case 'N':
				charWidth = cs->m_N.m_width + m_charPadding;
				break;
			case 'O':
				charWidth = cs->m_O.m_width + m_charPadding;
				break;
			case 'P':
				charWidth = cs->m_P.m_width + m_charPadding;
				break;
			case 'Q':
				charWidth = cs->m_Q.m_width + m_charPadding;
				break;
			case 'R':
				charWidth = cs->m_R.m_width + m_charPadding;
				break;
			case 'S':
				charWidth = cs->m_S.m_width + m_charPadding;
				break;
			case 'T': 
				charWidth = cs->m_T.m_width + m_charPadding;
				break;
			case 'U':
				charWidth = cs->m_U.m_width + m_charPadding;
				break;
			case 'V':
				charWidth = cs->m_V.m_width + m_charPadding;
				break;
			case 'W':
				charWidth = cs->m_W.m_width + m_charPadding;
				break;
			case 'X':
				charWidth = cs->m_X.m_width + m_charPadding;
				break;
			case 'Y':
				charWidth = cs->m_Y.m_width + m_charPadding;
				break;
			case 'Z':
				charWidth = cs->m_Z.m_width + m_charPadding;
				break;
			case '0':
				charWidth = cs->m_num0.m_width + m_charPadding;
				break;
			case '1':
				charWidth = cs->m_num1.m_width + m_charPadding;
				break;
			case '2':
				charWidth = cs->m_num2.m_width + m_charPadding;
				break;
			case '3':
				charWidth = cs->m_num3.m_width + m_charPadding;
				break;
			case '4':
				charWidth = cs->m_num4.m_width + m_charPadding;
				break;
			case '5':
				charWidth = cs->m_num5.m_width + m_charPadding;
				break;
			case '6':
				charWidth = cs->m_num6.m_width + m_charPadding;
				break;
			case '7':
				charWidth = cs->m_num7.m_width + m_charPadding;
				break;
			case '8':
				charWidth = cs->m_num8.m_width + m_charPadding;
				break;
			case '9':
				charWidth = cs->m_num9.m_width + m_charPadding;
				break;
			case '.':
				charWidth = cs->m_period.m_width + m_charPadding;
				break;
			case ',':
				charWidth = cs->m_comma.m_width + m_charPadding;
				break;
			case '?':
				charWidth = cs->m_questionMark.m_width + m_charPadding;
				break;
			case ':':
				charWidth = cs->m_colon.m_width + m_charPadding;
				break;
			case ';':
				charWidth = cs->m_semicolon.m_width + m_charPadding;
				break;
			case '(':
				charWidth = cs->m_openParen.m_width + m_charPadding;
				break;
			case ')':
				charWidth = cs->m_closeParen.m_width + m_charPadding;
				break;
			case '!':
				charWidth = cs->m_exclamation.m_width + m_charPadding;
				break;
			case '/':
				charWidth = cs->m_forwardSlash.m_width + m_charPadding;
				break;
			case '\\':
				charWidth = cs->m_backSlash.m_width + m_charPadding;
				break;
			case '=':
				charWidth = cs->m_equal.m_width + m_charPadding;
				break;
			case '+':
				charWidth = cs->m_plus.m_width + m_charPadding;
				break;
			case '-':
				charWidth = cs->m_dash.m_width + m_charPadding;
				break;
			case '*':
				charWidth = cs->m_asterisk.m_width + m_charPadding;
				break;
			case '%':
				charWidth = cs->m_percent.m_width + m_charPadding;
				break;
			case '\'':
				charWidth = cs->m_apostrophe.m_width + m_charPadding;
				break;
			case '#':
				charWidth = cs->m_poundSign.m_width + m_charPadding;
				break;
			case '$':
				charWidth = cs->m_dollarSign.m_width + m_charPadding;
				break;
			case '&':
				charWidth = cs->m_ampersand.m_width + m_charPadding;
				break;
			case '\"':
				charWidth = cs->m_doubleQuote.m_width + m_charPadding;
				break;
			case '<':
				charWidth = cs->m_lessThan.m_width + m_charPadding;
				break;
			case '>':
				charWidth = cs->m_greaterThan.m_width + m_charPadding;
				break;
			case '@':
				charWidth = cs->m_atSign.m_width + m_charPadding;
				break;
			case '[':
				charWidth = cs->m_openSquareBracket.m_width + m_charPadding;
				break;
			case ']':
				charWidth = cs->m_closeSquareBracket.m_width + m_charPadding;
				break;
			case '{':
				charWidth = cs->m_openCurlyBrace.m_width + m_charPadding;
				break;
			case '}':
				charWidth = cs->m_closeCurlyBrace.m_width + m_charPadding;
				break;
			case '^':
				charWidth = cs->m_caret.m_width + m_charPadding;
				break;
			case '_':
				charWidth = cs->m_underscore.m_width + m_charPadding;
				break;
			case '|':
				charWidth = cs->m_verticalBar.m_width + m_charPadding;
				break;
			case '\n':
				breakFromInnerLoop = true;
				break;
			case ' ':
				charWidth = m_scaledSpaceChar;
				break;
			default:
				break;
			}

			currentIndex++;
			currentLine += charWidth;
			if ((breakFromInnerLoop) || (currentIndex >= m_numCurrChars))
				break;
		}

		if (currentLine > longestLine)
			longestLine = currentLine;
	}

	return longestLine;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getNumHardLineBreaks()
{
	//Make sure we have chars in the text box
	if (m_numCurrChars == 0)
		return 0;

	//Pass back the cached value or re-evaluate
	if (m_numHardLineBreaks != -1)
		return m_numHardLineBreaks;

	XU32 len = (XU32)strlen(m_pCharBuf);
	m_numHardLineBreaks = 0;
	for (XU32 i = 0;i < len;++i)
	{
		if (m_pCharBuf[i] == '\n')
			++m_numHardLineBreaks;
	}

	return m_numHardLineBreaks;
}
	
	

//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::onResize(XF32 xScale,XF32 yScale)
{
	//Save scaling information for the render method
	m_xScale = xScale;
	m_yScale = yScale;
	
	//Adjust the size of the scaled rect used for the background image.
	m_scaledScreenRect.left = (int)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (int)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (int)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (int)(m_nativeScreenRect.bottom * yScale);
	
	m_scaledSpaceChar = (XU32)(m_spaceChar * m_xScale);

	
	m_cachedCharIndex = -1;

	return X_OK;
}



//*****************************************************************************
/**
Allows the text box to update any time dependent, internal data.  If the focus
is set, allows the text box to receive keyboard input.

@param dt	The amount of time, in seconds, that has passed since the
					last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface text box.
@param yScale	The vertical scale factor relative to the original
				height of the user interface text box.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUITextBox::update(XF32 dt)
{
	if (!m_editable)
		return X_OK;

	//If a cursor is loaded, update it.
	if (m_cursorLoaded)
		m_cursor.update(dt);

	//If this textbox does not have the m_focus then it will ignore
	//keyboard input.
	if (!m_focus)
		return X_OK;
	
	//If there are no keys pressed, then we can safely bail early.
	if (!XVP::InputMgr.anyKeyboardInput())
		return X_OK;

	//First grab a pointer to the charset
	XCharSet* cs = NULL;		
	cs = XVP::getCharSetInterface(m_charSetHandle);
	if (!cs)
		return X_ERR_OPERATION_FAILED;

	XBOOL shift = false;

	//Check for the shift keys
	if (XVP::InputMgr.keyDown(X_KEY_LSHIFT) || XVP::InputMgr.keyDown(X_KEY_RSHIFT))
		shift = true;

	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_0))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_closeParen.m_width != 0))
				appendText(")");
		}
		else 
		{
			if ((m_type & X_TBT_NUM) && (cs->m_num0.m_width != 0))
				appendText("0");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_1))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_exclamation.m_width != 0))
				appendText("!");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num1.m_width != 0))
				appendText("1");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_2))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_atSign.m_width != 0))
				appendText("@");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num2.m_width != 0))
				appendText("2");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_3))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_poundSign.m_width != 0))
				appendText("#");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num3.m_width != 0))
				appendText("3");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_4))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_dollarSign.m_width != 0))
				appendText("$");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num4.m_width != 0))
				appendText("4");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_5))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_percent.m_width != 0))
				appendText("%");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num5.m_width != 0))
				appendText("5");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_6))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_caret.m_width != 0))
				appendText("^");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num6.m_width != 0))
				appendText("6");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_7))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_ampersand.m_width != 0))
				appendText("&");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num7.m_width != 0))
				appendText("7");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_8))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_asterisk.m_width != 0))
				appendText("*");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num8.m_width != 0))
				appendText("8");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_9))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_openParen.m_width != 0))
				appendText("(");
		}
		else
		{ 
			if ((m_type & X_TBT_NUM) && (cs->m_num9.m_width != 0))
				appendText("9");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_A))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_A.m_width != 0))
				appendText("A");
		}
		else
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_a.m_width != 0))
				appendText("a"); 
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_B))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_B.m_width != 0))
				appendText("B");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_b.m_width != 0))
				appendText("b");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_C))
		if (shift)
			
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_C.m_width != 0))
				appendText("C");
		}
		else
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_c.m_width != 0))
				appendText("c");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_D))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_D.m_width != 0))
				appendText("D");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_d.m_width != 0))
				appendText("d");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_E))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_E.m_width != 0))
				appendText("E");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_e.m_width != 0))
				appendText("e");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_F))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_F.m_width != 0))
				appendText("F");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_f.m_width != 0))
				appendText("f");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_G))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_G.m_width != 0))
				appendText("G");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_g.m_width != 0))
				appendText("g");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_H))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_H.m_width != 0))
				appendText("H");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_h.m_width != 0))
				appendText("h");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_I))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_I.m_width != 0))
				appendText("I");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_i.m_width != 0))
				appendText("i");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_J))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_J.m_width != 0))
				appendText("J");
		}
		else
		{ 
			if ((m_type & X_TBT_LOWER) && (cs->m_j.m_width != 0))
				appendText("j");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_K))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_K.m_width != 0))
				appendText("K");
		}
		else	
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_k.m_width != 0))
				appendText("k");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_L))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_L.m_width != 0))
				appendText("L");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_l.m_width != 0))
				appendText("l");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_M))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_M.m_width != 0))
				appendText("M");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_m.m_width != 0))
				appendText("m");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_N))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_N.m_width != 0))
				appendText("N");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_n.m_width != 0))
				appendText("n");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_O))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_O.m_width != 0))
				appendText("O");
		}
		else
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_o.m_width != 0))
				appendText("o");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_P))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_P.m_width != 0))
				appendText("P");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_p.m_width != 0))
				appendText("p");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_Q))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_Q.m_width != 0))
				appendText("Q");
		}
		else 	
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_q.m_width != 0))
				appendText("q");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_R))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_R.m_width != 0))
				appendText("R");
		}
		else
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_r.m_width != 0))
				appendText("r");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_S))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_S.m_width != 0))
				appendText("S");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_s.m_width != 0))
				appendText("s");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_T))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_T.m_width != 0))
				appendText("T");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_t.m_width != 0))
				appendText("t");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_U))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_U.m_width != 0))
				appendText("U");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_u.m_width != 0))
				appendText("u");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_V))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_V.m_width != 0))
				appendText("V");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_v.m_width != 0))
				appendText("v");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_W))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_W.m_width != 0))
				appendText("W");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_w.m_width != 0))
				appendText("w");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_X))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_X.m_width != 0))
				appendText("X");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_x.m_width != 0))
				appendText("x");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_Y))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_Y.m_width != 0))
				appendText("Y");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_y.m_width != 0))
				appendText("y");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_Z))
		if (shift)
		{
			if ((m_type & X_TBT_CAPS) && (cs->m_Z.m_width != 0))
				appendText("Z");
		}
		else 
		{
			if ((m_type & X_TBT_LOWER) && (cs->m_z.m_width != 0))
				appendText("z");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_MINUS) && (m_type & X_TBT_SYM))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_underscore.m_width != 0))
				appendText("_");
		}
		else 
		{
			if ((m_type & X_TBT_SYM) && (cs->m_dash.m_width != 0))
				appendText("-");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_PERIOD))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_greaterThan.m_width != 0))
				appendText(">");
		}
		else 
		{
			if ((m_type & X_TBT_SYM) || (m_type & X_TBT_FLOATING))
				if (cs->m_period.m_width != 0)
					appendText(".");
		}
 	if (XVP::InputMgr.keyDownOnce(X_KEY_COMMA))
		if (shift)
		{
			if ((m_type & X_TBT_SYM) && (cs->m_lessThan.m_width != 0))
				appendText("<");
		}
		else 
		{
			if ((m_type & X_TBT_SYM) && (cs->m_comma.m_width != 0))
				appendText(",");
		}
 	if (XVP::InputMgr.keyDownOnce(X_KEY_SEMICOLON) && (m_type & X_TBT_SYM))
		if (shift)
		{
			if (cs->m_colon.m_width != 0)
				appendText(":");
		}
		else 
		{
			if (cs->m_semicolon.m_width != 0)
				appendText(";");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_SLASH) && (m_type & X_TBT_SYM))
		if (shift)
		{
			if (cs->m_questionMark.m_width != 0)
				appendText("?");
		}
		else 
		{
			if (cs->m_forwardSlash.m_width != 0)
				appendText("/");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_BACKSLASH) && (m_type & X_TBT_SYM))
		if (shift)
		{	
			if (cs->m_verticalBar.m_width != 0)
				appendText("|");
		}
		else 
		{
			if (cs->m_backSlash.m_width != 0)
				appendText("\\");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_EQUALS) && (m_type & X_TBT_SYM))
		if (shift)
		{	
			if (cs->m_plus.m_width != 0)
				appendText("+");
		}
		else 
		{
			if (cs->m_equal.m_width != 0)
				appendText("=");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_APOSTROPHE) && (m_type & X_TBT_SYM))
		if (shift)
		{	
			if (cs->m_doubleQuote.m_width != 0)
				appendText("\"");
		}
		else 
		{
			if (cs->m_apostrophe.m_width != 0)
				appendText("\'");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_LBRACKET) && (m_type & X_TBT_SYM))
		if (shift)
		{	
			if (cs->m_openCurlyBrace.m_width != 0)
				appendText("{");
		}
		else 
		{
			if (cs->m_openSquareBracket.m_width != 0)
				appendText("[");
		}
	if (XVP::InputMgr.keyDownOnce(X_KEY_RBRACKET) && (m_type & X_TBT_SYM))
		if (shift)
		{	
			if (cs->m_closeCurlyBrace.m_width != 0)
				appendText("}");
		}
		else 
		{
			if (cs->m_closeSquareBracket.m_width != 0)
				appendText("]");
		}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_BACK) && (m_type & X_TBT_BACK))
	{
		//Make sure that by removing one character we won't be violating
		//a read only section of the text box.
		if (getTextLength() > 0)
		{
			if (getTextLength() > m_readOnlyMarker)
				removeTextFromEnd(1);
		}
	}
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_SPACE) && (m_type & X_TBT_SPACE))
		appendText(" ");
	if (XVP::InputMgr.keyDownOnceRepeat(X_KEY_RETURN) && (m_type & X_TBT_RETURN))
		appendText("\n");


	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::onMouseOver(XVector2D& hotSpot,XUIElement* pChildElement)
{
	if (!m_stickyHighlightMode)
	{
		XRect scaledTextBoxRect;
		getScaledTextBoxRect(scaledTextBoxRect);
	 
		float scaledLineHeight = ((float)getLineHeight() * m_yScale);
			
		m_hoverIndex = (XS32)((hotSpot.y - scaledTextBoxRect.top) / scaledLineHeight);
		
		if (m_hoverIndex > (getNumLinesVisible() - 1))
			--m_hoverIndex;
	}

	XUIElement::onMouseOver(hotSpot,pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::onMouseDown(XVector2D& hotSpot,X_MOUSE_BUTTON button,
								XUIElement* pChildElement)
{
	if (m_stickyHighlightMode)
	{
		XRect scaledTextBoxRect;
		getScaledTextBoxRect(scaledTextBoxRect);
	 
		float scaledLineHeight = ((float)getLineHeight() * m_yScale);
			
		m_hoverIndex = (XS32)((hotSpot.y - scaledTextBoxRect.top) / scaledLineHeight);
		
		if (m_hoverIndex > (getNumLinesVisible() - 1))
			--m_hoverIndex;
	}

	XUIElement::onMouseDown(hotSpot,button,pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::onMouseLeave(XUIElement* pChildElement)
{
	if (!m_stickyHighlightMode)
		m_hoverIndex = -1;	

	XUIElement::onMouseLeave(pChildElement);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XUITextBox::getHoverIndex()
{
	return m_hoverIndex;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUITextBox::setTextPadding(XU32 textPadding)
{
	m_textPadding = textPadding;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XUITextBox::getTextPadding()
{
	return m_textPadding;
}
