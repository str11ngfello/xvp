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
//	Module Name:	XResourceList.cpp
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
Constructor for the resource list.
*/
//*****************************************************************************
XResourceList::XResourceList()
{
	m_pName = NULL;
	m_resourceArray = NULL;
	m_numResources = 0;
	m_maxResources = 0;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the resource list.
*/
//*****************************************************************************
XResourceList::~XResourceList()
{
}



//*****************************************************************************
/**
Initializes a resource list to an initial size.

@param initialSize The initial number of resources the list can hold.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XResourceList::init(XS8* pName,XU32 size)
{
	X_ASSERTM(pName,"Invalid name");
	if (size == 0)
		return X_ERR_INVALID_PARAMS;

	//Create the resource array.  Assign NULL to each pointer to represent
	//an empty slot.
	m_pName = XOSMgr::createString(pName);
	m_maxResources = size;
	XNewArray(m_resourceArray,XResource*,m_maxResources,XOSMgr::getDefaultHeap());
	for (XU32 i = 0;i < m_maxResources;++i)
		m_resourceArray[i] = NULL;

	m_inited = true;

	return X_OK;
}



//*****************************************************************************
/**
Destroys all resources in the resource list.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XResourceList::destroy()
{
	//Clean up resource array.
	XOSMgr::destroyString(&m_pName);
	destroyAllResources();
	XDeleteArray(m_resourceArray);
	m_inited = false;

	return X_OK;
}


XStatus XResourceList::replaceResource(XHandle handle,XResource* pResource,XS8* pResourceName)
{
	X_ASSERT(m_inited);
	X_ASSERT(pResource);
	
	//The user is requesting us to insert the resource into a specific slot.  That
	//slot must be in use.  This method only allows the user to replace a currently
	//allocated resource
	X_ASSERT((handle > 0 && handle < m_maxResources));

	m_resourceArray[handle] = pResource;
	if (pResourceName)
		pResource->setName(pResourceName);	

	return X_OK;
}

//*****************************************************************************
/**
Adds a resource to the resource list.  If the resource list is full, it's
capacity will be doubled before adding the new resource.

@param pResource	The resource to add.
@param pResourceName A string identifier to use for the resource.  This parameter
					 is optional.
@return A handle to the resource.
*/
//*****************************************************************************
XHandle XResourceList::addResource(XResource* pResource,XS8* pResourceName)
{
	if (!m_inited)
		return X_INVALID_HANDLE;

	//Validate resource pointer.
	if (!pResource)
		return X_INVALID_HANDLE;
	
	//We need to find the first empty slot in the array
	//and stick this resource there.
	for (XU32 i = 1;i < m_maxResources;++i)
	{
		if (!m_resourceArray[i])
		{	
			if (pResourceName)
				pResource->setName(pResourceName);	
			pResource->incRefCount();
			m_resourceArray[i] = pResource;
			++m_numResources;
			return i;
		}
	}
	
	//If we make it here then the resource list is maxed out.  Assert
	//and notify the user they need to increase the size of the list
	X_DEBUG
	(
		XS8 buf[256];
		sprintf(buf,"Resource list \"%s\" has exhausted it's capacity and needs to be increased.",m_pName);
		X_ASSERT_FORCED(buf);
	)

	return X_INVALID_HANDLE;
	/*
	XResource** tempArray = new XResource*[m_maxResources*2];

	//Make sure we were able to allocate enough space.
	if (!tempArray)
		return X_INVALID_HANDLE;

	for (XU32 ctr = 0;ctr < (m_maxResources * 2);++ctr)
		tempArray[ctr] = NULL;
	memcpy(tempArray,m_resourceArray,(m_maxResources * sizeof(XResource*)));
	XDeleteArray(m_resourceArray);
	m_resourceArray = tempArray;	
	m_maxResources = (m_maxResources * 2);	

	return addResource(pResource,pResourceName);*/
}



//*****************************************************************************
/**
Gets a resource from the resource list.

@param resourceHandle	The handle of the resource to retrieve.
@return The queried resource.  If the resource could not be found, this
		parameter will be NULL.
*/
//*****************************************************************************
XResource* XResourceList::getResource(XHandle resourceHandle)
{
	X_ASSERTM(m_inited,"Resource list not initialized");
	X_ASSERTM((resourceHandle > 0) && (resourceHandle <= (m_maxResources - 1)),"Invalid resource handle");

	return m_resourceArray[resourceHandle];
}



//*****************************************************************************
/**
Removes a resource from the resource list.

@param resourceHandle	The handle of the resource to remove.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XResourceList::removeResource(XHandle resourceHandle)
{
	if (!m_inited)
		return X_ERR_NOT_INITIALIZED;

	//If resource handle is valid, destroy the resource.
	if ((resourceHandle > 0) && (resourceHandle <= (m_maxResources - 1))) 
		if (m_resourceArray[resourceHandle])
		{		
			//Decrement the reference count.  If the reference count has gone
			//to 0, then this resource is truly no longer in use.  Destroy it.
			m_resourceArray[resourceHandle]->decRefCount();
			if (m_resourceArray[resourceHandle]->getRefCount() == 0)
			{
				m_resourceArray[resourceHandle]->destroy();
				XDelete(m_resourceArray[resourceHandle]);
				--m_numResources;
			}

			return X_OK;
		}

	//Resource handle was invalid.
	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Removes all resources from the resource list.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XResourceList::destroyAllResources()
{
	if (!m_inited)
		return X_ERR_NOT_INITIALIZED;

	//Check to make sure the array is still valid
	if (!m_resourceArray)
		return X_ERR_OPERATION_FAILED;

	//Destroy all resource regardless of their reference counts.
	for (XU32 i = 1;i < m_maxResources;++i)
	{
		if (m_resourceArray[i])
		{		
			m_resourceArray[i]->destroy();
			XDelete(m_resourceArray[i]);
			--m_numResources;
		}
	}

	return X_OK;
}



//*****************************************************************************
/**
Search the resource list for a particular resource by using a string identifier.

@param pName	This is the string identifier given to the resource when it
				was added to the list.
@param resourceHandle	A variable to hold the handle of the retrieved resource.
						If the resource could not be found, this parameter will
						be set to X_INVALID_HANDLE.
@param A return code of type XStatus.
*/
//*****************************************************************************
XResource* XResourceList::findResourceByName(XS8* pName,XHandle& resourceHandle)
{
	if (!m_inited)
		return NULL;

	//Validate the resource name.
	if (pName)
	{
		XS8* pResourceName = NULL;

		//We need to find the resource by name.  Spin through the resource
		//list looking for the passed in name.
		for (XU32 i = 1;i < m_maxResources;++i)
		{
			if (m_resourceArray[i])
			{	
				//Get the resource's name
				pResourceName = m_resourceArray[i]->getName();

				//If it had a name
				if (pResourceName)
				{
					//If it matches, we've found the resource we're
					//looking for.
					if (strcmp(pResourceName,pName) == 0)
					{
						resourceHandle = i;
						return m_resourceArray[i];
					}
				}			
			}
		}
	}
	
	//No resource with that name
	resourceHandle = X_INVALID_HANDLE;
	return NULL;
}
	


//*****************************************************************************
/**
Returns the number of resources in the resource list.  This is not the capacity.

@return The number of resources in the list.
*/
//*****************************************************************************
XU32 XResourceList::getNumResources()
{
	return m_numResources;
}



//*****************************************************************************
/**
Returns the capacity of the resource list.  I.e, the number of resources the
list can hold.

@return The capacity of the resource list.
*/
//*****************************************************************************
XU32 XResourceList::getCapacity()
{
	return m_maxResources;
}
