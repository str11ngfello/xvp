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
//	Module Name:	XEdgePostEffect.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XEdgePostEffect::XEdgePostEffect()
{
	m_samplerState = X_INVALID_HANDLE;
}


//*****************************************************************************
//*****************************************************************************
XStatus XEdgePostEffect::init(XU32 viewportWidth,XU32 viewportHeight)
{
	XVP::GraphicsMgr.createPixelShader("red.postfx","mainPS",m_redQuadPS);

	XSamplerDesc samplerDesc;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_samplerState);

	return X_OK;
}
	


XStatus XEdgePostEffect::destroy()
{
	XVP::GraphicsMgr.destroySamplerState(m_samplerState);
	XVP::GraphicsMgr.destroyPixelShader(m_redQuadPS);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XEdgePostEffect::render(XF32 dt,XScene* pScene,
			   XHandle srcRenderTargetArray,
			   XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport)
{
	XVP::GraphicsMgr.setSamplerState(m_samplerState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
	XVP::GraphicsMgr.setPixelShader(m_redQuadPS);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));
	XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);
	
	return X_OK;
}

