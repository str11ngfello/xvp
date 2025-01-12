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
//	Module Name:	XAnimTrack.cpp
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
XAnimTrack::XAnimTrack()
{
	m_timeLength = 0;
	m_pName = NULL;
}



//*****************************************************************************
/**
Destructor for the animation track.
*/
//*****************************************************************************
XAnimTrack::~XAnimTrack()
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
XStatus XAnimTrack::init(XS8* pName,XS8* pFilename)
{
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	XBSChunkHeader inHeader;
	FILE* inFile = fopen(pFilename,"rb");
	
	//Get the file size
	int lSize = XOSMgr::fileGetSize(pFilename);

	XU32 ID;
	XU32 numKeyFrames;

	while (ftell(inFile) < lSize)
	{
		fread(&inHeader,sizeof(inHeader),1,inFile);
	
		//XKeyFrameSet destKeySet;
		//m_keysList.push_back(destKeySet);
		XU32 currentList;// = m_keysList.size()-1;

		switch (inHeader.chunkID)
		{
		case XBS_ANIM_TRACK:
			fread(&ID,sizeof(ID),1,inFile);
			
			//If we have padded enough elements then we can set the current list and continue,
			//otherwise, we need to expand the number of slots so we can hold this ID.
			if (m_keysList.size() < ID+1)
			{
				//Pad the list by allocating enough slots to store this ID so that
				//during runtime we can directly index the list with this ID.
				for (XU32 i = (XU32)m_keysList.size();i < ID;++i)
				{
					XKeyFrameSet* pSet = NULL;
					m_keysList.push_back(pSet);
				}

				//Now add the slot this ID will actually reference.
				XKeyFrameSet* pSet = NULL;
				XNew(pSet,XKeyFrameSet,XOSMgr::getAnimHeap());
				m_keysList.push_back(pSet);
			}
			else
			{
				XKeyFrameSet* pSet = NULL;
				XNew(pSet,XKeyFrameSet,XOSMgr::getAnimHeap());
				X_ASSERTM(!m_keysList[ID],"Duplicate node ID found in anim track.");
				m_keysList[ID] = pSet;
			}
			currentList = ID;

			//Get keyframe size and allocate enough space
			fread(&numKeyFrames,sizeof(numKeyFrames),1,inFile);
			m_keysList[currentList]->rotKeyList.init(numKeyFrames,NULL,XOSMgr::getAnimHeap());
			for (XU32 i = 0;i < numKeyFrames;++i)
			{
				XRotationKey* pKey = m_keysList[currentList]->rotKeyList.insertBack();
				fread(&pKey->time,sizeof(pKey->time),1,inFile);
				fread(&pKey->quat,sizeof(pKey->quat),1,inFile);

				if (pKey->time > m_timeLength)
					m_timeLength = pKey->time;
			}

			fread(&numKeyFrames,sizeof(numKeyFrames),1,inFile);
			m_keysList[currentList]->posKeyList.init(numKeyFrames,NULL,XOSMgr::getAnimHeap());
			for (XU32 i = 0;i < numKeyFrames;++i)
			{
				XPositionKey* pKey = m_keysList[currentList]->posKeyList.insertBack();
				fread(&pKey->time,sizeof(pKey->time),1,inFile);
				fread(&pKey->vec,sizeof(pKey->vec),1,inFile);

				if (pKey->time > m_timeLength)
					m_timeLength = pKey->time;
			}
			
			fread(&numKeyFrames,sizeof(numKeyFrames),1,inFile);
			m_keysList[currentList]->scaleKeyList.init(numKeyFrames,NULL,XOSMgr::getAnimHeap());
			for (XU32 i = 0;i < numKeyFrames;++i)
			{
				XScaleKey* pKey = m_keysList[currentList]->scaleKeyList.insertBack();
				fread(&pKey->time,sizeof(pKey->time),1,inFile);
				fread(&pKey->vec,sizeof(pKey->vec),1,inFile);

				if (pKey->time > m_timeLength)
					m_timeLength = pKey->time;
			}
			break;
		default:
			fseek(inFile,inHeader.sizeInBytes,SEEK_CUR);
			break;
		}

		//fseek(inFile,inHeader.sizeInBytes,SEEK_CUR);		
	}

	fclose(inFile);

	if (pName)
		m_pName = XOSMgr::createString(pName);

	return X_OK;
}


XStatus XAnimTrack::initFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize)
{
	X_ASSERTM(pMemoryChunk,"Invalid mem chunk passed to init");
	X_ASSERTM(memChunkSize > 0,"Invalid mem chunk size");

	XBSChunkHeader inHeader;

	XU32 ID;
	XU32 numKeyFrames;
	XS8* pCurrent = pMemoryChunk;

	while (XU32(pCurrent - pMemoryChunk) < memChunkSize)
	{
		inHeader = *(XBSChunkHeader*)pCurrent; pCurrent += sizeof(inHeader);
	
		XU32 currentList;

		switch (inHeader.chunkID)
		{
		case XBS_ANIM_TRACK:
			ID = *(XU32*)pCurrent; pCurrent += sizeof(ID);
						
			//If we have padded enough elements then we can set the current list and continue,
			//otherwise, we need to expand the number of slots so we can hold this ID.
			if (m_keysList.size() < ID+1)
			{
				//Pad the list by allocating enough slots to store this ID so that
				//during runtime we can directly index the list with this ID.
				for (XU32 i = (XU32)m_keysList.size();i < ID;++i)
				{
					XKeyFrameSet* pSet = NULL;
					m_keysList.push_back(pSet);
				}

				//Now add the slot this ID will actually reference.
				XKeyFrameSet* pSet = NULL;
				XNew(pSet,XKeyFrameSet,XOSMgr::getAnimHeap());
				m_keysList.push_back(pSet);
			}
			else
			{
				XKeyFrameSet* pSet = NULL;
				XNew(pSet,XKeyFrameSet,XOSMgr::getAnimHeap());
				X_ASSERTM(!m_keysList[ID],"Duplicate node ID found in anim track.");
				m_keysList[ID] = pSet;
			}
			currentList = ID;

			//Get keyframe size and allocate enough space
			numKeyFrames = *(XU32*)pCurrent; pCurrent += sizeof(numKeyFrames);
			
			m_keysList[currentList]->rotKeyList.init(numKeyFrames,NULL,XOSMgr::getAnimHeap());
			for (XU32 i = 0;i < numKeyFrames;++i)
			{
				XRotationKey* pKey = m_keysList[currentList]->rotKeyList.insertBack();
				pKey->time = *(XU32*)pCurrent; pCurrent += sizeof(pKey->time);
				pKey->quat = *(XQuaternion*)pCurrent; pCurrent += sizeof(pKey->quat);
		
				if (pKey->time > m_timeLength)
					m_timeLength = pKey->time;
			}

			numKeyFrames = *(XU32*)pCurrent; pCurrent += sizeof(numKeyFrames);
			m_keysList[currentList]->posKeyList.init(numKeyFrames,NULL,XOSMgr::getAnimHeap());
			for (XU32 i = 0;i < numKeyFrames;++i)
			{
				XPositionKey* pKey = m_keysList[currentList]->posKeyList.insertBack();
				pKey->time = *(XU32*)pCurrent; pCurrent += sizeof(pKey->time);
				pKey->vec = *(XVector3D*)pCurrent; pCurrent += sizeof(pKey->vec);

				if (pKey->time > m_timeLength)
					m_timeLength = pKey->time;
			}
			
			numKeyFrames = *(XU32*)pCurrent; pCurrent += sizeof(numKeyFrames);
			m_keysList[currentList]->scaleKeyList.init(numKeyFrames,NULL,XOSMgr::getAnimHeap());
			for (XU32 i = 0;i < numKeyFrames;++i)
			{
				XScaleKey* pKey = m_keysList[currentList]->scaleKeyList.insertBack();
				pKey->time = *(XU32*)pCurrent; pCurrent += sizeof(pKey->time);
				pKey->vec = *(XVector3D*)pCurrent; pCurrent += sizeof(pKey->vec);

				if (pKey->time > m_timeLength)
					m_timeLength = pKey->time;
			}
			break;
		default:
			pCurrent += inHeader.sizeInBytes;
			break;
		}	
	}

	if (pName)
		m_pName = XOSMgr::createString(pName);

	return X_OK;
}




//*****************************************************************************
/**
Destroys the animation track by freeing all of its allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XAnimTrack::destroy()
{
	for (XU32 i = 0;i < m_keysList.size();++i)
	{
		if (m_keysList[i])
			XDelete(m_keysList[i]);
	}
	m_keysList.clear();	
	m_keysList.~vector();
	XOSMgr::destroyString(&m_pName);

	return X_OK;
}



XS8* XAnimTrack::getName()
{
	return m_pName;
}

XStatus XAnimTrack::setName(XS8* pName)
{
	if (pName)
		m_pName = XOSMgr::createString(pName);
	else
		XOSMgr::destroyString(&m_pName);

	return X_OK;
}