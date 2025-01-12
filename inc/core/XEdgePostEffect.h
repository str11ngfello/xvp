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
//	Module Name:	XEdgePostEffect.h
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_EDGE_POST_EFFECT_H__
#define __X_EDGE_POST_EFFECT_H__




#define X_MAX_STAR_TEXTURES 12


//*****************************************************************************
//! Provides a viewpoint into the 3D scene.
/**
*/
//*****************************************************************************
class XEdgePostEffect : public XPostEffect
{
protected:
	XHandle					m_redQuadPS;
	XHandle					m_samplerState;
	
public:

	XEdgePostEffect();

	XStatus init(XU32 viewportWidth,XU32 viewportHeight);
	XStatus destroy();
	XStatus render(XF32 dt,XScene* pScene,
				   XHandle srcRenderTargetArray,
				   XViewport& srcViewport,
				   XHandle destRenderTargetArray,
				   XViewport& destViewport);
};


#endif
