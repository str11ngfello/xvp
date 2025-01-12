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
//	Module Name:	XViewingFrustum.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VIEWING_FRUSTUM_H__
#define __X_VIEWING_FRUSTUM_H__





//****************************************************************************
//! Frustum object.
/**
This object represents a viewing frustum for X-VP.  The viewing
frustum is made up of 6 planes which are ordered as follows:

INDEX/PLANE:	<br>

0/left clipping plane <br>
1/right clipping plane <br>
2/top clipping plane <br>
3/bottom clipping plane <br>
4/near clipping plane <br>
5/far clipping plane <br>
*/
//****************************************************************************
struct XViewingFrustum
{
	XPlane frustumPlanes[6];
};

#endif