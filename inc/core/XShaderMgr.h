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
//	Module Name:	XShaderMgr.h
//	Author:			Lynn Duke
//	Creation Date:	7-7-07
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SHADER_MGR_H__
#define __X_SHADER_MGR_H__




//*****************************************************************************
//*****************************************************************************
class XShaderMgr
{
private:
	static XObjectFactory<XShaderController>;

public:
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus init();
	static XStatus destroy();

}



#endif