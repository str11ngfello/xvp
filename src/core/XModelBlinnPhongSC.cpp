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
//	Module Name:	XModelBlinnPhongSC.cpp
//	Author:			Lynn Duke
//	Creation Date:	10-16-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the model render controller.  
*/
//*****************************************************************************
XModelBlinnPhongSC::XModelBlinnPhongSC()
{
	m_vsHandle = X_INVALID_HANDLE;
	m_psHandle = X_INVALID_HANDLE;
	m_vsCBHandle = X_INVALID_HANDLE;
	m_psCBHandle = X_INVALID_HANDLE;
	m_inited = false;
	m_maxDir = 4;
	m_maxPoint = 4;
	m_maxSpot = 4;
}



//*****************************************************************************
/**
Destructor for the model render controller.  
*/
//*****************************************************************************
XModelBlinnPhongSC::~XModelBlinnPhongSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
char* XModelBlinnPhongSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XModelBlinnPhongSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelBlinnPhongSC::init(XS8* pShaderName)
{
	X_ASSERT(pShaderName);

	//Create vertex and pixel shaders necessary for rendering model objects.
	XStatus status = X_OK;

	status = XVP::GraphicsMgr.createVertexShader(pShaderName,"mainVS",m_vsHandle);
	if (status == X_OK)
	{
		status = XVP::GraphicsMgr.createPixelShader(pShaderName,"mainPS",m_psHandle);
		m_inited = true;
	}

	//Create constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(vsCB),false,m_vsCBHandle);
	XVP::GraphicsMgr.createConstantBuffer(sizeof(psCB),false,m_psCBHandle);

	return status;
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XModelBlinnPhongSC::destroy()
{
	//Must call base class destroy
	XShaderController::destroy();

	XVP::GraphicsMgr.destroyVertexShader(m_vsHandle);
	m_vsHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyPixelShader(m_psHandle);
	m_psHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyConstantBuffer(m_vsCBHandle);
	m_vsCBHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyConstantBuffer(m_psCBHandle);
	m_psCBHandle = X_INVALID_HANDLE;

	m_inited = false;

	return X_OK;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void XModelBlinnPhongSC::setShaderParamBlock(XS8* pMemoryChunk)
{
	m_psCB.v4ScaleParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams2.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_psCB.v4ScaleParams.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_psCB.v4ScaleParams2.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams2.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams2.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_psCB.v4AnimParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_vsCB.v4MiscParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4MiscParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

}

//*****************************************************************************
//*****************************************************************************
XStatus XModelBlinnPhongSC::render(XRenderEvent* pRenderEvent)
{
	XCamera* pCam = pRenderEvent->pScene->getActiveCamera();

	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	XMatrix4x4 invWorld = pRenderEvent->pSceneNode->getWorldTransform().getInverse();
	XVector3D camPos;
	pCam->getWorldPosition(camPos);

	//Set VS constants
	m_vsCB.wvpMatrix = pRenderEvent->pSceneNode->getWorldTransform() * pCam->getViewMatrix() * pCam->getProjMatrix();
	m_vsCB.prevWVPMatrix = pRenderEvent->pSceneNode->getPrevWorldTransform() * pCam->getPrevViewMatrix() * pCam->getPrevProjMatrix();
	m_vsCB.worldMatrix = pRenderEvent->pSceneNode->getWorldTransform();
	m_vsCB.wvMatrix = pRenderEvent->pSceneNode->getWorldTransform() * pCam->getViewMatrix();
	m_vsCB.v4CamPosObjSpace = invWorld * camPos;
		
	//First disable all lights
	for (XU32 i = 0;i < MAX_NUM_LIGHTS;++i)
		m_psCB.lightEnabled[i] = m_psCB.occEnabled[i] = 0;

	//Directional lights
	XLight* pLights[MAX_NUM_LIGHTS];
	XU32 numDirLights = 0;
	numDirLights = pRenderEvent->pScene->getLightMgr()->getDirectionalLights(pLights,m_maxDir);
	for (XU32 i = 0;i < numDirLights;++i)
	{
		//If we exceed the maximum number of allowed lights, we're done
		if (i >= MAX_NUM_LIGHTS)
			break;

		m_psCB.lightEnabled[i] = 1;
		m_psCB.lightTypes[i] = 0;
		m_psCB.v4LightDir[i] = pLights[i]->getWorldTransform().row3 * invWorld;
		m_psCB.v4LightDir[i].normalize();
		m_psCB.v4LightDiffuse[i] = (XVector4D)pLights[i]->m_diffuse;
		m_psCB.v4LightSpecular[i] = (XVector4D)pLights[i]->m_specular;

		if (pLights[i]->getOcclusionMatrix())
		{
			m_psCB.occEnabled[i] = 1;
			m_vsCB.occMatrices[i] = *pLights[i]->getOcclusionMatrix();
		}
	}

	//Point lights
	XVector3D nodeWorldPos;
	XVector3D lightWorldPos;
	pRenderEvent->pSceneNode->getWorldPosition(nodeWorldPos);
	XU32 numPointLights = pRenderEvent->pScene->getLightMgr()->getPointLightsInRange(nodeWorldPos,pLights,m_maxPoint);
	for (XU32 i = numDirLights;i < numDirLights + numPointLights;++i)
	{
		//If we exceed the maximum number of allowed lights, we're done
		if (i >= MAX_NUM_LIGHTS)
			break;

		XU32 lightOffset = i-numDirLights;
		m_psCB.lightEnabled[i] = 1;
		m_psCB.lightTypes[i] = 1;
		pLights[lightOffset]->getWorldPosition(lightWorldPos);
		m_psCB.v4LightPos[i] = invWorld * lightWorldPos;
		m_psCB.v4LightDiffuse[i] = (XVector4D)pLights[lightOffset]->m_diffuse;
		m_psCB.v4LightSpecular[i] = (XVector4D)pLights[lightOffset]->m_specular;
		m_psCB.v4LightParams[i] = pLights[lightOffset]->m_params;
	}

	//Spot lights
	XU32 numSpotLights = pRenderEvent->pScene->getLightMgr()->getSpotLightsInRange(nodeWorldPos,pLights,m_maxSpot);
	for (XU32 i = numDirLights + numPointLights;i < numDirLights + numPointLights + numSpotLights;++i)
	{
		//If we exceed the maximum number of allowed lights, we're done
		if (i >= MAX_NUM_LIGHTS)
			break;

		XU32 lightOffset = i-numDirLights-numPointLights;
		m_psCB.lightEnabled[i] = 1;
		m_psCB.lightTypes[i] = 2;
		pLights[lightOffset]->getWorldPosition(lightWorldPos);
		m_psCB.v4LightDir[i] = pLights[lightOffset]->getWorldTransform().row3 * invWorld;
		m_psCB.v4LightDir[i].normalize();
		m_psCB.v4LightPos[i] = invWorld * lightWorldPos;
		m_psCB.v4LightDiffuse[i] = (XVector4D)pLights[lightOffset]->m_diffuse;
		m_psCB.v4LightSpecular[i] = (XVector4D)pLights[lightOffset]->m_specular;
		m_psCB.v4LightParams[i] = pLights[lightOffset]->m_params;

		if (pLights[i]->getOcclusionMatrix())
		{
			m_psCB.occEnabled[i] = 1;
			m_vsCB.occMatrices[i] = *pLights[i]->getOcclusionMatrix();
		}
	}

	XVP::GraphicsMgr.setConstantBufferData(m_vsCBHandle,&m_vsCB,sizeof(m_vsCB));
	XVP::GraphicsMgr.setConstantBufferData(m_psCBHandle,&m_psCB,sizeof(m_psCB));
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_vsCBHandle);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_psCBHandle);

	//Now, with constants set up, render the node.
	pRenderEvent->pSceneNode->render(pRenderEvent);

	return X_OK;
}
	


void XModelBlinnPhongSC::getLightLimits(XU32& maxDir,XU32& maxPoint,XU32& maxSpot)
{
	maxDir = m_maxDir;
	maxPoint = m_maxPoint;
	maxSpot = m_maxSpot;
}



void XModelBlinnPhongSC::setLightLimits(XU32 maxDir,XU32 maxPoint,XU32 maxSpot)
{
	m_maxDir = maxDir;
	m_maxPoint = maxPoint;
	m_maxSpot = maxSpot;
}



XF32 XModelBlinnPhongSC::getSpecPower()
{
	return m_psCB.v4ScaleParams.x;
}



void XModelBlinnPhongSC::setSpecPower(XF32 specPower)
{
	m_psCB.v4ScaleParams.x = X_MATH_MAX(specPower,0);
}



XF32 XModelBlinnPhongSC::getAOScale()
{
	return m_psCB.v4ScaleParams.y;
}



void XModelBlinnPhongSC::setAOScale(XF32 aoScale)
{
	m_psCB.v4ScaleParams.y = X_MATH_MAX(aoScale,0);
}



XF32 XModelBlinnPhongSC::getEnvScale()
{
	return m_psCB.v4ScaleParams.z;
}



void XModelBlinnPhongSC::setEnvScale(XF32 envScale)
{
	m_psCB.v4ScaleParams.z = X_MATH_MAX(envScale,0);
}



XF32 XModelBlinnPhongSC::getSpecScale()
{
	return m_psCB.v4ScaleParams.w;
}


void XModelBlinnPhongSC::setSpecScale(XF32 specScale)
{
	m_psCB.v4ScaleParams.w = X_MATH_MAX(specScale,0);
}

XF32 XModelBlinnPhongSC::getDiffScale()
{
	return m_psCB.v4ScaleParams2.x;
}


void XModelBlinnPhongSC::setDiffScale(XF32 diffScale)
{
	m_psCB.v4ScaleParams2.x = X_MATH_MAX(diffScale,0);
}


XF32 XModelBlinnPhongSC::getEmissScale()
{
	return m_psCB.v4ScaleParams2.y;
}


void XModelBlinnPhongSC::setEmissScale(XF32 emissScale)
{
	m_psCB.v4ScaleParams2.y = X_MATH_MAX(emissScale,0);
}

XF32 XModelBlinnPhongSC::getDNormTilingFactor()
{
	return m_psCB.v4ScaleParams2.z;
}


void XModelBlinnPhongSC::setDNormTilingFactor(XF32 tilingFactor)
{
	m_psCB.v4ScaleParams2.z = X_MATH_MAX(tilingFactor,0);
}

XF32 XModelBlinnPhongSC::getDiffUSpeed()
{
	return m_psCB.v4AnimParams.x;
}
void XModelBlinnPhongSC::setDiffUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.x = uSpeed;
}

XF32 XModelBlinnPhongSC::getDiffVSpeed()
{
	return m_psCB.v4AnimParams.y;
}
void XModelBlinnPhongSC::setDiffVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.y = vSpeed;
}

XF32 XModelBlinnPhongSC::getEmissUSpeed()
{
	return m_psCB.v4AnimParams.z;
}
void XModelBlinnPhongSC::setEmissUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.z = uSpeed;
}
XF32 XModelBlinnPhongSC::getEmissVSpeed()
{
	return m_psCB.v4AnimParams.w;
}
void XModelBlinnPhongSC::setEmissVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.w = vSpeed;
}

XF32 XModelBlinnPhongSC::getFresnelIndex()
{
	return m_psCB.v4ScaleParams2.w;
}
void XModelBlinnPhongSC::setFresnelIndex(XF32 fresnelIndex)
{
	m_psCB.v4ScaleParams2.w = fresnelIndex;
}
XF32 XModelBlinnPhongSC::getObjectAlpha()
{
	return m_psCB.v4MiscParams.x;
}
void XModelBlinnPhongSC::setObjectAlpha(XF32 alpha)
{
	m_psCB.v4MiscParams.x = alpha;
	X_MATH_CLAMP(m_psCB.v4MiscParams.x,0.0f,1.0f);
}
XF32 XModelBlinnPhongSC::getInflateAmount()
{
	return m_vsCB.v4MiscParams.x;
}
void XModelBlinnPhongSC::setInflateAmount(XF32 inflation)
{
	m_vsCB.v4MiscParams.x = inflation;
	X_MATH_CLAMP(m_vsCB.v4MiscParams.x,-50.0f,50.0f);
}

XStatus XModelBlinnPhongSC::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("Spec Power",m_psCB.v4ScaleParams.x = value)
	PB_MAKE_PROPERTY("Fresnel Index",m_psCB.v4ScaleParams2.w = value)
	PB_MAKE_PROPERTY("AO Map Scale",m_psCB.v4ScaleParams.y = value)
	PB_MAKE_PROPERTY("Env Map Scale",m_psCB.v4ScaleParams.z = value)
	PB_MAKE_PROPERTY("Spec Map Scale",m_psCB.v4ScaleParams.w = value)
	PB_MAKE_PROPERTY("Diff Map Scale",m_psCB.v4ScaleParams2.x = value)
	PB_MAKE_PROPERTY("Emiss Map Scale",m_psCB.v4ScaleParams2.y = value)
	PB_MAKE_PROPERTY("Detail Normal Tiling",m_psCB.v4ScaleParams2.z = value)
	PB_MAKE_PROPERTY("Diff Map U Speed",m_psCB.v4AnimParams.x = value)
	PB_MAKE_PROPERTY("Diff Map V Speed",m_psCB.v4AnimParams.y = value)
	PB_MAKE_PROPERTY("Emiss Map U Speed",m_psCB.v4AnimParams.z = value)
	PB_MAKE_PROPERTY("Emiss Map V Speed",m_psCB.v4AnimParams.w = value)
	PB_MAKE_PROPERTY("Object Alpha",m_psCB.v4MiscParams.x = value)
	PB_MAKE_PROPERTY("Inflate Amount",m_vsCB.v4MiscParams.x = value)

	return X_ERR_NO_SUCH_OBJECT;
}