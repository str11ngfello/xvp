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
//	Module Name:	XBaseSound.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XBaseSound::XBaseSound()
{
	m_ALSource = 0;
	m_playing = false;
	m_volume = 1.0;
	m_pitch = 1.0;
	m_rolloffFactor = 1.0;
	m_loop = false;
	m_isMono = false;
	m_position[0] = m_position[1] = m_position[2] = 0;
	m_velocity[0] = m_velocity[1] = m_velocity[2] = 0;
}



//*****************************************************************************
//*****************************************************************************
XBaseSound::~XBaseSound()
{
}
