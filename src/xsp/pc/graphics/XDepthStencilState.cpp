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
//	Module Name:	XDepthStencilState.cpp
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
XDepthStencilState::XDepthStencilState()
{
	m_pDepthStencilState = NULL;
	m_stencilRefValue = 0;
}



//*****************************************************************************
//*****************************************************************************
XDepthStencilState::~XDepthStencilState()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XDepthStencilState::init(XDepthStencilDesc& desc)
{
	//Destroy any old data
	destroy();

	//Create the D3D depth stencil state
	HRESULT hr;
	D3D10_DEPTH_STENCIL_DESC dsDesc;
		
	//Depth test parameters
	dsDesc.DepthEnable = desc.depthTestEnable;
	if (desc.depthWriteEnable)
		dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
	else
		dsDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = translateComparisonFuncType(desc.depthFunc);

	//Stencil test parameters
	dsDesc.StencilEnable = desc.stencilTestEnable;
	dsDesc.StencilReadMask = desc.stencilReadMask;
	dsDesc.StencilWriteMask = desc.stencilWriteMask;

	//Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = translateStencilOpType(desc.stencilFailOpFrontFace);
	dsDesc.FrontFace.StencilDepthFailOp = translateStencilOpType(desc.stencilDepthFailOpFrontFace);
	dsDesc.FrontFace.StencilPassOp = translateStencilOpType(desc.stencilPassOpFrontFace);
	dsDesc.FrontFace.StencilFunc = translateComparisonFuncType(desc.stencilFuncFrontFace);

	//Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = translateStencilOpType(desc.stencilFailOpBackFace);
	dsDesc.BackFace.StencilDepthFailOp = translateStencilOpType(desc.stencilDepthFailOpBackFace);
	dsDesc.BackFace.StencilPassOp = translateStencilOpType(desc.stencilPassOpBackFace);
	dsDesc.BackFace.StencilFunc = translateComparisonFuncType(desc.stencilFuncBackFace);
	
	m_stencilRefValue = desc.stencilRefValue;

	//Create depth stencil state
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateDepthStencilState(&dsDesc,&m_pDepthStencilState);
	X_ASSERTM(hr == S_OK,"Failed to create depth stencil state.");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDepthStencilState::set()
{
	XVP::GraphicsMgr.m_pD3DDevice->OMSetDepthStencilState(m_pDepthStencilState,m_stencilRefValue);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XDepthStencilState::destroy()
{
	SAFE_RELEASE(m_pDepthStencilState);
	XResource::destroy();
	return X_OK;
}

