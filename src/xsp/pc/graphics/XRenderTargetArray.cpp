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
//	Module Name:	XRenderTargetArray.cpp
//	Author:			Lynn Duke
//	Creation Date:	3-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XRenderTargetArray::XRenderTargetArray()
{
    m_pDepthStencilTexture = NULL; 
    m_pNoMSAADepthStencilTexture = NULL; 
    m_pDepthStencilView = NULL;  
    m_pNoMSAADepthStencilView = NULL;  
    m_pNoMSAADepthStencilShaderResourceView = NULL;

	m_pDepthStencilTextureOvr = NULL; 
    m_pNoMSAADepthStencilTextureOvr = NULL; 
    m_pDepthStencilViewOvr = NULL;  
    m_pNoMSAADepthStencilViewOvr = NULL;  
    m_pNoMSAADepthStencilShaderResourceViewOvr = NULL;

	for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)
	{
		m_textures[i] = NULL;
		m_resolvedTextures[i] = NULL;
		m_shaderResourceViews[i] = NULL;
		m_resolvedShaderResourceViews[i] = NULL;
		m_renderTargetViews[i] = NULL;
		m_resolvedRenderTargetViews[i] = NULL;
	}
}



//*****************************************************************************
//*****************************************************************************
XRenderTargetArray::~XRenderTargetArray()
{
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::init(XRenderTargetArrayDesc& desc)
{

	X_DEBUG
	(
		X_ASSERTM(desc.width != 0,"Render target width cannot be 0. It must be a minimum of 1");
		X_ASSERTM(desc.height != 0,"Render target width cannot be 0. It must be a minimum of 1");
		X_ASSERTM(desc.numRenderTargets != 0,"Number of render targets cannot be 0");
		for (XU32 i = 1;i < desc.numRenderTargets;++i)
		{
			X_ASSERTM((desc.formats[i] != X_DFT_UNKNOWN),"Unspecified format for render target");
		}

		//if (desc.useDepthStencil)
			//X_ASSERTM(desc.depthStencilFormat != X_DFT_UNKNOWN,"Unspecified format for depth stencil buffer");
	)

	//Save description
	m_desc = desc;

	//Texture
	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.Width = desc.width;
	texDesc.Height = desc.height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = desc.msaaCount;
	texDesc.SampleDesc.Quality = desc.msaaQuality;
	texDesc.Usage = D3D10_USAGE_DEFAULT;
	texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//Shader view
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;

	//Render target view
	HRESULT hr;
	D3D10_RENDER_TARGET_VIEW_DESC RTVDesc;
	RTVDesc.Texture2D.MipSlice = 0;

	//If multisampling is enabled, create a multisample capable texture and a corresponding
	//resolve texture for each render target
	if (desc.msaaCount > 1)
	{
		RTVDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DMS;
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DMS;

		//Create multisample render targets
		for (XU32 i = 0;i < desc.numRenderTargets;++i)
		{
			texDesc.Format = SRVDesc.Format = RTVDesc.Format = XVP::GraphicsMgr.translateDataFormatType(desc.formats[i]);

			hr = XVP::GraphicsMgr.m_pD3DDevice->CreateTexture2D(&texDesc,NULL,&m_textures[i] );
			X_ASSERTM(hr == S_OK,"Could not create texture for render target");
			hr = XVP::GraphicsMgr.m_pD3DDevice->CreateShaderResourceView(m_textures[i],&SRVDesc,&m_shaderResourceViews[i]);
			X_ASSERTM(hr == S_OK,"Could not create shader resource view");
			hr = XVP::GraphicsMgr.m_pD3DDevice->CreateRenderTargetView(m_textures[i],&RTVDesc,&m_renderTargetViews[i]);
			X_ASSERTM(hr == S_OK,"Could not create render target view");
		}
	}
	
	//Now create resolve textures
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	RTVDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	for (XU32 i = 0;i < desc.numRenderTargets;++i)
	{
		texDesc.Format = SRVDesc.Format = RTVDesc.Format = XVP::GraphicsMgr.translateDataFormatType(desc.formats[i]);

		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateTexture2D(&texDesc,NULL,&m_resolvedTextures[i] );
		X_ASSERTM(hr == S_OK,"Could not create texture for render target");
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateShaderResourceView(m_resolvedTextures[i],&SRVDesc,&m_resolvedShaderResourceViews[i]);
		X_ASSERTM(hr == S_OK,"Could not create shader resource view");
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateRenderTargetView(m_resolvedTextures[i],&RTVDesc,&m_resolvedRenderTargetViews[i]);
		X_ASSERTM(hr == S_OK,"Could not create render target view");
	}

	//Depth Stencil
	if (desc.useDepthStencil)
	{
		//Create depth stencil texture
		D3D10_TEXTURE2D_DESC descDepth;
		descDepth.Width = desc.width;
		descDepth.Height = desc.height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;//XVP::GraphicsMgr.translateDataFormatType(desc.depthStencilFormat);
		descDepth.SampleDesc.Count = desc.msaaCount;
		descDepth.SampleDesc.Quality = desc.msaaQuality;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;// | D3D10_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		if (desc.msaaCount > 1)
		{
			hr = XVP::GraphicsMgr.m_pD3DDevice->CreateTexture2D(&descDepth,NULL,&m_pDepthStencilTexture);
			X_ASSERTM(hr == S_OK,"Could not create depth stencil texture.")
		}
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateTexture2D(&descDepth,NULL,&m_pNoMSAADepthStencilTexture);
		X_ASSERTM(hr == S_OK,"Could not create depth stencil texture.")


		//Create the depth stencil view
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//XVP::GraphicsMgr.translateDataFormatType(desc.depthStencilFormat);
		descDSV.Texture2D.MipSlice = 0;

		if (desc.msaaCount > 1)
		{
			descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2DMS;
			hr = XVP::GraphicsMgr.m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilTexture,&descDSV,&m_pDepthStencilView);
			X_ASSERTM(hr == S_OK,"Could not create depth stencil view.")
		}
		descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateDepthStencilView(m_pNoMSAADepthStencilTexture,&descDSV,&m_pNoMSAADepthStencilView);
		X_ASSERTM(hr == S_OK,"Could not create depth stencil view.")

		D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;//XVP::GraphicsMgr.translateDataFormatType(desc.depthStencilFormat);
		SRVDesc.Texture2D.MipLevels = 1;
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateShaderResourceView(m_pNoMSAADepthStencilTexture,&SRVDesc,&m_pNoMSAADepthStencilShaderResourceView);
		X_ASSERTM(hr == S_OK,"Could not create depth stencil shader resource view.")
	}

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::destroy()
{
	if (m_desc.msaaCount > 1)
	{
		for (XU32 i = 0;i < m_desc.numRenderTargets;++i)
		{
			SAFE_RELEASE(m_textures[i]);
			SAFE_RELEASE(m_shaderResourceViews[i]);
			SAFE_RELEASE(m_renderTargetViews[i]);
		}

		SAFE_RELEASE(m_pDepthStencilTexture);
		SAFE_RELEASE(m_pDepthStencilView);

		//Don't release depth surfaces that were shared with us
		m_pDepthStencilTextureOvr = NULL;
		m_pDepthStencilView = NULL;

	}
	

	for (XU32 i = 0;i < m_desc.numRenderTargets;++i)
	{
		SAFE_RELEASE(m_resolvedTextures[i]);
		SAFE_RELEASE(m_resolvedShaderResourceViews[i]);
		SAFE_RELEASE(m_resolvedRenderTargetViews[i]);
	}
	
	SAFE_RELEASE(m_pNoMSAADepthStencilTexture);
	SAFE_RELEASE(m_pNoMSAADepthStencilView);
	SAFE_RELEASE(m_pNoMSAADepthStencilShaderResourceView);

	//Don't release depth surfaces that were shared with us
	m_pNoMSAADepthStencilTextureOvr = NULL;
	m_pNoMSAADepthStencilView = NULL;
	m_pNoMSAADepthStencilShaderResourceView = NULL;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::resolve()
{
	if (m_desc.msaaCount > 1)
	{
		for (XU32 i = 0;i < m_desc.numRenderTargets;++i)
			XVP::GraphicsMgr.m_pD3DDevice->ResolveSubresource(m_resolvedTextures[i], 0, m_textures[i], 0, XVP::GraphicsMgr.translateDataFormatType(m_desc.formats[i]));
		
	}
		
	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::setToDevice(XBOOL setDepthBuffer)
{
	//Depth surfaces that were shared with us take precendence over our own.
	if (m_desc.msaaCount > 1)
	{
		if (m_pDepthStencilViewOvr)
			XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(m_desc.numRenderTargets,m_renderTargetViews,setDepthBuffer ? m_pDepthStencilViewOvr : NULL);
		else
		{
			if (m_pDepthStencilView)
				XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(m_desc.numRenderTargets,m_renderTargetViews,setDepthBuffer ? m_pDepthStencilView : NULL);
			else
				XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(m_desc.numRenderTargets,m_renderTargetViews,NULL);
		}
	}
	else
	{
		if (m_pNoMSAADepthStencilViewOvr)
			XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(m_desc.numRenderTargets,m_resolvedRenderTargetViews,setDepthBuffer ? m_pNoMSAADepthStencilViewOvr : NULL);
		else
		{
			if (m_pNoMSAADepthStencilView)
				XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(m_desc.numRenderTargets,m_resolvedRenderTargetViews,setDepthBuffer ? m_pNoMSAADepthStencilView : NULL);
			else
				XVP::GraphicsMgr.m_pD3DDevice->OMSetRenderTargets(m_desc.numRenderTargets,m_resolvedRenderTargetViews,NULL);
		}
	}

	return X_OK;
}

	 
//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::clear(XBOOL color,XBOOL depth,XBOOL stencil)
{
	//Clear surfaces based on passed in parameters.  Remember, we don't ever clear
	//depth surfaces that were shared with us...we only clear our own.  So,
	//don't looks at ANY of the depth override surfaces when making clearing decisions.
	if (m_desc.msaaCount > 1)
	{
		if (color)
		{
			for (XU32 i = 0;i < m_desc.numRenderTargets;++i)
			{
				XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_renderTargetViews[i],
																	(XF32*)&m_desc.clearColors[i]);
				XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_resolvedRenderTargetViews[i],
																	(XF32*)&m_desc.clearColors[i]);
			}
		}

		if (depth && m_pDepthStencilView)
		{
			XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView,
																D3D10_CLEAR_DEPTH,
																m_desc.depthClearValue,
																m_desc.stencilClearValue);
			XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pNoMSAADepthStencilView,
																D3D10_CLEAR_DEPTH,
																m_desc.depthClearValue,
																m_desc.stencilClearValue);
		}

		if (stencil && m_pDepthStencilView)
		{
			XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView,
																D3D10_CLEAR_STENCIL,
																m_desc.depthClearValue,
																m_desc.stencilClearValue);
			XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pNoMSAADepthStencilView,
																D3D10_CLEAR_STENCIL,
																m_desc.depthClearValue,
																m_desc.stencilClearValue);
		}
	}
	else
	{
		if (color)
		{
			for (XU32 i = 0;i < m_desc.numRenderTargets;++i)
				XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_resolvedRenderTargetViews[i],
																	(XF32*)&m_desc.clearColors[i]);
		}

		if (depth && m_pNoMSAADepthStencilView)
			XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pNoMSAADepthStencilView,
																D3D10_CLEAR_DEPTH,
																m_desc.depthClearValue,
																m_desc.stencilClearValue);

		if (stencil && m_pNoMSAADepthStencilView)
			XVP::GraphicsMgr.m_pD3DDevice->ClearDepthStencilView(m_pNoMSAADepthStencilView,
																D3D10_CLEAR_STENCIL,
																m_desc.depthClearValue,
																m_desc.stencilClearValue);
	}

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::clear(XU32 renderTargetIndex)
{
	X_ASSERTM(renderTargetIndex < m_desc.numRenderTargets,"No such render target index");

	if (m_desc.msaaCount > 1)
	{
		XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_renderTargetViews[renderTargetIndex],
															(XF32*)&m_desc.clearColors[renderTargetIndex]);
		XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_resolvedRenderTargetViews[renderTargetIndex],
															(XF32*)&m_desc.clearColors[renderTargetIndex]);
	}
	else
	{
		XVP::GraphicsMgr.m_pD3DDevice->ClearRenderTargetView(m_resolvedRenderTargetViews[renderTargetIndex],
															(XF32*)&m_desc.clearColors[renderTargetIndex]);
	}

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::setRenderTargetClearColor(XU32 renderTargetIndex,XColor& color)
{
	X_ASSERTM(renderTargetIndex < m_desc.numRenderTargets,"No such render target index");

	m_desc.clearColors[renderTargetIndex] = color;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::getRenderTargetClearColor(XU32 renderTargetIndex,XColor& color)
{
	X_ASSERTM(renderTargetIndex < m_desc.numRenderTargets,"No such render target index");

	color = m_desc.clearColors[renderTargetIndex];

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::setClearColor(XColor& color)
{
	for (XU32 i = 0;i < m_desc.numRenderTargets;++i)
	{
		m_desc.clearColors[i] = color;
	}

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::setDepthClearValue(XF32 value)
{
	m_desc.depthClearValue = value;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XF32 XRenderTargetArray::getDepthClearValue()
{

	return m_desc.depthClearValue;
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XRenderTargetArray::setStencilClearValue(XU32 value)
{

	m_desc.stencilClearValue = value;

	return X_OK;
}



//***************************************************************************** 
//***************************************************************************** 
XU32 XRenderTargetArray::getStencilClearValue()
{

	return m_desc.stencilClearValue;
}