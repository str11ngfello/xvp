//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//     _  ___    ______ 
//    | |/ / |  / / __ \
//    |   /| | / / /_/ /
//   /   | | |/ / ____/ 
//  /_/|_| |___/_/       
//
//	eXtensible Visualization Platform  
//	(c) Copyright 2000. All Rights Reserved.
//	Seventh Night Studios (www.xvpsdk.com)
//
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef __X_DISTORTION_SC_H__
#define __X_DISTORTION_SC_H__




//*****************************************************************************
//*****************************************************************************
class XDistortionSC : public XShaderController
{
private:
	XHandle m_vsHandle;
	XHandle m_psHandle;
	XHandle m_vsCBHandle;
	XHandle m_psCBHandle;
	bool m_inited;

	struct vsCB
	{
		XMatrix4x4  wvpMatrix;
	};
	vsCB m_vsCB;

	struct psCB
	{
		XVector4D params;
	};
	psCB m_psCB;

	XF32			m_uSpeed;
	XF32			m_vSpeed;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XDistortionSC();
	virtual ~XDistortionSC();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pShaderName = NULL);
	XStatus destroy();
	XS8* getTypeDesc();
	XStatus update(XF32 dt);
	XStatus render(XRenderEvent* pRenderEvent);

	void setUAnimSpeed(XF32 speed);
	void setVAnimSpeed(XF32 speed);

	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);

};


#endif