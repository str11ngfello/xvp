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
//	Module Name:	XGameServerDesc.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GAME_SERVER_DESC_H__
#define __X_GAME_SERVER_DESC_H__



//*****************************************************************************
//!XGameServer description structure.
/**
This structure is used to initialize an XGameServer object.
*/
//*****************************************************************************
struct XGameServerDesc
{
	XNetAddress address; 			/*!< Network address to use for the game server. */
	XU32 packetPoolSize;			/*!< Number of packets to allocate for the game server. */
	XU32 maxPacketSizeInBytes;		/*!< Maximum packet size in bytes. */
	XBOOL validationRequired;        /*!< Determines if players must be validated. */
	XBOOL acceptingConnections; 		/*!< Determines if the server is accepting connections. */
	XU32 maxPlayers; 				/*!< Maximum number of connected players. */

	XGameServerDesc::XGameServerDesc()
	{
		XU32 packetPoolSize = 0;
		XU32 maxPacketSizeInBytes = 0;
		acceptingConnections = true;
		maxPlayers = 64;
	}
};

#endif
