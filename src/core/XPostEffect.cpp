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
//	Module Name:	XPostEffect.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-20-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


XHandle		XPostEffect::m_downSample2x2PS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_downSample4x4PS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_copyPS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_guassianBlur5x5PS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_initialLumPS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_finalLumPS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_adaptLumPS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_brightPassPS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_hdrTonemapPS = X_INVALID_HANDLE;
XHandle		XPostEffect::m_bloomPS = X_INVALID_HANDLE;

XHandle		XPostEffect::m_pointClampState = X_INVALID_HANDLE;
XHandle		XPostEffect::m_linearClampState = X_INVALID_HANDLE;

XHandle		XPostEffect::m_sampleData = X_INVALID_HANDLE;
XHandle		XPostEffect::m_hdrData = X_INVALID_HANDLE;
	



VOID GetSampleOffsets_DownScale2x2( DWORD dwTexWidth, DWORD dwTexHeight, XVector4D* pvSampleOffsets )
{
    FLOAT tu = 1.0f / (FLOAT)dwTexWidth;
    FLOAT tv = 1.0f / (FLOAT)dwTexHeight;

    // Sample from the 4 surrounding points. Since the center point will be in the exact
    // center of 4 texels, a 0.5f offset is needed to specify a texel center.
    for( int y = 0; y < 2; y++ )
    {
        for( int x = 0; x < 2; x++ )
        {
            pvSampleOffsets->x = ( (FLOAT)x - 0.5f ) * tu;
            pvSampleOffsets->y = ( (FLOAT)y - 0.5f ) * tv;
            pvSampleOffsets++;
        }
    }
}

VOID GetSampleOffsets_DownScale3x3( DWORD dwTexWidth, DWORD dwTexHeight, XVector4D* pvSampleOffsets )
{
    FLOAT tu = 1.0f / (FLOAT)dwTexWidth;
    FLOAT tv = 1.0f / (FLOAT)dwTexHeight;

    // Sample from the 9 surrounding points. Since the center point will be in the exact
    // center of 4 texels, a 1.0f offset is needed to specify a texel center.
    for( int x = 0; x < 3; x++ )
    {
        for( int y = 0; y < 3; y++ )
        {
            pvSampleOffsets->x = ( x - 1.0f ) * tu;
            pvSampleOffsets->y = ( y - 1.0f ) * tv;
            pvSampleOffsets++;
        }
    }
}

VOID GetSampleOffsets_DownScale4x4( DWORD dwTexWidth, DWORD dwTexHeight, XVector4D* pvSampleOffsets )
{
    FLOAT tu = 1.0f / (FLOAT)dwTexWidth;
    FLOAT tv = 1.0f / (FLOAT)dwTexHeight;

    // Sample from the 16 surrounding points. Since the center point will be in the
    // exact center of 16 texels, a 1.5f offset is needed to specify a texel center.
    for( int y = 0; y < 4; y++ )
    {
        for( int x = 0; x < 4; x++ )
        {
            pvSampleOffsets->x = ( (FLOAT)x - 1.5f ) * tu;
            pvSampleOffsets->y = ( (FLOAT)y - 1.5f ) * tv;
            pvSampleOffsets++;
        }
    }
}


inline FLOAT GaussianDistribution( FLOAT x, FLOAT y, FLOAT rho )
{
    return expf( -( ( x * x ) + ( y * y ) ) / ( 2 * rho * rho ) ) / sqrtf( 2 * X_MATH_PI * rho * rho );
}


VOID GetSampleOffsets_GaussBlur5x5( DWORD     dwTexWidth, 
                                                   DWORD     dwTexHeight,
                                                   XVector4D* pvTexCoordOffsets,
                                                   XVector4D* pvSampleWeights,
                                                   FLOAT     fMultiplier = 1.0f )
{
    FLOAT tu = 1.0f / (FLOAT)dwTexWidth;
    FLOAT tv = 1.0f / (FLOAT)dwTexHeight;

    XVector4D vWhite( 1.0f, 1.0f, 1.0f, 1.0f );

    FLOAT fTotalWeight = 0.0f;
    DWORD index        = 0;

    for( int x = -2; x <= 2; x++ )
    {
        for( int y = -2; y <= 2; y++ )
        {
            // Exclude pixels with a block distance greater than 2. This will
            // create a kernel which approximates a 5x5 kernel using only 13
            // sample points instead of 25; this is necessary since 2.0 shaders
            // only support 16 texture grabs.
            if( fabs( (FLOAT)x ) + fabs( (FLOAT)y ) > 2.0f )
            {
                continue;
            }

            // Get the unscaled Gaussian intensity for this offset
            pvTexCoordOffsets[index].x = (FLOAT)x * tu;
            pvTexCoordOffsets[index].y = (FLOAT)y * tv;
            pvTexCoordOffsets[index].z = 0.0f;
            pvTexCoordOffsets[index].w = 0.0f;
            pvSampleWeights[index]     = vWhite * GaussianDistribution( (FLOAT)x, (FLOAT)y, 1.0f );

            fTotalWeight += pvSampleWeights[index].x;

            index++;
        }
    }

    // Divide the current weight by the total weight of all the samples; Gaussian
    // blur kernels add to 1.0f to ensure that the intensity of the image isn't
    // changed when the blur occurs. An optional multiplier variable is used to
    // add or remove image intensity during the blur.
    for( DWORD i = 0; i < index; i++ )
    {
        pvSampleWeights[i] /= fTotalWeight;
        pvSampleWeights[i] *= fMultiplier;
    }
}

VOID GetSampleOffsets_Bloom( DWORD     dwTextureWidth, 
                                            DWORD     dwTextureHeight, 
                                            FLOAT     fAngle, 
                                            XVector4D* pvTexCoordOffsets,
                                            XVector4D* pvColorWeights, 
                                            FLOAT     fDeviation,
                                            FLOAT     fMultiplier )
{
    FLOAT tu = cosf(fAngle) / (FLOAT)dwTextureWidth;
    FLOAT tv = sinf(fAngle) / (FLOAT)dwTextureHeight;

    // Fill the center texel
    FLOAT fWeight        = fMultiplier * GaussianDistribution( 0, 0, fDeviation );
    pvColorWeights[0]    = XVector4D( fWeight, fWeight, fWeight, 1.0f );
    pvTexCoordOffsets[0] = XVector4D( 0.0f, 0.0f, 0.0f, 0.0f );

    // Fill the first half
    for( DWORD i = 1; i < 8; i++ )
    {
        // Get the Gaussian intensity for this offset
        fWeight              = fMultiplier * GaussianDistribution( (FLOAT)i, 0, fDeviation );
        pvColorWeights[i]    = XVector4D( fWeight, fWeight, fWeight, 1.0f );
        pvTexCoordOffsets[i] = XVector4D( i * tu, i * tv, 0.0f, 0.0f );
    }

    // Mirror to the second half
    for( DWORD i = 8; i < 15; i++ )
    {
        pvColorWeights[i]    =  pvColorWeights[i - 7];
        pvTexCoordOffsets[i] = -pvTexCoordOffsets[i - 7];
    }
}
/*
///
/// Summary: Get the texcoord offsets to be used inside the Bloom pixel shader.
///
/// Details: Get the texcoord offsets to be used inside the Bloom pixel shader.
///
/// Params: DWORD     - Texture width
///         DWORD     - Texture height
///         FLOAT     - Angle
///         XMVECTOR* - Pointer to coordinate offsets
///         XMVECTOR* - Pointer to color weights
///         FLOAT     - Deviation
///         FLOAT     - Scalar
///
/// Output: 
///
/// Notes:
///    - 3/21/2006 - [Shawn Halwes]: Initial revision
///
VOID GetSampleOffsets_Bloom( DWORD     dwTextureWidth, 
                                            DWORD     dwTextureHeight, 
                                            FLOAT     fAngle, 
                                            XMVECTOR* pvTexCoordOffsets,
                                            XMVECTOR* pvColorWeights, 
                                            FLOAT     fDeviation,
                                            FLOAT     fMultiplier )
{
    FLOAT tu = cosf(fAngle) / (FLOAT)dwTextureWidth;
    FLOAT tv = sinf(fAngle) / (FLOAT)dwTextureHeight;

    // Fill the center texel
    FLOAT fWeight        = fMultiplier * GaussianDistribution( 0, 0, fDeviation );
    pvColorWeights[0]    = XMVectorSet( fWeight, fWeight, fWeight, 1.0f );
    pvTexCoordOffsets[0] = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

    // Fill the first half
    for( DWORD i = 1; i < 8; i++ )
    {
        // Get the Gaussian intensity for this offset
        fWeight              = fMultiplier * GaussianDistribution( (FLOAT)i, 0, fDeviation );
        pvColorWeights[i]    = XMVectorSet( fWeight, fWeight, fWeight, 1.0f );
        pvTexCoordOffsets[i] = XMVectorSet( i * tu, i * tv, 0.0f, 0.0f );
    }

    // Mirror to the second half
    for( DWORD i = 8; i < 15; i++ )
    {
        pvColorWeights[i]    =  pvColorWeights[i - 7];
        pvTexCoordOffsets[i] = -pvTexCoordOffsets[i - 7];
    }
}

///
/// Summary: Get the texcoord offsets to be used inside the Star pixel shader.
///
/// Details: Get the texcoord offsets to be used inside the Star pixel shader.
///
/// Params: DWORD     - Texture size
///         XMVECTOR* - Pointer to coordinate offsets
///         XMVECTOR* - Pointer to color weights
///         FLOAT     - Deviation
///
/// Output: 
///
/// Notes:
///    - 3/21/2006 - [Shawn Halwes]: Initial revision
///
VOID GetSampleOffsets_Star( DWORD     dwTexSize, 
                                           XMVECTOR* pvTexCoordOffsets,
                                           XMVECTOR* pvColorWeights, 
                                           FLOAT     fDeviation )
{
    FLOAT tu = 1.0f / (FLOAT)dwTexSize;

    // Fill the center texel
    FLOAT fWeight        = 1.0f * GaussianDistribution( 0, 0, fDeviation );
    pvColorWeights[0]    = XMVectorSet( fWeight, fWeight, fWeight, 1.0f );
    pvTexCoordOffsets[0] = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );

    // Fill the first half
    for( DWORD i = 1; i < 8; i++ )
    {
        // Get the Gaussian intensity for this offset
        fWeight              = 1.0f * GaussianDistribution( (FLOAT)i, 0, fDeviation );
        pvColorWeights[i]    = XMVectorSet( fWeight, fWeight, fWeight, 1.0f );
        pvTexCoordOffsets[i] = XMVectorSet( i * tu, 0.0f, 0.0f, 0.0f );
    }

    // Mirror to the second half
    for( DWORD i = 8; i < 15; i++ )
    {
        pvColorWeights[i]    =  pvColorWeights[i - 7];
        pvTexCoordOffsets[i] = -pvTexCoordOffsets[i - 7];
    }
}
*/




//*****************************************************************************
/**
Constructor for the resource.
*/
//*****************************************************************************
XPostEffect::XPostEffect()
{
	m_viewportWidth = 0;
	m_viewportHeight = 0;
	m_enableRender = true;
	m_enableUpdate = true;
	m_pName = NULL;
}



//*****************************************************************************
/**
Destructor for the resource.
*/
//*****************************************************************************
XPostEffect::~XPostEffect()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XPostEffect::initStaticData()
{
	//Create constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(SampleData),true,m_sampleData);
	XVP::GraphicsMgr.createConstantBuffer(sizeof(HDRData),true,m_hdrData);

	//Create states
	XSamplerDesc samplerDesc;
	samplerDesc.filter = X_TFT_MIN_MAG_MIP_POINT;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_pointClampState);

	samplerDesc.filter = X_TFT_MIN_MAG_LINEAR_MIP_POINT;
	samplerDesc.addressU = X_TAT_CLAMP;
	samplerDesc.addressW = X_TAT_CLAMP;
	samplerDesc.addressV = X_TAT_CLAMP;
	XVP::GraphicsMgr.createSamplerState(samplerDesc,m_linearClampState);

	//Create shaders
	XVP::GraphicsMgr.createPixelShader("DownSample2x2.postfx","mainPS",m_downSample2x2PS);
	XVP::GraphicsMgr.createPixelShader("DownSample4x4.postfx","mainPS",m_downSample4x4PS);
	XVP::GraphicsMgr.createPixelShader("Copy.postfx","mainPS",m_copyPS);
	XVP::GraphicsMgr.createPixelShader("GaussianBlur5x5.postfx","mainPS",m_guassianBlur5x5PS);
	XVP::GraphicsMgr.createPixelShader("InitialLum.postfx","mainPS",m_initialLumPS);
	XVP::GraphicsMgr.createPixelShader("FinalLum.postfx","mainPS",m_finalLumPS);
	XVP::GraphicsMgr.createPixelShader("AdaptLum.postfx","mainPS",m_adaptLumPS);
	XVP::GraphicsMgr.createPixelShader("BrightPass.postfx","mainPS",m_brightPassPS);
	XVP::GraphicsMgr.createPixelShader("HDRTonemap.postfx","mainPS",m_hdrTonemapPS);
	XVP::GraphicsMgr.createPixelShader("Bloom.postfx","mainPS",m_bloomPS);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPostEffect::destroyStaticData()
{
	//Destroy constant buffers
	XVP::GraphicsMgr.destroyConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.destroyConstantBuffer(m_hdrData);

	//Destroy states
	XVP::GraphicsMgr.destroySamplerState(m_pointClampState);
	XVP::GraphicsMgr.destroySamplerState(m_linearClampState);

	//Destroy shaders
	XVP::GraphicsMgr.destroyPixelShader(m_downSample2x2PS);
	XVP::GraphicsMgr.destroyPixelShader(m_downSample4x4PS);
	XVP::GraphicsMgr.destroyPixelShader(m_copyPS);
	XVP::GraphicsMgr.destroyPixelShader(m_guassianBlur5x5PS);
	XVP::GraphicsMgr.destroyPixelShader(m_initialLumPS);
	XVP::GraphicsMgr.destroyPixelShader(m_finalLumPS);
	XVP::GraphicsMgr.destroyPixelShader(m_adaptLumPS);
	XVP::GraphicsMgr.destroyPixelShader(m_brightPassPS);
	XVP::GraphicsMgr.destroyPixelShader(m_hdrTonemapPS);
	XVP::GraphicsMgr.destroyPixelShader(m_bloomPS);

	return X_OK;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XPostEffect::init(XU32 viewportWidth,XU32 viewportHeight)
{
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPostEffect::destroy()
{
	XOSMgr::destroyString(&m_pName);
	return X_OK;
}



XStatus XPostEffect::prepForUpdate(XF32 dt,XScene* pScene)
{
	if (m_enableUpdate)
		return update(dt,pScene);
	else
		return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPostEffect::update(XF32 dt,XScene* pScene)
{
	return X_OK;
}



XStatus XPostEffect::prepForRender(XF32 dt,XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport)
{
	if (m_enableRender)
		return render( dt,pScene,
								   srcRenderTargetArray,
								   srcViewport,/*
								    srcMinU,
								    srcMinV,
								    srcMaxU,
								    srcMaxV,*/
								    destRenderTargetArray,
								    destViewport);
	else
		return X_ERR_NOT_RUNNING;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPostEffect::render(XF32 dt,XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport)
{
	return X_OK;
}



//*****************************************************************************
/**
Sets the name of the resource. 

@param pName	A string identifier for the resource.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XPostEffect::setName(XS8* pName)
{
	if (!pName)
		return X_ERR_INVALID_PARAMS;
	else
	{
		XOSMgr::destroyString(&m_pName);
		m_pName = XOSMgr::createString(pName);
		return X_OK;
	}
}



//*****************************************************************************
/**
Gets the name of the resource.

@return The string identifier of the resource.
*/
//*****************************************************************************
XS8* XPostEffect::getName()
{
	return m_pName;
}


XStatus XPostEffect::enableUpdate(XBOOL enable)
{
	m_enableUpdate = enable;
	return X_OK;
}


XBOOL XPostEffect::isUpdateEnabled()
{
	return m_enableUpdate;
}


XStatus XPostEffect::enableRender(XBOOL enable)
{
	m_enableRender = enable;
	return X_OK;
}


XBOOL XPostEffect::isRenderEnabled()
{
	return m_enableRender;
}


XStatus XPostEffect::handleResizeEvent(XU32 width,XU32 height)
{
	return X_OK;
}



XStatus XPostEffect::downSample2x2(XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport)
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
	XVP::GraphicsMgr.setPixelShader(m_downSample2x2PS);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	SampleData* pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
    GetSampleOffsets_DownScale2x2( srcViewport.width, srcViewport.height, pSampleData->offsets );
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_sampleData);

	return XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);

}



XStatus XPostEffect::downSample4x4(XScene* pScene,
								   XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport)
{
	XVP::GraphicsMgr.removeAllTextures();
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
	XVP::GraphicsMgr.setPixelShader(m_downSample4x4PS);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	SampleData* pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
    GetSampleOffsets_DownScale4x4( srcViewport.width,srcViewport.height,pSampleData->offsets );
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_sampleData);

	return XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);


}


XStatus XPostEffect::copy(XScene* pScene,
						XHandle srcRenderTargetArray,
								XViewport& srcViewport,
			   XHandle destRenderTargetArray,
			   XViewport& destViewport)
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
	XVP::GraphicsMgr.setPixelShader(m_copyPS);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	return XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);


}


XStatus XPostEffect::gaussianBlur5x5(XScene* pScene,
									 XHandle srcRenderTargetArray,
								   XViewport& srcViewport,
								   XHandle destRenderTargetArray,
								   XViewport& destViewport)
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(srcRenderTargetArray,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(destRenderTargetArray);
	XVP::GraphicsMgr.setPixelShader(m_guassianBlur5x5PS);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	SampleData* pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
	GetSampleOffsets_GaussBlur5x5(srcViewport.width,srcViewport.height, pSampleData->offsets, pSampleData->weights );
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_sampleData);

	return XVP::GraphicsMgr.renderViewportQuad(pScene,srcRenderTargetArray,srcViewport);


}

XStatus XPostEffect::measureLuminance(XScene* pScene,
									  XViewport& scaledSceneViewport,
									  XHandle m_pScaledSceneTexture,
									  XHandle m_pToneMapTexture64x64,
									  XHandle m_pToneMapTexture16x16,
									  XHandle m_pToneMapTexture4x4,
									  XHandle m_pToneMapTexture1x1)
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XViewport destViewport;
	destViewport.topLeftX = 0;
	destViewport.topLeftY = 0;	
	
	XViewport srcViewport;
	srcViewport.topLeftX = 0;
	srcViewport.topLeftY = 0;

	//Scale to 64x64
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(m_pScaledSceneTexture,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(m_pToneMapTexture64x64);
	XVP::GraphicsMgr.setPixelShader(m_initialLumPS);

	destViewport.width = 64;
	destViewport.height = 64;
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	SampleData* pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
	GetSampleOffsets_DownScale3x3( scaledSceneViewport.width, scaledSceneViewport.height, pSampleData->offsets );
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_sampleData);

	XVP::GraphicsMgr.renderViewportQuad(pScene,m_pScaledSceneTexture,scaledSceneViewport);

	//Scale to 16x16
	XVP::GraphicsMgr.removeRenderTargetArray();
	srcViewport.width = 64;
	srcViewport.height = 64;
	destViewport.width = 16;
	destViewport.height = 16;
	downSample4x4(pScene,m_pToneMapTexture64x64,srcViewport,m_pToneMapTexture16x16,destViewport);
	
	//Scale to 4x4
	XVP::GraphicsMgr.removeRenderTargetArray();
	srcViewport.width = 16;
	srcViewport.height = 16;
	destViewport.width = 4;
	destViewport.height = 4;
	downSample4x4(pScene,m_pToneMapTexture16x16,srcViewport,m_pToneMapTexture4x4,destViewport);

	//Scale to 1x1 and calculate average luminance
	XVP::GraphicsMgr.removeRenderTargetArray();
	srcViewport.width = 4;
	srcViewport.height = 4;
	destViewport.width = 1;
	destViewport.height = 1;

	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(m_pToneMapTexture4x4,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(m_pToneMapTexture1x1);
	XVP::GraphicsMgr.setPixelShader(m_finalLumPS);

	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
	GetSampleOffsets_DownScale4x4( srcViewport.width, srcViewport.height, pSampleData->offsets );
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_sampleData);

	XVP::GraphicsMgr.renderViewportQuad(pScene,m_pToneMapTexture4x4,srcViewport);


	return X_OK;
}



XStatus XPostEffect::adaptLuminance(XScene* pScene,
									XHandle pAdaptedLuminanceTexture, 
                                    XHandle pToneMapTexture, 
                                    FLOAT              fElapsedTime,
                                    FLOAT              fAdaptPercent,
									FLOAT			   fExposureClamp,
                                    XHandle pDstTexture )
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,1);
	XVP::GraphicsMgr.setRenderTargetAsTexture(pAdaptedLuminanceTexture,0,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(pToneMapTexture,0,1);
	
 	XVP::GraphicsMgr.setRenderTargetArray(pDstTexture);

	XViewport viewport;
	viewport.width = 1;
	viewport.height = 1;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	XVP::GraphicsMgr.setViewport(viewport);
	XVP::GraphicsMgr.setScissorRect(viewport);

    // Clamp adapt percent into valid range
    float fTemp = max( 0.001f, min( 1.0f, 1.0f - fAdaptPercent ) );

	HDRData* pData = (HDRData*)XVP::GraphicsMgr.lockConstantBuffer(m_hdrData,X_LT_DISCARD);
	pData->dt = fElapsedTime;
	pData->exposureClamp = fExposureClamp;
	pData->adaptPercent = fTemp;
	XVP::GraphicsMgr.unlockConstantBuffer(m_hdrData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_hdrData);

	XVP::GraphicsMgr.setPixelShader(m_adaptLumPS);


    // Create and set a render target
//  $  pePushRenderTarget( 0L, GetRenderTarget( pDstTexture, 0 ) );


  return XVP::GraphicsMgr.renderViewportQuad(pScene,pAdaptedLuminanceTexture,viewport);

}



XStatus XPostEffect::brightPassFilter(XScene* pScene,
									  XViewport& srcViewport,
										 XHandle pSrcTexture,
										 XHandle pAdaptedLuminanceTexture, 
                                         XF32              fMiddleGrayKeyValue,
                                         XF32              fBrightPassThreshold,
                                         XF32              fBrightPassOffset,
                                         XF32              fLuminanceWhite,
										 XHandle pDstBloomTexture,
										 XHandle pDstStarTexture )
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,1);
	XVP::GraphicsMgr.setRenderTargetAsTexture(pSrcTexture,0,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(pAdaptedLuminanceTexture,0,1);
	//pD3DDevice->SetTexture( 2, Get3DStencilTexture() );
	XVP::GraphicsMgr.setRenderTargetArray(pDstBloomTexture);
	XVP::GraphicsMgr.setViewport(srcViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,srcViewport.width,srcViewport.height));

    //pePushRenderTarget( 1, GetRenderTarget( pDstStarTexture, XGSurfaceSize( 320, 180, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE ) ) );

	XVP::GraphicsMgr.setPixelShader(m_brightPassPS);

   	/*SampleData* pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
	GetSampleOffsets_GaussBlur5x5(srcViewport.width,srcViewport.height, pSampleData->offsets, pSampleData->weights );
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_PS_USER_SLOT0,m_sampleData);*/


	HDRData* pBrightPassData = (HDRData*)XVP::GraphicsMgr.lockConstantBuffer(m_hdrData,X_LT_DISCARD);
	pBrightPassData->middleGray = fMiddleGrayKeyValue;
	pBrightPassData->brightPassThreshold = fBrightPassThreshold;
	pBrightPassData->brightPassOffset = fBrightPassOffset;
	pBrightPassData->luminanceWhite = fLuminanceWhite;
	XVP::GraphicsMgr.unlockConstantBuffer(m_hdrData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_hdrData);
  

	return XVP::GraphicsMgr.renderViewportQuad(pScene,pSrcTexture,srcViewport);
	//RenderAndResolveVpQuad( srcViewport, true, pDstBloomTexture, pDstStarTexture );
}


XStatus XPostEffect::tonemap(XScene* pScene,
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
										 XColor&           colorBalance)
{
	XVP::GraphicsMgr.removeRenderTargetArray();
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setSamplerState(m_linearClampState,1);
	XVP::GraphicsMgr.setSamplerState(m_linearClampState,2);
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,3);
	XVP::GraphicsMgr.setRenderTargetAsTexture(pSrcTexture,0,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(bloomTexture,0,1);
	XVP::GraphicsMgr.setRenderTargetAsTexture(starTexture,0,2);
	XVP::GraphicsMgr.setRenderTargetAsTexture(adaptedLumTexture,0,3);
	XVP::GraphicsMgr.setRenderTargetArray(pDestTexture);

	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));

	XVP::GraphicsMgr.setPixelShader(m_hdrTonemapPS);

   	HDRData* pHDRData = (HDRData*)XVP::GraphicsMgr.lockConstantBuffer(m_hdrData,X_LT_DISCARD);
	pHDRData->middleGray = fMiddleGrayKeyValue;
	pHDRData->luminanceWhite = fLuminanceWhite;
	pHDRData->bloomScale = bloomScale;
	pHDRData->starScale = starScale;
	pHDRData->brightness = brightness;
	pHDRData->contrast = contrast;
	pHDRData->desaturate = desaturate;
	pHDRData->colorBalance = colorBalance;
	XVP::GraphicsMgr.unlockConstantBuffer(m_hdrData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_hdrData);
 

	return XVP::GraphicsMgr.renderViewportQuad(pScene,pSrcTexture,srcViewport);
}



XStatus XPostEffect::bloomTexture(XScene* pScene,
								  XViewport&  viewport,
								  XHandle pSrcTexture,
                                  BOOL               bBloomAcrossWidth,
								  XViewport& destViewport,
                                  XHandle pDstTexture,
                                  FLOAT              fSize, 
                                  FLOAT              fBrightness )
{
	XVP::GraphicsMgr.removeRenderTargetArray();

	SampleData* pSampleData = (SampleData*)XVP::GraphicsMgr.lockConstantBuffer(m_sampleData,X_LT_DISCARD);
    if( bBloomAcrossWidth )
    {
        GetSampleOffsets_Bloom( viewport.width, viewport.height, 0.0f, pSampleData->offsets, pSampleData->weights, fSize, fBrightness );
    }
    else
    {
        GetSampleOffsets_Bloom( viewport.width, viewport.height, X_MATH_PIDIV2, pSampleData->offsets, pSampleData->weights, fSize, fBrightness );
    }
	XVP::GraphicsMgr.unlockConstantBuffer(m_sampleData);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT0,m_sampleData);

	XVP::GraphicsMgr.setPixelShader(m_bloomPS);
	XVP::GraphicsMgr.setSamplerState(m_pointClampState,0);
	XVP::GraphicsMgr.setRenderTargetAsTexture(pSrcTexture,0,0);
	XVP::GraphicsMgr.setRenderTargetArray(pDstTexture);
	XVP::GraphicsMgr.setViewport(destViewport);
	XVP::GraphicsMgr.setScissorRect(XRect(0,0,destViewport.width,destViewport.height));
	
	return XVP::GraphicsMgr.renderViewportQuad(pScene,pSrcTexture,viewport);
}

/*

XStatus XPostEffect::merge(XHandle* pSrcRenderTargetArrays,XHandle destRenderTargetArray,XU32 numSources)
{
}



XStatus XPostEffect::add(XHandle* pSrcRenderTargetArrays,XHandle destRenderTargetArray,XU32 numSources)
{
}*/