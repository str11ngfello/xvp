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
//	Module Name:	XMaterialList.h
//	Author:			Lynn Duke
//	Creation Date:	2-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MATERIAL_LIST_H_
#define __X_MATERIAL_LIST_H_





//*****************************************************************************
//! Holds a list of XResources.
/**
A render state list is a special kind of list that holds render state resources.
X-VP uses render state lists in conjunction with rendering the scene graph.       

See the member functions below for more information.
*/
//*****************************************************************************
class XMaterialList : public XResourceList
{
private:		  
	

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMaterialList();
	~XMaterialList();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	//XStatus render();
};


#endif 
