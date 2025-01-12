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
//	Module Name:	XBlendState.cpp
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
XBlendState::XBlendState()
{
	m_pBlendState = NULL;
	m_blendFactor[0] = m_blendFactor[1] = m_blendFactor[2] = m_blendFactor[3] = 1.0f;
	m_sampleMask = 0xFFFFFFFF;
}



//*****************************************************************************
//*****************************************************************************
XBlendState::~XBlendState()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XBlendState::init(XBlendDesc& desc)
{
	//Destroy any old data
	destroy();

	//Create the D3D blend state
	HRESULT hr;

	D3D10_BLEND_DESC blendDesc;
	for (XU32 i = 0;i < X_MAX_RENDER_TARGETS;++i)
	{
		blendDesc.BlendEnable[i] = desc.blendEnable[i];
		blendDesc.RenderTargetWriteMask[i] = X_CWF_ALL;
	}
	blendDesc.BlendOpAlpha = translateBlendOpType(desc.blendOpAlpha);
	blendDesc.SrcBlendAlpha = translateBlendType(desc.srcBlendAlpha);
	blendDesc.DestBlendAlpha = translateBlendType(desc.destBlendAlpha);
	blendDesc.BlendOp = translateBlendOpType(desc.blendOp);
	blendDesc.SrcBlend = translateBlendType(desc.srcBlend);
	blendDesc.DestBlend = translateBlendType(desc.destBlend);
	blendDesc.AlphaToCoverageEnable = desc.alphaToCoverageEnable;
	m_blendFactor[0] = desc.blendFactor.m_R;
	m_blendFactor[1] = desc.blendFactor.m_G;
	m_blendFactor[2] = desc.blendFactor.m_B;
	m_blendFactor[3] = desc.blendFactor.m_A;
	m_sampleMask = desc.sampleMask;

	//Create blend state
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateBlendState(&blendDesc,&m_pBlendState);
	X_ASSERTM(hr == S_OK,"Failed to create blend state.");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XBlendState::set()
{
	XVP::GraphicsMgr.m_pD3DDevice->OMSetBlendState(m_pBlendState,m_blendFactor,m_sampleMask);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XBlendState::destroy()
{
	SAFE_RELEASE(m_pBlendState);
	XResource::destroy();
	return X_OK;
}

