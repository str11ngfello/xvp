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
//	Module Name:	XUIProgressBar.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-30-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
/**
Constructor for the user interface progress bar.
*/
//*****************************************************************************
XUIProgressBar::XUIProgressBar()
{
	m_percentage = 0;
	m_coverSpriteWidth = 0;
	m_coverSpriteHeight = 0;
	m_coverFirst = false;
	m_xScale = 1;
	m_yScale = 1;

}



//*****************************************************************************
/**
Destructor for the user interface progress bar.
*/
//*****************************************************************************
XUIProgressBar::~XUIProgressBar()
{
}



//*****************************************************************************
/**
Initializes the user interface progress bar.

@param pParentUI	The user interface to which this progress bar belongs.
@param ID	The element ID used to identify this user interface progress bar 
			within a user interface.
@param zOrder	The zOrder of the user interface progress bar with respect to other
				user interface elements. 
@param screenRect	The rectangular area of the screen in which the track image
					of the progress bar will be displayed.
@param pTrackTexFilename	The texture file to use for the track image.
@param pCoverTexFilename	The texture file to use for the cover image.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIProgressBar::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect,
							XS8* pTemplateFile)
{
	//Validate parameters.
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
		TiXmlElement* pProgressBarElement = NULL;
		
		pNode = file.FirstChild( "progressbar" );
		if (pNode)
		{
			pProgressBarElement = pNode->ToElement();
			if (pProgressBarElement)
			{	
				XS8* pTrackFilename;
				XS8* pCoverFilename;
				XS32 intValue = 0;
				pTrackFilename = (XS8*)pProgressBarElement->Attribute("TrackFile");
				pCoverFilename = (XS8*)pProgressBarElement->Attribute("CoverFile");
				pProgressBarElement->QueryIntAttribute("CoverFirst",&intValue);
				m_coverFirst = (XBOOL)intValue;

				if (pTrackFilename && pCoverFilename)
				{
					if (m_trackSprite.init(pTrackFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					if (m_coverSprite.init(pCoverFilename) != X_OK)
						return X_ERR_OPERATION_FAILED;
					else
					{
						m_nativeScreenRect = screenRect;
						m_coverSpriteWidth = XVP::GraphicsMgr.getTextureWidth(m_coverSprite.getTextureHandle());
						m_coverSpriteHeight = XVP::GraphicsMgr.getTextureHeight(m_coverSprite.getTextureHandle());
					}
				}
			}
		}
		else
			return X_ERR_OPERATION_FAILED;
	
		setPercentage(0.0f);
		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);
	}

	return X_OK;
}	




//*****************************************************************************
/**
Destroys the user interface progress bar and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIProgressBar::destroy()
{
	m_trackSprite.destroy();
	m_coverSprite.destroy();

	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;
}
								 
								 
								 
//*****************************************************************************
/**
Updates the user interface progress bar.  This function is called automatically by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface progress bar.
@param yScale	The vertical scale factor relative to the original
				height of the user interface progress bar.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIProgressBar::update(XF32 dt)
{
	m_trackSprite.update(dt);
	m_coverSprite.update(dt);

	return X_OK;
}



//*****************************************************************************
/**
Renders the user interface progress bar.  This function is called
automatically by the engine once per frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUIProgressBar::render()
{
	if (!m_visible)
		return X_OK;

	if (m_coverFirst)
	{
		m_coverSprite.render();
		m_trackSprite.render();
	}
	else
	{
		m_trackSprite.render();
		m_coverSprite.render();
	}

	return X_OK;
}



//*****************************************************************************
/**
Returns the percentage of the progress bar.  For example, if the progess
bar is not filled at all, the percentage is 0.  Similarly, if the progress bar is completely
filled, the percentage returned is 1..

@return The completion percentage represented by the progress bar.
*/
//*****************************************************************************
XF32 XUIProgressBar::getPercentage()
{
	return m_percentage;
}



//*****************************************************************************
/**
Sets the percentage of the user interface progress bar.

@param percent	The completion percentage of the progress bar.  If the value is
				.5, the progress bar will be half filled.  If the value is
				1, the progress bar will be completely filled.
*/
//*****************************************************************************
XStatus XUIProgressBar::setPercentage(XF32 percent)
{
	if (percent < 0)
		m_percentage = 0;
	else if (percent > 1.0)
		m_percentage = 1;
	else
		m_percentage = percent;

	m_coverSprite.setSrcRect(XRect(0,0,(XU32)(m_coverSpriteWidth * m_percentage),m_coverSpriteHeight));

	m_coverSprite.setDestRect(XRect(m_scaledScreenRect.left,
									m_scaledScreenRect.top,
		(XU32)((m_nativeScreenRect.left + ((XU32)(m_nativeScreenRect.right - m_nativeScreenRect.left) * m_percentage)) * m_xScale),
		(XU32)((m_nativeScreenRect.top + (m_nativeScreenRect.bottom - m_nativeScreenRect.top)) * m_yScale)));
	
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIProgressBar::renderCoverFirst(XBOOL coverFirst)
{
	m_coverFirst = coverFirst;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XUIProgressBar::isCoverFirst()
{
	return m_coverFirst;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIProgressBar::setOpacity(XF32 opacity)
{
	m_trackSprite.setOpacity(opacity);
	m_coverSprite.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XUIProgressBar::move(XVector2D& moveVector)
{
	XRect r;
	m_trackSprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_trackSprite.setDestRect(r);

	m_coverSprite.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_coverSprite.setDestRect(r);

	m_nativeScreenRect.left += (XU32)moveVector.x;
	m_nativeScreenRect.right += (XU32)moveVector.x;
	m_nativeScreenRect.top += (XU32)moveVector.y;
	m_nativeScreenRect.bottom += (XU32)moveVector.y;

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUIProgressBar::onResize(XF32 xScale,XF32 yScale)
{
	//Save the scaling info
	m_xScale = xScale;
	m_yScale = yScale;

	//Adjust the size of the progress bar if necessary.
	m_scaledScreenRect.left = (int)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (int)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (int)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (int)(m_nativeScreenRect.bottom * yScale);
	m_trackSprite.setDestRect(m_scaledScreenRect);

	return X_OK;
}