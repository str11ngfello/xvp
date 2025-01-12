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
//	Module Name:	X2DMovie.h
//	Author:			Lynn Duke
//	Creation Date:	6-5-04
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_2D_MOVIE_H__
#define __X_2D_MOVIE_H__




//*****************************************************************************
//! Plays movies.
/**
The X2DMovie object provides capabilites to play movie files into
rectanglular areas of the screen.  It supports operations such as play,
stop, pause, resume, etc.      

See the member functions below for more information.
*/
//*****************************************************************************
class X2DMovie : public XBaseSprite
{					  
private:
	XHandle		m_movieHandle;
	XRect		m_srcRect;
	XRect		m_destRect;
	XU32		m_width;
	XU32		m_height;
	XF32		m_opacity;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	X2DMovie();
	~X2DMovie();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pMovieFilename);
	XStatus destroy();
	XStatus render();
	XStatus play(XS32 playCount);
	XStatus pause();
	XStatus resume();
	XStatus stop();
	XStatus setTime(XU32 milliseconds);
	XU32 getTime();
	XStatus setPlayCount(XS32 playCount);
	XS32 getPlayCount();
	XU32 getDuration();
	XU32 getWidth();
	XU32 getHeight();
	bool isPlaying();
	void setSrcRect(XRect& srcRect);
	void getSrcRect(XRect& srcRect);
	void setDestRect(XRect& destRect);
	void getDestRect(XRect& destRect);
	void setPosition(XVector2D& position);
	XVector2D getPosition();
	bool intersects(XRect& r);
	XStatus setOpacity(XF32 opacity);
	XF32 getOpacity();
};


#endif
