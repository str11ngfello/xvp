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
//	Module Name:	XTextBoxDesc.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TEXT_BOX_DESC_H__
#define __X_TEXT_BOX_DESC_H__


//*****************************************************************************
//*****************************************************************************
enum X_TB_JUST_TYPE
{
	X_TBJ_NONE,
	X_TBJ_LEFT,
	X_TBJ_RIGHT,
	X_TBJ_CENTER
};


//*****************************************************************************
//Defines for text box types. 
//*****************************************************************************
#define X_TBT_ALL		0xFF
#define X_TBT_CAPS		0x01
#define X_TBT_LOWER		0x02
#define X_TBT_SYM		0x04
#define X_TBT_NUM		0x08
#define X_TBT_RETURN	0x10
#define X_TBT_SPACE		0x20
#define X_TBT_BACK		0x40
#define X_TBT_FLOATING	0x80



//*****************************************************************************
//!Text box description structure.
/**
This structure is used to describe the settings of a text box.  
*/
//*****************************************************************************
struct XTextBoxDesc
{     
	XRect screenRect;				/*!< Rectangular screen location for text area. */	
	XU32 maxChars;					/*!< Maximum number of charaters allowed in text box. */	
    XS8 lineHeightBias;			/*!< Number of pixels to adjust the line height by */	
	XU8 charPadding;				/*!< Number of pixels to skip between successive characters. */	
	X_TB_JUST_TYPE justification;
	XU8 type;
	XBOOL showCursor;
	XF32 cursorBlinkRate;
	XHandle charSetHandle;

	XTextBoxDesc::XTextBoxDesc()
	{
		maxChars = 0;
		lineHeightBias = 0;
		charPadding = 0;
		justification = X_TBJ_LEFT;
		type = X_TBT_ALL;
		showCursor = false;
		cursorBlinkRate = .5;
		charSetHandle = X_INVALID_HANDLE;
	}
};

#endif