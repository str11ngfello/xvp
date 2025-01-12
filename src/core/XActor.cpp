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
//	Module Name:	XActor.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

#define X_ACTOR_LUA_LIB_NAME	"Actor"

static const luaL_reg XActorLuaLib[] = 
{
	{"load",LAPI_load},
	{NULL,NULL}
};

XFreeList<XActor> XActor::m_actorFreeList;


//*****************************************************************************
/**
Constructor for the WAD file.  
*/
//*****************************************************************************
XActor::XActor()
{
}



//*****************************************************************************
/**
Destructor for the WAD file.  
*/
//*****************************************************************************
XActor::~XActor()
{
}

//*****************************************************************************
//*****************************************************************************
XStatus XActor::initFromWadFile(XS8* pFilename)
{
	X_ASSERTM(pFilename,"Invalid filename");
	
	//Find the actor manifest in the wad
	m_wad.initFromFile(pFilename);
	
	XS8* pActorManifest = NULL;
	XS8* pCurrent = NULL;
	XU32 amLength = 0;
	m_wad.extractFile("Actor.manifest",pActorManifest,amLength);
	
	X_ASSERTM(pActorManifest,"Could not find the Actor.manifest file in wad");
	pCurrent = pActorManifest;

	XU32 numEntries = (XU32)*pCurrent;
	pCurrent += sizeof(XU32);
	m_actorManifest.numEntries = numEntries;

	for (XU32 i = 0;i < numEntries;++i)
	{
		//We must insert the entry first and then use the pointer
		//to initialize the entry so that it creates it's string map
		//exactly once
		XPluginManifest entry;
		m_actorManifest.entries.insertBack(entry);
		XPluginManifest* pEntry = &m_actorManifest.entries.getBack();
		pEntry->init();

		XU32 stringLen = (XU32)*pCurrent;
		pCurrent += sizeof(XU32);

		pEntry->pName = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		stringLen = (XU32)*pCurrent;
		pCurrent += sizeof(XU32);

		pEntry->pPluginID = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		stringLen = (XU32)*pCurrent;
		pCurrent += sizeof(XU32);

		pEntry->pManifestName = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		XS8* pManifestBytes = NULL;
		XU32 manifestLen = 0;
		m_wad.extractFile(pEntry->pManifestName,pManifestBytes,manifestLen);
	
		//If we found a manifest for this plugin instance, let's populate the string map
		if (pManifestBytes)
		{
			//Break up each line by the seperating '\n'.  Every two lines is a single entry.  The first
			//line in the pair is the name, the second line is the value
			XS8* nextLineToken = NULL;
			XS8* nameToken = strtok_s(pManifestBytes,"\n",&nextLineToken);
			XS8* valueToken = strtok_s(NULL,"\n",&nextLineToken);
			X_ASSERTM(nameToken,"Invalid name in manifest");
			X_ASSERTM(valueToken,"Invalid value in manifest");

			do
			{
				//We're done if we encounter and empty line
				if (!valueToken)
					break;

				nameToken[strlen(nameToken)-1] = '\0';
				valueToken[strlen(valueToken)-1] = '\0';
				
				pEntry->manifestMap.insert(nameToken,XOSMgr::createString(valueToken));
				
				nameToken = strtok_s(NULL,"\n",&nextLineToken);
				valueToken = strtok_s(NULL,"\n",&nextLineToken);
			}
			while (nameToken != NULL);
		}

		XDeleteArray(pManifestBytes);
	}

	//Don't forget to free the buffer
	XDeleteArray(pActorManifest);

	//Install actor dependencies first
	installDependencies();

	//Call user init
	handleInit();

	//Cleanup
	m_wad.destroy();

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XActor::initFromWadMemory(XS8* pMemChunk,XMatrix4x4& transform)
{
	X_ASSERTM(pMemChunk,"Invalid memchunk");
	
	//Find the actor manifest in the wad
	m_wad.initFromMemory(pMemChunk);
	
	m_transform = transform;

	XS8* pActorManifest = NULL;
	XS8* pCurrent = NULL;
	XU32 amLength = 0;
	m_wad.extractFile("Actor.manifest",pActorManifest,amLength);
	
	X_ASSERTM(pActorManifest,"Could not find the Actor.manifest file in wad");
	pCurrent = pActorManifest;

	XU32 numEntries = (XU32)*pCurrent;
	pCurrent += sizeof(XU32);
	m_actorManifest.numEntries = numEntries;

	for (XU32 i = 0;i < numEntries;++i)
	{
		//We must insert the entry first and then use the pointer
		//to initialize the entry so that it creates it's string map
		//exactly once
		XPluginManifest entry;
		m_actorManifest.entries.insertBack(entry);
		XPluginManifest* pEntry = &m_actorManifest.entries.getBack();
		pEntry->init();

		XU32 stringLen = (XU32)*pCurrent;
		pCurrent += sizeof(XU32);

		pEntry->pName = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		stringLen = (XU32)*pCurrent;
		pCurrent += sizeof(XU32);

		pEntry->pPluginID = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		stringLen = (XU32)*pCurrent;
		pCurrent += sizeof(XU32);

		pEntry->pManifestName = XOSMgr::createString(pCurrent);
		pCurrent += stringLen;

		XS8* pManifestBytes = NULL;
		XU32 manifestLen = 0;
		m_wad.extractFile(pEntry->pManifestName,pManifestBytes,manifestLen);
	
		//If we found a manifest for this plugin instance, let's populate the string map
		if (pManifestBytes)
		{
			//Break up each line by the seperating '\n'.  Every two lines is a single entry.  The first
			//line in the pair is the name, the second line is the value
			XS8* nextLineToken = NULL;
			XS8* nameToken = strtok_s(pManifestBytes,"\n",&nextLineToken);
			XS8* valueToken = strtok_s(NULL,"\n",&nextLineToken);
			X_ASSERTM(nameToken,"Invalid name in manifest");
			X_ASSERTM(valueToken,"Invalid value in manifest");

			do
			{
				//We're done if we encounter and empty line
				if (!valueToken)
					break;

				nameToken[strlen(nameToken)-1] = '\0';
				valueToken[strlen(valueToken)-1] = '\0';
				
				pEntry->manifestMap.insert(nameToken,XOSMgr::createString(valueToken));
				
				nameToken = strtok_s(NULL,"\n",&nextLineToken);
				valueToken = strtok_s(NULL,"\n",&nextLineToken);
			}
			while (nameToken != NULL);
		}

		XDeleteArray(pManifestBytes);
	}

	//Don't forget to free the buffer
	XDeleteArray(pActorManifest);

	//Install actor dependencies first
	installDependencies();

	//Call user init
	handleInit();

	//Cleanup
	m_wad.destroy();

	return X_OK;
}


XStatus XActor::handleInit()
{
	return X_OK;
}


XStatus XActor::destroy()
{
	XMemBlockList<XHandle>::iterator itr;

	//Destroy audio
	itr.set(m_soundHandles.begin());
	while (itr.isValid())
	{
		XVP::AudioMgr.destroySound(itr.getValue());
		itr.getNext();
	}
	m_soundHandles.clear();

	//Destroy materials
	itr.set(m_matHandles.begin());
	while (itr.isValid())
	{
		XVP::GraphicsMgr.destroyMaterial(itr.getValue());
		itr.getNext();
	}
	m_matHandles.clear();

	//Destroy textures
	itr.set(m_texHandles.begin());
	while (itr.isValid())
	{
		XVP::GraphicsMgr.destroyTexture(itr.getValue());
		itr.getNext();
	}
	m_texHandles.clear();

	//Destroy scripts
	itr.set(m_scriptHandles.begin());
	while (itr.isValid())
	{
		XLua::destroyScript(itr.getValue());
		itr.getNext();
	}
	m_scriptHandles.clear();

	m_actorManifest.destroy();
	return X_OK;
}


XStatus XActor::update(XF32 dt)
{
	
	return X_OK;
}


XStatus XActor::installDependencies()
{
	XS8* pBytes = NULL;
	XU32 byteLen = 0;
	XHandle tempHandle;
	XMemBlockList<XS8*> m_filenames;
	XMemBlockList<XS8*>::iterator itr;

	//Create scripts
	m_wad.findFiles(".luo",m_filenames);
	itr.set(m_filenames.begin());
	while (itr.isValid())
	{
		m_wad.extractFile(itr.getValue(),pBytes,byteLen);
		X_ASSERTF(pBytes,"Could not find script resource %s in wad file while creating actor dependencies",itr.getValue());
		XLua::createScriptFromMemory(pBytes,byteLen,tempHandle);
		m_scriptHandles.insertBack(tempHandle);
		XDeleteArray(pBytes);
		itr.getNext();
	}
	m_filenames.clear();

	//Create audio
	m_wad.findFiles(".xau",m_filenames);
	itr.set(m_filenames.begin());
	while (itr.isValid())
	{
		m_wad.extractFile(itr.getValue(),pBytes,byteLen);
		X_ASSERTF(pBytes,"Could not find audio resource %s in wad file while creating actor dependencies",itr.getValue());
		XVP::AudioMgr.createSoundFromMemory(itr.getValue(),pBytes,byteLen,tempHandle);
		m_soundHandles.insertBack(tempHandle);
		XVP::AudioMgr.setSoundLoopingState(tempHandle,true);
		XVP::AudioMgr.playSound(tempHandle);
		XDeleteArray(pBytes);
		itr.getNext();
	}
	m_filenames.clear();

	//Create textures
	m_wad.findFiles(".xif",m_filenames);
	itr.set(m_filenames.begin());
	while (itr.isValid())
	{
		m_wad.extractFile(itr.getValue(),pBytes,byteLen);
		X_ASSERTF(pBytes,"Could not find texture resource %s in wad file while creating actor dependencies",itr.getValue());
		XVP::GraphicsMgr.createTextureFromMemory(itr.getValue(),pBytes,byteLen,tempHandle);
		m_texHandles.insertBack(tempHandle);
		XDeleteArray(pBytes);
		itr.getNext();
	}
	m_filenames.clear();

	//Create materials
	m_wad.findFiles(".xma",m_filenames);
	itr.set(m_filenames.begin());
	while (itr.isValid())
	{
		m_wad.extractFile(itr.getValue(),pBytes,byteLen);
		X_ASSERTF(pBytes,"Could not find material resource %s in wad file while creating actor dependencies",itr.getValue());
		XVP::GraphicsMgr.createMaterial(pBytes,itr.getValue(),tempHandle);
		m_matHandles.insertBack(tempHandle);
		XDeleteArray(pBytes);
		itr.getNext();
	}
	m_filenames.clear();

	return X_OK;
}


XStatus XActor::addToScene(XScene& scene,XBOOL update,XBOOL render,XU32 updateLevel,XU32 renderLevel)
{
	return X_OK;
}



XStatus XActor::setWorldPosition(XVector3D position)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XActor::initLuaGlue()
{
	static XBOOL initialized = false;
	if (initialized)
		return X_ERR_REDUNDANT_OPERATION;
	else
	{
		initialized = true;
		return XLua::registerLibrary(X_ACTOR_LUA_LIB_NAME,XActorLuaLib);
	}
}