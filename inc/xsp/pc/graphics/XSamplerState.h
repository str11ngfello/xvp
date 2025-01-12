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
//	Module Name:	XSamperState.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SAMPLER_STATE_H__
#define __X_SAMPLER_STATE_H__

#include <XOSMgr.h>
#include <XGraphicsMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Texture Sampler State.
//*****************************************************************************
class XSamplerState : public XResource
{
protected:
	ID3D10SamplerState* 	 m_pSamplerState;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	D3D10_FILTER translateTextureFilterType(X_TEXTURE_FILTER_TYPE filter);
	D3D10_TEXTURE_ADDRESS_MODE translateTextureAddressType(X_TEXTURE_ADDRESS_TYPE address);
	D3D10_COMPARISON_FUNC translateComparisonFuncType(X_CMP_FUNC_TYPE cmpFunc);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSamplerState();
	virtual ~XSamplerState();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XSamplerDesc& desc);
	XStatus destroy();
	XStatus set(XU32 texStage);
};



//*****************************************************************************
//*****************************************************************************
inline D3D10_FILTER XSamplerState::translateTextureFilterType(X_TEXTURE_FILTER_TYPE filter)
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
inline D3D10_TEXTURE_ADDRESS_MODE XSamplerState::translateTextureAddressType(X_TEXTURE_ADDRESS_TYPE address)
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
inline D3D10_COMPARISON_FUNC XSamplerState::translateComparisonFuncType(X_CMP_FUNC_TYPE cmpFunc)
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

#endif