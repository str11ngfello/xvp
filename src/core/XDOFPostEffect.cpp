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
//	Module Name:	XDOFPostEffect.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XDOFPostEffect::XDOFPostEffect()
{
	m_fFocalPlane = 10.0f;
    m_fNearBlurPlane = 1.0f; 
    m_fFarBlurPlane = 150.0f; 
    m_fMaxBlur = .5f;  
   
	m_pScaledSceneTexture = X_INVALID_HANDLE;      
	m_pBlurredSceneTexture = X_INVALID_HANDLE;     
	m_dofPS = X_INVALID_HANDLE;
	m_dofData = X_INVALID_HANDLE;
}


//*****************************************************************************
//*****************************************************************************
XStatus XDOFPostEffect::init(XU32 viewportWidth,XU32 viewportHeight)
{
	XVP::GraphicsMgr.createPixelShader("DOF.postfx","mainPS",m_dofPS);

	if (viewportWidth < 4)
		viewportWidth = 4;
	if (viewportHeight < 4)
		viewportHeight = 4;

	//Create states
	XSamplerDesc samplerDesc;
	samplerDesc.filter = X_TFT_MIN_MAG_MIP_POINT;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_pointClampState);

	samplerDesc.filter = X_TFT_MIN_MAG_MIP_LINEAR;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_linearClampState);

	XBlendDesc blendDesc;
	blendDesc.blendEnable[0] = false;
	XVP::GraphicsMgr.createBlendState(blendDesc,m_noBlendState);

	XRenderTargetArrayDesc desc;
	desc.useDepthStencil = false;
	desc.numRenderTargets = 1;
	m_scaledSceneViewport.width = viewportWidth / 4; 
	m_scaledSceneViewport.height = viewportHeight / 4; 
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pScaledSceneTexture);
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pBlurredSceneTexture);

	XVP::GraphicsMgr.createConstantBuffer(sizeof(DOFData),true,m_dofData);

	m_viewportWidth = viewportWidth;
	m_viewportHeight = viewportHeight;

	return X_OK;
}
	


XStatus XDOFPostEffect::destroy()
{
	XVP::GraphicsMgr.destroyPixelShader(m_dofPS);
	XVP::GraphicsMgr.destroySamplerState(m_pointClampState);
	XVP::GraphicsMgr.destroySamplerState(m_linearClampState);
	XVP::GraphicsMgr.destroyBlendState(m_noBlendState);

	XVP::GraphicsMgr.destroyRenderTargetArray(m_pScaledSceneTexture);      
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pBlurredSceneTexture);      

	XVP::GraphicsMgr.destroyConstantBuffer(m_dofData);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDOFPostEffect::update(XF32 dt,XScene* pScene)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDOFPostEffect::render(XF32 dt,XScene* pScene,
			   XHandle srcRenderTargetArray,
			   XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport)
{
	//Create a downsampled copy of the viewport
	XVP::GraphicsMgr.setBlendState(m_noBlendState);
	XPostEffect::downSample4x4(pScene,
								srcRenderTargetArray,
							   srcViewport,
							   m_pScaledSceneTexture,
							   m_scaledSceneViewport);

	//Blur the scene texture using a 5x5 guassian blur
	XPostEffect::gaussianBlur5x5(pScene,m_pScaledSceneTexture,m_scaledSceneViewport,m_pBlurredSceneTexture,m_scaledSceneViewport);
		
	// Transform plane distances to projection depth space
    XMatrix4x4& projMatrix = pScene->getActiveCamera()->getProjMatrix();// pm4xProj = pPostProcess->Get3DProj();

    XVector4D v4Focal = XVector4D( 0.0f, 0.0f, m_fFocalPlane, 1.0f );
    XVector4D v4Near  = XVector4D( 0.0f, 0.0f, m_fNearBlurPlane, 1.0f );
    XVector4D v4Far   = XVector4D( 0.0f, 0.0f, m_fFarBlurPlane, 1.0f );

	v4Focal = v4Focal * projMatrix;
    v4Near  = v4Near * projMatrix;
    v4Far   = v4Far * projMatrix;
   /* v4Focal = projMatrix.TransformVector( v4Focal );
    v4Near  = projMatrix.TransformVector( v4Near );
    v4Far   = projMatrix.TransformVector( v4Far );*/

    float fFocal = ( v4Focal.z / v4Focal.w );
    float fNear  = ( v4Near.z / v4Near.w );
    float fFar   = ( v4Far.z / v4Far.w );

    // Clamp ranges appropriately
    //fFocal = min( 1.0f, max( fFocal, 0.0f ) );
    //fNear  = min( 1.0f, max( fFocal, fNear ) );
    //fFar   = min( fFocal, max( fFar, 0.0f ) );

	
	DOFData* pDOFData = (DOFData*)XVP::GraphicsMgr.lockConstantBuffer(m_dofData,X_LT_DISCARD);
    pDOFData->focalPlane = fFocal;
    pDOFData->nearBlurPlane = fNear;
    pDOFData->farBlurPlane = fFar;
    pDOFData->maxBlur = m_fMaxBlur;
    pDOFData->nearMinusFocal =  1.0f / ( fNear - fFocal );
    pDOFData->focalMinusFar = 1.0f / ( fFocal - fFar );
	XVP::GraphicsMgr.unlockConstantBuffer(m_dofData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_dofData);

	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(m_pBlurredSceneTexture,0,1);
	XVP::GraphicsMgr.setDepthStencilAsTexture(pScene->getRenderTargetArray(),2);
	XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setSamplerState(m_linearClampState,1);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));
	XVP::GraphicsMgr.setPixelShader(m_dofPS);

	return XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);
}


XStatus XDOFPostEffect::handleResizeEvent(XU32 width,XU32 height)
{
	if ((m_viewportWidth == width) || (m_viewportHeight == height))
		return X_OK;

	destroy();
	return init(width,height);
}


XF32 XDOFPostEffect::getFocalPlane()
{
	return m_fFocalPlane;
}

XStatus XDOFPostEffect::setFocalPlane(XF32 focalPlane)
{
	m_fFocalPlane = focalPlane;
	return X_OK;
}

XF32 XDOFPostEffect::getNearBlurPlane()
{
	return m_fNearBlurPlane;
}

 
XStatus XDOFPostEffect::setNearBlurPlane(XF32 nearBlurPlane)
{
	m_fNearBlurPlane = nearBlurPlane;
	return X_OK;
}
 
XF32 XDOFPostEffect::getFarBlurPlane()
{
	return m_fFarBlurPlane;
}


XStatus XDOFPostEffect::setFarBlurPlane(XF32 farBlurPlane)
{
	m_fFarBlurPlane = farBlurPlane;
	return X_OK;
}
    
XF32 XDOFPostEffect::getMaxBlur()
{
	return m_fMaxBlur;
}


XStatus XDOFPostEffect::setMaxBlur(XF32 maxBlur)
{
	m_fMaxBlur = maxBlur;
	return X_OK;
}

XStatus XDOFPostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("FocalPlane",setFocalPlane(value))
	PB_MAKE_PROPERTY("NearBlurPlane",setNearBlurPlane(value))
	PB_MAKE_PROPERTY("FarBlurPlane",setFarBlurPlane(value))
	PB_MAKE_PROPERTY("MaxBlur",setMaxBlur(value))
	
	return X_ERR_NO_SUCH_OBJECT;
}