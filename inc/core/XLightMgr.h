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
//	Module Name:	XLightMgr.h
//	Author:			Lynn Duke
//	Creation Date:	1-1-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_LIGHT_MGR_H__
#define __X_LIGHT_MGR_H__



#define X_MAX_OCCLUDING_LIGHTS	8

enum XLightingMode
{
	X_LM_DEFAULT,
	X_LM_SYSTEM,
	X_LM_SCENE
};


//*****************************************************************************
//
//*****************************************************************************
class XLightMgr
{
private:
	XScene*							m_pScene;
	XColor							m_ambientLight;
	XMemBlockList<XLight*>			m_directionalLights;
	XMemBlockList<XLight*>			m_pointLights;
	XMemBlockList<XLight*>			m_spotLights;
	XBOOL							m_inited;
	XLightingMode					m_lightingMode;
	XLight							m_defaultLight;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XLightMgr();
	virtual ~XLightMgr();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XScene* pScene);
	XStatus destroy();
	XStatus addLight(XLight* pLight);
	XStatus removeLight(XLight* pLight);
	XStatus removeAllLights();
	XU32 getDirectionalLights(XLight* pLights[],XU32 maxLights);
	XU32 getPointLightsInRange(XVector3D& position,XLight* pLights[],XU32 maxLights);
	XU32 getSpotLightsInRange(XVector3D& position,XLight* pLights[],XU32 maxLights);
	XU32 getNumDirectionalLights();
	XU32 getNumPointLights();
	XU32 getNumSpotLights();
	XStatus setAmbientLight(XColor& ambient);
	XColor& getAmbientLight();
	XStatus setLightingMode(XLightingMode mode);
	XLightingMode getLightingMode();
	//XStatus getOcclusionMatrices(XMatrix4x4** ppOcclusionMatrices,XU32 maxLights);

};

#endif