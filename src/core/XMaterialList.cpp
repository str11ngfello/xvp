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
//	Module Name:	XMaterialList.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-1-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
/**
The constructor for the render state list.
*/
//*****************************************************************************
XMaterialList::XMaterialList()
{
}



//*****************************************************************************
/**
The destructor for the render state list.
*/
//*****************************************************************************
XMaterialList::~XMaterialList()
{
}



//*****************************************************************************
/**
Renders all the clients in the render state list.

@return A return code of type XStatus.
*/
//*****************************************************************************
/*XStatus XMaterialList::render()
{
	for (XU32 i = 1;i < m_maxResources;i++)
		if (m_resourceArray[i])
		{
			if (((XMaterial*)m_resourceArray[i])->getNumRenderEvents() != 0)
				((XMaterial*)m_resourceArray[i])->render(i);
		}
	return X_OK;
}
*/


