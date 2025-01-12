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
//	Module Name:	XDistortionPostEffect.h
//	Author:			Lynn Duke
//	Creation Date:	5-31-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_DISTORTION_POST_EFFECT_H__
#define __X_DISTORTION_POST_EFFECT_H__




class XDistortionPostEffect : public XPostEffect
{
public:
	
	XHandle					m_distortionPS;
	XHandle					m_samplerState;
	XHandle					m_srcRTA;
	
	XDistortionPostEffect()
	{
		m_distortionPS = X_INVALID_HANDLE;
		m_samplerState = X_INVALID_HANDLE;
		m_srcRTA = X_INVALID_HANDLE;
	}

	XStatus init()
	{
		XVP::GraphicsMgr.createPixelShader("distortion.postfx","mainPS",m_distortionPS);

		XSamplerDesc samplerDesc;
		samplerDesc.addressU = X_TAT_CLAMP;
		samplerDesc.addressW = X_TAT_CLAMP;
		samplerDesc.addressV = X_TAT_CLAMP;
		XVP::GraphicsMgr.createSamplerState(samplerDesc,m_samplerState);


		return X_OK;
	}
	
	XStatus destroy()
	{
		XVP::GraphicsMgr.destroyPixelShader(m_distortionPS);
		m_distortionPS = X_INVALID_HANDLE;
		XVP::GraphicsMgr.destroySamplerState(m_samplerState);
		m_samplerState = X_INVALID_HANDLE;

		//We don't destroy our source RTA, it belongs to someone else
		m_srcRTA = X_INVALID_HANDLE;

		return X_OK;
	}

	XStatus render(XF32 dt,XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport)
	{

		//If we don't have a source, we can't render
		if (m_srcRTA == X_INVALID_HANDLE)
		{
			X_WARNING("No srcRTA for distortion post effect..bailing");
			return X_ERR_NOT_READY;
		}

		XVP::GraphicsMgr.setSamplerState(m_samplerState,0);
		XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
		XVP::GraphicsMgr.setRenderTargetAsTexture(m_srcRTA,0,1);
		XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
		XVP::GraphicsMgr.setPixelShader(m_distortionPS);
		XVP::GraphicsMgr.setViewport(destViewport);
		XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));
		XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);

		return X_OK;
	}

	void setSourceRTA(XHandle srcRTA)
	{
		m_srcRTA = srcRTA;
	}
};


#endif
