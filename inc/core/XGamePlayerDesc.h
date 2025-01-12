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
//	Module Name:	XGamePlayerDesc.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GAME_PLAYER_DESC_H__
#define __X_GAME_PLAYER_DESC_H__



//*****************************************************************************
//!XGamePlayer description structure.
/**
This structure is used to initialize an XGamePlayer object.
*/
//*****************************************************************************
struct XGamePlayerDesc
{
	XNetAddress address;            /*!< Network address to use for the game player. */
	XU32 packetPoolSize;           /*!< Number of packets to allocate for the game player. */
	XU32 maxPacketSizeInBytes;     /*!< Maximum packet size in bytes. */

	XGamePlayerDesc::XGamePlayerDesc()
	{
		XU32 packetPoolSize = 0;
		XU32 maxPacketSizeInBytes = 0;
	}
};

#endif