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
//	Module Name:	XRasterizerState.cpp
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
XRasterizerState::XRasterizerState()
{
	m_pRasterState = NULL;
}



//*****************************************************************************
//*****************************************************************************
XRasterizerState::~XRasterizerState()
{
}


//*****************************************************************************
//*****************************************************************************
XStatus XRasterizerState::init(XRasterizerDesc& desc)
{
	//Destroy any old data
	destroy();

	//Create the D3D rasterizer state
	HRESULT hr;

    D3D10_RASTERIZER_DESC rasDesc;
    rasDesc.FillMode = translateFillModeType(desc.fillMode);
    rasDesc.CullMode = translateCullType(desc.cullMode);
    rasDesc.FrontCounterClockwise = desc.frontCounterClockwise;
    rasDesc.DepthBias = desc.depthBias;
    rasDesc.DepthBiasClamp = desc.depthBiasClamp;
    rasDesc.SlopeScaledDepthBias = desc.slopeScaledDepthBias;
    rasDesc.DepthClipEnable = desc.depthClipEnable;
    rasDesc.ScissorEnable = desc.scissorEnable;
    rasDesc.MultisampleEnable = desc.multisampleEnable;
    rasDesc.AntialiasedLineEnable = desc.antialiasedLineEnable;
	XVP::GraphicsMgr.m_pD3DDevice->CreateRasterizerState(&rasDesc,&m_pRasterState);


	//Create rasterizer state
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateRasterizerState(&rasDesc,&m_pRasterState);
	X_ASSERTM(hr == S_OK,"Failed to create rasterizer state.");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRasterizerState::set()
{	
	XVP::GraphicsMgr.m_pD3DDevice->RSSetState(m_pRasterState);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XRasterizerState::destroy()
{
	SAFE_RELEASE(m_pRasterState);
	XResource::destroy();
	return X_OK;
}



