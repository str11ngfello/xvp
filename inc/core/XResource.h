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
//	Module Name:	XResource.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RESOURCE_H__
#define __X_RESOURCE_H__




//*****************************************************************************
//! Basic resource object.
/**
The XResource provides an interface that X-VP and the drivers can use to 
standardize the way resources are dealt with.  Resources like textures, audio,
etc, derive from this object so that they can fit into X-VP's resource framework.       

See the member functions below for more information.
*/
//*****************************************************************************
class XResource
{
private:		  
	XU32 m_refCount;
	XS8* m_pName;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XResource();
	virtual ~XResource();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	XStatus setName(XS8* pName);
	XS8* getName();
    void incRefCount();
	void decRefCount();
	XU32 getRefCount();
	void setRefCount(XU32 refCount);
};


#endif 
