// This is the main DLL file.

#include "XVPManaged.h"
#include "xvpsdk.h"
#include "ParticleSystem.h"
#include "MaterialNP.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "ActorContainer.h"
#include "AnimController.h"
#include "PhysXController.h"

namespace XVPManaged
{
	XVPM::XVPM()
	{
		m_shadowSampler = X_INVALID_HANDLE;
	}

	XVPM::~XVPM()
	{
	}

	bool XVPM::inited()
	{
		return m_inited;
	}
	void XVPM::init(IntPtr hInstance)
	{
		if (XVP::init(hInstance.ToPointer()) != X_OK)
		{
			MessageBoxA(NULL,"XVP failed to initialize.","",MB_OK);
		}

		//Initialize physics
		if (XPhysXController::initPhysics() != X_OK)
		{
			MessageBoxA(NULL,"Failed to start physics engine.","",MB_OK);
		}
	
		//Register shader controllers
		XVP::ShaderFactory.registerObject<XWhiteSC>("White.fx");
		XVP::ShaderFactory.registerObject<XVertexColorSC>("Vertex Color.fx");
		XVP::ShaderFactory.registerObject<XMaterialColorSC>("Material Color.fx");
		XVP::ShaderFactory.registerObject<XParticleSystemBlinnPhongSC>("Particle System-Blinn Phong.fx");
		XVP::ShaderFactory.registerObject<XParticleSystemNoLightSC>("Particle System-No Light.fx");
		XVP::ShaderFactory.registerObject<XModelBlinnPhongSC>("Model-Blinn Phong.fx");
		XVP::ShaderFactory.registerObject<XModelNoLightSC>("Model-No Light.fx");
		XVP::ShaderFactory.registerObject<XDistortionSC>("Model-Distortion.fx");
		
		XSamplerDesc samplerDesc;
		samplerDesc.addressU = X_TAT_CLAMP;
		samplerDesc.addressW = X_TAT_CLAMP;
		samplerDesc.addressV = X_TAT_CLAMP;
		samplerDesc.filter = X_TFT_MIN_MAG_MIP_POINT;
		XVP::GraphicsMgr.createSamplerState(samplerDesc,m_shadowSampler);


		for (XU32 i = 0;i < MAX_CONTROLS;++i)
		{
			m_renderContexts[i] = X_INVALID_HANDLE;
			m_pScenes[i] = NULL;
			m_pDistortionScenes[i] = NULL;
			
			m_sceneViews[i].cameras[0].init();
			m_sceneViews[i].cameras[1].init();
			m_sceneViews[i].cameras[2].init();
			m_sceneViews[i].cameras[3].init();
			m_sceneViews[i].distortionCameras[0].init();
			m_sceneViews[i].distortionCameras[1].init();
			m_sceneViews[i].distortionCameras[2].init();
			m_sceneViews[i].distortionCameras[3].init();
			m_sceneViews[i].cameras[0].setID(0);
			m_sceneViews[i].cameras[1].setID(1);
			m_sceneViews[i].cameras[2].setID(2);
			m_sceneViews[i].cameras[3].setID(3);
			m_sceneViews[i].distortionCameras[0].setID(0);
			m_sceneViews[i].distortionCameras[1].setID(1);
			m_sceneViews[i].distortionCameras[2].setID(2);
			m_sceneViews[i].distortionCameras[3].setID(3);
			
			m_sceneViews[i].cameras[1].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[i].cameras[1].setOrthoZoom(INITIAL_ORTHO_ZOOM);
			m_sceneViews[i].distortionCameras[1].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[i].distortionCameras[1].setOrthoZoom(INITIAL_ORTHO_ZOOM);
			m_sceneViews[i].cameras[2].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[i].cameras[2].setOrthoZoom(INITIAL_ORTHO_ZOOM);
			m_sceneViews[i].distortionCameras[2].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[i].distortionCameras[2].setOrthoZoom(INITIAL_ORTHO_ZOOM);
			m_sceneViews[i].cameras[3].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[i].cameras[3].setOrthoZoom(INITIAL_ORTHO_ZOOM);
			m_sceneViews[i].distortionCameras[3].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[i].distortionCameras[3].setOrthoZoom(INITIAL_ORTHO_ZOOM);

			m_sceneViews[i].gridSC.init();
			m_sceneViews[i].grids[0].init(1.0f,20,10);
			m_sceneViews[i].grids[0].attachShaderController(NULL,&m_sceneViews[i].gridSC);
			m_sceneViews[i].grids[0].setCameraID(0);
			m_sceneViews[i].grids[1].init(1.0f,20,10);
			m_sceneViews[i].grids[1].attachShaderController(NULL,&m_sceneViews[i].gridSC);
			m_sceneViews[i].grids[1].setCameraID(1);
			m_sceneViews[i].grids[2].init(1.0f,20,10);
			m_sceneViews[i].grids[2].attachShaderController(NULL,&m_sceneViews[i].gridSC);
			m_sceneViews[i].grids[2].setCameraID(2);
			m_sceneViews[i].grids[3].init(1.0f,20,10);
			m_sceneViews[i].grids[3].attachShaderController(NULL,&m_sceneViews[i].gridSC);
			m_sceneViews[i].grids[3].setCameraID(3);		
		}

		m_inited = true;
	}



	XU32 XVPM::addWindow(System::IntPtr windowHandle,XU32 width,XU32 height)
	{
		//Find a free handle and create a new scene
		XU32 sceneIndex = 0;
		for (sceneIndex = 0;sceneIndex < MAX_CONTROLS;++sceneIndex)
		{
			if (m_pScenes[sceneIndex] == NULL)
				break;
		}

		X_ASSERTM(sceneIndex != MAX_CONTROLS,"Scene limit reached.  Could not create a new scene.");
		
		//Allocate new render context and new scene
		XNew(m_pScenes[sceneIndex],XScene(),XOSMgr::getDefaultHeap());
		XNew(m_pDistortionScenes[sceneIndex],XScene(),XOSMgr::getDefaultHeap());
		
		//Create a render context
		XRenderContextDesc rcDesc;
		XU32 initialWidth = width;
		XU32 initialHeight = height;
		rcDesc.pWindowHandle = (void*)windowHandle;
		rcDesc.width = initialWidth;
		rcDesc.height = initialHeight;
		rcDesc.format = X_DFT_R8G8B8A8_UNORM;
		XVP::GraphicsMgr.createRenderContext(rcDesc,m_renderContexts[sceneIndex]);
		XVP::GraphicsMgr.setRenderContextClearColor(m_renderContexts[sceneIndex],VIEWPORT_COLOR);

		//**************************************************************************
		//*******************  Main Scene Creation *********************************
		//**************************************************************************
		XSceneDesc sceneDesc;
		//sceneDesc.renderContext = m_renderContexts[sceneIndex];
		strcpy(sceneDesc.name,"Main Scene");
		sceneDesc.autoAlphaRenderLevel = RL_ALPHA;
		//sceneDesc.maxPreRenderEvents = 8192;
		sceneDesc.enableResizeEvents = true;
		
		//Render Targets
		sceneDesc.renderTargetArrayDesc.numRenderTargets = 2;
		sceneDesc.renderTargetArrayDesc.width = initialWidth;
		sceneDesc.renderTargetArrayDesc.height = initialHeight;
		sceneDesc.renderTargetArrayDesc.formats[0] = X_DFT_R16G16B16A16_FLOAT;
		sceneDesc.renderTargetArrayDesc.formats[1] = X_DFT_R16G16_FLOAT;
		sceneDesc.renderTargetArrayDesc.useDepthStencil = true;
		sceneDesc.renderTargetArrayDesc.clearColors[0] = VIEWPORT_COLOR;
		sceneDesc.renderTargetArrayDesc.clearColors[1] = XColor(0,0,0,0);

		//Viewports
		sceneDesc.viewportArrayDesc.numViewports = 1;
		sceneDesc.viewportArrayDesc.viewports[0].topLeftX = 0;
		sceneDesc.viewportArrayDesc.viewports[0].topLeftY = 0;
		sceneDesc.viewportArrayDesc.viewports[0].width = initialWidth;
		sceneDesc.viewportArrayDesc.viewports[0].height = initialHeight;
		sceneDesc.viewportArrayDesc.enablePostEffects[0] = true;
	
		//Update levels
		sceneDesc.numUpdateLevels = NUM_UPDATE_LEVELS;
		strcpy(sceneDesc.updateLevelDescs[UL_DEFAULT].name,"DEFAULT");

		//Render levels
		sceneDesc.numRenderLevels = NUM_RENDER_LEVELS;
		strcpy(sceneDesc.renderLevelDescs[RL_OPAQUE].name,"OPAQUE");
		sceneDesc.renderLevelDescs[RL_OPAQUE].sortType = X_ST_FRONT_TO_BACK;
		sceneDesc.renderLevelDescs[RL_OPAQUE].maxRenderEvents = 2048;
		sceneDesc.renderLevelDescs[RL_OPAQUE].autoAlphaManagement = true;
		
		strcpy(sceneDesc.renderLevelDescs[RL_OCCLUSION].name,"OCCLUSION");
		sceneDesc.renderLevelDescs[RL_OCCLUSION].sortType = X_ST_BACK_TO_FRONT;
		sceneDesc.renderLevelDescs[RL_OCCLUSION].maxRenderEvents = 512;
		
		strcpy(sceneDesc.renderLevelDescs[RL_ALPHA].name,"ALPHA");
		sceneDesc.renderLevelDescs[RL_ALPHA].sortType = X_ST_BACK_TO_FRONT;
		sceneDesc.renderLevelDescs[RL_ALPHA].maxRenderEvents = 2048;
		
		strcpy(sceneDesc.renderLevelDescs[RL_PARTICLES].name,"PARTICLES");
		sceneDesc.renderLevelDescs[RL_PARTICLES].sortType = X_ST_BACK_TO_FRONT;
		sceneDesc.renderLevelDescs[RL_PARTICLES].maxRenderEvents = 1024;
		sceneDesc.renderLevelDescs[RL_PARTICLES].depthBufferSamplerSlot = 11;

		strcpy(sceneDesc.renderLevelDescs[RL_DEBUG].name,"DEBUG");
		sceneDesc.renderLevelDescs[RL_DEBUG].sortType = X_ST_BACK_TO_FRONT;
		sceneDesc.renderLevelDescs[RL_DEBUG].maxRenderEvents = 2048;

		//Initialize main scene
		m_pScenes[sceneIndex]->init(sceneDesc);
		m_pScenes[sceneIndex]->getLightMgr()->setLightingMode(X_LM_DEFAULT);

		//**************************************************************************
		//*******************  Distortion Scene Creation ***************************
		//**************************************************************************
		//sceneDesc.renderContext = m_renderContexts[sceneIndex];
		strcpy(sceneDesc.name,"Distortion Scene");
		sceneDesc.autoAlphaRenderLevel = 0;
		//sceneDesc.maxPreRenderEvents = 8192;
		sceneDesc.enableResizeEvents = true;
		
		//Render Targets
		sceneDesc.renderTargetArrayDesc.numRenderTargets = 1;
		sceneDesc.renderTargetArrayDesc.width = initialWidth;
		sceneDesc.renderTargetArrayDesc.height = initialHeight;
		sceneDesc.renderTargetArrayDesc.formats[0] = X_DFT_R8G8B8A8_UNORM;
		sceneDesc.renderTargetArrayDesc.useDepthStencil = false;
		//Zero distortion a normal of .5 .5 .5 before biasing so set the clear color to that
		sceneDesc.renderTargetArrayDesc.clearColors[0] = XColor(.5f,.5f,.5f,1);

		//Viewports
		sceneDesc.viewportArrayDesc.numViewports = 1;
		sceneDesc.viewportArrayDesc.viewports[0].topLeftX = 0;
		sceneDesc.viewportArrayDesc.viewports[0].topLeftY = 0;
		sceneDesc.viewportArrayDesc.viewports[0].width = initialWidth;
		sceneDesc.viewportArrayDesc.viewports[0].height = initialHeight;
		sceneDesc.viewportArrayDesc.enablePostEffects[0] = false;
		sceneDesc.viewportArrayDesc.copyToRenderContext[0] = false;

		//Update levels
		sceneDesc.numUpdateLevels = 1;
		strcpy(sceneDesc.updateLevelDescs[0].name,"UL_DEFAULT");
		
		//Render levels
		sceneDesc.numRenderLevels = 1;
		strcpy(sceneDesc.renderLevelDescs[0].name,"RL_DEFAULT");
		sceneDesc.renderLevelDescs[0].sortType = X_ST_NONE;
		sceneDesc.renderLevelDescs[0].maxRenderEvents = 2048;
		sceneDesc.renderLevelDescs[0].autoAlphaManagement = false;

		//Initialize scene
		m_pDistortionScenes[sceneIndex]->init(sceneDesc);
		XVP::GraphicsMgr.overrideDepthBuffer(m_pDistortionScenes[sceneIndex]->getRenderTargetArray(),
											 m_pScenes[sceneIndex]->getRenderTargetArray());

		//Add scenes in the order we want.  Scenes are updated and rendered in the order they
		//are added to the graphics manager. If we need to add a scene somewhere other than
		//then end, we can specify a dependent scene and our new scene will be added before
		//the dependent
		XVP::GraphicsMgr.addScene(m_renderContexts[sceneIndex],0,m_pDistortionScenes[sceneIndex],NULL);
		XVP::GraphicsMgr.addScene(m_renderContexts[sceneIndex],0,m_pScenes[sceneIndex],NULL);


		//Initialize scene map
		m_sceneMap[sceneIndex].init(5);
		m_sceneMap[sceneIndex].insert("Main",m_pScenes[sceneIndex]);
		m_sceneMap[sceneIndex].insert("Distortion",m_pDistortionScenes[sceneIndex]);
		
		for (XU32 i = 0;i < 4;++i)
		{
			m_pScenes[sceneIndex]->addUpdateNode(&m_sceneViews[sceneIndex].cameras[i]);
			m_sceneViews[sceneIndex].cameras[i].attachController(&m_sceneViews[sceneIndex].PRZControllers[i]);	
			m_pDistortionScenes[sceneIndex]->addUpdateNode(&m_sceneViews[sceneIndex].distortionCameras[i]);
			m_sceneViews[sceneIndex].distortionCameras[i].attachController(&m_sceneViews[sceneIndex].PRZControllers[i]);	
		}

		//Create lights 
		XLightDesc ld;
		ld.priority = SYSTEM_LIGHT_PRIORITY;
		ld.type = X_LT_DIRECTIONAL;
		ld.enableOcclusion = true;
		XMatrix4x4 rotMatrix;
		XMatrix4x4 angleMatrix;
		XMatrix4x4 transMatrix;
		transMatrix.createTranslation(XVector3D(0.0f,300.0f,-300.0f));
		angleMatrix.createXRotation(m_sceneViews[sceneIndex].lightAngle);
		//rotMatrix.createYRotation(-1.57f);
		m_sceneViews[sceneIndex].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix * transMatrix);
		rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120));
		m_sceneViews[sceneIndex].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
		rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(240));
		m_sceneViews[sceneIndex].scenarioLights[2].setModelTransform(angleMatrix * rotMatrix);
		m_sceneViews[sceneIndex].scenarioLights[0].init(ld);
		m_sceneViews[sceneIndex].scenarioLights[1].init(ld);
		m_sceneViews[sceneIndex].scenarioLights[2].init(ld);

		//The occlusion scenes need to be added
		XVP::GraphicsMgr.addScene(m_renderContexts[sceneIndex],0,m_sceneViews[sceneIndex].scenarioLights[0].getOcclusionScene(),m_pScenes[sceneIndex]);
		XVP::GraphicsMgr.addScene(m_renderContexts[sceneIndex],0,m_sceneViews[sceneIndex].scenarioLights[1].getOcclusionScene(),m_pScenes[sceneIndex]);
		XVP::GraphicsMgr.addScene(m_renderContexts[sceneIndex],0,m_sceneViews[sceneIndex].scenarioLights[2].getOcclusionScene(),m_pScenes[sceneIndex]);

		//Override the main scene sampler slots with the shadow maps
		m_pScenes[sceneIndex]->setTextureOverrideFromRTA(12,m_shadowSampler,m_sceneViews[sceneIndex].scenarioLights[0].getOcclusionScene()->getRenderTargetArray(),0);
		m_pScenes[sceneIndex]->setTextureOverrideFromRTA(13,m_shadowSampler,m_sceneViews[sceneIndex].scenarioLights[1].getOcclusionScene()->getRenderTargetArray(),0);
		m_pScenes[sceneIndex]->setTextureOverrideFromRTA(14,m_shadowSampler,m_sceneViews[sceneIndex].scenarioLights[2].getOcclusionScene()->getRenderTargetArray(),0);

		//Disable shadows by default
		enableShadows(sceneIndex,false);

		resetViewports(sceneIndex);	
		

		m_pScenes[sceneIndex]->setViewportCamera(0,&m_sceneViews[sceneIndex].cameras[0]);
		m_pDistortionScenes[sceneIndex]->setViewportCamera(0,&m_sceneViews[sceneIndex].distortionCameras[0]);
		
		m_pScenes[sceneIndex]->addUpdateNode(&m_sceneViews[sceneIndex].grids[0]);
		m_pScenes[sceneIndex]->addUpdateNode(&m_sceneViews[sceneIndex].grids[1]);
		m_pScenes[sceneIndex]->addUpdateNode(&m_sceneViews[sceneIndex].grids[2]);
		m_pScenes[sceneIndex]->addUpdateNode(&m_sceneViews[sceneIndex].grids[3]);
		m_pScenes[sceneIndex]->addRenderNode(&m_sceneViews[sceneIndex].grids[0]);
		m_pScenes[sceneIndex]->addRenderNode(&m_sceneViews[sceneIndex].grids[1]);
		m_pScenes[sceneIndex]->addRenderNode(&m_sceneViews[sceneIndex].grids[2]);
		m_pScenes[sceneIndex]->addRenderNode(&m_sceneViews[sceneIndex].grids[3]);

		m_sceneViews[sceneIndex].hdrPostEffects[0].init(64,64);
		m_sceneViews[sceneIndex].hdrPostEffects[1].init(64,64);
		m_sceneViews[sceneIndex].hdrPostEffects[2].init(64,64);
		m_sceneViews[sceneIndex].hdrPostEffects[3].init(64,64);
		m_sceneViews[sceneIndex].cameras[0].addPostEffect(&m_sceneViews[sceneIndex].hdrPostEffects[0]);
		m_sceneViews[sceneIndex].cameras[1].addPostEffect(&m_sceneViews[sceneIndex].hdrPostEffects[1]);
		m_sceneViews[sceneIndex].cameras[2].addPostEffect(&m_sceneViews[sceneIndex].hdrPostEffects[2]);
		m_sceneViews[sceneIndex].cameras[3].addPostEffect(&m_sceneViews[sceneIndex].hdrPostEffects[3]);

		m_pScenes[sceneIndex]->getSelectionMaterialSC()->setColor(XColor(1,1,1,1));
		
		return sceneIndex;
	}

	void XVPM::removeWindow(XU32 handle)
	{
		if (m_pScenes[handle] != NULL)
		{
			//Remove scenario lights from scene and destroy them
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[handle]->removeUpdateNode(&m_sceneViews[handle].scenarioLights[i]);
			for (XU32 i = 0;i < 3;++i)
				m_sceneViews[handle].scenarioLights[i].destroy();

			m_sceneMap[handle].destroy();

			m_pScenes[handle]->removeAllUpdateNodes();
			m_pScenes[handle]->removeAllRenderNodes();
			m_pDistortionScenes[handle]->removeAllUpdateNodes();
			m_pDistortionScenes[handle]->removeAllRenderNodes();

			//Clear all viewport cameras
			m_pScenes[handle]->removeAllViewportCameras();
			m_pDistortionScenes[handle]->removeAllViewportCameras();

			m_pScenes[handle]->destroy();
			XDelete(m_pScenes[handle]);
			m_pScenes[handle] = NULL;

			m_pDistortionScenes[handle]->destroy();
			XDelete(m_pDistortionScenes[handle]);
			m_pDistortionScenes[handle] = NULL;

			m_sceneViews[handle].hdrPostEffects[0].destroy();
			m_sceneViews[handle].hdrPostEffects[1].destroy();
			m_sceneViews[handle].hdrPostEffects[2].destroy();
			m_sceneViews[handle].hdrPostEffects[3].destroy();
			m_sceneViews[handle].cameras[0].removePostEffect(&m_sceneViews[handle].hdrPostEffects[0]);
			m_sceneViews[handle].cameras[1].removePostEffect(&m_sceneViews[handle].hdrPostEffects[1]);
			m_sceneViews[handle].cameras[2].removePostEffect(&m_sceneViews[handle].hdrPostEffects[2]);
			m_sceneViews[handle].cameras[3].removePostEffect(&m_sceneViews[handle].hdrPostEffects[3]);

			XVP::GraphicsMgr.destroyRenderContext(m_renderContexts[handle]);
			m_renderContexts[handle] = X_INVALID_HANDLE;
		}
	}

	void XVPM::resizeWindow(XU32 handle,XU32 width,XU32 height)
	{
		if (m_pScenes[handle] != NULL)
			XVP::GraphicsMgr.resizeWindow(m_renderContexts[handle],width,height);

		//Don't forget to re-override the distortion's depth buffer since resizing will
		//cause scenes to recreate their render target arrays
		XVP::GraphicsMgr.overrideDepthBuffer(m_pDistortionScenes[handle]->getRenderTargetArray(),
											 m_pScenes[handle]->getRenderTargetArray());

	}

	void XVPM::update(XF32 dt)
	{
		for (int i = 0;i < MAX_CONTROLS;++i)
		{
			if (m_pScenes[i] == NULL)
				continue;
			
			//If lights need to be updated
			if (m_sceneViews[i].lightAnimSpeed > 0.0f && m_sceneViews[i].currentLightingMode == X_LM_SYSTEM)
			{
				m_sceneViews[i].lightAnimAngle += m_sceneViews[i].lightAnimSpeed * dt;
				XMatrix4x4 rotMatrix;
				XMatrix4x4 angleMatrix;
				angleMatrix.createXRotation(m_sceneViews[i].lightAngle);
				rotMatrix.createYRotation(m_sceneViews[i].lightAnimAngle);
				m_sceneViews[i].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
				rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120) + m_sceneViews[i].lightAnimAngle);
				m_sceneViews[i].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
				rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(240) + m_sceneViews[i].lightAnimAngle);
				m_sceneViews[i].scenarioLights[2].setModelTransform(angleMatrix * rotMatrix);
			}

			//Update distortion cameras
			/*for (XU32 j = 0;j < 4;++j)
			{
				if (m_sceneViews[i].userCameras[j] == NULL)
					m_sceneViews[i].distortionCameras[j].setModelTransform(m_sceneViews[i].cameras[j].getModelTransform());	
				else
					m_sceneViews[i].distortionCameras[j].setModelTransform(m_sceneViews[i].userCameras[j]->getModelTransform());	
			}*/
		}

		XVP::update();
	}

	void XVPM::destroy()
	{	
		//Destroy and deallocate render contexts and scenes
		for (int i = 0;i < MAX_CONTROLS;++i)
		{
			if (m_pScenes[i] != NULL)
				removeWindow(i);
			
			m_sceneViews[i].hdrPostEffects[0].destroy();
			m_sceneViews[i].hdrPostEffects[1].destroy();
			m_sceneViews[i].hdrPostEffects[2].destroy();
			m_sceneViews[i].hdrPostEffects[3].destroy();

			m_sceneViews[i].PRZControllers[0].destroy();
			m_sceneViews[i].PRZControllers[1].destroy();
			m_sceneViews[i].PRZControllers[2].destroy();
			m_sceneViews[i].PRZControllers[3].destroy();

			m_sceneViews[i].cameras[0].destroy();
			m_sceneViews[i].cameras[1].destroy();
			m_sceneViews[i].cameras[2].destroy();
			m_sceneViews[i].cameras[3].destroy();
			m_sceneViews[i].distortionCameras[0].destroy();
			m_sceneViews[i].distortionCameras[1].destroy();
			m_sceneViews[i].distortionCameras[2].destroy();
			m_sceneViews[i].distortionCameras[3].destroy();

			m_sceneViews[i].grids[0].destroy();
			m_sceneViews[i].grids[1].destroy();
			m_sceneViews[i].grids[2].destroy();
			m_sceneViews[i].grids[3].destroy();
			m_sceneViews[i].gridSC.destroy();
		}

		XVP::GraphicsMgr.destroySamplerState(m_shadowSampler);

		//Shutdown physics
		XPhysXController::destroyPhysics();
	
		XVP::destroy();
		XOSMgr::dumpAllocations();
		m_inited = false;
	}

	
	void XVPM::activate(XU32 handle,XBOOL activate)
	{
		m_pScenes[handle]->enable(activate);
		m_pDistortionScenes[handle]->enable(activate);
	}

	XBOOL XVPM::isGridShown(XU32 handle,XU32 viewportIndex)
	{
		return m_sceneViews[handle].grids[viewportIndex].isRenderEnabled();
	}
	
	void XVPM::showGrid(XU32 handle,XU32 viewportIndex,XBOOL show)
	{
		m_sceneViews[handle].grids[viewportIndex].enableRender(show);
	}

	void XVPM::configureGrid(XU32 handle,XF32 unitSize,XU32 gridSize,XU32 majorLine)
	{
		m_sceneViews[handle].grids[0].init(unitSize,gridSize,majorLine);
		m_sceneViews[handle].grids[1].init(unitSize,gridSize,majorLine);
		m_sceneViews[handle].grids[2].init(unitSize,gridSize,majorLine);
		m_sceneViews[handle].grids[3].init(unitSize,gridSize,majorLine);
		m_sceneViews[handle].grids[0].attachShaderController(NULL,&m_sceneViews[handle].gridSC);
		m_sceneViews[handle].grids[1].attachShaderController(NULL,&m_sceneViews[handle].gridSC);
		m_sceneViews[handle].grids[2].attachShaderController(NULL,&m_sceneViews[handle].gridSC);
		m_sceneViews[handle].grids[3].attachShaderController(NULL,&m_sceneViews[handle].gridSC);
	}

	XU32 XVPM::getGridSize(XU32 handle)
	{
		return m_sceneViews[handle].grids[0].getNumCellsAcross();
	}

	XF32 XVPM::getGridUnitSize(XU32 handle)
	{
		return m_sceneViews[handle].grids[0].getGridCellWidth();
	}

	XU32 XVPM::getGridMajorLine(XU32 handle)
	{
		return m_sceneViews[handle].grids[0].getMajorLine();
	}

	XS32 XVPM::getViewType(XU32 handle,XU32 viewportIndex)
	{
		return m_sceneViews[handle].viewTypes[viewportIndex];
	}

	void XVPM::setViewType(XU32 handle,XU32 viewportIndex,XS32 viewType,XBOOL preserveZoom)
	{
		m_sceneViews[handle].viewTypes[viewportIndex] = (ViewType)viewType;
		setCameraViewOrientation(m_sceneViews[handle].PRZControllers[viewportIndex],(ViewType)viewType,preserveZoom);

		//Rotate grids according to view
		XMatrix4x4 rotMat;
		switch ((ViewType)viewType)
		{
		case VT_LEFT:
		case VT_RIGHT:
			rotMat.createZRotation(X_MATH_PI/2.0f);
			m_sceneViews[handle].grids[viewportIndex].setModelTransform(rotMat);
			m_sceneViews[handle].cameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[handle].distortionCameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			if (m_sceneViews[handle].userCameras[viewportIndex] != NULL)
				m_sceneViews[handle].userCameras[viewportIndex]->setProjectionType(ORTHOGRAPHIC);
			break;
		case VT_TOP:
		case VT_BOTTOM:
			m_sceneViews[handle].grids[viewportIndex].setModelTransform(rotMat);
			m_sceneViews[handle].cameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[handle].distortionCameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			if (m_sceneViews[handle].userCameras[viewportIndex] != NULL)
				m_sceneViews[handle].userCameras[viewportIndex]->setProjectionType(ORTHOGRAPHIC);
			break;
		case VT_PERSPECTIVE:
			m_sceneViews[handle].grids[viewportIndex].setModelTransform(rotMat);
			m_sceneViews[handle].cameras[viewportIndex].setProjectionType(PERSPECTIVE);
			m_sceneViews[handle].distortionCameras[viewportIndex].setProjectionType(PERSPECTIVE);
			if (m_sceneViews[handle].userCameras[viewportIndex] != NULL)
				m_sceneViews[handle].userCameras[viewportIndex]->setProjectionType(PERSPECTIVE);
			break;
		case VT_FRONT:
		case VT_BACK:
			rotMat.createXRotation(X_MATH_PI/2.0f);
			m_sceneViews[handle].grids[viewportIndex].setModelTransform(rotMat);
			m_sceneViews[handle].cameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[handle].distortionCameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			if (m_sceneViews[handle].userCameras[viewportIndex] != NULL)
				m_sceneViews[handle].userCameras[viewportIndex]->setProjectionType(ORTHOGRAPHIC);
			break;
		}

		setCameraOrthoType(handle,viewportIndex,viewType);
	}
		

	void XVPM::addCameraRotation(XU32 handle,XU32 viewportIndex,XF32 xAngle,XF32 yAngle)
	{
		if (inputEnabled && m_sceneViews[handle].viewTypes[viewportIndex] == PERSPECTIVE)
			m_sceneViews[handle].PRZControllers[viewportIndex].addRotation(xAngle,yAngle);
	}
		
	void XVPM::addCameraPan(XU32 handle,XU32 viewportIndex,XS32 startX,XS32 startY,XS32 endX,XS32 endY)
	{
		if (inputEnabled)
		{
			//Get the right camera
			XCamera* pCam = NULL;
			if (m_sceneViews[handle].userCameras[viewportIndex] == NULL)
				pCam = &m_sceneViews[handle].cameras[viewportIndex];
			else
				pCam = m_sceneViews[handle].userCameras[viewportIndex];

			XVector3D line1Start,line1End,line2Start,line2End;
			XICreatePickLine(startX,
							 startY,
							 pCam->getViewportWidth(),
							 pCam->getViewportHeight(),
							 pCam->getViewMatrix(),
							 pCam->getProjMatrix(),
							 line1Start,
							 line1End);
			XICreatePickLine(endX,
							 endY,
							 pCam->getViewportWidth(),
							 pCam->getViewportHeight(),
							 pCam->getViewMatrix(),
							 pCam->getProjMatrix(),
							 line2Start,
							 line2End);

			XVector3D intersectionPoint1,intersectionPoint2;
			XF32 time;


			XILinePlane(line1Start,line1End,XVector3D(0,0,0),-XVector3D(pCam->getWorldTransform().row3),intersectionPoint1,time);
			XILinePlane(line2Start,line2End,XVector3D(0,0,0),-XVector3D(pCam->getWorldTransform().row3),intersectionPoint2,time);
			
			m_sceneViews[handle].PRZControllers[viewportIndex].addPan(-(intersectionPoint2 - intersectionPoint1));
		}
	}

	void XVPM::addCameraZoom(XU32 handle,XU32 viewportIndex,XF32 distance)
	{
		if (inputEnabled)
		{
			if (m_sceneViews[handle].cameras[viewportIndex].getProjectionType() == PERSPECTIVE)
			{
				m_sceneViews[handle].PRZControllers[viewportIndex].addZoom(distance * (getCameraZoom(handle,viewportIndex) / 1000.0f));
			}
			else
			{
				//Zoom the correct camera if a user camera is supplied
				if (m_sceneViews[handle].userCameras[viewportIndex] == NULL)
				{
					m_sceneViews[handle].cameras[viewportIndex].setOrthoZoom(m_sceneViews[handle].cameras[viewportIndex].getOrthoZoom() + (m_sceneViews[handle].cameras[viewportIndex].getOrthoZoom() * distance * .001f));
					m_sceneViews[handle].distortionCameras[viewportIndex].setOrthoZoom(m_sceneViews[handle].cameras[viewportIndex].getOrthoZoom());
				}
				else
				{
					m_sceneViews[handle].userCameras[viewportIndex]->setOrthoZoom(m_sceneViews[handle].userCameras[viewportIndex]->getOrthoZoom() + (m_sceneViews[handle].userCameras[viewportIndex]->getOrthoZoom() * distance * .001f));
					m_sceneViews[handle].distortionCameras[viewportIndex].setOrthoZoom(m_sceneViews[handle].userCameras[viewportIndex]->getOrthoZoom());
				}
			}
		}
	}

	void XVPM::getCameraRotation(XU32 handle,XS32 viewportIndex,IntPtr rotation)
	{
		XVector2D vec = m_sceneViews[handle].PRZControllers[viewportIndex].getRotation();
		XVector2D* pVec = (XVector2D*)rotation.ToPointer();
		pVec->x = vec.x;
		pVec->y = vec.y;
	}
	
	void XVPM::getCameraPan(XU32 handle,XS32 viewportIndex,IntPtr pan)
	{
		XVector3D vec = m_sceneViews[handle].PRZControllers[viewportIndex].getTarget();
		XVector3D* pVec = (XVector3D*)pan.ToPointer();
		pVec->x = vec.x;
		pVec->y = vec.y;
		pVec->z = vec.z;
	}

	XF32 XVPM::getCameraZoom(XU32 handle,XS32 viewportIndex)
	{
		if (m_sceneViews[handle].cameras[viewportIndex].getProjectionType() == PERSPECTIVE)
			return m_sceneViews[handle].PRZControllers[viewportIndex].getZoom();
		else
		{
			if (m_sceneViews[handle].userCameras[viewportIndex] == NULL)
				return m_sceneViews[handle].cameras[viewportIndex].getOrthoZoom();
			else
				return m_sceneViews[handle].userCameras[viewportIndex]->getOrthoZoom();
		}
	}

	void XVPM::setCameraRotation(XU32 handle,XU32 viewportIndex,IntPtr rotation)
	{
		XVector2D* pVec = (XVector2D*)rotation.ToPointer();
		m_sceneViews[handle].PRZControllers[viewportIndex].setRotation(pVec->x,pVec->y);
	}

	void XVPM::setCameraPan(XU32 handle,XU32 viewportIndex,IntPtr pan)
	{
		XVector3D* pVec = (XVector3D*)pan.ToPointer();
		m_sceneViews[handle].PRZControllers[viewportIndex].setTarget(*pVec);
	}

	void XVPM::setCameraZoom(XU32 handle,XU32 viewportIndex,XF32 distance)
	{
		m_sceneViews[handle].PRZControllers[viewportIndex].setZoom(distance);
	}

	
	void XVPM::setCameraOrthoType(XU32 handle,XU32 viewportIndex,XS32 viewType)
	{
		if (m_sceneViews[handle].userCameras[viewportIndex] == NULL)
		{
			m_sceneViews[handle].cameras[viewportIndex].setEditorOrthoType(viewType);
			m_sceneViews[handle].distortionCameras[viewportIndex].setEditorOrthoType(viewType);
		}
		else
		{
			m_sceneViews[handle].userCameras[viewportIndex]->setEditorOrthoType(viewType);
			m_sceneViews[handle].distortionCameras[viewportIndex].setEditorOrthoType(viewType);
		}
	}

	void XVPM::enableInput(XU32 handle,XU32 viewportIndex)
	{
		inputEnabled = true;
	}
	void XVPM::disableInput(XU32 handle)
	{
		inputEnabled = false;
	}

	void XVPM::selectSceneNodes(XU32 handle,XS32 x,XS32 y,XBOOL multiple)
	{
		m_pScenes[handle]->selectSceneNodes(x,y,multiple);
	}

	void XVPM::selectAll(XU32 windowHandle)
	{
		m_pScenes[windowHandle]->selectAll();
	}

	void XVPM::invertSelection(XU32 windowHandle)
	{
		m_pScenes[windowHandle]->invertSelection();
	}
	
	void XVPM::clearSelection(XU32 windowHandle)
	{
		m_pScenes[windowHandle]->clearSelection();
	}

	void XVPM::zoomSelection(XU32 handle,XU32 viewportIndex)
	{
		//First, make sure there is a selection set to zoom in on
		XMemBlockList<XSceneNode*>* selection = m_pScenes[handle]->getSelectedNodes();
		if (selection->getNumItems() != 0)
		{
			XMemBlockList<XSceneNode*>::iterator i(selection->begin());
			XSphere finalSphere = i.getValue()->getBoundingSphere();
			i.getNext();
			while (i.isValid())
			{
				//Create a sphere that surrounds all selected objects's bounding spheres
				XISphereEnclosingSpheres(finalSphere,finalSphere,i.getValue()->getBoundingSphere());
				i.getNext();
			}

			m_sceneViews[handle].PRZControllers[viewportIndex].setTarget(finalSphere.center);
			XF32 distanceToCenter = finalSphere.radius / sinf(m_sceneViews[handle].cameras[viewportIndex].getHorizontalFOV()/2.0f);
			//distanceToCenter += finalSphere.radius / cosf(m_sceneViews[handle].cameras[viewportIndex].getHorizontalFOV()/2.0f);
			m_sceneViews[handle].PRZControllers[viewportIndex].setZoom(distanceToCenter);
		}
	}

	void XVPM::zoomAll(XU32 handle,XU32 viewportIndex)
	{
		//If no render nodes, don't do anything
		/*bool finished = true;
		for (XU32 i = 0;i < m_pScenes[handle]->getNumRenderLevels();++i)
		{
			m_pScenes[handle]->getnum
		}*/

		//For each render list, pick all the nodes and build a giant bounding
		//sphere than encompasses all of them
		XMemBlockList<XSceneNode*>* renderLevel = NULL;
		XMemBlockList<XSceneNode*>::iterator itr;
		XMemBlockList<XSceneNode*>::iterator itr2;
		XMemBlockList<XSceneNode*> allNodes;
		XSphere finalSphere;
		for (XU32 i = 0;i < m_pScenes[handle]->getNumRenderLevels();++i)
		{
			renderLevel = m_pScenes[handle]->getRenderLevel(i);
			itr.set(renderLevel->begin());
			while (itr.isValid())
			{
				if (i == 0 && itr.getValue()==renderLevel->getFront())
					finalSphere = itr.getValue()->getBoundingSphere();

				XISphereEnclosingSpheres(finalSphere,finalSphere,itr.getValue()->getBoundingSphere());

				//Create a sphere that surrounds all sub objects in the node
				itr.getValue()->pickAll(allNodes);
				itr2.set(allNodes.begin());
				while (itr2.isValid())
				{
					XISphereEnclosingSpheres(finalSphere,finalSphere,itr2.getValue()->getBoundingSphere());
					itr2.getNext();	
				}
				allNodes.clear();
				itr.getNext();
			}
		}
		
		//If the bounding sphere is non-existent, we didn't have any scene nodes to zoom into
		if (finalSphere.radius == 0.0f)
			return;

		m_sceneViews[handle].PRZControllers[viewportIndex].setTarget(finalSphere.center);
		XF32 distanceToCenter = finalSphere.radius / sinf(m_sceneViews[handle].cameras[viewportIndex].getHorizontalFOV()/2.0f);
		m_sceneViews[handle].PRZControllers[viewportIndex].setZoom(distanceToCenter);
	}

	void XVPM::setSelectionColor(XU32 handle,XF32 r,XF32 g,XF32 b)
	{
		m_pScenes[handle]->getSelectionMaterialSC()->setColor(XColor(r,g,b,1.0f));
	}

	XU32 XVPM::getNumSelectedNodes(XU32 handle)
	{
		//First, make sure there is a selection set to zoom in on
		XMemBlockList<XSceneNode*>* selection = m_pScenes[handle]->getSelectedNodes();
		return selection->getNumItems();
	}

	
	void XVPM::enableWireShading(XU32 handle,XU32 viewportIndex,XBOOL enable)
	{
		m_pScenes[handle]->enableWireShading(viewportIndex,enable);
	}
		
	void XVPM::setBackgroundColor(XU32 handle,XF32 r,XF32 g,XF32 b)
	{
		//Set both the render context and render target clear colors so
		//that they match.  This helps with C# panel flickering.
		XColor color(r,g,b,1.0f);
		XVP::GraphicsMgr.setRenderContextClearColor(m_renderContexts[handle],color);
		XVP::GraphicsMgr.setRenderTargetClearColor(m_pScenes[handle]->getRenderTargetArray(),0,color);
	}

	void XVPM::setBackgroundColor(XU32 handle,XColor& color)
	{
		//Set both the render context and render target clear colors so
		//that they match.  This helps with C# panel flickering.
		XVP::GraphicsMgr.setRenderContextClearColor(m_renderContexts[handle],color);
		XVP::GraphicsMgr.setRenderTargetClearColor(m_pScenes[handle]->getRenderTargetArray(),0,color);
	}

	void XVPM::setLayout(XU32 handle,XU32 layout,
						System::Drawing::Rectangle v0,
						System::Drawing::Rectangle v1,
						System::Drawing::Rectangle v2,
						System::Drawing::Rectangle v3)
	{
		XViewportArrayDesc desc;
		
		//Can't allow a negative, zero, or too smale of a width or height
		if (v0.Width < 1) v0.Width = 1;
		if (v0.Height < 1) v0.Height = 1;
		if (v1.Width < 1) v1.Width = 1;
		if (v1.Height < 1) v1.Height = 1;
		if (v2.Width < 1) v2.Width = 1;
		if (v2.Height < 1) v2.Height = 1;
		if (v3.Width < 1) v3.Width = 1;
		if (v3.Height < 1) v3.Height = 1;


		switch (layout)
		{
		case 0:
			desc.numViewports = 1;
			desc.viewports[0].topLeftX = v0.Left;
			desc.viewports[0].topLeftY = v0.Top;
			desc.viewports[0].width = v0.Width;//XVP::GraphicsMgr.getRenderContextWidth(m_renderContexts[handle]);
			desc.viewports[0].height = v0.Height;//XVP::GraphicsMgr.getRenderContextHeight(m_renderContexts[handle]);
			desc.enablePostEffects[0] = true;
			m_pScenes[handle]->setViewportArray(desc);

			desc.enablePostEffects[0] = false;
			desc.copyToRenderContext[0] = false;
			m_pDistortionScenes[handle]->setViewportArray(desc);

			//If no user cam is applied, set the standard cam
			if (m_sceneViews[handle].userCameras[0] == NULL)
			{
				m_pScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].cameras[0]);
			}
			else
			{
				m_pScenes[handle]->setViewportCamera(0,m_sceneViews[handle].userCameras[0]);
			}
			m_pDistortionScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].distortionCameras[0]);
			break;
		case 1:
			desc.numViewports = 2;
			desc.viewports[0].topLeftX = v0.Left;
			desc.viewports[0].topLeftY = v0.Top;
			desc.viewports[0].width = v0.Width;//XVP::GraphicsMgr.getRenderContextWidth(m_renderContexts[handle])/2;
			desc.viewports[0].height = v0.Height;//XVP::GraphicsMgr.getRenderContextHeight(m_renderContexts[handle]);
			desc.enablePostEffects[0] = true;
			desc.viewports[1].topLeftX = v1.Left;//XVP::GraphicsMgr.getRenderContextWidth(m_renderContexts[handle])/2;
			desc.viewports[1].topLeftY = v1.Top;//0;
			desc.viewports[1].width = v1.Width;//XVP::GraphicsMgr.getRenderContextWidth(m_renderContexts[handle])/2;
			desc.viewports[1].height = v1.Height;//XVP::GraphicsMgr.getRenderContextHeight(m_renderContexts[handle]);
			desc.enablePostEffects[1] = true;
			m_pScenes[handle]->setViewportArray(desc);


			desc.enablePostEffects[0] = false;
			desc.copyToRenderContext[0] = false;
			desc.enablePostEffects[1] = false;
			desc.copyToRenderContext[1] = false;
			m_pDistortionScenes[handle]->setViewportArray(desc);

			if (m_sceneViews[handle].userCameras[0] == NULL)	
			{
				m_pScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].cameras[0]);
			}
			else
				m_pScenes[handle]->setViewportCamera(0,m_sceneViews[handle].userCameras[0]);
			m_pDistortionScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].distortionCameras[0]);
			if (m_sceneViews[handle].userCameras[1] == NULL)	
			{
				m_pScenes[handle]->setViewportCamera(1,&m_sceneViews[handle].cameras[1]);
			}
			else
				m_pScenes[handle]->setViewportCamera(1,m_sceneViews[handle].userCameras[1]);
			m_pDistortionScenes[handle]->setViewportCamera(1,&m_sceneViews[handle].distortionCameras[1]);
			//m_sceneViews[handle].cameras[0].addPostEffect(&m_sceneViews[handle].hdrPostEffects[0]);
			//m_sceneViews[handle].cameras[1].addPostEffect(&m_sceneViews[handle].hdrPostEffects[1]);
			break;		
		case 2:
			desc.numViewports = 2;
			desc.viewports[0].topLeftX = v0.Left;
			desc.viewports[0].topLeftY = v0.Top;
			desc.viewports[0].width = v0.Width;//XVP::GraphicsMgr.getRenderContextWidth(m_renderContexts[handle]);
			desc.viewports[0].height = v0.Height;//XVP::GraphicsMgr.getRenderContextHeight(m_renderContexts[handle])/2;
			desc.enablePostEffects[0] = true;
			desc.viewports[1].topLeftX = v1.Left;
			desc.viewports[1].topLeftY = v1.Top;//XVP::GraphicsMgr.getRenderContextHeight(m_renderContexts[handle])/2;
			desc.viewports[1].width = v1.Width;//XVP::GraphicsMgr.getRenderContextWidth(m_renderContexts[handle]);
			desc.viewports[1].height = v1.Height;//;XVP::GraphicsMgr.getRenderContextHeight(m_renderContexts[handle])/2;
			desc.enablePostEffects[1] = true;
			m_pScenes[handle]->setViewportArray(desc);

			desc.enablePostEffects[0] = false;
			desc.copyToRenderContext[0] = false;
			desc.enablePostEffects[1] = false;
			desc.copyToRenderContext[1] = false;
			m_pDistortionScenes[handle]->setViewportArray(desc);

			if (m_sceneViews[handle].userCameras[0] == NULL)
			{
				m_pScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].cameras[0]);
			}
			else
				m_pScenes[handle]->setViewportCamera(0,m_sceneViews[handle].userCameras[0]);
			m_pDistortionScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].distortionCameras[0]);
			if (m_sceneViews[handle].userCameras[1] == NULL)	
			{
				m_pScenes[handle]->setViewportCamera(1,&m_sceneViews[handle].cameras[1]);
			}
			else
				m_pScenes[handle]->setViewportCamera(1,m_sceneViews[handle].userCameras[1]);
			m_pDistortionScenes[handle]->setViewportCamera(1,&m_sceneViews[handle].distortionCameras[1]);

			break;
		case 3:
			desc.numViewports = 4;
			desc.viewports[0].topLeftX = v0.Left;
			desc.viewports[0].topLeftY = v0.Top;
			desc.viewports[0].width = v0.Width;
			desc.viewports[0].height = v0.Height;
			desc.enablePostEffects[0] = true;
			desc.viewports[1].topLeftX = v1.Left;
			desc.viewports[1].topLeftY = v1.Top;
			desc.viewports[1].width = v1.Width;
			desc.viewports[1].height = v1.Height;
			desc.enablePostEffects[1] = true;
			desc.viewports[2].topLeftX = v2.Left;
			desc.viewports[2].topLeftY = v2.Top;
			desc.viewports[2].width = v2.Width;
			desc.viewports[2].height = v2.Height;
			desc.enablePostEffects[2] = true;
			desc.viewports[3].topLeftX = v3.Left;
			desc.viewports[3].topLeftY = v3.Top;
			desc.viewports[3].width = v3.Width;
			desc.viewports[3].height = v3.Height;
			desc.enablePostEffects[3] = true;
			m_pScenes[handle]->setViewportArray(desc);

			desc.enablePostEffects[0] = false;
			desc.copyToRenderContext[0] = false;
			desc.enablePostEffects[1] = false;
			desc.copyToRenderContext[1] = false;
			desc.enablePostEffects[2] = false;
			desc.copyToRenderContext[2] = false;
			desc.enablePostEffects[3] = false;
			desc.copyToRenderContext[3] = false;
			m_pDistortionScenes[handle]->setViewportArray(desc);

			if (m_sceneViews[handle].userCameras[0] == NULL)	
			{
				m_pScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].cameras[0]);
				
			}
			else
				m_pScenes[handle]->setViewportCamera(0,m_sceneViews[handle].userCameras[0]);
			m_pDistortionScenes[handle]->setViewportCamera(0,&m_sceneViews[handle].distortionCameras[0]);
			if (m_sceneViews[handle].userCameras[1] == NULL)	
			{
				m_pScenes[handle]->setViewportCamera(1,&m_sceneViews[handle].cameras[1]);
				
			}
			else
				m_pScenes[handle]->setViewportCamera(1,m_sceneViews[handle].userCameras[1]);
			m_pDistortionScenes[handle]->setViewportCamera(1,&m_sceneViews[handle].distortionCameras[1]);
			if (m_sceneViews[handle].userCameras[2] == NULL)
			{
				m_pScenes[handle]->setViewportCamera(2,&m_sceneViews[handle].cameras[2]);
				
			}
			else
				m_pScenes[handle]->setViewportCamera(2,m_sceneViews[handle].userCameras[2]);
			m_pDistortionScenes[handle]->setViewportCamera(2,&m_sceneViews[handle].distortionCameras[2]);
			if (m_sceneViews[handle].userCameras[3] == NULL)
			{
				m_pScenes[handle]->setViewportCamera(3,&m_sceneViews[handle].cameras[3]);
				
			}
			else
				m_pScenes[handle]->setViewportCamera(3,m_sceneViews[handle].userCameras[3]);
			m_pDistortionScenes[handle]->setViewportCamera(3,&m_sceneViews[handle].distortionCameras[3]);
			break;
		}
	}

	void XVPM::resetViewports(XU32 windowHandle)
	{
		setBackgroundColor(windowHandle,VIEWPORT_COLOR);

		//m_sceneViews[windowHandle].gridSC.setColor(XColor(.764f,.764f,.764f,1.0f));
		m_sceneViews[windowHandle].grids[0].init(1.0f,20,10);
		m_sceneViews[windowHandle].grids[0].attachShaderController(NULL,&m_sceneViews[windowHandle].gridSC);
		m_sceneViews[windowHandle].grids[0].setCameraID(0);
		m_sceneViews[windowHandle].grids[1].init(1.0f,20,10);
		m_sceneViews[windowHandle].grids[1].attachShaderController(NULL,&m_sceneViews[windowHandle].gridSC);
		m_sceneViews[windowHandle].grids[1].setCameraID(1);
		m_sceneViews[windowHandle].grids[2].init(1.0f,20,10);
		m_sceneViews[windowHandle].grids[2].attachShaderController(NULL,&m_sceneViews[windowHandle].gridSC);
		m_sceneViews[windowHandle].grids[2].setCameraID(2);
		m_sceneViews[windowHandle].grids[3].init(1.0f,20,10);
		m_sceneViews[windowHandle].grids[3].attachShaderController(NULL,&m_sceneViews[windowHandle].gridSC);
		m_sceneViews[windowHandle].grids[3].setCameraID(3);

		for (XU32 i = 0;i < 4;++i)
		{
			m_sceneViews[windowHandle].cameras[i].detachAllControllers();
			m_sceneViews[windowHandle].cameras[i].attachController(&m_sceneViews[windowHandle].PRZControllers[i]);	
		}

		setViewType(windowHandle,0,VT_PERSPECTIVE,false);
		m_sceneViews[windowHandle].PRZControllers[0].setZoom(INITIAL_ZOOM);
		showGrid(windowHandle,0,true);
		setViewType(windowHandle,1,VT_TOP,false);
		m_sceneViews[windowHandle].PRZControllers[1].setZoom(INITIAL_ZOOM);
		showGrid(windowHandle,1,true);
		setViewType(windowHandle,2,VT_LEFT,false);
		m_sceneViews[windowHandle].PRZControllers[2].setZoom(INITIAL_ZOOM);
		showGrid(windowHandle,2,true);
		setViewType(windowHandle,3,VT_FRONT,false);
		m_sceneViews[windowHandle].PRZControllers[3].setZoom(INITIAL_ZOOM);
		showGrid(windowHandle,3,true);

		XViewportArrayDesc desc;
		m_pScenes[windowHandle]->getViewportArray(desc);
		switch (desc.numViewports)
		{
		case 1:
			m_pScenes[windowHandle]->setViewportCamera(0,&m_sceneViews[windowHandle].cameras[0]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(0,&m_sceneViews[windowHandle].distortionCameras[0]);
			m_sceneViews[windowHandle].userCameras[0] = NULL; 
			break;
		case 2:
			m_pScenes[windowHandle]->setViewportCamera(0,&m_sceneViews[windowHandle].cameras[0]);
			m_pScenes[windowHandle]->setViewportCamera(1,&m_sceneViews[windowHandle].cameras[1]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(0,&m_sceneViews[windowHandle].distortionCameras[0]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(1,&m_sceneViews[windowHandle].distortionCameras[1]);
			m_sceneViews[windowHandle].userCameras[0] = NULL; 
			m_sceneViews[windowHandle].userCameras[1] = NULL; 
			break;
		case 4:
			m_pScenes[windowHandle]->setViewportCamera(0,&m_sceneViews[windowHandle].cameras[0]);
			m_pScenes[windowHandle]->setViewportCamera(1,&m_sceneViews[windowHandle].cameras[1]);
			m_pScenes[windowHandle]->setViewportCamera(2,&m_sceneViews[windowHandle].cameras[2]);
			m_pScenes[windowHandle]->setViewportCamera(3,&m_sceneViews[windowHandle].cameras[3]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(0,&m_sceneViews[windowHandle].distortionCameras[0]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(1,&m_sceneViews[windowHandle].distortionCameras[1]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(2,&m_sceneViews[windowHandle].distortionCameras[2]);
			m_pDistortionScenes[windowHandle]->setViewportCamera(3,&m_sceneViews[windowHandle].distortionCameras[3]);
			m_sceneViews[windowHandle].userCameras[0] = NULL; 
			m_sceneViews[windowHandle].userCameras[1] = NULL; 
			m_sceneViews[windowHandle].userCameras[2] = NULL; 
			m_sceneViews[windowHandle].userCameras[3] = NULL; 
			break;
		}
	}

	void XVPM::resetViewportCamera(XU32 windowHandle,XU32 viewportIndex)
	{
		setViewType(windowHandle,viewportIndex,VT_PERSPECTIVE,false);
		m_sceneViews[windowHandle].PRZControllers[viewportIndex].setZoom(40);
	}

	void XVPM::setGizmoMode(XU32 windowHandle,XU32 mode)
	{
		m_pScenes[windowHandle]->setGizmoMode((X_GIZMO_MODE)mode);
		if (mode == X_GM_MOVE ||
			mode == X_GM_ROTATE ||
			mode == X_GM_SCALE)
			m_pScenes[windowHandle]->setSelectionFilter(X_SF_ACTOR);
		else
			m_pScenes[windowHandle]->setSelectionFilter(X_SF_NONE);
	}

	void XVPM::updateGizmoHover(XU32 windowHandle,XS32 x,XS32 y)
	{
		m_pScenes[windowHandle]->updateGizmoHover(x,y);
	}

	void XVPM::updateGizmoDrag(XU32 windowHandle,XF32 startX,XF32 startY,XF32 endX,XF32 endY)
	{
		m_pScenes[windowHandle]->updateGizmoDrag(startX,startY,endX,endY);
	}

	void XVPM::gizmoOnMouseUp(XU32 windowHandle)
	{
		m_pScenes[windowHandle]->gizmoOnMouseUp();
	}

	void XVPM::getPlacementTransform(XU32 windowHandle,IntPtr transform,XU32 viewportIndex,int screenX,int screenY)
	{
		//Translate screen coords into viewport coords first
		XViewportArrayDesc desc;
		m_pScenes[windowHandle]->getViewportArray(desc);
		XS32 vpX = screenX - desc.viewports[viewportIndex].topLeftX;
		XS32 vpY = screenY - desc.viewports[viewportIndex].topLeftY;

		XMatrix4x4* pMatrix = (XMatrix4x4*)transform.ToPointer();
		XMatrix4x4 newTransform;
		XVector3D lineStart,lineEnd;
		XCamera* pCam = m_pScenes[windowHandle]->getViewportCamera(viewportIndex);
		XICreatePickLine(vpX,
						 vpY,
						 pCam->getViewportWidth(),
						 pCam->getViewportHeight(),
						 pCam->getViewMatrix(),
						 pCam->getProjMatrix(),
						 lineStart,
						 lineEnd);

		XVector3D pickLineDir = (lineEnd-lineStart).normalize();
		XVector3D planeNormal;	

		XVector3D intersectionPoint;

		//If the pick line is getting close to parallel with the intersection plane
		planeNormal = XVector3D(0.0f,1.0f,0.0f);
		//if (fabsf(pickLineDir.dot(planeNormal)) < .1f)
		//	planeNormal = XVector3D(nodeMat.row3);

		XF32 time;
		if (XILinePlane(lineStart,lineEnd,XVector3D(0,0,0),planeNormal,intersectionPoint,time))
		{
			newTransform._41 = intersectionPoint.x;
			newTransform._42 = intersectionPoint.y;
			newTransform._43 = intersectionPoint.z;
		}

		(*pMatrix) = newTransform;
	}

	XBOOL XVPM::isMoveSnapEnabled(XU32 windowHandle)
	{
		return m_pScenes[windowHandle]->getGizmoMgr()->isMoveSnapEnabled();
	}

	void XVPM::enableMoveSnap(XU32 windowHandle,XBOOL enable)
	{
		m_pScenes[windowHandle]->getGizmoMgr()->enableMoveSnap(enable);
	}

	XF32 XVPM::getMoveSnapAmount(XU32 windowHandle)
	{
		return m_pScenes[windowHandle]->getGizmoMgr()->getMoveSnapAmount();
	}
		
	void XVPM::setMoveSnapAmount(XU32 windowHandle,XF32 amount)
	{
		m_pScenes[windowHandle]->getGizmoMgr()->setMoveSnapAmount(amount);
	}
		
	XBOOL XVPM::isRotateSnapEnabled(XU32 windowHandle)
	{
		return m_pScenes[windowHandle]->getGizmoMgr()->isRotateSnapEnabled();
	}

	void XVPM::enableRotateSnap(XU32 windowHandle,XBOOL enable)
	{
		m_pScenes[windowHandle]->getGizmoMgr()->enableRotateSnap(enable);
	}

	XF32 XVPM::getRotateSnapAmount(XU32 windowHandle)
	{
		return X_MATH_RAD_TO_DEG(m_pScenes[windowHandle]->getGizmoMgr()->getRotateSnapAmount());
	}
		
	void XVPM::setRotateSnapAmount(XU32 windowHandle,XF32 amount)
	{
		m_pScenes[windowHandle]->getGizmoMgr()->setRotateSnapAmount(X_MATH_DEG_TO_RAD(amount));
	}

	XBOOL XVPM::isScaleSnapEnabled(XU32 windowHandle)
	{
		return m_pScenes[windowHandle]->getGizmoMgr()->isScaleSnapEnabled();
	}

	void XVPM::enableScaleSnap(XU32 windowHandle,XBOOL enable)
	{
		m_pScenes[windowHandle]->getGizmoMgr()->enableScaleSnap(enable);
	}

	XF32 XVPM::getScaleSnapAmount(XU32 windowHandle)
	{
		return m_pScenes[windowHandle]->getGizmoMgr()->getScaleSnapAmount();
	}
		
	void XVPM::setScaleSnapAmount(XU32 windowHandle,XF32 amount)
	{
		m_pScenes[windowHandle]->getGizmoMgr()->setScaleSnapAmount(amount);
	}

	void XVPM::loadShaderWad(IntPtr pFilename)
	{
		XVP::GraphicsMgr.loadShaderWad((XS8*)pFilename.ToPointer());
	}

	XBOOL XVPM::isShaderRegistered(IntPtr pShaderName)
	{
		return XVP::ShaderFactory.isObjectRegistered((XS8*)pShaderName.ToPointer());
	}

	XBOOL XVPM::shaderExists(IntPtr pShaderName)
	{
		if (XVP::GraphicsMgr.getVertexShaderByName((XS8*)pShaderName.ToPointer()))
			return true;
		else
			return false;
	}

	XU32 XVPM::getLightingMode(XU32 windowHandle)
	{
		return m_pScenes[windowHandle]->getLightMgr()->getLightingMode();
	}

	void XVPM::setLightingMode(XU32 windowHandle,XU32 mode)
	{
		//Remove scenario lights from scene 
		if (mode != 1)
		{
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->removeUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}

		//Depending on the mode, add scenario lights
		/*if (mode == 1)
		{
			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}*/

		m_sceneViews[windowHandle].currentLightingMode = (XLightingMode)mode;
		m_pScenes[windowHandle]->getLightMgr()->setLightingMode((XLightingMode)mode);
	}

	void XVPM::setLightScenario(XU32 windowHandle,IntPtr pScenarioName)
	{
		//Remove scenario lights from scene
		for (XU32 i = 0;i < 3;++i)
			m_pScenes[windowHandle]->removeUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);

		//Destroy and re-init lights as appropriate types
		//for (XU32 i = 0;i < 3;++i)
		//	m_sceneViews[windowHandle].scenarioLights[i].destroy();
		
		//XLightDesc ld;
		//ld.priority = SYSTEM_LIGHT_PRIORITY;
		XMatrix4x4 rotMatrix;
		XMatrix4x4 transMatrix;
		XMatrix4x4 angleMatrix;
		angleMatrix.createXRotation(m_sceneViews[windowHandle].lightAngle);
		
		if (strcmp("btnDirSW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			//ld.type = X_LT_DIRECTIONAL;
			transMatrix.createTranslation(XVector3D(0.0f,300.0f,-300.0f));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix * transMatrix);
			//m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[0]);

		}
		else if (strcmp("btnDirDW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			//ld.type = X_LT_DIRECTIONAL;
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
			rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
			//m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);
			//m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 2;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnDirTW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			//ld.type = X_LT_DIRECTIONAL;
			//rotMatrix.createYRotation(-1.57f);
			XMatrix4x4 angleMatrix;
			angleMatrix.createXRotation(m_sceneViews[windowHandle].lightAngle);
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
			rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
			rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(240));
			m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(angleMatrix * rotMatrix);
			//m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);
			//m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor(1,1,1,1);
			//m_sceneViews[windowHandle].scenarioLights[2].init(ld);
			m_sceneViews[windowHandle].scenarioLights[2].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnDirSRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			//ld.type = X_LT_DIRECTIONAL;
			//rotMatrix.createYRotation(-1.57f);
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
			//m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;

			//Add the lights back to the scene 
			m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[0]);
		}
		else if (strcmp("btnDirDRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			//ld.type = X_LT_DIRECTIONAL;
			//rotMatrix.createYRotation(-1.57f);
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
			rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
			//m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;
			//m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor::Green;

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 2;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnDirTRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			//ld.type = X_LT_DIRECTIONAL;
			//rotMatrix.createYRotation(-1.57f);
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
			rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
			rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(240));
			m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(angleMatrix * rotMatrix);
			//m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;
			//m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor::Green;
			//m_sceneViews[windowHandle].scenarioLights[2].init(ld);
			m_sceneViews[windowHandle].scenarioLights[2].m_diffuse = XColor(0,0,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		/*else if (strcmp("btnPointSW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_POINT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[0]);
		}
		else if (strcmp("btnPointDW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_POINT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(transMatrix);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 2;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnPointTW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_POINT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(transMatrix);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(transMatrix);
			transMatrix.createTranslation(XVector3D(0,25,0));
			m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor(1,1,1,1);
			m_sceneViews[windowHandle].scenarioLights[2].init(ld);
			m_sceneViews[windowHandle].scenarioLights[2].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnPointSRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_POINT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;

			//Add the lights back to the scene 
			m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[0]);
		}
		else if (strcmp("btnPointDRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_POINT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(transMatrix);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor::Green;

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 2;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnPointTRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_POINT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(transMatrix);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(transMatrix);
			transMatrix.createTranslation(XVector3D(0,25,0));
			m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor::Green;
			m_sceneViews[windowHandle].scenarioLights[2].init(ld);
			m_sceneViews[windowHandle].scenarioLights[2].m_diffuse = XColor(0,0,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnSpotSW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_SPOT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			ld.cosHalfAngleDropoff = 1.57f;
			rotMatrix.createYRotation(-1.57f);
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(rotMatrix * transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[0]);
		}
		else if (strcmp("btnSpotDW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_SPOT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			ld.cosHalfAngleDropoff = 1.57f;
			rotMatrix.createYRotation(-1.57f);
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(rotMatrix * transMatrix);
			rotMatrix.createYRotation(3.14f);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(rotMatrix * transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 2;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnSpotTW",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_SPOT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			ld.cosHalfAngleDropoff = 1.57f;
			rotMatrix.createYRotation(-1.57f);
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(rotMatrix * transMatrix);
			rotMatrix.createYRotation(3.14f);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(rotMatrix * transMatrix);
			rotMatrix.createXRotation(1.57f);
			transMatrix.createTranslation(XVector3D(0,25,0));
			m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(rotMatrix * transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor(1,1,1,1);
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor(1,1,1,1);
			m_sceneViews[windowHandle].scenarioLights[2].init(ld);
			m_sceneViews[windowHandle].scenarioLights[2].m_diffuse = XColor(1,1,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnSpotSRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_SPOT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			ld.cosHalfAngleDropoff = 1.57f;
			rotMatrix.createYRotation(-1.57f);
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(rotMatrix * transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;

			//Add the lights back to the scene 
			m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[0]);
		}
		else if (strcmp("btnSpotDRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_SPOT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			ld.cosHalfAngleDropoff = 1.57f;
			rotMatrix.createYRotation(-1.57f);
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(rotMatrix * transMatrix);
			rotMatrix.createYRotation(3.14f);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(rotMatrix * transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor::Green;

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 2;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}
		else if (strcmp("btnSpotTRGB",(XS8*)pScenarioName.ToPointer()) == 0)
		{
			ld.type = X_LT_SPOT;
			ld.cutoffDistance = 100;
			ld.hotSpotDistance = 50;
			ld.cosHalfAngleDropoff = 1.57f;
			rotMatrix.createYRotation(-1.57f);
			transMatrix.createTranslation(XVector3D(25,0,0));
			m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(rotMatrix * transMatrix);
			rotMatrix.createYRotation(3.14f);
			transMatrix.createTranslation(XVector3D(0,0,25));
			m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(rotMatrix * transMatrix);
			rotMatrix.createXRotation(1.57f);
			transMatrix.createTranslation(XVector3D(0,25,0));
			m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(rotMatrix * transMatrix);
			m_sceneViews[windowHandle].scenarioLights[0].init(ld);
			m_sceneViews[windowHandle].scenarioLights[0].m_diffuse = XColor::Red;
			m_sceneViews[windowHandle].scenarioLights[1].init(ld);
			m_sceneViews[windowHandle].scenarioLights[1].m_diffuse = XColor::Green;
			m_sceneViews[windowHandle].scenarioLights[2].init(ld);
			m_sceneViews[windowHandle].scenarioLights[2].m_diffuse = XColor(0,0,1,1);

			//Add the lights back to the scene 
			for (XU32 i = 0;i < 3;++i)
				m_pScenes[windowHandle]->addUpdateNode(&m_sceneViews[windowHandle].scenarioLights[i]);
		}*/
	}

	void XVPM::setScenarioLightColor(XU32 windowHandle,XU32 lightIndex,XF32 r,XF32 g,XF32 b)
	{
		m_sceneViews[windowHandle].scenarioLights[lightIndex].m_diffuse = XColor(r,g,b,m_sceneViews[windowHandle].scenarioLights[lightIndex].m_diffuse.m_A);
	}

	XF32 XVPM::getScenarioLightIntensity(XU32 windowHandle,XU32 lightIndex)
	{
		return m_sceneViews[windowHandle].scenarioLights[lightIndex].m_diffuse.m_A;
	}

	void XVPM::setScenarioLightIntensity(XU32 windowHandle,XU32 lightIndex,XF32 intensity)
	{
		m_sceneViews[windowHandle].scenarioLights[lightIndex].m_diffuse.m_A = intensity;
		m_sceneViews[windowHandle].scenarioLights[lightIndex].m_specular.m_A = intensity;
	}

	void XVPM::setScenarioLightAnimSpeed(XU32 windowHandle,XF32 speed)
	{
		m_sceneViews[windowHandle].lightAnimSpeed = speed;
	}

	void XVPM::setScenarioLightAngle(XU32 windowHandle,XS32 angle)
	{
		m_sceneViews[windowHandle].lightAngle = X_MATH_DEG_TO_RAD(angle);

		//Update light angle
		XMatrix4x4 rotMatrix;
		XMatrix4x4 angleMatrix;
		angleMatrix.createXRotation(m_sceneViews[windowHandle].lightAngle);
		rotMatrix.createYRotation(m_sceneViews[windowHandle].lightAnimAngle);
		m_sceneViews[windowHandle].scenarioLights[0].setModelTransform(angleMatrix * rotMatrix);
		rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(120) + m_sceneViews[windowHandle].lightAnimAngle);
		m_sceneViews[windowHandle].scenarioLights[1].setModelTransform(angleMatrix * rotMatrix);
		rotMatrix.createYRotation(X_MATH_DEG_TO_RAD(240) + m_sceneViews[windowHandle].lightAnimAngle);
		m_sceneViews[windowHandle].scenarioLights[2].setModelTransform(angleMatrix * rotMatrix);
	}

	void XVPM::enableShadows(XU32 windowHandle,XBOOL enable)
	{
		m_sceneViews[windowHandle].scenarioLights[0].enableOccluders(enable);
		m_sceneViews[windowHandle].scenarioLights[1].enableOccluders(enable);
		m_sceneViews[windowHandle].scenarioLights[2].enableOccluders(enable);
	}

	void XVPM::setCameraViewOrientation(XPRZController& przController,ViewType viewType,bool preservePanAndZoom)
	{
		XF32 oldZoom = przController.getZoom();
		XVector3D oldTarget = przController.getTarget();
		przController.reset();
		przController.setTarget(XVector3D(0,0,0));

		switch (viewType)
		{
		case VT_PERSPECTIVE:
			przController.addRotation(-.785f,.5f);
			//przController.addZoom(20);
			break;
		case VT_LEFT:
			przController.addRotation(-1.5707f,0.0f);
			//przController.addZoom(20);
			break;
		case VT_TOP:
			przController.addRotation(0.0f,1.5707f);
			//przController.addZoom(20);
			break;
		case VT_RIGHT:
			przController.addRotation(1.5707f,0.0f);
			//przController.addZoom(20);
			break;
		case VT_BOTTOM:
			przController.addRotation(0.0f,-1.5707f);
			//przController.addZoom(20);
			break;
		case VT_FRONT:
			przController.addRotation(X_MATH_PI,0.0f);
			//przController.addZoom(20);
			break;
		case VT_BACK:
			//przController.addZoom(20);
			break;
		}

		if (preservePanAndZoom)
		{
			przController.setZoom(oldZoom);
			przController.setTarget(oldTarget);
		}
	}

	void XVPM::setActorContainer(IntPtr sceneNode,IntPtr actorContainer)
	{
		ProxyObject* pProxy = (ProxyObject*)sceneNode.ToPointer();
		ActorContainer* pActorContainer = (ActorContainer*)actorContainer.ToPointer();
		pProxy->setActorContainer(pActorContainer->getActorContainer());
	}
		
	
/*
	XU32 XVPM::createShaderController(XS8* shaderName)
	{
		for (XU32 i = 0;i < MAX_SHADER_CONTROLLERS;++i)
		{
			if (m_pShaderControllers[i] == NULL)
			{
				m_pShaderControllers[i] = XVP::ShaderFactory.createObject(shaderName);
				return i;
			}
		}

		X_ASSERT_FORCED("No available shader controller slots");
	}

	void XVPM::destroyShaderController(XU32 shaderController)
	{
		if (m_pShaderControllers[shaderController] != NULL)
		{
			XDelete(m_pShaderControllers[shaderController]);
			m_pShaderControllers[shaderController] = NULL;
		}
	}*/


	void XVPM::setFogColor(XU32 windowHandle,XF32 r,XF32 g,XF32 b)
	{
		m_pScenes[windowHandle]->setFogColor(XColor(r,g,b,1.0f));
	}

	void XVPM::setFogStart(XU32 windowHandle,XF32 fogStart)
	{
		m_pScenes[windowHandle]->setFogStart(fogStart);
	}

	void XVPM::setFogEnd(XU32 windowHandle,XF32 fogEnd)
	{
		m_pScenes[windowHandle]->setFogEnd(fogEnd);
	}

	void XVPM::setFogDensity(XU32 windowHandle,XF32 fogDensity)
	{
		m_pScenes[windowHandle]->setFogDensity(fogDensity);
	}

	void XVPM::setGlobalAmbient(XU32 windowHandle,XF32 r,XF32 g,XF32 b)
	{
		m_pScenes[windowHandle]->setGlobalAmbient(XColor(r,g,b,1.0f));
	}

	IntPtr XVPM::initParticleSystem(XU32 windowHandle,XU32 numEmitters)
	{
		//Create a new proxy object for the particle system
		ParticleSystem* pPSProxy = NULL;
		XNew(pPSProxy,ParticleSystem(),XOSMgr::getDefaultHeap());
		
		pPSProxy->init(m_pScenes[windowHandle],numEmitters);

		//m_proxyObjects.insert((XS8*)pName.ToPointer(),pPS);

		return IntPtr(pPSProxy);
	}

	void XVPM::destroyParticleSystem(IntPtr particleSystem)
	{
		//Get the particle system and destroy it
		//ParticleSystem* pPS = (ParticleSystem*)m_proxyObjects.getItem(pName);
		//m_proxyObjects.remove(pName);
		
		X_ASSERTM(particleSystem != IntPtr::Zero,"Destroying a invalid object");
		ParticleSystem* pPS = (ParticleSystem*)particleSystem.ToPointer();
		pPS->destroy();
		XDelete(pPS);
	}

	IntPtr XVPM::initEmitter(IntPtr particleSystem,XU32 emitterIndex)
	{
		//Find the emitter with the name given.  Then create and return a proxy object
		//for the emitter.
		ParticleSystem* pPSProxy = (ParticleSystem*)particleSystem.ToPointer();
		XParticleEmitter* pEmitter = pPSProxy->getEmitter(emitterIndex);
		//X_ASSERTM(pEmitter,"Could not find emitter by name");

		//Now that we've found the emitter, let's create an emitter proxy and return it
		Emitter* pEmitterProxy = NULL;
		XNew(pEmitterProxy,Emitter(),XOSMgr::getDefaultHeap());
		pEmitterProxy->init(pEmitter);

		return IntPtr(pEmitterProxy);
	}

	IntPtr XVPM::getEmitterMaterial(IntPtr emitter)
	{
		Emitter* pEmitterProxy = (Emitter*)emitter.ToPointer();
		XMaterial* pMaterial = pEmitterProxy->createMaterial();
		X_ASSERTM(pMaterial,"Could not find emitter material");

		//Now that we've found the material, let's create an material proxy and return it
		MaterialNP* pProxy = NULL;
		XNew(pProxy,MaterialNP(),XOSMgr::getDefaultHeap());
		pProxy->init(pMaterial);

		return IntPtr(pProxy);
	}

	
	void XVPM::startParticleSystem(IntPtr particleSystem)
	{
		ParticleSystem* pPSProxy = (ParticleSystem*)particleSystem.ToPointer();
		pPSProxy->start();
	}

	void XVPM::stopParticleSystem(IntPtr particleSystem)
	{
		ParticleSystem* pPSProxy = (ParticleSystem*)particleSystem.ToPointer();
		pPSProxy->stop();
	}

	void XVPM::restartParticleSystem(IntPtr particleSystem)
	{
		ParticleSystem* pPSProxy = (ParticleSystem*)particleSystem.ToPointer();
		pPSProxy->restart();
	}

	void XVPM::addParticleSystemEmitter(IntPtr particleSystem)
	{
		ParticleSystem* pPSProxy = (ParticleSystem*)particleSystem.ToPointer();
		pPSProxy->addEmitter();
	}
	
	IntPtr XVPM::initModel(XU32 windowHandle,IntPtr pFilename,XU32 numLODs)
	{
		//Create a new proxy object for the model
		Model* pProxy = NULL;
		XNew(pProxy,Model(),XOSMgr::getDefaultHeap());
		
		pProxy->init(m_pScenes[windowHandle],(XS8*)pFilename.ToPointer(),numLODs);
		
		m_sceneViews[windowHandle].scenarioLights[0].addOccluder(pProxy->getModel());
		m_sceneViews[windowHandle].scenarioLights[1].addOccluder(pProxy->getModel());
		m_sceneViews[windowHandle].scenarioLights[2].addOccluder(pProxy->getModel());

		return IntPtr(pProxy);
	}

	void XVPM::destroyModel(IntPtr model,XU32 windowHandle)
	{
		X_ASSERTM(model != IntPtr::Zero,"Destroying a invalid object");
		Model* pModelProxy = (Model*)model.ToPointer();
		m_sceneViews[windowHandle].scenarioLights[0].removeOccluder(pModelProxy->getModel());
		m_sceneViews[windowHandle].scenarioLights[1].removeOccluder(pModelProxy->getModel());
		m_sceneViews[windowHandle].scenarioLights[2].removeOccluder(pModelProxy->getModel());

		pModelProxy->destroy();
		XDelete(pModelProxy);
	}
	
	void XVPM::postInitModel(IntPtr model)
	{
		Model* pModelProxy = (Model*)model.ToPointer();
		pModelProxy->postInit();

		
	}
	
	void XVPM::mergeModelLOD(IntPtr model,IntPtr pFilename,XF32 LODDistance)
	{
		Model* pModelProxy = (Model*)model.ToPointer();
		pModelProxy->mergeLOD((XS8*)pFilename.ToPointer(),LODDistance);
	}

	void XVPM::setModelLODDist(IntPtr model,XU32 LODIndex,XF32 LODDist)
	{
		Model* pModelProxy = (Model*)model.ToPointer();
		pModelProxy->setLODDist(LODIndex,LODDist);
	}

	IntPtr XVPM::getModelMaterial(IntPtr model,IntPtr pMaterialName)
	{
		Model* pModelProxy = (Model*)model.ToPointer();
		XMaterial* pMaterial = pModelProxy->getMaterial((XS8*)pMaterialName.ToPointer());

		//Now that we've found the material, let's create a material proxy and return it
		MaterialNP* pProxy = NULL;
		XNew(pProxy,MaterialNP(),XOSMgr::getDefaultHeap());
		pProxy->init(pMaterial);

		return IntPtr(pProxy);
	}

	void XVPM::destroyModelMaterial(IntPtr material)
	{
		X_ASSERTM(material != IntPtr::Zero,"Destroying a invalid object");
		MaterialNP* pProxy = (MaterialNP*)material.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}

	void XVPM::attachControllerToModel(IntPtr model,IntPtr controller,IntPtr pNodeName)
	{
		Model* pModelProxy = (Model*)model.ToPointer();
		BaseController* pControllerProxy = (BaseController*)controller.ToPointer();

		pModelProxy->attachController(pControllerProxy->getController(),(XS8*)pNodeName.ToPointer());

	}

	void XVPM::detachControllerFromModel(IntPtr model,IntPtr controller,IntPtr pNodeName)
	{
		Model* pModelProxy = (Model*)model.ToPointer();
		BaseController* pControllerProxy = (BaseController*)controller.ToPointer();

		pModelProxy->detachController(pControllerProxy->getController(),(XS8*)pNodeName.ToPointer());

	}

	bool XVPM::setScene(XU32 windowHandle,IntPtr proxy,IntPtr pSceneName)
	{
		//Look up the scene pointer, if we don't find it, then we will just add this
		//scene node to the main scene
		ProxyObject* pProxy = (ProxyObject*)proxy.ToPointer();
		XScene** ppScene = m_sceneMap[windowHandle].getItem((XS8*)pSceneName.ToPointer());
		if (ppScene)
		{
			pProxy->setScene(*ppScene);
			return true;
		}
		else
		{
			pProxy->setScene(m_pScenes[windowHandle]);
			return false;
		}
	}

	void XVPM::generateLightmap(array<IntPtr> ^models,array<IntPtr>^ lights,array<System::String^>^ filenames,XU32 maxSize,XU32 pixelsPerSurfAreaUnit,XU32 forcedSize,XF32 blurSize)
	{
		XModel** ppModels = NULL;
		XLight** ppLights = NULL;
		XNewArray(ppModels,XModel*,models->Length,XOSMgr::getModelHeap());
		XNewArray(ppLights,XLight*,lights->Length,XOSMgr::getModelHeap());
		for (XS32 i = 0;i < models->Length;++i)
			ppModels[i] = ((Model*)models[i].ToPointer())->getModel();
		for (XS32 i = 0;i < lights->Length;++i)
			ppLights[i] = ((Light*)lights[i].ToPointer())->getLight();

		for (XS32 i = 0;i < models->Length;++i)
		{
			XS8* pFilename = (XS8*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filenames[i])).ToPointer();
			((Model*)models[i].ToPointer())->generateLightmap(models->Length,ppModels,lights->Length,ppLights,pFilename,maxSize,pixelsPerSurfAreaUnit,forcedSize,blurSize);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pFilename));
		}

		XDeleteArray(ppModels);
		XDeleteArray(ppLights);
	}

	void XVPM::generateAOMaps(array<IntPtr> ^models,array<System::String^>^ filenames,XU32 maxSize,XU32 quality,XU32 forcedSize,XF32 blurSize)
	{
		XModel** ppModels = NULL;
		XNewArray(ppModels,XModel*,models->Length,XOSMgr::getModelHeap());
		for (XS32 i = 0;i < models->Length;++i)
			ppModels[i] = ((Model*)models[i].ToPointer())->getModel();

		//Create a hemisphere of rays to sample with
		XU32 numPoints = 32;
		switch (quality)
		{
			case 1: numPoints = 64; break;
			case 2: numPoints = 128; break;
			case 3: numPoints = 128; break;
			case 4: numPoints = 256; break;
			case 5: numPoints = 256; break;
			case 6: numPoints = 512; break;
			case 7: numPoints = 512; break;
			case 8: numPoints = 1024; break;
		}
		XVector3D points[2048];
		XU32 numHSPoints = XIGenUniformHemiSpherePoints(numPoints*2,points,.3f);
		for (XU32 i = 0;i < numHSPoints;++i)
		{
			points[i] = points[i] - XVector3D::Zero;
			points[i].normalize();
		}

		for (XS32 i = 0;i < models->Length;++i)
		{
			XS8* pFilename = (XS8*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filenames[i])).ToPointer();
			((Model*)models[i].ToPointer())->generateAOMap(models->Length,ppModels,numHSPoints,points,pFilename,maxSize,quality,forcedSize,blurSize);
			System::Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pFilename));
		}

		XDeleteArray(ppModels);
	}

	IntPtr XVPM::initCamera(XU32 windowHandle)
	{
		//Create a new proxy object for the camera
		Camera* pProxy = NULL;
		XNew(pProxy,Camera(),XOSMgr::getDefaultHeap());
		
		pProxy->init(m_pScenes[windowHandle]);

		return IntPtr(pProxy);
	}

	void XVPM::destroyCamera(XU32 windowHandle,IntPtr camera)
	{
		X_ASSERTM(camera != IntPtr::Zero,"Destroying a invalid object");

		//Destroy all camera information
		Camera* pProxy = (Camera*)camera.ToPointer();

		//If the camera is currently set to a viewport, remove it first.
		for (XU32 i = 0;i < 4;++i)
			if (pProxy->getCamera() == m_sceneViews[windowHandle].userCameras[i])
				setStandardCamera(windowHandle,i);
			
		pProxy->destroy();
		XDelete(pProxy);
	}

	bool XVPM::setUserCamera(XU32 windowHandle,IntPtr camera,XU32 viewportIndex)
	{
		Camera* pProxy = (Camera*)camera.ToPointer();

		//We don't allow the same camera to be set to multiple viewports
		for (XU32 i = 0;i < 4;++i)
		{
			if (m_sceneViews[windowHandle].userCameras[i] == pProxy->getCamera())
				return false;
		}

		m_sceneViews[windowHandle].userCameras[viewportIndex] = pProxy->getCamera();

		//Set the user camera projection to match the selected projection
		if (getViewType(windowHandle,viewportIndex) == VT_PERSPECTIVE)
		{
			m_sceneViews[windowHandle].userCameras[viewportIndex]->setProjectionType(PERSPECTIVE);
			m_sceneViews[windowHandle].distortionCameras[viewportIndex].setProjectionType(PERSPECTIVE);
		}
		else
		{
			m_sceneViews[windowHandle].userCameras[viewportIndex]->setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[windowHandle].distortionCameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
		}

		
		m_sceneViews[windowHandle].distortionCameras[viewportIndex].setOrthoZoom(pProxy->getCamera()->getOrthoZoom());

		m_sceneViews[windowHandle].cameras[viewportIndex].detachController(&m_sceneViews[windowHandle].PRZControllers[viewportIndex]);
		pProxy->setToViewport(viewportIndex,&m_sceneViews[windowHandle].PRZControllers[viewportIndex]);

		return true;
	}

	void XVPM::setStandardCamera(XU32 windowHandle,XU32 viewportIndex)
	{
		if (m_sceneViews[windowHandle].userCameras[viewportIndex] != NULL)
			m_sceneViews[windowHandle].userCameras[viewportIndex]->detachController(&m_sceneViews[windowHandle].PRZControllers[viewportIndex]);
		m_sceneViews[windowHandle].userCameras[viewportIndex] = NULL;

		m_sceneViews[windowHandle].cameras[viewportIndex].attachController(&m_sceneViews[windowHandle].PRZControllers[viewportIndex]);

		//Set the user camera projection to match the selected projection
		if (getViewType(windowHandle,viewportIndex) == VT_PERSPECTIVE)
		{
			m_sceneViews[windowHandle].cameras[viewportIndex].setProjectionType(PERSPECTIVE);
			m_sceneViews[windowHandle].distortionCameras[viewportIndex].setProjectionType(PERSPECTIVE);
		}
		else
		{
			m_sceneViews[windowHandle].cameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
			m_sceneViews[windowHandle].distortionCameras[viewportIndex].setProjectionType(ORTHOGRAPHIC);
		}

		m_sceneViews[windowHandle].distortionCameras[viewportIndex].setOrthoZoom(m_sceneViews[windowHandle].cameras[viewportIndex].getOrthoZoom());

		m_pScenes[windowHandle]->setViewportCamera(viewportIndex,&m_sceneViews[windowHandle].cameras[viewportIndex]);
	}

	void XVPM::removeAllPostEffects(IntPtr camera)
	{
		Camera* pProxy = (Camera*)camera.ToPointer();
		pProxy->getCamera()->removeAllPostEffects();
	}
		
	void XVPM::addPostEffect(IntPtr camera,IntPtr postEffect)
	{
		Camera* pCameraProxy = (Camera*)camera.ToPointer();
		BasePostEffect* pBasePostEffectProxy = (BasePostEffect*)postEffect.ToPointer();
		pCameraProxy->getCamera()->addPostEffect(pBasePostEffectProxy->getPostEffect());
	}

	IntPtr XVPM::initLight(XU32 windowHandle)
	{
		//Create a new proxy object for the light
		Light* pProxy = NULL;
		XNew(pProxy,Light(),XOSMgr::getDefaultHeap());
		
		pProxy->init(m_pScenes[windowHandle]);

		return IntPtr(pProxy);
	}

	void XVPM::destroyLight(IntPtr light)
	{
		X_ASSERTM(light != IntPtr::Zero,"Destroying a invalid object");

		//Destroy all light information
		Light* pProxy = (Light*)light.ToPointer();
			
		pProxy->destroy();
		XDelete(pProxy);
	}

	IntPtr XVPM::initHDRPostEffect(IntPtr camera)
	{
		Camera* pCamera = (Camera*)camera.ToPointer();

		HDRPostEffect* pProxy = NULL;
		XNew(pProxy,HDRPostEffect(),XOSMgr::getDefaultHeap());
		pProxy->init(pCamera->getCamera());

		return IntPtr(pProxy);
	}

	void XVPM::destroyHDRPostEffect(IntPtr postEffect)
	{
		X_ASSERTM(postEffect != IntPtr::Zero,"Destroying a invalid object");
		HDRPostEffect* pProxy = (HDRPostEffect*)postEffect.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}

	IntPtr XVPM::initDOFPostEffect(IntPtr camera)
	{
		Camera* pCamera = (Camera*)camera.ToPointer();

		DOFPostEffect* pProxy = NULL;
		XNew(pProxy,DOFPostEffect(),XOSMgr::getDefaultHeap());
		pProxy->init(pCamera->getCamera());

		return IntPtr(pProxy);
	}

	void XVPM::destroyDOFPostEffect(IntPtr postEffect)
	{
		X_ASSERTM(postEffect != IntPtr::Zero,"Destroying a invalid object");
		DOFPostEffect* pProxy = (DOFPostEffect*)postEffect.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}
	
	IntPtr XVPM::initEdgePostEffect(IntPtr camera)
	{
		Camera* pCamera = (Camera*)camera.ToPointer();

		EdgePostEffect* pProxy = NULL;
		XNew(pProxy,EdgePostEffect(),XOSMgr::getDefaultHeap());
		pProxy->init(pCamera->getCamera());

		return IntPtr(pProxy);
	}

	void XVPM::destroyEdgePostEffect(IntPtr postEffect)
	{
		X_ASSERTM(postEffect != IntPtr::Zero,"Destroying a invalid object");
		EdgePostEffect* pProxy = (EdgePostEffect*)postEffect.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}
	
	IntPtr XVPM::initDistortionPostEffect(XU32 windowHandle,IntPtr camera)
	{
		Camera* pCamera = (Camera*)camera.ToPointer();

		DistortionPostEffect* pProxy = NULL;
		XNew(pProxy,DistortionPostEffect(),XOSMgr::getDefaultHeap());
		pProxy->init(m_pDistortionScenes[windowHandle],pCamera->getCamera());

		return IntPtr(pProxy);
	}

	void XVPM::destroyDistortionPostEffect(IntPtr postEffect)
	{
		X_ASSERTM(postEffect != IntPtr::Zero,"Destroying a invalid object");
		DistortionPostEffect* pProxy = (DistortionPostEffect*)postEffect.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}

	IntPtr XVPM::initMotionBlurPostEffect(XU32 windowHandle,IntPtr camera)
	{
		Camera* pCamera = (Camera*)camera.ToPointer();

		MotionBlurPostEffect* pProxy = NULL;
		XNew(pProxy,MotionBlurPostEffect(),XOSMgr::getDefaultHeap());
		pProxy->init(pCamera->getCamera());

		return IntPtr(pProxy);
	}

	void XVPM::destroyMotionBlurPostEffect(IntPtr postEffect)
	{
		X_ASSERTM(postEffect != IntPtr::Zero,"Destroying a invalid object");
		MotionBlurPostEffect* pProxy = (MotionBlurPostEffect*)postEffect.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}
	
	IntPtr XVPM::initAnimController(XU32 windowHandle)
	{
		//Create a new proxy object
		AnimController* pProxy = NULL;
		XNew(pProxy,AnimController(),XOSMgr::getDefaultHeap());
		
		pProxy->init(m_pScenes[windowHandle]);

		return IntPtr(pProxy);
	}

	void XVPM::destroyAnimController(IntPtr animController)
	{
		X_ASSERTM(animController != IntPtr::Zero,"Destroying a invalid object");

		//Destroy all anim controller information
		AnimController* pProxy = (AnimController*)animController.ToPointer();
			
		pProxy->destroy();
		XDelete(pProxy);
	}

	void XVPM::setNumAnimTracks(IntPtr animController,XU32 numAnimTracks)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->setNumAnimTracks(numAnimTracks);
	}

	void XVPM::setAnimTrack(IntPtr animController,XU32 trackIndex,IntPtr pTrackFilename)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->setAnimTrack(trackIndex,(XS8*)pTrackFilename.ToPointer());
	}

	XU32 XVPM::getAnimTrackTime(IntPtr animController,XU32 trackIndex)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		return pProxy->getAnimTrackTime(trackIndex);
	}

	XU32 XVPM::getAnimTrackDuration(IntPtr animController,XU32 trackIndex)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		return pProxy->getAnimTrackDuration(trackIndex);
	}

	void XVPM::setAnimTrackTime(IntPtr animController,XU32 trackIndex,XU32 milliseconds)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->setAnimTrackTime(trackIndex,milliseconds);
	}

	void XVPM::playAnimTrack(IntPtr animController,XU32 trackIndex,XF32 speedScale,XS32 playCount,XF32 weight)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->playAnimTrack(trackIndex,speedScale,playCount,weight);
	}

	void XVPM::pauseAnimTrack(IntPtr animController,XU32 trackIndex)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->pauseAnimTrack(trackIndex);
	}

	void XVPM::stopAnimTrack(IntPtr animController,XU32 trackIndex)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->stopAnimTrack(trackIndex);
	}
	
	void XVPM::stopAllTracks(IntPtr animController)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->stopAllTracks();
	}
	
	void XVPM::setAnimTrackSpeed(IntPtr animController,XU32 trackIndex,XF32 speed)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->setAnimTrackSpeed(trackIndex,speed);
	}

	void XVPM::setAnimTrackPlayCount(IntPtr animController,XU32 trackIndex,XS32 playCount)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->setAnimTrackPlayCount(trackIndex,playCount);
	}

	void XVPM::setAnimTrackWeight(IntPtr animController,XU32 trackIndex,XF32 weight)
	{
		AnimController* pProxy = (AnimController*)animController.ToPointer();
		pProxy->setAnimTrackWeight(trackIndex,weight);
	}

	
	IntPtr XVPM::initActorContainer(XU32 windowHandle)
	{
		//Create a new proxy object
		ActorContainer* pProxy = NULL;
		XNew(pProxy,ActorContainer(),XOSMgr::getDefaultHeap());
		
		pProxy->init(m_pScenes[windowHandle]);

		return IntPtr(pProxy);
	}

	void XVPM::destroyActorContainer(IntPtr actorContainer)
	{
		X_ASSERTM(actorContainer != IntPtr::Zero,"Destroying a invalid object");
		ActorContainer* pProxy = (ActorContainer*)actorContainer.ToPointer();
		pProxy->destroy();
		XDelete(pProxy);
	}

	
	XF32* XVPM::getActorContainerTransform(IntPtr actorContainer)
	{
		ActorContainer* pProxy = (ActorContainer*)actorContainer.ToPointer();
		return (XF32*)&pProxy->getActorContainer()->getModelTransform();
	}

	IntPtr XVPM::initPhysXController(XU32 windowHandle)
	{
		//Create a new proxy object
		PhysXController* pProxy = NULL;
		XNew(pProxy,PhysXController(),XOSMgr::getDefaultHeap());
		
		pProxy->init(m_pScenes[windowHandle]);

		return IntPtr(pProxy);
	}

	void XVPM::destroyPhysXController(IntPtr physXController)
	{
		X_ASSERTM(physXController != IntPtr::Zero,"Destroying a invalid object");

		//Destroy all anim controller information
		PhysXController* pProxy = (PhysXController*)physXController.ToPointer();
			
		pProxy->destroy();
		XDelete(pProxy);
	}

	void XVPM::addForce(IntPtr physXController,XF32 x,XF32 y,XF32 z)
	{
		PhysXController* pProxy = (PhysXController*)physXController.ToPointer();
		pProxy->addForce(XVector3D(x,y,z));
	}

	void XVPM::setIntProperty(IntPtr pProxyObject,IntPtr pPropertyName,XS32 value)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setIntProperty((XS8*)pPropertyName.ToPointer(),value);
	}

	void XVPM::setUIntProperty(IntPtr pProxyObject,IntPtr pPropertyName,XU32 value)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setUIntProperty((XS8*)pPropertyName.ToPointer(),value);
	}

	void XVPM::setFloatProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 value)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setFloatProperty((XS8*)pPropertyName.ToPointer(),value);
	}

	void XVPM::setDoubleProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF64 value)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setDoubleProperty((XS8*)pPropertyName.ToPointer(),value);
	}

	void XVPM::setBoolProperty(IntPtr pProxyObject,IntPtr pPropertyName,XBOOL value)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setBoolProperty((XS8*)pPropertyName.ToPointer(),value);
	}

	void XVPM::setColorProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 r,XF32 g,XF32 b,XF32 a)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setColorProperty((XS8*)pPropertyName.ToPointer(),XColor(r,g,b,a));
	}

	void XVPM::setEnumProperty(IntPtr pProxyObject,IntPtr pPropertyName,XS32 value)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setEnumProperty((XS8*)pPropertyName.ToPointer(),value);
	}

	void XVPM::setVector2DProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 x,XF32 y)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setVector2DProperty((XS8*)pPropertyName.ToPointer(),XVector2D(x,y));
	}

	void XVPM::setVector3DProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 x,XF32 y,XF32 z)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setVector3DProperty((XS8*)pPropertyName.ToPointer(),XVector3D(x,y,z));
	}

	void XVPM::setVector4DProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 x,XF32 y,XF32 z,XF32 w)
	{
		ProxyObject* pPO = (ProxyObject*)pProxyObject.ToPointer();
		pPO->setVector4DProperty((XS8*)pPropertyName.ToPointer(),XVector4D(x,y,z,w));
	}

	void XVPM::setMaterialShaderName(IntPtr pProxyObject,IntPtr pShaderName)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setShaderName((XS8*)pShaderName.ToPointer());
	}

	void XVPM::setMaterialTexture(IntPtr pProxyObject,XU32 samplerIndex,IntPtr pFilename)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setTexture(samplerIndex,(XS8*)pFilename.ToPointer());
	}

	void XVPM::setSamplerAddressU(IntPtr pProxyObject,XU32 samplerIndex,XS32 addressType)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerAddressU(samplerIndex,addressType);
	}

	void XVPM::setSamplerAddressV(IntPtr pProxyObject,XU32 samplerIndex,XS32 addressType)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerAddressV(samplerIndex,addressType);
	}

	void XVPM::setSamplerAddressW(IntPtr pProxyObject,XU32 samplerIndex,XS32 addressType)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerAddressW(samplerIndex,addressType);
	}

	void XVPM::setSamplerMipLODBias(IntPtr pProxyObject,XU32 samplerIndex,XF32 bias)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerMipLODBias(samplerIndex,bias);
	}

	void XVPM::setSamplerMaxAnisotropy(IntPtr pProxyObject,XU32 samplerIndex,XU32 maxAnisotropy)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerMaxAnisotropy(samplerIndex,maxAnisotropy);
	}

	void XVPM::setSamplerMinLOD(IntPtr pProxyObject,XU32 samplerIndex,XF32 minLOD)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerMinLOD(samplerIndex,minLOD);
	}

	void XVPM::setSamplerMaxLOD(IntPtr pProxyObject,XU32 samplerIndex,XF32 maxLOD)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerMaxLOD(samplerIndex,maxLOD);
	}

	void XVPM::setSamplerBorderColor(IntPtr pProxyObject,XU32 samplerIndex,XF32 r,XF32 g,XF32 b,XF32 a)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerBorderColor(samplerIndex,r,g,b,a);
	}

	void XVPM::setSamplerFilter(IntPtr pProxyObject,XU32 samplerIndex,XS32 filterType)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerFilter(samplerIndex,filterType);
	}

	void XVPM::setSamplerFunc(IntPtr pProxyObject,XU32 samplerIndex,XS32 comparisonFunc)
	{
		MaterialNP* pMaterialNP = (MaterialNP*)pProxyObject.ToPointer();
		pMaterialNP->setSamplerFunc(samplerIndex,comparisonFunc);
	}

}