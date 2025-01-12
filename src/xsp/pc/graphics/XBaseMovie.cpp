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
//	Module Name:	XBaseMovie.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-31-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XMemBlockList<XBaseMovie*> XBaseMovie::m_movieList;


//*****************************************************************************
//*****************************************************************************
XBaseMovie::XBaseMovie()
{
	m_movieList.insertBack(this);
}



//*****************************************************************************
//*****************************************************************************
XBaseMovie::~XBaseMovie()
{
	m_movieList.remove(this);
}

