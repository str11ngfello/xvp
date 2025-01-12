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
//	Module Name:	XUI2DMovie.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-14-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the user interface movie.
*/
//*****************************************************************************
XUI2DMovie::XUI2DMovie()
{
}



//*****************************************************************************
/**
Destructor for the user interface movie.
*/
//*****************************************************************************
XUI2DMovie::~XUI2DMovie()
{
}



//*****************************************************************************
/**
Initializes the user interface movie.

@param pParentUI	The user interface to which this movie belongs.
@param ID	The element ID used to identify this user interface movie within
			a user interface.
@param zOrder	The zOrder of the user interface movie with respect to other
				user interface elements. 
@param screenRect	The rectangular area of the screen in which the user interface
					movie will play.
@param pMovieFilename	The movie file to load.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUI2DMovie::init(XUserInterface* pParentUI,XU32 zOrder,XRect& screenRect, 
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
		TiXmlElement* pMovieElement = NULL;
		
		pNode = file.FirstChild( "movie" );
		if (pNode)
		{
			pMovieElement = pNode->ToElement();
			if (pMovieElement)
			{	
				XS8* pFilename = NULL;
				pFilename = (XS8*)pMovieElement->Attribute("MovieFile");

				if (pFilename)
				{
					XStatus status = m_movie.init(pFilename);
					if (status == X_OK)
						m_nativeScreenRect = screenRect;
					else
						return X_ERR_OPERATION_FAILED;
				}
			}
		}
		else
			return X_ERR_OPERATION_FAILED;
	
		m_zOrderOffset = zOrder;
		m_pParentUI = pParentUI;
		XUIElement::setOwner(pParentUI,this,zOrder);
	}

	return X_OK;
}



//*****************************************************************************
/**
Destroys the user interface movie and frees all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUI2DMovie::destroy()
{
	m_movie.destroy();
	///XUIElement::removeOwner(m_pParentUI,this);
	XUIElement::destroy();
	return X_OK;

}



//*****************************************************************************
/**
Updates the user interface movie.  This function is called automatically by the
engine once per frame.

@param dt	The amount of time, in seconds, that has elapsed since
					the last update.
@param xScale	The horizontal scale factor relative to the original
				width of the user interface movie.
@param yScale	The vertical scale factor relative to the original
				height of the user interface movie.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUI2DMovie::update(XF32 dt)
{
	//Update the play.
	m_movie.update(dt);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUI2DMovie::onResize(XF32 xScale,XF32 yScale)
{
	//Adjust the size of the movie if necessary.
	m_scaledScreenRect.left = (int)(m_nativeScreenRect.left * xScale);
	m_scaledScreenRect.top = (int)(m_nativeScreenRect.top * yScale);
	m_scaledScreenRect.right = (int)(m_nativeScreenRect.right * xScale);
	m_scaledScreenRect.bottom = (int)(m_nativeScreenRect.bottom * yScale);
	m_movie.setDestRect(m_scaledScreenRect);

	return X_OK;
}



//*****************************************************************************
/**
Renders the current movie frame to the screen.  This function is called
automatically by the engine once per frame.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XUI2DMovie::render()
{
	if (!m_visible)
		return X_OK;

	m_movie.render();
	return X_OK;
}



//*****************************************************************************
/**
Returns the X2DMovie object associated with the user interface movie.

@return The X2DMovie object associated with the user interface movie.
*/
//*****************************************************************************
X2DMovie* XUI2DMovie::getMovie()
{
	return &m_movie;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUI2DMovie::setOpacity(XF32 opacity)
{
	m_movie.setOpacity(opacity);
	XUIElement::setOpacity(opacity);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XUI2DMovie::move(XVector2D& moveVector)
{
	XRect r;
	m_movie.getDestRect(r);
	r.left += (XU32)moveVector.x;
	r.right += (XU32)moveVector.x;
	r.top += (XU32)moveVector.y;
	r.bottom += (XU32)moveVector.y;
	m_movie.setDestRect(r);

	m_nativeScreenRect.left += (XU32)moveVector.x;
	m_nativeScreenRect.right += (XU32)moveVector.x;
	m_nativeScreenRect.top += (XU32)moveVector.y;
	m_nativeScreenRect.bottom += (XU32)moveVector.y;

	onResize(m_pParentUI->getXScale(),m_pParentUI->getYScale());

	return X_OK;
}