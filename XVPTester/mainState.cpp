
#include <xvpsdk.h>
#include <mainState.h>
#include <characterActor.h>
#include <mmsystem.h>

MainState MainState::m_MainState;


enum
{
	UL_DEFAULT,
	UL_CAMERAS,
	NUM_UPDATE_LEVELS
};

enum
{
	RL_OPAQUE,
	RL_OCCLUSION,
	RL_ALPHA,
	RL_PARTICLES,
	RL_DEBUG,
	NUM_RENDER_LEVELS
};

XPRZController nc;




XStatus MainState::init()
{
	XSceneDesc sceneDesc;
	strcpy(sceneDesc.name,"Main Scene");
	sceneDesc.autoAlphaRenderLevel = RL_ALPHA;
	sceneDesc.enableResizeEvents = true;
	
	//Render Targets
	sceneDesc.renderTargetArrayDesc.numRenderTargets = 2;
	sceneDesc.renderTargetArrayDesc.width = 1280;
	sceneDesc.renderTargetArrayDesc.height = 1024;
	sceneDesc.renderTargetArrayDesc.formats[0] = X_DFT_R16G16B16A16_FLOAT;
	sceneDesc.renderTargetArrayDesc.formats[1] = X_DFT_R16G16_FLOAT;
	sceneDesc.renderTargetArrayDesc.useDepthStencil = true;
	sceneDesc.renderTargetArrayDesc.clearColors[0] = XColor(.3f,.3f,.3f,1);
	sceneDesc.renderTargetArrayDesc.clearColors[1] = XColor(0,0,0,0);

	//Viewports
	sceneDesc.viewportArrayDesc.numViewports = 1;
	sceneDesc.viewportArrayDesc.viewports[0].topLeftX = 0;
	sceneDesc.viewportArrayDesc.viewports[0].topLeftY = 0;
	sceneDesc.viewportArrayDesc.viewports[0].width = XVP::GraphicsMgr.getRenderContextWidth(rcHandle);
	sceneDesc.viewportArrayDesc.viewports[0].height = XVP::GraphicsMgr.getRenderContextHeight(rcHandle);
	sceneDesc.viewportArrayDesc.enablePostEffects[0] = true;

	sceneDesc.viewportArrayDesc.viewports[1].topLeftX = XVP::GraphicsMgr.getRenderContextWidth(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[1].topLeftY = 0;
	sceneDesc.viewportArrayDesc.viewports[1].width = XVP::GraphicsMgr.getRenderContextWidth(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[1].height = XVP::GraphicsMgr.getRenderContextHeight(rcHandle)/2;
	sceneDesc.viewportArrayDesc.enablePostEffects[1] = true;

	sceneDesc.viewportArrayDesc.viewports[2].topLeftX = 0;
	sceneDesc.viewportArrayDesc.viewports[2].topLeftY = XVP::GraphicsMgr.getRenderContextHeight(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[2].width = XVP::GraphicsMgr.getRenderContextWidth(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[2].height = XVP::GraphicsMgr.getRenderContextHeight(rcHandle)/2;
	sceneDesc.viewportArrayDesc.enablePostEffects[2] = true;

	sceneDesc.viewportArrayDesc.viewports[3].topLeftX = XVP::GraphicsMgr.getRenderContextWidth(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[3].topLeftY = XVP::GraphicsMgr.getRenderContextHeight(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[3].width = XVP::GraphicsMgr.getRenderContextWidth(rcHandle)/2;
	sceneDesc.viewportArrayDesc.viewports[3].height = XVP::GraphicsMgr.getRenderContextHeight(rcHandle)/2;
	sceneDesc.viewportArrayDesc.enablePostEffects[3] = true;



	//Update levels
	sceneDesc.numUpdateLevels = NUM_UPDATE_LEVELS;
	strcpy(sceneDesc.updateLevelDescs[UL_DEFAULT].name,"DEFAULT");
	strcpy(sceneDesc.updateLevelDescs[UL_CAMERAS].name,"UL_CAMERAS");


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
	sceneDesc.renderLevelDescs[RL_DEBUG].sortType = X_ST_NONE;
	sceneDesc.renderLevelDescs[RL_DEBUG].maxRenderEvents = 2048;

	//Initialize scene
	m_mainScene.init(sceneDesc);
	//m_mainScene.setPostRenderCallback(RenderCallback);
	XVP::GraphicsMgr.addScene(1,0,&m_mainScene,NULL);


	//Create UI scene
	XSceneDesc UISceneDesc;
	strcpy(UISceneDesc.name,"UI Scene");
	UISceneDesc.enableResizeEvents = true;
	
	//Render Targets
	UISceneDesc.renderTargetArrayDesc.numRenderTargets = 0;

	//Viewports
	UISceneDesc.viewportArrayDesc.numViewports = 1;
	UISceneDesc.viewportArrayDesc.viewports[0].topLeftX = 0;
	UISceneDesc.viewportArrayDesc.viewports[0].topLeftY = 0;
	UISceneDesc.viewportArrayDesc.viewports[0].width = XVP::GraphicsMgr.getRenderContextWidth(rcHandle);
	UISceneDesc.viewportArrayDesc.viewports[0].height = XVP::GraphicsMgr.getRenderContextHeight(rcHandle);
	UISceneDesc.viewportArrayDesc.enablePostEffects[0] = false;

	//Update levels
	UISceneDesc.numUpdateLevels = 0;

	//Render levels
	UISceneDesc.numRenderLevels = 0;
	
	//Initialize scene
	m_uiScene.init(UISceneDesc);
	XVP::GraphicsMgr.addScene(1,1,&m_uiScene,NULL);
	
	c.init();
	
	
	m_mainScene.addUpdateNode(&c,UL_CAMERAS);

	hdrPostEffect.init(XVP::GraphicsMgr.getRenderContextWidth(rcHandle),XVP::GraphicsMgr.getRenderContextHeight(rcHandle));
	hdrPostEffect.setMiddleGrayKey(.9f);

	
	m_mainScene.setViewportCamera(0,&c);
	c.addPostEffect(&hdrPostEffect);
	

	DWORD start = timeGetTime();
	
	m_mainScene.getLightMgr()->setLightingMode(X_LM_SCENE);


	
	fpc.init();
	
	c.attachController(&fpc);
	fpc.setPosition(XVector3D(0,0,-70));
	

	/*XOSMgr::setPath(X_PATH_TEXTURE,"../samples/common/media/ui/");
	XOSMgr::setPath(X_PATH_MOVIE,"../samples/common/media/ui/");
	XOSMgr::setPath(X_PATH_AUDIO,"../samples/common/media/ui/");
	XOSMgr::setPath(X_PATH_UI_TEMPLATE,"../samples/common/media/ui/");
	m_UI.load("../samples/5.1/ui.lua");
	m_uiScene.addUIToScene(&m_UI);
	m_UI.setOpacity(.5f);*/


	mt.init("d:/scratch/test");
	//mt.mergeLOD("c:/scratch/bike_max8_test_lod1.xmf",100); 
	//mt.mergeLOD("c:/scratch/bike_max8_test_lod2.xmf",200); 
	mt.generateModel(m);

dsc.init();
	//m.setModelPosition(XVector3D(0,0,-20));
	//m.attachController(NULL,&mlc,true);
	m_mainScene.addUpdateNode(&m);
	m_mainScene.addRenderNode(&m);
	mt.attachShaderController(NULL,&dsc);
	
	

	
	//sphere = XPhysXController::CreateSphere(NxVec3(-5,0,5), 1, 10);
	//capsule = XPhysXController::CreateCapsule(NxVec3(0,0,0), 2, 0.5, 10);


	XState::init();
	return X_OK;
}

XStatus MainState::destroy()
{

	c.destroy();
	
	

	
	m.destroy();
	mt.destroy();

	
	fpc.destroy();



	

	//m_UI.destroy();

	m_mainScene.destroy();
	hdrPostEffect.destroy();

	
	XState::destroy();
	return X_OK;
}

XStatus MainState::activate(void* pUserData)
{
	return X_OK;
}
	
XStatus MainState::deactivate()
{
	return X_OK;
}
	
XStatus MainState::update(XF32 dt)
{		

	if (XVP::InputMgr.keyDown(X_KEY_ESCAPE))
		PostQuitMessage(0);

	if (XVP::InputMgr.keyDown(X_KEY_W))
		fpc.setLookVelocity(100);
	if (XVP::InputMgr.keyDown(X_KEY_S))
		fpc.setLookVelocity(-100);
	if (XVP::InputMgr.keyDown(X_KEY_A))
		fpc.setRightVelocity(-100);
	if (XVP::InputMgr.keyDown(X_KEY_D))
		fpc.setRightVelocity(100);
	if (XVP::InputMgr.keyDown(X_KEY_Q))
		fpc.setUpVelocity(-100);
	
		
	return X_OK;
}

XS8* MainState::getStateDesc()
{
	static char description[X_STATE_DESC_LEN] = "MainState";
	return description; 
}


