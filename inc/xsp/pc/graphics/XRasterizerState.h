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
//	Module Name:	XRasterizerState.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_RASTERIZER_STATE_H__
#define __X_RASTERIZER_STATE_H__

#include <XOSMgr.h>
#include <XGraphicsMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Rasterizer State.
//*****************************************************************************
class XRasterizerState : public XResource
{
protected:
	ID3D10RasterizerState*		m_pRasterState;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	D3D10_CULL_MODE translateCullType(X_CULL_TYPE cullType);
	D3D10_FILL_MODE translateFillModeType(X_FILL_MODE_TYPE fillModeType);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XRasterizerState();
	virtual ~XRasterizerState();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	XStatus init(XRasterizerDesc& desc);
	XStatus destroy();
	XStatus set();
};



//*****************************************************************************
//*****************************************************************************
inline D3D10_CULL_MODE XRasterizerState::translateCullType(X_CULL_TYPE cullType)
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
inline D3D10_FILL_MODE XRasterizerState::translateFillModeType(X_FILL_MODE_TYPE fillModeType)
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


#endif