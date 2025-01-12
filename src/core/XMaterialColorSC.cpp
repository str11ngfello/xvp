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
//	Module Name:	XMaterialColorSC.cpp
//	Author:			Lynn Duke
//	Creation Date:	11-19-2005
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
XMaterialColorSC::XMaterialColorSC()
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
XMaterialColorSC::~XMaterialColorSC()
{
}



//*****************************************************************************
/**
Returns a string description of the model render controller.  This is the
description that appears in log files and other debug output.

@return A string description of the model render controller.
*/
//*****************************************************************************
XS8* XMaterialColorSC::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XMaterialColorSC";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XMaterialColorSC::init(XS8* pShaderName)
{
	//Create vertex and pixel shaders necessary for rendering particle systems.
	XStatus status = X_OK;
	
	status = XVP::GraphicsMgr.createVertexShader("Material Color.fx","mainVS",m_vsHandle);
	if (status == X_OK)
	{
		status = XVP::GraphicsMgr.createPixelShader("Material Color.fx","mainPS",m_psHandle);
		m_inited = true;
	}

	//Create constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(vsCB),false,m_vsCBHandle);
	XVP::GraphicsMgr.createConstantBuffer(sizeof(psCB),false,m_psCBHandle);
	XVP::GraphicsMgr.setConstantBufferData(m_vsCBHandle,&m_vsCB,sizeof(m_vsCB));
	XVP::GraphicsMgr.setConstantBufferData(m_psCBHandle,&m_psCB,sizeof(m_psCB));

	return status;
}

	

//*****************************************************************************
//*****************************************************************************
XStatus XMaterialColorSC::destroy()
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


//*****************************************************************************
//*****************************************************************************
XStatus XMaterialColorSC::render(XRenderEvent* pRenderEvent)
{
	//Set shaders
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	m_vsCB.wvpMatrix = pRenderEvent->pSceneNode->getWorldTransform() * 
		pRenderEvent->pScene->getActiveCamera()->getViewMatrix() * 
		pRenderEvent->pScene->getActiveCamera()->getProjMatrix();

	XVP::GraphicsMgr.setConstantBufferData(m_vsCBHandle,&m_vsCB,sizeof(m_vsCB));
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_vsCBHandle);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_psCBHandle);

	//Now, with constants set up, render the node.
	pRenderEvent->pSceneNode->render(pRenderEvent);

	return X_OK;
}

XColor XMaterialColorSC::getColor()
{
	return m_psCB.materialColor;
}

void XMaterialColorSC::setColor(XColor& color)
{
	m_psCB.materialColor = color;
	XVP::GraphicsMgr.setConstantBufferData(m_psCBHandle,&m_psCB,sizeof(m_psCB));
}

XF32 XMaterialColorSC::getDepthBias()
{
	return m_vsCB.depthBias.x;
}


void XMaterialColorSC::setDepthBias(XF32 depthBias)
{
	m_vsCB.depthBias.x = depthBias;
}
	  


XStatus XMaterialColorSC::setColorProperty(XS8* pPropertyName,XColor& value)
{
	PB_MAKE_PROPERTY("Material Color",setColor(value))

	return X_ERR_NO_SUCH_OBJECT;
}
