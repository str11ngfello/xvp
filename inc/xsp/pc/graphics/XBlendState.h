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
//	Module Name:	XBlendState.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BLEND_STATE_H__
#define __X_BLEND_STATE_H__

#include <XOSMgr.h>
#include <XGraphicsMgr.h>
#include <XResource.h>
#include <d3d10.h>
#include <d3dx10.h>


#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }



//*****************************************************************************
//! Blend State.
//*****************************************************************************
class XBlendState : public XResource
{
protected:
	ID3D10BlendState*	m_pBlendState;
	XF32				m_blendFactor[4];
	XS32				m_sampleMask;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	D3D10_BLEND translateBlendType(X_BLEND_TYPE blendType);
	D3D10_BLEND_OP translateBlendOpType(X_BLEND_OP_TYPE blendType);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBlendState();
	virtual ~XBlendState();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XBlendDesc& desc);
	XStatus destroy();
	XStatus set();
};


//*****************************************************************************
//*****************************************************************************
inline D3D10_BLEND XBlendState::translateBlendType(X_BLEND_TYPE blendType)
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
inline D3D10_BLEND_OP XBlendState::translateBlendOpType(X_BLEND_OP_TYPE blendType)
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


#endif