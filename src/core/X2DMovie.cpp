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
//	Module Name:	X2DMovie.cpp
//	Author:			Lynn Duke  
//	Creation Date:	6-4-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
/**
The constructor for the 2D movie.
*/
//*****************************************************************************
X2DMovie::X2DMovie()
{
	m_movieHandle = X_INVALID_HANDLE;
	m_width = 0;
	m_height = 0;
	m_opacity = 1.0f;
}



//*****************************************************************************
/**
The destructor for the 2D movie.
*/
//*****************************************************************************
X2DMovie::~X2DMovie()
{
	//Stop playing the movie and destroy it if X-VP is still alive.
	//if (m_pX)
	//{
		XVP::GraphicsMgr.stopMovie(m_movieHandle);
		XVP::GraphicsMgr.destroyMovie(m_movieHandle);
	//}
}



//*****************************************************************************
/**
Initializes the 2D movie with a movie file.

@param pMovieFilename	The movie file to load.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::init(XS8* pMovieFilename)
{
	//Make sure the filename is valid.
	if (!pMovieFilename)
		return X_ERR_INVALID_PARAMS;

	//Destroy any resources this object may already have allocated.
	destroy();

	//Create the movie.
	if (XVP::GraphicsMgr.createMovie(pMovieFilename,m_movieHandle) != X_OK)
		return X_ERR_INVALID_PARAMS;
	else
	{
		//Get the dimensions of the movie
		XU32 width = XVP::GraphicsMgr.getMovieWidth(m_movieHandle);
		XU32 height = XVP::GraphicsMgr.getMovieHeight(m_movieHandle);
		m_width = width;
		m_height = height;
		m_srcRect.set(0,0,width,height);
		m_destRect = m_srcRect;

		return X_OK;
	}
}



//*****************************************************************************
/**
Destroys a 2D movie by freeing all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::destroy()
{
	//if (m_pX)
	//{
		if (m_movieHandle != 0)
		{
			XStatus status;
			XVP::GraphicsMgr.stopMovie(m_movieHandle);
			status = XVP::GraphicsMgr.destroyMovie(m_movieHandle);
			m_movieHandle = 0;
			return status;
		}
	//}

	m_movieHandle = 0;
	return X_OK;
}



//*****************************************************************************
/**
Renders the movie to the screen.  This method will render the current frame
of the movie to the screen and is called by X-VP.  

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::render()
{
	if (m_movieHandle != 0)
	{
		XVP::GraphicsMgr.setTexture(m_movieHandle,0);
		return XVP::GraphicsMgr.renderTexture(m_width,m_height,m_srcRect,m_destRect,
											m_opacity);
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Plays the movie.

@param playCount	The number of times to play the movie.  If this value is
					-1, the movie will repeat infinitely.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::play(int playCount)
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.playMovie(m_movieHandle,playCount);
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Pauses a movie.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::pause()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.pauseMovie(m_movieHandle);
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Resumes a paused movie.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::resume()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.resumeMovie(m_movieHandle);
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Stops a movie.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::stop()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.stopMovie(m_movieHandle);
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Sets the play position to a particular time within the movie.

@param milliseconds	The play time to set the movie to.  For example, if this
					value is 2000, the play position is set to 2 seconds into
					the movie.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus X2DMovie::setTime(XU32 milliseconds)
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.setMovieTime(m_movieHandle,milliseconds);
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
//*****************************************************************************
XU32 X2DMovie::getTime()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.getMovieTime(m_movieHandle);
	else
		return 0;
}



//*****************************************************************************
/**
Sets the play count for the movie.  This play count determines how many times
the movie will loop.

@param playCount   The number of times to play the movie. 
*/
//*****************************************************************************
XStatus X2DMovie::setPlayCount(int playCount)
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.setMoviePlayCount(m_movieHandle,playCount);
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
//*****************************************************************************
XS32 X2DMovie::getPlayCount()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.getMoviePlayCount(m_movieHandle);
	else
		return 0;
}



//*****************************************************************************
/**
Gets the width of the movie in pixels.

@return The width of the movie in pixels.
*/
//*****************************************************************************
XU32 X2DMovie::getWidth()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.getMovieWidth(m_movieHandle);
	else
		return 0;
}



//*****************************************************************************
//*****************************************************************************
XU32 X2DMovie::getDuration()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.getMovieDuration(m_movieHandle);
	else
		return 0;
}



//*****************************************************************************
/**
Gets the height of the movie in pixels.

@return The height of the movie in pixels.
*/
//*****************************************************************************
XU32 X2DMovie::getHeight()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.getMovieHeight(m_movieHandle);
	else
		return 0;
}



//*****************************************************************************
/**
Determines if the movie is playing.

@return True if the movie is playing, false otherwise.
*/
//*****************************************************************************
XBOOL X2DMovie::isPlaying()
{
	if (m_movieHandle != 0)
		return XVP::GraphicsMgr.isMoviePlaying(m_movieHandle);
	else
		return false;
}



//*****************************************************************************
/**
Sets the source rectangle.  This is the retangular region in the source image
that will be blitted to the screen.  By default this value is set to be
the entire movie.

@param srcRect	The rectangle representing the area to blit from the source image.
@return None.
*/
//*****************************************************************************
void X2DMovie::setSrcRect(XRect& srcRect)
{
	m_srcRect = srcRect;
}



//*****************************************************************************
/**
Gets the source rectangle.  This is the retangular region in the source image
that will be blitted to the screen. 

@param srcRect	A variable to receive the source rectangle.
@return None.
*/
//*****************************************************************************
void X2DMovie::getSrcRect(XRect& srcRect)
{
	srcRect = m_srcRect;
}



//*****************************************************************************
/**
Sets the destination rectangle.  This is the retangular region on the screen
where the source image will be blitted.  

@param destRect	The rectangle representing the area to blit to on the screen.
@return None.
*/
//*****************************************************************************
void X2DMovie::setDestRect(XRect& destRect)
{
	m_destRect = destRect;
}



//*****************************************************************************
/**
Gets the destination rectangle.  This is the retangular region on the screen
where the source image will be blitted. 

@param destRect	A variable to receive the destination rectangle.
@return None.
*/
//*****************************************************************************
void X2DMovie::getDestRect(XRect& destRect)
{
	destRect = m_destRect;
}



//*****************************************************************************
/**
Sets the position of the movie on the screen.

@param position	The x,y screen position of the movie.
@return None.
*/
//*****************************************************************************
void X2DMovie::setPosition(XVector2D& position)
{
	XU32 width = m_destRect.right - m_destRect.left;
	XU32 height = m_destRect.bottom - m_destRect.top;
	m_destRect.left = (int)position.x;
	m_destRect.top = (int)position.y;
	m_destRect.right = m_destRect.left + width;
	m_destRect.bottom = m_destRect.top + height;}



//*****************************************************************************
/**
Returns the upper-left coordinate of the destination rectangle.

@return The upper-left coordinate of the destination rectangle.
*/
//*****************************************************************************
XVector2D X2DMovie::getPosition()
{
	return XVector2D((XF32)m_destRect.left,(XF32)m_destRect.top);
}



//*****************************************************************************
/**
Determines if a rectangle intersects with the movie's destination rectangle
on the screen.

@param rect	The rectangle to intersect the movie with.
@return True if the movie's destination rectangle and the passed in rectangle
		intersect, false otherwise.
*/
//*****************************************************************************
XBOOL X2DMovie::intersects(XRect& rect) 
{
	return m_destRect.intersects(rect);
}



//*****************************************************************************
//*****************************************************************************
XStatus X2DMovie::setOpacity(XF32 opacity)
{
	m_opacity = opacity;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 X2DMovie::getOpacity()
{
	return m_opacity;
}

