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
//	Module Name:	CharacterActor.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************
#include <xvpsdk.h>
#include <characterActor.h>


 
//*****************************************************************************
/**
Constructor for the WAD file.  
*/
//*****************************************************************************
CharacterActor::CharacterActor()
{
	m_pAnimController = NULL;
	//m_pAnimTracks = NULL;
}



//*****************************************************************************
/**
Destructor for the WAD file.  
*/
//*****************************************************************************
CharacterActor::~CharacterActor()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus CharacterActor::handleInit()
{
	//Must call base class first
	XActor::handleInit();

	//Init string maps
	m_modelTemplates.init(1);
	m_models.init(1);
	m_particleSystemTemplates.init(1);
	m_particleSystems.init(1);
	m_materials.init(1);
	m_cameras.init(1);
	m_lights.init(1);
	m_controllers.init(1);

	//Load in actor components
	loadControllers();
	loadModels();
	loadParticleSystems();
	loadMaterials();
	loadCameras();
	loadLights();

	//Look for animation controller
	/*pPluginManifest = m_actorManifest.findManifest("Character Animations","Animation");
	if (pPluginManifest)
	{
		XNew(m_pAnimController,XAnimController(),XOSMgr::getAnimHeap());
		m_pAnimController->init(atoi(*pPluginManifest->manifestMap.getItem("NumTracks")));
		XS8* p = *pPluginManifest->manifestMap.getItem("Tracks");
		XS8* pToken = strtok_s(p,":",&p);
		while (pToken != NULL)
		{
			m_wad.extractFile(pToken,pBytes,byteLength);
			X_ASSERTF(pPluginManifest,"Could not find anim track %s in wad file",pToken);
			m_animTrack.initFromMemory(pBytes,byteLength);
			XDeleteArray(pBytes);
			pToken = strtok_s(p,":",&p);
		}
		
		m_pAnimController->addTrack(&m_animTrack,m_atHandle);
		m_pAnimController->playTrack(m_atHandle,1.0f,-1,1.0f);

		p = *pPluginManifest->manifestMap.getItem("_C0");
		pToken = strtok_s(p,":",&p);
		while (pToken != NULL)
		{
			m_model.attachController(pToken,m_pAnimController);			
			pToken = strtok_s(p,":",&p);
		}

	}
	
*/
	
/*
	//Texture
	pPluginManifest = m_actorManifest.findManifest("ExtraTex","Texture");
	X_ASSERTM(pPluginManifest,"Could not find manifest for texture");
	p = *pPluginManifest->manifestMap.getItem("Name");
	m_wad.extractFile(p,pBytes,byteLength);
	XHandle thandle;
	XVP::GraphicsMgr.createTexture(pBytes,byteLength,p,thandle);
	XDeleteArray(pBytes);


	//Audio clip
	pPluginManifest = m_actorManifest.findManifest("AClip","Audio Clip");
	X_ASSERTM(pPluginManifest,"Could not find manifest for audio clip");
	p = *pPluginManifest->manifestMap.getItem("Name");
	m_wad.extractFile(p,pBytes,byteLength);
	//XHandle thandle;
	//XVP::GraphicsMgr.createTexture(pBytes,byteLength,p,thandle);
	XDeleteArray(pBytes);
*/
	return X_OK;
}

XStatus CharacterActor::destroy()
{
	//Must call base class first
	XActor::destroy();
	
	destroyModels();
	destroyParticleSystems();
	destroyMaterials();
	destroyCameras();
	destroyLights();
	destroyControllers();

	XMemBlockList<XAnimTrack*>::iterator i(m_animTracks.begin());
	while(i.isValid())
	{
		i.getValue()->destroy();
		XDelete(i.getValue());
	}
	m_animTracks.clear();

	if (m_pAnimController)
	{
		m_pAnimController->destroy();
		XDelete(m_pAnimController);
	}
	m_animTrack.destroy();
	return X_OK;
}


XStatus CharacterActor::addToScene(XScene& scene,XBOOL update,XBOOL render,XU32 updateLevel,XU32 renderLevel)
{
	for (XU32 i = 0;i < m_models.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XModel*>::KeyValuePair>::iterator itr(m_models.getBucket(i)->begin());
		while (itr.isValid())
		{
			if (update)
				scene.addUpdateNode(*itr.getValue().getValue(),updateLevel);
			if (render)
				scene.addRenderNode(*itr.getValue().getValue(),renderLevel);
			itr.getNext();
		}
	}
	
	for (XU32 i = 0;i < m_particleSystems.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XParticleSystem*>::KeyValuePair>::iterator itr(m_particleSystems.getBucket(i)->begin());
		while (itr.isValid())
		{
			if (update)
				scene.addUpdateNode(*itr.getValue().getValue(),updateLevel);
			if (render)
				scene.addRenderNode(*itr.getValue().getValue(),3);
			itr.getNext();
		}
	}

	for (XU32 i = 0;i < m_cameras.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XCamera*>::KeyValuePair>::iterator itr(m_cameras.getBucket(i)->begin());
		while (itr.isValid())
		{
			if (update)
				scene.addUpdateNode(*itr.getValue().getValue(),updateLevel);
			itr.getNext();
		}
	}

	for (XU32 i = 0;i < m_lights.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XLight*>::KeyValuePair>::iterator itr(m_lights.getBucket(i)->begin());
		while (itr.isValid())
		{
			if (update)
				scene.addUpdateNode(*itr.getValue().getValue(),updateLevel);
			itr.getNext();
		}
	}

	return X_OK;
}

XStatus CharacterActor::setWorldPosition(XVector3D position)
{
	for (XU32 i = 0;i < m_models.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XModel*>::KeyValuePair>::iterator itr(m_models.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->setModelPosition(position);
			itr.getNext();
		}
	}

	for (XU32 i = 0;i < m_particleSystems.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XParticleSystem*>::KeyValuePair>::iterator itr(m_particleSystems.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->setModelPosition(position);
			itr.getNext();
		}
	}

	for (XU32 i = 0;i < m_cameras.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XCamera*>::KeyValuePair>::iterator itr(m_cameras.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->setModelPosition(position);
			itr.getNext();
		}
	}

	for (XU32 i = 0;i < m_lights.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XLight*>::KeyValuePair>::iterator itr(m_lights.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->setModelPosition(position);
			itr.getNext();
		}
	}

	return X_OK;
}

XStatus CharacterActor::loadModels()
{
	XMemBlockList<XPluginManifest*> pluginManifests;
	m_actorManifest.findManifestsByPluginID("Model",pluginManifests);
	XMemBlockList<XPluginManifest*>::iterator itr(pluginManifests.begin());
	while (itr.isValid())
	{
		XModelTemplate* pMT = NULL;
		XModel* pM = NULL;
		XNew(pMT,XModelTemplate,XOSMgr::getModelHeap());
		XNew(pM,XModel,XOSMgr::getModelHeap());

		XS8* pBytes = NULL;
		XU32 byteLength = 0;
		XU32 numLODs = atoi(*itr.getValue()->manifestMap.getItem("NumLODs"));
		X_ASSERTM(numLODs >= 1,"Expected at least one LOD for model");
		m_wad.extractFile(*itr.getValue()->manifestMap.getItem("LOD 0"),pBytes,byteLength);
		pMT->initFromMemory(pBytes,byteLength,numLODs);
		XDeleteArray(pBytes);

		//Load in remaining LODs if they exist
		XS8 lodName[16];
		for (XU32 i = 1;i < numLODs;++i)
		{
			sprintf(lodName,"LOD %d",i);
			m_wad.extractFile(*itr.getValue()->manifestMap.getItem(lodName),pBytes,byteLength);
			pMT->mergeLODFromMemory(pBytes,byteLength,i*100);
			XDeleteArray(pBytes);
		}

		//Create model and set transform
		pMT->generateModel(*pM);
		pM->setModelTransform(m_transform);
	pM->attachController(NULL,*m_controllers.getItem("Animation"),true);
		m_modelTemplates.insert(itr.getValue()->pName,pMT);
		m_models.insert(itr.getValue()->pName,pM);

		itr.getNext();
	}

	pluginManifests.clear();

	return X_OK;
}


XStatus CharacterActor::destroyModels()
{
	for (XU32 i = 0;i < m_models.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XModel*>::KeyValuePair>::iterator itr(m_models.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}	
	for (XU32 i = 0;i < m_modelTemplates.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XModelTemplate*>::KeyValuePair>::iterator itr(m_modelTemplates.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}

	m_modelTemplates.clear();
	m_models.clear();

	return X_OK;
}


XStatus CharacterActor::loadParticleSystems()
{
	XMemBlockList<XPluginManifest*> pluginManifests;
	m_actorManifest.findManifestsByPluginID("Particle System",pluginManifests);
	XMemBlockList<XPluginManifest*>::iterator itr(pluginManifests.begin());
	while (itr.isValid())
	{
		XParticleSystemTemplate* pPST = NULL;
		XParticleSystem* pPS = NULL;
		XNew(pPST,XParticleSystemTemplate,XOSMgr::getModelHeap());
		XNew(pPS,XParticleSystem,XOSMgr::getModelHeap());

		XS8* pBytes = NULL;
		XU32 byteLength = 0;
		m_wad.extractFile(*itr.getValue()->manifestMap.getItem("Name"),pBytes,byteLength);

		pPST->initFromMemory(pBytes);
		XDeleteArray(pBytes);

		//Create particle system and set transform
		pPST->generateParticleSystem(*pPS);
		pPS->setModelTransform(m_transform);

		m_particleSystemTemplates.insert(itr.getValue()->pName,pPST);
		m_particleSystems.insert(itr.getValue()->pName,pPS);

		itr.getNext();
	}

	pluginManifests.clear();

	return X_OK;
}


XStatus CharacterActor::destroyParticleSystems()
{
	for (XU32 i = 0;i < m_particleSystems.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XParticleSystem*>::KeyValuePair>::iterator itr(m_particleSystems.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}	
	for (XU32 i = 0;i < m_particleSystemTemplates.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XParticleSystemTemplate*>::KeyValuePair>::iterator itr(m_particleSystemTemplates.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}

	m_particleSystemTemplates.clear();
	m_particleSystems.clear();

	return X_OK;
}


XStatus CharacterActor::loadMaterials()
{
	XMemBlockList<XPluginManifest*> pluginManifests;
	m_actorManifest.findManifestsByPluginID("Material",pluginManifests);
	XMemBlockList<XPluginManifest*>::iterator itr(pluginManifests.begin());
	while (itr.isValid())
	{
		XHandle matHandle = XVP::GraphicsMgr.getMaterial(*itr.getValue()->manifestMap.getItem("Name"));
		X_ASSERTM(matHandle != X_INVALID_HANDLE,"Could not locate a material expected to be installed");

		m_materials.insert(itr.getValue()->pName,matHandle);

		itr.getNext();
	}

	pluginManifests.clear();

	return X_OK;
}


XStatus CharacterActor::destroyMaterials()
{
	for (XU32 i = 0;i < m_materials.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XHandle>::KeyValuePair>::iterator itr(m_models.getBucket(i)->begin());
		while (itr.isValid())
		{
			XVP::GraphicsMgr.destroyMaterial((*itr.getValue().getValue()));
			itr.getNext();
		}
	}	

	m_materials.clear();

	return X_OK;
}


XStatus CharacterActor::loadCameras()
{
	XMemBlockList<XPluginManifest*> pluginManifests;
	m_actorManifest.findManifestsByPluginID("Camera",pluginManifests);
	XMemBlockList<XPluginManifest*>::iterator itr(pluginManifests.begin());
	while (itr.isValid())
	{
		XCamera* pCam = NULL;
		XNew(pCam,XCamera,XOSMgr::getCameraHeap());

		XS8* pBytes = NULL;
		XU32 byteLength = 0;
		m_wad.extractFile(*itr.getValue()->manifestMap.getItem("CameraFile"),pBytes,byteLength);

		pCam->initFromMemory(pBytes,byteLength);
		XDeleteArray(pBytes);

		//Set transform
		pCam->setModelTransform(m_transform);

		m_cameras.insert(*itr.getValue()->manifestMap.getItem("CameraName"),pCam);

		itr.getNext();
	}

	pluginManifests.clear();

	return X_OK;
}


XStatus CharacterActor::destroyCameras()
{
	for (XU32 i = 0;i < m_cameras.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XCamera*>::KeyValuePair>::iterator itr(m_cameras.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}	

	m_cameras.clear();

	return X_OK;
}


XStatus CharacterActor::loadLights()
{
	XMemBlockList<XPluginManifest*> pluginManifests;
	m_actorManifest.findManifestsByPluginID("Light",pluginManifests);
	XMemBlockList<XPluginManifest*>::iterator itr(pluginManifests.begin());
	while (itr.isValid())
	{
		XLight* pLight = NULL;
		XNew(pLight,XLight,XOSMgr::getModelHeap());

		XS8* pBytes = NULL;
		XU32 byteLength = 0;
		m_wad.extractFile(*itr.getValue()->manifestMap.getItem("LightFile"),pBytes,byteLength);

		pLight->initFromMemory(pBytes,byteLength);
		XDeleteArray(pBytes);

		//Set transform
		pLight->setModelTransform(m_transform);

		m_lights.insert(*itr.getValue()->manifestMap.getItem("LightName"),pLight);

		itr.getNext();
	}

	pluginManifests.clear();

	return X_OK;
}


XStatus CharacterActor::destroyLights()
{
	for (XU32 i = 0;i < m_lights.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XLight*>::KeyValuePair>::iterator itr(m_lights.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}	

	m_lights.clear();

	return X_OK;
}


XStatus CharacterActor::loadControllers()
{
	XMemBlockList<XPluginManifest*> pluginManifests;

	//Animation Controllers
	m_actorManifest.findManifestsByPluginID("Animation",pluginManifests);
	XMemBlockList<XPluginManifest*>::iterator itr(pluginManifests.begin());
	while (itr.isValid())
	{
		XAnimController* pController = NULL;
		XNew(pController,XAnimController,XOSMgr::getModelHeap());

		XS8* pBytes = NULL;
		XU32 byteLength = 0;
		pController->init(atoi(*itr.getValue()->manifestMap.getItem("NumTracks")));
		XS8* p = *itr.getValue()->manifestMap.getItem("Tracks");
		XS8* pToken = strtok_s(p,":",&p);
		XHandle tempHandle;
		while (pToken != NULL)
		{
			m_wad.extractFile(pToken,pBytes,byteLength);
			X_ASSERTF(pToken,"Could not find anim track %s in wad file",pToken);
			XAnimTrack* pAnimTrack;
			XNew(pAnimTrack,XAnimTrack,XOSMgr::getAnimHeap());
			pAnimTrack->initFromMemory(pToken,pBytes,byteLength);
			pController->addTrack(pAnimTrack,tempHandle);
			XDeleteArray(pBytes);
			m_animTracks.insertBack(pAnimTrack);
			pToken = strtok_s(p,":",&p);
		}
		pController->playTrack(0,1,-1,1.0f);
		m_controllers.insert(itr.getValue()->pName,pController);
		itr.getNext();
	}
	pluginManifests.clear();

	//Physics Controllers


	return X_OK;
}


XStatus CharacterActor::destroyControllers()
{
	for (XU32 i = 0;i < m_controllers.getNumBuckets();++i)
	{
		XMemBlockList<XStringMap<XController*>::KeyValuePair>::iterator itr(m_controllers.getBucket(i)->begin());
		while (itr.isValid())
		{
			(*itr.getValue().getValue())->destroy();
			XDelete(*itr.getValue().getValue());
			itr.getNext();
		}
	}	

	m_controllers.clear();

	return X_OK;
}