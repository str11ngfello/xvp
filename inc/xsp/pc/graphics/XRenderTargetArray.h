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
//	Module Name:	XRenderTargetArray.h
//	Author:			Lynn Duke
//	Creation Date:	3-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RENDER_TARGET_ARRAY_H__
#define __X_RENDER_TARGET_ARRAY_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Render Target Array.
//*****************************************************************************
class XRenderTargetArray : public XResource
{
protected:

    ID3D10Texture2D*			m_pDepthStencilTexture; 
    ID3D10Texture2D*			m_pNoMSAADepthStencilTexture; 
    ID3D10DepthStencilView*		m_pDepthStencilView;
    ID3D10DepthStencilView*		m_pNoMSAADepthStencilView;
    ID3D10ShaderResourceView*	m_pNoMSAADepthStencilShaderResourceView;  

	ID3D10Texture2D*			m_pDepthStencilTextureOvr; 
    ID3D10Texture2D*			m_pNoMSAADepthStencilTextureOvr; 
    ID3D10DepthStencilView*		m_pDepthStencilViewOvr;
    ID3D10DepthStencilView*		m_pNoMSAADepthStencilViewOvr;
    ID3D10ShaderResourceView*	m_pNoMSAADepthStencilShaderResourceViewOvr;  

	ID3D10Texture2D*			m_textures[X_MAX_RENDER_TARGETS]; 
	ID3D10Texture2D*			m_resolvedTextures[X_MAX_RENDER_TARGETS]; 
    ID3D10ShaderResourceView*	m_shaderResourceViews[X_MAX_RENDER_TARGETS];  
    ID3D10ShaderResourceView*	m_resolvedShaderResourceViews[X_MAX_RENDER_TARGETS];  
    ID3D10RenderTargetView*		m_renderTargetViews[X_MAX_RENDER_TARGETS];
    ID3D10RenderTargetView*		m_resolvedRenderTargetViews[X_MAX_RENDER_TARGETS];



	XRenderTargetArrayDesc		m_desc;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XRenderTargetArray();
	virtual ~XRenderTargetArray();

	//*****************************************************************************
	//Standard Render Context Functions
	//*****************************************************************************
	XStatus init(XRenderTargetArrayDesc& desc);
	XStatus destroy();
	XStatus setToDevice(XBOOL setDepthBuffer);
	XStatus resolve();
	XStatus clear(XBOOL color,XBOOL depth,XBOOL stencil);
	XStatus clear(XU32 renderTargetIndex);
	XStatus setRenderTargetClearColor(XU32 renderTargetIndex,XColor& color);
	XStatus getRenderTargetClearColor(XU32 renderTargetIndex,XColor& color);
	XStatus setClearColor(XColor& color);
	XStatus setDepthClearValue(XF32 value);
	XF32 getDepthClearValue();
	XStatus setStencilClearValue(XU32 value);
	XU32 getStencilClearValue();
	void overrideDepthBuffer(ID3D10Texture2D* pDepthStencilTexture,
							ID3D10Texture2D* pNoMSAADepthStencilTexture,
							ID3D10DepthStencilView* pDepthStencilView,
							ID3D10DepthStencilView* pNoMSAADepthStencilView,
							ID3D10ShaderResourceView* pNoMSAADepthStencilShaderResourceView)
	{
		m_pDepthStencilTextureOvr =  pDepthStencilTexture;
    	m_pNoMSAADepthStencilTextureOvr = pNoMSAADepthStencilTexture;  
    	m_pDepthStencilViewOvr =  pDepthStencilView;
    	m_pNoMSAADepthStencilViewOvr =  pNoMSAADepthStencilView;
		m_pNoMSAADepthStencilShaderResourceViewOvr =  pNoMSAADepthStencilShaderResourceView;
	}

	XRenderTargetArrayDesc* getDesc() {return &m_desc;}
	ID3D10Resource* getD3D10Resource(XU32 renderTargetIndex)
	{ 
		X_ASSERTM(renderTargetIndex < m_desc.numRenderTargets,"No such render target index"); 
		return m_resolvedTextures[renderTargetIndex]; 
	}

	ID3D10ShaderResourceView* getD3D10ShaderResourceView(XU32 renderTargetIndex) 
	{
		X_ASSERTM(renderTargetIndex < m_desc.numRenderTargets,"No such render target index"); 
		return m_resolvedShaderResourceViews[renderTargetIndex]; 
	}
	


    ID3D10Texture2D*			getDepthStencilTexture()
	{
		return m_pDepthStencilTexture;
	}

    ID3D10Texture2D*			getNoMSAADepthStencilTexture()
	{
		return m_pNoMSAADepthStencilTexture;
	} 
    ID3D10DepthStencilView*		getDepthStencilView()
	{
		return m_pDepthStencilView;
	}
    ID3D10DepthStencilView*		getNoMSAADepthStencilView()
	{
		return m_pNoMSAADepthStencilView;
	}
 

	ID3D10ShaderResourceView* getNoMSAADepthStencilSRV() 
	{ 
		X_ASSERTM(m_pNoMSAADepthStencilShaderResourceViewOvr || m_pNoMSAADepthStencilShaderResourceView ,"The render target array does not have a depth buffer."); 
		X_ASSERTM(m_desc.msaaCount == 1,"DX10 does not support resolving multisampled depth buffers as shader resources.  Since this render target array has an msaa count other than 1, the depth buffer is not accessible.");

		//Remember, overrides take precendence
		if (m_pNoMSAADepthStencilShaderResourceViewOvr)
			return m_pNoMSAADepthStencilShaderResourceViewOvr;
		else
			return m_pNoMSAADepthStencilShaderResourceView; 
	}

};


#endif