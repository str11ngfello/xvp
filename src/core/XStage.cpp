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
//	Module Name:	XStage.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
/**
Constructor for the WAD file.  
*/
//*****************************************************************************
XStage::XStage()
{
}



//*****************************************************************************
/**
Destructor for the WAD file.  
*/
//*****************************************************************************
XStage::~XStage()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XStage::initFromWadFile(XS8* pFilename)
{
	X_ASSERTM(pFilename,"Invalid filename");
	
	m_actorMap.init(256);

	//Find the stage manifest in the wad
	m_wad.initFromFile(pFilename);
	
	XS8* pStageManifest = NULL;
	XS8* pCurrent = NULL;
	XU32 smLength = 0;
	m_wad.extractFile("stage.manifest",pStageManifest,smLength);
	
	X_ASSERTM(pStageManifest,"Could not find the stage.manifest file in wad");
	pCurrent = pStageManifest;

	XU32 numEntries = (XU32)*pCurrent;
	pCurrent += sizeof(XU32);

	XU32 stringLen = 0;
	XS8* pActorType = NULL;
	XS8* pActorName = NULL;
	
	for (XU32 i = 0;i < numEntries;++i)
	{	
		ActorInfo ai;

		XU32 stringLen = *(XU32*)pCurrent;
		pCurrent += sizeof(XU32);

		pActorName = pCurrent;
		pCurrent += stringLen;

		stringLen = *(XU32*)pCurrent;
		pCurrent += sizeof(XU32);

		ai.pManifest = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		ai.transform = *(XMatrix4x4*)pCurrent;
		pCurrent += sizeof(XMatrix4x4);

		//Insert new entry into hashtable
		m_actorMap.insert(pActorName,ai);
	}

	//Don't forget to free the buffer
	XDeleteArray(pStageManifest);

	return X_OK;
}

XStatus XStage::generateActor(XS8* pActorName,XActor& actor)
{
	//Find the actor given the name
	ActorInfo* pActorInfo = m_actorMap.getItem(pActorName);
	X_ASSERTF(pActorInfo,"Could not find actor %s in stage %s",pActorName,m_wad.getFilename());
		
	//Extract the actor wad 
	XS8* pActorWadBytes = NULL;
	XU32 actorByteLen = 0;
	m_wad.extractFile(pActorInfo->pManifest,pActorWadBytes,actorByteLen);
	X_ASSERTM(pActorWadBytes,"Could not locate actor wad in stage");

	//Now initialize the actor with the wad file in memory
	actor.initFromWadMemory(pActorWadBytes,pActorInfo->transform);

	//Delete the actor wad
	XDelete(pActorWadBytes);

	return X_OK;
}

XStatus XStage::destroy()
{
	//Delete strings and string map
	for (XU32 i = 0;i < m_actorMap.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<ActorInfo>::KeyValuePair>::iterator itr(m_actorMap.getBucket(i)->begin());
		while (itr.isValid())
		{
			XOSMgr::destroyString(&itr.getValue().getValue()->pManifest);
			itr.getNext();
		}
	}
	m_actorMap.destroy();
	
	//Cleanup
	m_wad.destroy();

	return X_OK;
}



