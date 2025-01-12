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
//	Module Name:	XAnimSprite.h
//	Author:			Lynn Duke
//	Creation Date:	12-17-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ANIM_SPRITE_H__
#define __X_ANIM_SPRITE_H__





//*****************************************************************************
//! 2D Sprite object that supports animated images.
/**
The XAnimSprite object is similar to the XSprite object in that it allows
you to display a 2D image.  However, the XAnimSprite gives you the
capability to cycle through individual portions of the 2D image, namely frames,
in an almost "film strip" manner.

For example, if you have an single image file that contains 20 frames, this
object will allow you to play those frames sequentially to achieve traditional
2D sprite animation.  There are several parameters that control exactly which
frames are played, how fast the frames are cycled, loop count, and so on.

See the member functions below for more information.
*/
//*****************************************************************************
class XAnimSprite : public XSprite
{
protected:
	XU32			m_currentRow;
	XU32			m_currentColumn;		
	XU32			m_beginRow;
	XU32			m_endRow;
	XU32			m_endCol;
	XU32			m_frameWidth;
	XU32			m_frameHeight;
	XF32			m_delayFrameTime;
	XF32			m_elapsedTime;
	XF32			m_numFramesToElapse;
	XS32			m_playCount;

public:
	
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XAnimSprite();
	virtual ~XAnimSprite();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pTextureFilename,XAnimSpriteDesc& desc);
	XStatus init(XAnimSpriteDesc& desc);
	virtual XStatus destroy();
	XStatus setBeginRow(XU32 begRow);
	XU32 getBeginRow();
	XStatus setEndRow(XU32 endRow);
	XU32 getEndRow();
	XU32 getCurrentRow();
	XU32 getCurrentColumn();
	XU32 getFrameWidth();
	XU32 getFrameHeight();
	XStatus setFrameDelay(XF32 frameDelay);
	XF32 getFrameDelay();
	XStatus setPlayCount(XS32 playCount);
	XS32 getPlayCount();
	XStatus play(XS32 playCount);
	XStatus stop();
	XBOOL isPlaying();
	virtual XStatus update(XF32 dt);
};



#endif