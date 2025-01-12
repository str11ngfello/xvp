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
//	Module Name:	XMaterial.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


#if 0
//*****************************************************************************
/**
Constructor for texture stage.  
*/
//*****************************************************************************
XTexStageState::XTexStageState()
{
	/*m_addressU = X_TAT_WRAP;
	m_addressV = X_TAT_WRAP;
	m_addressW = X_TAT_WRAP;
	m_magFilter = X_TFT_LINEAR;
	m_minFilter = X_TFT_LINEAR;
	m_mipFilter = X_TFT_LINEAR;*/

	D3D10_SAMPLER_DESC desc;
	desc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D10_FLOAT32_MAX;
	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
	desc.MaxAnisotropy = 0;
	XVP::GraphicsMgr.m_pD3DDevice->CreateSamplerState(&desc,&m_pSamplerState);
}



//*****************************************************************************
XTexStageState::~XTexStageState()
{
	SAFE_RELEASE(m_pSamplerState);
}



//*****************************************************************************
/**
Resets the texture stage parameters to default values.
*/
//*****************************************************************************
XStatus XTexStageState::resetToDefaults()
{
	SAFE_RELEASE(m_pSamplerState);
	XTexStageState();
	return X_OK;
}
#endif

XBOOL							XMaterial::m_globalLockTaken = false;
D3D10_DEPTH_STENCIL_DESC		XMaterial::m_dsDesc;
D3D10_BLEND_DESC				XMaterial::m_blendDesc;
D3D10_RASTERIZER_DESC			XMaterial::m_rasDesc;
D3D10_SAMPLER_DESC				XMaterial::m_samDesc[X_MAX_SAMPLERS];

//*****************************************************************************
//*****************************************************************************
XMaterial::XMaterial()
{
	m_pShaderController = NULL;
	m_pOrgShaderController = NULL;
	for (int i = 0;i < X_MAX_SAMPLERS;++i)
		m_textureHandles[i] = X_INVALID_HANDLE;	

	m_pDepthStencilState = NULL;
	m_stencilRefValue = 0;
	m_pRasterizerState = NULL;
	m_pBlendState = NULL;
	m_sampleMask = 0xFFFFFFFF;
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		m_samplerStates[i] = NULL;

	m_properties = 0;
	
	m_locked = false;
}



//*****************************************************************************
//*****************************************************************************
XMaterial::~XMaterial()
{
}



//*****************************************************************************
/**
Initializes an XMaterial object.  This function must be called before
the material can be used for rendering geometry.  By default, a render
state will set up a gray, texture-less material until further parameters
are given.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMaterial::init()
{
	//Call destroy first then create default states.
	destroy();
	createDefaultStates();

	//Allocate the appropriate size of texture stages.
	/*XNewArray(m_pTextureHandles,XHandle,XVP::GraphicsMgr.getMaxTextureStages(),XOSMgr::getDefaultHeap());
	if (!m_pTextureHandles)
		return X_ERR_MEMORY_EXHAUSTED;
	else
	{
		for (XU32 i = 0;i < XVP::GraphicsMgr.getMaxTextureStages();++i)
			m_pTextureHandles[i] = X_INVALID_HANDLE;
	}*/

	/*XDepthStencilDesc dssDesc;
	dssDesc.depthTestEnable = false;
	XVP::GraphicsMgr.createDepthStencilState(dssDesc,m_dssHandle);
	XRasterizerDesc rsDesc;
	XVP::GraphicsMgr.createRasterizerState(rsDesc,m_rsHandle);
	XBlendDesc bsDesc;
	XVP::GraphicsMgr.createBlendState(bsDesc,m_bsHandle);
	*/

	


	/*XNewArray(m_pTextureStates,XTexStageState,XVP::GraphicsMgr.getMaxTextureStages(),XOSMgr::getDefaultHeap());
	if (!m_pTextureStates)
		return X_ERR_MEMORY_EXHAUSTED;*/

	return X_OK;
}



//*****************************************************************************
/**
Initializes an XMaterial object.  This function must be called before
the material can be used for rendering geometry.  By default, a render
state will set up a gray, texture-less material until further parameters
are given.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMaterial::init(XS8* pMemoryChunk)
{
	//Call destroy first then create default states
	destroy();
	createDefaultStates();

	//Allocate the appropriate size of texture stages.
	/*XNewArray(m_pTextureHandles,XHandle,XVP::GraphicsMgr.getMaxTextureStages(),XOSMgr::getDefaultHeap());
	if (!m_pTextureHandles)
		return X_ERR_MEMORY_EXHAUSTED;
	else
	{
		for (XU32 i = 0;i < XVP::GraphicsMgr.getMaxTextureStages();++i)
			m_pTextureHandles[i] = X_INVALID_HANDLE;
	}
	XNewArray(m_pTextureStates,XTexStageState,XVP::GraphicsMgr.getMaxTextureStages(),XOSMgr::getDefaultHeap());
	if (!m_pTextureStates)
		return X_ERR_MEMORY_EXHAUSTED;*/
	

	/*XDepthStencilDesc dssDesc;
	XVP::GraphicsMgr.createDepthStencilState(dssDesc,m_dssHandle);
	XRasterizerDesc rsDesc;
	XVP::GraphicsMgr.createRasterizerState(rsDesc,m_rsHandle);
	XBlendDesc bsDesc;
	XVP::GraphicsMgr.createBlendState(bsDesc,m_bsHandle);*/

	//createDefaultStates();
	

	//Material name
	XU32 nameLength = *(XU32*)pMemoryChunk; pMemoryChunk += sizeof(XU32); 
	pMemoryChunk += nameLength; 

	//Get shader name and create the correct shader controller
	nameLength = *(XU32*)pMemoryChunk; pMemoryChunk += sizeof(XU32); 
	
	//Let's determine if this is a shader template by looking for a '#'.  For example,
	//Eyebal.fx#Model-BlinnPhong.fx
	XS8* pDerivedName = pMemoryChunk;
	XS8* pTemplateName = pMemoryChunk;
	if (strstr(pMemoryChunk,"#"))
	{
		XS8* nextLineToken = NULL;
		XS8* pDerivedName = strtok_s(pMemoryChunk,"#",&pTemplateName);
	}

	XShaderController* pShaderController = XVP::ShaderFactory.createObject(pTemplateName);
	pShaderController->init(pDerivedName);

	//Attach the shader controller
	pShaderController->prepForAttachment();
	m_pOrgShaderController = pShaderController;
	m_pShaderController = pShaderController;

	
	pMemoryChunk += nameLength; 

	//Lock material and set properties
	lock();
	enableDepthTest(*(XBOOL*)pMemoryChunk);						pMemoryChunk += sizeof(XBOOL);
	enableDepthWrite(*(XBOOL*)pMemoryChunk);					pMemoryChunk += sizeof(XBOOL);
	setDepthFunc(*(X_CMP_FUNC_TYPE*)pMemoryChunk);				pMemoryChunk += sizeof(X_CMP_FUNC_TYPE);
	setDepthBias(*(XS32*)pMemoryChunk);							pMemoryChunk += sizeof(XS32);
	setDepthBiasClamp(*(XF32*)pMemoryChunk);					pMemoryChunk += sizeof(XF32);
	setSlopeScaledDepthBias(*(XF32*)pMemoryChunk);				pMemoryChunk += sizeof(XF32);
	enableDepthClip(*(XBOOL*)pMemoryChunk);						pMemoryChunk += sizeof(XBOOL);
	setFillMode(*(X_FILL_MODE_TYPE*)pMemoryChunk);				pMemoryChunk += sizeof(X_FILL_MODE_TYPE);
	enableScissor(*(XBOOL*)pMemoryChunk);						pMemoryChunk += sizeof(XBOOL);
	enableMultisample(*(XBOOL*)pMemoryChunk);					pMemoryChunk += sizeof(XBOOL);
	enableAntialiasedLine(*(XBOOL*)pMemoryChunk);				pMemoryChunk += sizeof(XBOOL);
	enableAlphaToCoverage(*(XBOOL*)pMemoryChunk);				pMemoryChunk += sizeof(XBOOL);
	XBOOL alphaBlend = *(XBOOL*)pMemoryChunk;					pMemoryChunk += sizeof(XBOOL);
	for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)
		enableAlphaBlend(i,alphaBlend); 
	setBlendOp(*(X_BLEND_OP_TYPE*)pMemoryChunk);				pMemoryChunk += sizeof(X_BLEND_OP_TYPE);
	setSrcBlend(*(X_BLEND_TYPE*)pMemoryChunk);					pMemoryChunk += sizeof(X_BLEND_TYPE);
	setDestBlend(*(X_BLEND_TYPE*)pMemoryChunk);					pMemoryChunk += sizeof(X_BLEND_TYPE);
	setBlendOpAlpha(*(X_BLEND_OP_TYPE*)pMemoryChunk);			pMemoryChunk += sizeof(X_BLEND_OP_TYPE);
	setSrcBlendAlpha(*(X_BLEND_TYPE*)pMemoryChunk);				pMemoryChunk += sizeof(X_BLEND_TYPE);
	setDestBlendAlpha(*(X_BLEND_TYPE*)pMemoryChunk);			pMemoryChunk += sizeof(X_BLEND_TYPE);

	//Look for material parameters
	XU32 numTextures = *(XU32*)pMemoryChunk; pMemoryChunk += sizeof(XU32);
	XHandle texHandle = X_INVALID_HANDLE;
	for (XU32 i = 0;i < numTextures;++i)
	{
		nameLength = *(XU32*)pMemoryChunk; pMemoryChunk += sizeof(XU32);
		//if (nameLength != 0)
	//	{
		XVP::GraphicsMgr.createTextureFromFile(pMemoryChunk,texHandle,true);
		pMemoryChunk += nameLength; 
	//	}
		m_textureHandles[i] = texHandle;
			//setTextureHandle(samplerIndex,texHandle);
			
		setSamplerAddressU(i,*(X_TEXTURE_ADDRESS_TYPE*)pMemoryChunk);	pMemoryChunk += sizeof(X_TEXTURE_ADDRESS_TYPE);
		setSamplerAddressV(i,*(X_TEXTURE_ADDRESS_TYPE*)pMemoryChunk);	pMemoryChunk += sizeof(X_TEXTURE_ADDRESS_TYPE);
		setSamplerAddressW(i,*(X_TEXTURE_ADDRESS_TYPE*)pMemoryChunk);	pMemoryChunk += sizeof(X_TEXTURE_ADDRESS_TYPE);
		setSamplerMipLODBias(i,*(XF32*)pMemoryChunk);					pMemoryChunk += sizeof(XF32);
		setSamplerMaxAnisotropy(i,*(XU32*)pMemoryChunk);					pMemoryChunk += sizeof(XU32);
		setSamplerFunc(i,*(X_CMP_FUNC_TYPE*)pMemoryChunk);				pMemoryChunk += sizeof(X_CMP_FUNC_TYPE);
		setSamplerMinLOD(i,*(XF32*)pMemoryChunk);						pMemoryChunk += sizeof(XF32);
		setSamplerMaxLOD(i,*(XF32*)pMemoryChunk);						pMemoryChunk += sizeof(XF32);
		setSamplerBorderColor(i,*(XColor*)pMemoryChunk);					pMemoryChunk += sizeof(XColor);
		setSamplerFilter(i,*(X_TEXTURE_FILTER_TYPE*)pMemoryChunk);		pMemoryChunk += sizeof(X_TEXTURE_FILTER_TYPE);
	}

	//Set shader param block
	pShaderController->setShaderParamBlock(pMemoryChunk);

	unlock();

	return X_OK;
}



//*****************************************************************************
/**
Destroys the material by freeing all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMaterial::destroy()
{	
	//Clean up state objects
	/*if (m_dssHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyDepthStencilState(m_dssHandle);
	if (m_rsHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyRasterizerState(m_rsHandle);
	if (m_bsHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyBlendState(m_bsHandle);*/

	//If the material owns a shader controller, free it
	XDelete(m_pOrgShaderController);
	m_pShaderController = NULL;

	//Release any previous objects
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pBlendState);
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		SAFE_RELEASE(m_samplerStates[i]);

	//Clean up the texture slots 
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		if (m_textureHandles[i] != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.destroyTexture(m_textureHandles[i]);
			m_textureHandles[i] = X_INVALID_HANDLE;
		}
		//if (m_samplerHandles[i] != X_INVALID_HANDLE)
		//	XVP::GraphicsMgr.destroySamplerState(m_samplerHandles[i]);
	}
	//XDeleteArray(m_pTextureHandles);
	//XDeleteArray(m_pTextureStates);

	//Clean the client list.
	//m_renderEventList.clear();

	XResource::destroy();

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::lock()
{
	X_ASSERTM(!m_globalLockTaken,"Only one material across all threads can be locked.  This assert was caused by a second material trying to take the global lock.");
	X_ASSERTM(!m_locked,"Material is already locked.  It cannot be locked again.");
	X_ASSERTM(m_pDepthStencilState,"Material can't be locked before default states have been created.");

	m_locked = true;
	m_globalLockTaken = true;

	//Get the current state objects and store them so that subsequent
	//state changes get recorded.  When unlocked, the state changes
	//will be used to re-create the state objects
	m_pDepthStencilState->GetDesc(&m_dsDesc);
	m_pRasterizerState->GetDesc(&m_rasDesc);
	m_pBlendState->GetDesc(&m_blendDesc);
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		m_samplerStates[i]->GetDesc(&m_samDesc[i]);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::unlock()
{
	X_ASSERTM(m_locked,"Material is not locked.  It cannot be unlocked.");

	m_locked = false;
	m_globalLockTaken = false;

	//Re-create state objects with the new description structs that contain
	//any recorded state changes
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pBlendState);
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		SAFE_RELEASE(m_samplerStates[i]);
	
	HRESULT hr = XVP::GraphicsMgr.m_pD3DDevice->CreateDepthStencilState(&m_dsDesc,&m_pDepthStencilState);
	X_ASSERTM(hr == S_OK,"Failed to create depth stencil state during unlock.");

	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateBlendState(&m_blendDesc,&m_pBlendState);
	X_ASSERTM(hr == S_OK,"Failed to create blend state during unlock.");
	
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateRasterizerState(&m_rasDesc,&m_pRasterizerState);
	X_ASSERTM(hr == S_OK,"Failed to create rasterizer state during unlock.");
	
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateSamplerState(&m_samDesc[i],&m_samplerStates[i]);
		X_ASSERTM(hr == S_OK,"Failed to create sampler state during unlock.");
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::apply()
{
	//Set state objects
	//XVP::GraphicsMgr.setDepthStencilState(m_dssHandle);
	//XVP::GraphicsMgr.setRasterizerState(m_rsHandle);
	//XVP::GraphicsMgr.setBlendState(m_bsHandle);
	XVP::GraphicsMgr.m_pD3DDevice->OMSetDepthStencilState(m_pDepthStencilState,m_stencilRefValue);
	XVP::GraphicsMgr.m_pD3DDevice->RSSetState(m_pRasterizerState);
	XVP::GraphicsMgr.m_pD3DDevice->OMSetBlendState(m_pBlendState,(XF32*)&m_blendFactor,m_sampleMask);

	//Run through the texture array and set the textures and associated
	//texture stage states.
	for (XU32 i = 0;i < X_MAX_SAMPLERS;i++)
	{
		if (m_textureHandles[i] != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.setTexture(m_textureHandles[i],i);
			XVP::GraphicsMgr.m_pD3DDevice->PSSetSamplers(i,1,&m_samplerStates[i]);
			
			//XVP::GraphicsMgr.m_pD3DDevice->PSSetSamplers(i,1,&m_pTextureStates[i].m_pSamplerState);
			//Now set this stages states.
			/*XVP::GraphicsMgr.setAddressU(i,(X_TEXTURE_ADDRESS_TYPE)m_pTextureStates[i].m_addressU);
			XVP::GraphicsMgr.setAddressV(i,(X_TEXTURE_ADDRESS_TYPE)m_pTextureStates[i].m_addressW);
			XVP::GraphicsMgr.setAddressW(i,(X_TEXTURE_ADDRESS_TYPE)m_pTextureStates[i].m_addressW);
			XVP::GraphicsMgr.setMagFilter(i,(X_TEXTURE_FILTER_TYPE)m_pTextureStates[i].m_magFilter);
			XVP::GraphicsMgr.setMinFilter(i,(X_TEXTURE_FILTER_TYPE)m_pTextureStates[i].m_minFilter);
			XVP::GraphicsMgr.setMipFilter(i,(X_TEXTURE_FILTER_TYPE)m_pTextureStates[i].m_mipFilter);*/
		}
		else
			break;
			//@@@@ Can't just blindly remove textures because we could have an override by a render level (i.e depth buffer)
			//XVP::GraphicsMgr.removeTexture(i);
	}

	//Everything went okay.
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::render(XRenderEvent* pRenderEvent)
{
	X_ASSERTM(pRenderEvent,"Invalid render event");

	if (m_pShaderController)
		return m_pShaderController->render(pRenderEvent);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::createDefaultStates()
{
	//Release any previous objects
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pBlendState);
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		SAFE_RELEASE(m_samplerStates[i]);
	
	//Create the D3D depth stencil state
	HRESULT hr;
	D3D10_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D10_COMPARISON_LESS;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
	m_stencilRefValue = 0;
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateDepthStencilState(&dsDesc,&m_pDepthStencilState);
	X_ASSERTM(hr == S_OK,"Failed to create depth stencil state.");

	//Create blend state
	D3D10_BLEND_DESC blendDesc;
	for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)
	{
		blendDesc.BlendEnable[i] = false;
		blendDesc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL;
	}
	blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	blendDesc.AlphaToCoverageEnable = false;
	m_blendFactor = XColor(1.0f,1.0f,1.0f,1.0f);
	m_sampleMask = 0xFFFFFFFF;
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateBlendState(&blendDesc,&m_pBlendState);
	X_ASSERTM(hr == S_OK,"Failed to create blend state.");

	//Create rasterizer state
	D3D10_RASTERIZER_DESC rasDesc;
    rasDesc.FillMode = D3D10_FILL_SOLID;
    rasDesc.CullMode = D3D10_CULL_BACK;
    rasDesc.FrontCounterClockwise = false;
    rasDesc.DepthBias = 0;
    rasDesc.DepthBiasClamp = 0.0f;
    rasDesc.SlopeScaledDepthBias = 0.0f;
    rasDesc.DepthClipEnable = true;
    rasDesc.ScissorEnable = true;
    rasDesc.MultisampleEnable = true;
    rasDesc.AntialiasedLineEnable = false;
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateRasterizerState(&rasDesc,&m_pRasterizerState);
	X_ASSERTM(hr == S_OK,"Failed to create rasterizer state.");

	//Create sampler states
	D3D10_SAMPLER_DESC desc;
	desc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D10_FLOAT32_MAX;
	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
	desc.MaxAnisotropy = 1;
	for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
	{
		hr = XVP::GraphicsMgr.m_pD3DDevice->CreateSamplerState(&desc,&m_samplerStates[i]);
		X_ASSERTM(hr == S_OK,"Failed to create sampler state.");
	}

	return X_OK;
}


//*****************************************************************************
/**
Renders all registered scene nodes.  This function will notify each registered
scene node that it should render itself.  Once this function completes,
all scene node registrations will be cleared.

@param matHandle	The handle of the material for which to render its clients.
@return None.
*/
//*****************************************************************************
/*XStatus XMaterial::render(XHandle matHandle)
{
	
}*/
	




/*

//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setDiffuse(XColor& diffuse)
{
	m_diffuse = diffuse;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setAmbient(XColor& ambient)
{
	m_ambient = ambient;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSpecular(XColor& specular)
{
	m_specular = specular;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSpecPower(XF32 specPower)
{
	m_specPower = specPower;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XColor& XMaterial::getDiffuse()
{
	return m_diffuse;
}



//*****************************************************************************
//*****************************************************************************
XColor& XMaterial::getAmbient()
{
	return m_ambient;
}



//*****************************************************************************
//*****************************************************************************
XColor& XMaterial::getSpecular()
{
	return m_specular;
}



//*****************************************************************************
//*****************************************************************************
XF32 XMaterial::getSpecPower()
{
	return m_specPower;
}
*/


XStatus XMaterial::attachShaderController(XShaderController* pShaderController)
{
	X_ASSERTM(pShaderController,"Invalid shader controller");

	//Do nothing if we already have this shader controller
	if (m_pShaderController == pShaderController)
		return X_OK;

	//Okay, the shader controller is being replaced, detach the old one and attach
	//the new one and remember nulls are allowed
	if (m_pShaderController)
		m_pShaderController->prepForDetachment();
	
	m_pShaderController = pShaderController;

	if (m_pShaderController)
		pShaderController->prepForAttachment();
	
	return X_OK;
}

XShaderController* XMaterial::getShaderController()
{
	return m_pShaderController;
}

//*****************************************************************************
/**
Sets up an initialized texture within the XMaterial object.  A handle
to the texture being replaced is returned to allow the application to destroy
it if necessary.

@param	samplerIndex	The texture stage for which to set a texture.
@param	texHandle	A handle to an initialized texture.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XMaterial::setTextureHandle(XU32 samplerIndex,XHandle texHandle)
{
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index index out of range.");

	//Destroy the old texture handle and create the new one. Don't forget to bump 
	//the reference count
	if (m_textureHandles[samplerIndex] != X_INVALID_HANDLE)
		XVP::GraphicsMgr.destroyTexture(m_textureHandles[samplerIndex]);

	m_textureHandles[samplerIndex] = texHandle;

	if (texHandle != X_INVALID_HANDLE)
		XVP::GraphicsMgr.addTextureRef(texHandle);

	return X_OK;
}



//*****************************************************************************
/**
Gets the texture associated with a particular texture stage.

@param samplerIndex	The texture stage from which to retrieve the associated texture.
@return A handle to the texture.  This value may be X_INVALID_HANDLE if no
		texture has been associated with the passed in texture stage.
*/
//*****************************************************************************
XHandle XMaterial::getTextureHandle(XU32 samplerIndex)
{
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index index out of range.");

	//Get the texture index.
	return m_textureHandles[samplerIndex];
}


#if 0
//*****************************************************************************
/**
Sets the fill mode for the material.

@param value	Fill mode.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setFillMode(X_FILL_MODE_TYPE value)
{
	m_fillMode = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the fill mode for the material.

@return Fill mode.
*/
//*****************************************************************************
X_FILL_MODE_TYPE XMaterial::getFillMode()
{
	return (X_FILL_MODE_TYPE)m_fillMode;
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableAlphaTest(XBOOL enable)
{
	if (enable)
		m_properties |= X_RSP_ALPHA_TEST;
	else
		m_properties &= ~X_RSP_ALPHA_TEST;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isAlphaTestEnabled()
{
	return (XBOOL)(m_properties & X_RSP_ALPHA_TEST);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableAlphaBlend(XBOOL enable)
{
	if (enable)
		m_properties |= X_RSP_ALPHA_BLEND;
	else
		m_properties &= ~X_RSP_ALPHA_BLEND;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isAlphaBlendEnabled()
{
	return (XBOOL)(m_properties & X_RSP_ALPHA_BLEND);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableDepthTest(XBOOL enable)
{
	if (enable)
		m_properties |= X_RSP_DEPTH_TEST;
	else
		m_properties &= ~X_RSP_DEPTH_TEST;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isDepthTestEnabled()
{
	return (XBOOL)(m_properties & X_RSP_DEPTH_TEST);
}


//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableDepthWrite(XBOOL enable)
{
	if (enable)
		m_properties |= X_RSP_DEPTH_WRITE;
	else
		m_properties &= ~X_RSP_DEPTH_WRITE;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isDepthWriteEnabled()
{
	return (XBOOL)(m_properties & X_RSP_DEPTH_WRITE);
}



//*****************************************************************************
/**
Sets the source blend type for the material.

@param value	Source blend type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setSrcBlend(X_BLEND_TYPE value)
{
	m_srcBlend = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the source blend type for the material.

@return Source blend type.
*/
//*****************************************************************************
X_BLEND_TYPE XMaterial::getSrcBlend()
{
	return (X_BLEND_TYPE)m_srcBlend;
}



//*****************************************************************************
/**
Sets the destination blend type for the material.

@param value	Destination blend type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setDestBlend(X_BLEND_TYPE value)
{
	m_destBlend = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the destination blend type for the material.

@return Destination blend type.
*/
//*****************************************************************************
X_BLEND_TYPE XMaterial::getDestBlend()
{
	return (X_BLEND_TYPE)m_destBlend;
}



//*****************************************************************************
/**
Sets the depth function for the material.

@param value	Depth function type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setDepthFunc(X_CMP_FUNC_TYPE value)
{
	m_depthFunc = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the depth function type for the material.

@return Depth function type.
*/
//*****************************************************************************
X_CMP_FUNC_TYPE XMaterial::getDepthFunc()
{
	return (X_CMP_FUNC_TYPE)m_depthFunc;
}



//*****************************************************************************
/**
Sets the alpha function for the material.

@param value	Alpha function type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setAlphaFunc(X_CMP_FUNC_TYPE value)
{
	m_alphaFunc = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the alpha function type for the material.

@return Alpha function type.
*/
//*****************************************************************************
X_CMP_FUNC_TYPE XMaterial::getAlphaFunc()
{
	return (X_CMP_FUNC_TYPE)m_alphaFunc;
}



//*****************************************************************************
/**
Sets the alpha reference value for the material.

@param value	Alpha reference value.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setAlphaRef(XU32 value)
{
	m_alphaRef = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the alpha reference value for the material.

@return Alpha reference value.
*/
//*****************************************************************************
XU32 XMaterial::getAlphaRef()
{
	return m_alphaRef;
}



//*****************************************************************************
/**
Sets the depth bias for the material.

@param value	Depth bias.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setDepthBias(XF32 value)
{
	m_depthBias = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the depth bias for the material.

@return Depth bias.
*/
//*****************************************************************************
XF32 XMaterial::getDepthBias()
{
	return m_depthBias;
}



//*****************************************************************************
/**
Sets the U texture addressing type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@param value	Texture addressing type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setAddressU(XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value)
{
	m_pTextureStates[stageNum].m_addressU = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the U texture addressing mode for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@return Texture addressing type.
*/
//*****************************************************************************
X_TEXTURE_ADDRESS_TYPE XMaterial::getAddressU(XU32 stageNum)
{
	return (X_TEXTURE_ADDRESS_TYPE)m_pTextureStates[stageNum].m_addressU;
}



//*****************************************************************************
/**
Sets the V texture addressing type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@param value	Texture addressing type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setAddressV(XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value)
{
	m_pTextureStates[stageNum].m_addressV = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the V texture addressing type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@return Texture addressing type.
*/
//*****************************************************************************
X_TEXTURE_ADDRESS_TYPE XMaterial::getAddressV(XU32 stageNum)
{
	return (X_TEXTURE_ADDRESS_TYPE)m_pTextureStates[stageNum].m_addressV;
}



//*****************************************************************************
/**
Sets the W texture addressing type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@param value	Texture addressing type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setAddressW(XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value)
{
	m_pTextureStates[stageNum].m_addressW = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the W texture addressing type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@return Texture addressing type.
*/
//*****************************************************************************
X_TEXTURE_ADDRESS_TYPE XMaterial::getAddressW(XU32 stageNum)
{
	return (X_TEXTURE_ADDRESS_TYPE)m_pTextureStates[stageNum].m_addressW;
}



//*****************************************************************************
/**
Sets the magnification filter for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@param value	Magnification filter.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setMagFilter(XU32 stageNum,X_TEXTURE_FILTER_TYPE value)
{
	m_pTextureStates[stageNum].m_magFilter = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the magnification filter for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@return Magnification filter.
*/
//*****************************************************************************
X_TEXTURE_FILTER_TYPE XMaterial::getMagFilter(XU32 stageNum)
{
	return (X_TEXTURE_FILTER_TYPE)m_pTextureStates[stageNum].m_magFilter;
}



//*****************************************************************************
/**
Sets the minification filter type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@param value	Minification filter.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setMinFilter(XU32 stageNum,X_TEXTURE_FILTER_TYPE value)
{
	m_pTextureStates[stageNum].m_minFilter = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the minification filter for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@return Minification filter.
*/
//*****************************************************************************
X_TEXTURE_FILTER_TYPE XMaterial::getMinFilter(XU32 stageNum)
{
	return (X_TEXTURE_FILTER_TYPE)m_pTextureStates[stageNum].m_minFilter;
}



//*****************************************************************************
/**
Sets the mip filter type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@param value	Mip filter type.
@return None.
*/
//*****************************************************************************
XStatus XMaterial::setMipFilter(XU32 stageNum,X_TEXTURE_FILTER_TYPE value)
{
	m_pTextureStates[stageNum].m_mipFilter = value;
	return X_OK;
}



//*****************************************************************************
/**
Gets the mip filter type for a texture stage in the material.

@param stageNum	The texture stage to operate on.
@return Mip filter type.
*/
//*****************************************************************************
X_TEXTURE_FILTER_TYPE XMaterial::getMipFilter(XU32 stageNum)
{
	return (X_TEXTURE_FILTER_TYPE)m_pTextureStates[stageNum].m_mipFilter;
}
#endif


//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableDepthTest(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.DepthEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isDepthTestEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_dsDesc.DepthEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableDepthWrite(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	if (enable)
		m_dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	else
		m_dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isDepthWriteEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	
	if (m_dsDesc.DepthWriteMask == D3D10_DEPTH_WRITE_MASK_ALL)
		return true;
	else
		return false;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setDepthFunc(X_CMP_FUNC_TYPE cmpFunc)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.DepthFunc = translateComparisonFuncType(cmpFunc);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_CMP_FUNC_TYPE XMaterial::getDepthFunc()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateComparisonFuncType(m_dsDesc.DepthFunc);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableStencilTest(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.StencilEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isStencilTestEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_dsDesc.StencilEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilReadMask(XU8 readMask)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.StencilReadMask = readMask;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU8 XMaterial::getStencilReadMask()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_dsDesc.StencilReadMask;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilWriteMask(XU8 writeMask)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.StencilWriteMask = writeMask;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU8 XMaterial::getStencilWriteMask()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_dsDesc.StencilWriteMask;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilFailOpFrontFace(X_STENCIL_OP_TYPE opType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.FrontFace.StencilFailOp = translateStencilOpType(opType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_STENCIL_OP_TYPE XMaterial::getStencilFailOpFrontFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateStencilOpType(m_dsDesc.FrontFace.StencilFailOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilDepthFailOpFrontFace(X_STENCIL_OP_TYPE opType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.FrontFace.StencilDepthFailOp = translateStencilOpType(opType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_STENCIL_OP_TYPE XMaterial::getStencilDepthFailOpFrontFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateStencilOpType(m_dsDesc.FrontFace.StencilDepthFailOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilPassOpFrontFace(X_STENCIL_OP_TYPE opType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.FrontFace.StencilPassOp = translateStencilOpType(opType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_STENCIL_OP_TYPE XMaterial::getStencilPassOpFrontFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateStencilOpType(m_dsDesc.FrontFace.StencilPassOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilFuncFrontFace(X_CMP_FUNC_TYPE cmpFunc)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.FrontFace.StencilFunc = translateComparisonFuncType(cmpFunc);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_CMP_FUNC_TYPE XMaterial::getStencilFuncFrontFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateComparisonFuncType(m_dsDesc.FrontFace.StencilFunc);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilFailOpBackFace(X_STENCIL_OP_TYPE opType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.BackFace.StencilFailOp = translateStencilOpType(opType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_STENCIL_OP_TYPE XMaterial::getStencilFailOpBackFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateStencilOpType(m_dsDesc.BackFace.StencilFailOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilDepthFailOpBackFace(X_STENCIL_OP_TYPE opType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.BackFace.StencilDepthFailOp = translateStencilOpType(opType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_STENCIL_OP_TYPE XMaterial::getStencilDepthFailOpBackFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateStencilOpType(m_dsDesc.BackFace.StencilDepthFailOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilPassOpBackFace(X_STENCIL_OP_TYPE opType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.BackFace.StencilPassOp = translateStencilOpType(opType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_STENCIL_OP_TYPE XMaterial::getStencilPassOpBackFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateStencilOpType(m_dsDesc.BackFace.StencilPassOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilFuncBackFace(X_CMP_FUNC_TYPE cmpFunc)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_dsDesc.BackFace.StencilFunc = translateComparisonFuncType(cmpFunc);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_CMP_FUNC_TYPE XMaterial::getStencilFuncBackFace()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateComparisonFuncType(m_dsDesc.BackFace.StencilFunc);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setStencilRefValue(XU32 refValue)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_stencilRefValue = refValue;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XMaterial::getStencilRefValue()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_stencilRefValue;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableAlphaToCoverage(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.AlphaToCoverageEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isAlphaToCoverageEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_blendDesc.AlphaToCoverageEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableAlphaBlend(XU32 renderTargetIndex,XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(renderTargetIndex < X_MAX_RENDER_TARGETS,"Render target index out of range.");
	if (enable)
		m_properties |= X_MP_ALPHA_BLEND;
	else
		m_properties &= ~X_MP_ALPHA_BLEND;
	m_blendDesc.BlendEnable[renderTargetIndex] = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isAlphaBlendEnabled(XU32 renderTargetIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(renderTargetIndex < X_MAX_RENDER_TARGETS,"Render target index out of range.");
	return (m_blendDesc.BlendEnable[renderTargetIndex] == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSrcBlend(X_BLEND_TYPE blendType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.SrcBlend = translateBlendType(blendType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_BLEND_TYPE XMaterial::getSrcBlend()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateBlendType(m_blendDesc.SrcBlend);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setDestBlend(X_BLEND_TYPE blendType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.DestBlend = translateBlendType(blendType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_BLEND_TYPE XMaterial::getDestBlend()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateBlendType(m_blendDesc.DestBlend);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setBlendOp(X_BLEND_OP_TYPE blendOpType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.BlendOp = translateBlendOpType(blendOpType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_BLEND_OP_TYPE XMaterial::getBlendOp()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateBlendOpType(m_blendDesc.BlendOp);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSrcBlendAlpha(X_BLEND_TYPE blendType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.SrcBlendAlpha = translateBlendType(blendType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_BLEND_TYPE XMaterial::getSrcBlendAlpha()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateBlendType(m_blendDesc.SrcBlendAlpha);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setDestBlendAlpha(X_BLEND_TYPE blendType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.DestBlendAlpha = translateBlendType(blendType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_BLEND_TYPE XMaterial::getDestBlendAlpha()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateBlendType(m_blendDesc.DestBlendAlpha);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setBlendOpAlpha(X_BLEND_OP_TYPE blendOpType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendDesc.BlendOpAlpha = translateBlendOpType(blendOpType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_BLEND_OP_TYPE XMaterial::getBlendOpAlpha()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateBlendOpType(m_blendDesc.BlendOpAlpha);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setRenderTargetWriteMask(XU32 renderTargetIndex,X_COLOR_WRITE_FLAGS flags)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(renderTargetIndex < X_MAX_RENDER_TARGETS,"Render target index out of range.");
	m_blendDesc.RenderTargetWriteMask[renderTargetIndex] = flags;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_COLOR_WRITE_FLAGS XMaterial::getRenderTargetWriteMask(XU32 renderTargetIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(renderTargetIndex < X_MAX_RENDER_TARGETS,"Render target index out of range.");
	return (X_COLOR_WRITE_FLAGS)m_blendDesc.RenderTargetWriteMask[renderTargetIndex];
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setBlendFactor(XColor& blendFactor)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_blendFactor = blendFactor;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XColor XMaterial::getBlendFactor()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_blendFactor;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSampleMask(XU32 sampleMask)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_sampleMask = sampleMask;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XMaterial::getSampleMask()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_sampleMask;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setFillMode(X_FILL_MODE_TYPE fillMode)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.FillMode = translateFillModeType(fillMode);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_FILL_MODE_TYPE XMaterial::getFillMode()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateFillModeType(m_rasDesc.FillMode);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setCullMode(X_CULL_TYPE cullMode)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.CullMode = translateCullType(cullMode);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_CULL_TYPE XMaterial::getCullMode()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return translateCullType(m_rasDesc.CullMode);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setFrontCounterClockwise(XBOOL frontIsCC)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.FrontCounterClockwise = frontIsCC;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isFrontCounterClockwise()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_rasDesc.FrontCounterClockwise == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setDepthBias(XS32 depthBias)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.DepthBias = depthBias;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XS32 XMaterial::getDepthBias()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_rasDesc.DepthBias;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setDepthBiasClamp(XF32 depthBiasClamp)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.DepthBiasClamp = depthBiasClamp;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XMaterial::getDepthBiasClamp()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_rasDesc.DepthBiasClamp;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSlopeScaledDepthBias(XF32 slopeScaledDepthBias)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.SlopeScaledDepthBias = slopeScaledDepthBias;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XMaterial::getSlopeScaledDepthBias()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return m_rasDesc.SlopeScaledDepthBias;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableDepthClip(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.DepthClipEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isDepthClipEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_rasDesc.DepthClipEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableScissor(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.ScissorEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isScissorEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_rasDesc.ScissorEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableMultisample(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.MultisampleEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isMultisampleEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_rasDesc.MultisampleEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::enableAntialiasedLine(XBOOL enable)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	m_rasDesc.AntialiasedLineEnable = enable;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XMaterial::isAntialiasedLineEnabled()
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	return (m_rasDesc.AntialiasedLineEnable == 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerFilter(XU32 samplerIndex,X_TEXTURE_FILTER_TYPE filterType)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].Filter = translateTextureFilterType(filterType);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_TEXTURE_FILTER_TYPE XMaterial::getSamplerFilter(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return translateTextureFilterType(m_samDesc[samplerIndex].Filter);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerAddressU(XU32 samplerIndex,X_TEXTURE_ADDRESS_TYPE addressU)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].AddressU = translateTextureAddressType(addressU);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_TEXTURE_ADDRESS_TYPE XMaterial::getSamplerAddressU(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return translateTextureAddressType(m_samDesc[samplerIndex].AddressU);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerAddressV(XU32 samplerIndex,X_TEXTURE_ADDRESS_TYPE addressV)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].AddressV = translateTextureAddressType(addressV);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_TEXTURE_ADDRESS_TYPE XMaterial::getSamplerAddressV(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return translateTextureAddressType(m_samDesc[samplerIndex].AddressV);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerAddressW(XU32 samplerIndex,X_TEXTURE_ADDRESS_TYPE addressW)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].AddressW = translateTextureAddressType(addressW);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_TEXTURE_ADDRESS_TYPE XMaterial::getSamplerAddressW(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return translateTextureAddressType(m_samDesc[samplerIndex].AddressW);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerMipLODBias(XU32 samplerIndex,XF32 bias)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].MipLODBias = bias;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XMaterial::getSamplerMipLODBias(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return m_samDesc[samplerIndex].MipLODBias;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerFunc(XU32 samplerIndex,X_CMP_FUNC_TYPE cmpFunc)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].ComparisonFunc = translateComparisonFuncType(cmpFunc);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
X_CMP_FUNC_TYPE XMaterial::getSamplerFunc(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return translateComparisonFuncType(m_samDesc[samplerIndex].ComparisonFunc);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerMaxAnisotropy(XU32 samplerIndex,XU32 maxAnisotropy)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].MaxAnisotropy = maxAnisotropy;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XMaterial::getSamplerMaxAnisotropy(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return m_samDesc[samplerIndex].MaxAnisotropy;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerBorderColor(XU32 samplerIndex,XColor& borderColor)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].BorderColor[0] = borderColor.m_R;
	m_samDesc[samplerIndex].BorderColor[1] = borderColor.m_G;
	m_samDesc[samplerIndex].BorderColor[2] = borderColor.m_B;
	m_samDesc[samplerIndex].BorderColor[3] = borderColor.m_A;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XColor XMaterial::getSamplerBorderColor(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return XColor(m_samDesc[samplerIndex].BorderColor[0],
				  m_samDesc[samplerIndex].BorderColor[1],
				  m_samDesc[samplerIndex].BorderColor[2],
				  m_samDesc[samplerIndex].BorderColor[3]);
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerMinLOD(XU32 samplerIndex,XF32 minLOD)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].MinLOD = minLOD;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XMaterial::getSamplerMinLOD(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return m_samDesc[samplerIndex].MinLOD;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setSamplerMaxLOD(XU32 samplerIndex,XF32 maxLOD)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	m_samDesc[samplerIndex].MaxLOD = maxLOD;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XF32 XMaterial::getSamplerMaxLOD(XU32 samplerIndex)
{
	X_ASSERTM(m_locked,"Cannot modify/query material state without locking the material first.");
	X_ASSERTM(samplerIndex < X_MAX_SAMPLERS,"Sampler index is out of range.");
	return m_samDesc[samplerIndex].MaxLOD;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setEnumProperty(XS8* pPropertyName,XS32 value)
{
	lock();
	PB_MAKE_PROPERTY("FillMode",setFillMode((X_FILL_MODE_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("DepthFunc",setDepthFunc((X_CMP_FUNC_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("BlendOp",setBlendOp((X_BLEND_OP_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("SrcBlend",setSrcBlend((X_BLEND_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("DestBlend",setDestBlend((X_BLEND_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("BlendOpAlpha",setBlendOpAlpha((X_BLEND_OP_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("SrcBlendAlpha",setSrcBlendAlpha((X_BLEND_TYPE)value);unlock();)
	PB_MAKE_PROPERTY("DestBlendAlpha",setDestBlendAlpha((X_BLEND_TYPE)value);unlock();)
	unlock();

	return X_ERR_NO_SUCH_OBJECT;
}


//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setBoolProperty(XS8* pPropertyName,XBOOL value)
{
	lock();
	PB_MAKE_PROPERTY("DepthTest",enableDepthTest(value);unlock();)
	PB_MAKE_PROPERTY("DepthWrite",enableDepthWrite(value);unlock();)
	PB_MAKE_PROPERTY("Scissor",enableScissor(value);unlock();)
	PB_MAKE_PROPERTY("Multisample",enableMultisample(value);unlock();)
	PB_MAKE_PROPERTY("AntialiasedLine",enableAntialiasedLine(value);unlock();)
	PB_MAKE_PROPERTY("AlphaToCoverage",enableAlphaToCoverage(value);unlock();)
	if (strcmp("AlphaBlend",pPropertyName) == 0)
	{
		for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)
			enableAlphaBlend(i,value);
		unlock();
		return X_OK;
	}
	unlock();

	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setIntProperty(XS8* pPropertyName,XS32 value)
{
	lock();
	PB_MAKE_PROPERTY("DepthBias",setDepthBias(value);unlock();)
	unlock();

	return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterial::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	lock();
	PB_MAKE_PROPERTY("DepthBiasClamp",setDepthBiasClamp(value);unlock();)
	PB_MAKE_PROPERTY("SlopeScaledDepthBias",setSlopeScaledDepthBias(value);unlock();)
	unlock();

	return X_ERR_NO_SUCH_OBJECT;
}