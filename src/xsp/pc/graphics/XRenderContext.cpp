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
//	Module Name:	XRenderContext.cpp
//	Author:			Lynn Duke
//	Creation Date:	3-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

XScene					XRenderContext::m_debugScene;

//*****************************************************************************
//*****************************************************************************
XRenderContext::XRenderContext()
{
	m_pSwapChain = NULL;
	m_pRenderTargetView = NULL;
	m_pDepthStencilView = NULL;
	m_pDepthStencil = NULL;
	m_pBackBufferTexture = NULL;

	//Set default clear values for depth and stencil.
	m_depthClearValue = 1.0;
	m_stencilClearValue = 0;

	m_orgWidth = 0;
	m_orgHeight = 0;
	m_isFullScreenMode = false;

	m_inited = false;

 	m_updateState = X_US_NORMAL;
	m_resizing = false;

#ifndef XVP_FINAL
	m_ownsDebugScene = false;
#endif
}



//*****************************************************************************
//*****************************************************************************
XRenderContext::~XRenderContext()
{
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderContext::init(XRenderContextDesc& desc)
{
	//Store the description
	m_desc = desc;

	//Are we going fullscreen?  If so, call the user supplied callback to select an
	//initial fullscreen display mode.  Otherwise, use the user supplied window
	//width and height to create a window
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,sizeof(sd));
	/*if (!windowed)
	{
		//If no callback was supplied to choose a display mode, choose a default mode
		XU32 chosenModeIndex = 0;
		if (initDesc.pFullScreenModeCallback)
			chosenModeIndex = initDesc.pFullScreenModeCallback(m_pFullScreenModes,m_numFullScreenModes);
		X_ASSERTM(chosenModeIndex < m_numFullScreenModes,"Display mode index returned from display mode callback is out of range.");

		sd.BufferCount = 1;
		sd.BufferDesc.Width = pDescs[chosenModeIndex].Width;
		sd.BufferDesc.Height = pDescs[chosenModeIndex].Height;
		sd.BufferDesc.Format = pDescs[chosenModeIndex].Format;
		sd.BufferDesc.RefreshRate.Numerator = pDescs[chosenModeIndex].RefreshRate.Numerator;
		sd.BufferDesc.RefreshRate.Denominator = pDescs[chosenModeIndex].RefreshRate.Denominator;
		sd.BufferDesc.ScanlineOrdering = pDescs[chosenModeIndex].ScanlineOrdering;
		sd.BufferDesc.Scaling = pDescs[chosenModeIndex].Scaling;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_windowHandle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = false;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		m_screenWidth = pDescs[chosenModeIndex].Width;
		m_screenHeight = pDescs[chosenModeIndex].Height;
		m_isFullScreenMode = true;
	}
	else*/
	{
		sd.BufferCount = 1;
		sd.BufferDesc.Width = desc.width;
		sd.BufferDesc.Height = desc.height;
		sd.BufferDesc.Format = XVP::GraphicsMgr.translateDataFormatType(desc.format);
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = (HWND)m_desc.pWindowHandle;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		m_orgWidth = desc.width;
		m_orgHeight = desc.height;
		m_isFullScreenMode = false;
	}

	HRESULT hr = XVP::GraphicsMgr.m_pDXGIFactory->CreateSwapChain(XVP::GraphicsMgr.m_pD3DDevice,&sd,&m_pSwapChain);
	X_ASSERTM(hr == S_OK,"Could not create swap chain for render context.");

	createSwapChainBuffers(desc.width,desc.height);

	m_updateState = X_US_NORMAL;
	m_inited = true;

#ifndef XVP_FINAL
	//If this render context has the debug flag specified, create our debug
	//scene and specify this render context to receive the debug output.  After
	//we create the debug scene, we never recreate it again.
	if (desc.createDebugScene)
	{
		//Only one render context can contain the debug scene
		X_ASSERTM(!m_debugScene.isInitialized(),"Only one render context can contain the debug scene.");

		//Create debug scene
		XSceneDesc debugDesc;
		strcpy(debugDesc.name,"Debug Scene");
		debugDesc.enableResizeEvents = true;
		debugDesc.renderTargetArrayDesc.numRenderTargets = 0;
		debugDesc.viewportArrayDesc.numViewports = 1;
		debugDesc.viewportArrayDesc.viewports[0].topLeftX = 0;
		debugDesc.viewportArrayDesc.viewports[0].topLeftY = 0;
		debugDesc.viewportArrayDesc.viewports[0].width = desc.width;
		debugDesc.viewportArrayDesc.viewports[0].height = desc.height;
		debugDesc.viewportArrayDesc.enablePostEffects[0] = false;
		debugDesc.numUpdateLevels = 0;
		debugDesc.numRenderLevels = 0;
		m_debugScene.init(debugDesc);
		m_ownsDebugScene = true;
	}
#endif

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderContext::destroy()
{
#ifndef XVP_FINAL
	if (m_debugScene.isInitialized() && m_ownsDebugScene)
		m_debugScene.destroy();
#endif
	//Release directx objects
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pBackBufferTexture);

	//DirectX 10 won't allow us to destroy the swap chain while in fullscreen mode
	m_resizing = true;
	m_pSwapChain->SetFullscreenState(false,NULL);
	m_resizing = false;
	SAFE_RELEASE(m_pSwapChain);

	for (XU32 i = 0;i < MAX_LAYERS;++i)
		m_scenes[i].clear();
	m_sceneCmdQueue.clear();

	m_inited = false;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderContext::update(XF32 dt)
{
	//Update scenes
	XMemBlockList<XScene*>::iterator sceneItr;
	processSceneCmds(dt);
	m_updateState = X_US_SCENES;

	for (XU32 i = 0;i < MAX_LAYERS;++i)
	{
		sceneItr.set(m_scenes[i].begin());
		while (sceneItr.isValid())
		{
			sceneItr.getValue()->update(dt);
			sceneItr.getNext();
		}
	}
	m_updateState = X_US_NORMAL;

#ifndef XVP_FINAL
	if (m_ownsDebugScene && m_debugScene.isInitialized())
		m_debugScene.update(dt);
#endif
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderContext::render(XF32 dt)
{
	//First, clear render context if clearing is enabled
	clear(true,true,true);

	//Render scenes
	setToDevice();

	m_updateState = X_US_SCENES;
	for (XU32 i = 0;i < MAX_LAYERS;++i)
	{
		XMemBlockList<XScene*>::iterator sceneItr(m_scenes[i].begin());
		while (sceneItr.isValid())
		{
			sceneItr.getValue()->render(dt);
			sceneItr.getNext();
		}
	
		//Render post effects for each scene
		setToDevice();
		sceneItr.set(m_scenes[i].begin());
		while (sceneItr.isValid())
		{
			sceneItr.getValue()->renderPostEffects(dt);
			sceneItr.getNext();
		}
	}

#ifndef XVP_FINAL
	if (m_ownsDebugScene && m_debugScene.isInitialized())
		m_debugScene.render(dt);
#endif
	m_updateState = X_US_NORMAL;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderContext::clear(XBOOL color,XBOOL depth,XBOOL stencil)
{
	if (color)
	{
		XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView,
															(XF32*)&m_desc.clearColor);
	}
	
	if (depth && m_pDepthStencilView)
		XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView,
															D3D10_CLEAR_DEPTH,
															m_depthClearValue,
															m_stencilClearValue);

	if (stencil && m_pDepthStencilView)
		XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView,
															D3D10_CLEAR_STENCIL,
															m_depthClearValue,
															m_stencilClearValue);

	return X_OK;
}



//***************************************************************************** 
/**
Notifies the graphics driver to present the rendered scene to the screen.  This
function is called by X-VP once all graphics are rendered to the screen
and allows the driver to do an necessary processing to update the screen.

@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XRenderContext::flip()
{
	if (m_pSwapChain->Present(NULL,NULL) != S_OK)
	{
		return X_ERR_NOT_READY;
	}
	else
		return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void* XRenderContext::getWindowHandle()
{
	return m_desc.pWindowHandle;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XRenderContext::isFullScreenMode()
{
	return m_isFullScreenMode;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::setWindowedMode()
{
	X_ASSERTM(m_pSwapChain,"Swap chain is null");
	X_DEBUG_LOG_LN("Switching to windowed mode",X_DLL_INFO);
	
	if (m_resizing)
	{
		X_DEBUG_LOG_LN("Ignoring setWindowedMode() call on render context because a resize event is already in progress",X_DLL_WARNING);
		return X_ERR_NOT_READY;
	}

	//Switch to windowed mode
	m_resizing = true;
	HRESULT hr = m_pSwapChain->SetFullscreenState(false,NULL);
	X_ASSERTM(hr == S_OK,"Could not switch to windowed mode");
	m_resizing = false;
	m_isFullScreenMode = false;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::setFullScreenMode(XFullScreenModeDesc& desc)
{
	X_ASSERTM(m_pSwapChain,"Swap chain is null");
	X_DEBUG
	(
		XS8 buf[512];
		sprintf(buf,"Switching to fullscreen mode - Resolution(%d x %d) Refresh(%d)",desc.width,
																				desc.height,
																				desc.refreshRate);
		if (desc.scanlineOrdering == X_SCANLINE_ORDER_PROGRESSIVE)
			strcat(buf," Scaline(Progressive)");
		if (desc.scanlineOrdering == X_SCANLINE_ORDER_UPPER_FIELD_FIRST)
			strcat(buf," Scaline(Upper First)");
		if (desc.scanlineOrdering == X_SCANLINE_ORDER_LOWER_FIELD_FIRST)
			strcat(buf," Scaline(Lower First)");
		if (desc.scalingMode == X_SCALING_CENTERED)
			strcat(buf," Scaling(Centered)");
		if (desc.scalingMode == X_SCALING_STRETCHED)
			strcat(buf," Scaling(Stretched)");

		X_DEBUG_LOG_LN(buf,X_DLL_INFO);
	)
	
	if (m_resizing)
	{
		X_DEBUG_LOG_LN("Ignoring setFullScreenMode() call on render context because a resize event is already in progress",X_DLL_WARNING);
		return X_ERR_NOT_READY;
	}

	//Switch to fullscreen mode
	m_resizing = true;
	HRESULT hr = m_pSwapChain->SetFullscreenState(true,NULL);
	X_ASSERTM(hr == S_OK,"Could not switch to fullscreen mode");

	//Resize swap chain
	DXGI_MODE_DESC modeDesc;
	modeDesc.Width = desc.width;
	modeDesc.Height = desc.height;
	modeDesc.Format = XGraphicsMgr::translateDataFormatType(desc.format);
	modeDesc.RefreshRate.Numerator = desc.refreshRate;
	modeDesc.RefreshRate.Denominator = 1;
	modeDesc.Scaling = XGraphicsMgr::translateScalingMode(desc.scalingMode);
	modeDesc.ScanlineOrdering = XGraphicsMgr::translateScanlineOrder(desc.scanlineOrdering);
	
	hr = m_pSwapChain->ResizeTarget(&modeDesc);
	X_ASSERTM(hr == S_OK,"Could not resize target for swap chain.");
	
	recreateSwapChainBuffers(modeDesc.Width,modeDesc.Height);

	m_isFullScreenMode = true;
	m_resizing = false;

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::resizeWindow(XU32 width,XU32 height)
{
	//We can only resize if we have a valid swap chain and the manager hasn't
	//been destroyed.  This will probably be called in response to Windows
	//resize messages and we may not yet be initialized.
	if (!m_pSwapChain || !m_inited)
		return X_ERR_NOT_READY;

	if (m_resizing)
	{
		X_DEBUG_LOG_LN("Ignoring resizeWindow() call on render context because a resize event is already in progress",X_DLL_WARNING);
		return X_ERR_NOT_READY;
	}

	m_resizing = true;

	X_DEBUG
	(
		XS8 buf[128];
		sprintf(buf,"Resizing window - Resolution(%d x %d)",width,height);
		X_DEBUG_LOG_LN(buf,X_DLL_INFO);
	)
	
	recreateSwapChainBuffers(width,height);

	m_resizing = false;

	return X_OK;
}


XStatus XRenderContext::createSwapChainBuffers(XU32 width,XU32 height)
{
	X_ASSERTM(width != 0,"Width of render context cannot be 0");
	X_ASSERTM(height != 0,"Height of render context cannot be 0");

	//Get the back buffer
    HRESULT hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&m_pBackBufferTexture );
	X_ASSERTM(hr == S_OK,"Could not get swap chain buffer.")
    
	//Create render target view
	D3D10_TEXTURE2D_DESC BBDesc;
    m_pBackBufferTexture->GetDesc( &BBDesc );
	D3D10_RENDER_TARGET_VIEW_DESC RTVDesc;
    RTVDesc.Format = BBDesc.Format;
    RTVDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
    RTVDesc.Texture2D.MipSlice = 0;	
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateRenderTargetView( m_pBackBufferTexture, &RTVDesc, &m_pRenderTargetView );
    X_ASSERTM(hr == S_OK,"Could not create render target view.")

	//Create depth stencil texture
    D3D10_TEXTURE2D_DESC descDepth;
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;//DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D10_USAGE_DEFAULT;
    descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = XVP::GraphicsMgr.m_pD3DDevice->CreateTexture2D(&descDepth,NULL,&m_pDepthStencil);
    X_ASSERTM(hr == S_OK,"Could not create depth stencil texture.")

    //Create the depth stencil view
    D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = XVP::GraphicsMgr.m_pD3DDevice->CreateDepthStencilView(m_pDepthStencil,&descDSV,&m_pDepthStencilView);
    X_ASSERTM(hr == S_OK,"Could not create depth stencil view.")


	//Set the buffers again
    //XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);

	m_desc.width = width;
	m_desc.height = height;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::recreateSwapChainBuffers(XU32 width,XU32 height)
{
	//Release all refrences to swap chain buffers
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pBackBufferTexture);

	//Resize the swapchain
	DXGI_SWAP_CHAIN_DESC swapDesc;
    m_pSwapChain->GetDesc(&swapDesc);
	HRESULT hr = m_pSwapChain->ResizeBuffers(swapDesc.BufferCount,width,height,swapDesc.BufferDesc.Format,0);
	X_ASSERTM(hr == S_OK,"Could not resize buffers for swap chain.");

	//Send a resize event to all scenes with the width and height scale change
	//XF32 widthScale = (XF32)width/(XF32)m_orgWidth;
	//XF32 heightScale = (XF32)height/(XF32)m_orgHeight;
	for (XU32 i = 0;i < MAX_LAYERS;++i)
	{
		XMemBlockList<XScene*>::iterator sceneItr(m_scenes[i].begin());
		while (sceneItr.isValid())
		{
			sceneItr.getValue()->handleResizeEvent(width,height/*,widthScale,heightScale*/);
			sceneItr.getNext();			
		}
	}

	return createSwapChainBuffers(width,height);
}


XStatus XRenderContext::setToDevice()
{
	XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(1,&m_pRenderTargetView,m_pDepthStencilView);
	//XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView,(XF32*)&m_clearColor);
	//XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView,D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL,1.0f,0);

	/*//Setup the viewport
	D3D10_VIEWPORT vp;
    vp.Width = m_screenWidth;
    vp.Height = m_screenHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    XVP::GraphicsMgr.m_pD3DDevice->RSSetViewports( 1, &vp );

	//Setup scissor region
	D3D10_RECT rects[1];
	rects[0].left = 0;
	rects[0].right = m_screenWidth;
	rects[0].top = 0;
	rects[0].bottom = m_screenHeight;
	XVP::GraphicsMgr.m_pD3DDevice->RSSetScissorRects(1,rects);*/

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::setClearColor(XColor& color)
{
	m_desc.clearColor = color;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::getClearColor(XColor& color)
{
	color = m_desc.clearColor;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::setDepthClearValue(XF32 value)
{
	m_depthClearValue = value;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XRenderContext::getDepthClearValue()
{
	return m_depthClearValue;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::setStencilClearValue(ULONG32 value)
{
	m_stencilClearValue = value;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XRenderContext::getStencilClearValue()
{
	return m_stencilClearValue;
}



//*****************************************************************************
/**
Adds a controller to X-VP.  Once the controller is added, the engine
will update the controller at the appropriate times.

@param pController	The controller to register with X-VP.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XRenderContext::addScene(XU32 layer,XScene* pScene)
{
	X_ASSERT(layer < MAX_LAYERS)
	X_ASSERTM(pScene,"Invalid scene pointer");
	X_DEBUG
	(
		for (XU32 i = 0;i < MAX_LAYERS;++i)
		{
			XMemBlockList<XScene*>::iterator itr(m_scenes[i].begin());
			while (itr.isValid())
			{
				if (itr.getValue() == pScene)
					X_ASSERT_FORCED("Scene cannot be added to render context more than once");
				itr.getNext();
			}
		}
	)

	//We can only add scenes if we are not in the scene update
	//state.
	if (m_updateState == X_US_SCENES)
	{
		XSceneCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.pScene = pScene;
		cmd.layer = layer;
		m_sceneCmdQueue.insertBack(cmd);
	}
	else
        m_scenes[layer].insertBack(pScene);

	return X_OK;
}



//*****************************************************************************
/**
Removes a controller from X-VP.  Once the controller is removed, it will
no longer be updated by the engine.

@param pController	The controller to remove from X-VP.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XRenderContext::removeScene(XScene* pScene)
{
	X_ASSERTM(pScene,"Invalid scene pointer");
	X_DEBUG
	(
		for (XU32 i = 0;i < MAX_LAYERS;++i)
		{
			XMemBlockList<XScene*>::iterator itr(m_scenes[i].begin());
			XBOOL found = false;
			while (itr.isValid())
			{
				if (itr.getValue() == pScene)
				{
					found = true;
					break;
				}
				itr.getNext();			
			}
			if (!found)
				X_ASSERT_FORCED("Scene does not exist in render context and therefore cannot be removed");
		}
	)

	//We can only remove scenes if we are not in the scene update
	//state.
	if (m_updateState == X_US_SCENES)
	{
		XSceneCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pScene = pScene;
		m_sceneCmdQueue.insertBack(cmd);
	}
	else
	{
		for (XU32 i = 0;i < MAX_LAYERS;++i)
		{
			m_scenes[i].remove(pScene);
		}
	}

	return X_OK;
}
	


//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::insertScene(XU32 layer,XScene* pDependentScene,XScene* pScene)
{
	X_ASSERT(layer < MAX_LAYERS)
	X_ASSERTM(pDependentScene,"Invalid scene pointer");
	X_ASSERTM(pScene,"Invalid scene pointer");
	X_DEBUG
	(
		XMemBlockList<XScene*>::iterator itr(m_scenes[layer].begin());
		XBOOL found = false;
		while (itr.isValid())
		{
			if (itr.getValue() == pDependentScene)
			{
				found = true;
				break;
			}
			itr.getNext();			
		}
		if (!found)
			X_ASSERT_FORCED("Dependent scene does not exist in render context");

		for (XU32 i = 0;i < MAX_LAYERS;++i)
		{
			XMemBlockList<XScene*>::iterator itr(m_scenes[i].begin());
			while (itr.isValid())
			{
				if (itr.getValue() == pScene)
					X_ASSERT_FORCED("Scene cannot be added to render context more than once");
				itr.getNext();
			}
		}
		
	)

	XMemBlockList<XScene*>::iterator sceneItr(m_scenes[layer].begin());
	while (sceneItr.isValid())
	{
		if (sceneItr.getValue() == pDependentScene)
		{
			m_scenes[layer].insert(sceneItr,pScene);
			break;
		}
		sceneItr.getNext();			
	}
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRenderContext::removeAllScenes()
{
	for (XU32 i = 0;i < MAX_LAYERS;++i)
	{
		m_scenes[i].clear();
	}
	return X_OK;
}



//*****************************************************************************
/**
Processes all controller commands on the command queue.

@return None.
*/
//*****************************************************************************
void XRenderContext::processSceneCmds(XF32 dt)
{
	//Let's process and empty the command queues.  
	while (m_sceneCmdQueue.getNumItems() != 0)
	{
		XSceneCmd* pCmd = &m_sceneCmdQueue.getFront();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			addScene(pCmd->layer,pCmd->pScene);
			pCmd->pScene->update(dt);
			break;
		case X_CMDT_REMOVE:
			removeScene(pCmd->pScene);
			break;
		}
		m_sceneCmdQueue.removeFront();		
	}
}

#ifndef XVP_FINAL
XScene* XRenderContext::getDebugScene()
{
	return &m_debugScene;
}
#endif