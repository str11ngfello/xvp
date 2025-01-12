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

#include <xvpsdk.h>

XF32 XParticleSystemBlinnPhongSC::m_preRotatedQuad[16];

//*****************************************************************************
/**
Constructor for the model render controller.  
*/
//*****************************************************************************
XParticleSystemBlinnPhongSC::XParticleSystemBlinnPhongSC()
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
XParticleSystemBlinnPhongSC::~XParticleSystemBlinnPhongSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
char* XParticleSystemBlinnPhongSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XParticleSystemBlinnPhongSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemBlinnPhongSC::init(XS8* pShaderName)
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
XStatus XParticleSystemBlinnPhongSC::destroy()
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
void XParticleSystemBlinnPhongSC::setShaderParamBlock(XS8* pMemoryChunk)
{
	m_psCB.v4ScaleParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
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

	m_psCB.v4ScaleParams2.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4MiscParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
}

//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemBlinnPhongSC::render(XRenderEvent* pRenderEvent)
{
	XCamera* pCam = pRenderEvent->pScene->getActiveCamera();

	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	//Get an inverse view matrix...it will be used to build
	//a pre rotated quad..
	XMatrix4x4 inverseView = pRenderEvent->pScene->getActiveCamera()->getViewMatrix().getInverse();

	XVector3D vRight(inverseView._11,inverseView._12,inverseView._13);
	XVector3D vUp(inverseView._21,inverseView._22,inverseView._23);

	XVector3D vPoint0 = (-vRight - vUp);
    XVector3D vPoint1 = ( vRight - vUp);
    XVector3D vPoint2 = ( vRight + vUp);
    XVector3D vPoint3 = (-vRight + vUp);

    m_preRotatedQuad[0] = vPoint0.x;
	m_preRotatedQuad[1] = vPoint0.y; 
	m_preRotatedQuad[2] = vPoint0.z;
	m_preRotatedQuad[3] = 0.0f;
    m_preRotatedQuad[4] = vPoint3.x;
	m_preRotatedQuad[5] = vPoint3.y; 
	m_preRotatedQuad[6] = vPoint3.z;
	m_preRotatedQuad[7] = 0.0f;
    m_preRotatedQuad[8] = vPoint1.x; 
	m_preRotatedQuad[9] = vPoint1.y; 
	m_preRotatedQuad[10] = vPoint1.z; 
	m_preRotatedQuad[11] = 0.0f;
    m_preRotatedQuad[12] = vPoint2.x;
	m_preRotatedQuad[13] = vPoint2.y;
	m_preRotatedQuad[14] = vPoint2.z;
	m_preRotatedQuad[15] = 0.0f;

	//We don't want to use the previous world transform from the emitter
	//because once particles are emitted they aren't affected by emitter
	//motion 
	m_vsCB.vpMatrix = pRenderEvent->pScene->getActiveCamera()->getPrevViewMatrix() * 
					pRenderEvent->pScene->getActiveCamera()->getPrevProjMatrix();
	memcpy(&m_vsCB.quad,m_preRotatedQuad,sizeof(m_preRotatedQuad)); 

	XMatrix4x4 invWorld = pRenderEvent->pSceneNode->getWorldTransform().getInverse();
	XVector3D camPos;
	pCam->getWorldPosition(camPos);

	//Set VS constants
	m_vsCB.wvpMatrix = pRenderEvent->pSceneNode->getWorldTransform() * pCam->getViewMatrix() * pCam->getProjMatrix();
	m_vsCB.prevWVPMatrix = pRenderEvent->pSceneNode->getPrevWorldTransform() * pCam->getPrevViewMatrix() * pCam->getPrevProjMatrix();
	m_vsCB.worldMatrix = pRenderEvent->pSceneNode->getWorldTransform();
	m_vsCB.wvMatrix = pRenderEvent->pSceneNode->getWorldTransform() * pCam->getViewMatrix();
	m_vsCB.v4CamPosWorldSpace = camPos;//invWorld * camPos;
		
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
	


void XParticleSystemBlinnPhongSC::getLightLimits(XU32& maxDir,XU32& maxPoint,XU32& maxSpot)
{
	maxDir = m_maxDir;
	maxPoint = m_maxPoint;
	maxSpot = m_maxSpot;
}



void XParticleSystemBlinnPhongSC::setLightLimits(XU32 maxDir,XU32 maxPoint,XU32 maxSpot)
{
	m_maxDir = maxDir;
	m_maxPoint = maxPoint;
	m_maxSpot = maxSpot;
}



XF32 XParticleSystemBlinnPhongSC::getSpecPower()
{
	return m_psCB.v4ScaleParams.x;
}



void XParticleSystemBlinnPhongSC::setSpecPower(XF32 specPower)
{
	m_psCB.v4ScaleParams.x = X_MATH_MAX(specPower,0);
}



XF32 XParticleSystemBlinnPhongSC::getAOScale()
{
	return m_psCB.v4ScaleParams.y;
}



void XParticleSystemBlinnPhongSC::setAOScale(XF32 aoScale)
{
	m_psCB.v4ScaleParams.y = X_MATH_MAX(aoScale,0);
}



XF32 XParticleSystemBlinnPhongSC::getEnvScale()
{
	return m_psCB.v4ScaleParams.z;
}



void XParticleSystemBlinnPhongSC::setEnvScale(XF32 envScale)
{
	m_psCB.v4ScaleParams.z = X_MATH_MAX(envScale,0);
}



XF32 XParticleSystemBlinnPhongSC::getSpecScale()
{
	return m_psCB.v4ScaleParams.w;
}


void XParticleSystemBlinnPhongSC::setSpecScale(XF32 specScale)
{
	m_psCB.v4ScaleParams.w = X_MATH_MAX(specScale,0);
}

XF32 XParticleSystemBlinnPhongSC::getDiffScale()
{
	return m_psCB.v4ScaleParams2.x;
}


void XParticleSystemBlinnPhongSC::setDiffScale(XF32 diffScale)
{
	m_psCB.v4ScaleParams2.x = X_MATH_MAX(diffScale,0);
}


XF32 XParticleSystemBlinnPhongSC::getEmissScale()
{
	return m_psCB.v4ScaleParams2.y;
}


void XParticleSystemBlinnPhongSC::setEmissScale(XF32 emissScale)
{
	m_psCB.v4ScaleParams2.y = X_MATH_MAX(emissScale,0);
}

XF32 XParticleSystemBlinnPhongSC::getDNormTilingFactor()
{
	return m_psCB.v4ScaleParams2.z;
}


void XParticleSystemBlinnPhongSC::setDNormTilingFactor(XF32 tilingFactor)
{
	m_psCB.v4ScaleParams2.z = X_MATH_MAX(tilingFactor,0);
}

XF32 XParticleSystemBlinnPhongSC::getDiffUSpeed()
{
	return m_psCB.v4AnimParams.x;
}
void XParticleSystemBlinnPhongSC::setDiffUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.x = uSpeed;
}

XF32 XParticleSystemBlinnPhongSC::getDiffVSpeed()
{
	return m_psCB.v4AnimParams.y;
}
void XParticleSystemBlinnPhongSC::setDiffVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.y = vSpeed;
}

XF32 XParticleSystemBlinnPhongSC::getEmissUSpeed()
{
	return m_psCB.v4AnimParams.z;
}
void XParticleSystemBlinnPhongSC::setEmissUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.z = uSpeed;
}
XF32 XParticleSystemBlinnPhongSC::getEmissVSpeed()
{
	return m_psCB.v4AnimParams.w;
}
void XParticleSystemBlinnPhongSC::setEmissVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.w = vSpeed;
}
XF32 XParticleSystemBlinnPhongSC::getObjectAlpha()
{
	return m_psCB.v4ScaleParams2.w;
}
void XParticleSystemBlinnPhongSC::setObjectAlpha(XF32 alpha)
{
	m_psCB.v4ScaleParams2.w = alpha;
	X_MATH_CLAMP(m_psCB.v4ScaleParams2.w,0.0f,1.0f);
}

XF32 XParticleSystemBlinnPhongSC::getSoftness()
{
	return m_psCB.v4MiscParams.x;
}
void XParticleSystemBlinnPhongSC::setSoftness(XF32 softness)
{
	m_psCB.v4MiscParams.x = softness;
	X_MATH_CLAMP(m_psCB.v4MiscParams.x,0.00001f,10.0f);
}

XStatus XParticleSystemBlinnPhongSC::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("Spec Power",m_psCB.v4ScaleParams.x = value)
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
	PB_MAKE_PROPERTY("Softness",m_psCB.v4MiscParams.x = value)

	return X_ERR_NO_SUCH_OBJECT;
}

















#if 0
#include <XParticleSystemBlinnPhongSC.h>
#include <XVP.h>
#include <XScene.h>


XF32 XParticleSystemBlinnPhongSC::m_preRotatedQuad[16];


//*****************************************************************************
/**
Constructor for the model render controller.  
*/
//*****************************************************************************
XParticleSystemBlinnPhongSC::XParticleSystemBlinnPhongSC()
{
	m_vsHandle = X_INVALID_HANDLE;
	m_psHandle = X_INVALID_HANDLE;
	m_vsCBHandle = X_INVALID_HANDLE;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the model render controller.  
*/
//*****************************************************************************
XParticleSystemBlinnPhongSC::~XParticleSystemBlinnPhongSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
XS8* XParticleSystemBlinnPhongSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XParticleSystemBlinnPhongSC";
	return description; 
}


//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemBlinnPhongSC::init(XS8* pShaderName)
{
	X_ASSERT(pShaderName);

	//Create vertex and pixel shaders necessary for rendering particle systems.
	XStatus status = X_OK;
	
	status = XVP::GraphicsMgr.createVertexShader(pShaderName,"mainVS",m_vsHandle);
	if (status == X_OK)
	{
		status = XVP::GraphicsMgr.createPixelShader(pShaderName,"mainPS",m_psHandle);
		m_inited = true;
	}

	//Create constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(vsCB),false,m_vsCBHandle);

	return status;
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemBlinnPhongSC::destroy()
{
	//Must call base class destroy
	XShaderController::destroy();

	XVP::GraphicsMgr.destroyVertexShader(m_vsHandle);
	m_vsHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyPixelShader(m_psHandle);
	m_psHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyConstantBuffer(m_vsCBHandle);
	m_vsCBHandle = X_INVALID_HANDLE;

	m_inited = false;

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemBlinnPhongSC::render(XRenderEvent* pRenderEvent)
{
	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	m_vsCB.vpMatrix = pRenderEvent->pScene->getActiveCamera()->getViewMatrix() * 
					pRenderEvent->pScene->getActiveCamera()->getProjMatrix();
	
	//Get an inverse view matrix...it will be used to build
	//a pre rotated quad..
	XMatrix4x4 inverseView = pRenderEvent->pScene->getActiveCamera()->getViewMatrix().getInverse();

	XVector3D vRight(inverseView._11,inverseView._12,inverseView._13);
	XVector3D vUp(inverseView._21,inverseView._22,inverseView._23);

	XVector3D vPoint0 = (-vRight - vUp);
    XVector3D vPoint1 = ( vRight - vUp);
    XVector3D vPoint2 = ( vRight + vUp);
    XVector3D vPoint3 = (-vRight + vUp);

    m_preRotatedQuad[0] = vPoint0.x;
	m_preRotatedQuad[1] = vPoint0.y; 
	m_preRotatedQuad[2] = vPoint0.z;
	m_preRotatedQuad[3] = 0.0f;
    m_preRotatedQuad[4] = vPoint3.x;
	m_preRotatedQuad[5] = vPoint3.y; 
	m_preRotatedQuad[6] = vPoint3.z;
	m_preRotatedQuad[7] = 0.0f;
    m_preRotatedQuad[8] = vPoint1.x; 
	m_preRotatedQuad[9] = vPoint1.y; 
	m_preRotatedQuad[10] = vPoint1.z; 
	m_preRotatedQuad[11] = 0.0f;
    m_preRotatedQuad[12] = vPoint2.x;
	m_preRotatedQuad[13] = vPoint2.y;
	m_preRotatedQuad[14] = vPoint2.z;
	m_preRotatedQuad[15] = 0.0f;
	
	XU32 s = sizeof(m_preRotatedQuad);
	
	//We don't want to use the previous world transform from the emitter
	//because once particles are emitted they aren't affected by emitter
	//motion 
	m_vsCB.vpMatrix = pRenderEvent->pScene->getActiveCamera()->getPrevViewMatrix() * 
					pRenderEvent->pScene->getActiveCamera()->getPrevProjMatrix();
	memcpy(&m_vsCB.quad,m_preRotatedQuad,sizeof(m_preRotatedQuad)); 

	XVP::GraphicsMgr.setConstantBufferData(m_vsCBHandle,&m_vsCB,sizeof(m_vsCB));
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_vsCBHandle);

	//Now, with constants set up, render the node.
	pRenderEvent->pSceneNode->render(pRenderEvent);

	return X_OK;
}
	  



#endif