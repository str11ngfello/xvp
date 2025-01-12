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
//	Module Name:	XPostEffect.h
//	Author:			Lynn Duke
//	Creation Date:	4-20-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_POST_EFFECT_H__
#define __X_POST_EFFECT_H__




#define MAX_SAMPLES		16


//*****************************************************************************
//*****************************************************************************
class XPostEffect : public XPropertyBag
{
private:		  

	static XHandle		m_downSample2x2PS;
	static XHandle		m_downSample4x4PS;
	static XHandle		m_copyPS;
	static XHandle		m_guassianBlur5x5PS;
	static XHandle		m_initialLumPS;
	static XHandle		m_finalLumPS;
	static XHandle		m_adaptLumPS;
	static XHandle		m_brightPassPS;
	static XHandle		m_hdrTonemapPS;
	static XHandle		m_bloomPS;

	static XHandle		m_pointClampState;
	static XHandle		m_linearClampState;
	
	static XHandle		m_sampleData;
	static XHandle		m_hdrData;

protected:
	XU32				m_viewportWidth;
	XU32				m_viewportHeight;

	XBOOL				m_enableRender;
	XBOOL				m_enableUpdate;
	XS8*				m_pName;

	struct SampleData
	{
		XVector4D offsets[MAX_SAMPLES];
		XVector4D weights[MAX_SAMPLES];
	};

	struct HDRData
	{
		XF32 middleGray;
		XF32 brightPassThreshold;
		XF32 brightPassOffset;
		XF32 luminanceWhite;
		XF32 bloomScale;
		XF32 starScale;
		XF32 exposureClamp;
		XF32 dt;
		XF32 adaptPercent;
		XF32 brightness;
		XF32 contrast;
		XF32 desaturate;
		XColor colorBalance;
	};



public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XPostEffect();
	virtual ~XPostEffect();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	static XStatus initStaticData();
	static XStatus destroyStaticData();
	virtual XStatus init(XU32 viewportWidth,XU32 viewportHeight);
	virtual XStatus destroy();
	XStatus prepForUpdate(XF32 dt,XScene* pScene);
	virtual XStatus update(XF32 dt,XScene* pScene);
	XStatus prepForRender(XF32 dt,XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport);
	virtual XStatus render(XF32 dt,XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport);
	XStatus setName(XS8* pName);
	XS8* getName();
	virtual XStatus enableUpdate(XBOOL enable);
	virtual XBOOL isUpdateEnabled();
	virtual XStatus enableRender(XBOOL enable);
	virtual XBOOL isRenderEnabled();
	virtual XStatus handleResizeEvent(XU32 width,XU32 height);
	static XStatus downSample2x2(XScene* pScene,
		XHandle srcRenderTargetArray,
							XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport);
	
	static XStatus downSample4x4(XScene* pScene,
		XHandle srcRenderTargetArray,
								XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport);
	static XStatus copy(XScene* pScene,
		XHandle srcRenderTargetArray,
								XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport);
	static XStatus gaussianBlur5x5(XScene* pScene,
		XHandle srcRenderTargetArray,
								XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport);
	static XStatus measureLuminance(XScene* pScene,
		XViewport& scaledSceneViewport,
									  XHandle m_pScaledSceneTexture,
									  XHandle m_pToneMapTexture64x64,
									  XHandle m_pToneMapTexture16x16,
									  XHandle m_pToneMapTexture4x4,
									  XHandle m_pToneMapTexture1x1);
	static XStatus adaptLuminance(XScene* pScene,
		XHandle pAdaptedLuminanceTexture, 
                                    XHandle pToneMapTexture, 
                                    FLOAT              fElapsedTime,
                                    FLOAT              fAdaptPercent,
									FLOAT			   fExposureClamp,
                                    XHandle pDstTexture );
	static XStatus brightPassFilter(XScene* pScene,
		XViewport& srcViewport,
										 XHandle pSrcTexture,
										 XHandle pAdaptedLuminanceTexture, 
                                         XF32              fMiddleGrayKeyValue,
                                         XF32              fBrightPassThreshold,
                                         XF32              fBrightPassOffset,
                                         XF32              fLuminanceWhite,
										 XHandle pDstBloomTexture,
										 XHandle pDstStarTexture );

	static XStatus tonemap(XScene* pScene,
		XViewport& srcViewport,
										 XHandle pSrcTexture,
										 XHandle bloomTexture,
										 XHandle starTexture,
										 XHandle adaptedLumTexture,
										 XViewport& destViewport,
										 XHandle pDestTexture,
                                         XF32              fMiddleGrayKeyValue,
                                         XF32              fLuminanceWhite,
                                         XF32              bloomScale,
                                         XF32              starScale,
										 XF32              brightness,
										 XF32              contrast,
										 XF32              desaturate,
										 XColor&           colorBalance );
	static XStatus bloomTexture(XScene* pScene,
		XViewport&  viewport,
								  XHandle pSrcTexture,
                                  BOOL               bBloomAcrossWidth,
								  XViewport& destViewport,
                                  XHandle pDstTexture,
                                  FLOAT              fSize = 3.0f, 
                                  FLOAT              fBrightness  = 2.0f );

	/*static XStatus merge(XHandle srcRenderTargetArray,
								XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport);
	static XStatus add(XHandle srcRenderTargetArray,
								XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport);
	*/
};


#endif 

