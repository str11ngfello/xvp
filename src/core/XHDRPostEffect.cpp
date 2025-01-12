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
//	Module Name:	XHDRPostEffect.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XHDRPostEffect::XHDRPostEffect()
{
	m_viewportWidth = 0;
	m_viewportHeight = 0;
	m_fMiddleGrayKeyValue = 0.7f;
    m_fBrightPassThreshold = 2.1f; 
    m_fBrightPassOffset = 2.2f;  
    m_fLuminanceWhite = 2.0f;        
    m_fBloomScale = 0.25f;
    m_fStarScale = 0.125f;
    //,m_bBlueShift =  false;
    m_fStarInclination = 0.0f;
    m_fAdaptLuminance = 0.0f;
    m_fExposureClamp = 0.0f;
    m_bInstantAdapt = false;

	m_brightness = 0.0f;           
	m_contrast = 1.0f;           
	m_desaturate = 0.0f;
	
	m_pScaledSceneTexture = X_INVALID_HANDLE;      ///> Scaled copy of the HDR scene
	m_pToneMapTexture64x64 = X_INVALID_HANDLE;     ///> Average luminance samples from the HDR render target
	m_pToneMapTexture32x32 = X_INVALID_HANDLE;     ///> Average luminance samples from the HDR render target
	m_pToneMapTexture16x16 = X_INVALID_HANDLE;     ///> Average luminance samples from the HDR render target
	m_pToneMapTexture4x4 = X_INVALID_HANDLE;       ///> Average luminance samples from the HDR render target
	m_pToneMapTexture1x1 = X_INVALID_HANDLE;       ///> Average luminance samples from the HDR render target        

	adaptedLuminanceTexture = X_INVALID_HANDLE; ///> The luminance the user is adapted to
	adaptedLuminanceWorkingTexture = X_INVALID_HANDLE;
   
	m_pBrightPassBloomTexture = X_INVALID_HANDLE;
	m_pBrightPassStarTexture = X_INVALID_HANDLE; 

   
	m_pStarSourceTexture = X_INVALID_HANDLE;       ///> Star effect source texture        

	m_pBloomWorkingTexture = X_INVALID_HANDLE;     
	m_pBloomSourceTexture = X_INVALID_HANDLE;      ///> Bloom effect source texture        
	m_pBloomTexture = X_INVALID_HANDLE;            ///> Blooming effect texture

	for (XU32 i = 0;i < X_MAX_STAR_TEXTURES;++i)
		m_apStarTextures[i] = X_INVALID_HANDLE; ///> Star effect working textures

	m_noBlendState = X_INVALID_HANDLE;
}


//*****************************************************************************
//*****************************************************************************
XStatus XHDRPostEffect::init(XU32 viewportWidth,XU32 viewportHeight)
{
	//XVP::GraphicsMgr.createPixelShader("red.postfx","mainPS",X_PST_HLSL,m_redQuadPS);

	XHDRPostEffect();

	XSamplerDesc samplerDesc;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_samplerState);

	XBlendDesc blendDesc;
	blendDesc.blendEnable[0] = false;
	XVP::GraphicsMgr.createBlendState(blendDesc,m_noBlendState);

	XRenderTargetArrayDesc desc;
	desc.useDepthStencil = false;
	desc.numRenderTargets = 1;

    // Scaled version of the HDR scene texture

	if (viewportWidth < 8)
		viewportWidth = 8;
	if (viewportHeight < 8)
		viewportHeight = 8;
	m_scaledSceneViewport.width = viewportWidth / 4; 
	m_scaledSceneViewport.height = viewportHeight / 4; 
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R16G16B16A16_FLOAT;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pScaledSceneTexture);

    // Create a texture to hold the intermediate results of the luminance calculation
	desc.width = 64;
	desc.height = 64;
	desc.formats[0] = X_DFT_R32_FLOAT;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pToneMapTexture64x64);
 
	desc.width = 32;
	desc.height = 32;
	desc.formats[0] = X_DFT_R32_FLOAT;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pToneMapTexture32x32);
   
	desc.width = 16;
	desc.height = 16;
	desc.formats[0] = X_DFT_R32_FLOAT;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pToneMapTexture16x16);

	desc.width = 4;
	desc.height = 4;
	desc.formats[0] = X_DFT_R32_FLOAT;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pToneMapTexture4x4);

    desc.width = 1;
	desc.height = 1;
	desc.formats[0] = X_DFT_R32_FLOAT;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pToneMapTexture1x1);

    // Create a 1x1 texture to hold the luminance that the user is currently adapted to.
    // This allows for a simple simulation of light adaptation.
	desc.width = 1;
	desc.height = 1;
	desc.formats[0] = X_DFT_R32_FLOAT;
	desc.clearColors[0] = XColor(.5f,.5f,.5f,1.0f);
	XVP::GraphicsMgr.createRenderTargetArray(desc,adaptedLuminanceTexture);
	XVP::GraphicsMgr.createRenderTargetArray(desc,adaptedLuminanceWorkingTexture);

    // Create the bright-pass filter texture.
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pBrightPassBloomTexture);

    // Create the bright-pass filter texture.
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pBrightPassStarTexture);

    // Create a texture to be used as the source for the star effect
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pStarSourceTexture);
   
	// Create a texture to be used as the workspace for the bloom effect
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pBloomWorkingTexture);
   
    // Create a texture to be used as the source for the bloom effect
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pBloomSourceTexture);
  
    // Create the temporary blooming effect textures
    // Texture has a black border of single texel thickness to fake border
    // addressing using clamp addressing
	m_scaledScene8x8.width = viewportWidth / 8; 
	m_scaledScene8x8.height = viewportHeight / 8; 

	desc.width = viewportWidth / 8;
	desc.height = viewportHeight / 8;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;
	XVP::GraphicsMgr.createRenderTargetArray(desc,m_pBloomTexture);
    
    // Create the star effect textures
	desc.width = viewportWidth / 4;
	desc.height = viewportHeight / 4;
	desc.formats[0] = X_DFT_R8G8B8A8_UNORM;

    for( XU32 i = 0; i < X_MAX_STAR_TEXTURES; i++ )
    {
		XVP::GraphicsMgr.createRenderTargetArray(desc,m_apStarTextures[i]);
    }


	//Clear textures
	XVP::GraphicsMgr.clearRenderTargetArray(adaptedLuminanceTexture,true,false,false);
	XVP::GraphicsMgr.clearRenderTargetArray(adaptedLuminanceWorkingTexture,true,false,false);
    XVP::GraphicsMgr.clearRenderTargetArray(m_pBrightPassBloomTexture,true,false,false);
    XVP::GraphicsMgr.clearRenderTargetArray(m_pBrightPassStarTexture,true,false,false);
    XVP::GraphicsMgr.clearRenderTargetArray(m_pStarSourceTexture,true,false,false);
    XVP::GraphicsMgr.clearRenderTargetArray(m_pBloomWorkingTexture,true,false,false);
    XVP::GraphicsMgr.clearRenderTargetArray(m_pBloomSourceTexture,true,false,false);
    XVP::GraphicsMgr.clearRenderTargetArray(m_pBloomTexture,true,false,false);

	//m_GlareDef.Initialize(GLT_FILTER_CROSSSCREEN);

	m_viewportWidth = viewportWidth;
	m_viewportHeight = viewportHeight;

	return X_OK;
}
	


XStatus XHDRPostEffect::destroy()
{
	XVP::GraphicsMgr.destroySamplerState(m_samplerState);
	XVP::GraphicsMgr.destroyBlendState(m_noBlendState);

	XVP::GraphicsMgr.destroyRenderTargetArray(m_pScaledSceneTexture);      ///> Scaled copy of the HDR scene
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pToneMapTexture64x64);     ///> Average luminance samples from the HDR render target
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pToneMapTexture32x32);     ///> Average luminance samples from the HDR render target
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pToneMapTexture16x16);     ///> Average luminance samples from the HDR render target
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pToneMapTexture4x4);       ///> Average luminance samples from the HDR render target
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pToneMapTexture1x1);       ///> Average luminance samples from the HDR render target        

	XVP::GraphicsMgr.destroyRenderTargetArray(adaptedLuminanceTexture); ///> The luminance the user is adapted to
	XVP::GraphicsMgr.destroyRenderTargetArray(adaptedLuminanceWorkingTexture); ///> The luminance the user is adapted to

	XVP::GraphicsMgr.destroyRenderTargetArray(m_pBrightPassBloomTexture);
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pBrightPassStarTexture); 
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pStarSourceTexture);       ///> Star effect source texture       
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pBloomWorkingTexture);     
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pBloomSourceTexture);      ///> Bloom effect source texture        
	XVP::GraphicsMgr.destroyRenderTargetArray(m_pBloomTexture);            ///> Blooming effect texture

	for (XU32 i = 0;i < X_MAX_STAR_TEXTURES;++i)
		XVP::GraphicsMgr.destroyRenderTargetArray(m_apStarTextures[i]); ///> Star effect working textures

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XHDRPostEffect::update(XF32 dt,XScene* pScene)
{
	m_fElapsedTime += dt;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XHDRPostEffect::render(XF32 dt,XScene* pScene,
			   XHandle srcRenderTargetArray,
			   XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport)
{
	//Create a downsampled copy of the viewport
	XVP::GraphicsMgr.setBlendState(m_noBlendState);
	XPostEffect::downSample4x4(pScene,srcRenderTargetArray,
							   srcViewport,
							   m_pScaledSceneTexture,
							   m_scaledSceneViewport);

	//Measure luminance of the scene
	XPostEffect::measureLuminance(pScene,m_scaledSceneViewport,
								  m_pScaledSceneTexture,
								  m_pToneMapTexture64x64,
								  m_pToneMapTexture16x16,
								  m_pToneMapTexture4x4,
								  m_pToneMapTexture1x1);


	//Adapt luminance
	XPostEffect::adaptLuminance(pScene,adaptedLuminanceTexture, 
                                 m_pToneMapTexture1x1,
                                (m_bInstantAdapt ? 999999.0f : dt ), 
                                 m_fAdaptLuminance,
								 m_fExposureClamp,
                                 adaptedLuminanceWorkingTexture );
	XViewport viewport1x1;
	viewport1x1.width = 1;
	viewport1x1.height = 1;
	viewport1x1.minDepth = 0.0f;
	viewport1x1.maxDepth = 1.0f;
	XVP::GraphicsMgr.copyViewportToRenderTarget(adaptedLuminanceWorkingTexture,
												0,
												viewport1x1,
												adaptedLuminanceTexture,
												0);

	//Perform bright pass filter
	XPostEffect::brightPassFilter(pScene,m_scaledSceneViewport,
								  m_pScaledSceneTexture,
								  adaptedLuminanceTexture,
								  m_fMiddleGrayKeyValue,
								  m_fBrightPassThreshold,
								  m_fBrightPassOffset,
								  m_fLuminanceWhite,
								  m_pBrightPassBloomTexture,
								  m_pBrightPassStarTexture);

	//Blur and downsample bloom texture
	XPostEffect::gaussianBlur5x5(pScene,m_pBrightPassBloomTexture,m_scaledSceneViewport,m_pBloomWorkingTexture,m_scaledSceneViewport);
	XPostEffect::downSample2x2(pScene, m_pBloomWorkingTexture, m_scaledSceneViewport,m_pBloomSourceTexture,m_scaledSceneViewport );

    //Blur star texture
    XPostEffect::gaussianBlur5x5(pScene,m_pBrightPassStarTexture,m_scaledSceneViewport, m_pStarSourceTexture, m_scaledSceneViewport );

	
	//Render bloom and star effects
	renderBloom(pScene);
    //renderStar( viewport, pPostProcess );
 
	XVP::GraphicsMgr.removeRenderTargetArray();
	XPostEffect::tonemap(pScene,
						srcViewport,
						srcRenderTargetArray,
						 m_pBloomTexture,
						m_apStarTextures[0],
						adaptedLuminanceTexture,
						destViewport,
						destRenderTargetArray,
                         m_fMiddleGrayKeyValue,
                         m_fLuminanceWhite,
                         m_fBloomScale,
                         m_fStarScale,
						 m_brightness,
						 m_contrast,
						 m_desaturate,
						 m_colorBalance);
	//XPostEffect::copy(m_pToneMapTexture64x64,t,destRenderTargetArray,
	//		   destViewport);
	
	
		
	return X_OK;
}



void XHDRPostEffect::renderBloom(XScene* pScene)
{

   /* if( ( m_fBloomScale == 0.0f ) || 
        ( m_GlareDef.m_fGlareLuminance <= 0.0f ) || 
        ( m_GlareDef.m_fBloomLuminance <= 0.0f ) )
    {
        pPostProcess->ClearTexture( m_pBloomTexture );
        return;
    }*/

    // Render to first bloom texture (Gaussian blur 5x5 m_pBloomSourceTexture to m_pBloomTexture)
	    XPostEffect::gaussianBlur5x5(pScene,m_pBloomSourceTexture,m_scaledSceneViewport, m_pBloomTexture, m_scaledScene8x8 );
//	pPostProcess->GaussBlur5x5Texture( viewport, m_pBloomSourceTexture, m_pBloomTexture );

    // Render to second bloom texture across width (m_pBloomTexture to m_pBloomTexture)
    XPostEffect::bloomTexture(pScene, m_scaledScene8x8, m_pBloomTexture, TRUE, m_scaledSceneViewport,m_pBloomWorkingTexture );

    // Render to final bloom texture and height (m_pBloomTexture to m_pBloomTexture)
    XPostEffect::bloomTexture(pScene, m_scaledSceneViewport, m_pBloomWorkingTexture, FALSE, m_scaledScene8x8,m_pBloomTexture );
}


XStatus XHDRPostEffect::handleResizeEvent(XU32 width,XU32 height)
{
	if ((m_viewportWidth == width) || (m_viewportHeight == height))
		return X_OK;

	destroy();
	return init(width,height);
}



XF32 XHDRPostEffect::getMiddleGrayKey()
{
	return m_fMiddleGrayKeyValue;
}



XStatus XHDRPostEffect::setMiddleGrayKey(XF32 middleGrayKey)
{
	m_fMiddleGrayKeyValue = middleGrayKey;
	return X_OK;
}




XF32 XHDRPostEffect::getBrightPassThreshold()
{
	return m_fBrightPassThreshold;
}




XStatus XHDRPostEffect::setBrightPassThreshold(XF32 brightPassThreshold)
{
	m_fBrightPassThreshold = brightPassThreshold;
	return X_OK;
}




XF32 XHDRPostEffect::getBrightPassOffset()
{
	return m_fBrightPassOffset;
}




XStatus XHDRPostEffect::setBrightPassOffset(XF32 brightPassOffset)
{
	m_fBrightPassOffset = brightPassOffset;
	return X_OK;
}




XF32 XHDRPostEffect::getLuminanceWhite()
{
	return m_fLuminanceWhite;
}




XStatus XHDRPostEffect::setLuminanceWhite(XF32 luminanceWhite)
{
	m_fLuminanceWhite = luminanceWhite;
	return X_OK;
}




XF32 XHDRPostEffect::getBloomScale()
{
	return m_fBloomScale;
}




XStatus XHDRPostEffect::setBloomScale(XF32 bloomScale)
{
	m_fBloomScale = bloomScale;
	return X_OK;
}




XF32 XHDRPostEffect::getExposureClamp()
{
	return m_fExposureClamp;
}




XStatus XHDRPostEffect::setExposureClamp(XF32 exposureClamp)
{
	m_fExposureClamp = exposureClamp;
	return X_OK;
}



XF32 XHDRPostEffect::getAdaptationSpeed()
{
	return m_fAdaptLuminance;
}



XStatus XHDRPostEffect::setAdaptationSpeed(XF32 adaptationSpeed)
{
	m_fAdaptLuminance = adaptationSpeed;
	return X_OK;
}



XBOOL XHDRPostEffect::getInstantAdapt()
{
	return m_bInstantAdapt;
}




XStatus XHDRPostEffect::setInstantAdapt(XBOOL instantAdapt)
{
	m_bInstantAdapt = instantAdapt;
	return X_OK;
}



XF32 XHDRPostEffect::getBrightness()
{
	return m_brightness;
}



XStatus XHDRPostEffect::setBrightness(XF32 brightness)
{
	m_brightness = brightness;
	return X_OK;
}



XF32 XHDRPostEffect::getContrast()
{
	return m_contrast;
}



XStatus XHDRPostEffect::setContrast(XF32 contrast)
{
	m_contrast = contrast;
	return X_OK;
}



XF32 XHDRPostEffect::getDesaturation()
{
	return m_desaturate;
}



XStatus XHDRPostEffect::setDesaturation(XF32 desaturate)
{
	m_desaturate = desaturate;
	return X_OK;
}



XColor XHDRPostEffect::getColorBalance()
{
	return m_colorBalance;
}



XStatus XHDRPostEffect::setColorBalance(XColor& colorBalance)
{
	m_colorBalance = colorBalance;
	return X_OK;
}










XStatus XHDRPostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("MiddleGrayKey",setMiddleGrayKey(value))
	PB_MAKE_PROPERTY("BrightPassThreshold",setBrightPassThreshold(value))
	PB_MAKE_PROPERTY("BrightPassOffset",setBrightPassOffset(value))
	PB_MAKE_PROPERTY("LuminanceWhite",setLuminanceWhite(value))
	PB_MAKE_PROPERTY("BloomScale",setBloomScale(value))
	PB_MAKE_PROPERTY("ExposureClamp",setExposureClamp(value))
	PB_MAKE_PROPERTY("AdaptationSpeed",setAdaptationSpeed(value))
	PB_MAKE_PROPERTY("Brightness",setBrightness(value))
	PB_MAKE_PROPERTY("Contrast",setContrast(value))
	PB_MAKE_PROPERTY("Desaturation",setDesaturation(value))
	PB_MAKE_PROPERTY("ColorBalanceRed",XColor color = getColorBalance(); color.m_R = value; setColorBalance(color))
	PB_MAKE_PROPERTY("ColorBalanceGreen",XColor color = getColorBalance(); color.m_G = value; setColorBalance(color))
	PB_MAKE_PROPERTY("ColorBalanceBlue",XColor color = getColorBalance(); color.m_B = value; setColorBalance(color))

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XHDRPostEffect::setBoolProperty(XS8* pPropertyName,XBOOL value)
{
	PB_MAKE_PROPERTY("InstantAdapt",setInstantAdapt(value))
	
	return X_ERR_NO_SUCH_OBJECT;
}
