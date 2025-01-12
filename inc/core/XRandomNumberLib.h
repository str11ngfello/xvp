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
//	Module Name:	XRandomNumberLib.h
//	Author:			Lynn Duke
//	Creation Date:	5-6-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RANDOM_NUMBER_LIB_H__
#define __X_RANDOM_NUMBER_LIB_H__





//*****************************************************************************
//*****************************************************************************
void XInitRandomNumberGenerator();
void init_by_array(XU32 init_key[], XS32 key_length);
XU32 XGenRandUINT32();
XF32 XGenRandFloat01();


#endif