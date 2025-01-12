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
//	Module Name:	XSamplerState.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XSamplerState::XSamplerState()
{
	m_pSamplerState = NULL;
}



//*****************************************************************************
//*****************************************************************************
XSamplerState::~XSamplerState()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XSamplerState::init(XSamplerDesc& desc)
{
	//Destroy any old data
	destroy();

	//Create the D3D sampler state
	HRESULT hr;
	
	D3D10_SAMPLER_DESC samplerDesc;
	samplerDesc.AddressU = translateTextureAddressType(desc.addressU);
	samplerDesc.AddressV = translateTextureAddressType(desc.addressV);
	samplerDesc.AddressW = translateTextureAddressType(desc.addressW);
	samplerDesc.Filter = translateTextureFilterType(desc.filter);
	samplerDesc.MipLODBias = desc.mipLODBias;
	samplerDesc.MinLOD = desc.minLOD;
	samplerDesc.MaxLOD = desc.maxLOD;
	samplerDesc.ComparisonFunc = translateComparisonFuncType(desc.comparisonFunc);
	samplerDesc.MaxAnisotropy = desc.maxAnisotropy;

	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateSamplerState(&samplerDesc,&m_pSamplerState);
	X_ASSERTM(hr == S_OK,"Failed to create sampler state.");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSamplerState::set(XU32 texStage)
{
	X_ASSERTM(texStage < X_MAX_SAMPLERS,"Texture stage index out of range.");
	XVP::GraphicsMgr.m_pD3DDevice->PSSetSamplers(texStage,1,&m_pSamplerState);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSamplerState::destroy()
{
	SAFE_RELEASE(m_pSamplerState);
	XResource::destroy();
	return X_OK;
}



