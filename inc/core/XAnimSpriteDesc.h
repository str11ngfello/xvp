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
//	Module Name:	XAnimSpriteDesc.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ANIM_SPRITE_DESC_H__
#define __X_ANIM_SPRITE_DESC_H__


//*****************************************************************************
//XAnimSprite description structure.
//*****************************************************************************
struct XAnimSpriteDesc
{     
	XVector2D position;
	XU32 frameWidth;
	XU32 frameHeight;
	XF32 delayFrameTime;
	bool visible;
	XU32 beginRow;
	XU32 endRow;
	XU32 endCol;

	XAnimSpriteDesc::XAnimSpriteDesc()
	{
		position = XVector2D(0,0);
		frameWidth = 0;
		frameHeight = 0;
		delayFrameTime = 0.0;
		visible = true;
		beginRow = 0;
		endRow = 0;
		endCol = 0;
	}
};

#endif