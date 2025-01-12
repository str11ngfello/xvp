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
//	Module Name:	XModelNoLightSC.cpp
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
XModelNoLightSC::XModelNoLightSC()
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
XModelNoLightSC::~XModelNoLightSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
char* XModelNoLightSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XModelNoLightSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelNoLightSC::init(XS8* pShaderName)
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
XStatus XModelNoLightSC::destroy()
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
void XModelNoLightSC::setShaderParamBlock(XS8* pMemoryChunk)
{
	m_psCB.v4ScaleParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4ScaleParams.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);

	m_psCB.v4AnimParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.y = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_psCB.v4AnimParams.w = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
		
	m_psCB.v4ScaleParams.z = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	m_vsCB.miscParams.x = (*(XF32*)pMemoryChunk);		pMemoryChunk += sizeof(XF32);
	
}

//*****************************************************************************
//*****************************************************************************
XStatus XModelNoLightSC::render(XRenderEvent* pRenderEvent)
{
	XCamera* pCam = pRenderEvent->pScene->getActiveCamera();

	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	//Set VS constants
	m_vsCB.wvpMatrix = pRenderEvent->pSceneNode->getWorldTransform() * pCam->getViewMatrix() * pCam->getProjMatrix();
	m_vsCB.prevWVPMatrix = pRenderEvent->pSceneNode->getPrevWorldTransform() * pCam->getPrevViewMatrix() * pCam->getPrevProjMatrix();
	m_vsCB.worldMatrix = pRenderEvent->pSceneNode->getWorldTransform();

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
	


XF32 XModelNoLightSC::getDiffScale()
{
	return m_psCB.v4ScaleParams.x;
}


void XModelNoLightSC::setDiffScale(XF32 diffScale)
{
	m_psCB.v4ScaleParams.x = X_MATH_MAX(diffScale,0);
}


XF32 XModelNoLightSC::getEmissScale()
{
	return m_psCB.v4ScaleParams.y;
}


void XModelNoLightSC::setEmissScale(XF32 emissScale)
{
	m_psCB.v4ScaleParams.y = X_MATH_MAX(emissScale,0);
}

XF32 XModelNoLightSC::getDiffUSpeed()
{
	return m_psCB.v4AnimParams.x;
}
void XModelNoLightSC::setDiffUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.x = uSpeed;
}

XF32 XModelNoLightSC::getDiffVSpeed()
{
	return m_psCB.v4AnimParams.y;
}
void XModelNoLightSC::setDiffVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.y = vSpeed;
}

XF32 XModelNoLightSC::getEmissUSpeed()
{
	return m_psCB.v4AnimParams.z;
}
void XModelNoLightSC::setEmissUSpeed(XF32 uSpeed)
{
	m_psCB.v4AnimParams.z = uSpeed;
}
XF32 XModelNoLightSC::getEmissVSpeed()
{
	return m_psCB.v4AnimParams.w;
}
void XModelNoLightSC::setEmissVSpeed(XF32 vSpeed)
{
	m_psCB.v4AnimParams.w = vSpeed;
}
XF32 XModelNoLightSC::getObjectAlpha()
{
	return m_psCB.v4ScaleParams.z;
}
void XModelNoLightSC::setObjectAlpha(XF32 alpha)
{
	m_psCB.v4ScaleParams.z = alpha;
	X_MATH_CLAMP(m_psCB.v4ScaleParams.z,0.0f,1.0f);
}
XF32 XModelNoLightSC::getInflateAmount()
{
	return m_vsCB.miscParams.x;
}
void XModelNoLightSC::setInflateAmount(XF32 inflation)
{
	m_vsCB.miscParams.x = inflation;
	X_MATH_CLAMP(m_vsCB.miscParams.x,-50.0f,50.0f);
}

XStatus XModelNoLightSC::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("Diff Map Scale",m_psCB.v4ScaleParams.x = value)
	PB_MAKE_PROPERTY("Emiss Map Scale",m_psCB.v4ScaleParams.y = value)
	PB_MAKE_PROPERTY("Diff Map U Speed",m_psCB.v4AnimParams.x = value)
	PB_MAKE_PROPERTY("Diff Map V Speed",m_psCB.v4AnimParams.y = value)
	PB_MAKE_PROPERTY("Emiss Map U Speed",m_psCB.v4AnimParams.z = value)
	PB_MAKE_PROPERTY("Emiss Map V Speed",m_psCB.v4AnimParams.w = value)
	PB_MAKE_PROPERTY("Object Alpha",m_psCB.v4ScaleParams.z = value)
	PB_MAKE_PROPERTY("Inflate Amount",m_vsCB.miscParams.x = value)

	return X_ERR_NO_SUCH_OBJECT;
}