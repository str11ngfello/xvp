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
//	Module Name:	XRenderContext.h
//	Author:			Lynn Duke
//	Creation Date:	3-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RENDER_CONTEXT_H__
#define __X_RENDER_CONTEXT_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XMemBlockList.h>
#include <d3d10.h>
#include <d3dx10.h>


#define MAX_LAYERS	3

struct XRenderContextDesc 
{
	void* pWindowHandle;
	XU32 width;
	XU32 height;
	X_DATA_FORMAT_TYPE format;
	XColor clearColor;
	XBOOL createDebugScene;
	
	XBOOL useDepthStencil;
	
	XRenderContextDesc()
	{
		pWindowHandle = NULL;
		width = 0;
		height = 0;
		format = X_DFT_UNKNOWN;
		useDepthStencil = false;
		createDebugScene = false;
	}
};


//*****************************************************************************
//! Render Context.
//*****************************************************************************
class XRenderContext : public XResource
{
protected:

	//*****************************************************************************
	//*****************************************************************************
	enum X_CMD_TYPE
	{
		X_CMDT_ADD,
		X_CMDT_REMOVE,
	};

	//*****************************************************************************
	//*****************************************************************************
	enum X_UPDATE_STATE
	{
		X_US_NORMAL,
		X_US_SCENES,
	};
	
	//*****************************************************************************
	//*****************************************************************************
	struct XSceneCmd
	{
		X_CMD_TYPE	cmdType;
		XScene*	pScene;
		XU32 layer;
		void*	pUserData;
	};

	XBOOL						m_resizing;
	X_UPDATE_STATE				m_updateState;

	XU32						m_orgWidth;
	XU32						m_orgHeight;

	XBOOL						m_windowed;
	XBOOL						m_isFullScreenMode;

	IDXGISwapChain*				m_pSwapChain;
	ID3D10RenderTargetView*		m_pRenderTargetView;
	ID3D10DepthStencilView*     m_pDepthStencilView;
	ID3D10Texture2D*            m_pDepthStencil;
	ID3D10Texture2D*			m_pBackBufferTexture;
	
	XF32						m_depthClearValue;
	XU32						m_stencilClearValue;
	HRESULT						m_hr;
	XBOOL						m_inited;

	XMemBlockList<XScene*>		m_scenes[MAX_LAYERS];
	XMemBlockList<XSceneCmd>	m_sceneCmdQueue;

	XRenderContextDesc			m_desc;

	XStatus createSwapChainBuffers(XU32 width,XU32 height);
	XStatus recreateSwapChainBuffers(XU32 width,XU32 height);

#ifndef XVP_FINAL
	XBOOL m_ownsDebugScene;
	static XScene					m_debugScene;
#endif
	void processSceneCmds(XF32 dt);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XRenderContext();
	virtual ~XRenderContext();

	//*****************************************************************************
	//Standard Render Context Functions
	//*****************************************************************************
	XStatus init(XRenderContextDesc& desc);
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus render(XF32 dt);
	XU32 getWidth()				{ return m_desc.width; }
    XU32 getHeight()			{ return m_desc.height; }
	XStatus clear(XBOOL color,XBOOL depth,XBOOL stencil);
	XStatus flip();
	void* getWindowHandle();
	XBOOL isFullScreenMode();
	XStatus setFullScreenMode(XFullScreenModeDesc& desc);
	XStatus setWindowedMode();
	XStatus resizeWindow(XU32 width,XU32 height);
	XRenderContextDesc* getDesc() { return &m_desc; }


	XStatus setToDevice();
	XStatus setClearColor(XColor& color);
	XStatus getClearColor(XColor& color);
	XStatus setDepthClearValue(XF32 value);
	XF32 getDepthClearValue();
	XStatus setStencilClearValue(ULONG32 value);
	XU32 getStencilClearValue();

	XStatus addScene(XU32 layer,XScene* pScene);
	XStatus insertScene(XU32 layer,XScene* pDependentScene,XScene* pScene);
	XStatus removeScene(XScene* pScene);
	XStatus removeAllScenes();

	ID3D10Resource* getD3D10BackBuffer() { return m_pBackBufferTexture; }

	static XScene* getDebugScene();
};


#endif