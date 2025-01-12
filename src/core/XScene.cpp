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
//	Module Name:	XScene.cpp
//	Author:			Lynn Duke
//	Creation Date:	3-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XVector3D XScene::m_activeCameraPosition;
//XMemBlockList<HitInfo> hits2;

//*****************************************************************************
//*****************************************************************************
XScene::XScene()
{
	//m_pAlphaRenderEvents = NULL;
	//m_numAlphaRenderEvents = 0;
	//m_maxAlphaRenderEvents = 0;
	m_alphaSort = true;
	m_renderSprites = true;
	m_spriteMaterial = X_INVALID_HANDLE;
	m_spriteVertexShader = X_INVALID_HANDLE;
	m_spritePixelShader = X_INVALID_HANDLE;
	m_spriteVBHandle = X_INVALID_HANDLE;
	m_VSSceneConstants = X_INVALID_HANDLE;
	m_GSSceneConstants = X_INVALID_HANDLE;
	m_PSSceneConstants = X_INVALID_HANDLE;
	m_renderContext = X_INVALID_HANDLE;
	m_renderTargetArray = X_INVALID_HANDLE;
	
	m_enableResizeEvents = false;

	//m_enableUpdates = true;
	m_numUpdateLevels = 0;
	m_pUpdateLevelEnabled = NULL;
	m_pUpdateLevelDescs = NULL;
	m_pUpdateNodeLists = NULL;
	m_pRenderNodeLists = NULL;
	//m_pSceneNodeCounts = NULL;
	//m_enableRenders = true;
	m_numRenderLevels = 0;
	m_pRenderLevelEnabled = NULL;
	m_pRenderLevelDescs = NULL;
	m_pRenderEventLists = NULL;
	m_pRenderEventCounts = NULL;
	m_activeRenderLevel = 0;
	//m_numPreRenderEvents = 0;
	//m_maxPreRenderEvents = 0;
	//m_pPreRenderEvents = NULL;
	m_enableColorClear = true;
	m_enableDepthClear = true;
	m_enableStencilClear = true;

	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
	{
		m_pViewportCameras[i] = NULL;
		m_viewportRTAs[0][i] = X_INVALID_HANDLE;
		m_viewportRTAs[1][i] = X_INVALID_HANDLE;
		m_lastRTA[i] = -1;
		m_wireFillMode[i] = false;
	}
	m_wireMatHandle = X_INVALID_HANDLE;
	m_pActiveCamera = NULL;
	m_activeViewportIndex = -1;

	m_pPreRenderCallback = NULL;
	m_pPostRenderCallback = NULL;

	m_selMatHandle = X_INVALID_HANDLE;
	m_selectionFilter = X_SF_NONE;

	m_enable = true;

	m_pointClampState = X_INVALID_HANDLE;

	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		m_texOverrides[i].rta = X_INVALID_HANDLE;
		m_texOverrides[i].rtIndex = 0;
		m_texOverrides[i].samplerState = X_INVALID_HANDLE;
	}

	m_materialOverride = X_INVALID_HANDLE;
	m_fogColor = XColor(.5f,.5f,.5f,1.0f);
	m_fogStart = 0.0f;
	m_fogEnd = 10000.0f;
	m_fogDensity = 1.0f;
	m_globalAmbient = XColor(.2f,.2f,.2f,1.0f);


	m_inited = false;
}



//*****************************************************************************
//*****************************************************************************
XScene::~XScene()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::init(XSceneDesc& desc)
{
	X_ASSERTM(desc.numUpdateLevels <= X_MAX_UPDATE_LEVELS,"Max number of update levels exceeded");
	X_ASSERTM(desc.numRenderLevels <= X_MAX_RENDER_LEVELS,"Max number of render levels exceeded");
	//X_ASSERTM((desc.enableUpdates || desc.enableRenders),"Updates and renders are disabled.  A scene must have at least one of these enabled.");
	X_ASSERTM((desc.autoAlphaRenderLevel >= -1) && (desc.autoAlphaRenderLevel < desc.numRenderLevels),"Invalid auto alpha render level");
	X_DEBUG
	(
		//if (desc.enableUpdates && desc.numUpdateLevels == 0)
		//	X_ASSERT_FORCED("Updates are enabled but there are no update levels");
		//if (desc.enableRenders && desc.numRenderLevels == 0)
		//	X_ASSERT_FORCED("Renders are enabled but there are no render levels");
		for (XU32 i = 0;i < desc.viewportArrayDesc.numViewports;++i)
		{
			if (desc.viewportArrayDesc.viewports[i].width == 0 ||	desc.viewportArrayDesc.viewports[i].height == 0)
				X_ASSERT_FORCED("Viewport width or height cannot be 0");
			if (desc.viewportArrayDesc.viewports[i].minDepth == 0.0f && desc.viewportArrayDesc.viewports[i].maxDepth == 0.0f)
				X_ASSERT_FORCED("Viewport min depth and max depth cannot both be 0");
		}
		//if (desc.enableRenders && desc.maxPreRenderEvents == 0)
		//	X_ASSERT_FORCED("Renders are enabled but max pre render events is 0.");
	)

		
	//Store the render context and add the scene to it
	//m_renderContext = desc.renderContext;
	//XVP::GraphicsMgr.addScene(desc.renderContext,this);
	
	//Store two copies of the viewport array.  One will always hold the inital dimiensions
	//of the viewports and the other will hold the scaled versions from resize events
	//m_orgViewportArrayDesc = desc.viewportArrayDesc;
	m_viewportArrayDesc = desc.viewportArrayDesc;
	for (XU32 i = 0;i < desc.viewportArrayDesc.numViewports;++i)
	{
		m_normalizedViewports[i].left = desc.viewportArrayDesc.viewports[i].topLeftX / (XF32)desc.renderTargetArrayDesc.width;
		m_normalizedViewports[i].top = desc.viewportArrayDesc.viewports[i].topLeftY / (XF32)desc.renderTargetArrayDesc.height;
		m_normalizedViewports[i].right = (desc.viewportArrayDesc.viewports[i].topLeftX + desc.viewportArrayDesc.viewports[i].width) / (XF32)desc.renderTargetArrayDesc.width;
		m_normalizedViewports[i].bottom = (desc.viewportArrayDesc.viewports[i].topLeftY + desc.viewportArrayDesc.viewports[i].height) / (XF32)desc.renderTargetArrayDesc.height;
	}

	//Store other important information
	m_enableResizeEvents = desc.enableResizeEvents;
	//m_enableUpdates = desc.enableUpdates;
	m_numUpdateLevels = desc.numUpdateLevels;
	//m_enableRenders = desc.enableRenders;
	m_numRenderLevels = desc.numRenderLevels;
	//m_maxPreRenderEvents = desc.maxPreRenderEvents;
	m_enableColorClear = desc.enableColorClear;
	m_enableDepthClear = desc.enableDepthClear;
	m_enableStencilClear = desc.enableStencilClear;

	//Create scene-specific render target array if specified
	if (desc.renderTargetArrayDesc.numRenderTargets != 0)
	{
		XStatus status = XVP::GraphicsMgr.createRenderTargetArray(desc.renderTargetArrayDesc,m_renderTargetArray);
		X_ASSERTM(status == X_OK,"Could not create render target array for scene");
	}
	
	//Allocate update levels
	if (desc.numUpdateLevels > 0)
	{
		XNewArray(m_pUpdateLevelEnabled,XBOOL,desc.numUpdateLevels,XOSMgr::getDefaultHeap());
		XNewArray(m_pUpdateLevelDescs,XUpdateLevelDesc,desc.numUpdateLevels,XOSMgr::getDefaultHeap());
		XNewArray(m_pUpdateNodeLists,XMemBlockList<XSceneNode*>,desc.numUpdateLevels,XOSMgr::getDefaultHeap());
		//XNewArray(m_pSceneNodeCounts,XU32,desc.numUpdateLevels,XOSMgr::getDefaultHeap());

		for (XU32 i = 0;i < desc.numUpdateLevels;++i)
		{
			//Copy update level description
			m_pUpdateLevelDescs[i] = desc.updateLevelDescs[i];
		}
	}

	//Allocate render levels
	if (desc.numRenderLevels > 0)
	{
		XNewArray(m_pRenderLevelEnabled,XBOOL,desc.numRenderLevels,XOSMgr::getDefaultHeap());
		XNewArray(m_pRenderLevelDescs,XRenderLevelDesc,desc.numRenderLevels,XOSMgr::getDefaultHeap());
		XNewArray(m_pRenderEventLists,XRenderEvent*,desc.numRenderLevels,XOSMgr::getDefaultHeap());
		XNewArray(m_pRenderNodeLists,XMemBlockList<XSceneNode*>,desc.numRenderLevels,XOSMgr::getDefaultHeap());
		XNewArray(m_pRenderEventCounts,XU32,desc.numRenderLevels,XOSMgr::getDefaultHeap());

		for (XS32 i = 0;i < desc.numRenderLevels;++i)
		{
			//Copy render level descriptionm
			m_pRenderLevelDescs[i] = desc.renderLevelDescs[i];

			//Allocate render event list for render level
			XNewArray(m_pRenderEventLists[i],XRenderEvent,desc.renderLevelDescs[i].maxRenderEvents,XOSMgr::getDefaultHeap());
			X_ASSERTM(m_pRenderEventLists[i],"Could not allocate render event list");	

			//Start count at 0
			m_pRenderEventCounts[i] = 0;
		}
	}
	
	//Allocate pre prender events
	/*if (m_maxPreRenderEvents > 0)
	{
		XNewArray(m_pPreRenderEvents,XSceneNode*,m_maxPreRenderEvents,XOSMgr::getDefaultHeap());
		X_ASSERTM(m_pPreRenderEvents,"Could not allocate pre prender event list");
	}*/

	//Allocate viewport post effect render targets
	//createViewportRenderTargets(desc.viewportArrayDesc);

	//Start up the light manager for the scene
	m_lightMgr.init(this);

	//Create sprite shaders and material
	XVP::GraphicsMgr.createVertexShader("Sprite.fx","mainVS",m_spriteVertexShader);
	XVP::GraphicsMgr.createPixelShader("Sprite.fx","mainPS",m_spritePixelShader);
	XVP::GraphicsMgr.createMaterial(NULL,m_spriteMaterial);
	XMaterial* pMaterial = XVP::GraphicsMgr.lockMaterial(m_spriteMaterial);
	pMaterial->enableDepthTest(false);
	pMaterial->enableAlphaBlend(0,true);
	pMaterial->setBlendOp(X_BOT_ADD);
	pMaterial->setSrcBlend(X_BT_SRC_ALPHA);
	pMaterial->setDestBlend(X_BT_INV_SRC_ALPHA);
	XVP::GraphicsMgr.unlockMaterial(m_spriteMaterial);

	//Create wireframe material
	XVP::GraphicsMgr.createMaterial(NULL,m_wireMatHandle);
	pMaterial = XVP::GraphicsMgr.lockMaterial(m_wireMatHandle);
	pMaterial->setFillMode(X_FMT_WIRE);
	XVP::GraphicsMgr.unlockMaterial(m_wireMatHandle);	

	//Init matrix stack for this scene
	m_matrixStack.init(desc.matrixStackSize,"MatrixStack");
	m_matrixStack.insertBack();

	//Create sprite vertex buffer
	XVertexDesc vertexDesc;
	vertexDesc.numColors = 1;
	vertexDesc.numUVs = 1;
	XVP::GraphicsMgr.createVertexBuffer(vertexDesc,X_PT_TRIANGLE_STRIP,4,true,m_spriteVBHandle);

	//Create scene and node constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(SceneConstants),true,m_VSSceneConstants);
	XVP::GraphicsMgr.createConstantBuffer(sizeof(SceneConstants),true,m_GSSceneConstants);
	XVP::GraphicsMgr.createConstantBuffer(sizeof(SceneConstants),true,m_PSSceneConstants);

	m_renderSprites = true;

	//Create selection material and controller
	m_selectionSC.init();
	m_selectionSC.setColor(XColor(1,1,1,1));
	m_selectionSC.setDepthBias(-.0005f);
	XVP::GraphicsMgr.createMaterial("_SelectionMaterial_",m_selMatHandle);
	XMaterial* pSelMat = XVP::GraphicsMgr.lockMaterial(m_selMatHandle);
	pSelMat->attachShaderController(&m_selectionSC);
	pSelMat->setFillMode(X_FMT_WIRE);
	XVP::GraphicsMgr.unlockMaterial(m_selMatHandle);

#ifdef XVP_EDITOR
	m_gizmoMgr.init(this);
#endif

	XSamplerDesc samplerDesc;
	samplerDesc.filter = X_TFT_MIN_MAG_MIP_POINT;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_pointClampState);

	m_inited = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::destroy()
{
	XVP::GraphicsMgr.destroySamplerState(m_pointClampState);

#ifdef XVP_EDITOR
	m_gizmoMgr.destroy();
#endif

	//Empty selection set
	m_selectedNodes.clear();
	m_selectionSC.destroy();

	//Destroy scene and node constant buffers
	if (m_VSSceneConstants != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyConstantBuffer(m_VSSceneConstants);
	if (m_GSSceneConstants != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyConstantBuffer(m_GSSceneConstants);
	if (m_PSSceneConstants != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyConstantBuffer(m_PSSceneConstants);

	removeAllUpdateNodes();
	removeAllRenderNodes();
	m_frontSpriteArray.clear();
	m_backSpriteArray.clear();
	m_spriteCmdQueue.clear();
	m_UIArray.clear();
	m_UICmdQueue.clear();
	m_nodeCmdQueue.clear();
	
	//if (m_renderContext != X_INVALID_HANDLE)
	//	XVP::GraphicsMgr.removeScene(m_renderContext,this);

	//Destroy scene's render target array
	if (m_renderTargetArray != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyRenderTargetArray(m_renderTargetArray);
	m_renderTargetArray = X_INVALID_HANDLE;

	//Destroy post effect render target arrays
	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
	{
		if (m_viewportRTAs[0][i] != X_INVALID_HANDLE)
			XVP::GraphicsMgr.destroyRenderTargetArray(m_viewportRTAs[0][i]);
		if (m_viewportRTAs[1][i] != X_INVALID_HANDLE)
			XVP::GraphicsMgr.destroyRenderTargetArray(m_viewportRTAs[1][i]);
		m_viewportRTAs[0][i] = X_INVALID_HANDLE;
		m_viewportRTAs[1][i] = X_INVALID_HANDLE;
	}

	XDeleteArray(m_pUpdateLevelEnabled);
	XDeleteArray(m_pUpdateLevelDescs);
	XDeleteArray(m_pRenderLevelEnabled);
	XDeleteArray(m_pRenderLevelDescs);
	XDeleteArray(m_pRenderEventCounts);
	if (m_pUpdateNodeLists)
	{
		for (XU32 i = 0;i < m_numUpdateLevels;++i)
			m_pUpdateNodeLists[i].clear();			
		XDeleteArray(m_pUpdateNodeLists);
	}

	if (m_pRenderNodeLists)
	{
		for (XU32 i = 0;i < m_numRenderLevels;++i)
			m_pRenderNodeLists[i].clear();			
		XDeleteArray(m_pRenderNodeLists);
	}

	if (m_pRenderEventLists)
	{
		for (XU32 i = 0;i < m_numRenderLevels;++i)
			XDeleteArray(m_pRenderEventLists[i]);			
		XDeleteArray(m_pRenderEventLists);
	}
#ifdef XVP_EDITOR
	m_gizmoList.clear();
#endif

	//XDeleteArray(m_pPreRenderEvents);

	m_lightMgr.destroy();

	if (m_spriteVertexShader != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyVertexShader(m_spriteVertexShader);
	if (m_spritePixelShader != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyPixelShader(m_spritePixelShader);
	if (m_spriteMaterial != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyMaterial(m_spriteMaterial);
	if (m_wireMatHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyMaterial(m_wireMatHandle);
	if (m_spriteVBHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyVertexBuffer(m_spriteVBHandle);

	m_matrixStack.destroy();

	#pragma message("can't destroy cursor only once because of static calls from multiple XScenes!! now what?")
	XUserInterface::destroyCursor();

	m_renderContext = X_INVALID_HANDLE;
	m_inited = false;
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XScene::update(XF32 dt)
{
	//If not enabled, we're done
	if (!m_enable)
		return X_OK;

	//Update the scene graph.
	X_DEBUG_LOG_LN("---------- BEGIN SCENE UPDATE ----------",X_DLL_VERBOSE);

	m_updateState = X_US_SCENE_GRAPH;
	XMemBlockList<XSceneNode*>::iterator itr;
	
	//Update scene nodes and prep them for rendering
	for(XU32 updateLevel = 0;updateLevel < m_numUpdateLevels;++updateLevel)
	{
		if (!m_pUpdateLevelEnabled[updateLevel])
			continue;

		itr.set(m_pUpdateNodeLists[updateLevel].begin());
		while (itr.isValid())
		{
			//Update the node and don't forget to remove one from the matrix stack afterwards
			itr.getValue()->prepForUpdate(dt,this,NULL,&m_matrixStack);
			m_matrixStack.removeBack();

			itr.getNext();
		}
	}

#ifdef XVP_EDITOR
	m_gizmoMgr.update(dt);

	itr.set(m_gizmoList.begin());
	while (itr.isValid())
	{
		//Update the node and don't forget to remove one from the matrix stack afterwards
		itr.getValue()->prepForUpdate(dt,this,NULL,&m_matrixStack);
		m_matrixStack.removeBack();

		itr.getNext();
	}
#endif

	m_updateState = X_US_NORMAL;
	processNodeCmds();

	X_DEBUG_LOG_LN("---------- END SCENE UPDATE ----------",X_DLL_VERBOSE);

	postUpdate(dt);

	return X_OK;
}


//***************************************************************************** 
//***************************************************************************** 
XStatus XScene::postUpdate(XF32 dt)
{
	return X_OK;
}



//***************************************************************************** 
/**
Renders all registered scene nodes.  This function will notify each registered
scene node that it should render itself.  Once this function completes,
all scene node registrations will be cleared.

@param matHandle	The handle of the material for which to render its clients.
@return None.
*/
//***************************************************************************** 
XStatus XScene::render(XF32 dt)
{
	//If not enabled, we're done
	if (!m_enable)
		return X_OK;

	//If we have our own render target array, let's activate it and render to it.  Otherwise
	//we'll render this scene into it's parent render context.  The parent's render context
	//should already be set so there's really nothing more to do if we don't have our own render
	//target array.
	if (m_renderTargetArray != X_INVALID_HANDLE)
	{
		XVP::GraphicsMgr.setRenderTargetArray(m_renderTargetArray);
		XVP::GraphicsMgr.clearRenderTargetArray(m_renderTargetArray,m_enableColorClear,m_enableDepthClear,m_enableStencilClear);
	}
	
	//Set texture overrides if they exist
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		if (m_texOverrides[i].rta != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.setTextureOverrideFromRTA(i,m_texOverrides[i].samplerState,m_texOverrides[i].rta,m_texOverrides[i].rtIndex);
			//XVP::GraphicsMgr.setSamplerState(m_samplerOverride,m_texOverrides[i].rtIndex);
		}
	}

	m_updateState = X_US_RENDER;

	XMemBlockList<XSpriteInfo>::iterator spriteItr;

	beginSpriteRendering();
	X_DEBUG_LOG_LN("---------- BEGIN BACK SPRITE ARRAY RENDER ----------",X_DLL_VERBOSE);
	X_START_PROFILE("Back Sprites Render")
	if (m_renderSprites)
	{
		m_updateState = X_US_SPRITES;
		spriteItr.set(m_backSpriteArray.begin());
		while (spriteItr.isValid())
		{
			spriteItr.getValue().pSprite->update(dt);
			spriteItr.getNext();
		}
		m_updateState = X_US_NORMAL;
		processSpriteCmds(dt);
		m_updateState = X_US_SPRITES;
		spriteItr.set(m_backSpriteArray.begin());
		while (spriteItr.isValid())
		{
			spriteItr.getValue().pSprite->render();
			spriteItr.getNext();
		}
		m_updateState = X_US_NORMAL;
		processSpriteCmds(dt);
	}
	endSpriteRendering();
	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END BACK SPRITE ARRAY RENDER ----------",X_DLL_VERBOSE);

	XVP::GraphicsMgr.removeAllTextures();

	//For each viewport in the scene, render scene nodes
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		//If there is no camera assigned to this viewport, we can't render anything
		//so continue
		if (!m_pViewportCameras[i])
			continue;

		//Set the camera to match the viewport
		m_pViewportCameras[i]->setViewportWidth(m_viewportArrayDesc.viewports[i].width);
		m_pViewportCameras[i]->setViewportHeight(m_viewportArrayDesc.viewports[i].height);

		//Set the viewport and scissor rect to the device
		XVP::GraphicsMgr.setViewport(m_viewportArrayDesc.viewports[i]);
		XVP::GraphicsMgr.setScissorRect(XRect(m_viewportArrayDesc.viewports[i].topLeftX,
											  m_viewportArrayDesc.viewports[i].topLeftY,
											  m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width,
											  m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height));

		//Update the scene's viewport constant buffer and bind it
		SceneConstants* pVSSC = (SceneConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSSceneConstants,X_LT_DISCARD);
		SceneConstants* pGSSC = (SceneConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_GSSceneConstants,X_LT_DISCARD);
		SceneConstants* pPSSC = (SceneConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSSceneConstants,X_LT_DISCARD);
		
		pVSSC->viewMatrix = m_pViewportCameras[i]->getViewMatrix();
		pVSSC->invViewMatrix = pVSSC->viewMatrix.getInverse();
		pVSSC->projMatrix = m_pViewportCameras[i]->getProjMatrix();
		pVSSC->invProjMatrix = pVSSC->projMatrix.getInverse();
		pVSSC->viewProjMatrix = pVSSC->viewMatrix * pVSSC->projMatrix;
		pVSSC->invViewProjMatrix = pVSSC->viewProjMatrix.getInverse();

		//Set occlusion matrices
		//m_lightMgr.getOccludingLights(&pVSSC->occlusionMatrices);
	
		//Create UVs that represent the viewport coordinates in the original
		//render target array for the scene.  
		XRenderTargetArrayDesc* pBaseRTADesc = XVP::GraphicsMgr.getRenderTargetArrayDesc(m_renderTargetArray);
		XF32 minU = m_viewportArrayDesc.viewports[i].topLeftX / (XF32)pBaseRTADesc->width;
		XF32 minV = m_viewportArrayDesc.viewports[i].topLeftY / (XF32)pBaseRTADesc->height;
		XF32 maxU =	(m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width) / (XF32)pBaseRTADesc->width;
		XF32 maxV = (m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height) / (XF32)pBaseRTADesc->height;

		//width, height, xoffset, yoffset..all normalized
		pVSSC->viewportParams.x = maxU - minU;
		pVSSC->viewportParams.y = maxV - minV;
		pVSSC->viewportParams.z = minU;
		pVSSC->viewportParams.w = minV;

		//Fog params and elapsed time
		pVSSC->v4FogParams.x = m_fogStart;
		pVSSC->v4FogParams.y = m_fogEnd;
		pVSSC->v4FogParams.z = m_fogDensity;
		pVSSC->v4FogParams.w = XVP::getFrameDt();
		pVSSC->v4FogColor.x = m_fogColor.m_R;
		pVSSC->v4FogColor.y = m_fogColor.m_G;
		pVSSC->v4FogColor.z = m_fogColor.m_B;
		pVSSC->v4FogColor.w = XVP::getDuration();
		pVSSC->v4GlobalAmbient.x = m_globalAmbient.m_R;
		pVSSC->v4GlobalAmbient.y = m_globalAmbient.m_G;
		pVSSC->v4GlobalAmbient.z = m_globalAmbient.m_B;

		//Copy vs struct in gs and ps
		memcpy(pGSSC,pVSSC,sizeof(SceneConstants));
		memcpy(pPSSC,pVSSC,sizeof(SceneConstants));

		XVP::GraphicsMgr.unlockConstantBuffer(m_VSSceneConstants);
		XVP::GraphicsMgr.unlockConstantBuffer(m_GSSceneConstants);
		XVP::GraphicsMgr.unlockConstantBuffer(m_PSSceneConstants);


		XVP::GraphicsMgr.setVSConstantBuffer(X_SCENE_SLOT,m_VSSceneConstants);
		XVP::GraphicsMgr.setPSConstantBuffer(X_SCENE_SLOT,m_GSSceneConstants);
		XVP::GraphicsMgr.setGSConstantBuffer(X_SCENE_SLOT,m_PSSceneConstants);

		//Set the active camera and active viewport
		m_pActiveCamera = m_pViewportCameras[i];
		m_activeViewportIndex = i;

		//If there's a pre render callback registered, call it now
		if (m_pPreRenderCallback)
			m_pPreRenderCallback(this);

		//At this point, with an active camera set, we can prep each scene node for rendering.
		XMemBlockList<XSceneNode*>::iterator itr;
		for(XU32 renderLevel = 0;renderLevel < m_numRenderLevels;++renderLevel)
		{
			if (!m_pRenderLevelEnabled[renderLevel])
				continue;

			itr.set(m_pRenderNodeLists[renderLevel].begin());
			m_activeRenderLevel = renderLevel;
			while (itr.isValid())
			{
				itr.getValue()->prepForRender(dt,this);
				itr.getNext();
			}
		}
		//for (XU32 preRenderEvent = 0;preRenderEvent < m_numPreRenderEvents;++preRenderEvent)
		//	m_pPreRenderEvents[preRenderEvent]->prepForRender(dt,this);

		//With the pre prender events processed, we now have our render levels populated
		//with regular render events.  Run through the render levels, sort the contents if necessary
		//and render the scene nodes. Cache a copy of the viewport camera position for the sorting callback
		XMatrix4x4& activeCameraMatrix = m_pViewportCameras[i]->getWorldTransform();
		m_activeCameraPosition.x = activeCameraMatrix._41;
		m_activeCameraPosition.y = activeCameraMatrix._42;
		m_activeCameraPosition.z = activeCameraMatrix._43;

		for (XU32 j = 0;j < m_numRenderLevels;++j)
		{
			//Does this render level require the depth buffer to be set to a sampler?
			if (m_pRenderLevelDescs[j].depthBufferSamplerSlot != -1)
			{
				XVP::GraphicsMgr.setRenderTargetArray(m_renderTargetArray,false);
				XVP::GraphicsMgr.setDepthStencilAsTexture(m_renderTargetArray,m_pRenderLevelDescs[j].depthBufferSamplerSlot);
				XVP::GraphicsMgr.setSamplerState(m_pointClampState,m_pRenderLevelDescs[j].depthBufferSamplerSlot);
			}

			//Do we need to sort this render level?  
			switch (m_pRenderLevelDescs[j].sortType)
			{
			case X_ST_BACK_TO_FRONT:
				qsort(m_pRenderEventLists[j],m_pRenderEventCounts[j],sizeof(XRenderEvent),sortBackToFront);
				break;
			case X_ST_FRONT_TO_BACK:
				qsort(m_pRenderEventLists[j],m_pRenderEventCounts[j],sizeof(XRenderEvent),sortFrontToBack);
				break;
			}

			for (XU32 k = 0;k < m_pRenderEventCounts[j];++k)
			{
	#pragma message("still not binning by material!  Should we when there is no sorting type?? Don't forget todo something here!!!!")
#ifdef XVP_EDITOR
				//If viewport wire frame mode is enabled, use a special material
				if (m_wireFillMode[i])
				{
					//attach the shader controller to our special material
					XMaterial* pWireMat = XVP::GraphicsMgr.getMaterialInterface(m_wireMatHandle);
					XMaterial* pNodeMat = XVP::GraphicsMgr.getMaterialInterface(m_pRenderEventLists[j][k].materialHandle);
					pWireMat->attachShaderController(pNodeMat->getShaderController());
					XVP::GraphicsMgr.applyMaterial(m_wireMatHandle);
				}
				else
				{
					//Check for visualization mode fix up...
					XDUL::fixupRenderEventForVisualization(m_pRenderEventLists[j][k]);
					
					//Check for material override...
					if (m_materialOverride != X_INVALID_HANDLE)
						m_pRenderEventLists[j][k].materialHandle = m_materialOverride;

					XVP::GraphicsMgr.applyMaterial(m_pRenderEventLists[j][k].materialHandle);
				}
				XVP::GraphicsMgr.renderEvent(&m_pRenderEventLists[j][k]);

				//If the scene node of this render event is selected, render it again in
				//a special wireframe mode.  
				XMemBlockList<XSceneNode*>::iterator selItr(m_selectedNodes.begin());
				while(selItr.isValid())
				{
					if (m_pRenderEventLists[j][k].pSceneNode == selItr.getValue())
					{
						XRenderEvent re = m_pRenderEventLists[j][k];
						re.materialHandle = m_selMatHandle;
						XVP::GraphicsMgr.applyMaterial(m_selMatHandle);
						XVP::GraphicsMgr.renderEvent(&re);

						//Render transform gizmo
						XDUL::drawTransform(this,re.pSceneNode->getWorldTransform(),false);

						break;
					}
					selItr.getNext();
				}
#else
				if (m_materialOverride != X_INVALID_HANDLE)
					m_pRenderEventLists[j][k].materialHandle = m_materialOverride;
				XVP::GraphicsMgr.applyMaterial(m_pRenderEventLists[j][k].materialHandle);
				XVP::GraphicsMgr.renderEvent(&m_pRenderEventLists[j][k]);
#endif
			}

			if (m_pRenderLevelDescs[j].depthBufferSamplerSlot != -1)
			{
				XVP::GraphicsMgr.removeTexture(m_pRenderLevelDescs[j].depthBufferSamplerSlot);
				XVP::GraphicsMgr.setRenderTargetArray(m_renderTargetArray);
			}

		}

		for (XU32 j = 0;j < m_numRenderLevels;++j)
			m_pRenderEventCounts[j] = 0;
		
		//Set last RTA to -1 to indicate that the last render target was the scenes's
		//render target
		m_lastRTA[i] = -1;

		//If there's a post render callback registered, call it now
		if (m_pPostRenderCallback)
			m_pPostRenderCallback(this);
	}

	//Render the front sprite array.
	beginSpriteRendering();
	X_DEBUG_LOG_LN("---------- BEGIN FRONT SPRITE ARRAY RENDER ----------",X_DLL_VERBOSE);
	X_START_PROFILE("Front Sprites Render")
	if (m_renderSprites)
	{
		m_updateState = X_US_SPRITES;
		spriteItr.set(m_frontSpriteArray.begin());
		while (spriteItr.isValid())
		{
			spriteItr.getValue().pSprite->update(dt);
			spriteItr.getNext();
		}
		m_updateState = X_US_NORMAL;
		processSpriteCmds(dt);
		m_updateState = X_US_SPRITES;
		spriteItr.set(m_frontSpriteArray.begin());
		while (spriteItr.isValid())
		{
			spriteItr.getValue().pSprite->render();
			spriteItr.getNext();
		}
		m_updateState = X_US_NORMAL;
		processSpriteCmds(dt);
	}

	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END FRONT SPRITE ARRAY RENDER ----------",X_DLL_VERBOSE);

	X_DEBUG_LOG_LN("---------- BEGIN UI ARRAY RENDER ----------",X_DLL_VERBOSE);
	X_START_PROFILE("User Interface Render")
	m_updateState = X_US_USER_INTERFACES;
	
	//Set the first viewport and scissor rect to the device.  For now, UIs can only be rendered
	//into the first viewport of a scene.
	XVP::GraphicsMgr.setViewport(m_viewportArrayDesc.viewports[0]);
	XVP::GraphicsMgr.setScissorRect(XRect(m_viewportArrayDesc.viewports[0].topLeftX,
										  m_viewportArrayDesc.viewports[0].topLeftY,
										  m_viewportArrayDesc.viewports[0].topLeftX + m_viewportArrayDesc.viewports[0].width,
										  m_viewportArrayDesc.viewports[0].topLeftY + m_viewportArrayDesc.viewports[0].height));
	if (m_UIArray.getNumItems() != 0)
		m_UIArray.getBack().pUI->update(dt);
	XMemBlockList<XUIInfo>::iterator m(m_UIArray.begin());
	while (m.isValid())
	{
		m.getValue().pUI->render();
		m.getNext();
	}
	m_updateState = X_US_NORMAL;
	processUICmds();
	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END UI ARRAY RENDER ----------",X_DLL_VERBOSE);

	//Render the cursor for the user interfaces if we have some.
	if (m_UIArray.getNumItems() != 0)
		XUserInterface::renderCursor();
#pragma message("again..can't render the cursor only once because of static calls from mulitple XScenes/... what now??")
	endSpriteRendering();

	//Remove texture overrides if they exist
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		if (m_texOverrides[i].rta != X_INVALID_HANDLE)
			XVP::GraphicsMgr.removeTextureOverride(i);
	}

	//Clear active viewport camera
	m_pActiveCamera = NULL;
	
	m_updateState = X_US_NORMAL;

	//Before we return, if we used our own render target array, set the render context back
	//if (m_renderTargetArray != X_INVALID_HANDLE)
		XVP::GraphicsMgr.setRenderContext(m_renderContext);


	return X_OK;
}



XStatus XScene::renderPostEffects(XF32 dt)
{
	//Resolve render targets if we had our own
	if (m_renderTargetArray != X_INVALID_HANDLE)
		XVP::GraphicsMgr.resolveRenderTargetArray(m_renderTargetArray);

	//If post effects aren't enabled, we're done
	if (!m_enable)
		return X_OK;

	m_updateState = X_US_RENDER;

	XVP::GraphicsMgr.removeAllTextures();

	//If we have our own render target array, let's activate it and render to it.  Otherwise
	//we'll render this scene into it's parent render context.  
	if (m_renderTargetArray != X_INVALID_HANDLE)
		XVP::GraphicsMgr.setRenderTargetArray(m_renderTargetArray);

	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		//If there's no active camera, or post effects are enabled for this
		//viewport, we're done so continue...
		if (!m_pViewportCameras[i] || !m_viewportArrayDesc.enablePostEffects[i])
			continue;

		//Set the active camera and active viewport
		m_pActiveCamera = m_pViewportCameras[i];
		m_activeViewportIndex = i;

		X_ASSERTM(m_viewportRTAs[0][i] != X_INVALID_HANDLE,"Viewport post effect render target does not exist.  Enable post effects for this viewport.");
				
		//Render Post Effects - the first post effect for each viewport will be using the main scene render
		//target as the source texture and the one of the post effect textures for the destination. 
		//This gives the first post effect the ability to convert from the scene's render
		//target format to the format of the back buffer.  All subsequent post effects for a viewport
		//after the first, will ping-pong source and destination textures using only the
		//post effect textures.  After post effects have rendered for a viewport, the last
		//destination texture will be copied into the back buffer.
		XU32 currentRTA = 0;
		XMemBlockList<XPostEffect*> postEffects = m_pViewportCameras[i]->getPostEffects();//(m_viewportPostEffects[i].begin());
		XMemBlockList<XPostEffect*>::iterator postEffectItr(postEffects.begin());

		//No post effects?  We're done.
		if (!postEffectItr.isValid())
			continue;

		//Unbind all textures and render targets so we can ensure that the same
		//resource is not set as both source and destination. Then, calculate some common information
		//and pass it to the post effect so it can do its rendering work
		XVP::GraphicsMgr.removeAllTextures();
		XVP::GraphicsMgr.removeRenderTargetArray();

		XViewport destViewport;
		destViewport.topLeftX = 0;
		destViewport.topLeftY = 0;
		destViewport.width = m_viewportArrayDesc.viewports[i].width;
		destViewport.height = m_viewportArrayDesc.viewports[i].height;
	
		if (postEffectItr.getValue()->isRenderEnabled())
		{
			postEffectItr.getValue()->prepForRender(dt,
													this,
													m_renderTargetArray,
													m_viewportArrayDesc.viewports[i],
													m_viewportRTAs[currentRTA][i],
													destViewport);
			m_lastRTA[i] = currentRTA;
		}

		postEffectItr.getNext();	
		
		//Spin through the rest of the post effects for this viewport
		XViewport srcViewport;
		srcViewport.width = m_viewportArrayDesc.viewports[i].width;
		srcViewport.height = m_viewportArrayDesc.viewports[i].height;
		while (postEffectItr.isValid())
		{
			//Unbind all textures and render targets so we can ensure that the same
			//resource is not set as both source and destination. Then, calculate some common information
			//and pass it to the post effect so it can do its rendering work
			XVP::GraphicsMgr.removeAllTextures();
			XVP::GraphicsMgr.removeRenderTargetArray();
			
			if (postEffectItr.getValue()->isRenderEnabled())
			{
				currentRTA = !currentRTA;
				postEffectItr.getValue()->prepForRender(dt,this,
														m_viewportRTAs[!currentRTA][i],
														srcViewport,
														m_viewportRTAs[currentRTA][i],
														destViewport);
				m_lastRTA[i] = currentRTA;
			}
			postEffectItr.getNext();
		}
	}

	//Clear active viewport camera
	m_pActiveCamera = NULL;

	//Before we return, if we used our own render target array, set the render context back
	XVP::GraphicsMgr.setRenderContext(m_renderContext);

	postRender(dt);

	m_updateState = X_US_NORMAL;
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XScene::postRender(XF32 dt)
{	
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		if (!m_viewportArrayDesc.copyToRenderContext[i])
			continue;

		XRect srcRect(0,0,m_viewportArrayDesc.viewports[i].width,m_viewportArrayDesc.viewports[i].height);
		
		
		//If the final viewport rendering is in the scene's main render target...
		if (m_lastRTA[i] == -1)
		{
			//If we don't really have a render target array then we don't need to copy because we've been rendering
			//in the parent render context all along
			if (m_renderTargetArray != X_INVALID_HANDLE)
				XVP::GraphicsMgr.copyViewportToRenderContext(m_renderTargetArray,
															 0,
															 m_viewportArrayDesc.viewports[i],
															 m_renderContext);
		}
		//else the final viewport rendering is in one of the post effect render targets
		else
		{
			XVP::GraphicsMgr.copyRectToRenderContext(m_viewportRTAs[m_lastRTA[i]][i],
													0,
													srcRect,
													m_renderContext,
													m_viewportArrayDesc.viewports[i].topLeftX,
													m_viewportArrayDesc.viewports[i].topLeftY);
		}
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
/*XStatus XScene::addPreRenderEvent(XSceneNode* pSceneNode)
{
	X_ASSERTM(pSceneNode,"Invalid scene node");
	X_ASSERTM(m_numPreRenderEvents < m_maxPreRenderEvents,"Max number of pre render events exceeded. Increase max pre render events.");
	
	m_pPreRenderEvents[m_numPreRenderEvents] = pSceneNode;
	++m_numPreRenderEvents;

	return X_OK;
}
*/


//*****************************************************************************
/**
Allows a scene node to register itself with a material so it can be
notified when it should render itself.  This allows nodes that share
a common material to be "batched" together at runtime for more efficient
material changes.  

Once a scene node has registered with an XMaterial object, it will receive
a single notification from the material that will signal to the node that
it should now render itself.  Once the scene node finishes rendering itself,
the material object will clear the node's registration.  Therefore, the
node must re-register for each render pass it wants to be rendered in.

@param	pNode	The scene node to register with the material.
@return None.
*/
//*****************************************************************************
XStatus XScene::addRenderEvent(XSceneNode* pSceneNode,void* pEventData,XHandle materialHandle)
{
	X_ASSERTM(pSceneNode,"Invalid scene node cannot be added as render event");
	X_ASSERTM(materialHandle != X_INVALID_HANDLE,"Invalid material handle cannot be used when adding a render event");

	X_DEBUG
	(
		XS8 buf[256];
		sprintf(buf,"Max number of render events exceeded for render level \"%s\".  Current max = %d",m_pRenderLevelDescs[m_activeRenderLevel].name,m_pRenderLevelDescs[m_activeRenderLevel].maxRenderEvents);
		X_ASSERTM(m_pRenderEventCounts[m_activeRenderLevel] < m_pRenderLevelDescs[m_activeRenderLevel].maxRenderEvents,buf);
	)
	
	//Add new render event 
	m_pRenderEventLists[m_activeRenderLevel][m_pRenderEventCounts[m_activeRenderLevel]].pScene = this;
	m_pRenderEventLists[m_activeRenderLevel][m_pRenderEventCounts[m_activeRenderLevel]].pSceneNode = pSceneNode;
	m_pRenderEventLists[m_activeRenderLevel][m_pRenderEventCounts[m_activeRenderLevel]].pEventData = pEventData;
	m_pRenderEventLists[m_activeRenderLevel][m_pRenderEventCounts[m_activeRenderLevel]].materialHandle = materialHandle;
	m_pRenderEventLists[m_activeRenderLevel][m_pRenderEventCounts[m_activeRenderLevel]].renderLevel = m_activeRenderLevel;
	++m_pRenderEventCounts[m_activeRenderLevel];

		//If alpha blending then we need to sort it
		/*if (m_properties & X_MP_ALPHA_BLEND)
			if (XVP::GraphicsMgr.addAlphaRenderEvent(&re) == X_OK)
				return X_OK;*/

	//m_renderEventList.insertBack(re);



	return X_OK;
}


XStatus XScene::setPreRenderCallback(void (*callback)(XScene* pScene))
{
	m_pPreRenderCallback = callback;
	return X_OK;
}



XStatus XScene::setPostRenderCallback(void (*callback)(XScene* pScene))
{
	m_pPostRenderCallback = callback;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XScene::isEnabled()
{
	return m_enable;
}

//*****************************************************************************
//*****************************************************************************
XStatus XScene::enable(XBOOL enable)
{
	m_enable = enable;
	return X_OK;
}

XBOOL XScene::isInitialized()
{
	return m_inited;
}


//*****************************************************************************
//*****************************************************************************
/*XStatus XScene::addAlphaRenderEvent(XRenderEvent* pRenderEvent)
{
	//Is alpha sorting enabled?
	if (!m_alphaSort)
		return X_ERR_NOT_READY;

	//If pRenderEvent is not null
	if (pRenderEvent)
	{	
		if (m_numAlphaRenderEvents < m_maxAlphaRenderEvents)
		{
			m_pAlphaRenderEvents[m_numAlphaRenderEvents].pUserData = pRenderEvent->pUserData;
			m_pAlphaRenderEvents[m_numAlphaRenderEvents].pMaterial = pRenderEvent->pMaterial;
			m_pAlphaRenderEvents[m_numAlphaRenderEvents++].pSceneNode = pRenderEvent->pSceneNode;
			return X_OK;
		}
		else
		{
			X_DEBUG_LOG_LN("Alpha render event list is maxed out.",X_DLL_ERROR);
			return X_ERR_OPERATION_FAILED;
		}

		return X_OK;
	}
	else
		return X_ERR_INVALID_PARAMS;
}
*/


//***************************************************************************** 
/**
Gets the current projection matrix.

@return The current projection matrix.
*/
//***************************************************************************** 
/*XMatrix4x4& XScene::getProjMatrix()
{
	return m_currentProjMatrix;
}*/



//***************************************************************************** 
/**
Gets the current view matrix.

@return The current view matrix.
*/ 
//***************************************************************************** 
/*XMatrix4x4& XScene::getViewMatrix()
{
	return m_currentViewMatrix;
}*/


#if 0
//***************************************************************************** 
/**
Sets the projection matrix.

@param projMatrix	The projection matrix to set.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XScene::setProjMatrix(XMatrix4x4& projMatrix)
{	
	m_currentProjMatrix = projMatrix;
	return X_OK;
}



//***************************************************************************** 
/**
Sets the view matrix.

@param viewMatrix	The view matrix to set.
@return A return code of type XStatus.
*/ 
//***************************************************************************** 
XStatus XScene::setViewMatrix(XMatrix4x4& viewMatrix)
{
	m_currentViewMatrix = viewMatrix;
	return X_OK;
}
#endif


//*****************************************************************************
/**
Enables alpha depth sorting for node's that render with transparency.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::enableAlphaSorting(XBOOL enable)
{
	m_alphaSort = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XScene::isAlphaSortingEnabled()
{
	return m_alphaSort;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::setViewportArray(XViewportArrayDesc& desc)
{
	//If we are in the middle of rendering viewports, we can't allow them to be changed
	if (m_updateState == X_US_RENDER)
		return X_ERR_NOT_READY;

	//Store two copies of the viewport array.  One will always hold the inital dimiensions
	//of the viewports and the other will hold the scaled versions from resize events
	//m_orgViewportArrayDesc = desc;
	m_viewportArrayDesc = desc;
	XRenderTargetArrayDesc* pRTADesc = XVP::GraphicsMgr.getRenderTargetArrayDesc(m_renderTargetArray);
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		m_normalizedViewports[i].left = m_viewportArrayDesc.viewports[i].topLeftX / (XF32)pRTADesc->width;
		m_normalizedViewports[i].top = m_viewportArrayDesc.viewports[i].topLeftY / (XF32)pRTADesc->height;
		m_normalizedViewports[i].right = (m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width) / (XF32)pRTADesc->width;
		m_normalizedViewports[i].bottom = (m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height) / (XF32)pRTADesc->height;
	}

	//Clear viewport cameras
	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
	{
		m_pViewportCameras[i] = NULL;
		//m_viewportPostEffects[i].clear();
	}

	createViewportRenderTargets(desc);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::getViewportArray(XViewportArrayDesc& desc)
{
	desc = m_viewportArrayDesc;
	return X_OK;
}

	

//*****************************************************************************
/**
Sets the XCamera object that X-VP will use when rendering the scene.
If no camera is active, the engine cannot render the scene.  Only one
camera can be active at any give point in time.

@param pCamera	The XCamera object to set active.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::setViewportCamera(XU32 viewportIndex,XCamera* pCamera)
{
	X_ASSERTM(viewportIndex < m_viewportArrayDesc.numViewports,"No such viewport index");
	
	m_pViewportCameras[viewportIndex] = pCamera;
	if (pCamera)
	{
		pCamera->handleViewportResize(m_viewportArrayDesc.viewports[viewportIndex].width,
									  m_viewportArrayDesc.viewports[viewportIndex].height);
	}

	return X_OK;
}



//*****************************************************************************
/**
Gets the active camera.

@return The active XCamera object.
*/
//*****************************************************************************
XCamera* XScene::getViewportCamera(XU32 viewportIndex)
{
	if (viewportIndex < m_viewportArrayDesc.numViewports)
		return m_pViewportCameras[viewportIndex];
	else
		return NULL;
}



XStatus XScene::removeAllViewportCameras()
{
	//Clear viewport cameras
	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
		m_pViewportCameras[i] = NULL;

	return X_OK;
}


//*****************************************************************************
/**
Gets the active camera.

@return The active XCamera object.
*/
//*****************************************************************************
XCamera* XScene::getActiveCamera()
{
	return m_pActiveCamera;
}


XS32 XScene::getActiveViewportIndex()
{
	return m_activeViewportIndex;
}

//*****************************************************************************
//*****************************************************************************
XLightMgr* XScene::getLightMgr()
{
	return &m_lightMgr;
}



//*****************************************************************************
//*****************************************************************************
XHandle XScene::getRenderTargetArray()
{
	return m_renderTargetArray;
}



//*****************************************************************************
//*****************************************************************************
/*XStatus XScene::setRenderContext(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle.");
	m_renderContext = renderContext;
	return X_OK;	
}*/



//*****************************************************************************
//*****************************************************************************
XHandle XScene::getRenderContext()
{
	return m_renderContext;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::setRenderContext(XHandle renderContext)
{
	X_ASSERTM(m_inited,"Scene must be initialized before the render context can be set")
	m_renderContext = renderContext;

	createViewportRenderTargets(m_viewportArrayDesc);

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XScene::handleResizeEvent(XU32 width,XU32 height/*,XF32 widthScale,XF32 heightScale*/)
{
	//m_viewportArrayDesc.viewports[0].width = width;
	//m_viewportArrayDesc.viewports[0].height = height;

	//Are resize events enabled?
	if (!m_enableResizeEvents)
		return X_OK;

	//Resize each viewport based on the normalized viewports.
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		m_viewportArrayDesc.viewports[i].topLeftX = m_normalizedViewports[i].left * width; 
		m_viewportArrayDesc.viewports[i].topLeftY = m_normalizedViewports[i].top * height; 
		m_viewportArrayDesc.viewports[i].width = (m_normalizedViewports[i].right * width) - m_viewportArrayDesc.viewports[i].topLeftX;
		m_viewportArrayDesc.viewports[i].height = (m_normalizedViewports[i].bottom * height) - m_viewportArrayDesc.viewports[i].topLeftY;

		//Width and Height of a viewport must be non 0.  If we detect that one
		//has gone to 0 then force to 1
		if (m_viewportArrayDesc.viewports[i].width == 0)
			m_viewportArrayDesc.viewports[i].width = 1;
		if (m_viewportArrayDesc.viewports[i].height == 0)
			m_viewportArrayDesc.viewports[i].height = 1;
	}
	/*for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		m_viewportArrayDesc.viewports[i].topLeftX = (XU32)((XF32)m_viewportArrayDesc.viewports[i].topLeftX * widthScale);
		m_viewportArrayDesc.viewports[i].topLeftY = (XU32)((XF32)m_viewportArrayDesc.viewports[i].topLeftY * heightScale);
		m_viewportArrayDesc.viewports[i].width = (XU32)((XF32)m_viewportArrayDesc.viewports[i].width * widthScale);
		m_viewportArrayDesc.viewports[i].height = (XU32)((XF32)m_viewportArrayDesc.viewports[i].height * heightScale);
	}*/

	//Send the message to each attached camera
	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
	{
		if (m_pViewportCameras[i])
		{
			m_pViewportCameras[i]->handleViewportResize(m_viewportArrayDesc.viewports[i].width,
														m_viewportArrayDesc.viewports[i].height);
		}
	}

	if (m_renderTargetArray != X_INVALID_HANDLE)
	{
		XRenderTargetArrayDesc newRTADesc = *XVP::GraphicsMgr.getRenderTargetArrayDesc(m_renderTargetArray);
		newRTADesc.width = width;
		newRTADesc.height = height;
		XVP::GraphicsMgr.destroyRenderTargetArray(m_renderTargetArray);
		XVP::GraphicsMgr.createRenderTargetArray(newRTADesc,m_renderTargetArray);
	}

	createViewportRenderTargets(m_viewportArrayDesc);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::enableClears(XBOOL color,XBOOL depth,XBOOL stencil)
{
	m_enableColorClear = color;
	m_enableDepthClear = depth;
	m_enableStencilClear = stencil;
	return X_OK;
}

XU32 XScene::getNumUpdateLevels()
{
	return m_numUpdateLevels;
}


XMemBlockList<XSceneNode*>* XScene::getUpdateLevel(XU32 levelIndex)
{
	X_ASSERTM(levelIndex < m_numUpdateLevels,"No such update level");
	return &m_pUpdateNodeLists[levelIndex];
}

XU32 XScene::getNumRenderLevels()
{
	return m_numRenderLevels;
}
XMemBlockList<XSceneNode*>* XScene::getRenderLevel(XU32 levelIndex)
{
	X_ASSERTM(levelIndex < m_numRenderLevels,"No such render level");
	return &m_pRenderNodeLists[levelIndex];
}
XBOOL XScene::isUpdateLevelEnabled(XU32 updateLevelIndex)
{
	X_ASSERTM(updateLevelIndex < m_numUpdateLevels,"No such update level")
	return &m_pUpdateLevelEnabled[updateLevelIndex];
}
XStatus XScene::enableUpdateLevel(XU32 updateLevelIndex,XBOOL enable)
{
	X_ASSERTM(updateLevelIndex < m_numUpdateLevels,"No such update level")
	m_pUpdateLevelEnabled[updateLevelIndex] = enable;
	return X_OK;
}
XBOOL XScene::isRenderLevelEnabled(XU32 renderLevelIndex)
{
	X_ASSERTM(renderLevelIndex < m_numRenderLevels,"No such render level")
	return m_pRenderLevelEnabled[renderLevelIndex];
}
XStatus XScene::enableRenderLevel(XU32 renderLevelIndex,XBOOL enable)
{
	X_ASSERTM(renderLevelIndex < m_numRenderLevels,"No such render level")
	m_pRenderLevelEnabled[renderLevelIndex] = enable;
	return X_OK;
}

XStatus XScene::setTextureOverrideFromRTA(XU32 samplerIndex,XHandle samplerState,XHandle renderTargetArray,XU32 renderTargetIndex)
{
	X_ASSERT(samplerIndex < X_MAX_SAMPLERS)
	m_texOverrides[samplerIndex].rta = renderTargetArray;
	m_texOverrides[samplerIndex].rtIndex = renderTargetIndex;
	m_texOverrides[samplerIndex].samplerState = samplerState;
	return X_OK;
}

XStatus XScene::removeTextureOverride(XU32 samplerIndex)
{
	X_ASSERT(samplerIndex < X_MAX_SAMPLERS)
	m_texOverrides[samplerIndex].rta = X_INVALID_HANDLE;
	m_texOverrides[samplerIndex].rtIndex = 0;
	m_texOverrides[samplerIndex].samplerState = X_INVALID_HANDLE;
	return X_OK;
}


XStatus XScene::setMaterialOverride(XHandle matHandle)
{
	X_ASSERT(matHandle != X_INVALID_HANDLE);
	m_materialOverride = matHandle;
	return X_OK;
}

XStatus XScene::removeMaterialOverride()
{
	m_materialOverride = X_INVALID_HANDLE;
	return X_OK;
}

XColor& XScene::getFogColor()
{
	return m_fogColor;
}
void XScene::setFogColor(XColor& color)
{
	m_fogColor = color;
}
XF32 XScene::getFogStart()
{
	return m_fogStart;
}
void XScene::setFogStart(XF32 start)
{
	m_fogStart = start;
}
XF32 XScene::getFogEnd()
{
	return m_fogEnd;
}
void XScene::setFogEnd(XF32 end)
{
	m_fogEnd = end;
}
XF32 XScene::getFogDensity()
{
	return m_fogDensity;
}
void XScene::setFogDensity(XF32 density)
{
	m_fogDensity = density;
}
XColor& XScene::getGlobalAmbient()
{
	return m_globalAmbient;
}

void XScene::setGlobalAmbient(XColor& ambient)
{
	m_globalAmbient = ambient;
}

//*****************************************************************************
/**
Addes a node to X-VP's 3D scene graph.  Once the node has been added, it
will be managed by the engine automatically each time the engine is updated.
This includes calling the update() and render() methods of the scene node.

@param pNode	The scene node to add to the 3D scene graph.
@param pParentName	The name of the existing scene node to which this scene node
					will be added as a child.  If this parameter is NULL, the
					scene node will be added to the root node.  If a parent
					name is given but not found in the scene graph, this
					function will fail.
@param pNodeName The name to give this newly added scene node.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::addUpdateNode(XSceneNode* pNode,XU32 updateLevel)
{
	X_ASSERTM(pNode,"null pointer");
	X_ASSERTM(updateLevel < m_numUpdateLevels,"no such update level");

	//Are we adding a light to the scene?  If so, also add it to the light
	//manager.
	if (X_SNT_GET_BASE_SNID(pNode->getType()) == X_SNID_LIGHT)
		m_lightMgr.addLight((XLight*)pNode);

	//Check to make sure we can add the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_updateState == X_US_SCENE_GRAPH)
	{
		XNodeCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.pNode = pNode;
		cmd.level = updateLevel;
		cmd.update = true;
		m_nodeCmdQueue.insertBack(cmd);
		return X_OK;
	}

	//pNode->onAddedToScene(this,true);

	return m_pUpdateNodeLists[updateLevel].insertBack(pNode);
}



//*****************************************************************************
/**
Removes a node from X-VP's 3D scene graph.  Once the node has been removed,
it will no longer be managed by the engine.

@param pNode	The scene node to remove from the 3D scene graph.
@return A return code of type XStatus.
@remark If this node has children, they to will be removed from the scene graph
		and will no longer be managed by the engine.
*/
//*****************************************************************************
XStatus XScene::removeUpdateNode(XSceneNode* pNode)
{
	X_ASSERTM(pNode,"null pointer");

	//Are we removing a light from the scene?  If so, also remove the light
	//from the light manager.
	if (X_SNT_GET_BASE_SNID(pNode->getType()) == X_SNID_LIGHT)
		m_lightMgr.removeLight((XLight*)pNode);

	//Check to make sure we can remove the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_updateState == X_US_SCENE_GRAPH)
	{
		XNodeCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pNode = pNode;
		cmd.update = true;
		m_nodeCmdQueue.insertBack(cmd);
		return X_OK;
	}

	for (XU32 i = 0;i < m_numUpdateLevels;++i)
		m_pUpdateNodeLists[i].remove(pNode);

	//pNode->onRemovedFromScene(this,true);

	return X_OK;
}



XStatus XScene::removeUpdateNode(XSceneNode* pNode,XU32 updateLevel)
{
	X_ASSERTM(pNode,"null pointer");
	X_ASSERTM(updateLevel < m_numUpdateLevels,"no such update level");

	//Are we removing a light from the scene?  If so, also remove the light
	//from the light manager.
	if (X_SNT_GET_BASE_SNID(pNode->getType()) == X_SNID_LIGHT)
		m_lightMgr.removeLight((XLight*)pNode);

	//Check to make sure we can remove the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_updateState == X_US_SCENE_GRAPH)
	{
		XNodeCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pNode = pNode;
		cmd.update = true;
		m_nodeCmdQueue.insertBack(cmd);
		return X_OK;
	}

	//pNode->onRemovedFromScene(this,true);

	return m_pUpdateNodeLists[updateLevel].remove(pNode);
}


//*****************************************************************************
/**
Removes all nodes in X-VP's scene graph.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::removeAllUpdateNodes()
{
	m_lightMgr.removeAllLights();
	if (m_pUpdateNodeLists)
	{
		for (XU32 i = 0;i < m_numUpdateLevels;++i)
		{
			//For each scene node
			/*XMemBlockList<XSceneNode*>::iterator itr(m_pUpdateNodeLists[i].begin());
			while (itr.isValid())
			{
				itr.getValue()->onRemovedFromScene(this,true);
				itr.getNext();
			}*/

			m_pUpdateNodeLists[i].clear();
		}
	}

	return X_OK;
}



XStatus XScene::removeAllUpdateNodes(XU32 updateLevel)
{
	if (m_pUpdateNodeLists)
	{
		XMemBlockList<XSceneNode*>::iterator i(m_pUpdateNodeLists[updateLevel].begin());
		while (i.isValid())
		{
			//Make sure the node is removed from the light manager if it's a light
			if (X_SNT_GET_BASE_SNID(i.getValue()->getType()) == X_SNID_LIGHT)
					m_lightMgr.removeLight((XLight*)i.getValue());
			//i.getValue()->onRemovedFromScene(this,true);
			i.getNext();
		}
			
		//Finally clear the update level
		m_pUpdateNodeLists[updateLevel].clear();
	}
	
	return X_OK;
}

//*****************************************************************************
/**
Determines if a node is in X-VP's 3D scene graph.

@param pNodeName	The name of the node to search for.
@return True if the node is found in the scene graph, false otherwise.
*/
//*****************************************************************************
XBOOL XScene::containsUpdateNode(XSceneNode* pSceneNode)
{
	X_ASSERTM(pSceneNode,"null pointer");

	if (m_pUpdateNodeLists)
	{
		for (XU32 i = 0;i < m_numUpdateLevels;++i)
		{
			XMemBlockList<XSceneNode*>::iterator itr(m_pUpdateNodeLists[i].begin());
			while (itr.isValid())
			{
				if (itr.getValue() == pSceneNode)
					return true;
				itr.getNext();
			}
		}
	}

	return false;
}


XBOOL XScene::containsUpdateNode(XSceneNode* pSceneNode,XU32 updateLevel)
{
	X_ASSERTM(pSceneNode,"null pointer");
	X_ASSERTM(updateLevel <= m_numUpdateLevels,"no such update level");

	if (m_pUpdateNodeLists)
	{
		XMemBlockList<XSceneNode*>::iterator itr(m_pUpdateNodeLists[updateLevel].begin());
		while (itr.isValid())
		{
			if (itr.getValue() == pSceneNode)
				return true;
			itr.getNext();
		}
	}

	return false;
}


//*****************************************************************************
/**
Addes a node to X-VP's 3D scene graph.  Once the node has been added, it
will be managed by the engine automatically each time the engine is updated.
This includes calling the update() and render() methods of the scene node.

@param pNode	The scene node to add to the 3D scene graph.
@param pParentName	The name of the existing scene node to which this scene node
					will be added as a child.  If this parameter is NULL, the
					scene node will be added to the root node.  If a parent
					name is given but not found in the scene graph, this
					function will fail.
@param pNodeName The name to give this newly added scene node.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::addRenderNode(XSceneNode* pNode,XU32 renderLevel)
{
	X_ASSERTM(pNode,"null pointer");
	X_ASSERTM(renderLevel < m_numRenderLevels,"no such render level");

	//Lights aren't added as render nodes...only update nodes
	//X_ASSERTM(X_SNT_GET_BASE_SNID(pNode->getType()) != X_SNID_LIGHT,"Lights can only be added as update nodes to a scene")

	//Check to make sure we can add the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_updateState == X_US_SCENE_GRAPH)
	{
		XNodeCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.pNode = pNode;
		cmd.level = renderLevel;
		cmd.update = false;
		m_nodeCmdQueue.insertBack(cmd);
		return X_OK;
	}

	//pNode->onAddedToScene(this,false);

	return m_pRenderNodeLists[renderLevel].insertBack(pNode);
}



//*****************************************************************************
/**
Removes a node from X-VP's 3D scene graph.  Once the node has been removed,
it will no longer be managed by the engine.

@param pNode	The scene node to remove from the 3D scene graph.
@return A return code of type XStatus.
@remark If this node has children, they to will be removed from the scene graph
		and will no longer be managed by the engine.
*/
//*****************************************************************************
XStatus XScene::removeRenderNode(XSceneNode* pNode)
{
	X_ASSERTM(pNode,"null pointer");

#ifdef XVP_EDITOR
	//If selected, this node needs to be removed from the list
	m_selectedNodes.remove(pNode);
#endif

	//Check to make sure we can remove the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_updateState == X_US_SCENE_GRAPH)
	{
		XNodeCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pNode = pNode;
		cmd.update = false;
		m_nodeCmdQueue.insertBack(cmd);
		return X_OK;
	}

	for (XU32 i = 0;i < m_numRenderLevels;++i)
		m_pRenderNodeLists[i].remove(pNode);

	//pNode->onRemovedFromScene(this,false);

	return X_OK;
}



XStatus XScene::removeRenderNode(XSceneNode* pNode,XU32 renderLevel)
{
	X_ASSERTM(pNode,"null pointer");
	X_ASSERTM(renderLevel < m_numRenderLevels,"no such render level");

	//Are we removing a light from the scene?  If so, also remove the light
	//from the light manager.
	if (X_SNT_GET_BASE_SNID(pNode->getType()) == X_SNID_LIGHT)
		m_lightMgr.removeLight((XLight*)pNode);

	//Check to make sure we can remove the node right now.  If we can't, then
	//queue the request for later processing.
	if (m_updateState == X_US_SCENE_GRAPH)
	{
		XNodeCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pNode = pNode;
		cmd.update = false;
		m_nodeCmdQueue.insertBack(cmd);
		return X_OK;
	}

	//pNode->onRemovedFromScene(this,false);

	return m_pRenderNodeLists[renderLevel].remove(pNode);
}


//*****************************************************************************
/**
Removes all nodes in X-VP's scene graph.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::removeAllRenderNodes()
{
	if (m_pRenderNodeLists)
	{
		for (XU32 i = 0;i < m_numRenderLevels;++i)
		{
			//For each scene node
			/*XMemBlockList<XSceneNode*>::iterator itr(m_pRenderNodeLists[i].begin());
			while (itr.isValid())
			{
				itr.getValue()->onRemovedFromScene(this,false);
				itr.getNext();
			}*/

			m_pRenderNodeLists[i].clear();
		}
	}

	return X_OK;
}



XStatus XScene::removeAllRenderNodes(XU32 renderLevel)
{
	if (m_pRenderNodeLists)
	{
		XMemBlockList<XSceneNode*>::iterator i(m_pRenderNodeLists[renderLevel].begin());
		while (i.isValid())
		{
			//Make sure the node is removed from the light manager if it's a light
			if (X_SNT_GET_BASE_SNID(i.getValue()->getType()) == X_SNID_LIGHT)
					m_lightMgr.removeLight((XLight*)i.getValue());
			//i.getValue()->onRemovedFromScene(this,false);
			i.getNext();
		}
			
		//Finally clear the render level
		m_pRenderNodeLists[renderLevel].clear();
	}
	
	return X_OK;
}

//*****************************************************************************
/**
Determines if a node is in X-VP's 3D scene graph.

@param pNodeName	The name of the node to search for.
@return True if the node is found in the scene graph, false otherwise.
*/
//*****************************************************************************
XBOOL XScene::containsRenderNode(XSceneNode* pSceneNode)
{
	X_ASSERTM(pSceneNode,"null pointer");

	if (m_pRenderNodeLists)
	{
		for (XU32 i = 0;i < m_numRenderLevels;++i)
		{
			XMemBlockList<XSceneNode*>::iterator itr(m_pRenderNodeLists[i].begin());
			while (itr.isValid())
			{
				if (itr.getValue() == pSceneNode)
					return true;
				itr.getNext();
			}
		}
	}

	return false;
}


XBOOL XScene::containsRenderNode(XSceneNode* pSceneNode,XU32 renderLevel)
{
	X_ASSERTM(pSceneNode,"null pointer");
	X_ASSERTM(renderLevel <= m_numRenderLevels,"no such render level");

	if (m_pRenderNodeLists)
	{
		XMemBlockList<XSceneNode*>::iterator itr(m_pRenderNodeLists[renderLevel].begin());
		while (itr.isValid())
		{
			if (itr.getValue() == pSceneNode)
				return true;
			itr.getNext();
		}
	}

	return false;
}


//*****************************************************************************
/**
Gets X-VP's matrix stack capacity.

@return X-VP's matrix stack capacity.
*/
//*****************************************************************************
XU32 XScene::getMatrixStackCapacity()
{
	return m_matrixStack.getCapacity();
}



//*****************************************************************************
//*****************************************************************************
XFreeArray<XMatrix4x4>* XScene::getMatrixStack()
{
	return &m_matrixStack;
}



//*****************************************************************************
/**
Adds a sprite to X-VP's 2D scene graph.  Once the sprite is added,
it will be mangaged by the engine.  Sprites are organized by
Z order in which lower values bring a sprite to the "front" of the scene and
higher values send a sprite to the "back" of the scene.

While Z values can be both negative and positive, they have special meaning
in either case.  If a sprite's Z order is negative, it will be rendered
after the 3D geometry in the 3D scene graph.  On the other hand, if the 
sprite's Z order is positive, it will be rendered before 3D geometry in the
3D scene graph.   This additional complexity allows for placement of sprites
behind or in front of 3D geometry.

@param pSprite	The sprite to add to the 2D scene graph.
@param zOrder	The sprites Z order.  This value can be positive or negative.  If
				its positive, the sprite will be rendered behind 3D geometry.  If
				its negative, the sprite will be rendered in front of 3D
				geometry.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::addSpriteToScene(XBaseSprite* pSprite,int zOrder)
{
	//Make sure that we have a valid sprite
	if (!pSprite)
		return X_ERR_INVALID_PARAMS;

	//Make sure we are clear for adding a new sprite.  That is, if we are
	//currently updating the sprite list, we must queue the request and process
	//it once we are done updating the sprites.
	if (m_updateState == X_US_SPRITES)
	{
		XSpriteCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.info.pSprite = pSprite;
		cmd.info.zOrder = zOrder;
		m_spriteCmdQueue.insertBack(cmd);
		return X_OK;
	}
	
	XMemBlockList<XSpriteInfo>::iterator itr;
	XU32 i = 0;
	if (zOrder < 0)
	{
		//Cycle through the list looking for the correct insertion point
		itr.set(m_frontSpriteArray.begin());
		while (itr.isValid())
		{
			if (zOrder >= itr.getValue().zOrder)
				break;
			itr.getNext();
		}

		XSpriteInfo SI;
		SI.pSprite = pSprite;
		SI.zOrder = zOrder;
		m_frontSpriteArray.insert(itr,SI);

		X_DEBUG_LOG_LN("Added sprite to front list.",X_DLL_INFO);
	}	
	else
	{	
		//Cycle through the list looking for the correct insertion point
		itr.set(m_backSpriteArray.begin());
		while (itr.isValid())
		{
			if (zOrder >= itr.getValue().zOrder)
				break;
			itr.getNext();
		}
		XSpriteInfo SI;
		SI.pSprite = pSprite;
		SI.zOrder = zOrder;
		m_backSpriteArray.insert(itr,SI);

		X_DEBUG_LOG_LN("Added sprite to back list.",X_DLL_INFO);
	}

	return X_OK;
}



//*****************************************************************************
/**
Removes a sprite from X-VP's 2D scene graph.  Once the sprite is removed,
it will no longer be managed by the engine and consequently won't be updated
or rendered.

@param pSprite	The sprite to remove from the 2D scene graph.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::removeSpriteFromScene(XBaseSprite* pSprite)
{
	//Make sure the sprite is valid.
	if (!pSprite)
		return X_ERR_INVALID_PARAMS;

	//Make sure we are clear for removing a sprite.  That is, if we are
	//currently updating the sprite list, we must queue the request and process
	//it once we are done updating the sprites.
	if (m_updateState == X_US_SPRITES)
	{
		XSpriteCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.info.pSprite = pSprite;
		m_spriteCmdQueue.insertBack(cmd);
		return X_OK;
	}

	XSpriteInfo s;
	s.pSprite = pSprite;

	m_frontSpriteArray.remove(s);
	m_backSpriteArray.remove(s);

	X_DEBUG_LOG_LN("Removed sprite from scene.",X_DLL_INFO);
	
	return X_OK;
}

	

//*****************************************************************************
/**
Determines if a sprite is in X-VP's 2D scene graph.

@param pSprite	The sprite to search for.
@return	True if the sprite exists in the 2D scene graph, false otherwise.
*/
//*****************************************************************************
XBOOL XScene::isSpriteInScene(XBaseSprite* pSprite)
{
	//Make sure that we have a valid sprite
	if (!pSprite)
		return false;

	XMemBlockList<XSpriteInfo>::iterator i(m_frontSpriteArray.begin());
	while (i.isValid())
	{
		if (i.getValue().pSprite == pSprite)
			return true;
		i.getNext();
	}
	i.set(m_backSpriteArray.begin());
	while (i.isValid())
	{
		if (i.getValue().pSprite == pSprite)
			return true;
		i.getNext();
	}

	//If we make it here then we did not find the sprite.
	return false;
}



//*****************************************************************************
/**
Sets the Z order of a sprite that is in X-VP's 2D scene graph.

@param pSprite	The sprite for which to set the Z order.
@param zOrder	The new Z order for the sprite.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::setSpriteZOrder(XBaseSprite* pSprite,XU32 zOrder)
{
	//Make sure that we have a valid sprite
	if (!pSprite)
		return X_ERR_INVALID_PARAMS;

	//Remove the sprite first.
	removeSpriteFromScene(pSprite);

	//Add it back with the new z order.
	addSpriteToScene(pSprite,zOrder);

	return X_OK;
}



//*****************************************************************************
/**
Gets the Z order of a sprite that is in X-VP's 2D scene graph.

@param pSprite	The sprite for which to get the Z order.
@param zOrder	A variable to receive the sprite's Z order.
@return	True if the sprite was found in the 2D scene graph, false otherwise.
*/
//*****************************************************************************
XBOOL XScene::getSpriteZOrder(XBaseSprite* pSprite,int& zOrder)
{
	//Make sure that we have a valid sprite
	if (!pSprite)
		return false;
	
	//Cycle through the front list looking for pSprite
	XMemBlockList<XSpriteInfo>::iterator i(m_frontSpriteArray.begin());
	while (i.isValid())
	{
		if (i.getValue().pSprite = pSprite)
		{
			zOrder = i.getValue().zOrder;
			return true;
		}
		i.getNext();
	}

	//Cycle through the back list looking for pSprite
	i.set(m_backSpriteArray.begin());
	while (i.isValid())
	{
		if (i.getValue().pSprite = pSprite)
		{
			zOrder = i.getValue().zOrder;
			return true;
		}
		i.getNext();
	}

	//If we make it here then we could not find the sprite
	//so return false.
	return false;
}



//*****************************************************************************
/**
Enables sprites in X-VP.  By default, sprites are enabled.  When 
sprites are enabled, X-VP will update and render sprites via the 2D
scene graph. 

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::enableSprites()
{
	//Enable sprite rendering.
	m_renderSprites = true;
	X_DEBUG_LOG_LN("Enabling sprites.",X_DLL_INFO);

	return X_OK;
}



//*****************************************************************************
/**
Disables sprites in X-VP.  By default, sprites are enabled.  When 
sprites are disabled, X-VP will not update and render sprites.  

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XScene::disableSprites()
{
	//Disable sprite rendering.
	m_renderSprites = false;
	X_DEBUG_LOG_LN("Disabling sprites.",X_DLL_INFO);

	return X_OK;
}



//*****************************************************************************
/**
This method prepares the engine for rendering 2D sprites.  

@return None.
*/
//*****************************************************************************
void XScene::beginSpriteRendering()
{
	XVP::GraphicsMgr.applyMaterial(m_spriteMaterial);
	/*XU32 numStages = GraphicsMgr.getMaxTextureStages();
	for (XU32 i = 0;i < numStages;i++)
	{
		GraphicsMgr.removeTexture(i);
		


		//Set the states of the stage were going to use for the sprite texture
		if (i == 0)
		{
			XVP::GraphicsMgr.m_pD3DDevice->PSSetSamplers(i,1,&pSpriteSamplerState);

			GraphicsMgr.setAddressU(i,X_TAT_WRAP);
			GraphicsMgr.setAddressV(i,X_TAT_WRAP);
			GraphicsMgr.setAddressW(i,X_TAT_WRAP);
			GraphicsMgr.setMagFilter(i,X_TFT_LINEAR);
			GraphicsMgr.setMinFilter(i,X_TFT_LINEAR);
			GraphicsMgr.setMipFilter(i,X_TFT_LINEAR);
		}
	}

	//Set graphic device states. 
	GraphicsMgr.enableDepthWrite(true);
	GraphicsMgr.enableAlphaTest(false);

	GraphicsMgr.enableAlphaBlend(true);
	GraphicsMgr.setSrcBlend(X_BT_SRC_ALPHA);
	GraphicsMgr.setDestBlend(X_BT_INV_SRC_ALPHA);
	GraphicsMgr.setBlendOp(X_BOT_ADD);

	GraphicsMgr.enableStencilBuffer(false);
			
	GraphicsMgr.setDepthBuffer(X_DBT_NONE);
	
	GraphicsMgr.setCullMode(X_CULLT_CCW);
	GraphicsMgr.setFillMode(X_FMT_SOLID);
*/

#pragma message("Sprite shader needs the current proj matrix...but can't fix until sprite rendering is fixed.")
	//XVP::GraphicsMgr.setVertexShaderConstant(0,(XF32*)&m_currentProjMatrix,4);
	XVP::GraphicsMgr.setVertexShader(m_spriteVertexShader);
	XVP::GraphicsMgr.setPixelShader(m_spritePixelShader);
}



//*****************************************************************************
/**
This method prepares the engine for rendering 2D sprites.  

@return None.
*/
//*****************************************************************************
void XScene::endSpriteRendering()
{
	//GraphicsMgr.setDepthBuffer(X_DBT_Z_BUFFER);
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::addUIToScene(XUserInterface* pUI,int zOrder)
{
	//Make sure that we have a valid UI
	if (!pUI)
		return X_ERR_INVALID_PARAMS;

	//Make sure we are clear for adding a new UI.  That is, if we are
	//currently updating the UI list, we must queue the request and process
	//it once we are done updating the UIs.
	if (m_updateState == X_US_USER_INTERFACES)
	{
		XUICmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.info.pUI = pUI;
		cmd.info.zOrder = zOrder;
		m_UICmdQueue.insertBack(cmd);
		return X_OK;
	}
	
	//Create new UI structure.
	XUIInfo ui;
	ui.pUI = pUI;
	ui.zOrder = zOrder;

	XMemBlockList<XUIInfo>::iterator i(m_UIArray.begin());
		
	//Cycle through the list looking for the correct insertion point
	while (i.isValid())
	{
		if (zOrder >= i.getValue().zOrder)
			break;
		i.getNext();
	}
	m_UIArray.insert(i,ui);

	pUI->notify(X_UILE_ADDED_TO_SCENE);
	X_DEBUG_LOG_LN("Added UI to scene.",X_DLL_INFO);
	
	return X_OK;
}



//*****************************************************************************
/**
Processes all sprite commands on the command queue.

@return None.
*/
//*****************************************************************************
void XScene::processSpriteCmds(XF32 dt)
{
	//Let's process and empty the command queues.  Remember to call the update()
	//for each sprite so that they don't miss a beat.
	while (m_spriteCmdQueue.getNumItems() != 0)
	{
		XSpriteCmd* pCmd = &m_spriteCmdQueue.getFront();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			addSpriteToScene(pCmd->info.pSprite,pCmd->info.zOrder);
			pCmd->info.pSprite->update(dt);
			break;
		case X_CMDT_REMOVE:
			removeSpriteFromScene(pCmd->info.pSprite);
			break;
		}
		m_spriteCmdQueue.removeFront();		
	}
}



//*****************************************************************************
/**
Processes all UI commands on the command queue.

@return None.
*/
//*****************************************************************************
void XScene::processUICmds()
{
	//Let's process and empty the command queues.  Remember to call the update()
	//for each sprite so that they don't miss a beat.
	while (m_UICmdQueue.getNumItems() != 0)
	{
		XUICmd* pCmd = &m_UICmdQueue.getFront();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			addUIToScene(pCmd->info.pUI,pCmd->info.zOrder);
			break;
		case X_CMDT_REMOVE:
			removeUIFromScene(pCmd->info.pUI);
			break;
		}
		m_UICmdQueue.removeFront();		
	}
}



//*****************************************************************************
/**
Processes all node commands on the command queue.

@return None.
*/
//*****************************************************************************
void XScene::processNodeCmds()
{
	//Let's process and empty the command queues.  
	while (m_nodeCmdQueue.getNumItems() != 0)
	{
		XNodeCmd* pCmd = &m_nodeCmdQueue.getFront();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			if (pCmd->update)
				addUpdateNode(pCmd->pNode,pCmd->level);
			else
				addRenderNode(pCmd->pNode,pCmd->level);
			break;
		case X_CMDT_REMOVE:
			if (pCmd->update)
				removeUpdateNode(pCmd->pNode);
			else
				removeRenderNode(pCmd->pNode);
			break;
		}
		m_nodeCmdQueue.removeFront();		
	}
}


//*****************************************************************************
//*****************************************************************************
XStatus XScene::removeUIFromScene(XUserInterface* pUI)
{
	//Make sure the UI is valid.
	if (!pUI)
		return X_ERR_INVALID_PARAMS;

	//Make sure we are clear to remove a UI.  That is, if we are
	//currently updating the UI list, we must queue the request and process
	//it once we are done updating the UIs.
	if (m_updateState == X_US_USER_INTERFACES)
	{
		XUICmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.info.pUI = pUI;
		m_UICmdQueue.insertBack(cmd);
		return X_OK;
	}

	XUIInfo ui;
	ui.pUI = pUI;

	m_UIArray.remove(ui);

	pUI->notify(X_UILE_REM_FROM_SCENE);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XScene::isUIInScene(XUserInterface* pUI)
{
	//Make sure that we have a valid UI
	if (!pUI)
		return false;

	XMemBlockList<XUIInfo>::iterator i(m_UIArray.begin());
	while (i.isValid())
	{
		if (i.getValue().pUI == pUI)
			return true;
		i.getNext();
	}

	//If we make it here then we did not find the sprite.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::setUIZOrder(XUserInterface* pUI,XU32 zOrder)
{
	//Make sure that we have a valid UI
	if (!pUI)
		return X_ERR_INVALID_PARAMS;

	//Remove the UI first.
	removeUIFromScene(pUI);

	//Add it back with the new z order.
	return addUIToScene(pUI,zOrder);
}



//*****************************************************************************
//*****************************************************************************
XU32 XScene::getNumUIs()
{
	return m_UIArray.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XScene::getUIZOrder(XUserInterface* pUI,int& zOrder)
{
	//Make sure that we have a valid UI
	if (!pUI)
		return false;
		
	XMemBlockList<XUIInfo>::iterator i(m_UIArray.begin());

	//Cycle through the list looking for pUI
	while (i.isValid())
	{
		if (pUI == i.getValue().pUI)
		{
			zOrder = i.getValue().zOrder;
			return true;
		}
		i.getNext();
	}

	//If we make it here then we could not find the UI
	//so return false.
	return false;
}



int XScene::sortBackToFront(const void* pNode1,const void* pNode2)
{
	XRenderEvent* pRE = (XRenderEvent*)pNode1;
	XRenderEvent* pRE2 = (XRenderEvent*)pNode1;
	if (m_activeCameraPosition.distanceSquared(pRE->pSceneNode->getBoundingSphere().center) >
		m_activeCameraPosition.distanceSquared(pRE2->pSceneNode->getBoundingSphere().center))
        return -1;
	else
		return 1;
	/*if (m_activeCameraPosition.distanceSquared((*(XSceneNode**)(pNode1))->getBoundingSphere().center) >=
		m_activeCameraPosition.distanceSquared((*(XSceneNode**)(pNode2))->getBoundingSphere().center))
        return -1;
	else
		return 1;*/
}


int XScene::sortFrontToBack(const void* pNode1,const void* pNode2)
{
	XRenderEvent* pRE = (XRenderEvent*)pNode1;
	XRenderEvent* pRE2 = (XRenderEvent*)pNode1;
	if (m_activeCameraPosition.distanceSquared(pRE->pSceneNode->getBoundingSphere().center) <
		m_activeCameraPosition.distanceSquared(pRE2->pSceneNode->getBoundingSphere().center))
        return -1;
	else
		return 1;
	/*if (m_activeCameraPosition.distanceSquared((*(XSceneNode**)(pNode1))->getBoundingSphere().center) >=
		m_activeCameraPosition.distanceSquared((*(XSceneNode**)(pNode2))->getBoundingSphere().center))
        return -1;
	else
		return 1;*/
}


//*****************************************************************************
/**
Gets the number of sprites in X-VP's 2D scene graph.

@return The number of sprites in the 2D scene graph.
*/
//*****************************************************************************
XU32 XScene::getNumSprites()
{
	return m_frontSpriteArray.getNumItems() + m_backSpriteArray.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
/*XSceneGraphNode* XScene::onLevelDown(XSceneGraphNode* pNode,void* pUserData)
{
	((XScene*)pUserData)->addPreRenderEvent(pNode->m_pSceneNode);
	return pNode;
}
*/

XMemBlockList<XSceneNode*>* XScene::getSelectedNodes()
{
	return &m_selectedNodes;
}



//*****************************************************************************
//*****************************************************************************
XStatus XScene::pickSceneNodes(XMemBlockList<HitInfo>& hits,XS32 screenX,
							   XS32 screenY)
{
	//Find which viewport we're dealing with
	XS32 viewportIndex = -1;
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		if ((screenX >= m_viewportArrayDesc.viewports[i].topLeftX) && 
			(screenX <= (m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width)) &&
			(screenY >= m_viewportArrayDesc.viewports[i].topLeftY) &&
			(screenY <= (m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height)))
		{
			viewportIndex = i;
			break;
		}
	}

	//If we didn't intersect one of the scene's viewports, we're done.
	if (viewportIndex == -1)
		return X_ERR_NO_SUCH_OBJECT;

	//Now that we know which viewport to use for the pick, let's calculate a pick ray
	XS32 vpX = screenX - m_viewportArrayDesc.viewports[viewportIndex].topLeftX;
	XS32 vpY = screenY - m_viewportArrayDesc.viewports[viewportIndex].topLeftY;
	XCamera* pCamera = m_pViewportCameras[viewportIndex];
	if (pCamera)
	{
		XRay3D pickRay = XICreatePickRay(vpX,vpY,
										 pCamera->getViewportWidth(),
										 pCamera->getViewportHeight(),
										 pCamera->getViewMatrix(),
										 pCamera->getProjMatrix());

		//Run through all the scene nodes and test for collisions with
		//the pick ray subject to the selection filter
		XMemBlockList<XSceneNode*>::iterator itr;

		/*if (m_selectionFilter == X_SF_ACTOR)
		{
			for(XU32 updateLevel = 0;updateLevel < m_numUpdateLevels;++updateLevel)
			{
				itr.set(m_pUpdateNodeLists[updateLevel].begin());
				while (itr.isValid() && strcmp("XActorContainer",itr.getValue()->getTypeDesc()) == 0)
				{
					itr.getValue()->pick(pickRay,hits);			
					itr.getNext();			
				}
			}		
		}
		else
		{*/
			for(XU32 updateLevel = 0;updateLevel < m_numUpdateLevels;++updateLevel)
			{
				itr.set(m_pUpdateNodeLists[updateLevel].begin());
				while (itr.isValid())
				{
					itr.getValue()->pick(pickRay,hits);			
					itr.getNext();			
				}
			}		
		//}
	}

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XScene::selectSceneNodes(XS32 screenX,XS32 screenY,XBOOL multiple)
{
	//Find which viewport we're dealing with
	XS32 viewportIndex = -1;
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		if ((screenX >= m_viewportArrayDesc.viewports[i].topLeftX) && 
			(screenX <= (m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width)) &&
			(screenY >= m_viewportArrayDesc.viewports[i].topLeftY) &&
			(screenY <= (m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height)))
		{
			viewportIndex = i;
			break;
		}
	}

	//If we didn't intersect one of the scene's viewports, we're done.
	if (viewportIndex == -1)
		return X_ERR_NO_SUCH_OBJECT;

	XCamera* pCamera = m_pViewportCameras[viewportIndex];
	X_ASSERTM(pCamera,"Viewport has no camera associated with it.  Can't select scene nodes");

	//First, we pick gizmo nodes only.  They get priority over regular scene nodes.
	//If we find a gizmo node was picked then ignore don't perform a scene node selection
	//and route this selection to the gizmo manager.  If we dont' find a gizmo, then
	//continue scene node selection.  Gizmos are not subject to the selection filter.
	XSceneNode* pGizmoNode = selectGizmoNode(screenX,screenY);
	if (pGizmoNode)
	{
		//We found a gizmo node, notify it 
		return m_gizmoMgr.onMouseDown(pGizmoNode,screenX,screenY);
	}
	
	//Pick all the scene nodes given the screen location
	XMemBlockList<HitInfo> hits;
	pickSceneNodes(hits,screenX,screenY);
	
	//Clear previously selected nodes only if we aren't in multiple selection mode
	if (!multiple)
		clearSelection();

	//Now prune the hit list to only include the closest hit
	XF32 smallestDist = FLT_MAX;
	HitInfo closestHit;
	closestHit.pSceneNode = NULL;
	XMemBlockList<HitInfo>::iterator i(hits.begin());
	XVector3D pos,pos2;
	pCamera->getWorldPosition(pos);
	while (i.isValid())
	{	
		pos2 = i.getValue().intersectionPoint;
		XF32 d = pos.distance(pos2);
		if (d < smallestDist)
		{
			smallestDist = d;
			closestHit = i.getValue();
		}
		//hits2.insertBack(i.getValue());
		i.getNext();
	}
	
	if (closestHit.pSceneNode)
	{
		//If there is an actor container, select it instead
		if (closestHit.pSceneNode->getActorContainer())
		{
			closestHit.pSceneNode = (XSceneNode*)closestHit.pSceneNode->getActorContainer();
		}	

		hits.clear();
		hits.insertBack(closestHit);
	}
	
	//If we are in mulitple selection mode, add new hits but remove
	//hits that already exist in the list.
	if (multiple)
	{
		XMemBlockList<HitInfo>::iterator i(hits.begin());
		while (i.isValid())
		{
			XBOOL foundNode = false;
			XMemBlockList<XSceneNode*>::iterator j(m_selectedNodes.begin());
			while (j.isValid())
			{
				if (i.getValue().pSceneNode == j.getValue())
				{
					foundNode = true;
					break;
				}
				j.getNext();
			}

			//If the node was not already in the selection list, add it
			if (!foundNode)
				m_selectedNodes.insertBack(i.getValue().pSceneNode);
			else
				m_selectedNodes.remove(i.getValue().pSceneNode);

			i.getNext();
		}

	}
	else
	{
		//If we have hits, add them all, otherwise we aren't in multiple selection
		//mode so clear all selections
		if (hits.getNumItems() != 0)
		{
			XMemBlockList<HitInfo>::iterator i(hits.begin());
			while (i.isValid())
			{
				m_selectedNodes.insertBack(i.getValue().pSceneNode);
				i.getNext();
			}
		}
		else
			clearSelection();
	}

	hits.clear();

#ifdef XVP_EDITOR
	//Notify gizmo manager about the selection change
	m_gizmoMgr.onSelectionChanged();
#endif


	return X_OK;
}



XStatus XScene::clearSelection()
{
	m_selectedNodes.clear();
	return X_OK;
}



XStatus XScene::invertSelection()
{
	//Gather all scene nodes
	XMemBlockList<XSceneNode*> allNodes;
	XMemBlockList<XSceneNode*>::iterator itr;
	for(XU32 updateLevel = 0;updateLevel < m_numUpdateLevels;++updateLevel)
	{
		itr.set(m_pUpdateNodeLists[updateLevel].begin());
		while (itr.isValid())
		{
			itr.getValue()->pickAll(allNodes);
			itr.getNext();
		}
	}

	//Run through all the scene nodes and if they aren't in the selected node list,
	//add them, if they are, remove them.
	itr.set(allNodes.begin());
	while (itr.isValid())
	{
		if (m_selectedNodes.contains(itr.getValue()))
			m_selectedNodes.remove(itr.getValue());
		else
		{
			//Must have a non-zero bounding sphere radius
			if (itr.getValue()->getBoundingSphere().radius > 0)
				m_selectedNodes.insertBack(itr.getValue());
		}

		itr.getNext();
	}
	allNodes.clear();

	return X_OK;
}


XStatus XScene::selectAll()
{
	//First, clear all selected nodes and the add all nodes back to the list
	clearSelection();

	XMemBlockList<XSceneNode*>::iterator itr;
	for(XU32 updateLevel = 0;updateLevel < m_numUpdateLevels;++updateLevel)
	{
		itr.set(m_pUpdateNodeLists[updateLevel].begin());
		while (itr.isValid())
		{
			//Must have a non-zero bounding sphere radius
			itr.getValue()->pickAll(m_selectedNodes);
			itr.getNext();
		}
	}

	return X_OK;
}


XStatus XScene::setSelectionFilter(X_SELECTION_FILTER filter)
{
	m_selectionFilter = filter;
	return X_OK;
}

XStatus XScene::addGizmoNode(XSceneNode* pNode)
{
	m_gizmoList.insertBack(pNode);
	XU32 lastRenderLevel = getNumRenderLevels() - 1;
	addRenderNode(pNode,lastRenderLevel);
	return X_OK;
}

XStatus XScene::removeGizmoNode(XSceneNode* pNode)
{
	m_gizmoList.remove(pNode);
	removeRenderNode(pNode);
	return X_OK;
}

XStatus XScene::setGizmoMode(X_GIZMO_MODE mode)
{
	return m_gizmoMgr.setMode(mode);
}


XStatus XScene::updateGizmoHover(XS32 x,XS32 y)
{
	return m_gizmoMgr.updateHover(x,y);
}

XStatus XScene::updateGizmoDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY)
{
	return m_gizmoMgr.updateDrag(startX,startY,endX,endY);
}

XStatus XScene::gizmoOnMouseUp()
{
	return m_gizmoMgr.onMouseUp();
}

//*****************************************************************************
//*****************************************************************************
XSceneNode* XScene::selectGizmoNode(XS32 screenX,XS32 screenY)
{
	//Find which viewport we're dealing with
	XS32 viewportIndex = -1;
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		if ((screenX >= m_viewportArrayDesc.viewports[i].topLeftX) && 
			(screenX <= (m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width)) &&
			(screenY >= m_viewportArrayDesc.viewports[i].topLeftY) &&
			(screenY <= (m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height)))
		{
			viewportIndex = i;
			break;
		}
	}

	//If we didn't intersect one of the scene's viewports, we're done.
	if (viewportIndex == -1)
		return NULL;

	XCamera* pCamera = m_pViewportCameras[viewportIndex];
	X_ASSERTM(pCamera,"Viewport has no camera associated with it.  Can't select gizmo node");

	//Pick all the scene nodes given the screen location
	XMemBlockList<HitInfo> hits;
	//hits2.clear();
	pickGizmoNodes(hits,screenX,screenY);
	
	//Now prune the hit list to only include the closest hit.  Also, we are
	//only interested in gizmos that have that have a matching viewport mask
	XF32 smallestDist = FLT_MAX;
	HitInfo closestHit;
	closestHit.pSceneNode = NULL;
	XMemBlockList<HitInfo>::iterator i(hits.begin());
	XVector3D pos,pos2;
	pCamera->getWorldPosition(pos);
	while (i.isValid())
	{
		pos2 = i.getValue().intersectionPoint;
		XF32 d = pos.distance(pos2);
		if (d < smallestDist && (i.getValue().pSceneNode->isViewportEnabled(viewportIndex)))
		{
			smallestDist = d;
			closestHit = i.getValue();
		}
		i.getNext();
	}
	
	hits.clear();

	return closestHit.pSceneNode;
}


//*****************************************************************************
//*****************************************************************************
XStatus XScene::pickGizmoNodes(XMemBlockList<HitInfo>& hits,XS32 screenX,
							   XS32 screenY)
{
	//Find which viewport we're dealing with
	XS32 viewportIndex = -1;
	for (XU32 i = 0;i < m_viewportArrayDesc.numViewports;++i)
	{
		if ((screenX >= m_viewportArrayDesc.viewports[i].topLeftX) && 
			(screenX <= (m_viewportArrayDesc.viewports[i].topLeftX + m_viewportArrayDesc.viewports[i].width)) &&
			(screenY >= m_viewportArrayDesc.viewports[i].topLeftY) &&
			(screenY <= (m_viewportArrayDesc.viewports[i].topLeftY + m_viewportArrayDesc.viewports[i].height)))
		{
			viewportIndex = i;
			break;
		}
	}

	//If we didn't intersect one of the scene's viewports, we're done.
	if (viewportIndex == -1)
		return X_ERR_NO_SUCH_OBJECT;

	//Now that we know which viewport to use for the pick, let's calculate a pick ray
	XS32 vpX = screenX - m_viewportArrayDesc.viewports[viewportIndex].topLeftX;
	XS32 vpY = screenY - m_viewportArrayDesc.viewports[viewportIndex].topLeftY;
	XCamera* pCamera = m_pViewportCameras[viewportIndex];
	if (pCamera)
	{
		XRay3D pickRay = XICreatePickRay(vpX,vpY,
										 pCamera->getViewportWidth(),
										 pCamera->getViewportHeight(),
										 pCamera->getViewMatrix(),
										 pCamera->getProjMatrix());
		
		//Run through all the gizmo nodes and test for collisions with
		//the pick ray 
		XMemBlockList<XSceneNode*>::iterator itr;
		
		//Collect hits from this ray. 
		itr.set(m_gizmoList.begin());
		while (itr.isValid())
		{
			itr.getValue()->pick(pickRay,hits);			
			itr.getNext();			
		}	
	}


	return X_OK;
}

XGizmoMgr* XScene::getGizmoMgr()
{
	return &m_gizmoMgr;
}

//*****************************************************************************
//*****************************************************************************
void XScene::createViewportRenderTargets(XViewportArrayDesc& desc)
{
	//First deallocate any current render targets
	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
	{
		if (m_viewportRTAs[0][i] != X_INVALID_HANDLE)
			XVP::GraphicsMgr.destroyRenderTargetArray(m_viewportRTAs[0][i]);
		if (m_viewportRTAs[1][i] != X_INVALID_HANDLE)
			XVP::GraphicsMgr.destroyRenderTargetArray(m_viewportRTAs[1][i]);
		m_viewportRTAs[0][i] = X_INVALID_HANDLE;
		m_viewportRTAs[1][i] = X_INVALID_HANDLE;
	}

	//Now, allocate post effect render targets if post effects are enabled for viewports
	for (XU32 i = 0;i < X_MAX_VIEWPORTS;++i)
	{
		if (desc.enablePostEffects[i])
		{
			//Create a render target the same dimensions as the viewport using
			//the format of the render context
			XRenderTargetArrayDesc rtaDesc;
			rtaDesc.numRenderTargets = 1;
			rtaDesc.width = desc.viewports[i].width;
			rtaDesc.height = desc.viewports[i].height;

			rtaDesc.formats[0] = XVP::GraphicsMgr.getRenderContextDesc(m_renderContext)->format;

			rtaDesc.useDepthStencil = false;

			XVP::GraphicsMgr.createRenderTargetArray(rtaDesc,m_viewportRTAs[0][i]);
			XVP::GraphicsMgr.createRenderTargetArray(rtaDesc,m_viewportRTAs[1][i]);
		}
	}
}


XHandle XScene::getSelectionMaterial()
{
	return m_selMatHandle;
}

XMaterialColorSC* XScene::getSelectionMaterialSC()
{
	return &m_selectionSC;
}

XStatus XScene::enableWireShading(XU32 viewportIndex,XBOOL enableWire)
{
	X_ASSERTM(viewportIndex < m_viewportArrayDesc.numViewports,"No such viewport index");
	m_wireFillMode[viewportIndex] = enableWire;
	return X_OK;
}