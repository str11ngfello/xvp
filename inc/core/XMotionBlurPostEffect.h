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
//	Module Name:	XMotionBlurPostEffect.h
//	Author:			Lynn Duke
//	Creation Date:	5-31-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MOTION_BLUR_POST_EFFECT_H__
#define __X_MOTION_BLUR_POST_EFFECT_H__




class XMotionBlurPostEffect : public XPostEffect
{
public:
	
	XHandle					m_motionBlurPS;
	XHandle					m_velSS;
	XHandle					m_samplerState;
	
	XMotionBlurPostEffect()
	{
		m_motionBlurPS = X_INVALID_HANDLE;
		m_samplerState = X_INVALID_HANDLE;
		m_velSS = X_INVALID_HANDLE;
	}

	XStatus init()
	{
		XVP::GraphicsMgr.createPixelShader("MotionBlur.postfx","mainPS",m_motionBlurPS);

		XSamplerDesc samplerDesc;
		samplerDesc.addressU = X_TAT_CLAMP;
		samplerDesc.addressW = X_TAT_CLAMP;
		samplerDesc.addressV = X_TAT_CLAMP;
		samplerDesc.filter = X_TFT_MIN_MAG_MIP_LINEAR;
		XVP::GraphicsMgr.createSamplerState(samplerDesc,m_samplerState);


		samplerDesc.addressU = X_TAT_CLAMP;
		samplerDesc.addressW = X_TAT_CLAMP;
		samplerDesc.addressV = X_TAT_CLAMP;
		samplerDesc.filter = X_TFT_MIN_MAG_MIP_POINT;
		XVP::GraphicsMgr.createSamplerState(samplerDesc,m_velSS);


		return X_OK;
	}
	
	XStatus destroy()
	{
		XVP::GraphicsMgr.destroyPixelShader(m_motionBlurPS);
		m_motionBlurPS = X_INVALID_HANDLE;
		XVP::GraphicsMgr.destroySamplerState(m_samplerState);
		m_samplerState = X_INVALID_HANDLE;
		XVP::GraphicsMgr.destroySamplerState(m_velSS);
		m_velSS = X_INVALID_HANDLE;

		return X_OK;
	}

	XStatus render(XF32 dt,XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport)
	{


		XVP::GraphicsMgr.setSamplerState(m_samplerState,0);
		XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
		XVP::GraphicsMgr.setSamplerState(m_velSS,1);
		XVP::GraphicsMgr.setRenderTargetAsTexture(pScene->getRenderTargetArray(),1,1);
		XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
		XVP::GraphicsMgr.setPixelShader(m_motionBlurPS);
		XVP::GraphicsMgr.setViewport(destViewport);
		XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));
		XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);

		return X_OK;
	}


};


#endif
