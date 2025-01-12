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
//	Module Name:	XBaseSprite.h
//	Author:			Lynn Duke
//	Creation Date:	2-22-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_SPRITE_H__
#define __X_BASE_SPRITE_H__





//*****************************************************************************
//! Defines the interface requirements for sprites.
/**
The is the base object for all sprites managed by X-VP.  The term
"sprite" in the context of X-VP is any 2D object that is rendered
outside of the context of the 3D world.  For example, a sprite can be a 
static bitmap, animated image, cursor or even a movie.

The XBaseSprite object defines the interface requirements that all sprites
must implement to be compatible with X-VP.  Typically, objects that are 
derived from XBaseSprite provide custom functionality by overriding key
member functions such as update() and render().

If you decide to create a custom sprite object, you must derive it from
this class so that X-VP knows how to manage it.  By deriving
from this class, your sprite can be added to X-VP's 2D Scene Graph and
will be subjected to the same z ordering rules as other sprites.
*/
//*****************************************************************************
class XBaseSprite
{
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseSprite();
	virtual ~XBaseSprite();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus update(XF32 dt);
	virtual XStatus render();
};


#endif