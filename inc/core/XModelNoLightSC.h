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
//	Module Name:	XModelBlinnPhong.h
//	Author:			Lynn Duke
//	Creation Date:	10-16-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_NO_LIGHT_SC_H__
#define __X_MODEL_NO_LIGHT_SC_H__




#define MAX_NUM_LIGHTS 4

//*****************************************************************************
//*****************************************************************************
class XModelNoLightSC : public XShaderController
{
private:
	XHandle m_vsHandle;
	XHandle m_psHandle;
	XHandle m_vsCBHandle;
	XHandle m_psCBHandle;

	bool m_inited;

	//Constant Buffers
	struct vsCB
	{
		XMatrix4x4 wvpMatrix;
		XMatrix4x4 prevWVPMatrix;
		XMatrix4x4 worldMatrix;
		XMatrix4x4 occMatrices[MAX_NUM_LIGHTS];	
		XVector4D miscParams; //x = inflate

		vsCB()
		{
			miscParams.x = 0.0f;
		}
	};
	vsCB m_vsCB;

	struct psCB
	{
		XVector4D v4ScaleParams; //x = diffuse scale,y = emissive scale,z = object alpha
		XVector4D v4AnimParams; //x = diff map u speed,y = diff map v speed,z = emiss map u speed,w = emiss map v speed
		int occEnabled[MAX_NUM_LIGHTS];

		psCB()
		{
			v4ScaleParams.x = 1.0f;
			v4ScaleParams.y = 1.0f;
			v4ScaleParams.z = 1.0f;
			v4AnimParams.x = 0.0f;
			v4AnimParams.y = 0.0f;
			v4AnimParams.z = 0.0f;
			v4AnimParams.w = 0.0f;
		}
	};
	psCB m_psCB;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModelNoLightSC();
	virtual ~XModelNoLightSC();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pShaderName);
	XStatus destroy();
	XS8* getTypeDesc();
	void setShaderParamBlock(XS8* pMemoryChunk);
	XStatus render(XRenderEvent* pRenderEvent);
	XF32 getDiffScale();
	void setDiffScale(XF32 diffScale);
	XF32 getEmissScale();
	void setEmissScale(XF32 emissScale);
	XF32 getDiffUSpeed();
	void setDiffUSpeed(XF32 uSpeed);
	XF32 getDiffVSpeed();
	void setDiffVSpeed(XF32 vSpeed);
	XF32 getEmissUSpeed();
	void setEmissUSpeed(XF32 uSpeed);
	XF32 getEmissVSpeed();
	void setEmissVSpeed(XF32 vSpeed);
	XF32 getObjectAlpha();
	void setObjectAlpha(XF32 alpha);
	XF32 getInflateAmount();
	void setInflateAmount(XF32 inflation);

	//*****************************************************************************
	//Property Bag Interface
	//*****************************************************************************
	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
};


#endif