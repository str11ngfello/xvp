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
//	Module Name:	XDepthStencilState.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_DEPTH_STENCIL_STATE_H__
#define __X_DEPTH_STENCIL_STATE_H__

#include <XOSMgr.h>
#include <XGraphicsMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Depth Stencil State.
//*****************************************************************************
class XDepthStencilState : public XResource
{
protected:
	ID3D10DepthStencilState*	m_pDepthStencilState;
	XU32						m_stencilRefValue;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	D3D10_COMPARISON_FUNC translateComparisonFuncType(X_CMP_FUNC_TYPE cmpFunc);
	D3D10_STENCIL_OP translateStencilOpType(X_STENCIL_OP_TYPE stenilOp);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XDepthStencilState();
	virtual ~XDepthStencilState();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XDepthStencilDesc& desc);
	XStatus destroy();
	XStatus set();
};



//*****************************************************************************
//*****************************************************************************
inline D3D10_COMPARISON_FUNC XDepthStencilState::translateComparisonFuncType(X_CMP_FUNC_TYPE cmpFunc)
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
inline D3D10_STENCIL_OP XDepthStencilState::translateStencilOpType(X_STENCIL_OP_TYPE stenilOp)
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


#endif