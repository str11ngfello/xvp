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
//	Module Name:	XActor.h
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ACTOR_H__
#define __X_ACTOR_H__




//*****************************************************************************
//*****************************************************************************
class XActor
{					  
protected:
	
	static XFreeList<XActor> m_actorFreeList;

	struct XPluginManifest
	{
		XS8* pName;
		XS8* pPluginID;
		XS8* pManifestName;
		XStringMap<XS8*> manifestMap;

		XPluginManifest()
		{
			pName = NULL;
			pPluginID = NULL;
			pManifestName = NULL;
		}

		XStatus init()
		{
			return manifestMap.init(16);
		}
		
		XStatus destroy()
		{
			if (pName)
				XOSMgr::destroyString(&pName);
			if (pPluginID)
				XOSMgr::destroyString(&pPluginID);
			if (pManifestName)
				XOSMgr::destroyString(&pManifestName);

			//Delete strings and string map
			for (XU32 i = 0;i < manifestMap.getNumBuckets();++i)
			{
				XMemBlockList<XStringMap<XS8*>::KeyValuePair>::iterator itr(manifestMap.getBucket(i)->begin());
				while (itr.isValid())
				{
					XOSMgr::destroyString(itr.getValue().getValue());
					itr.getNext();
				}
			}
			manifestMap.destroy();

			return X_OK;
		}
	};

	struct XActorManifest
	{
		XU32 numEntries;
		XMemBlockList<XPluginManifest> entries;

		XActorManifest()
		{
			numEntries = 0;
		}

		XStatus destroy()
		{
			XMemBlockList<XPluginManifest>::iterator itr(entries.begin());
			while (itr.isValid())
			{
				itr.getValue().destroy();
				itr.getNext();
			}

			numEntries = 0;
			entries.clear();

			return X_OK;
		}

		XPluginManifest* findManifest(XS8* pName,XS8* pPluginID)
		{
			XMemBlockList<XPluginManifest>::iterator itr(entries.begin());
			while (itr.isValid())
			{
				if (strcmp(pName,itr.getValue().pName) == 0)
				{
					if (strcmp(pPluginID,itr.getValue().pPluginID) == 0)
						return &itr.getValue();					
				}
				itr.getNext();
			}

			return NULL;
		}

		XStatus findManifestsByPluginID(XS8* pPluginID,XMemBlockList<XPluginManifest*>& manifests)
		{
			X_ASSERTM(pPluginID,"Invalid plugin ID");
			
			XMemBlockList<XPluginManifest>::iterator itr(entries.begin());
			while (itr.isValid())
			{
				if (strcmp(pPluginID,itr.getValue().pPluginID) == 0)
					manifests.insertBack(&itr.getValue());	
				itr.getNext();
			}

			return X_OK;
		}
	};

	XWad						m_wad;
	XActorManifest				m_actorManifest;
	XMemBlockList<XHandle>		m_texHandles;
	XMemBlockList<XHandle>		m_matHandles;
	XMemBlockList<XHandle>		m_soundHandles;
	XMemBlockList<XHandle>		m_scriptHandles;
	XMatrix4x4					m_transform;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	virtual XStatus installDependencies();

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XActor();
	virtual ~XActor();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus initLuaGlue();
	virtual XStatus initFromWadFile(XS8* pFilename);
	virtual XStatus initFromWadMemory(XS8* pMemChunk,XMatrix4x4& transform);
	virtual XStatus handleInit();
	virtual XStatus destroy();
	virtual XStatus update(XF32 dt);
	virtual XStatus addToScene(XScene& scene,XBOOL update,XBOOL render,XU32 updateLevel,XU32 renderLevel);
	virtual XStatus setWorldPosition(XVector3D position);
};

#endif
