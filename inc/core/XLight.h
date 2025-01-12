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
//	Module Name:	XLight.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_LIGHT_H__
#define __X_LIGHT_H__





//*****************************************************************************
//! Light Object.
//*****************************************************************************
class XLight : public XSceneNode
{
private:
	XU32				m_priority;


public:
	bool				m_enabled;
	X_LIGHT_TYPE		m_lightType;
	//XF32				m_squaredRange;
	XColor				m_diffuse;		//a = diffuse intensity
	XColor				m_specular;		//a = specular intensity
	XVector4D			m_params;

	//Occlusion 
	XScene*				m_pOccScene;
	XCamera*			m_pOccCam;
	XHandle				m_pOccMat;
	XShadowCasterSSMSC*	m_pOccSC;
	XMatrix4x4*			m_pOccMatrix;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XLight();
	~XLight();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc(); 
	virtual XStatus init(XLightDesc& desc);
	virtual XStatus initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize);
	virtual XStatus destroy();
	XU32 getPriority();
	virtual XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus prepForRender(XF32 dt,XScene* pScene);
	XStatus render(XRenderEvent* pRenderEvent);
	
	//Occlusion
	XStatus enableOcclusion(XBOOL enable);
	XScene* getOcclusionScene();
	XStatus addOccluder(XSceneNode* pOccluder);
	XStatus removeOccluder(XSceneNode* pOccluder);
	XBOOL containsOccluder(XSceneNode* pOccluder);
	XMatrix4x4* getOcclusionMatrix();
	XBOOL isOccludersEnabled();
	XStatus enableOccluders(XBOOL enable);

	//Property Bag Methods
	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
	XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
	XStatus setColorProperty(XS8* pPropertyName,XColor& value);
	XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
};


#endif