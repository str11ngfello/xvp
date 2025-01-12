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
//	Module Name:	XShadowCasterSSMSC.h
//	Author:			Lynn Duke
//	Creation Date:	10-13-2008
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SHADOW_CASTER_SSM_SC_H__
#define __X_SHADOW_CASTER_SSM_SC_H__





//*****************************************************************************
//*****************************************************************************
class XShadowCasterSSMSC : public XShaderController
{
private:
	XHandle				m_vsHandle;
	XHandle				m_psHandle;
	XHandle				m_vsCBHandle;
	XBOOL				m_inited;

	struct vsCB
	{
		XMatrix4x4  wvpMatrix;
		XVector4D depthBias;

		vsCB()
		{
			depthBias.x = 0.0f;
		}
	};
	vsCB m_vsCB;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XShadowCasterSSMSC();
	virtual ~XShadowCasterSSMSC();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pShaderName = NULL);
	XStatus destroy();
	XS8* getTypeDesc();
	XStatus render(XRenderEvent* pRenderEvent);
	XF32 getDepthBias();
	void setDepthBias(XF32 depthBias);
};


#endif