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
//	Module Name:	XBaseSprite.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-22-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the base sprite.
*/
//*****************************************************************************
XBaseSprite::XBaseSprite()
{
}



//*****************************************************************************
/**
Destructor for the base sprite.
*/
//*****************************************************************************
XBaseSprite::~XBaseSprite()
{
}



//*****************************************************************************
/**
Updates the sprite.  This method is called by X-VP at the appropriate times.
When deriving a new sprite class, you can override this method to provide your
custom update functionality.

@param dt	The amount of time that has elapsed since the last update.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XBaseSprite::update(XF32 dt)
{
	return X_OK;
}



//*****************************************************************************
/**
Renders the sprite.  This method is called by X-VP at the appropriate times.
When deriving a new sprite class, you can override this method to provide your
custom rendering code.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XBaseSprite::render()
{
	return X_OK;
}