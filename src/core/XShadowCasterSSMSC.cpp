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
//	Module Name:	XShadowCasterSSMSC.cpp
//	Author:			Lynn Duke
//	Creation Date:	10-13-2008
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
XShadowCasterSSMSC::XShadowCasterSSMSC()
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
XShadowCasterSSMSC::~XShadowCasterSSMSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
XS8* XShadowCasterSSMSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XShadowCasterSSMSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XShadowCasterSSMSC::init(XS8* pShaderName)
{
	//Create vertex and pixel shaders necessary for rendering particle systems.
	XStatus status = X_OK;
	
	status = XVP::GraphicsMgr.createVertexShader("Shadow Caster SSM.fx","mainVS",m_vsHandle);
	if (status == X_OK)
	{
		status = XVP::GraphicsMgr.createPixelShader("Shadow Caster SSM.fx","mainPS",m_psHandle);
		m_inited = true;
	}

	//Create constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(vsCB),false,m_vsCBHandle);

	return status;
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XShadowCasterSSMSC::destroy()
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
XStatus XShadowCasterSSMSC::render(XRenderEvent* pRenderEvent)
{
	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	m_vsCB.wvpMatrix = pRenderEvent->pSceneNode->getWorldTransform() *
			pRenderEvent->pScene->getActiveCamera()->getViewMatrix() * 
			pRenderEvent->pScene->getActiveCamera()->getProjMatrix();
	
	XVP::GraphicsMgr.setConstantBufferData(m_vsCBHandle,&m_vsCB,sizeof(m_vsCB));
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_vsCBHandle);

	//Now, with constants set up, render the node.
	pRenderEvent->pSceneNode->render(pRenderEvent);

	return X_OK;
}


XF32 XShadowCasterSSMSC::getDepthBias()
{
	return m_vsCB.depthBias.x;
}


void XShadowCasterSSMSC::setDepthBias(XF32 depthBias)
{
	m_vsCB.depthBias.x = depthBias;
}
	  

