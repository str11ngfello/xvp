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
//	Module Name:	XHDRPostEffect.h
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_HDR_POST_EFFECT_H__
#define __X_HDR_POST_EFFECT_H__




#define X_MAX_STAR_TEXTURES 12


//*****************************************************************************
//! Provides a viewpoint into the 3D scene.
/**
*/
//*****************************************************************************
class XHDRPostEffect : public XPostEffect
{
protected:
	XHandle					m_redQuadPS;
	XHandle					m_samplerState;
	XHandle					m_noBlendState;
	XViewport				m_scaledSceneViewport;
	XViewport				m_scaledScene8x8;

	XF32 m_fMiddleGrayKeyValue;  ///> Middle gray key value for tone mapping
	XF32 m_fBrightPassThreshold; ///> Bright pass threshold parameter
	XF32 m_fBrightPassOffset;    ///> Bright pass offset parameter
	XF32 m_fLuminanceWhite;      ///> Luminance white parameter
	XF32 m_fBloomScale;          ///> Scale factor for bloom
	XF32 m_fStarScale;           ///> Scale factor for star
	XF32 m_brightness;           ///> Scale factor for star
	XF32 m_contrast;           ///> Scale factor for star
	XF32 m_desaturate;
	XColor m_colorBalance;
	
	//BOOL  m_bBlueShift;           ///> Activates blue shifting saturation

	XF32 m_fAdaptLuminance;  ///> Percent luminance adapts per frame based on 30 fps
	XF32 m_fExposureClamp;	  ///> Max,Min Exposure
	XF32 m_fStarInclination; ///> Controls star orientation

   
	XF32 m_fElapsedTime;

	XBOOL  m_bInstantAdapt;   ///> When true, luminance adapt happens instantly

	XHandle m_pScaledSceneTexture;      ///> Scaled copy of the HDR scene
	XHandle m_pToneMapTexture64x64;     ///> Average luminance samples from the HDR render target
	XHandle m_pToneMapTexture32x32;     ///> Average luminance samples from the HDR render target
	XHandle m_pToneMapTexture16x16;     ///> Average luminance samples from the HDR render target
	XHandle m_pToneMapTexture4x4;       ///> Average luminance samples from the HDR render target
	XHandle m_pToneMapTexture1x1;       ///> Average luminance samples from the HDR render target        

	XHandle adaptedLuminanceTexture; ///> The luminance the user is adapted to
	XHandle adaptedLuminanceWorkingTexture; 

   
	XHandle m_pBrightPassBloomTexture;
	XHandle m_pBrightPassStarTexture; 

   
	XHandle m_pStarSourceTexture;       ///> Star effect source texture        

	XHandle m_pBloomWorkingTexture;     
	XHandle m_pBloomSourceTexture;      ///> Bloom effect source texture        
	XHandle m_pBloomTexture;            ///> Blooming effect texture

   
	XHandle m_apStarTextures[X_MAX_STAR_TEXTURES]; ///> Star effect working textures


	//peGlareDef m_GlareDef; ///> Glare definition

/*
	/// 
        /// Measure the average log luminance in the scene
        /// 
        void measureLuminance( const edViewport* viewport, pePostProcess* pPostProcess );

        /// 
        /// Render the blooming effect
        /// 
        void renderBloom( const edViewport* viewport, pePostProcess* pPostProcess );

        /// 
        /// Render the star effect
        /// 
        void renderStar( const edViewport* viewport, pePostProcess* pPostProcess );

		void buildBrightPassTextures( const edViewport*		viewport,
									  pePostProcess*		pPostProcess,
									  LPDIRECT3DTEXTURE9	adaptedLuminanceTexture );
									  */
	void renderBloom(XScene* pScene);
public:

	XHDRPostEffect();

	XStatus init(XU32 viewportWidth,XU32 viewportHeight);
	XStatus destroy();
	XStatus update(XF32 dt,XScene* pScene);
	XStatus render(XF32 dt,XScene* pScene,
				   XHandle srcRenderTargetArray,
				   XViewport& srcViewport,
				   XHandle destRenderTargetArray,
				   XViewport& destViewport);
	XStatus handleResizeEvent(XU32 width,XU32 height);

	XF32 getMiddleGrayKey();
	XStatus setMiddleGrayKey(XF32 middleGrayKey);
	XF32 getBrightPassThreshold();
	XStatus setBrightPassThreshold(XF32 brightPassThreshold);
	XF32 getBrightPassOffset();
	XStatus setBrightPassOffset(XF32 brightPassOffset);
	XF32 getLuminanceWhite();
	XStatus setLuminanceWhite(XF32 luminanceWhite);
	XF32 getBloomScale();
	XStatus setBloomScale(XF32 bloomScale);
	XF32 getExposureClamp();
	XStatus setExposureClamp(XF32 exposureClamp);
	XF32 getAdaptationSpeed();
	XStatus setAdaptationSpeed(XF32 adaptationSpeed);
	XBOOL getInstantAdapt();
	XStatus setInstantAdapt(XBOOL instantAdapt);

	XF32 getBrightness();
	XStatus setBrightness(XF32 brightness);
	XF32 getContrast();
	XStatus setContrast(XF32 contrast);
	XF32 getDesaturation();
	XStatus setDesaturation(XF32 desaturate);
	XColor getColorBalance();
	XStatus setColorBalance(XColor& colorBalance);

	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
	XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);

};


#endif
