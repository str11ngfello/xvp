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
//	Module Name:	XOSMgrLuaGlue.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-29-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


static const XS8* XOSLuaLibDirectory[] = 
{

	NULL
};




int LAPI_os(lua_State* pLuaState)
{
	XS8** p = (XS8**)XOSLuaLibDirectory;
	while (*p)
	{
		X_CONSOLE(XConsole::setLastCommandResponseString(NULL,*p));
		++p;
	}
	return 0;
}


