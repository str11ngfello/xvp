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
//	Module Name:	XResourceList.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RESOURCE_LIST_H__
#define __X_RESOURCE_LIST_H__




//*****************************************************************************
//! Holds a list of XResources.
/**
An XResourceList holds XResource objects and provides several manipulation
methods.       

See the member functions below for more information.
*/
//*****************************************************************************
class XResourceList
{
protected:		  
	XS8*			m_pName;
	XResource**		m_resourceArray;
	XU32			m_maxResources;
	XU32			m_numResources;
	XBOOL			m_inited;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XResourceList();
	virtual ~XResourceList();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pName,XU32 size);
	XStatus destroy();
	XStatus destroyAllResources();
	XStatus replaceResource(XHandle handle,XResource* pResource,XS8* pResourceName);
	XHandle addResource(XResource* pResource,XS8* pResourceName);
	XStatus removeResource(XHandle resourceHandle);
	XResource* getResource(XHandle resourceHandle);
	XResource* findResourceByName(XS8* pName,XHandle& resourceHandle);
	XU32 getNumResources();
	XU32 getCapacity();

};


#endif 
