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


//*****************************************************************************
/**
Constructor for the model render controller.  
*/
//*****************************************************************************
XDistortionSC::XDistortionSC()
{
	m_vsHandle = X_INVALID_HANDLE;
	m_psHandle = X_INVALID_HANDLE;
	m_vsCBHandle = X_INVALID_HANDLE;
	m_psCBHandle = X_INVALID_HANDLE;
	m_inited = false;
	m_uSpeed = 0.0f;
	m_vSpeed = 0.0f;
}



//*****************************************************************************
/**
Destructor for the model render controller.  
*/
//*****************************************************************************
XDistortionSC::~XDistortionSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
char* XDistortionSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XDistortionSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XDistortionSC::init(XS8* pShaderName)
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
XStatus XDistortionSC::destroy()
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


XStatus XDistortionSC::update(XF32 dt)
{
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XDistortionSC::render(XRenderEvent* pRenderEvent)
{
	XCamera* pCam = pRenderEvent->pScene->getActiveCamera();

	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	//Set VS constants
	m_vsCB.wvpMatrix = pRenderEvent->pSceneNode->getWorldTransform() * pCam->getViewMatrix() * pCam->getProjMatrix();

	XVP::GraphicsMgr.setConstantBufferData(m_vsCBHandle,&m_vsCB,sizeof(m_vsCB));
	XVP::GraphicsMgr.setConstantBufferData(m_psCBHandle,&m_psCB,sizeof(m_psCB));
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_vsCBHandle);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_psCBHandle);

	//Now, with constants set up, render the node.
	pRenderEvent->pSceneNode->render(pRenderEvent);

	return X_OK;
}
	

void XDistortionSC::setUAnimSpeed(XF32 speed)
{
	m_psCB.params.x = speed;
}

void XDistortionSC::setVAnimSpeed(XF32 speed)
{
	m_psCB.params.y = speed;
}


XStatus XDistortionSC::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("U Anim Speed",setUAnimSpeed(value))
	PB_MAKE_PROPERTY("V Anim Speed",setVAnimSpeed(value))

	return X_ERR_NO_SUCH_OBJECT;
}