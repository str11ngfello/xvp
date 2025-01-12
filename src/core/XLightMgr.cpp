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
//	Module Name:	XLightMgr.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-6-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
/**
Constructor for the controller.
*/
//*****************************************************************************
XLightMgr::XLightMgr()
{
	m_lightingMode = X_LM_SCENE;
	m_inited = false;
}



//*****************************************************************************
/**
*/
//*****************************************************************************
XLightMgr::~XLightMgr()
{
	destroy();
}



//*****************************************************************************
//*****************************************************************************
XStatus XLightMgr::init(XScene* pScene)
{
	X_ASSERTM(pScene,"Invalid scene");
	m_pScene = pScene;

	XLightDesc lightDesc;
	lightDesc.type = X_LT_DIRECTIONAL;
	lightDesc.diffuse = XColor(1,1,1,1);
	m_defaultLight.init(lightDesc);

	m_inited = true;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLightMgr::destroy()
{
	if (!m_inited)
		return X_OK;

	m_directionalLights.clear();
	m_pointLights.clear();
	m_spotLights.clear();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLightMgr::addLight(XLight* pLight)
{
	//Make sure we have a valid light pointer
	X_ASSERTM(pLight,"Invalid light pointer");

	//Insert the light into the proper position given it's priority in order
	//to maintain each list in descending order of priority
	XMemBlockList<XLight*>::iterator itr;

	switch (pLight->m_lightType)
	{
	case X_LT_DIRECTIONAL:
		X_DEBUG
		(
			if (m_directionalLights.contains(pLight))
				X_ASSERT_FORCED("Directional light already exists in scene. It cannot be added more than once");
		)
		itr.set(m_directionalLights.begin());
		while (itr.isValid())
		{
			if (pLight->getPriority() > itr.getValue()->getPriority())
				break;
			itr.getNext();
		}
		m_directionalLights.insert(itr,pLight);
		break;
	case X_LT_POINT:
		X_DEBUG
		(
			if (m_pointLights.contains(pLight))
				X_ASSERT_FORCED("Point light already exists in scene. It cannot be added more than once");
		)
		itr.set(m_pointLights.begin());
		while (itr.isValid())
		{
			if (pLight->getPriority() > itr.getValue()->getPriority())
				break;
			itr.getNext();
		}
		m_pointLights.insertBack(pLight);
		break;
	case X_LT_SPOT:
		X_DEBUG
		(
			if (m_spotLights.contains(pLight))
				X_ASSERT_FORCED("Spot light already exists in scene. It cannot be added more than once");
		)
		itr.set(m_spotLights.begin());
		while (itr.isValid())
		{
			if (pLight->getPriority() > itr.getValue()->getPriority())
				break;
			itr.getNext();
		}
		m_spotLights.insertBack(pLight);
		break;
	}
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLightMgr::removeLight(XLight* pLight)
{
	X_ASSERTM(pLight,"Invalid light pointer");

	switch (pLight->m_lightType)
	{
	case X_LT_DIRECTIONAL:
		m_directionalLights.remove(pLight);
		break;
	case X_LT_POINT:
		m_pointLights.remove(pLight);
		break;
	case X_LT_SPOT:
		m_spotLights.remove(pLight);
		break;
	}
	
	return X_OK;
}



XStatus XLightMgr::removeAllLights()
{	
	m_directionalLights.clear();
	m_pointLights.clear();
	m_spotLights.clear();
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XLightMgr::getNumDirectionalLights()
{
	return m_directionalLights.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XU32 XLightMgr::getNumPointLights()
{
	return m_pointLights.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XU32 XLightMgr::getNumSpotLights()
{
	return m_spotLights.getNumItems();
}



//*****************************************************************************
//*****************************************************************************
XU32 XLightMgr::getDirectionalLights(XLight* pLights[],XU32 maxLights)
{
	X_ASSERTM(maxLights > 0,"max lights cannot be 0");

	//If we are in default light mode, used for debug functions, just
	//return a directional light matching the scene camera's look
	//vector.  If we are in system mode, only use lights that have system priority.
	//If we are in scene mode, use all available lights.
	XU32 numLights = 0;
	XMemBlockList<XLight*>::iterator i;
	switch (m_lightingMode)
	{
	case X_LM_DEFAULT:
		if (m_pScene->getActiveCamera())
		{
			m_defaultLight.setWorldTransform(m_pScene->getActiveCamera()->getWorldTransform());
			pLights[numLights++] = &m_defaultLight;
		}
		break;
	case X_LM_SYSTEM:
		i.set(m_directionalLights.begin());
		while (i.isValid())
		{
			if (i.getValue()->m_enabled && i.getValue()->getPriority() == SYSTEM_LIGHT_PRIORITY)
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	case X_LM_SCENE:
		//Return the number of directional lights requested
		i.set(m_directionalLights.begin());
		while (i.isValid())
		{
			if (i.getValue()->m_enabled)
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	}

	return numLights;
}



//*****************************************************************************
//*****************************************************************************
XU32 XLightMgr::getPointLightsInRange(XVector3D& position,XLight* pLights[],XU32 maxLights)
{
	X_ASSERTM(maxLights > 0,"max lights cannot be 0");

	//If we are in default light mode, don't return any point lights. If we're in
	//system mode, use only lights with a system priority.  If we're in scene mode,
	//use all available lights
	XU32 numLights = 0;
	XMemBlockList<XLight*>::iterator i;
	switch (m_lightingMode)
	{
	case X_LM_DEFAULT:
		numLights = 0;
		break;
	case X_LM_SYSTEM:
		i.set(m_pointLights.begin());
		while (i.isValid())
		{
			if (i.getValue()->m_enabled && i.getValue()->getPriority() == SYSTEM_LIGHT_PRIORITY)
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	case X_LM_SCENE:
		//Return the point lights within range of the position passed in.  Do not
		//exceed the max number of lights indicated by the user
		i.set(m_pointLights.begin());
		XVector3D worldPos;
		while (i.isValid())
		{
			i.getValue()->getWorldPosition(worldPos);
			//if ((i.getValue()->m_enabled) && (position.distanceSquared(worldPos) <= (i.getValue())->m_squaredRange))
			if ((i.getValue()->m_enabled) && (position.distance(worldPos) <= (i.getValue())->m_params.x))
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	}
	
	return numLights;
}



//*****************************************************************************
//*****************************************************************************
XU32 XLightMgr::getSpotLightsInRange(XVector3D& position,XLight* pLights[],XU32 maxLights)
{
	X_ASSERTM(maxLights > 0,"max lights cannot be 0");

	//If we are in default light mode, don't return any spot lights. If we're in
	//system mode, use only lights with a system priority.  If we're in scene mode,
	//use all available lights
	XU32 numLights = 0;
	XMemBlockList<XLight*>::iterator i;
	switch (m_lightingMode)
	{
	case X_LM_DEFAULT:
		numLights = 0;
		break;
	case X_LM_SYSTEM:
		i.set(m_spotLights.begin());
		while (i.isValid())
		{
			if (i.getValue()->m_enabled && i.getValue()->getPriority() == SYSTEM_LIGHT_PRIORITY)
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	case X_LM_SCENE:
		//Return the spot lights within range of the position passed in.  Do not
		//exceed the max number of lights indicated by the user
		i.set(m_spotLights.begin());
		XVector3D worldPos;
		while (i.isValid())
		{
			i.getValue()->getWorldPosition(worldPos);
			//if ((i.getValue()->m_enabled) && (position.distanceSquared(worldPos) <= (i.getValue())->m_squaredRange))
			if ((i.getValue()->m_enabled) && (position.distance(worldPos) <= (i.getValue())->m_params.x))
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	}
	
	return numLights;
}



//*****************************************************************************
//*****************************************************************************
XStatus XLightMgr::setAmbientLight(XColor& ambient)
{
	m_ambientLight = ambient;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XColor& XLightMgr::getAmbientLight()
{
	return m_ambientLight;
}

XStatus XLightMgr::setLightingMode(XLightingMode mode)
{
	m_lightingMode = mode;
	return X_OK;
}

XLightingMode XLightMgr::getLightingMode()
{
	return m_lightingMode;
}


/*XStatus XLightMgr::getOcclusionMatrices(XMatrix4x4** ppOcclusionMatrices,XU32 maxLights)
{
	XU32 numLights = 0;
	XMemBlockList<XLight*>::iterator i;
	switch (m_lightingMode)
	{
	case X_LM_DEFAULT:
		break;
	case X_LM_SYSTEM:
		i.set(m_pointLights.begin());
		while (i.isValid())
		{
			if (i.getValue()->m_enabled && i.getValue()->getPriority() == SYSTEM_LIGHT_PRIORITY &&
				i.getValue()->isOcclusionEnabled())
				*ppOcclusionMatrices[numLights++] = &i.getValue()->getOcclusionMatrix();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	case X_LM_SCENE:
		//Return the point lights within range of the position passed in.  Do not
		//exceed the max number of lights indicated by the user
		i.set(m_pointLights.begin());
		XVector3D worldPos;
		while (i.isValid())
		{
			i.getValue()->getWorldPosition(worldPos);
			//if ((i.getValue()->m_enabled) && (position.distanceSquared(worldPos) <= (i.getValue())->m_squaredRange))
			if ((i.getValue()->m_enabled) && (position.distance(worldPos) <= (i.getValue())->m_params.x))
				pLights[numLights++] = i.getValue();
			if (numLights >= maxLights)
				break;
			i.getNext();
		}
		break;
	}
	
	return numLights;
}*/
