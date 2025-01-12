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
//	Module Name:	XMatAnimTrack.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-10-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
/**
Constructor for the animation track.
*/
//*****************************************************************************
XMatAnimTrack::XMatAnimTrack()
{
	m_timeLength = 0;
	m_lastUOffsetIndex = 0;
	m_lastVOffsetIndex = 0;
	m_lastWAngleIndex = 0;
}



//*****************************************************************************
/**
Destructor for the animation track.
*/
//*****************************************************************************
XMatAnimTrack::~XMatAnimTrack()
{
}



//*****************************************************************************
/**
Initializes the animation track.  This method must be called once for the
animation track in order to properly load in the specified animation data.
You must initialize an animation track before adding it to an 
animation controller.

@param pFilename	Path to an animation file.  This file must have a ".xaf"
					extension, otherwise this call will fail. 
@return A status code of type XStatus.
*/
//*****************************************************************************
XStatus XMatAnimTrack::init(XS8* pFilename)
{
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;
	
	//Reset the data and free any previous resources.
	destroy();
	XMatAnimTrack();

	XModelLoader loader;
		
	if (loader.load(pFilename) == X_OK)
	{
		//We need to copy the data read in into this animation
		//track.  Also, we will determine the length of the animation by
		//keeping track of the longest timestamp between all keys.
		m_uOffsetKeyList.init(loader.m_model.m_matAnimationTrack.uOffsetKeyList.size(),NULL,XOSMgr::getAnimHeap());
		for (XU32 j = 0;j < loader.m_model.m_matAnimationTrack.uOffsetKeyList.size();j++)
		{
			XFloatKey* pFloatKey = m_uOffsetKeyList.insertBack();
			pFloatKey->time = loader.m_model.m_matAnimationTrack.uOffsetKeyList[j].time;
			pFloatKey->value = loader.m_model.m_matAnimationTrack.uOffsetKeyList[j].value;

			if (pFloatKey->time > m_timeLength)
				m_timeLength = pFloatKey->time;
		}
				
		m_vOffsetKeyList.init(loader.m_model.m_matAnimationTrack.vOffsetKeyList.size(),NULL,XOSMgr::getAnimHeap());
		for (XU32 j = 0;j < loader.m_model.m_matAnimationTrack.vOffsetKeyList.size();j++)
		{
			XFloatKey* pFloatKey = m_vOffsetKeyList.insertBack();
			pFloatKey->time = loader.m_model.m_matAnimationTrack.vOffsetKeyList[j].time;
			pFloatKey->value = loader.m_model.m_matAnimationTrack.vOffsetKeyList[j].value;

			if (pFloatKey->time > m_timeLength)
				m_timeLength = pFloatKey->time;
		}
		
		m_wAngleKeyList.init(loader.m_model.m_matAnimationTrack.wAngleKeyList.size(),NULL,XOSMgr::getAnimHeap());
		for (XU32 j = 0;j < loader.m_model.m_matAnimationTrack.wAngleKeyList.size();j++)
		{
			XFloatKey* pFloatKey = m_wAngleKeyList.insertBack();
			pFloatKey->time = loader.m_model.m_matAnimationTrack.wAngleKeyList[j].time;
			pFloatKey->value = loader.m_model.m_matAnimationTrack.wAngleKeyList[j].value;

			if (pFloatKey->time > m_timeLength)
				m_timeLength = pFloatKey->time;
		}
		
		//Destroy model data
		loader.destroyModelData();
		
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Destroys the animation track by freeing all of its allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMatAnimTrack::destroy()
{
	m_uOffsetKeyList.clear();
	m_vOffsetKeyList.clear();
	m_wAngleKeyList.clear();
		
	return X_OK;
}
