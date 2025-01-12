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
//	Module Name:	XDOFPostEffect.h
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_DOF_POST_EFFECT_H__
#define __X_DOF_POST_EFFECT_H__





//*****************************************************************************
//! Provides a viewpoint into the 3D scene.
/**
*/
//*****************************************************************************
class XDOFPostEffect : public XPostEffect
{
protected:
	
	struct DOFData
	{
		XF32 focalPlane;
		XF32 nearBlurPlane;
		XF32 farBlurPlane;
		XF32 maxBlur;
		XF32 nearMinusFocal;
		XF32 focalMinusFar;
		XF32 pad0;
		XF32 pad1;
	};
	

	XHandle					m_dofPS;
	XHandle					m_pointClampState;
	XHandle					m_linearClampState;
	XHandle					m_noBlendState;
	XHandle					m_dofData;
	XViewport				m_scaledSceneViewport;

	XF32 m_fFocalPlane;  
	XF32 m_fNearBlurPlane; 
	XF32 m_fFarBlurPlane;    
	XF32 m_fMaxBlur;      
	
	XHandle m_pScaledSceneTexture;      
	XHandle m_pBlurredSceneTexture;          

public:

	XDOFPostEffect();

	XStatus init(XU32 viewportWidth,XU32 viewportHeight);
	XStatus destroy();
	XStatus update(XF32 dt,XScene* pScene);
	XStatus render(XF32 dt,XScene* pScene,
				   XHandle srcRenderTargetArray,
				   XViewport& srcViewport,
				   XHandle destRenderTargetArray,
				   XViewport& destViewport);
	XStatus handleResizeEvent(XU32 width,XU32 height);

	XF32 getFocalPlane();  
	XStatus setFocalPlane(XF32 focalPlane);  
	XF32 getNearBlurPlane(); 
	XStatus setNearBlurPlane(XF32 nearBlurPlane); 
	XF32 getFarBlurPlane();    
	XStatus setFarBlurPlane(XF32 farBlurPlane);    
	XF32 getMaxBlur();   
	XStatus setMaxBlur(XF32 maxBlur);   

	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
};


#endif
