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
//	Module Name:	XMaterial.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MATERIAL_H__
#define __X_MATERIAL_H__

#include <XOSMgr.h>
#include <XColor.h>
#include <XSceneNode.h>
#include <XResource.h>
#include <XGeometry.h>
#include <XShaderController.h>
#include <XMemBlockList.h>
#include <d3d10.h>



//*****************************************************************************
//Material Property Defines
//*****************************************************************************
enum X_MATERIAL_PROPERTY 
{
	X_MP_ALPHA_BLEND			=	0x01,
};


#if 0
//*****************************************************************************
//*****************************************************************************
class XTexStageState
{
public:
	/*XU8					m_addressU;
	XU8					m_addressV;
	XU8					m_addressW;
	XU8					m_magFilter;
	XU8					m_minFilter;
	XU8					m_mipFilter;*/

	ID3D10SamplerState* m_pSamplerState; 

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTexStageState();
	~XTexStageState();
	
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus resetToDefaults();
};
#endif


//*****************************************************************************
//! Material Object.
//*****************************************************************************
class XMaterial : public XResource, public XPropertyBag
{
public:
	

private:

	enum ShaderParamType
	{
		X_SPT_TEX2D,
		X_SPT_TEX3D,
		X_SPT_TEXCUBE,
		X_SPT_BOOL,
		X_SPT_ENUM,
		X_SPT_INT,
		X_SPT_UINT,
		X_SPT_FLOAT,
		X_SPT_VEC2,
		X_SPT_VEC3,
		X_SPT_VEC4
	};

	XShaderController*	m_pOrgShaderController;
	XShaderController*	m_pShaderController;

	XHandle				m_textureHandles[X_MAX_SAMPLERS];
	//XHandle				m_samplerHandles[X_MAX_SAMPLERS];	
	//XHandle				m_dssHandle;
	//XHandle				m_rsHandle;
	//XHandle				m_bsHandle;
	

	//Currently only one material across all threads can be locked at a time.
	//This can be changed by using a thread synchronized free list to
	//allow material instances to use one of several sets of description
	//structures.  
	static XBOOL									m_globalLockTaken;
	static D3D10_DEPTH_STENCIL_DESC					m_dsDesc;
	static D3D10_BLEND_DESC							m_blendDesc;
	static D3D10_RASTERIZER_DESC					m_rasDesc;
	static D3D10_SAMPLER_DESC						m_samDesc[X_MAX_SAMPLERS];
	
	
	ID3D10DepthStencilState*						m_pDepthStencilState;
	ID3D10RasterizerState*							m_pRasterizerState;
	ID3D10BlendState*								m_pBlendState;
	ID3D10SamplerState*								m_samplerStates[X_MAX_SAMPLERS];
	XU32											m_stencilRefValue;
	XColor											m_blendFactor;
	XS32											m_sampleMask;

	XU8					m_properties;	
	XBOOL				m_locked;
	//XU32				m_refCount;			
	/*XU8						m_fillMode;
	XU8						m_srcBlend;			
	XU8						m_destBlend;
	XU8						m_alphaRef;
	XU8						m_alphaFunc;
	XU8						m_depthFunc;
	XF32					m_depthBias;
	XU8						m_blendOp;*/

	//XMemBlockList<XRenderEvent>	m_renderEventList;

	XStatus createDefaultStates();
	D3D10_COMPARISON_FUNC translateComparisonFuncType(X_CMP_FUNC_TYPE cmpFunc);
	X_CMP_FUNC_TYPE translateComparisonFuncType(D3D10_COMPARISON_FUNC  cmpFunc);
	D3D10_STENCIL_OP translateStencilOpType(X_STENCIL_OP_TYPE stenilOp);
	X_STENCIL_OP_TYPE translateStencilOpType(D3D10_STENCIL_OP stenilOp);
	D3D10_BLEND translateBlendType(X_BLEND_TYPE blendType);
	X_BLEND_TYPE translateBlendType(D3D10_BLEND blendType);
	D3D10_BLEND_OP translateBlendOpType(X_BLEND_OP_TYPE blendType);
	X_BLEND_OP_TYPE translateBlendOpType(D3D10_BLEND_OP blendType);
	D3D10_CULL_MODE translateCullType(X_CULL_TYPE cullType);
	X_CULL_TYPE translateCullType(D3D10_CULL_MODE cullType);
	D3D10_FILL_MODE translateFillModeType(X_FILL_MODE_TYPE fillModeType);
	X_FILL_MODE_TYPE translateFillModeType(D3D10_FILL_MODE fillModeType);
	D3D10_FILTER translateTextureFilterType(X_TEXTURE_FILTER_TYPE filter);
	X_TEXTURE_FILTER_TYPE translateTextureFilterType(D3D10_FILTER filter);
	D3D10_TEXTURE_ADDRESS_MODE translateTextureAddressType(X_TEXTURE_ADDRESS_TYPE address);
	X_TEXTURE_ADDRESS_TYPE translateTextureAddressType(D3D10_TEXTURE_ADDRESS_MODE address);


public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMaterial();
	~XMaterial();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init();
	XStatus init(XS8* pMemoryChunk);
	XStatus destroy();
	XStatus lock();
	XStatus unlock();
	XStatus apply();
	XStatus render(XRenderEvent* pRenderEvent);
	/*XStatus setAmbient(XColor& ambient);
	XColor& getAmbient();
	XStatus setDiffuse(XColor& diffuse);
	XColor& getDiffuse();
	XStatus setSpecular(XColor& specular);
	XColor& getSpecular();
	XStatus setSpecPower(XF32 specPower);
	XF32 getSpecPower();*/
	XStatus attachShaderController(XShaderController* pShaderController);
	XShaderController* getShaderController();
	XStatus setTextureHandle(XU32 samplerIndex,XHandle texHandle);
	XHandle getTextureHandle(XU32 samplerIndex);

	//Depth API
	XStatus enableDepthTest(XBOOL enable);
	XBOOL isDepthTestEnabled();
	XStatus enableDepthWrite(XBOOL enable);
	XBOOL isDepthWriteEnabled();
	XStatus setDepthFunc(X_CMP_FUNC_TYPE cmpFunc);
	X_CMP_FUNC_TYPE getDepthFunc();

	//Stencil API
	XStatus enableStencilTest(XBOOL enable);
	XBOOL isStencilTestEnabled();
	XStatus setStencilReadMask(XU8 readMask);
	XU8 getStencilReadMask();
	XStatus setStencilWriteMask(XU8 writeMask);
	XU8 getStencilWriteMask();
	XStatus setStencilFailOpFrontFace(X_STENCIL_OP_TYPE opType);
	X_STENCIL_OP_TYPE getStencilFailOpFrontFace();
	XStatus setStencilDepthFailOpFrontFace(X_STENCIL_OP_TYPE opType);
	X_STENCIL_OP_TYPE getStencilDepthFailOpFrontFace();
	XStatus setStencilPassOpFrontFace(X_STENCIL_OP_TYPE opType);
	X_STENCIL_OP_TYPE getStencilPassOpFrontFace();
	XStatus setStencilFuncFrontFace(X_CMP_FUNC_TYPE cmpFunc);
	X_CMP_FUNC_TYPE getStencilFuncFrontFace();
	XStatus setStencilFailOpBackFace(X_STENCIL_OP_TYPE opType);
	X_STENCIL_OP_TYPE getStencilFailOpBackFace();
	XStatus setStencilDepthFailOpBackFace(X_STENCIL_OP_TYPE opType);
	X_STENCIL_OP_TYPE getStencilDepthFailOpBackFace();
	XStatus setStencilPassOpBackFace(X_STENCIL_OP_TYPE opType);
	X_STENCIL_OP_TYPE getStencilPassOpBackFace();
	XStatus setStencilFuncBackFace(X_CMP_FUNC_TYPE cmpFunc);
	X_CMP_FUNC_TYPE getStencilFuncBackFace();
	XStatus setStencilRefValue(XU32 refValue);
	XU32 getStencilRefValue();

	//Blending API
	XStatus enableAlphaToCoverage(XBOOL enable);
	XBOOL isAlphaToCoverageEnabled();
	XStatus enableAlphaBlend(XU32 renderTargetIndex,XBOOL enable);
	XBOOL isAlphaBlendEnabled(XU32 renderTargetIndex);
	XStatus setSrcBlend(X_BLEND_TYPE blendType);
	X_BLEND_TYPE getSrcBlend();
	XStatus setDestBlend(X_BLEND_TYPE blendType);
	X_BLEND_TYPE getDestBlend();
	XStatus setBlendOp(X_BLEND_OP_TYPE blendOpType);
	X_BLEND_OP_TYPE getBlendOp();
	XStatus setSrcBlendAlpha(X_BLEND_TYPE blendType);
	X_BLEND_TYPE getSrcBlendAlpha();
	XStatus setDestBlendAlpha(X_BLEND_TYPE blendType);
	X_BLEND_TYPE getDestBlendAlpha();
	XStatus setBlendOpAlpha(X_BLEND_OP_TYPE blendOpType);
	X_BLEND_OP_TYPE getBlendOpAlpha();
	XStatus setRenderTargetWriteMask(XU32 renderTargetIndex,X_COLOR_WRITE_FLAGS flags);
	X_COLOR_WRITE_FLAGS getRenderTargetWriteMask(XU32 renderTargetIndex);
	XStatus setBlendFactor(XColor& blendFactor);
	XColor getBlendFactor();
	XStatus setSampleMask(XU32 sampleMask);
	XU32 getSampleMask();

	//Rasterizer
	XStatus setFillMode(X_FILL_MODE_TYPE fillMode);
	X_FILL_MODE_TYPE getFillMode();
	XStatus setCullMode(X_CULL_TYPE cullMode);
	X_CULL_TYPE getCullMode();
	XStatus setFrontCounterClockwise(XBOOL frontIsCC);
	XBOOL isFrontCounterClockwise();
	XStatus setDepthBias(XS32 depthBias);
	XS32 getDepthBias();
	XStatus setDepthBiasClamp(XF32 depthBiasClamp);
	XF32 getDepthBiasClamp();
	XStatus setSlopeScaledDepthBias(XF32 depthBiasClamp);
	XF32 getSlopeScaledDepthBias();

	XStatus enableDepthClip(XBOOL enable);
	XBOOL isDepthClipEnabled();
	XStatus enableScissor(XBOOL enable);
	XBOOL isScissorEnabled();
	XStatus enableMultisample(XBOOL enable);
	XBOOL isMultisampleEnabled();
	XStatus enableAntialiasedLine(XBOOL enable);
	XBOOL isAntialiasedLineEnabled();

	//Sampler API
	XStatus setSamplerFilter(XU32 samplerIndex,X_TEXTURE_FILTER_TYPE filterType);
	X_TEXTURE_FILTER_TYPE getSamplerFilter(XU32 samplerIndex);
	XStatus setSamplerAddressU(XU32 samplerIndex,X_TEXTURE_ADDRESS_TYPE addressU);
	X_TEXTURE_ADDRESS_TYPE getSamplerAddressU(XU32 samplerIndex);
	XStatus setSamplerAddressV(XU32 samplerIndex,X_TEXTURE_ADDRESS_TYPE addressV);
	X_TEXTURE_ADDRESS_TYPE getSamplerAddressV(XU32 samplerIndex);
	XStatus setSamplerAddressW(XU32 samplerIndex,X_TEXTURE_ADDRESS_TYPE addressW);
	X_TEXTURE_ADDRESS_TYPE getSamplerAddressW(XU32 samplerIndex);
	XStatus setSamplerMipLODBias(XU32 samplerIndex,XF32 bias);
	XF32 getSamplerMipLODBias(XU32 samplerIndex);
	XStatus setSamplerFunc(XU32 samplerIndex,X_CMP_FUNC_TYPE cmpFunc);
	X_CMP_FUNC_TYPE getSamplerFunc(XU32 samplerIndex);
	XStatus setSamplerMaxAnisotropy(XU32 samplerIndex,XU32 maxAnisotropy);
	XU32 getSamplerMaxAnisotropy(XU32 samplerIndex);
	XStatus setSamplerBorderColor(XU32 samplerIndex,XColor& borderColor);
	XColor getSamplerBorderColor(XU32 samplerIndex);
	XStatus setSamplerMinLOD(XU32 samplerIndex,XF32 minLOD);
	XF32 getSamplerMinLOD(XU32 samplerIndex);
	XStatus setSamplerMaxLOD(XU32 samplerIndex,XF32 maxLOD);
	XF32 getSamplerMaxLOD(XU32 samplerIndex);

	



	
	/*
	XStatus setDepthBias(XF32 value);
	XF32 getDepthBias();
	
	XStatus setSrcBlend(X_BLEND_TYPE value);
	X_BLEND_TYPE getSrcBlend();
	XStatus setDestBlend(X_BLEND_TYPE value);
	X_BLEND_TYPE getDestBlend();
	XStatus setAlphaFunc(X_CMP_FUNC_TYPE value);
	X_CMP_FUNC_TYPE getAlphaFunc();
	XStatus setAlphaRef(XU32 value);
	XU32 getAlphaRef();
	XStatus setBlendOp(X_BLEND_OP_TYPE value);
	X_BLEND_OP_TYPE getBlendOp();
	XStatus setAddressU(XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value);
	X_TEXTURE_ADDRESS_TYPE getAddressU(XU32 stageNum);
    XStatus setAddressV(XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value);
	X_TEXTURE_ADDRESS_TYPE getAddressV(XU32 stageNum);
	XStatus setAddressW(XU32 stageNum,X_TEXTURE_ADDRESS_TYPE value);
	X_TEXTURE_ADDRESS_TYPE getAddressW(XU32 stageNum);	
	XStatus setMagFilter(XU32 stageNum,X_TEXTURE_FILTER_TYPE value);
	X_TEXTURE_FILTER_TYPE getMagFilter(XU32 stageNum);
	XStatus setMinFilter(XU32 stageNum,X_TEXTURE_FILTER_TYPE value);
	X_TEXTURE_FILTER_TYPE getMinFilter(XU32 stageNum);
   	XStatus setMipFilter(XU32 stageNum,X_TEXTURE_FILTER_TYPE value);
	X_TEXTURE_FILTER_TYPE getMipFilter(XU32 stageNum);  */
	
	XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
	XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
	XStatus setIntProperty(XS8* pPropertyName,XS32 value);
	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);


};






//*****************************************************************************
//*****************************************************************************
inline D3D10_COMPARISON_FUNC XMaterial::translateComparisonFuncType(X_CMP_FUNC_TYPE cmpFunc)
{
	switch (cmpFunc)
	{
	case X_CFT_UNKNOWN:
		X_ASSERT_FORCED("Unknown comparison function type.  Defaulting to never.");
		return D3D10_COMPARISON_NEVER;
	case X_CFT_NEVER:
		return D3D10_COMPARISON_NEVER;
	case X_CFT_LESS:
		return D3D10_COMPARISON_LESS;
	case X_CFT_EQUAL:
		return D3D10_COMPARISON_EQUAL;
	case X_CFT_LESS_EQUAL:
		return D3D10_COMPARISON_LESS_EQUAL;
	case X_CFT_GREATER:
		return D3D10_COMPARISON_GREATER;
	case X_CFT_NOT_EQUAL:
		return D3D10_COMPARISON_NOT_EQUAL;
	case X_CFT_GREATER_EQUAL:
		return D3D10_COMPARISON_GREATER_EQUAL;
	case X_CFT_ALWAYS:
		return D3D10_COMPARISON_ALWAYS;
	default:
		X_ASSERT_FORCED("Unrecognized texture address type.  Defaulting to never.");
		return D3D10_COMPARISON_NEVER;
	}
}


//*****************************************************************************
//*****************************************************************************
inline X_CMP_FUNC_TYPE XMaterial::translateComparisonFuncType(D3D10_COMPARISON_FUNC cmpFunc)
{
	switch (cmpFunc)
	{
	case D3D10_COMPARISON_NEVER:
		return X_CFT_NEVER;
	case D3D10_COMPARISON_LESS:
		return X_CFT_LESS;
	case D3D10_COMPARISON_EQUAL:
		return X_CFT_EQUAL;
	case D3D10_COMPARISON_LESS_EQUAL:
		return X_CFT_LESS_EQUAL;
	case D3D10_COMPARISON_GREATER:
		return X_CFT_GREATER;
	case D3D10_COMPARISON_NOT_EQUAL:
		return X_CFT_NOT_EQUAL;
	case D3D10_COMPARISON_GREATER_EQUAL:
		return X_CFT_GREATER_EQUAL;
	case D3D10_COMPARISON_ALWAYS:
		return X_CFT_ALWAYS;
	default:
		X_ASSERT_FORCED("Unrecognized texture address type.  Defaulting to never.");
		return X_CFT_NEVER;
	}
}



//*****************************************************************************
//*****************************************************************************
inline D3D10_STENCIL_OP XMaterial::translateStencilOpType(X_STENCIL_OP_TYPE stenilOp)
{
	switch (stenilOp)
	{
	case X_SOT_UNKNOWN:
		X_ASSERT_FORCED("Unknown stenil op type.  Defaulting to keep.");
		return D3D10_STENCIL_OP_KEEP;
	case X_SOT_KEEP:
		return D3D10_STENCIL_OP_KEEP;
	case X_SOT_ZERO:
		return D3D10_STENCIL_OP_ZERO;
	case X_SOT_REPLACE:
		return D3D10_STENCIL_OP_REPLACE;
	case X_SOT_INCRSAT:
		return D3D10_STENCIL_OP_INCR_SAT;
	case X_SOT_DECRSAT:
		return D3D10_STENCIL_OP_DECR_SAT;
	case X_SOT_INVERT:
		return D3D10_STENCIL_OP_INVERT;
	case X_SOT_INCR:
		return D3D10_STENCIL_OP_INCR;
	case X_SOT_DECR:
		return D3D10_STENCIL_OP_DECR;
	default:
		X_ASSERT_FORCED("Unrecognized stencil op type.  Defaulting to keep.");
		return D3D10_STENCIL_OP_KEEP;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_STENCIL_OP_TYPE XMaterial::translateStencilOpType(D3D10_STENCIL_OP stenilOp)
{
	switch (stenilOp)
	{
	case D3D10_STENCIL_OP_KEEP:
		return X_SOT_KEEP;
	case D3D10_STENCIL_OP_ZERO:
		return X_SOT_ZERO;
	case D3D10_STENCIL_OP_REPLACE:
		return X_SOT_REPLACE;
	case D3D10_STENCIL_OP_INCR_SAT:
		return X_SOT_INCRSAT;
	case D3D10_STENCIL_OP_DECR_SAT:
		return X_SOT_DECRSAT;
	case D3D10_STENCIL_OP_INVERT:
		return X_SOT_INVERT;
	case D3D10_STENCIL_OP_INCR:
		return X_SOT_INCR;
	case D3D10_STENCIL_OP_DECR:
		return X_SOT_DECR;
	default:
		X_ASSERT_FORCED("Unrecognized stencil op type.  Defaulting to keep.");
		return X_SOT_KEEP;
	}
}



//*****************************************************************************
//*****************************************************************************
inline D3D10_BLEND XMaterial::translateBlendType(X_BLEND_TYPE blendType)
{
	switch (blendType)
	{
	case X_BT_UNKNOWN:
		X_ASSERT_FORCED("Unknown blend type.  Defaulting to one.");
		return D3D10_BLEND_ONE;
	case X_BT_ZERO:
		return D3D10_BLEND_ZERO;
	case X_BT_ONE:
		return D3D10_BLEND_ONE;
	case X_BT_SRC_COLOR:
		return D3D10_BLEND_SRC_COLOR;
	case X_BT_INV_SRC_COLOR:
		return D3D10_BLEND_INV_SRC_COLOR;
	case X_BT_SRC_ALPHA:
		return D3D10_BLEND_SRC_ALPHA;
	case X_BT_INV_SRC_ALPHA:
		return D3D10_BLEND_INV_SRC_ALPHA;
	case X_BT_DEST_ALPHA:
		return D3D10_BLEND_DEST_ALPHA;
	case X_BT_INV_DEST_ALPHA:
		return D3D10_BLEND_INV_DEST_ALPHA;
	case X_BT_DEST_COLOR:
		return D3D10_BLEND_DEST_COLOR;
	case X_BT_INV_DEST_COLOR:
		return D3D10_BLEND_INV_DEST_COLOR;
	case X_BT_SRC_ALPHA_SAT:
		return D3D10_BLEND_SRC_ALPHA_SAT;
	case X_BT_BLEND_FACTOR:
		return D3D10_BLEND_BLEND_FACTOR;
	case X_BT_INV_BLEND_FACTOR:
		return D3D10_BLEND_INV_BLEND_FACTOR;
	case X_BT_SRC1_COLOR:
		return D3D10_BLEND_SRC1_COLOR;
	case X_BT_INV_SRC1_COLOR:
		return D3D10_BLEND_INV_SRC1_COLOR;
	case X_BT_SRC1_ALPHA:
		return D3D10_BLEND_SRC1_ALPHA;
	case X_BT_INV_SRC1_ALPHA:
		return D3D10_BLEND_INV_SRC1_ALPHA;
	default:
		X_ASSERT_FORCED("Unrecognized blend type.  Defaulting to one.");
		return D3D10_BLEND_ONE;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_BLEND_TYPE XMaterial::translateBlendType(D3D10_BLEND blendType)
{
	switch (blendType)
	{
	case D3D10_BLEND_ZERO:
		return X_BT_ZERO;
	case D3D10_BLEND_ONE:
		return X_BT_ONE;
	case D3D10_BLEND_SRC_COLOR:
		return X_BT_SRC_COLOR;
	case D3D10_BLEND_INV_SRC_COLOR:
		return X_BT_INV_SRC_COLOR;
	case D3D10_BLEND_SRC_ALPHA:
		return X_BT_SRC_ALPHA;
	case D3D10_BLEND_INV_SRC_ALPHA:
		return X_BT_INV_SRC_ALPHA;
	case D3D10_BLEND_DEST_ALPHA:
		return X_BT_DEST_ALPHA;
	case D3D10_BLEND_INV_DEST_ALPHA:
		return X_BT_INV_DEST_ALPHA;
	case D3D10_BLEND_DEST_COLOR:
		return X_BT_DEST_COLOR;
	case D3D10_BLEND_INV_DEST_COLOR:
		return X_BT_INV_DEST_COLOR;
	case D3D10_BLEND_SRC_ALPHA_SAT:
		return X_BT_SRC_ALPHA_SAT;
	case D3D10_BLEND_BLEND_FACTOR:
		return X_BT_BLEND_FACTOR;
	case D3D10_BLEND_INV_BLEND_FACTOR:
		return X_BT_INV_BLEND_FACTOR;
	case D3D10_BLEND_SRC1_COLOR:
		return X_BT_SRC1_COLOR;
	case D3D10_BLEND_INV_SRC1_COLOR:
		return X_BT_INV_SRC1_COLOR;
	case D3D10_BLEND_SRC1_ALPHA:
		return X_BT_SRC1_ALPHA;
	case D3D10_BLEND_INV_SRC1_ALPHA:
		return X_BT_INV_SRC1_ALPHA;
	default:
		X_ASSERT_FORCED("Unrecognized blend type.  Defaulting to one.");
		return X_BT_ONE;
	}
}



//*****************************************************************************
//*****************************************************************************
inline D3D10_BLEND_OP XMaterial::translateBlendOpType(X_BLEND_OP_TYPE blendType)
{
	switch (blendType)
	{
	case X_BOT_UNKNOWN:
		X_ASSERT_FORCED("Unknown blend op type.  Defaulting to add.");
		return D3D10_BLEND_OP_ADD;
	case X_BOT_ADD:
		return D3D10_BLEND_OP_ADD;
	case X_BOT_SUBTRACT:
		return D3D10_BLEND_OP_SUBTRACT;
	case X_BOT_REVSUBTRACT:
		return D3D10_BLEND_OP_REV_SUBTRACT;
	case X_BOT_MIN:
		return D3D10_BLEND_OP_MIN;
	case X_BOT_MAX:
		return D3D10_BLEND_OP_MAX;
	default:
		X_ASSERT_FORCED("Unrecognized blend op type.  Defaulting to add.");
		return D3D10_BLEND_OP_ADD;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_BLEND_OP_TYPE XMaterial::translateBlendOpType(D3D10_BLEND_OP blendType)
{
	switch (blendType)
	{
	case D3D10_BLEND_OP_ADD:
		return X_BOT_ADD;
	case D3D10_BLEND_OP_SUBTRACT:
		return X_BOT_SUBTRACT;
	case D3D10_BLEND_OP_REV_SUBTRACT:
		return X_BOT_REVSUBTRACT;
	case D3D10_BLEND_OP_MIN:
		return X_BOT_MIN;
	case D3D10_BLEND_OP_MAX:
		return X_BOT_MAX;
	default:
		X_ASSERT_FORCED("Unrecognized blend op type.  Defaulting to add.");
		return X_BOT_ADD;
	}
}



//*****************************************************************************
//*****************************************************************************
inline D3D10_CULL_MODE XMaterial::translateCullType(X_CULL_TYPE cullType)
{
	switch (cullType)
	{
	case X_CULLT_UNKNOWN:
		X_ASSERT_FORCED("Unknown cull mode type.  Defaulting to back.");
		return D3D10_CULL_BACK;
	case X_CULLT_NONE:
		return D3D10_CULL_NONE;
	case X_CULLT_FRONT:
		return D3D10_CULL_FRONT;
	case X_CULLT_BACK:
		return D3D10_CULL_BACK;
	default:
		X_ASSERT_FORCED("Unrecognized cull mode type.  Defaulting to back.");
		return D3D10_CULL_BACK;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_CULL_TYPE XMaterial::translateCullType(D3D10_CULL_MODE cullType)
{
	switch (cullType)
	{
	case D3D10_CULL_NONE:
		return X_CULLT_NONE;
	case D3D10_CULL_FRONT:
		return X_CULLT_FRONT;
	case D3D10_CULL_BACK:
		return X_CULLT_BACK;
	default:
		X_ASSERT_FORCED("Unrecognized cull mode type.  Defaulting to back.");
		return X_CULLT_BACK;
	}
}



//*****************************************************************************
//*****************************************************************************
inline D3D10_FILL_MODE XMaterial::translateFillModeType(X_FILL_MODE_TYPE fillModeType)
{
	switch (fillModeType)
	{
	case X_FMT_UNKNOWN:
		X_ASSERT_FORCED("Unknown fill mode type.  Defaulting to solid.");
		return D3D10_FILL_SOLID;
	case X_FMT_WIRE:
		return D3D10_FILL_WIREFRAME;
	case X_FMT_SOLID:
		return D3D10_FILL_SOLID;
	default:
		X_ASSERT_FORCED("Unrecognized fill mode type.  Defaulting to solid.");
		return D3D10_FILL_SOLID;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_FILL_MODE_TYPE XMaterial::translateFillModeType(D3D10_FILL_MODE fillModeType)
{
	switch (fillModeType)
	{
	case D3D10_FILL_WIREFRAME:
		return X_FMT_WIRE;
	case D3D10_FILL_SOLID:
		return X_FMT_SOLID;
	default:
		X_ASSERT_FORCED("Unrecognized fill mode type.  Defaulting to solid.");
		return X_FMT_SOLID;
	}
}



//*****************************************************************************
//*****************************************************************************
inline D3D10_FILTER XMaterial::translateTextureFilterType(X_TEXTURE_FILTER_TYPE filter)
{
	switch (filter)
	{
	case X_TFT_UNKNOWN:
		X_ASSERT_FORCED("Unknown texture filter type.  Defaulting to a linear filter.");
		return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	case X_TFT_MIN_MAG_MIP_POINT:
		return D3D10_FILTER_MIN_MAG_MIP_POINT;
	case X_TFT_MIN_MAG_POINT_MIP_LINEAR:
		return D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	case X_TFT_MIN_POINT_MAG_LINEAR_MIP_POINT:
		return D3D10_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case X_TFT_MIN_POINT_MAG_MIP_LINEAR:
		return D3D10_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	case X_TFT_MIN_LINEAR_MAG_MIP_POINT:
		return D3D10_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	case X_TFT_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		return D3D10_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case X_TFT_MIN_MAG_LINEAR_MIP_POINT:
		return D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	case X_TFT_MIN_MAG_MIP_LINEAR:
		return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	case X_TFT_ANISOTROPIC:
		return D3D10_FILTER_ANISOTROPIC;
	case X_TFT_COMPARISON_MIN_MAG_MIP_POINT:
		return D3D10_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	case X_TFT_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
		return D3D10_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
	case X_TFT_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
		return D3D10_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case X_TFT_COMPARISON_MIN_POINT_MAG_MIP_LINEAR0x85:
		return D3D10_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
	case X_TFT_COMPARISON_MIN_LINEAR_MAG_MIP_POINT0x90:
		return D3D10_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
	case X_TFT_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		return D3D10_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case X_TFT_COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
		return D3D10_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	case X_TFT_COMPARISON_MIN_MAG_MIP_LINEAR:
		return D3D10_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	case X_TFT_COMPARISON_ANISOTROPIC:
		return D3D10_FILTER_COMPARISON_ANISOTROPIC;
	case X_TFT_TEXT_1BIT:
		return D3D10_FILTER_TEXT_1BIT;
	default:
		X_ASSERT_FORCED("Unrecognized texture filter type. Defaulting to a linear filter.");
		return D3D10_FILTER_MIN_MAG_MIP_LINEAR;
	}
}
		


//*****************************************************************************
//*****************************************************************************
inline X_TEXTURE_FILTER_TYPE XMaterial::translateTextureFilterType(D3D10_FILTER filter)
{
	switch (filter)
	{
	case D3D10_FILTER_MIN_MAG_MIP_POINT:
		return X_TFT_MIN_MAG_MIP_POINT;
	case D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR:
		return X_TFT_MIN_MAG_POINT_MIP_LINEAR;
	case D3D10_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT:
		return X_TFT_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case D3D10_FILTER_MIN_POINT_MAG_MIP_LINEAR:
		return X_TFT_MIN_POINT_MAG_MIP_LINEAR;
	case D3D10_FILTER_MIN_LINEAR_MAG_MIP_POINT:
		return X_TFT_MIN_LINEAR_MAG_MIP_POINT;
	case D3D10_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		return X_TFT_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT:
		return X_TFT_MIN_MAG_LINEAR_MIP_POINT;
	case D3D10_FILTER_MIN_MAG_MIP_LINEAR:
		return X_TFT_MIN_MAG_MIP_LINEAR;
	case D3D10_FILTER_ANISOTROPIC:
		return X_TFT_ANISOTROPIC;
	case D3D10_FILTER_COMPARISON_MIN_MAG_MIP_POINT:
		return X_TFT_COMPARISON_MIN_MAG_MIP_POINT;
	case D3D10_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR:
		return X_TFT_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
	case D3D10_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:
		return X_TFT_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	case D3D10_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR:
		return X_TFT_COMPARISON_MIN_POINT_MAG_MIP_LINEAR0x85;
	case D3D10_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT:
		return X_TFT_COMPARISON_MIN_LINEAR_MAG_MIP_POINT0x90;
	case D3D10_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
		return X_TFT_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	case D3D10_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT:
		return X_TFT_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	case D3D10_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR:
		return X_TFT_COMPARISON_MIN_MAG_MIP_LINEAR;
	case D3D10_FILTER_COMPARISON_ANISOTROPIC:
		return X_TFT_COMPARISON_ANISOTROPIC;
	case D3D10_FILTER_TEXT_1BIT:
		return X_TFT_TEXT_1BIT;
	default:
		X_ASSERT_FORCED("Unrecognized texture filter type. Defaulting to a linear filter.");
		return X_TFT_MIN_MAG_MIP_LINEAR;
	}
}
		


//*****************************************************************************
//*****************************************************************************
inline D3D10_TEXTURE_ADDRESS_MODE XMaterial::translateTextureAddressType(X_TEXTURE_ADDRESS_TYPE address)
{
	switch (address)
	{
	case X_TAT_UNKNOWN:
		X_ASSERT_FORCED("Unknown texture address type.  Defaulting to wrapping mode.");
		return D3D10_TEXTURE_ADDRESS_WRAP;
	case X_TAT_WRAP:
		return D3D10_TEXTURE_ADDRESS_WRAP;
	case X_TAT_MIRROR:
		return D3D10_TEXTURE_ADDRESS_MIRROR;
	case X_TAT_CLAMP:
		return D3D10_TEXTURE_ADDRESS_CLAMP;
	case X_TAT_BORDER:
		return D3D10_TEXTURE_ADDRESS_BORDER;
	case X_TAT_MIRROR_ONCE:
		return D3D10_TEXTURE_ADDRESS_MIRROR_ONCE;
	default:
		X_ASSERT_FORCED("Unrecognized texture address type.  Defaulting to wrapping mode.");
		return D3D10_TEXTURE_ADDRESS_WRAP;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_TEXTURE_ADDRESS_TYPE XMaterial::translateTextureAddressType(D3D10_TEXTURE_ADDRESS_MODE address)
{
	switch (address)
	{
	case D3D10_TEXTURE_ADDRESS_WRAP:
		return X_TAT_WRAP;
	case D3D10_TEXTURE_ADDRESS_MIRROR:
		return X_TAT_MIRROR;
	case D3D10_TEXTURE_ADDRESS_CLAMP:
		return X_TAT_CLAMP;
	case D3D10_TEXTURE_ADDRESS_BORDER:
		return X_TAT_BORDER;
	case D3D10_TEXTURE_ADDRESS_MIRROR_ONCE:
		return X_TAT_MIRROR_ONCE;
	default:
		X_ASSERT_FORCED("Unrecognized texture address type.  Defaulting to wrapping mode.");
		return X_TAT_WRAP;
	}
}







#endif