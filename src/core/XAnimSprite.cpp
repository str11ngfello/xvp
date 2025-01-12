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
//	Module Name:	XAnimSprite.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-17-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the animation sprite.  
*/
//*****************************************************************************
XAnimSprite::XAnimSprite()
{	
	m_elapsedTime = 0;
	m_currentRow = 0;
	m_currentColumn = 0;
	m_frameWidth = 0;
	m_frameHeight = 0;
	m_visible = false;
	m_delayFrameTime = 1;
	m_beginRow = 0;
	m_endRow = 0;
	m_endCol = 0;
	m_numFramesToElapse = 0;
	m_playCount = 0;
}



//*****************************************************************************
/**
Destructor for the animation sprite.  
*/
//*****************************************************************************
XAnimSprite::~XAnimSprite()
{
}



//*****************************************************************************
/**
Initializes the animation sprite.  

@param	pTextureFilename	Path to an image which will be used for the sprite
							image data.
@param	desc	Describes how the animation sprite should be created, the layout
				of the image frames, and runtime parameters of the sprite.
@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimSprite::init(XS8* pTextureFilename,XAnimSpriteDesc& desc)
{
	//Check the validity of parameters.
	if ((desc.frameHeight == 0) || (desc.frameWidth == 0))
		return X_ERR_INVALID_PARAMS;
	
	//Init the base sprite.  Destroy any previous resources and reset the data.
	destroy();
	XAnimSprite();

	if (XSprite::init(pTextureFilename) != X_OK)
		return X_ERR_OPERATION_FAILED;
	
	m_frameWidth			= desc.frameWidth;
	m_frameHeight			= desc.frameHeight;
	m_visible				= desc.visible;
	m_beginRow				= desc.beginRow;
	m_endRow				= desc.endRow;
	m_endCol				= desc.endCol;
	m_currentRow			= desc.beginRow;
	m_currentColumn			= 0;

	//delayFrameTime cannot be less than or equal to 0.  If it is,
	//then default it to 1 second.
	if (desc.delayFrameTime <= 0)
		m_delayFrameTime = 1;
	else
		m_delayFrameTime = desc.delayFrameTime;
	
	
	//Set up the source rectangle
	m_srcRect.top			= desc.beginRow * desc.frameHeight;
	m_srcRect.left			= 0;
	m_srcRect.right			= desc.frameWidth;
	m_srcRect.bottom		= m_srcRect.top + desc.frameHeight;
	
	//Set up the destination rectangle
	m_destRect.left = (int)desc.position.x;
	m_destRect.top = (int)desc.position.y;
	m_destRect.right = m_destRect.left + m_frameWidth;
	m_destRect.bottom	= m_destRect.top  + m_frameHeight;

	//Record the position.
	m_position = desc.position;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAnimSprite::init(XAnimSpriteDesc& desc)
{
	m_frameWidth			= desc.frameWidth;
	m_frameHeight			= desc.frameHeight;
	m_visible				= desc.visible;
	m_beginRow				= desc.beginRow;
	m_endRow				= desc.endRow;
	m_endCol				= desc.endCol;
	m_currentRow			= desc.beginRow;
	m_currentColumn			= 0;

	//delayFrameTime cannot be less than or equal to 0.  If it is,
	//then default it to 1 second.
	if (desc.delayFrameTime <= 0)
		m_delayFrameTime = 1;
	else
		m_delayFrameTime = desc.delayFrameTime;
	
	//Set up the source rectangle
	m_srcRect.top			= desc.beginRow * desc.frameHeight;
	m_srcRect.left			= 0;
	m_srcRect.right			= desc.frameWidth;
	m_srcRect.bottom		= m_srcRect.top + desc.frameHeight;
	
	//Set up the destination rectangle
	m_destRect.left = (int)desc.position.x;
	m_destRect.top = (int)desc.position.y;
	m_destRect.right = m_destRect.left + m_frameWidth;
	m_destRect.bottom	= m_destRect.top  + m_frameHeight;

	//Record the position.
	m_position = desc.position;

	return X_OK;
}



//*****************************************************************************
/**
Destroys and animated sprite by freeing all of its allocated resources.
*/
//*****************************************************************************
XStatus XAnimSprite::destroy()
{
	return XSprite::destroy();
}



//*****************************************************************************
/**
This is the main update method for the animation sprite.  It's typically called
once per frame to update the sprite.  During this update call, the sprite will 
update it's animation state and other parameters.  Once the animation sprite
has been added to the scene, X-VP will handle this call.

@param	dt	The amount of time, in seconds, that has passed since the
					last frame update.  
@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimSprite::update(XF32 dt)
{
	if (m_playCount == 0)
		return X_OK;

	//Add dt which is the amount of time that has passed since
	//we last updated the sprite
	m_elapsedTime += dt;

	//If enough time has elapsed such that at least 1 whole frame should
	//have been elapsed, then we need to determine the number of frames
	//that we should skip based on the amount of time as well as keep
	//the left over amount of elapsed time that does not contribute to 
	//the elapsed frames for this update.  We keep this "left over" amount
	//in the m_elapsedTime variable for use during the next update method.
	if ((m_elapsedTime / m_delayFrameTime) >= 1.0)
	{
		modf((m_elapsedTime / m_delayFrameTime),&m_numFramesToElapse);
		m_elapsedTime = fmod(m_elapsedTime,m_delayFrameTime);
	}
	else
		m_numFramesToElapse = 0;

	while (m_numFramesToElapse > 0)
	{
		//Make sure we haven't gone over the play count
		if (m_playCount == 0)
			return X_OK;

		//if we are at the end of the column we must move to next column.
		if (m_currentColumn == m_endCol)
		{
			//if we are at the end of the frames PERIOD. Then reset to frame number 1
			if (m_currentRow == m_endRow)
			{
				//If this animation loops, then reset to frame 1. If it doesn't,
				//then the frame will stick on the last possible frame in the bitmap.
				if (m_playCount < 0 || m_playCount > 0)
				{
					m_currentRow = m_beginRow;
					m_currentColumn = 0;
					m_srcRect.left = 0;
					m_srcRect.top = m_beginRow * m_frameHeight;
					m_srcRect.right = m_frameWidth;
					m_srcRect.bottom = m_srcRect.top + m_frameHeight;
				} 

				if (m_playCount > 0)
					--m_playCount;
			}
			//else we are at the end of the row.
			else
			{
				++m_currentRow;
				m_currentColumn = 0;
				m_srcRect.left = 0;
				m_srcRect.top += m_frameHeight;
				m_srcRect.right = m_frameWidth;
				m_srcRect.bottom += m_frameHeight;
			}
		}
		//else we aren't at the end of a row or column so we just move 1 frame to the right.
		else
		{
			++m_currentColumn;
			m_srcRect.left += m_frameWidth;
			m_srcRect.right += m_frameWidth;
		}

		m_numFramesToElapse -= 1;
	}

	//Everything went ok.
	return X_OK;
}
	


//*****************************************************************************
/**
Sets the beginning row in the sprite image from which the animation should
start playing.  

@param	beginRow	The 0 based row number from which the animation should 
					start playing.  If the animation loops, then this is the
					row number that the next cycle of animation begins with.
					Valid values range from 0 to 1 less the total number of 
					rows in the sprite image.  For example, if an image consists
					of 5 rows and 5 columns for a total of 25 frames, you could
					have the animation start at the first frame in the image
					by setting the beginning row to 0.  Likewise, you could have
					the animation begin playing at frame 10 by setting the
					beginning row to 2.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimSprite::setBeginRow(XU32 beginRow)
{
	m_beginRow = beginRow;
	return X_OK;
}



//*****************************************************************************
/**
Gets the beginning row in the sprite image from which the animation will
start playing.  

@return	The 0 based row number in the sprite image where playback begins.
*/
//*****************************************************************************
XU32 XAnimSprite::getBeginRow()
{
	return m_beginRow;
}



//*****************************************************************************
/**
Sets the ending row in the sprite image at which the animation should
stop playing.  

@param	endRow		The 0 based row number at which the animation should 
					stop playing.  If the animation loops, then this is the
					last row number that the cycle of animation includes before
					looping back to the beginning row number.  Valid values range
					from 0 to 1 less the total number of rows in the sprite image.
					For example, if an image consists of 5 rows and 5 columns for
					a total of 25 frames, you could have the animation stop at
					the last frame in the image by setting the ending row to 4.
					Likewise, you could have the animation stop playing at frame
					9 by setting the ending row to 1.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimSprite::setEndRow(XU32 endRow)
{
	m_endRow = endRow;
	return X_OK;
}



//*****************************************************************************
/**
Gets the ending row in the sprite image at which the animation will
stop playing.  

@return	The 0 based row number in the sprite image where animation stops.
*/
//*****************************************************************************
XU32 XAnimSprite::getEndRow()
{
	return m_endRow;
}



//*****************************************************************************
/**
Gets the current row in the sprite image at which the animation is playing.  

@return	The 0 based row number in the sprite image at which the animation
		is playing.
*/
//*****************************************************************************
XU32 XAnimSprite::getCurrentRow()
{
	return m_currentRow;
}



//*****************************************************************************
/**
Gets the current column in the sprite image at which the animation is playing.  

@return	The 0 based column number in the sprite image at which the animation
		is playing.
*/
//*****************************************************************************
XU32 XAnimSprite::getCurrentColumn()
{
	return m_currentColumn;
}



//*****************************************************************************
/**
Gets the frame width of the sprite.     

@return	The frame width of the sprite image in pixels.
*/
//*****************************************************************************
XU32 XAnimSprite::getFrameWidth()
{
	return m_frameWidth;
}



//*****************************************************************************
/**
Gets the frame height of the sprite.    

@return	The frame height of the sprite image in pixels.
*/
//*****************************************************************************
XU32 XAnimSprite::getFrameHeight()
{
	return m_frameHeight;
}



//*****************************************************************************
/**
Sets the playback speed of the sprite animation.    

@param	frameDelay	The amount of time that elapses before the animation advances
					to the next frame.  This value is specified in seconds.  Negative
					values are not valid.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimSprite::setFrameDelay(XF32 frameDelay)
{
	//delayFrameTime cannot be less than or equal to 0.  If it is,
	//then default to 1 second.
	if (X_MATH_IS_XF32_ZERO(frameDelay))
	{
		m_delayFrameTime = 1;
		return X_ERR_INVALID_PARAMS;
	}
	else
	{
		m_delayFrameTime = frameDelay;
		return X_OK;
	}
}



//*****************************************************************************
/**
Gets the playback speed of the sprite animation.    

@return	The amount of time that elapses before the animation advances
		to the next frame.  This value is specified in seconds. 
*/
//*****************************************************************************
XF32 XAnimSprite::getFrameDelay()
{
	return m_delayFrameTime;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAnimSprite::setPlayCount(XS32 playCount)
{
	m_playCount = playCount;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XAnimSprite::getPlayCount()
{
	return m_playCount;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAnimSprite::play(XS32 playCount)
{
	if (playCount == 0)
		return X_ERR_INVALID_PARAMS;
	m_playCount = playCount;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XAnimSprite::stop()
{
	m_playCount = 0;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XAnimSprite::isPlaying()
{
	return XBOOL(m_playCount != 0);
}

