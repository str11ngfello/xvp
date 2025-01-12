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
//	Module Name:	XParticleSystemNoLightSC.cpp
//	Author:			Lynn Duke
//	Creation Date:	10-16-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

XF32 XParticleSystemNoLightSC::m_preRotatedQuad[16];

//*****************************************************************************
/**
Constructor for the model render controller.  
*/
//*****************************************************************************
XParticleSystemNoLightSC::XParticleSystemNoLightSC()
{
	m_vsHandle = X_INVALID_HANDLE;
	m_psHandle = X_INVALID_HANDLE;
	m_vsCBHandle = X_INVALID_HANDLE;
	m_psCBHandle = X_INVALID_HANDLE;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the model render controller.  
*/
//*****************************************************************************
XParticleSystemNoLightSC::~XParticleSystemNoLightSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
char* XParticleSystemNoLightSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XParticleSystemNoLightSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemNoLightSC::init(XS8* pShaderName)
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
XStatus XParticleSystemNoLightSC::destroy()
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
void XParticleSystemNoLightSC::setShaderParamBlock(XS8* pMemoryChunk)
{
	m_psCB.v4ScaleParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_psCB.v4AnimParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_psCB.v4ScaleParams.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

}

//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemNoLightSC::render(XRenderEvent* pRenderEvent)
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

	//Directional lights
	XLight* pLights[MAX_NUM_LIGHTS];
	XU32 numDirLights = 0;
	numDirLights = pRenderEvent->pScene->getLightMgr()->getDirectionalLights(pLights,MAX_NUM_LIGHTS);
	for (XU32 i = 0;i < numDirLights;++i)
	{
		//If we exceed the maximum number of allowed lights, we're done
		if (i >= MAX_NUM_LIGHTS)
			break;

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
	


XF32 XParticleSystemNoLightSC::getDiffScale()
{
	return m_psCB.v4ScaleParams.x;
}


void XParticleSystemNoLightSC::setDiffScale(XF32 diffScale)
{
	m_psCB.v4ScaleParams.x = X_MATH_MAX(diffScale,0);
}


XF32 XParticleSystemNoLightSC::getEmissScale()
{
	return m_psCB.v4ScaleParams.y;
}


void XParticleSystemNoLightSC::setEmissScale(XF32 emissScale)
{
	m_psCB.v4ScaleParams.y = X_MATH_MAX(emissScale,0);
}

XF32 XParticleSystemNoLightSC::getDiffUSpeed()
{
	return m_psCB.v4AnimParams.x;
}
void XParticleSystemNoLightSC::setDiffUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.x = uSpeed;
}

XF32 XParticleSystemNoLightSC::getDiffVSpeed()
{
	return m_psCB.v4AnimParams.y;
}
void XParticleSystemNoLightSC::setDiffVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.y = vSpeed;
}

XF32 XParticleSystemNoLightSC::getEmissUSpeed()
{
	return m_psCB.v4AnimParams.z;
}
void XParticleSystemNoLightSC::setEmissUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.z = uSpeed;
}
XF32 XParticleSystemNoLightSC::getEmissVSpeed()
{
	return m_psCB.v4AnimParams.w;
}
void XParticleSystemNoLightSC::setEmissVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.w = vSpeed;
}
XF32 XParticleSystemNoLightSC::getObjectAlpha()
{
	return m_psCB.v4ScaleParams.z;
}
void XParticleSystemNoLightSC::setObjectAlpha(XF32 alpha)
{
	m_psCB.v4ScaleParams.z = alpha;
	X_MATH_CLAMP(m_psCB.v4ScaleParams.z,0.0f,1.0f);
}

XF32 XParticleSystemNoLightSC::getSoftness()
{
	return m_psCB.v4ScaleParams.w;
}
void XParticleSystemNoLightSC::setSoftness(XF32 softness)
{
	m_psCB.v4ScaleParams.w = softness;
	X_MATH_CLAMP(m_psCB.v4ScaleParams.w,0.00001f,10.0f);
}

XStatus XParticleSystemNoLightSC::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("Diff Map Scale",m_psCB.v4ScaleParams.x = value)
	PB_MAKE_PROPERTY("Emiss Map Scale",m_psCB.v4ScaleParams.y = value)
	PB_MAKE_PROPERTY("Diff Map U Speed",m_psCB.v4AnimParams.x = value)
	PB_MAKE_PROPERTY("Diff Map V Speed",m_psCB.v4AnimParams.y = value)
	PB_MAKE_PROPERTY("Emiss Map U Speed",m_psCB.v4AnimParams.z = value)
	PB_MAKE_PROPERTY("Emiss Map V Speed",m_psCB.v4AnimParams.w = value)
	PB_MAKE_PROPERTY("Object Alpha",m_psCB.v4ScaleParams.z = value)
	PB_MAKE_PROPERTY("Softness",m_psCB.v4ScaleParams.w = value)

	return X_ERR_NO_SUCH_OBJECT;
}