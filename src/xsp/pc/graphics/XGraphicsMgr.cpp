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
//	Module Name:	XGraphicsMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

	
#define X_GRAPHICS_LUA_LIB_NAME	"Graphics"

#define X_IS_MOVIE_HANDLE(x)		(x & 0x80000000)
#define X_MAKE_MOVIE_HANDLE(x)		(x |= 0x80000000)
#define X_MAKE_INTERNAL_HANDLE(x)	(x &= 0x7FFFFFFF)

const luaL_reg XGraphicsLuaLib[] = 
{ 
#ifdef X_CONSOLE
	{"showMethods",LAPI_graphics},
#endif
	/*{"getScreenWidth",LAPI_getScreenWidth},
	{"getScreenHeight",LAPI_getScreenHeight},
	{"setClearColor",LAPI_setClearColor},
	{"getClearColor",LAPI_getClearColor},
	{"setDepthClearValue",LAPI_setDepthClearValue},
	{"getDepthClearValue",LAPI_getDepthClearValue},
	{"setStencilClearValue",LAPI_setStencilClearValue},
	{"getStencilClearValue",LAPI_getStencilClearValue},*/
	{"dumpScreenshot",LAPI_dumpScreenshot},
	{"setRedGammaPercent",LAPI_setRedGammaPercent},
	{"setBlueGammaPercent",LAPI_setBlueGammaPercent},
	{"setGreenGammaPercent",LAPI_setGreenGammaPercent},
	{"setRGBGammaPercent",LAPI_setRGBGammaPercent},
	{"invertRGBGamma",LAPI_invertRGBGamma},
	{"getRedGammaPercent",LAPI_getRedGammaPercent},
	{"getBlueGammaPercent",LAPI_getBlueGammaPercent},
	{"getGreenGammaPercent",LAPI_getGreenGammaPercent},
	{"enableAlphaSorting",LAPI_enableAlphaSorting},
	/*{"createLight",LAPI_createLight},
	{"getDeviceCaps",LAPI_getDeviceCaps},*/
	{NULL,NULL}
};



//***************************************************************************** 
/**
Constructor for the graphics driver.  
*/
//*****************************************************************************
XGraphicsMgr::XGraphicsMgr()
{
	//Set XGraphicsMgr system variables.
	m_pDXGIFactory = NULL;
	m_pD3DDevice = NULL;
	m_pFullScreenModes = NULL;
	m_numFullScreenModes = 0;

	m_spriteVBHandle = X_INVALID_HANDLE;
	
	m_pHeap = NULL;

	m_vsHandleCache = X_INVALID_HANDLE;
	m_gsHandleCache = X_INVALID_HANDLE;
	m_psHandleCache = X_INVALID_HANDLE;
	m_matHandleCache = X_INVALID_HANDLE;
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		m_texHandleCache[i] = X_INVALID_HANDLE;
		m_texOverrides[i] = false;
	}

	m_viewportQuadVB = X_INVALID_HANDLE;
	m_viewportQuadVS = X_INVALID_HANDLE;
	m_viewportQuadMat = X_INVALID_HANDLE;

	m_activeRenderContext = X_INVALID_HANDLE;

	m_defaultTexHandle = X_INVALID_HANDLE;
	m_missingTexHandle = X_INVALID_HANDLE;
	m_missingMatHandle = X_INVALID_HANDLE;
	m_streamingTexHandle = X_INVALID_HANDLE;

	m_inited = false;
}



//***************************************************************************** 
/**
Destructor for the graphics driver.  
*/
//*****************************************************************************
XGraphicsMgr::~XGraphicsMgr()
{
} 



#ifdef OLD
//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRedGammaPercent(XF32 percent)
{
	D3DGAMMARAMP ramp;
	XF32 p,delta;
	m_pD3DDevice->GetGammaRamp(0,&ramp);
	
	//If percentage is out of range, return an error.
	if ((percent < 0) || (percent > 1))
		return X_ERR_INVALID_PARAMS;

	//If percent is greater than .5, then we will use a different
	//algoritm to move the ramp closer to full intensity.
	if (percent > .5f)
	{
		p = 1 - ((percent - .5f)/.5f); 
		delta = p * (XF32)256;
		for (int i = 255;i >= 0;--i)
			ramp.red[i] = (WORD)(65535 - ((255-i) * delta));
	}
	else
	{
		p = percent/.5f;
		delta = p * (XF32)256;
		for (XU32 i = 0;i < 256;++i)
			ramp.red[i] = (WORD)(i * delta);
	}

	m_pD3DDevice->SetGammaRamp(0,0,&ramp);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setBlueGammaPercent(XF32 percent)
{
	D3DGAMMARAMP ramp;
	XF32 p,delta;
	m_pD3DDevice->GetGammaRamp(0,&ramp);
	
	//If percentage is out of range, return an error.
	if ((percent < 0) || (percent > 1))
		return X_ERR_INVALID_PARAMS;

	//If percent is greater than .5, then we will use a different
	//algoritm to move the ramp closer to full intensity.
	if (percent > .5f)
	{
		p = 1 - ((percent - .5f)/.5f); 
		delta = p * (XF32)256;
		for (int i = 255;i >= 0;--i)
			ramp.blue[i] = (WORD)(65535 - ((255-i) * delta));
	}
	else
	{
		p = percent/.5f;
		delta = p * (XF32)256;
		for (XU32 i = 0;i < 256;++i)
			ramp.blue[i] = (WORD)(i * delta);
	}

	m_pD3DDevice->SetGammaRamp(0,0,&ramp);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setGreenGammaPercent(XF32 percent)
{
	D3DGAMMARAMP ramp;
	XF32 p,delta;
	m_pD3DDevice->GetGammaRamp(0,&ramp);

	//If percentage is out of range, return an error.
	if ((percent < 0) || (percent > 1))
		return X_ERR_INVALID_PARAMS;

	//If percent is greater than .5, then we will use a different
	//algoritm to move the ramp closer to full intensity.
	if (percent > .5f)
	{
		p = 1 - ((percent - .5f)/.5f); 
		delta = p * (XF32)256;
		for (int i = 255;i >= 0;--i)
			ramp.green[i] = (WORD)(65535 - ((255-i) * delta));
	}
	else
	{
		p = percent/.5f;
		delta = p * (XF32)256;
		for (XU32 i = 0;i < 256;++i)
			ramp.green[i] = (WORD)(i * delta);
	}

	m_pD3DDevice->SetGammaRamp(0,0,&ramp);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRGBGammaPercent(XF32 rPercent,XF32 gPercent,XF32 bPercent)
{
	D3DGAMMARAMP ramp;
	XF32 p,delta;

	//If percentages are out of range, return an error.
	if ((rPercent < 0) || (rPercent > 1) ||
		(gPercent < 0) || (gPercent > 1) ||
		(bPercent < 0) || (bPercent > 1))
		return X_ERR_INVALID_PARAMS;

	//If percent is greater than .5, then we will use a different
	//algoritm to move the ramp closer to full intensity.
	if (rPercent > .5f)
	{
		p = 1 - ((rPercent - .5f)/.5f); 
		delta = p * (XF32)256;
		for (int i = 255;i >= 0;--i)
			ramp.red[i] = (WORD)(65535 - ((255-i) * delta));
	}
	else
	{
		p = rPercent/.5f;
		delta = p * (XF32)256;
		for (XU32 i = 0;i < 256;++i)
			ramp.red[i] = (WORD)(i * delta);
	}

	//If percent is greater than .5, then we will use a different
	//algoritm to move the ramp closer to full intensity.
	if (gPercent > .5f)
	{
		p = 1 - ((gPercent - .5f)/.5f); 
		delta = p * (XF32)256;
		for (int i = 255;i >= 0;--i)
			ramp.green[i] = (WORD)(65535 - ((255-i) * delta));
	}
	else
	{
		p = gPercent/.5f;
		delta = p * (XF32)256;
		for (XU32 i = 0;i < 256;++i)
			ramp.green[i] = (WORD)(i * delta);
	}

	//If percent is greater than .5, then we will use a different
	//algoritm to move the ramp closer to full intensity.
	if (bPercent > .5f)
	{
		p = 1 - ((bPercent - .5f)/.5f); 
		delta = p * (XF32)256;
		for (int i = 255;i >= 0;--i)
			ramp.blue[i] = (WORD)(65535 - ((255-i) * delta));
	}
	else
	{
		p = bPercent/.5f;
		delta = p * (XF32)256;
		for (XU32 i = 0;i < 256;++i)
			ramp.blue[i] = (WORD)(i * delta);
	}

	m_pD3DDevice->SetGammaRamp(0,0,&ramp);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::invertRGBGamma()
{
	XMemBlockList<WORD> redList;
	XMemBlockList<WORD> greenList;
	XMemBlockList<WORD> blueList;
	D3DGAMMARAMP ramp;
	m_pD3DDevice->GetGammaRamp(0,&ramp);

	for (XU32 i = 0;i < 256;++i)
	{
		redList.insertBack(ramp.red[i]);
		greenList.insertBack(ramp.green[i]);
		blueList.insertBack(ramp.blue[i]);
	}

	for (XU32 i = 0;i < 256;++i)
	{
		ramp.red[i] = redList.getBack();
		redList.removeBack();
		ramp.green[i] = greenList.getBack();
		greenList.removeBack();
		ramp.blue[i] = blueList.getBack();
		blueList.removeBack();
	}

	m_pD3DDevice->SetGammaRamp(0,0,&ramp);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XGraphicsMgr::getRedGammaPercent()
{
	D3DGAMMARAMP ramp;
	m_pD3DDevice->GetGammaRamp(0,&ramp);
	XF32 p = (ramp.red[5]-ramp.red[4])/(XF32)256;
	if (ramp.red[0] == 0)
		return (p * .5f);
	else
		return .5f + (.5f - (p * .5f));
}



//*****************************************************************************
//*****************************************************************************
XF32 XGraphicsMgr::getBlueGammaPercent()
{
	D3DGAMMARAMP ramp;
	m_pD3DDevice->GetGammaRamp(0,&ramp);
	XF32 p = (ramp.blue[5]-ramp.blue[4])/(XF32)256;
	if (ramp.blue[0] == 0)
		return (p * .5f);
	else
		return .5f + (.5f - (p * .5f));
}



//*****************************************************************************
//*****************************************************************************
XF32 XGraphicsMgr::getGreenGammaPercent()
{
	D3DGAMMARAMP ramp;
	m_pD3DDevice->GetGammaRamp(0,&ramp);
	XF32 p = (ramp.green[5]-ramp.green[4])/(XF32)256;
	if (ramp.green[0] == 0)
		return (p * .5f);
	else
		return .5f + (.5f - (p * .5f));
}

#endif



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createRenderContext(XRenderContextDesc& desc,XHandle& rcHandle)
{
	XRenderContext* pRenderContext;
	XNew(pRenderContext,XRenderContext,m_pHeap);
	if ((m_status = pRenderContext->init(desc)) != X_OK)
	{
		XDelete(pRenderContext);
		X_ASSERT_FORCED("Could not create render context");
	}
	else
	{
		//Add to resource list
		rcHandle = m_renderContexts.addResource(pRenderContext,NULL);
		X_ASSERTM(rcHandle != X_INVALID_HANDLE,"Could not add render context to resource list");
#ifndef XVP_FINAL
		if (desc.createDebugScene)
			XRenderContext::getDebugScene()->setRenderContext(rcHandle);
#endif
	}

	return m_status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyRenderContext(XHandle rcHandle)
{
	if (rcHandle != X_INVALID_HANDLE)
	{
		return m_renderContexts.removeResource(rcHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getNumRenderContexts()
{
	return m_renderContexts.getNumResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllRenderContexts()
{
	m_renderContexts.destroyAllResources();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addRenderContextRef(XHandle rcHandle)
{
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(rcHandle);
	if (pRenderContext)
	{
		pRenderContext->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRenderContext(XHandle renderContext)
{
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	if (pRenderContext)
	{
		//m_pRenderContext = pRenderContext;
		pRenderContext->setToDevice();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;	
}



//*****************************************************************************
//*****************************************************************************
XHandle XGraphicsMgr::getRenderContext()
{
	return m_activeRenderContext;
}


XRenderContextDesc* XGraphicsMgr::getRenderContextDesc(XHandle renderContext)
{
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	if (pRenderContext)
	{
		return pRenderContext->getDesc();
	}
	else 
		return NULL;	
}


//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::copyRenderTargetToRenderContext(XHandle renderTargetArray,
													  XU32 renderTargetIndex,
													  XHandle renderContext)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);

	m_pD3DDevice->CopyResource(pRenderContext->getD3D10BackBuffer(),pRenderTargetArray->getD3D10Resource(renderTargetIndex));

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::copyViewportToRenderContext(XHandle renderTargetArray, 
									XU32 renderTargetIndex,
									XViewport& viewport,
									XHandle renderContext)
{
	//Can't copy a viewport that is not within the render context
	if (viewport.topLeftX < 0 || viewport.topLeftY < 0)
		return X_ERR_OPERATION_FAILED;

	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);

	D3D10_BOX srcBox;
	srcBox.left = viewport.topLeftX;
	srcBox.top = viewport.topLeftY;
	srcBox.right = viewport.topLeftX + viewport.width;
	srcBox.bottom = viewport.topLeftY + viewport.height;
	srcBox.back = 1;
	srcBox.front = 0;

	//DirectX requires the src region to fit entirely within dest region.  If this isn't
	//the case, bail.
	/*if (pRenderContext->getWidth() < srcBox.right ||
		pRenderContext->getHeight() < srcBox.bottom ||
		srcBox.left < 0 ||
		srcBox.top < 0)
		return X_ERR_OPERATION_FAILED;*/

	m_pD3DDevice->CopySubresourceRegion(pRenderContext->getD3D10BackBuffer(),
										0,
										viewport.topLeftX,
										viewport.topLeftY,
										0,
										pRenderTargetArray->getD3D10Resource(renderTargetIndex),
										0,
										&srcBox);
						
	return X_OK;
}
	


//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::copyViewportToRenderTarget(XHandle srcRenderTargetArray,
												 XU32 srcRenderTargetIndex,
												 XViewport& srcViewport,
												 XHandle destRenderTargetArray,
												 XU32 destRenderTargetIndex)
{
	XRenderTargetArray* pSrcRTA = (XRenderTargetArray*)m_renderTargetArrays.getResource(srcRenderTargetArray);
	XRenderTargetArray* pDestRTA = (XRenderTargetArray*)m_renderTargetArrays.getResource(destRenderTargetArray);
	
	D3D10_BOX srcBox;
	srcBox.left = srcViewport.topLeftX;
	srcBox.top = srcViewport.topLeftY;
	srcBox.right = srcViewport.topLeftX + srcViewport.width;
	srcBox.bottom = srcViewport.topLeftY + srcViewport.height;
	srcBox.back = 1;
	srcBox.front = 0;

	m_pD3DDevice->CopySubresourceRegion(pDestRTA->getD3D10Resource(destRenderTargetIndex),
										0,
										srcViewport.topLeftX,
										srcViewport.topLeftY,
										0,
										pSrcRTA->getD3D10Resource(srcRenderTargetIndex),
										0,
										&srcBox);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::copyRectToRenderTarget(XHandle srcRenderTargetArray,
										 XU32 srcRenderTargetIndex,
										 XRect& srcRect,
										 XHandle destRenderTargetArray,
										 XU32 destRenderTargetIndex,
										 XU32 destTopLeftX,
										 XU32 destTopLeftY)
{
	XRenderTargetArray* pSrcRTA = (XRenderTargetArray*)m_renderTargetArrays.getResource(srcRenderTargetArray);
	XRenderTargetArray* pDestRTA = (XRenderTargetArray*)m_renderTargetArrays.getResource(destRenderTargetArray);
	
	D3D10_BOX srcBox;
	srcBox.left = srcRect.left;
	srcBox.top = srcRect.top;
	srcBox.right = srcRect.right;
	srcBox.bottom = srcRect.bottom;
	srcBox.back = 1;
	srcBox.front = 0;

	m_pD3DDevice->CopySubresourceRegion(pDestRTA->getD3D10Resource(destRenderTargetIndex),
										0,
										destTopLeftX,
										destTopLeftY,
										0,
										pSrcRTA->getD3D10Resource(srcRenderTargetIndex),
										0,
										&srcBox);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::copyRectToRenderContext(XHandle srcRenderTargetArray,
										 XU32 srcRenderTargetIndex,
										 XRect& srcRect,
										 XHandle destRenderContext,
										 XU32 destTopLeftX,
										 XU32 destTopLeftY)
{
	XRenderTargetArray* pSrcRTA = (XRenderTargetArray*)m_renderTargetArrays.getResource(srcRenderTargetArray);
	XRenderContext* pDestRC = (XRenderContext*)m_renderContexts.getResource(destRenderContext);
	
	D3D10_BOX srcBox;
	srcBox.left = srcRect.left;
	srcBox.top = srcRect.top;
	srcBox.right = srcRect.right;
	srcBox.bottom = srcRect.bottom;
	srcBox.back = 1;
	srcBox.front = 0;

	//DirectX requires the src region to fit entirely within dest region.  If this isn't
	//the case, bail.
	/*if (pDestRC->getWidth() < (srcBox.right - srcBox.left) ||
		pDestRC->getHeight() < (srcBox.bottom - srcBox.top) ||
		destTopLeftX >= pDestRC->getWidth() ||
		destTopLeftY >= pDestRC->getHeight())
		return X_ERR_OPERATION_FAILED;*/

	m_pD3DDevice->CopySubresourceRegion(pDestRC->getD3D10BackBuffer(),
										0,
										destTopLeftX,
										destTopLeftY,
										0,
										pSrcRTA->getD3D10Resource(srcRenderTargetIndex),
										0,
										&srcBox);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::createRenderTargetArray(XRenderTargetArrayDesc& desc,XHandle& renderTargetArray)
{
	XRenderTargetArray* pRenderTargetArray;
	XNew(pRenderTargetArray,XRenderTargetArray,m_pHeap);
	if ((m_status = pRenderTargetArray->init(desc)) != X_OK)
	{
		XDelete(pRenderTargetArray);
		X_ASSERT_FORCED("Could not create render target array");
	}
	else
	{
		//Add to resource list
		renderTargetArray = m_renderTargetArrays.addResource(pRenderTargetArray,NULL);
		X_ASSERTM(renderTargetArray != X_INVALID_HANDLE,"Could not add render target array to resource list");
	}

	return m_status;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::clearRenderTargetArray(XHandle renderTargetArray,XBOOL color,XBOOL depth,XBOOL stencil)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->clear(color,depth,stencil);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRenderTargetClearColor(XHandle renderTargetArray,XU32 renderTargetIndex,XColor& color)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->setRenderTargetClearColor(renderTargetIndex,color);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::getRenderTargetClearColor(XHandle renderTargetArray,XU32 renderTargetIndex,XColor& color)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->getRenderTargetClearColor(renderTargetIndex,color);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRenderTargetArrayClearColor(XHandle renderTargetArray,XColor& color)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->setClearColor(color);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setDepthClearValue(XHandle renderTargetArray,XF32 value)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->setDepthClearValue(value);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



XStatus XGraphicsMgr::overrideDepthBuffer(XHandle renderTargetArray,XHandle overrideRTA)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	XRenderTargetArray* pOverrideRTA = (XRenderTargetArray*)m_renderTargetArrays.getResource(overrideRTA);

	if (pRenderTargetArray && pOverrideRTA)
	{
		X_ASSERTM(pRenderTargetArray->getDesc()->msaaCount == pOverrideRTA->getDesc()->msaaCount,"MSAA count settings do not match for render target arrays");
		X_ASSERTM(pRenderTargetArray->getDesc()->msaaQuality == pOverrideRTA->getDesc()->msaaQuality,"MSAA quality settings do not match for render target arrays");
		pRenderTargetArray->overrideDepthBuffer(pOverrideRTA->getDepthStencilTexture(),
												pOverrideRTA->getNoMSAADepthStencilTexture(), 
												pOverrideRTA->getDepthStencilView(),
												pOverrideRTA->getNoMSAADepthStencilView(),
												pOverrideRTA->getNoMSAADepthStencilSRV());
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XF32 XGraphicsMgr::getDepthClearValue(XHandle renderTargetArray)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		return pRenderTargetArray->getDepthClearValue();
	}
	else 
		return 0.0f;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setStencilClearValue(XHandle renderTargetArray,XU32 value)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->setStencilClearValue(value);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XU32 XGraphicsMgr::getStencilClearValue(XHandle renderTargetArray)
{	
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		return pRenderTargetArray->getStencilClearValue();
	}
	else 
		return 0;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::clearRenderTargetArray(XHandle renderTargetArray,XU32 renderTargetIndex)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->clear(renderTargetIndex);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::destroyRenderTargetArray(XHandle renderTargetArray)
{
	if (renderTargetArray != X_INVALID_HANDLE)
	{
		return m_renderTargetArrays.removeResource(renderTargetArray);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XU32 XGraphicsMgr::getNumRenderTargetArrays()
{
	return m_renderTargetArrays.getNumResources();
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::destroyAllRenderTargetArrays()
{
	m_renderTargetArrays.destroyAllResources();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::addRenderTargetArrayRef(XHandle renderTargetArray)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::resolveRenderTargetArray(XHandle renderTargetArray)
{	
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->resolve();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;	
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRenderTargetArray(XHandle renderTargetArray,XBOOL setDepthBuffer)
{	
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		pRenderTargetArray->setToDevice(setDepthBuffer);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;	
}


//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::removeRenderTargetArray()
{
	ID3D10RenderTargetView *const pRTVs[X_MAX_RENDER_TARGETS] = {NULL};
	XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(X_MAX_RENDER_TARGETS,pRTVs,NULL);
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XRenderTargetArrayDesc* XGraphicsMgr::getRenderTargetArrayDesc(XHandle renderTargetArray)
{
	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	if (pRenderTargetArray)
	{
		return pRenderTargetArray->getDesc();
	}
	else 
		return NULL;	
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setRenderTargetAsTexture(XHandle renderTargetArray,
											   XU32 renderTargetIndex,
											   XU32 texStage)
{
	X_ASSERTM(texStage < X_MAX_SAMPLERS,"Texture stage index out of range.")

	//Setting a render target as a texure is a special case and invalidates
	//the cache.  Only regular texture calls should be checked against the 
	//cache.    
	m_texHandleCache[texStage] = X_INVALID_HANDLE;
	
	ID3D10ShaderResourceView* pD3DTexture = NULL;

	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	ID3D10ShaderResourceView* pSRV = pRenderTargetArray->getD3D10ShaderResourceView(renderTargetIndex);
	m_pD3DDevice->PSSetShaderResources(texStage,1,&pSRV);
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setDepthStencilAsTexture(XHandle renderTargetArray,
											   XU32 texStage)
{
	//Setting a depth stencil as a texure is a special case and invalidates
	//the cache.  Only regular texture calls should be checked against the 
	//cache.    
	m_texHandleCache[texStage] = X_INVALID_HANDLE;
	
	ID3D10ShaderResourceView* pD3DTexture = NULL;

	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	ID3D10ShaderResourceView* pSRV = pRenderTargetArray->getNoMSAADepthStencilSRV();
	m_pD3DDevice->PSSetShaderResources(texStage,1,&pSRV);
	
	return X_OK;
}

XBOOL XGraphicsMgr::isFullScreenMode(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle.");

	//Get render context
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->isFullScreenMode();

}


//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setFullScreenMode(XHandle renderContext,XU32 fullScreenModeIndex)
{
	X_ASSERTM(fullScreenModeIndex < m_numFullScreenModes,"Display mode index out of range.");
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle.");

	//Get render context
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->setFullScreenMode(m_pFullScreenModes[fullScreenModeIndex]);
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::setWindowedMode(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle.");

	//Get render context
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->setWindowedMode();
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::resizeWindow(XHandle renderContext,XU32 width,XU32 height)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle.");

	//Get render context
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	//Bail if we the size is already what we want
	if ((pRenderContext->getWidth() == width) && (pRenderContext->getHeight() == height))
		return X_OK;

	return pRenderContext->resizeWindow(width,height);
}



//***************************************************************************** 
/**
Creates an index buffer and its required resources in the graphics driver.

@param numIndices	The number of indices in the index buffer.
@param ibHandle	A variable to hold the index of the newly created
						index buffer.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::createIndexBuffer(XU32 numIndices,XBOOL dynamic,XHandle& ibHandle)
{
	XIndexBuffer* pIndexBuffer;
	XNew(pIndexBuffer,XIndexBuffer,m_pHeap);
	if ((m_status = pIndexBuffer->init(numIndices,dynamic)) != X_OK)
	{
		XDelete(pIndexBuffer);//X_SAFE_DELETE(pIndexBuffer);
	}
	else
	{
		//Add to resource list
		ibHandle = m_indexBuffers.addResource(pIndexBuffer,NULL);
		if (ibHandle == X_INVALID_HANDLE)
			return X_ERR_OPERATION_FAILED;
	}

	return m_status;
}



//***************************************************************************** 
/**
Locks an index buffer for modification and returns a pointer to the index buffer
data.  The locked index buffer must be unlocked when you are done with it.

@param ibHandle	The handle of the index buffer to lock.
@return A pointer to the index buffer data.
*/
//***************************************************************************** 
WORD* XGraphicsMgr::lockIndexBuffer(XHandle ibHandle,X_LOCK_TYPE lockType)
{
	XBaseIndexBuffer* pIndexBuffer = (XBaseIndexBuffer*)m_indexBuffers.getResource(ibHandle);
	if (pIndexBuffer)
	{
		return pIndexBuffer->lock(lockType);
	}
	else 
		return NULL;
}



//***************************************************************************** 
/**
Unlocks an index buffer after it has been locked.  

@param ibHandle	The handle of the index buffer to unlock.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::unlockIndexBuffer(XHandle ibHandle)
{
	XBaseIndexBuffer* pIndexBuffer = (XBaseIndexBuffer*)m_indexBuffers.getResource(ibHandle);
	if (pIndexBuffer)
	{
		return pIndexBuffer->unlock();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


XIndexBuffer* XGraphicsMgr::getIndexBuffer(XHandle ibHandle)
{
	return (XIndexBuffer*)m_indexBuffers.getResource(ibHandle);
}

//***************************************************************************** 
/**
Gets the length of an index buffer.  The length corresponds to the number
of indices contained within the index buffer.

@param ibHandle	The handle of the index buffer for which to get the length.
@return The number of indices in the index buffer.
*/
//***************************************************************************** 
XU32 XGraphicsMgr::getIndexBufferLength(XHandle ibHandle)
{
	XBaseIndexBuffer* pIndexBuffer = (XBaseIndexBuffer*)m_indexBuffers.getResource(ibHandle);
	if (pIndexBuffer)
	{
		return pIndexBuffer->getLength();
	}
	else 
		return 0;
}



//***************************************************************************** 
/**
Destroys all index buffers and all associated resources in the graphics driver.

@return None.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllIndexBuffers()
{	
	m_indexBuffers.destroyAllResources();

	return X_OK;
}



//***************************************************************************** 
/**
Gets the number of currently allocated index buffers.

@return The number of allocated index buffers.
*/
//***************************************************************************** 
XU32 XGraphicsMgr::getNumIndexBuffers()
{
	return m_indexBuffers.getNumResources();
}



//***************************************************************************** 
/**
Destroys an index buffer and its associated resources.

@param ibHandle	The handle of the index buffer to destroy.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::destroyIndexBuffer(XHandle ibHandle)
{
	if (ibHandle != X_INVALID_HANDLE)
	{
		return m_indexBuffers.removeResource(ibHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addIndexBufferRef(XHandle ibHandle)
{
	XBaseIndexBuffer* pIndexBuffer = (XBaseIndexBuffer*)m_indexBuffers.getResource(ibHandle);
	if (pIndexBuffer)
	{
		pIndexBuffer->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************  
//*****************************************************************************  
XStatus XGraphicsMgr::createVertexDeclarationName(XVertexDesc& desc,
													 XS8 name[X_MAX_VERT_DCL_NAME_LEN])
{
	//Create a unique name for this vertex description using the following
	//symbology in the following order. All vertex declarations have a position.
	//
	// Color			- c
	// Normal			- n
	// UV Coordinates	- uv
	// Tangent			- tan
	// Binormal			- bin
	// Blend Indices	- bi
	// Blend Weights	- bw
	//
	// Example ->  "1c1n2uv1tan1bin1bi1bw" - 1 color, 1 normal, 2 uv sets, 1 tangent
	// 1 binormal, 1 set of 4 bones, 1 set of 4 weights

	XS8 number[10];
	name[0] = '\0';

	//Colors
	if (desc.numColors != 0)
	{
		strcat(name,_itoa(desc.numColors,number,10));
		strcat(name,"c");
	}
	//Normals
	if (desc.numNormals != 0)
	{
		strcat(name,_itoa(desc.numNormals,number,10));
		strcat(name,"n");
	}
	//UVs
	if (desc.numUVs != 0)
	{
		strcat(name,_itoa(desc.numUVs,number,10));
		strcat(name,"uv");
	}
	//Tangents
	if (desc.numTangents != 0)
	{
		strcat(name,_itoa(desc.numTangents,number,10));
		strcat(name,"tan");
	}
	//Binormals
	if (desc.numBinormals != 0)
	{
		strcat(name,_itoa(desc.numBinormals,number,10));
		strcat(name,"bin");
	}
	//Blend Indices
	if (desc.numBoneIndices4Tuplets != 0)
	{
		strcat(name,_itoa(desc.numBoneIndices4Tuplets,number,10));
		strcat(name,"bi");
	}
	//Blend Weights
	if (desc.numBoneWeights4Tuplets != 0)
	{
		strcat(name,_itoa(desc.numBoneWeights4Tuplets,number,10));
		strcat(name,"bw");
	}

	return X_OK;
}

XStatus XGraphicsMgr::createShaderSignature(XVertexDesc& desc,XS8* pShaderName)
{
	X_ASSERT(pShaderName);
	XS8 buf[2048] =	"struct VS_INPUT \n {\n float4 position	: POSITION;\n";

	//Add vertex attributes based on what the vertex declaration contains

	//Colors
	XS8 number[10];
	for (XU32 i = 0;i < desc.numColors;++i)
	{
		strcat(buf,"float4 diffuse");
		strcat(buf,_itoa(i,number,10));
		strcat(buf," : COLOR"); 
		strcat(buf,number);
		strcat(buf,";\n");
	}

	//Normals
	for (XU32 i = 0;i < desc.numNormals;++i)
	{
		strcat(buf,"float4 normal");
		strcat(buf,_itoa(i,number,10));
		strcat(buf," : NORMAL"); 
		strcat(buf,number);
		strcat(buf,";\n");
	}
	
	//UVs
	for (XU32 i = 0;i < desc.numUVs;++i)
	{
		strcat(buf,"float2 uv");
		strcat(buf,_itoa(i,number,10));
		strcat(buf," : TEXCOORD"); 
		strcat(buf,number);
		strcat(buf,";\n");
	}
	//Tangents
	for (XU32 i = 0;i < desc.numTangents;++i)
	{
		strcat(buf,"float3 tangent");
		strcat(buf,_itoa(i,number,10));
		strcat(buf," : TANGENT"); 
		strcat(buf,number);
		strcat(buf,";\n");
	}
	//Binormals
	for (XU32 i = 0;i < desc.numBinormals;++i)
	{
		strcat(buf,"float3 binormal");
		strcat(buf,_itoa(i,number,10));
		strcat(buf," : BINORMAL"); 
		strcat(buf,number);
		strcat(buf,";\n");
	}
	//Blend Indices
	/*if (desc.numBoneIndices4Tuplets != 0)
	{
		strcat(buf,"float4 diffuse : NORMAL"); 
		strcat(buf,_itoa(desc.numBoneIndices4Tuplets,number,10));
		strcat(buf,";\n");
	}
	//Blend Weights
	if (desc.numBoneWeights4Tuplets != 0)
	{
		strcat(buf,"float4 diffuse : NORMAL"); 
		strcat(buf,_itoa(desc.numBoneWeights4Tuplets,number,10));
		strcat(buf,";\n");
	}*/
	
	strcat(buf,"};	struct VS_OUTPUT{float4 v4Position : SV_Position;};	VS_OUTPUT mainVS(VS_INPUT IN){VS_OUTPUT OUT;OUT.v4Position = float4(0,0,0,0);return OUT;}");
		
	XHandle vsHandle = X_INVALID_HANDLE;
	return createVertexShaderFromString(pShaderName,buf,strlen(buf),"mainVS",vsHandle);
}


//*****************************************************************************  
//*****************************************************************************  
XStatus XGraphicsMgr::createVertexDeclaration(XVertexDesc& desc,
											  XHandle& vdHandle)
{
	//First, check to see if we've already created this vertex declaration
	XS8 name[X_MAX_VERT_DCL_NAME_LEN];
	XHandle tempHandle = X_INVALID_HANDLE;
	createVertexDeclarationName(desc,name);
	XVertexDeclaration* pVertexDcl = (XVertexDeclaration*)m_vertexDeclarations.findResourceByName(name,tempHandle);
	if (tempHandle != X_INVALID_HANDLE)
	{
		pVertexDcl->incRefCount();
		vdHandle = tempHandle;
		return X_OK;
	}

	//Otherwise, this is a new vertex declaration we haven't seen before.  Create a
	//matching shader signature
	createShaderSignature(desc,name);

	XNew(pVertexDcl,XVertexDeclaration,m_pHeap);
	if ((m_status = pVertexDcl->init(desc,name)) != X_OK)
	{
		XDelete(pVertexDcl);
	}
	else
	{
		//Add to resource list
		vdHandle = m_vertexDeclarations.addResource(pVertexDcl,name);
		if (vdHandle == X_INVALID_HANDLE)
			return X_ERR_OPERATION_FAILED;
	}

	return m_status;
}


//*****************************************************************************  
//*****************************************************************************  
XStatus XGraphicsMgr::destroyVertexDeclaration(XHandle vdHandle)
{
	if (vdHandle != X_INVALID_HANDLE)
	{
		return m_vertexDeclarations.removeResource(vdHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************  
//*****************************************************************************  
XVertexDeclaration* XGraphicsMgr::getVertexDeclarationInterface(XHandle vdHandle)
{
	return (XVertexDeclaration*)m_vertexDeclarations.getResource(vdHandle);
}

	
	
//*****************************************************************************  
//*****************************************************************************  
XStatus XGraphicsMgr::getVertexDeclarationDesc(XHandle vdHandle,XVertexDesc& desc)
{
	if (vdHandle != X_INVALID_HANDLE)
	{
		XVertexDeclaration* pVertexDcl = (XVertexDeclaration*)m_vertexDeclarations.getResource(vdHandle);
		pVertexDcl->getDesc(desc);
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************  
//*****************************************************************************  
XU32 XGraphicsMgr::getNumVertexDeclarations(XHandle vdHandle)
{
	return m_vertexDeclarations.getNumResources();
}



//*****************************************************************************  
//*****************************************************************************  
XStatus XGraphicsMgr::destroyAllVertexDeclarations()
{
	m_vertexDeclarations.destroyAllResources();
	return X_OK;
}

	

//*****************************************************************************  
//*****************************************************************************  
XStatus XGraphicsMgr::addVertexDeclarationRef(XHandle vdHandle)
{
	XVertexDeclaration* pVertexDcl = (XVertexDeclaration*)m_vertexDeclarations.getResource(vdHandle);
	if (pVertexDcl)
	{
		pVertexDcl->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************  
/**
Creates a vertex buffer and its required resources in the graphics driver.

@param vbType	The type of vertex buffer to create.
@param primType	The type of geometric primitives the vertex buffer will hold.
@param numVertices	The number of vertices in the vertex buffer.
@param vbHandle	A variable to hold the index of the newly created
						vertex buffer.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::createVertexBuffer(XVertexDesc& desc,X_PRIMITIVE_TYPE primType,
									  XU32 numVertices,XBOOL dynamic,XHandle& vbHandle)
{
	//First, let's create the vertex declaration
	XHandle vdHandle = X_INVALID_HANDLE;
	createVertexDeclaration(desc,vdHandle);
	if (vdHandle == X_INVALID_HANDLE)
	{
		vbHandle = X_INVALID_HANDLE;
		return X_ERR_INVALID_PARAMS;
	}

	//We created a vertex declaration successfully so we can
	//pass it to the vertex buffer 
	vbHandle = X_INVALID_HANDLE;
	XVertexBuffer* pVertexBuffer = NULL;
	XNew(pVertexBuffer,XVertexBuffer,m_pHeap);
	if ((m_status = pVertexBuffer->init(vdHandle,primType,numVertices,dynamic)) != X_OK)
	{
		XDelete(pVertexBuffer);
	}
	else
	{
		//Add to resource list
		vbHandle = m_vertexBuffers.addResource(pVertexBuffer,NULL);
		if (vbHandle == X_INVALID_HANDLE)
			return X_ERR_OPERATION_FAILED;
	}

	return m_status;
}




XVertexBuffer* XGraphicsMgr::getVertexBuffer(XHandle vbHandle)
{
	return (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
}


//*****************************************************************************  
/**
Locks a vertex buffer for modification and returns a pointer to the vertex buffer
data.  The locked vertex buffer must be unlocked when you are done with it.

@param vbHandle	The handle of the vertex buffer to lock.
@param lockType	A combination of flags specifying how to lock the vertex
					buffer.
@return A pointer to the index buffer data.
*/
//***************************************************************************** 
XVertexBuffer* XGraphicsMgr::lockVertexBuffer(XHandle vbHandle,X_LOCK_TYPE lockType)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		if (pVertexBuffer->lockAllStreams(lockType) == X_OK)
			return pVertexBuffer;
		else
			return NULL;
	}
	else 
		return NULL;
}



//*****************************************************************************  
/**
Unlocks a vertex buffer after it has been locked.  

@param vbHandle	The handle of the vertex buffer to unlock.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::unlockVertexBuffer(XHandle vbHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		return pVertexBuffer->unlockAllStreams();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Renders a vertex buffer using the primitive type, vertex type, and other
information given when the vertex buffer was created.

@param vbHandle	The handle of the vertex buffer to render.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::renderVertexBuffer(XHandle vbHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		return pVertexBuffer->render();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Renders a vertex buffer using the primitive type, vertex type, and other
information given when the vertex buffer was created.

@param vbHandle	The handle of the vertex buffer to render.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::renderVertexBufferCount(XHandle vbHandle,XU32 primCount)
{
	X_ASSERTM(primCount != 0,"Cannot render 0 primitive count");

	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		return pVertexBuffer->render(primCount);
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Renders a vertex buffer through the use of an index buffer.

@param vbHandle	The handle of the vertex buffer to render.
@param ibHandle	The handle of the index buffer to use when rendering
						the vertex buffer.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::renderVertexBuffer(XHandle vbHandle,XHandle ibHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	XBaseIndexBuffer* pIndexBuffer = (XBaseIndexBuffer*)m_indexBuffers.getResource(ibHandle);
	if ((pVertexBuffer) && (pIndexBuffer))
	{
		return pVertexBuffer->renderWithIndexBuffer(pIndexBuffer);
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Renders a user vertex buffer.  A user vertex buffer is one in which you 
must supply the memory representing the vertices to be rendered.

@param pType	Primitive type.
@param vType	Vertex type.
@param numPrimitives	The number of primitives in the user's vertex buffer.
@param pPrimitiveArray	A pointer to the memory that holds the vertex buffer
						data.
@param primitiveStride	The memory width of each vertex in the vertex buffer.
@return  A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::renderUserVertexBuffer(X_PRIMITIVE_TYPE pType,
											  XHandle vertexDeclaration,
											XU32 numPrimitives,void* pPrimitiveArray,
											XU32 primitiveStride)
{
	return XVertexBuffer::renderUserVertexBuffer(pType,vertexDeclaration,numPrimitives,
												pPrimitiveArray,primitiveStride);
}


/*
//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::renderUserIndexedVertexBuffer(X_PRIMITIVE_TYPE pType,
													   X_VERTEX_TYPE vType,
													   XU32 numVertices,
													   void* pVertices,
													   XU32 numIndices,
													   void* pIndices,
													   XU32 numPrimitives,
													   XU32 vertexStride)
{
	return XVertexBuffer::renderUserIndexedVertexBuffer(pType,vType,numVertices,
													   pVertices,numIndices,pIndices,
													   numPrimitives,vertexStride);
}
*/


//***************************************************************************** 
/**
Destroys a vertex buffer and its associated resources.

@param vbHandle	The handle of the vertex buffer to destroy.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::destroyVertexBuffer(XHandle vbHandle)
{
	if (vbHandle != X_INVALID_HANDLE)
	{
		return m_vertexBuffers.removeResource(vbHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Destroys all vertex buffers and all associated resources in the graphics driver.

@return None.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllVertexBuffers()
{	
	return m_vertexBuffers.destroyAllResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addVertexBufferRef(XHandle vbHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		pVertexBuffer->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Gets the length of a vertex buffer.  The length corresponds to the number
of vertices contained within the vertex buffer.

@param vbHandle	The handle of the vertex buffer for which to get the length.
@return The number of vertices in the vertex buffer.
*/ 
//***************************************************************************** 
XU32 XGraphicsMgr::getVertexBufferLength(XHandle vbHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		return pVertexBuffer->getNumVerts();
	}
	else 
		return 0;
}



//***************************************************************************** 
/**
Gets the type of a vertex buffer.

@param vbHandle	The handle of the vertex buffer for which to get the type.
@return The type of the vertex buffer.
*/
//***************************************************************************** 
/*X_VERTEX_TYPE XGraphicsMgr::getVertexBufferType(XHandle vbHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		return pVertexBuffer->getType();
	}
	else 
		return X_VT_UNKNOWN;
}*/



//***************************************************************************** 
/**
Gets the primitive type of a vertex buffer.

@param vbHandle	The handle of the vertex buffer for which to get the 
						primitive type.
@return The primitive type of the vertex buffer.
*/
//***************************************************************************** 
X_PRIMITIVE_TYPE XGraphicsMgr::getVertexBufferPrimType(XHandle vbHandle)
{
	XVertexBuffer* pVertexBuffer = (XVertexBuffer*)m_vertexBuffers.getResource(vbHandle);
	if (pVertexBuffer)
	{
		return pVertexBuffer->getPrimType();
	}
	else 
		return X_PT_UNKNOWN;
}



//***************************************************************************** 
/**
Gets the number of currently allocated vertex buffers.

@return The number of allocated vertex buffers.
*/
//***************************************************************************** 
XU32 XGraphicsMgr::getNumVertexBuffers()
{
	return m_vertexBuffers.getNumResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createConstantBuffer(XU32 sizeInBytes,XBOOL dynamic,XHandle& cbHandle)
{
	//Create a new constant buffer
	cbHandle = X_INVALID_HANDLE;
	XConstantBuffer* pConstantBuffer = NULL;
	XNew(pConstantBuffer,XConstantBuffer,m_pHeap);
	if ((m_status = pConstantBuffer->init(sizeInBytes,dynamic)) != X_OK)
	{
		XDelete(pConstantBuffer);
	}
	else
	{
		//Add to resource list
		cbHandle = m_constantBuffers.addResource(pConstantBuffer,NULL);
		X_ASSERTM(cbHandle != X_INVALID_HANDLE,"Constant buffer limit exceeded.  Increase number of constant buffer resources.");
	}

	return m_status;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyConstantBuffer(XHandle cbHandle)
{	
	//X_ASSERTM(cbHandle != X_INVALID_HANDLE,"Attempting to destroy an invalid constant buffer handle."); 
	return m_constantBuffers.removeResource(cbHandle);
}



//***************************************************************************** 
//***************************************************************************** 
void* XGraphicsMgr::lockConstantBuffer(XHandle cbHandle,X_LOCK_TYPE lockType)
{
	XConstantBuffer* pConstantBuffer = (XConstantBuffer*)m_constantBuffers.getResource(cbHandle);
	X_ASSERTM(pConstantBuffer,"Attempting to lock an invalid constant buffer handle."); 
	X_ASSERTM(pConstantBuffer->isDynamic(),"In order to lock a constant buffer, it must be created with the dynamic flag set to true.");

	return pConstantBuffer->lock(translateLockType(lockType));
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::unlockConstantBuffer(XHandle cbHandle)
{
	XConstantBuffer* pConstantBuffer = (XConstantBuffer*)m_constantBuffers.getResource(cbHandle);
	X_ASSERTM(pConstantBuffer,"Attempting to unlock an invalid constant buffer handle."); 

	return pConstantBuffer->unlock();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setConstantBufferData(XHandle cbHandle,void* pData,XU32 dataSizeInBytes)
{
	X_ASSERTM(cbHandle != X_INVALID_HANDLE,"Handle cannot be null");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	XConstantBuffer* pConstantBuffer = (XConstantBuffer*)m_constantBuffers.getResource(cbHandle);
	X_ASSERTM(!pConstantBuffer->isDynamic(),"In order update a constant buffer without locking it, it must be created with the dynamic flag set to false.");
	
	return pConstantBuffer->setData(pData,dataSizeInBytes);
}

//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getNumConstantBuffers()
{
	return m_constantBuffers.getNumResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllConstantBuffers()
{
	return m_constantBuffers.destroyAllResources();
}



//***************************************************************************** 
/**
Creates a texture and its required resources in the graphics driver. The driver
will ensure that a texture is only created once, even if multiple createTexture()
calls are made that try to load the same texture file.  For example, if you call
createTexture() with the file "c:/textures/jpg/myTex.jpg", the driver will
create the texture from this file and return an index.  In the future, if
any more calls to createTexture() are received that use the same texture
filename, "c:/textures/jpg/myTex.jpg", the function will pass back the already
created index instead of allocating more memory for a second texture object.

In fact, the driver uses the texture's filename to uniquely identify the texture.
So, in the example above, "myTex.jpg" will only be created once no matter how
many times the request is made to create it.

The driver will use reference counts to determine when a texture should
actually be destroyed. See destroyTexture().

@param pFilename	The path to the texture to load.  Check your XSP
					documentation for supported file formats.
@param texIndex	A variable to hold the index of the newly created texture.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::createTextureFromFile(XS8* pFilename,XHandle& texHandle,bool absolutePath)
{
	//Make sure we have a valid path
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	XResource* pResource = (XBaseTexture*)m_textures.findResourceByName(pFilename,texHandle);

	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		char* pFinalFilename = pFilename;
		char fullPath[X_MAX_PATH_SIZE];
		if (!absolutePath)
		{
			//Create the full path
			XOSMgr::buildPath(X_PATH_TEXTURE,pFilename,fullPath);
			pFinalFilename = fullPath;
		}

		XTexture* p2DTexture = NULL;
		XNew(p2DTexture,XTexture,m_pHeap);
		if (p2DTexture->initFromFile(pFinalFilename) == X_OK)
		{
			texHandle = m_textures.addResource(p2DTexture,pFilename);
			return X_OK;
		}
		else
		{
			X_DEBUG
			(
				XS8 buf[512];
				sprintf(buf,"Could not find/load texture %s  Substituting placeholder texture.",pFilename);
				X_WARNING(buf);
			)
			
			texHandle = m_missingTexHandle;
			addTextureRef(texHandle);

			return X_ERR_OPERATION_FAILED;
		}
	}
}


XStatus XGraphicsMgr::createTextureFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize,XHandle& texHandle)
{
	X_ASSERTM(pMemoryChunk,"Invalid texture memory chunk pointer");
	X_ASSERTM(pName,"Invalid texture name pointer");

	XResource* pResource = (XBaseTexture*)m_textures.findResourceByName(pName,texHandle);

	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		XTexture* p2DTexture = NULL;
		XNew(p2DTexture,XTexture,m_pHeap);
		if (p2DTexture->initFromMemory(pMemoryChunk,memChunkSize) == X_OK)
		{
			texHandle = m_textures.addResource(p2DTexture,pName);
			return X_OK;
		}		
		else
		{
			X_DEBUG
			(
				XS8 buf[512];
				sprintf(buf,"Could not find/load texture %s  Substituting placeholder texture.",pName);
				X_WARNING(buf);
			)
			texHandle = m_missingTexHandle;
			addTextureRef(texHandle);

			return X_ERR_OPERATION_FAILED;
		}
	}
}


//***************************************************************************** 
/**
Destroys a texture and frees all associated resources in the graphics driver.
For each texture, a reference count is maintained.  This information allows
the driver to determine exactly when a texture should be destroyed.  For
example, if you call createTexture() seven times with the filename "myTex.jpg",
only one texture is actually created in memory.  Therefore, when a 
destroyTexture() call is made to destroy this texture, it will only destroy
the texture, when seven destroyTexture() calls are made. 

@param	texIndex	The handle of the texture to destroy.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::destroyTexture(XHandle texHandle)
{
	if (texHandle != X_INVALID_HANDLE)
	{
		for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
			if (texHandle == m_texHandleCache[i])
				m_texHandleCache[i] = X_INVALID_HANDLE;
		return m_textures.removeResource(texHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addTextureRef(XHandle texHandle)
{
	XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
	if (pTexture)
	{
		pTexture->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


XStatus XGraphicsMgr::setTextureOverrideFromTexture(XHandle texStage,XHandle texHandle)
{
	X_ASSERT(texStage < X_MAX_SAMPLERS)
	X_ASSERTM(!m_texOverrides[texStage],"An override texture is currently bound to this sampler.")
	
	m_texHandleCache[texStage] = X_INVALID_HANDLE;

	ID3D10ShaderResourceView* pD3DTexture = NULL;

	//Could be a movie or a traditional texture.
	if (X_IS_MOVIE_HANDLE(texHandle))
	{
		X_MAKE_INTERNAL_HANDLE(texHandle);
		XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(texHandle);
		pD3DTexture = pMovie->getD3DTexture();
	}
	else
	{
		X_MAKE_INTERNAL_HANDLE(texHandle);
		XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
		pD3DTexture = pTexture->getD3DTexture();
	}

	//Set texture and override flag
	m_pD3DDevice->PSSetShaderResources(texStage,1,&pD3DTexture);
	m_texOverrides[texStage] = true;

	return X_OK;
}


XStatus XGraphicsMgr::setTextureOverrideFromRTA(XHandle texStage,XHandle samplerState,XHandle renderTargetArray,XU32 renderTargetIndex)
{
	X_ASSERT(texStage < X_MAX_SAMPLERS)
	X_ASSERTM(!m_texOverrides[texStage],"An override texture is currently bound to this sampler.")

	//Setting a render target as a texure is a special case and invalidates
	//the cache.  Only regular texture calls should be checked against the 
	//cache.    
	m_texHandleCache[texStage] = X_INVALID_HANDLE;
	
	ID3D10ShaderResourceView* pD3DTexture = NULL;

	XRenderTargetArray* pRenderTargetArray = (XRenderTargetArray*)m_renderTargetArrays.getResource(renderTargetArray);
	ID3D10ShaderResourceView* pSRV = pRenderTargetArray->getD3D10ShaderResourceView(renderTargetIndex);
	m_pD3DDevice->PSSetShaderResources(texStage,1,&pSRV);
	
	XVP::GraphicsMgr.setSamplerState(samplerState,texStage);

	m_texOverrides[texStage] = true;
	return X_OK;
}


XStatus XGraphicsMgr::removeTextureOverride(XHandle texStage)
{
	X_ASSERT(texStage < X_MAX_SAMPLERS);
	m_texOverrides[texStage] = false;
	return X_OK;
}

//***************************************************************************** 
/**
Sets a texture to the rendering device in the specified texture stage.  All
subsequent geometry will be rendered with the texture. 

@param texIndex The handle of the texture to set.
@param texStage The texture stage to set the new texture in.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::setTexture(XHandle texHandle,XU32 texStage)
{
	X_ASSERTM(texStage < X_MAX_SAMPLERS,"Texture stage index out of range.")
	X_ASSERTM(!m_texOverrides[texStage],"An override texture is currently bound to this sampler.")

	//Check cache first
	if (texHandle == m_texHandleCache[texStage])
		return X_OK;
	else
		m_texHandleCache[texStage] = texHandle;

	ID3D10ShaderResourceView* pD3DTexture = NULL;

	//Could be a movie or a traditional texture.
	if (X_IS_MOVIE_HANDLE(texHandle))
	{
		X_MAKE_INTERNAL_HANDLE(texHandle);
		XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(texHandle);
		pD3DTexture = pMovie->getD3DTexture();
	}
	else
	{
		X_MAKE_INTERNAL_HANDLE(texHandle);
		XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
		pD3DTexture = pTexture->getD3DTexture();
	}

	m_pD3DDevice->PSSetShaderResources(texStage,1,&pD3DTexture);
	return X_OK;
}



//***************************************************************************** 
/**
Removes a texture, in a specific texture stage, from the rendering device.

@param The texture stage from which to remove the texture.  
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::removeTexture(XU32 texStage)
{
	X_ASSERTM(texStage < X_MAX_SAMPLERS,"Invalid tex stage.");

	//@@@ WE CAN"T do this BECAUSE DEPTH STENCIL COULD HAVE BEEN SET AND THAT CLEARS THE CACHE
	//if (m_texHandleCache[texStage] == X_INVALID_HANDLE)
	//	return X_OK;

	ID3D10ShaderResourceView *const pSRVs[1] = {NULL};
	m_pD3DDevice->PSSetShaderResources(texStage,1,pSRVs);
	m_texHandleCache[texStage] = X_INVALID_HANDLE;

	//Not a valid index
	return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::removeAllTextures()
{
	ID3D10ShaderResourceView *const pSRVs[X_MAX_SAMPLERS] = {NULL};
	m_pD3DDevice->PSSetShaderResources(0,X_MAX_SAMPLERS,pSRVs);
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		m_texHandleCache[i] = X_INVALID_HANDLE;

	return X_OK;
}



//*****************************************************************************
/**
Renders a rectangular portion of a texture to the screen.

@param texIndex	The handle of the texture to render.
@param srcRect The rectangular area of the texture to render to the screen.
@param destRect The rectangular area of the screen to receive the texture data.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::renderTexture(XU32 orgWidth,XU32 orgHeight,XRect& srcRect,
									   XRect& destRect,XF32 opacity)
{	
	
	//Calculate the scale between the src rect and the dest rect
	XF32 widthScale = ((XF32)destRect.right-destRect.left)/(srcRect.right-srcRect.left);
	XF32 heightScale = ((XF32)destRect.bottom-destRect.top)/(srcRect.bottom-srcRect.top);
	XF32 sourceWidth = (XF32)srcRect.right-srcRect.left;
	XF32 sourceHeight = (XF32)srcRect.bottom-srcRect.top;
			
	//Remember, DirectX requires you to subtract .5 from transformed coordinates in order to directly
	//map texels to pixels.
	XF32 finalLeft = destRect.left - .5f;
	XF32 finalTop = destRect.top - .5f;
	XF32 finalRight = (XF32)(destRect.left + (widthScale * sourceWidth)) - .5f;
	XF32 finalBottom = (XF32)(destRect.top + (heightScale * sourceHeight)) - .5f;
	
	//At this point we have screen space coordinates, now we need to transform them
	//to clip space for the vertex shader. Invert the y coordinate.
	finalLeft = (((finalLeft/(XF32)getRenderContextWidth(m_activeRenderContext)) -.5f) * 2.0f);
	finalTop = (((finalTop/(XF32)getRenderContextHeight(m_activeRenderContext)) -.5f) * 2.0f * -1);
	finalRight = (((finalRight/(XF32)getRenderContextWidth(m_activeRenderContext)) -.5f) * 2.0f);
	finalBottom = (((finalBottom/(XF32)getRenderContextHeight(m_activeRenderContext)) -.5f) * 2.0f * -1);

	//Calculate texture coordinates from the src rect
	XF32 textureLeft = ((XF32)srcRect.left)/(XF32)orgWidth;
	XF32 textureTop = ((XF32)srcRect.top)/(XF32)orgHeight;
	XF32 textureRight = ((XF32)srcRect.right)/(XF32)orgWidth;
	XF32 textureBottom = ((XF32)srcRect.bottom)/(XF32)orgHeight;

	//Fill in the quad and render the sprite.  
	XVertexBuffer* pVB = lockVertexBuffer(m_spriteVBHandle,X_LT_DISCARD);

	XU8 alpha = (XU8)(255 * opacity);

	pVB->setPosition(0,finalLeft,finalTop,.5f);
	pVB->setUV(0,0,textureLeft,textureTop);
	pVB->setColor(0,0,255,255,255,alpha);

	pVB->setPosition(1,finalRight,finalTop,.5f);
	pVB->setUV(1,0,textureRight,textureTop);
	pVB->setColor(1,0,255,255,255,alpha);

	pVB->setPosition(2,finalLeft,finalBottom,.5f);
	pVB->setUV(2,0,textureLeft,textureBottom);
	pVB->setColor(2,0,255,255,255,alpha);

	pVB->setPosition(3,finalRight,finalBottom,.5f);
	pVB->setUV(3,0,textureRight,textureBottom);
	pVB->setColor(3,0,255,255,255,alpha);

	unlockVertexBuffer(m_spriteVBHandle);
	renderVertexBuffer(m_spriteVBHandle);
	
	return X_OK;
}



//***************************************************************************** 
/**
Gets the width of a texture.

@param texIndex The handle of the texture for which to get the width.
@return The width of the texture in pixels.
*/
//***************************************************************************** 
XU32 XGraphicsMgr::getTextureWidth(XHandle texHandle)
{
	XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
	if (pTexture)
	{
		return pTexture->getWidth();
	}
	else 
		return 0;
}



//*****************************************************************************
/**
Gets the height of a texture.

@param texIndex The handle of the texture for which to get the height.
@return The height of the texture in pixels.
*/ 
//***************************************************************************** 
XU32 XGraphicsMgr::getTextureHeight(XHandle texHandle)
{
	XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
	if (pTexture)
	{
		return pTexture->getHeight();
	}
	else 
		return 0;
}



//***************************************************************************** 
/**
Gets the color depth of a texture.  

@param texIndex	The handle of the texture for which to get the color depth.
@return The color depth of the texture.
*/
//***************************************************************************** 
XU32 XGraphicsMgr::getTextureDepth(XHandle texHandle)
{
#ifdef OLD
	XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
	if (pTexture)
	{
		return pTexture->getColorDepth();
	}
	else 
		return 0;
#else
	return 0;
#endif
}



//***************************************************************************** 
/**
Gets the filename of the texture.

@param texIndex	The handle of the texture for which to get the filename.  The
				filename does not contain path information.
@return The filename of the texture minus the path.
*/
//***************************************************************************** 
XS8* XGraphicsMgr::getTextureFilename(XHandle texHandle)
{
	XBaseTexture* pTexture = (XBaseTexture*)m_textures.getResource(texHandle);
	if (pTexture)
	{
		return pTexture->getName();
	}
	else 
		return NULL;
}



//***************************************************************************** 
/**
Gets the number of currently allocated textures.

@return The number of allocated textures.
*/
//***************************************************************************** 
XU32 XGraphicsMgr::getNumTextures()
{
	return m_textures.getNumResources();
}



//***************************************************************************** 
/**
Destroys all textures and all associated resources in the graphics driver.

@return None.
*/
//*****************************************************************************
XStatus XGraphicsMgr::destroyAllTextures()
{	
	m_textures.destroyAllResources();
	return X_OK;
}



//*****************************************************************************
/**
Creates a render state.  Render states are used to hold the rendering properties
for the rendering device.  The number of render states that may exist at any
given time is virtually infinite.  

@param matHandle	A variable to receive the handle of the newly created render
				state.  
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGraphicsMgr::createMaterial(XS8* materialName,XHandle& matHandle)
{
	//First, see if the material already exists.
	/*if (materialName)
	{
		XResource* pResource = (XMaterial*)m_materials.findResourceByName(materialName,matHandle);
		if (pResource)
		{
			pResource->incRefCount();
			return X_OK;
		}
	}*/

	//Need to create a new material
	XMaterial* pMaterial = NULL;
	XNew(pMaterial,XMaterial,m_pHeap);
	pMaterial->init();
	matHandle = m_materials.addResource(pMaterial,materialName);

	if (matHandle != X_INVALID_HANDLE)
	{
		/*X_DEBUG
		(
			char buf[10];
			std::string s = "Created Render State (INDEX = ";
			s += _itoa((XS32)matHandle,buf,10);
			s += ")";
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
		)*/
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
XStatus XGraphicsMgr::createMaterial(XS8* pMemoryChunk,XS8* materialName,
									 XHandle& matHandle)
{
	X_ASSERTM(pMemoryChunk,"Memory chunk is null");
	//First, see if the material already exists.
	if (materialName)
	{
		XResource* pResource = (XMaterial*)m_materials.findResourceByName(materialName,matHandle);
		if (pResource)
		{
			pResource->incRefCount();
			return X_OK;
		}
	}

	//Need to create a new material
	XMaterial* pMaterial = NULL;
	XNew(pMaterial,XMaterial,m_pHeap);
	pMaterial->init(pMemoryChunk);
	matHandle = m_materials.addResource(pMaterial,materialName);

	if (matHandle != X_INVALID_HANDLE)
	{
		/*X_DEBUG
		(
			char buf[10];
			std::string s = "Created Render State (INDEX = ";
			s += _itoa((XS32)matHandle,buf,10);
			s += ")";
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
		)*/
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Clones a render state.  This function will copy all render state parameters from
one render state to another including texture info.  

@param rsToClone	The handle of the render state to clone.
@param matHandle	A variable to hold the handle ofnewly created render state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGraphicsMgr::cloneMaterial(XHandle matToClone,XHandle& matHandle)
{
	XMaterial* pOrgMat = getMaterialInterface(matToClone); 
	X_ASSERTM(pOrgMat,"No such material to clone");

	createMaterial(pOrgMat->getName(),matHandle);
	XMaterial* pClonedMat = getMaterialInterface(matHandle); 

	
	return X_OK;
}



//*****************************************************************************
/**
Destroys a render state and frees its associated resources.  

@param matHandle	The handle of the render state to destroy.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGraphicsMgr::destroyMaterial(XHandle matHandle)
{
	if (matHandle != X_INVALID_HANDLE)
	{
		m_matHandleCache = X_INVALID_HANDLE;
		return m_materials.removeResource(matHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XMaterial* XGraphicsMgr::lockMaterial(XHandle matHandle)
{
	X_ASSERTM(matHandle != X_INVALID_HANDLE,"Invalid material handle.");
	
	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
	X_ASSERTM(pMaterial,"Handle does not refer to a valid material.");

	pMaterial->lock();
	return pMaterial;
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::unlockMaterial(XHandle matHandle)
{
	X_ASSERTM(matHandle != X_INVALID_HANDLE,"Invalid material handle.");
	
	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
	X_ASSERTM(pMaterial,"Handle does not refer to a valid material.");

	return pMaterial->unlock();
}



//*****************************************************************************
//*****************************************************************************
XHandle XGraphicsMgr::getMaterial(XS8* materialName)
{
	if (!materialName)
		return X_ERR_INVALID_PARAMS;

	XHandle matHandle = X_INVALID_HANDLE;
	m_materials.findResourceByName(materialName,matHandle);
	return matHandle;
}



//*****************************************************************************
/**
Destroys all existing render states and all associated resources.

@return None.
*/
//*****************************************************************************
XStatus XGraphicsMgr::destroyAllMaterials()
{
	return m_materials.destroyAllResources();
}



//*****************************************************************************
//*****************************************************************************
XS8* XGraphicsMgr::getMaterialName(XHandle matHandle)
{
	if (matHandle != X_INVALID_HANDLE)
	{
		XResource* pResource = m_materials.getResource(matHandle);
		if (pResource)
			return pResource->getName();
	}

	return NULL;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addMaterialRef(XHandle matHandle)
{
	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
	if (pMaterial)
	{
		pMaterial->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Sets a render state to the rendering device.  The properties of the render
state are applied to the rendering device and all subsequent geometry is rendered
with the render state settings.

@param matHandle	The handle of the render state to set to the rendering device.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XGraphicsMgr::applyMaterial(XHandle matHandle)
{
	//Check cache first
	if (matHandle == m_matHandleCache)
		return X_OK;
	else
		m_matHandleCache = matHandle;

	return applyMaterial((XMaterial*)m_materials.getResource(matHandle));
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::applyMaterial(XMaterial* pMaterial)
{
	if (pMaterial)
		return pMaterial->apply();
	else
		return X_ERR_INVALID_PARAMS;
}



//*****************************************************************************
/**
Gets the interface to a render state.  Using the interface, you can set 
properties of the render state.

@param matHandle	The handle of the render state for which to get the interface.
@return A pointer to the interface for the render state.
*/
//*****************************************************************************
XMaterial* XGraphicsMgr::getMaterialInterface(XHandle matHandle)
{
	return (XMaterial*)m_materials.getResource(matHandle);
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::renderEvent(XRenderEvent* pRenderEvent)
{
	X_ASSERTM(pRenderEvent,"Invalid render event");

	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(pRenderEvent->materialHandle);
	X_ASSERTM(pMaterial,"Invalid material found during render event");

	return pMaterial->render(pRenderEvent);
}

	

//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addScene(XHandle renderContext,XU32 rcLayer,XScene* pScene,XScene* pDependentScene)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid render context");
	X_ASSERTM(pScene,"Invalid scene");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	pScene->setRenderContext(renderContext);
	if (pDependentScene)
		return pRenderContext->insertScene(rcLayer,pDependentScene,pScene);
	else
		return pRenderContext->addScene(rcLayer,pScene);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::removeScene(XHandle renderContext,XScene* pScene)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid render context");
	X_ASSERTM(pScene,"Invalid scene");
	
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->removeScene(pScene);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::removeAllScenes(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid render context");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->removeAllScenes();
}


	
//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setViewport(XViewport& viewport)
{
	//Set the viewport
	D3D10_VIEWPORT vp;
    vp.Width = viewport.width;
    vp.Height = viewport.height;
    vp.MinDepth = viewport.minDepth;
    vp.MaxDepth = viewport.maxDepth;
    vp.TopLeftX = viewport.topLeftX;
    vp.TopLeftY = viewport.topLeftY;
    m_pD3DDevice->RSSetViewports(1,&vp);

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setScissorRect(XRect& scissorRect)
{
	//Setup scissor rect
	D3D10_RECT rect;
	rect.left = scissorRect.left;
	rect.right = scissorRect.right;
	rect.top = scissorRect.top;
	rect.bottom = scissorRect.bottom;
	m_pD3DDevice->RSSetScissorRects(1,&rect);

	return X_OK;
}


	

//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setScissorRect(XViewport& viewport)
{
	//Setup scissor rect
	D3D10_RECT rect;
	rect.left = viewport.topLeftX;
	rect.right = viewport.width - viewport.topLeftX;
	rect.top = viewport.topLeftY;
	rect.bottom = viewport.height - viewport.topLeftY;
	m_pD3DDevice->RSSetScissorRects(1,&rect);

	return X_OK;
}



//***************************************************************************** 
/**
*/
//*****************************************************************************
XStatus XGraphicsMgr::createVertexShader(XS8* pFilename,XS8* pEntryFunctionName,XHandle& vsHandle)
{
	X_ASSERTM(pFilename && pEntryFunctionName,"Null filename or entry function name.");

	XS8 fullPath[X_MAX_PATH_SIZE];
	XS8 VSOFilenameOnly[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_SHADER,pFilename,fullPath);
	XS8* pFilenameOnly = XOSMgr::getPathFilename(fullPath);
	XS8* pFinalVSOFilename = pFilenameOnly;
	
	//Fixup the filename to ensure it ends in ".vso"
	if (!strstr(pFilenameOnly,".vso"))
	{
		strcpy(VSOFilenameOnly,pFilenameOnly);
		strcat(VSOFilenameOnly,".vso");
		pFinalVSOFilename = VSOFilenameOnly;
	}
	
	XResource* pResource = (XBaseVertexShader*)m_vertexShaders.findResourceByName(pFinalVSOFilename,vsHandle);
	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		//We need to load the shader from a file  
		XHLSLVertexShader* pShader = NULL;
		XNew(pShader,XHLSLVertexShader,m_pHeap);
		
		if (pShader->initFromSourceCode(fullPath,pEntryFunctionName) == X_OK)
			vsHandle = m_vertexShaders.addResource(pShader,pFinalVSOFilename);
		else
			XDelete(pShader);
		
		if (vsHandle != X_INVALID_HANDLE)
			return X_OK;
		else
			return X_ERR_OPERATION_FAILED;
	}
}


XStatus XGraphicsMgr::createVertexShaderFromString(XS8* pShaderName,XS8* pBuffer,XU32 bufferLen,XS8* pEntryFunctionName,XHandle& vsHandle)
{
	X_ASSERT(pBuffer && pEntryFunctionName);
	X_ASSERT(bufferLen);
	X_ASSERT(pShaderName);

	XS8 VSOFilenameOnly[X_MAX_PATH_SIZE];
	strcpy(VSOFilenameOnly,pShaderName);

	//Fixup the name to ensure it ends in ".vso"
	if (!strstr(VSOFilenameOnly,".vso"))
		strcat(VSOFilenameOnly,".vso");
	
	XResource* pResource = (XBaseVertexShader*)m_vertexShaders.findResourceByName(VSOFilenameOnly,vsHandle);
	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		//We need to compile the shader from the string
		XHLSLVertexShader* pShader = NULL;
		XNew(pShader,XHLSLVertexShader,m_pHeap);
		
		if (pShader->initFromSourceCodeString(pBuffer,bufferLen,pEntryFunctionName) == X_OK)
			vsHandle = m_vertexShaders.addResource(pShader,VSOFilenameOnly);
		else
			XDelete(pShader);
		
		if (vsHandle != X_INVALID_HANDLE)
			return X_OK;
		else
			return X_ERR_OPERATION_FAILED;
	}
}


//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyVertexShader(XHandle vsHandle)
{
	if (vsHandle != X_INVALID_HANDLE)
	{
		m_vsHandleCache = X_INVALID_HANDLE;
		return m_vertexShaders.removeResource(vsHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addVertexShaderRef(XHandle vsHandle)
{
	XBaseVertexShader* pVertexShader = (XBaseVertexShader*)m_vertexShaders.getResource(vsHandle);
	if (pVertexShader)
	{
		pVertexShader->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XBaseVertexShader* XGraphicsMgr::getVertexShaderByHandle(XHandle vsHandle)
{
	return (XBaseVertexShader*)m_vertexShaders.getResource(vsHandle);
}




//***************************************************************************** 
//***************************************************************************** 
XHandle XGraphicsMgr::getVertexShaderByName(XS8* pShaderName)
{
	XHandle vsHandle = X_INVALID_HANDLE;
	XS8 vsoName[X_MAX_PATH_SIZE];
	strcpy(vsoName,pShaderName);
	strcat(vsoName,".vso");
	
	m_vertexShaders.findResourceByName(vsoName,vsHandle);

	return vsHandle;
}

XHandle XGraphicsMgr::getBoundVertexShader()
{
	return m_vsHandleCache;
}


//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setVertexShader(XHandle vsHandle)
{
	//Check cache first
	if (vsHandle == m_vsHandleCache)
		return X_OK;
	else
		m_vsHandleCache = vsHandle;

	XBaseVertexShader* pShader = (XBaseVertexShader*)m_vertexShaders.getResource(vsHandle);
	if (pShader)
	{
		m_pD3DDevice->VSSetShader(pShader->getD3DVertexShader());
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setVSConstantBuffer(XU32 slotIndex,XHandle cbHandle)
{
	XConstantBuffer* pConstantBuffer = (XConstantBuffer*)m_constantBuffers.getResource(cbHandle);
	if (pConstantBuffer)
	{
		X_ASSERTM(!pConstantBuffer->isLocked(),"A VS constant buffer is being set while still locked");
		ID3D10Buffer* pBuf = pConstantBuffer->getD3DConstantBuffer();
		m_pD3DDevice->VSSetConstantBuffers(slotIndex,1,&pBuf);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::removeVertexShader()
{
	m_pD3DDevice->VSSetShader(NULL);
	m_vsHandleCache = X_INVALID_HANDLE;
	return X_OK;
}


//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllVertexShaders()
{
	m_vertexShaders.destroyAllResources();
	return X_OK;
}


//***************************************************************************** 
/**
*/
//*****************************************************************************
XStatus XGraphicsMgr::createGeometryShader(XS8* pFilename,XS8* pEntryFunctionName,XHandle& gsHandle)
{
	X_ASSERTM(pFilename && pEntryFunctionName,"Null filename or entry function name.");

	XS8 fullPath[X_MAX_PATH_SIZE];
	XS8 GSOFilenameOnly[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_SHADER,pFilename,fullPath);
	XS8* pFilenameOnly = XOSMgr::getPathFilename(fullPath);
	XS8* pFinalGSOFilename = pFilenameOnly;
	
	//Fixup the filename to ensure it ends in ".gso"
	if (!strstr(pFilenameOnly,".gso"))
	{
		strcpy(GSOFilenameOnly,pFilenameOnly);
		strcat(GSOFilenameOnly,".gso");
		pFinalGSOFilename = GSOFilenameOnly;
	}
	
	XResource* pResource = (XBaseGeometryShader*)m_geometryShaders.findResourceByName(pFinalGSOFilename,gsHandle);
	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		//We need to load the shader from a file  
		XHLSLGeometryShader* pShader = NULL;
		XNew(pShader,XHLSLGeometryShader,m_pHeap);
		
		if (pShader->initFromSourceCode(fullPath,pEntryFunctionName) == X_OK)
			gsHandle = m_geometryShaders.addResource(pShader,pFinalGSOFilename);
		else
			XDelete(pShader);
		
		if (gsHandle != X_INVALID_HANDLE)
			return X_OK;
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyGeometryShader(XHandle gsHandle)
{
	if (gsHandle != X_INVALID_HANDLE)
	{
		m_gsHandleCache = X_INVALID_HANDLE;
		return m_geometryShaders.removeResource(gsHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addGeometryShaderRef(XHandle gsHandle)
{
	XBaseGeometryShader* pGeometryShader = (XBaseGeometryShader*)m_geometryShaders.getResource(gsHandle);
	if (pGeometryShader)
	{
		pGeometryShader->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


//***************************************************************************** 
//***************************************************************************** 
XBaseGeometryShader* XGraphicsMgr::getGeometryShaderByName(XS8* pShaderName)
{
	XHandle gsHandle = X_INVALID_HANDLE;
	XS8 gsoName[X_MAX_PATH_SIZE];
	strcpy(gsoName,pShaderName);
	strcat(gsoName,".gso");
	return (XBaseGeometryShader*)m_geometryShaders.findResourceByName(gsoName,gsHandle);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setGeometryShader(XHandle gsHandle)
{
	//Check cache first
	if (gsHandle == m_gsHandleCache)
		return X_OK;
	else
		m_gsHandleCache = gsHandle;

	XBaseGeometryShader* pShader = (XBaseGeometryShader*)m_geometryShaders.getResource(gsHandle);
	if (pShader)
	{
		m_pD3DDevice->GSSetShader(pShader->getD3DGeometryShader());
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllGeometryShaders()
{
	m_geometryShaders.destroyAllResources();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::removeGeometryShader()
{
	m_pD3DDevice->GSSetShader(NULL);
	m_gsHandleCache = X_INVALID_HANDLE;
	return X_OK;
}

//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setGSConstantBuffer(XU32 slotIndex,XHandle cbHandle)
{
	XConstantBuffer* pConstantBuffer = (XConstantBuffer*)m_constantBuffers.getResource(cbHandle);
	if (pConstantBuffer)
	{
		//Check to make sure we don't still have the buffer locked
		X_ASSERTM(!pConstantBuffer->isLocked(),"A GS constant buffer is being set while still locked");
		ID3D10Buffer* pBuf = pConstantBuffer->getD3DConstantBuffer();
		m_pD3DDevice->GSSetConstantBuffers(slotIndex,1,&pBuf);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}




//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createPixelShader(XS8* pFilename,XS8* pEntryFunctionName,XHandle& psHandle)
{
	X_ASSERTM(pFilename && pEntryFunctionName,"Null filename or entry function name.");

	XS8 fullPath[X_MAX_PATH_SIZE];
	XS8 PSOFilenameOnly[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_SHADER,pFilename,fullPath);
	XS8* pFilenameOnly = XOSMgr::getPathFilename(fullPath);
	XS8* pFinalPSOFilename = pFilenameOnly;
	
	//Fixup the filename to ensure it ends in ".pso"
	if (!strstr(pFilenameOnly,".pso"))
	{
		strcpy(PSOFilenameOnly,pFilenameOnly);
		strcat(PSOFilenameOnly,".pso");
		pFinalPSOFilename = PSOFilenameOnly;
	}
	
	XResource* pResource = (XBasePixelShader*)m_pixelShaders.findResourceByName(pFinalPSOFilename,psHandle);
	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		//We need to load the shader from a file  
		XHLSLPixelShader* pShader = NULL;
		XNew(pShader,XHLSLPixelShader,m_pHeap);
		
		if (pShader->initFromSourceCode(fullPath,pEntryFunctionName) == X_OK)
			psHandle = m_pixelShaders.addResource(pShader,pFinalPSOFilename);
		else
			XDelete(pShader);
		
		if (psHandle != X_INVALID_HANDLE)
			return X_OK;
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyPixelShader(XHandle psHandle)
{
	if (psHandle != X_INVALID_HANDLE)
	{
		m_psHandleCache = X_INVALID_HANDLE;
		return m_pixelShaders.removeResource(psHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addPixelShaderRef(XHandle psHandle)
{
	XBasePixelShader* pPixelShader = (XBasePixelShader*)m_pixelShaders.getResource(psHandle);
	if (pPixelShader)
	{
		pPixelShader->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


//***************************************************************************** 
//***************************************************************************** 
XHandle XGraphicsMgr::getPixelShaderByName(XS8* pShaderName)
{
	XHandle psHandle = X_INVALID_HANDLE;
	XS8 psoName[X_MAX_PATH_SIZE];
	strcpy(psoName,pShaderName);
	strcat(psoName,".pso");
	m_pixelShaders.findResourceByName(psoName,psHandle);
	return psHandle;
}

//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setPSConstantBuffer(XU32 slotIndex,XHandle cbHandle)
{
	XConstantBuffer* pConstantBuffer = (XConstantBuffer*)m_constantBuffers.getResource(cbHandle);
	if (pConstantBuffer)
	{
		//Check to make sure we don't still have the buffer locked
		X_ASSERTM(!pConstantBuffer->isLocked(),"A PS constant buffer is being set while still locked");
		ID3D10Buffer* pBuf = pConstantBuffer->getD3DConstantBuffer();
		m_pD3DDevice->PSSetConstantBuffers(slotIndex,1,&pBuf);
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}


XHandle XGraphicsMgr::getBoundPixelShader()
{
	return m_psHandleCache;
}

//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setPixelShader(XHandle psHandle)
{
	//Check cache first
	if (psHandle == m_psHandleCache)
		return X_OK;
	else
		m_psHandleCache = psHandle;

	XBasePixelShader* pShader = (XBasePixelShader*)m_pixelShaders.getResource(psHandle);
	if (pShader)
	{
		m_pD3DDevice->PSSetShader(pShader->getD3DPixelShader());
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::removePixelShader()
{
	m_pD3DDevice->PSSetShader(NULL);
	m_psHandleCache = X_INVALID_HANDLE;
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllPixelShaders()
{
	m_pixelShaders.destroyAllResources();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createDepthStencilState(XDepthStencilDesc& desc,XHandle& dssHandle)
{
	//Create the depth stencil state
	dssHandle = X_INVALID_HANDLE;
	XDepthStencilState* pDepthStencilState = NULL;
	XNew(pDepthStencilState,XDepthStencilState,m_pHeap);
	m_status = pDepthStencilState->init(desc);
	X_ASSERTM(m_status == X_OK,"Could not create depth stencil state.");

	//Add to resource list
	dssHandle = m_depthStencilStates.addResource(pDepthStencilState,NULL);
	X_ASSERTM(dssHandle != X_INVALID_HANDLE,"Could not add depth stencil state to resource list.");
	
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyDepthStencilState(XHandle dssHandle)
{
	//X_ASSERTM(dssHandle != X_INVALID_HANDLE,"Invalid handle.");
	return m_depthStencilStates.removeResource(dssHandle);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setDepthStencilState(XHandle dssHandle)
{
	X_ASSERTM(dssHandle != X_INVALID_HANDLE,"Invalid handle.");

	XDepthStencilState* pDepthStencilState = (XDepthStencilState*)m_depthStencilStates.getResource(dssHandle);
	X_ASSERTM(pDepthStencilState,"Handle does not refer to a valid depth stencil state.");

	return pDepthStencilState->set();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllDepthStencilStates()
{
	return m_depthStencilStates.destroyAllResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addDepthStencilStateRef(XHandle dssHandle)
{
	X_ASSERTM(dssHandle != X_INVALID_HANDLE,"Invalid handle.");
	
	XDepthStencilState* pDepthStencilState = (XDepthStencilState*)m_depthStencilStates.getResource(dssHandle);
	X_ASSERTM(pDepthStencilState,"Handle does not refer to a valid depth stencil state.");
	
	pDepthStencilState->incRefCount();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getNumDepthStencilStates()
{
	return m_depthStencilStates.getNumResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createRasterizerState(XRasterizerDesc& desc,XHandle& rsHandle)
{
	//Create the rasterizer state
	rsHandle = X_INVALID_HANDLE;
	XRasterizerState* pRasterizerState = NULL;
	XNew(pRasterizerState,XRasterizerState,m_pHeap);
	m_status = pRasterizerState->init(desc);
	X_ASSERTM(m_status == X_OK,"Could not create rasterizer state.");

	//Add to resource list
	rsHandle = m_rasterizerStates.addResource(pRasterizerState,NULL);
	X_ASSERTM(rsHandle != X_INVALID_HANDLE,"Could not add rasterizer state to resource list.");
	
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyRasterizerState(XHandle rsHandle)
{
	//X_ASSERTM(rsHandle != X_INVALID_HANDLE,"Invalid handle.");
	return m_rasterizerStates.removeResource(rsHandle);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setRasterizerState(XHandle rsHandle)
{
	X_ASSERTM(rsHandle != X_INVALID_HANDLE,"Invalid handle.");

	XRasterizerState* pRasterizerState = (XRasterizerState*)m_rasterizerStates.getResource(rsHandle);
	X_ASSERTM(pRasterizerState,"Handle does not refer to a valid rasterizer state.");

	pRasterizerState->set();

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllRasterizerStates()
{
	return m_rasterizerStates.destroyAllResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addRasterizerStateRef(XHandle rsHandle)
{
	X_ASSERTM(rsHandle != X_INVALID_HANDLE,"Invalid handle.");
	
	XSamplerState* pSamplerState = (XSamplerState*)m_rasterizerStates.getResource(rsHandle);
	X_ASSERTM(pSamplerState,"Handle does not refer to a valid rasterizer state.");
	
	pSamplerState->incRefCount();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getNumRasterizerStates()
{
	return m_rasterizerStates.getNumResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createBlendState(XBlendDesc& desc,XHandle& bsHandle)
{
	//Create the blend state
	bsHandle = X_INVALID_HANDLE;
	XBlendState* pBlendState = NULL;
	XNew(pBlendState,XBlendState,m_pHeap);
	m_status = pBlendState->init(desc);
	X_ASSERTM(m_status == X_OK,"Could not create blend state.");

	//Add to resource list
	bsHandle = m_blendStates.addResource(pBlendState,NULL);
	X_ASSERTM(bsHandle != X_INVALID_HANDLE,"Could not add blend state to resource list.");
	
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyBlendState(XHandle bsHandle)
{
	//X_ASSERTM(bsHandle != X_INVALID_HANDLE,"Invalid handle.");
	return m_blendStates.removeResource(bsHandle);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setBlendState(XHandle bsHandle)
{
	X_ASSERTM(bsHandle != X_INVALID_HANDLE,"Invalid handle.");

	XBlendState* pBlendState = (XBlendState*)m_blendStates.getResource(bsHandle);
	X_ASSERTM(pBlendState,"Handle does not refer to a valid blend state.");

	pBlendState->set();

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllBlendStates()
{
	return m_blendStates.destroyAllResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addBlendStateRef(XHandle bsHandle)
{
	X_ASSERTM(bsHandle != X_INVALID_HANDLE,"Invalid handle.");
	
	XBlendState* pBlendState = (XBlendState*)m_blendStates.getResource(bsHandle);
	X_ASSERTM(pBlendState,"Handle does not refer to a valid blend state.");
	
	pBlendState->incRefCount();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getNumBlendStates()
{
	return m_blendStates.getNumResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::createSamplerState(XSamplerDesc& desc,XHandle& ssHandle)
{
	//Create the sampler state
	ssHandle = X_INVALID_HANDLE;
	XSamplerState* pSamplerState = NULL;
	XNew(pSamplerState,XSamplerState,m_pHeap);
	m_status = pSamplerState->init(desc);
	X_ASSERTM(m_status == X_OK,"Could not create sampler state.");

	//Add to resource list
	ssHandle = m_samplerStates.addResource(pSamplerState,NULL);
	X_ASSERTM(ssHandle != X_INVALID_HANDLE,"Could not add sampler state to resource list.");
	
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroySamplerState(XHandle ssHandle)
{
	//X_ASSERTM(ssHandle != X_INVALID_HANDLE,"Invalid handle.");
	return m_samplerStates.removeResource(ssHandle);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::setSamplerState(XHandle ssHandle,XU32 texStage)
{
	X_ASSERTM(ssHandle != X_INVALID_HANDLE,"Invalid handle.");
	X_ASSERTM(texStage < X_MAX_SAMPLERS,"Texture stage index out of range.");
	X_ASSERTM(!m_texOverrides[texStage],"An override texture is currently bound to this sampler.")

	XSamplerState* pSamplerState = (XSamplerState*)m_samplerStates.getResource(ssHandle);
	X_ASSERTM(pSamplerState,"Handle does not refer to a valid sampler state.");

	return pSamplerState->set(texStage);
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::destroyAllSamplerStates()
{
	return m_samplerStates.destroyAllResources();
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addSamplerStateRef(XHandle ssHandle)
{
	X_ASSERTM(ssHandle != X_INVALID_HANDLE,"Invalid handle.");
	
	XSamplerState* pSamplerState = (XSamplerState*)m_samplerStates.getResource(ssHandle);
	X_ASSERTM(pSamplerState,"Handle does not refer to a valid sampler state.");
	
	pSamplerState->incRefCount();
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getNumSamplerStates()
{
	return m_samplerStates.getNumResources();
}



//***************************************************************************** 
/**
Creates a movie and its required resources in the graphics driver. Check your
XSP documentation for supported movie formats.

@param pFilename	The path to the movie file.  
@param movieHandle	A variable to receive the index of the newly created movie.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::createMovie(XS8* pFilename,XHandle& movieHandle)
{

	//Make sure we have a valid path
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	char fullPath[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_MOVIE,pFilename,fullPath);

	XDSMovie* pMovie = NULL;
	XNew(pMovie,XDSMovie(),XOSMgr::getDefaultHeap())
	if (pMovie->init(fullPath) == X_OK)
		movieHandle = m_movies.addResource(pMovie,NULL);
	else
	{
		movieHandle = X_INVALID_HANDLE;
		return X_ERR_OPERATION_FAILED;
	}

	if (movieHandle != X_INVALID_HANDLE)
	{
		X_MAKE_MOVIE_HANDLE(movieHandle);
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;

	/*//Make sure we have a valid path
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	//We need to find the first "empty" spot in the array
	//and create a movie there.
	for (XU32 i = 1;i < m_maxTextures;i++)
		if (m_textureArray[i] == NULL)
		{	
			//Allocate the new movie.
			GraphicsMovie* pMovie = new GraphicsMovie;
			if (!pMovie)
			{
				destroyMovie(i);
				return X_ERR_MEMORY_EXHAUSTED;
			}
			
			//Copy over the filename.
			pMovie->m_pFilename = new XU8[strlen(pFilename)+1];
			if (!pMovie->m_pFilename)
			{
				destroyMovie(i);
				return X_ERR_MEMORY_EXHAUSTED;
			}
			strcpy(pMovie->m_pFilename,pFilename);
			
			pMovie->m_refCount = 1;
			
			//Load the movie.
			if (!pMovie->Load(m_pD3DDevice,pFilename))
			{
				destroyMovie(i);
				return X_ERR_MEMORY_EXHAUSTED;
			}

			m_textureArray[i] = pMovie;
			m_movieArray.push_back(i);
			
			//Now that we have created a new texture, fill in the index
			//and return.
			movieHandle = i;
			++m_numAllocatedTextures;
			return X_OK;
		}
	
	//If we make it here then we have some bad news.  We are maxed out
	//so re-dimension the list.
	GraphicsTexture** tempArray = new GraphicsTexture*[m_maxTextures + X_TL_SIZE];
	if (!tempArray)
		return X_ERR_MEMORY_EXHAUSTED;

	for (XU32 ctr = 0;ctr < (m_maxTextures + X_TL_SIZE);ctr++)
		tempArray[ctr] = NULL;
	memcpy(tempArray,m_textureArray,m_maxTextures*sizeof(GraphicsTexture*));
	XDeleteArray(m_textureArray);
	m_textureArray = tempArray;	
	m_maxTextures += X_TL_SIZE;	
	return createMovie(pFilename,movieHandle);

	return X_OK;*/
}



//*****************************************************************************  
/**
Destroys a movie and its associated resources.

@param movieHandle	The handle of the movie to destroy.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::destroyMovie(XHandle movieHandle)
{
	if (movieHandle != X_INVALID_HANDLE)
	{
		return m_movies.removeResource(movieHandle & 0x7FFFFFFF);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;

	/*//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return X_ERR_INVALID_PARAMS;

	//Let's take a look at the GraphicsMovie object in the passed in position.
	//If it is NULL, then bail; however, if there is a movie object
	//present, then we need to look at the reference count to determine
	//the next step.  We subtract 1 from the reference count and if it
	//becomes 0 then we must delete it, otherwise someone is still using
	//it.  If this is the case then we just decrement and return.
	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			freeMovieResources((GraphicsMovie*)m_textureArray[movieHandle]);			
			m_textureArray[movieHandle] = NULL;
			m_movieArray.remove(movieHandle);

			return X_OK;
		}

	//If we make it here then there was an invalid index passed in.
	return X_ERR_NO_SUCH_OBJECT;*/
}



//***************************************************************************** 
/**
Destroys all movies and all associated resources in the graphics driver.

@return None.
*/
//*****************************************************************************
XStatus XGraphicsMgr::destroyAllMovies()
{	

	return m_movies.destroyAllResources();
	/*//Movies are stored in the texture array.  We use a special array to hold
	//the indicies where movies exist.  Here we use the same code as in 
	//"destroyMovie".  The indices in movieArray cannot be deleted until we
	//have been through the entire list.
	list<XU32>::iterator i;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
	{
		//Release all COM objects
		freeMovieResources((GraphicsMovie*)m_textureArray[(*i)]);
		m_textureArray[(*i)] = NULL;
	}

	//Now it's safe to clear the movie array.
	m_movieArray.clear();
	return X_OK;*/
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::addMovieRef(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle);
	if (pMovie)
	{
		pMovie->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//***************************************************************************** 
/**
Plays a movie.  Note that calling this function does not cause the movie
to be rendered to the screen.  Instead, it causes the movie's audio and video
to be rendered to its private resources.  In order to render the movie to the
screen, you must use the movie index as a texture index and supply geometry
to render the movie on.

@param movieHandle	The handle of the movie to begin playing.
@param playCount The number of times to loop the movie.  -1 to loop forever.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::playMovie(XHandle movieHandle,int playCount)
{
	if (playCount == 0)
		return X_ERR_INVALID_PARAMS;

	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->play(playCount);
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
	/*//Validate playCount
	if (playCount == 0)
		return X_OK;

	//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return X_ERR_INVALID_PARAMS;

	//First check the index of the movie to make sure that it is valid.
	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			GraphicsMovie* pMovie = (GraphicsMovie*)m_textureArray[movieHandle];
			pMovie->playCount = playCount;
			pMovie->GotoTime(0);
			pMovie->Play();
			return X_OK;
		}

	//Not a valid index
	return X_ERR_NO_SUCH_OBJECT;*/
}



//*****************************************************************************
/**
Pauses playback of a movie.

@param movieHandle the index of the movie to pause.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::pauseMovie(XHandle movieHandle)
{

	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->pause();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
	/*
	return stopMovie(movieHandle); 
	return X_OK;*/
}



//***************************************************************************** 
/**
Resumes playback of a paused movie.

@param movieHandle	The handle of the movie to resume.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::resumeMovie(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->resume();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
	//Make sure this is a valid movie.
/*	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return X_ERR_INVALID_PARAMS;

	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			((GraphicsMovie*)m_textureArray[movieHandle])->Play();
			return X_OK;
		}

	//Not a valid index
	return X_ERR_NO_SUCH_OBJECT;*/
}



//***************************************************************************** 
/**
Stops playback of a movie.

@param movieHandle	The handle of the movie to stop.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::stopMovie(XHandle movieHandle)
{
	return X_OK;
	/*XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->stop();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;*/
	/*//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return X_ERR_INVALID_PARAMS;
	
	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			((GraphicsMovie*)m_textureArray[movieHandle])->Stop();
			return X_OK;
		}

	//Not a valid index
	return X_ERR_NO_SUCH_OBJECT;*/
}



//*****************************************************************************
/**
Immediately seeks to a specific time in the movie.

@param movieHandle The handle of the movie to seek.
@param milliseconds	The time, in milliseconds, to seek to.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::setMovieTime(XHandle movieHandle,XU32 milliseconds)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->setTime(milliseconds);
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
/*	//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return X_ERR_INVALID_PARAMS;

	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			((GraphicsMovie*)m_textureArray[movieHandle])->GotoTime(milliseconds/1000);
			return X_OK;
		}

	//Not a valid index
	return X_ERR_NO_SUCH_OBJECT;*/
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getMovieTime(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->getTime();
	}
	else 
		return 0;
}



//*****************************************************************************
/**
Sets the playback count of a movie.  

@param movieHandle	The handle of the movie for which to set the playback count.
@param playCount	The number of times to loop the movie.  -1 to loop forever.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::setMoviePlayCount(XHandle movieHandle,int playCount)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->setPlayCount(playCount);
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
	//Validate playCount
/*	if (playCount == 0)
		return X_OK;

	//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return X_ERR_INVALID_PARAMS;

	//First check the index of the movie to make sure that it is valid.
	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			GraphicsMovie* pMovie = (GraphicsMovie*)m_textureArray[movieHandle];
			pMovie->playCount = playCount;
			return X_OK;
		}

	//Not a valid index
	return X_ERR_NO_SUCH_OBJECT;*/
}



//***************************************************************************** 
//***************************************************************************** 
XS32 XGraphicsMgr::getMoviePlayCount(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->getPlayCount();
	}
	else 
		return 0;
}


//*****************************************************************************
//*****************************************************************************
XU32 XGraphicsMgr::getMovieDuration(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->getDuration();
	}
	else 
		return 0;
}



//*****************************************************************************
/**
Determines if a movie is currently playing.

@param movieHandle	The handle of the movie to query.
@return True if the movie is playing, false otherwise.
*/
//***************************************************************************** 
XBOOL XGraphicsMgr::isMoviePlaying(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->isPlaying();
	}
	else 
		return false;
	/*
	//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return false;

	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			GraphicsMovie* pMovie = (GraphicsMovie*)m_textureArray[movieHandle];
			return pMovie->isPlaying;
		}*/

	//Not a valid index
	return false;
}



//*****************************************************************************
//*****************************************************************************
XU32 XGraphicsMgr::getMovieWidth(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->getWidth();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
/*	//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return 0;

	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			GraphicsMovie* pMovie = (GraphicsMovie*)m_textureArray[movieHandle];
			return pMovie->m_info.Width;
		}

	return 0;*/
}



//*****************************************************************************
//*****************************************************************************
XU32 XGraphicsMgr::getMovieHeight(XHandle movieHandle)
{
	XBaseMovie* pMovie = (XBaseMovie*)m_movies.getResource(movieHandle & 0x7FFFFFFF);
	if (pMovie)
	{
		return pMovie->getHeight();
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
/*	//Make sure this is a valid movie.
	list<XU32>::iterator i;
	XF32 found = false;
	for (i = m_movieArray.begin();i != m_movieArray.end();++i)
		if ((*i) == movieHandle)
			found = true;
	if (!found)
		return 0;

	if ((movieHandle > 0) && (movieHandle <= m_maxTextures-1))
		if (m_textureArray[movieHandle] != NULL)
		{
			GraphicsMovie* pMovie = (GraphicsMovie*)m_textureArray[movieHandle];
			return pMovie->m_info.Height;
		}

	return 0;*/
}




//*****************************************************************************
/**
Updates the graphics driver.  This routine is called by X-VP during
an update sequence and gives the driver an opportunity to udpate any internal,
time dependent data.

@param dt	The amount of time, in seconds, that has passed since the last
					update.
@return A return code of type XStatus.
*/  
//***************************************************************************** 
XStatus XGraphicsMgr::update(XF32 dt)
{
	//Update movies
	XMemBlockList<XBaseMovie*>::iterator i(XBaseMovie::m_movieList.begin());
	while (i.isValid())
	{
		i.getValue()->update();
		i.getNext();
	}

	//Run through render context resources. If a valid render context is found, update it.
	XU32 numRenderContexts = m_renderContexts.getCapacity();
	XRenderContext* pRenderContext = NULL;
	for (XU32 i = 1;i < numRenderContexts;++i)
	{
		pRenderContext = (XRenderContext*)m_renderContexts.getResource(i);
		if (pRenderContext)
		{
			m_activeRenderContext = i;
			pRenderContext->update(dt);
		}
	}

	return X_OK;
}

	

//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getRenderContextWidth(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->getWidth();
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XGraphicsMgr::getRenderContextHeight(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->getHeight();
}



//***************************************************************************** 
/**
Sets the clear color.  The clear color is the color used to clear the frame
buffer at the beginning of each render sequence.

@param color	The color to clear the frame buffer with at the beginning of
				each render sequence.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::setRenderContextClearColor(XHandle renderContext,XColor& color)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->setClearColor(color);
}



//***************************************************************************** 
/**
Gets the clear color.  The clear color is the color used to clear the frame
buffer at the beginning of each render sequence.

@param color	A variable to hold the clear color.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::getRenderContextClearColor(XHandle renderContext,XColor& color)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->getClearColor(color);
}



//***************************************************************************** 
/**
Sets the depth buffer clear value.  This value is used to clear the depth buffer
at the beginning of each render sequence.

@param value	The value to clear the depth buffer with at the beginning of
				each render sequence.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::setRenderContextDepthClearValue(XHandle renderContext,XF32 value)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->setDepthClearValue(value);
}



//***************************************************************************** 
/**
Gets the depth buffer clear value.  This value is used to clear the depth buffer
at the beginning of each render sequence.

@return The depth buffer clear value.
*/
//***************************************************************************** 
XF32 XGraphicsMgr::getRenderContextDepthClearValue(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->getDepthClearValue();
}



//***************************************************************************** 
/**
Sets the stencil buffer clear value.  This value is used to clear the stencil buffer
at the beginning of each render sequence.

@param value	The value to clear the stencil buffer with at the beginning of
				each render sequence.
@return A return code of type XStatus.
*/ 
//***************************************************************************** 
XStatus XGraphicsMgr::setRenderContextStencilClearValue(XHandle renderContext,XU32 value)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->setStencilClearValue(value);
}



//***************************************************************************** 
/**
Gets the stencil buffer clear value.  This value is used to clear the stencil buffer
at the beginning of each render sequence.

@return The stencil clear value.
*/ 
//***************************************************************************** 
XU32 XGraphicsMgr::getRenderContextStencilClearValue(XHandle renderContext)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->getStencilClearValue();
}



//***************************************************************************** 
/**
Clears buffers such as the color, depth and stencil buffers depending on the 
flags passed in.

@param flags	Flags specifying which buffers to clear.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::clearRenderContext(XHandle renderContext,XBOOL color,XBOOL depth,XBOOL stencil)
{
	X_ASSERTM(renderContext != X_INVALID_HANDLE,"Invalid handle");

	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	X_ASSERTM(pRenderContext,"Could not find render context");

	return pRenderContext->clear(color,depth,stencil);
}

	

//*****************************************************************************
/**
Registers a scene node with a particular render state.  When the engine is ready
to render the scene node, it will preset the rendering device with the registered
render state settings for the node and then call the render() method for the node.
This function allows the engine to "batch" together scene nodes which share common
render states to provide more efficient render state switches.  

@param matHandle	The render state to associate a scene node with. A value of
				X_INVALID_HANDLE is valid.  It means that the scene node doesn't
				want to	register with a specific render state, but still wants to 
				recieve	a render notification from the engine.  Scene nodes who pass
				in X_INVALID_HANDLE for this parameter are responsible for setting
				their own render state settings to the rendering device before
				rendering themselves.
@param pNode	The scene node to register with the passed in render state.
@return A return code of type XStatus.
*/
//*****************************************************************************
/*XStatus XGraphicsMgr::addRenderEvent(XScene* pScene,XSceneNode* pSceneNode,XHandle matHandle,
							void* pUserData)
{
	X_ASSERTM(matHandle != X_INVALID_HANDLE,"Invalid handle");

	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
	X_ASSERTM(pMaterial,"Could not find material");

	pMaterial->addRenderEvent(pScene,pSceneNode,pUserData);

	return X_OK;
}
*/


//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::render(XF32 dt)
{
	//Run through render context resources. If a valid render context is found, render it.
	XU32 numRenderContexts = m_renderContexts.getCapacity();
	XRenderContext* pRenderContext = NULL;
	for (XU32 i = 1;i < numRenderContexts;++i)
	{
		pRenderContext = (XRenderContext*)m_renderContexts.getResource(i);
		if (pRenderContext)
		{
			m_activeRenderContext = i;
			pRenderContext->render(dt);
			pRenderContext->flip();
		}
	}
	//m_materials.render();

	//!!!!! BAD!!!
	//XRenderContext*	pRenderContext = (XRenderContext*)m_renderContexts.getResource(1);
	//pRenderContext->flip();
	return X_OK;
}



//***************************************************************************** 
/**
Shuts down the graphics driver.  This routine is called by X-VP during
shutdown and gives the driver an opportunity to clean up any remaining
resources and shut down the graphics device(s).

@return A return code of type XStatus.
*/ 
//***************************************************************************** 
XStatus XGraphicsMgr::destroy()
{
	m_inited = false;

	X_DEBUG_LOG_LN("\n*** BEGIN GRAPHICS MANAGER SHUTDOWN ***",X_DLL_INFO);

	//Destroy shaders from shader library
	XMemBlockList<XHandle>::iterator i(m_vsoHandles.begin());
	while (i.isValid())
	{
		destroyVertexShader(i.getValue());
		i.getNext();
	}
	i.set(m_gsoHandles.begin());
	while (i.isValid())
	{
		destroyGeometryShader(i.getValue());
		i.getNext();
	}
	i.set(m_psoHandles.begin());
	while (i.isValid())
	{
		destroyPixelShader(i.getValue());
		i.getNext();
	}
	m_vsoHandles.clear();
	m_gsoHandles.clear();
	m_psoHandles.clear();

	//Destroy debug textures
	destroyTexture(m_defaultTexHandle);
	destroyTexture(m_missingTexHandle);
	destroyTexture(m_missingMatHandle);
	destroyTexture(m_streamingTexHandle);

	destroyVertexBuffer(m_viewportQuadVB);
	destroyVertexShader(m_viewportQuadVS);
	destroyMaterial(m_viewportQuadMat);

	//DirectShow requires that COM interfaces be released during regular 
	//application running time.  For example, you cannot release COM interfaces
	//in destructors because COM needs to send messages when releasing
	//interfaces and if the release is called from a destructor, threads
	//block and the app crashes.  So we release all the movies here before
	//the destructor is called for the XGraphicsMgr, in fact, the destructor,
	//won't even attempt to release the movies.  So, if the call below is
	//not made now, the movies will not be freed and the app will crash.
	//destroyAllMovies();

	destroyAllMaterials();
	m_materials.destroy();

	//Destroy sprite vertex buffer
	destroyVertexBuffer(m_spriteVBHandle);

	//Destroy all textures
	m_textures.destroy();
	
	//Destroy all vertex buffers
	m_vertexBuffers.destroy();
	
	//Destroy all constant buffers
	m_constantBuffers.destroy();

	//Destroy all vertex declarations
	m_vertexDeclarations.destroy();
	
	//Destroy all render contexts
	m_renderContexts.destroy();

	//Destroy all render target arrays
	m_renderTargetArrays.destroy();

	//Destroy all index buffers
	m_indexBuffers.destroy();

	//Destroy all vertex shaders
	m_vertexShaders.destroy();

	//Destroy all vertex shaders
	m_geometryShaders.destroy();

	//Destroy all pixel shaders
	m_pixelShaders.destroy();

	//Destroy all depth stencil states
	m_depthStencilStates.destroy();
	
	//Destroy all rasterizer states
	m_rasterizerStates.destroy();
	
	//Destroy all blend states
	m_blendStates.destroy();
	
	//Destroy all sampler states
	m_samplerStates.destroy();

	//Destroy all movies
	m_movies.destroy();

	m_activeRenderContext = X_INVALID_HANDLE;

	SAFE_RELEASE(m_pD3DDevice);

	XDeleteArray(m_pFullScreenModes);

	//Shutdown DirectShow
    CoUninitialize();

	X_DEBUG_LOG_LN("*** END GRAPHICS MANAGER SHUTDOWN ***",X_DLL_INFO);

	return X_OK;
}



//*****************************************************************************
/**
Initializes the general internal data for the graphics driver.  This function
allows the driver to initialize non device specific data.

@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::generalInit(XVPDesc& desc)
{
	//Initialize light resource list.
	m_materials.init("Materials",desc.maxMaterials);
	m_textures.init("Textures",desc.maxTextures);	
	m_vertexDeclarations.init("VertexDeclarations",desc.maxVertexDeclarations);
	m_vertexBuffers.init("VertexBuffers",desc.maxVertexBuffers);
	m_renderContexts.init("RenderContexts",desc.maxRenderContexts);
	m_renderTargetArrays.init("RenderTargetArrays",desc.maxRenderTargetArrays);
	m_indexBuffers.init("IndexBuffers",desc.maxIndexBuffers);
	m_constantBuffers.init("ConstantBuffers",desc.maxConstantBuffers);
	m_vertexShaders.init("VertexShaders",desc.maxVertexShaders);
	m_geometryShaders.init("VertexShaders",desc.maxGeometryShaders);
	m_pixelShaders.init("PixelShaders",desc.maxPixelShaders);
	m_depthStencilStates.init("DepthStencilStates",desc.maxDepthStencilStates);
	m_rasterizerStates.init("RasterizerStates",desc.maxRasterizerStates);
	m_blendStates.init("BlendStates",desc.maxBlendStates);
	m_samplerStates.init("BlendStates",desc.maxBlendStates);
	m_movies.init("Movies",desc.maxMovies);

	//Load shader wads
	XS8 buf[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_SHADER,"ShaderCore.wad",buf);
	loadShaderWad(buf);
	XOSMgr::buildPath(X_PATH_SHADER,"ShaderDesigner.wad",buf);
	loadShaderWad(buf);

	//Create sprite vertex buffer
	XVertexDesc vertexDesc;
	vertexDesc.numColors = 1;
	vertexDesc.numUVs = 1;
 	createVertexBuffer(vertexDesc,X_PT_TRIANGLE_STRIP,4,true,m_spriteVBHandle);
		
	//Create viewport quad shader, material and vertex buffer
	XVertexDesc quadVD;
	quadVD.numUVs = 3;
	XVP::GraphicsMgr.createVertexBuffer(quadVD,X_PT_TRIANGLE_STRIP,4,true,m_viewportQuadVB);

	//Fill vertex buffer with normalized device coordinates to make a quad that,
	//when rendered, will cover the entire area of the current viewport
	/*
	
	(-1,1) ------- (1,1)
	   |			 |
	   |			 |
	   |			 |
	   |			 |
	(-1,-1)-------(1,-1)
	  
	*/

	XVertexBuffer* pVB = lockVertexBuffer(m_viewportQuadVB,X_LT_DISCARD);
	pVB->setPosition(0,-1,1,0);
	pVB->setUV(0,0,0,0);
	pVB->setPosition(1,1,1,0);
	pVB->setUV(1,0,1,0);
	pVB->setPosition(2,-1,-1,0);
	pVB->setUV(2,0,0,1);
	pVB->setPosition(3,1,-1,0);
	pVB->setUV(3,0,1,1);
	unlockVertexBuffer(m_viewportQuadVB);

	createVertexShader("Fullscreen Quad.fx","mainVS",m_viewportQuadVS);
	createMaterial(NULL,m_viewportQuadMat);
	XMaterial* pMaterial = XVP::GraphicsMgr.lockMaterial(m_viewportQuadMat);
	pMaterial->enableDepthTest(false);
	pMaterial->enableDepthWrite(false);
	pMaterial->enableAlphaBlend(0,true);
	unlockMaterial(m_viewportQuadMat);

	//Create debug textures
	XS8 filename[X_MAX_PATH_SIZE];
	XOSMgr::buildPath(X_PATH_SYSTEM,"textures/defaultTexture.png",filename);
	createTextureFromFile(filename,m_defaultTexHandle,true);
	XOSMgr::buildPath(X_PATH_SYSTEM,"textures/missingTexture.png",filename);
	createTextureFromFile(filename,m_missingTexHandle,true);
	XOSMgr::buildPath(X_PATH_SYSTEM,"textures/missingMaterial.png",filename);
	createTextureFromFile(filename,m_missingMatHandle,true);
	XOSMgr::buildPath(X_PATH_SYSTEM,"textures/streamingTexture.png",filename);
	createTextureFromFile(filename,m_streamingTexHandle,true);

	return X_OK;
}



//***************************************************************************** 
/**
Initializes the graphics driver with the best hardware capabilities possible.

@param initDesc	Initialization description structure.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::init(XVPDesc& initDesc)
{
	m_inited = true;

	X_DEBUG_LOG_LN("\n*** BEGIN GRAPHICS MANAGER INIT ***",X_DLL_INFO);

	//Create the graphics heap first
	m_pHeap = XOSMgr::createHeap("XGraphicsMgr");

	if (createDevice(initDesc) != X_OK)
		return X_ERR_OPERATION_FAILED;
	if (generalInit(initDesc) != X_OK)
		return X_ERR_OPERATION_FAILED;

	//Initialize lua glue
	initLuaGlue();

	//Set some default render states
	//setDepthBuffer(X_DBT_Z_BUFFER);

	X_DEBUG_LOG_LN("*** END GRAPHICS MANAGER INIT ***",X_DLL_INFO);

	//If we make it here then everything went OK.
	return X_OK;
}



//***************************************************************************** 
/**
Creates a rendering device with the best hardware capabilities possible.

@param initDesc	Initialization description structure.
@return A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XGraphicsMgr::createDevice(XVPDesc& initDesc)
{
	HRESULT hr = S_OK;

	//Create the DXGI Factory
    hr = CreateDXGIFactory(IID_IDXGIFactory,(void**)&m_pDXGIFactory);
	X_ASSERTM(hr == S_OK,"Could not create DXGI Factory.")
	//	extern HWND hwnd;
	//m_pDXGIFactory->MakeWindowAssociation(hwnd,0 );

	//Get the first adapter found
	IDXGIAdapter* pAdapter;
	hr = m_pDXGIFactory->EnumAdapters(0,&pAdapter);
	X_ASSERTM(hr == S_OK,"Could not enumerate first adapter.");
	DXGI_ADAPTER_DESC adapterDesc;
	pAdapter->GetDesc(&adapterDesc);

	//Log adapter description
	XS8 name[128];
	XS8 description[1024];
	wcstombs(name,adapterDesc.Description,128);
	sprintf(description,"%s VidMem(%d) SysMem(%d) Rev(%d) DeviceID(%d) VenderID(%d)",
			name,adapterDesc.DedicatedVideoMemory,adapterDesc.DedicatedSystemMemory,
			adapterDesc.Revision,adapterDesc.DeviceId,adapterDesc.VendorId);
	X_DEBUG_LOG_LN(description,X_DLL_INFO);
	
	//Get the first output of the first adapter
	IDXGIOutput* pOutput;
	hr = pAdapter->EnumOutputs(0,&pOutput);
	X_ASSERTM(hr == S_OK,"Could not enumerate first output of first adapter.");

	//Get the possible display modes
	m_numFullScreenModes = 0;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
	XU32 flags = DXGI_ENUM_MODES_INTERLACED | DXGI_ENUM_MODES_SCALING;

	//Get the number of available display modes for the given format and scanline order
	pOutput->GetDisplayModeList(format,flags,&m_numFullScreenModes,NULL);
	DXGI_MODE_DESC* pDescs = NULL;
	XNewArray(pDescs,DXGI_MODE_DESC,m_numFullScreenModes,m_pHeap);
	XNewArray(m_pFullScreenModes,XFullScreenModeDesc,m_numFullScreenModes,m_pHeap);
	pOutput->GetDisplayModeList(format,flags,&m_numFullScreenModes,pDescs);
	
	//Copy the display modes to our list 
	X_DEBUG_LOG_LN("Enumerating available display modes:",X_DLL_INFO);
	for (XU32 i = 0;i < m_numFullScreenModes;++i)
	{
		m_pFullScreenModes[i].width = pDescs[i].Width;
		m_pFullScreenModes[i].height = pDescs[i].Height;
		m_pFullScreenModes[i].format = translateDataFormatType(pDescs[i].Format);
		m_pFullScreenModes[i].refreshRate = pDescs[i].RefreshRate.Numerator / 
									   pDescs[i].RefreshRate.Denominator;
		m_pFullScreenModes[i].scalingMode = translateScalingMode(pDescs[i].Scaling);
		m_pFullScreenModes[i].scanlineOrdering = translateScanlineOrder(pDescs[i].ScanlineOrdering);

		X_DEBUG
		(
			XS8 buf[512];
			sprintf(buf,"Resolution(%d x %d) Refresh(%d)",m_pFullScreenModes[i].width,
														  m_pFullScreenModes[i].height,
														  m_pFullScreenModes[i].refreshRate);
			if (m_pFullScreenModes[i].scanlineOrdering == X_SCANLINE_ORDER_PROGRESSIVE)
				strcat(buf," Scaline(Progressive)");
			if (m_pFullScreenModes[i].scanlineOrdering == X_SCANLINE_ORDER_UPPER_FIELD_FIRST)
				strcat(buf," Scaline(Upper First)");
			if (m_pFullScreenModes[i].scanlineOrdering == X_SCANLINE_ORDER_LOWER_FIELD_FIRST)
				strcat(buf," Scaline(Lower First)");
			if (m_pFullScreenModes[i].scalingMode == X_SCALING_CENTERED)
				strcat(buf," Scaling(Centered)");
			if (m_pFullScreenModes[i].scalingMode == X_SCALING_STRETCHED)
				strcat(buf," Scaling(Stretched)");

			X_DEBUG_LOG_LN(buf,X_DLL_INFO);
		)
	}	

	XDeleteArray(pDescs);

	//At this point we've decided on windowed or fullscreen mode, so create the device.
#ifdef XVP_FINAL
	hr = D3D10CreateDevice( NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, 
                            D3D10_SDK_VERSION, &m_pD3DDevice );
#else

	// Look for 'NVIDIA PerfHUD' adapter
	// If it is present, override default settings
	IDXGIFactory *pDXGIFactory;
	HRESULT hRes;

	hRes = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
	// Search for a PerfHUD adapter.
	UINT nAdapter = 0;
	IDXGIAdapter* adapter = NULL;
	IDXGIAdapter* selectedAdapter = NULL;
	D3D10_DRIVER_TYPE driverType = D3D10_DRIVER_TYPE_HARDWARE;
	while (pDXGIFactory->EnumAdapters(nAdapter, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		if (adapter)
		{
			DXGI_ADAPTER_DESC adaptDesc;
			if (SUCCEEDED(adapter->GetDesc(&adaptDesc)))
			{
				const bool isPerfHUD = wcscmp(adaptDesc.Description, L"NVIDIA PerfHUD") == 0;
				// Select the first adapter in normal circumstances or the PerfHUD one if it exists.
				if(nAdapter == 0 || isPerfHUD)
					selectedAdapter = adapter;
				if(isPerfHUD)
					driverType = D3D10_DRIVER_TYPE_REFERENCE;
			}
		}
		++nAdapter;
	}

	if (initDesc.enableDeviceMessages)
	{
		hr = D3D10CreateDevice( selectedAdapter, driverType, NULL, D3D10_CREATE_DEVICE_DEBUG, 
								D3D10_SDK_VERSION, &m_pD3DDevice );
	}
	else
	{
		hr = D3D10CreateDevice( selectedAdapter, driverType, NULL, 0, 
								D3D10_SDK_VERSION, &m_pD3DDevice );
	}
#endif

	X_ASSERTM(hr == S_OK,"Could not create DX10 device.")
	X_DEBUG_LOG_LN("DX10 Device Created Successfully",X_DLL_INFO)

	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pOutput);


#ifdef OLD
	//Initialize Gamma Controls
	D3DGAMMARAMP r;
	for (XU32 rCtr = 0;rCtr < 256;++rCtr)
		r.red[rCtr] = r.green[rCtr] = r.blue[rCtr] = (rCtr * 256);
	m_pD3DDevice->SetGammaRamp(0,0,&r);
#endif

	//Initialize COM for DirectShow
    CoInitialize (NULL);

#ifdef OLD
	//Create Cg context
	m_context = cgCreateContext();
	cgD3D9SetDevice(m_pD3DDevice);
#endif


	return X_OK;
#ifdef OLD
	//This is the call the begins it all.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!m_pD3D)
	{
		X_DEBUG_LOG_LN("D3D9Create() failed.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}
	X_DEBUG_LOG_LN("D3D9Create() succeeded.",X_DLL_INFO);

	// Build a list of Direct3D adapters, modes and devices. The
    // ConfirmDevice() callback is used to confirm that only devices that
    // meet the app's requirements are considered.
	CD3DEnumeration graphicsDeviceEnumObj;
    graphicsDeviceEnumObj.SetD3D(m_pD3D);
    graphicsDeviceEnumObj.ConfirmDeviceCallback = NULL;
    m_hr = graphicsDeviceEnumObj.Enumerate();
  
	// Set up the presentation parameters
	ZeroMemory(&m_d3dpp,sizeof(m_d3dpp));
	m_d3dpp.Windowed = initDesc.windowed;	
    m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	
	XBOOL modeFound = false;
	gdriverColorInfo ci;
	D3DDISPLAYMODE* dm;
	D3DAdapterInfo* ai;

	//Store the available device modes.
	ai = (D3DAdapterInfo*)graphicsDeviceEnumObj.m_pAdapterInfoList->GetPtr(0);		
	for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
	{
		dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
		getColorDepth(dm->Format,&ci);
		XDeviceMode mode;
		mode.xRes = dm->Width;
		mode.yRes = dm->Height;
		mode.colorDepth = ci.bpp;
		mode.refreshRate = dm->RefreshRate;
		m_deviceModes.insertBack(mode);
	}
	
	//First, try to create the device exactly as specified by the user:
	switch (initDesc.colorDepth)
	{
		case 32:
			for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
			{
				dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
				getColorDepth(dm->Format,&ci);
				if ((dm->Width == initDesc.xResolution) && (dm->Height == initDesc.yResolution)
					&& (ci.bpp == 32))
				{
					m_d3dpp.BackBufferWidth  = dm->Width;
					m_d3dpp.BackBufferHeight = dm->Height;
					m_d3dpp.BackBufferFormat = dm->Format;
					modeFound = true;
					break;
				}	
			}
			break;
		case 24:
			for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
			{
				dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
				getColorDepth(dm->Format,&ci);
				if ((dm->Width == initDesc.xResolution) && (dm->Height == initDesc.yResolution)
					&& (ci.bpp == 24))
				{
					m_d3dpp.BackBufferWidth  = dm->Width;
					m_d3dpp.BackBufferHeight = dm->Height;
					m_d3dpp.BackBufferFormat = dm->Format;
					modeFound = true;
					break;
				}	
			}
			break;
		case 16:
			for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
			{
				dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
				getColorDepth(dm->Format,&ci);
				if ((dm->Width == initDesc.xResolution) && (dm->Height == initDesc.yResolution)
					&& (ci.bpp == 16))
				{
					m_d3dpp.BackBufferWidth  = dm->Width;
					m_d3dpp.BackBufferHeight = dm->Height;
					m_d3dpp.BackBufferFormat = dm->Format;
					modeFound = true;
					break;
				}	
			}
			break;
		default:
			modeFound = false;
			break;
	}

	//Since we could not create the exact device setting, try to find the
	//requested resolution in any color depth starting with 32.
	if (!modeFound)
	{
		for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
		{
			dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
			getColorDepth(dm->Format,&ci);
			if ((dm->Width == initDesc.xResolution) && (dm->Height == initDesc.yResolution)
				&& (ci.bpp == 32))
			{
				m_d3dpp.BackBufferWidth  = dm->Width;
				m_d3dpp.BackBufferHeight = dm->Height;
				m_d3dpp.BackBufferFormat = dm->Format;
				modeFound = true;
				break;
			}	
		}
	}

	//If we didn't find a 32 bit color depth, try a 24 bit color depth...
	if (!modeFound)
	{
		for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
		{
			dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
			getColorDepth(dm->Format,&ci);
			if ((dm->Width == initDesc.xResolution) && (dm->Height == initDesc.yResolution)
				&& (ci.bpp == 24))
			{
				m_d3dpp.BackBufferWidth  = dm->Width;
				m_d3dpp.BackBufferHeight = dm->Height;
				m_d3dpp.BackBufferFormat = dm->Format;
				modeFound = true;
				break;
			}	
		}
	}
	
	//If we didn't find a 24 bit color depth, try a 16 bit color depth...
	if (!modeFound)
	{
		for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
		{
			dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
			getColorDepth(dm->Format,&ci);
			if ((dm->Width == initDesc.xResolution) && (dm->Height == initDesc.yResolution)
				&& (ci.bpp == 16))
			{
				m_d3dpp.BackBufferWidth  = dm->Width;
				m_d3dpp.BackBufferHeight = dm->Height;
				m_d3dpp.BackBufferFormat = dm->Format;
				modeFound = true;
				break;
			}	
		}
	}

	//If we still didn't find anything, just use any mode returned by
	//the device.
	if (!modeFound)
	{
		for (XU32 j = 0;j < ai->pDisplayModeList->Count();j++)
		{
			dm = (D3DDISPLAYMODE*)ai->pDisplayModeList->GetPtr(j);
			getColorDepth(dm->Format,&ci);
			m_d3dpp.BackBufferWidth  = dm->Width;
			m_d3dpp.BackBufferHeight = dm->Height;
			m_d3dpp.BackBufferFormat = dm->Format;
			modeFound = true;
			break;
		}
	}


	//At this point we have a resolution and color depth found, now we need to find
	//the best depth and stencil buffer combination based on the user's request.
	//If the user wants a depth buffer but not a stencil buffer...
	if ((initDesc.useDepthBuffer) && (!initDesc.useStencilBuffer))
	{
		//Try to find a 32 bit depth buffer first
		{
			if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
				                         D3DDEVTYPE_HAL,
				                         m_d3dpp.BackBufferFormat,
				                         D3DUSAGE_DEPTHSTENCIL,
					                     D3DRTYPE_SURFACE,
					                     D3DFMT_D32) == D3D_OK)
			{
				m_d3dpp.EnableAutoDepthStencil = true;
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D32;
			}
			//If not 32 bit depth buffer, try for a 16 bit depth buffer
			else if (m_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,
				                         D3DDEVTYPE_HAL,
				                         m_d3dpp.BackBufferFormat,
				                         D3DUSAGE_DEPTHSTENCIL,
					                     D3DRTYPE_SURFACE,
					                     D3DFMT_D16) == D3D_OK)
			{
				m_d3dpp.EnableAutoDepthStencil = true;
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			}
			//No depth buffers availible so bail with an error
			else
				return X_ERR_OPERATION_FAILED;
		}
	}
	else if ((initDesc.useDepthBuffer) && (initDesc.useStencilBuffer))
	{
		//Try to find the best first, then proceed to the next best until either a
		//suitable mode is found or nothing is found at all
		if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
				                     D3DDEVTYPE_HAL,
				                     m_d3dpp.BackBufferFormat,
				                     D3DUSAGE_DEPTHSTENCIL,
					                 D3DRTYPE_SURFACE,
					                 D3DFMT_D24S8) == D3D_OK)
			{
				m_d3dpp.EnableAutoDepthStencil = true;
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
			}
		else if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
										  D3DDEVTYPE_HAL,
										  m_d3dpp.BackBufferFormat,
										  D3DUSAGE_DEPTHSTENCIL,
										  D3DRTYPE_SURFACE,
										  D3DFMT_D24X4S4) == D3D_OK)
			{
				m_d3dpp.EnableAutoDepthStencil = true;
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24X4S4;
			}
		else if (m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
										  D3DDEVTYPE_HAL,
										  m_d3dpp.BackBufferFormat,
										  D3DUSAGE_DEPTHSTENCIL,
										  D3DRTYPE_SURFACE,
										  D3DFMT_D15S1) == D3D_OK)
			{
				m_d3dpp.EnableAutoDepthStencil = true;
				m_d3dpp.AutoDepthStencilFormat = D3DFMT_D15S1;
			}
		//else nothing is available so bail with an error
		else 
			return X_ERR_OPERATION_FAILED;
	}
	else if ((!initDesc.useDepthBuffer) && (!initDesc.useStencilBuffer))
	{
			//Do Nothing.
	}
	else
		return X_ERR_OPERATION_FAILED;


	//If multihead, set the proper flag.  If windowed is set, then multihead is not
	//available.
	XU32 multiHeadFlag = 0;
	if ((!initDesc.windowed) && (initDesc.multiHead))
		multiHeadFlag = D3DCREATE_ADAPTERGROUP_DEVICE;

	//Create MAX_NUM_HEADS versions of the presentation parameters to support up to
	//MAX_NUM_HEADS output heads on the rendering device.
	D3DPRESENT_PARAMETERS p[MAX_NUM_HEADS];	
	memcpy(p,&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));
	memcpy(&p[1],&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));
	memcpy(&p[2],&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS));
	p[1].hDeviceWindow = hwnd[1];
	p[2].hDeviceWindow = hwnd[2];


    //Try to create the best device...first try for HAL and
	//hardware vertex processing.
    m_hr = m_pD3D->CreateDevice(/*m_pD3D->GetAdapterCount()-1*/D3DADAPTER_DEFAULT,/*D3DDEVTYPE_REF*/D3DDEVTYPE_HAL,
                             hwnd[0],multiHeadFlag | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,p,
                             &m_pD3DDevice);

	//If we failed, then try to create HAL with mixed vertex processing
    if (m_hr != D3D_OK)
	{  
	    m_hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
		                           hwnd[0],multiHeadFlag | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,p,
				                   &m_pD3DDevice);

		//If we failed, then try to create HAL with software vertex processing
		if (m_hr != D3D_OK)
		{
			m_hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
										hwnd[0],multiHeadFlag | D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,p,
										&m_pD3DDevice);

			//One last attempt, to create a reference device
			if (m_hr != D3D_OK)
			{
				m_hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,
										hwnd[0],multiHeadFlag | D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,p,
										&m_pD3DDevice);
				if (m_hr != D3D_OK)
					return X_ERR_OPERATION_FAILED;
				else
				{
					X_DEBUG_LOG_LN("REFERENCE DEVICE CREATED",X_DLL_INFO);
				}
			}
			else
			{
				X_DEBUG_LOG_LN("HAL/SOFTWARE VERTEX PROCESSING DEVICE CREATED",X_DLL_INFO);
			}
		}
		else
		{
			X_DEBUG_LOG_LN("HAL/MIXED VERTEX PROCESSING DEVICE CREATED",X_DLL_INFO);
		}
	}
	else
	{
		X_DEBUG_LOG_LN("HAL/HARDWARE VERTEX PROCESSING DEVICE CREATED",X_DLL_INFO);
	}
	  

	//Retrieve the capabilities of this device.
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_deviceCaps);

	//Determine how many heads were found and store a pointer to each back buffer.  A
	//maximum of MAX_NUM_HEADS heads are supported.
    m_numHeads = m_pD3DDevice->GetNumberOfSwapChains();
	if (m_numHeads > MAX_NUM_HEADS)
	{
		X_DEBUG_LOG_LN("A maximum of 3 heads (output ports) on the video device are supported.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}
	else
	{
		//Fill in pointers to surfaces.
		for (XU32 i = 0;i < m_numHeads;i++)
			m_pD3DDevice->GetBackBuffer(i,0,D3DBACKBUFFER_TYPE_MONO,&m_headSurfaces[i]);
	}
	
	//IDirect3DSurface9* s;
	//m_pD3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&s);
	//m_pD3DDevice->SetRenderTarget(0,s);

	//Generate the default devCaps file containing info about the
	//capabilities available on the current device.
	//dumpDeviceCapabilities(NULL);

	//Initialize Gamma Controls
	D3DGAMMARAMP r;
	for (XU32 rCtr = 0;rCtr < 256;++rCtr)
		r.red[rCtr] = r.green[rCtr] = r.blue[rCtr] = (rCtr * 256);
	m_pD3DDevice->SetGammaRamp(0,0,&r);
	
	//Turn on culling
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW); 
	
	//m_pD3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,   D3DMCS_MATERIAL );
    //Turn off lighting
    m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

    //Turn on the zbuffer
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);

	//Initialize COM for DirectShow
    CoInitialize (NULL);

	//Create Cg context
	m_context = cgCreateContext();
	cgD3D9SetDevice(m_pD3DDevice);


	return X_OK;
#endif
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::loadShaderWad(XS8* pFilename)
{
	X_ASSERT(pFilename);
	
	//Obtain the shader library file size and load the file into memory
	FILE* f = fopen(pFilename,"rb");
	if (!f)
	{
		XS8 buf[512];
		sprintf(buf,"Did not find shader library %s",pFilename);
		X_WARNING(buf);
		return X_ERR_NO_SUCH_OBJECT;
	}

	fseek(f,0,SEEK_END);
	XS32 size = ftell(f);
	rewind(f);

	XS8* pMem = NULL;
	XNew(pMem,XS8[size],m_pHeap);
	fread(pMem,size,1,f);
	fclose(f);

	//Init the wad file from memory and look for all vso, gso and pso files
	XWad wadFile;
	X_ASSERT(wadFile.initFromMemory(pMem) == X_OK);

	XMemBlockList<XS8*> filenames;
	XMemBlockList<XS8*>::iterator i;
	XHandle handle;
	XS8* pCode = NULL;
	XU32 codeSize = 0;

	wadFile.findFiles(".vso",filenames);
	i.set(filenames.begin());
	XU32 refCount = 0;
	while (i.isValid())
	{
		//Create a shader from object code
		XHLSLVertexShader* pShader = NULL;
		XNew(pShader,XHLSLVertexShader,m_pHeap);
		
		//Extract object code from wad
		wadFile.extractFile(i.getValue(),pCode,codeSize);
		X_ASSERT(pShader->initFromObjectCode(pCode,codeSize) == X_OK)

		//If we already have a shader loaded by that name, we want to
		//replace the current one while maintaining the current ref count.
		//Basically the app shouldn't even know about the switch
		XResource* pResource = m_vertexShaders.findResourceByName(i.getValue(),handle);
		if (pResource)
		{
			//Transfer refcount
			pShader->setRefCount(pResource->getRefCount());
			pResource->destroy();
			XDelete(pResource);
			m_vertexShaders.replaceResource(handle,pShader,i.getValue());
		}
		else
		{
			handle = m_vertexShaders.addResource(pShader,i.getValue());
			m_vsoHandles.insertBack(handle);
		}
		
		//Don't forget to release bytes
		XDeleteArray(pCode);
		
		i.getNext();
	}
	filenames.clear();

	wadFile.findFiles(".gso",filenames);
	i.set(filenames.begin());
	while (i.isValid())
	{
		//Create a shader from object code
		XHLSLGeometryShader* pShader = NULL;
		XNew(pShader,XHLSLGeometryShader,m_pHeap);
		
		//Extract object code from wad
		wadFile.extractFile(i.getValue(),pCode,codeSize);
		X_ASSERT(pShader->initFromObjectCode(pCode,codeSize) == X_OK)

		//If we already have a shader loaded by that name, we want to
		//replace the current one while maintaining the current ref count.
		//Basically the app shouldn't even know about the switch
		XResource* pResource = m_geometryShaders.findResourceByName(i.getValue(),handle);
		if (pResource)
		{
			//Transfer refcount
			pShader->setRefCount(pResource->getRefCount());
			pResource->destroy();
			XDelete(pResource);
			m_geometryShaders.replaceResource(handle,pShader,i.getValue());
		}
		else
		{
			handle = m_geometryShaders.addResource(pShader,i.getValue());
			m_gsoHandles.insertBack(handle);
		}
		
		//Don't forget to release bytes
		XDeleteArray(pCode);
		
		i.getNext();
	}
	filenames.clear();

	wadFile.findFiles(".pso",filenames);
	i.set(filenames.begin());
	while (i.isValid())
	{
		//Create a shader from object code
		XHLSLPixelShader* pShader = NULL;
		XNew(pShader,XHLSLPixelShader,m_pHeap);
		
		//Extract object code from wad
		wadFile.extractFile(i.getValue(),pCode,codeSize);
		X_ASSERT(pShader->initFromObjectCode(pCode,codeSize) == X_OK)

		//If we already have a shader loaded by that name, we want to
		//replace the current one while maintaining the current ref count.
		//Basically the app shouldn't even know about the switch
		XResource* pResource = m_pixelShaders.findResourceByName(i.getValue(),handle);
		if (pResource)
		{
			//Transfer refcount
			pShader->setRefCount(pResource->getRefCount());
			pResource->destroy();
			XDelete(pResource);
			m_pixelShaders.replaceResource(handle,pShader,i.getValue());
		}
		else
		{
			handle = m_pixelShaders.addResource(pShader,i.getValue());
			m_psoHandles.insertBack(handle);
		}
		
		//Don't forget to release bytes
		XDeleteArray(pCode);
		
		i.getNext();
	}
	filenames.clear();

	XDeleteArray(pMem);
	wadFile.destroy();
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XGraphicsMgr::getNumFullScreenModes()
{
	return m_numFullScreenModes;
}


	
//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::getFullScreenMode(XU32 index,XFullScreenModeDesc& fullScreenModeDesc)
{
	X_ASSERTM(index < m_numFullScreenModes,"Display mode index out of range.");
	fullScreenModeDesc = m_pFullScreenModes[index];
	return X_OK;
}

void* XGraphicsMgr::getWindowHandle(XHandle renderContext)
{
	XRenderContext* pRenderContext = (XRenderContext*)m_renderContexts.getResource(renderContext);
	if (pRenderContext)
	{
		return pRenderContext->getWindowHandle();
	}
	else 
		return NULL;	
}

//***************************************************************************** 
//***************************************************************************** 
XStatus XGraphicsMgr::dumpScreenshot(XS8* pFilename)
{
#ifdef OLD
	//Create the front buffer surface
	IDirect3DSurface9* pSurface;
	HRESULT hr = m_pD3DDevice->CreateOffscreenPlainSurface(m_d3dpp.BackBufferWidth,
														   m_d3dpp.BackBufferHeight,
														   D3DFMT_A8R8G8B8,
														   D3DPOOL_SYSTEMMEM,
														   &pSurface,
														   NULL);
	if (hr == D3D_OK)
	{
		//Get a copy of the front buffer
		hr = m_pD3DDevice->GetFrontBufferData(0,pSurface);
		if (hr == D3D_OK)
		{		
			//Use D3DX to save the front buffer. If we have a NULL filename,
			//generate one.
			hr = D3DXSaveSurfaceToFile(pFilename,D3DXIFF_BMP,pSurface,NULL,NULL);
		}
	}

	pSurface->Release();
	if (hr != D3D_OK)
		return X_ERR_OPERATION_FAILED;
	else
		return X_OK;
#else
	return X_OK;
#endif
}
	


//***************************************************************************** 
/**
Gets the rendering device.  The implementation of this function is heavily 
dependent on the environment.  Check your XSP documentation for more details.

@return A pointer to the rendering device.
*/
//***************************************************************************** 
void* XGraphicsMgr::getRenderingDevice()
{
	return m_pD3DDevice;
}




#if 0
//***************************************************************************** 
//***************************************************************************** 
void XGraphicsMgr::freeMovieResources(GraphicsMovie* pMovie)
{
	//Release all COM objects
	SAFE_RELEASE(pMovie->m_pMediaControl);
	SAFE_RELEASE(pMovie->m_pMediaEvent);
	SAFE_RELEASE(pMovie->m_pMediaPosition);
	SAFE_RELEASE(pMovie->m_pGraph);
	SAFE_RELEASE(pMovie->m_tex);
	XDeleteArray(pMovie->m_pFilename);
	XDelete(pMovie);
	--m_numAllocatedTextures; 
}

#endif

//***************************************************************************** 
//***************************************************************************** 
D3D10_MAP XGraphicsMgr::translateLockType(X_LOCK_TYPE lockType)
{
	switch (lockType)
	{
	case X_LT_READ_ONLY:
		return D3D10_MAP_READ;
		break;
	case X_LT_WRITE_ONLY:
		return D3D10_MAP_WRITE;
		break;
	case X_LT_READ_WRITE:
		return D3D10_MAP_READ_WRITE;
		break;
	case X_LT_DISCARD:
		return D3D10_MAP_WRITE_DISCARD;
		break;
	case X_LT_NO_OVERWRITE:
		return D3D10_MAP_WRITE_NO_OVERWRITE;
		break;
	default:
		X_ASSERT_FORCED("Lock type not implemented.");
		return D3D10_MAP_WRITE_DISCARD;
	break;
	}
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::initLuaGlue()
{
	return XLua::registerLibrary(X_GRAPHICS_LUA_LIB_NAME,XGraphicsLuaLib);
}



//*****************************************************************************
//*****************************************************************************
XStatus XGraphicsMgr::renderViewportQuad(XScene* pScene,XHandle srcRTA,XViewport& srcViewport)
{
	applyMaterial(m_viewportQuadMat);
	setVertexShader(m_viewportQuadVS);
	
	XRenderTargetArrayDesc* pRTADesc = XVP::GraphicsMgr.getRenderTargetArrayDesc(srcRTA);

	XF32 minU = srcViewport.topLeftX / (XF32)pRTADesc->width;
	XF32 minV = srcViewport.topLeftY / (XF32)pRTADesc->height;
	XF32 maxU =	(srcViewport.topLeftX + srcViewport.width) / (XF32)pRTADesc->width;
	XF32 maxV = (srcViewport.topLeftY + srcViewport.height) / (XF32)pRTADesc->height;

	XVertexBuffer* pVB = lockVertexBuffer(m_viewportQuadVB,X_LT_DISCARD);
	pVB->setPosition(0,-1,1,0);
	pVB->setUV(0,0,minU,minV);
	pVB->setPosition(1,1,1,0);
	pVB->setUV(1,0,maxU,minV);
	pVB->setPosition(2,-1,-1,0);
	pVB->setUV(2,0,minU,maxV);
	pVB->setPosition(3,1,-1,0);
	pVB->setUV(3,0,maxU,maxV);

	pVB->setUV(0,1,0,0);
	pVB->setUV(1,1,1,0);
	pVB->setUV(2,1,0,1);
	pVB->setUV(3,1,1,1);

	//Create UVs that represent the viewport coordinates in the original
	//render target array for the scene
	XViewportArrayDesc desc;
	pScene->getViewportArray(desc);
	XU32 viewportIndex = pScene->getActiveViewportIndex();
	XRenderTargetArrayDesc* pBaseRTADesc = XVP::GraphicsMgr.getRenderTargetArrayDesc(pScene->getRenderTargetArray());
	minU = desc.viewports[viewportIndex].topLeftX / (XF32)pBaseRTADesc->width;
	minV = desc.viewports[viewportIndex].topLeftY / (XF32)pBaseRTADesc->height;
	maxU =	(desc.viewports[viewportIndex].topLeftX + desc.viewports[viewportIndex].width) / (XF32)pBaseRTADesc->width;
	maxV = (desc.viewports[viewportIndex].topLeftY + desc.viewports[viewportIndex].height) / (XF32)pBaseRTADesc->height;

	pVB->setUV(0,2,minU,minV);
	pVB->setUV(1,2,maxU,minV);
	pVB->setUV(2,2,minU,maxV);
	pVB->setUV(3,2,maxU,maxV);

	unlockVertexBuffer(m_viewportQuadVB);

	return renderVertexBuffer(m_viewportQuadVB);
}


