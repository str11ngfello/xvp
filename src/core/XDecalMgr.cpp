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
//	Module Name:	XCamera.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the camera.  The camera is initialized with the following
default values:<br><br>

<ul>
<li> Horizontal FOV = X_MATH_PI/4 </li>
<li> Near Clipping Plane = 1.0 </li> 
<li> Far Clipping Plane = 20000 </li> 
</ul>
*/
//*****************************************************************************
XDecalMgr::XDecalMgr()
{
	m_pDecalsInUse = NULL;
	m_numActiveDecals = 0;
}



//*****************************************************************************
/**
Destructor for the camera.
*/
//*****************************************************************************
XDecalMgr::~XDecalMgr()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XDecalMgr::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_DECAL_MGR,X_SNID_DECAL_MGR);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the camera.  This is the
description that appears in log files and other debug output.

@return A string description of the camera.
*/
//*****************************************************************************
XS8* XDecalMgr::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XDecalMgr";
	return description; 
}



//*****************************************************************************
/**
Initializes the camera.

@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XDecalMgr::init(XU32 maxDecals)
{
	m_decalFreeList.init(maxDecals,"decals");
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDecalMgr::destroy()
{
	//Destroy any remaining decals
	for (XU32 i = 0;i < m_numActiveDecals;++i)
		m_decalFreeList.freeItem(m_pDecalsInUse[i]);
	m_decalFreeList.destroy();

	XDeleteArray(m_pDecalsInUse);
	m_numActiveDecals = 0;

	return X_OK;
}




//*****************************************************************************
/**
Updates the camera.  X-VP calls this method to allow the camera to update any
time dependent data. 

@param	dt	The time, in seconds, that has passed since the last update.
@param	pMatrixStack	The scene graph's matrix stack.  This parameter will
						never be NULL.
@return	A status code of type XStatus.
*/
//*****************************************************************************
XStatus XDecalMgr::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{	
	XS32 index = m_numActiveDecals;
	XS32 indexMinus1  = index - 1;
	XS32 i = 0;
	
	for (i = 0;i < index;++i)
	{
		//If the decal is dead...
		if (!(m_pDecalsInUse[i]->update(dt)))
		{
			m_decalFreeList.freeItem(m_pDecalsInUse[i]);

			//If there are more decals to process, grab one from the end of the list
			//and fill the hole created by destroying the previous decal
			if (i < (indexMinus1))
			{
				m_pDecalsInUse[i] = m_pDecalsInUse[indexMinus1];
				--i;
			}

			--index;
			--indexMinus1;
		}
	}

	m_numActiveDecals = index;

	return X_OK;
}


XStatus XDecalMgr::add(XVector3D& position,XF32 lifetime)
{
	if (m_numActiveDecals < m_decalFreeList.getMaxBlockCapacity())
	{
		XDecal* pDecal = m_decalFreeList.allocateItem();
		pDecal->init(position,lifetime);
		m_pDecalsInUse[m_numActiveDecals++] = pDecal;
		return X_OK;
	}
	else
		return X_ERR_LIMIT_REACHED;
}

XStatus XDecalMgr::removeAll()
{
	//Destroy all decals
	for (XU32 i = 0;i < m_numActiveDecals;++i)
	{
		m_pDecalsInUse[i]->destroy();
		m_decalFreeList.freeItem(m_pDecalsInUse[i]);
	}
	m_numActiveDecals = 0;

	return X_OK;
}

XStatus XDecalMgr::setMaxDecals(XU32 maxDecals)
{
	//Destroy any remaining decals
	for (XU32 i = 0;i < m_numActiveDecals;++i)
	{
		m_pDecalsInUse[i]->destroy();
		m_decalFreeList.freeItem(m_pDecalsInUse[i]);
	}
	m_decalFreeList.destroy();

	XDeleteArray(m_pDecalsInUse);
	m_numActiveDecals = 0;
	
	XU32 clampedDecals = 1;
	if (maxDecals > clampedDecals)
		clampedDecals = maxDecals;

	m_decalFreeList.init(clampedDecals,"decals");
	XNewArray(m_pDecalsInUse,XDecal*,clampedDecals,XOSMgr::getDefaultHeap());
	
	return X_OK;
}