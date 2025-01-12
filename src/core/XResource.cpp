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
//	Module Name:	XResource.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the resource.
*/
//*****************************************************************************
XResource::XResource()
{
	m_pName = NULL;
	m_refCount = 0;
}



//*****************************************************************************
/**
Destructor for the resource.
*/
//*****************************************************************************
XResource::~XResource()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XResource::destroy()
{
	XOSMgr::destroyString(&m_pName);
	return X_OK;
}



//*****************************************************************************
/**
Sets the name of the resource. 

@param pName	A string identifier for the resource.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XResource::setName(XS8* pName)
{
	if (!pName)
		return X_ERR_INVALID_PARAMS;
	else
	{
		XOSMgr::destroyString(&m_pName);
		m_pName = XOSMgr::createString(pName);
		return X_OK;
	}
}



//*****************************************************************************
/**
Gets the name of the resource.

@return The string identifier of the resource.
*/
//*****************************************************************************
XS8* XResource::getName()
{
	return m_pName;
}



//*****************************************************************************
/**
Increments the reference count for the resource.

@return None.
*/
//*****************************************************************************
void XResource::incRefCount()
{
	++m_refCount;
}



//*****************************************************************************
/**
Decrements the reference count for the resource.

@return None.
*/
//*****************************************************************************
void XResource::decRefCount()
{
	if (m_refCount != 0)
		--m_refCount;
}



//*****************************************************************************
/**
Gets the reference count for the resource.

@return The reference count for the resource.
*/
//*****************************************************************************
XU32 XResource::getRefCount()
{
	return m_refCount;
}

void XResource::setRefCount(XU32 refCount)
{
	m_refCount = refCount;
}

