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

#ifndef __X_MODEL_BLINN_PHONG_SC_H__
#define __X_MODEL_BLINN_PHONG_SC_H__



#define MAX_NUM_LIGHTS	4


//*****************************************************************************
//*****************************************************************************
class XModelBlinnPhongSC : public XShaderController
{
private:
	XHandle m_vsHandle;
	XHandle m_psHandle;
	XHandle m_vsCBHandle;
	XHandle m_psCBHandle;

	XU32 m_maxDir;
	XU32 m_maxPoint;
	XU32 m_maxSpot;

	bool m_inited;

	//Constant Buffers
	struct vsCB
	{
		XMatrix4x4 wvpMatrix;
		XMatrix4x4 prevWVPMatrix;
		XMatrix4x4 worldMatrix;
		XMatrix4x4 wvMatrix;
		XVector4D v4CamPosObjSpace;
		XMatrix4x4 occMatrices[MAX_NUM_LIGHTS];	
		XVector4D v4MiscParams; //x = inflate

		vsCB()
		{
			v4MiscParams.x = 1.0f;
		}
	};
	vsCB m_vsCB;

	struct psCB
	{
		int lightEnabled[MAX_NUM_LIGHTS];
		int occEnabled[MAX_NUM_LIGHTS];
		int lightTypes[MAX_NUM_LIGHTS];
		XVector4D v4LightDir[MAX_NUM_LIGHTS];	
		XVector4D v4LightPos[MAX_NUM_LIGHTS];
		XVector4D v4LightDiffuse[MAX_NUM_LIGHTS];
		XVector4D v4LightSpecular[MAX_NUM_LIGHTS];
		XVector4D v4LightParams[MAX_NUM_LIGHTS]; //x = fInvLightCutoff,y = fHotSpotDist,z = fCosHalfAngleCutoff,w = fCosHalfAngleDropoff
		XVector4D v4ScaleParams; //x = spec power,y = ao scale,z = env scale,w = spec scale
		XVector4D v4ScaleParams2; //x = diffuse scale,y = emissive scale,z = dnorm tile factor,w = fresnel index
		XVector4D v4AnimParams; //x = diff map u speed,y = diff map v speed,z = emiss map u speed,w = emiss map v speed
		XVector4D v4MiscParams; //x = object alpha

		psCB()
		{
			v4ScaleParams.x = 20.0f;
			v4ScaleParams.y = 1.0f;
			v4ScaleParams.z = 1.0f;
			v4ScaleParams.w = 1.0f;
			v4ScaleParams2.x = 1.0f;
			v4ScaleParams2.y = 1.0f;
			v4ScaleParams2.z = 10.0f;
			v4ScaleParams2.w = .184f;
			v4AnimParams.x = 0.0f;
			v4AnimParams.y = 0.0f;
			v4AnimParams.z = 0.0f;
			v4AnimParams.w = 0.0f;
			v4MiscParams.x = 1.0f;
		}
	};
	psCB m_psCB;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModelBlinnPhongSC();
	virtual ~XModelBlinnPhongSC();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pShaderName);
	XStatus destroy();
	XS8* getTypeDesc();
	void setShaderParamBlock(XS8* pMemoryChunk);
	XStatus render(XRenderEvent* pRenderEvent);
	void getLightLimits(XU32& maxDir,XU32& maxPoint,XU32& maxSpot);
	void setLightLimits(XU32 maxDir,XU32 maxPoint,XU32 maxSpot);
	void setGlobalAmbient(XColor& ambient);
	XF32 getSpecPower();
	void setSpecPower(XF32 specPower);
	XF32 getAOScale();
	void setAOScale(XF32 aoScale);
	XF32 getEnvScale();
	void setEnvScale(XF32 envScale);
	XF32 getSpecScale();
	void setSpecScale(XF32 specScale);
	XF32 getDiffScale();
	void setDiffScale(XF32 diffScale);
	XF32 getEmissScale();
	void setEmissScale(XF32 emissScale);
	XF32 getDNormTilingFactor();
	void setDNormTilingFactor(XF32 tilingFactor);
	XF32 getDiffUSpeed();
	void setDiffUSpeed(XF32 uSpeed);
	XF32 getDiffVSpeed();
	void setDiffVSpeed(XF32 vSpeed);
	XF32 getEmissUSpeed();
	void setEmissUSpeed(XF32 uSpeed);
	XF32 getEmissVSpeed();
	void setEmissVSpeed(XF32 vSpeed);
	XF32 getFresnelIndex();
	void setFresnelIndex(XF32 fresnelIndex);
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