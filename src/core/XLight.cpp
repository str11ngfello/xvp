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
//	Module Name:	XLight.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XLight::XLight()
{
	m_pOccScene = NULL;
	m_pOccMatrix = NULL;
	m_pOccCam = NULL;
	m_pOccSC = NULL;
}



//*****************************************************************************
//*****************************************************************************
XLight::~XLight()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XLight::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_LIGHT,X_SNID_LIGHT);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the light.  This is the
description that appears in log files and other debug output.

@return A string description of the light.
*/
//*****************************************************************************
XS8* XLight::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XLight";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XLight::init(XLightDesc& desc)
{
	m_lightType = desc.type;
	m_diffuse = desc.diffuse;
	m_specular = desc.specular;
	m_params.x = desc.cutoffDistance;
	m_params.y = desc.hotSpotDistance;
	m_params.z = desc.cosHalfAngleCutoff;
	m_params.w = desc.cosHalfAngleDropoff;
	m_priority = desc.priority;

	if (desc.enableOcclusion)
		enableOcclusion(true);

	m_boundingSphere.radius = desc.cutoffDistance;
	m_boundingSphere.center = XVector3D();

	m_enabled = true;

	return X_OK;
}



XStatus XLight::initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize)
{
	m_lightType = (X_LIGHT_TYPE)*(XU32*)pMemoryChunk; pMemoryChunk += sizeof(XU32);
	m_diffuse = *(XColor*)pMemoryChunk; pMemoryChunk += sizeof(XColor);
	m_specular = *(XColor*)pMemoryChunk; pMemoryChunk += sizeof(XColor);
	m_params.x = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
	m_params.y = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
	m_params.z = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
	m_params.w = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
	m_priority = *(XU32*)pMemoryChunk; pMemoryChunk += sizeof(XU32);

	m_boundingSphere.radius = m_params.x;
	m_boundingSphere.center = XVector3D();

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XLight::destroy()
{
	//Destroy occlusion data
	if (m_pOccSC)
	{
		m_pOccSC->destroy();
		XVP::GraphicsMgr.destroyMaterial(m_pOccMat);
		m_pOccScene->destroy();
		m_pOccCam->destroy();

		XDelete(m_pOccScene);
		XDelete(m_pOccCam);
		XDelete(m_pOccSC);
		XDelete(m_pOccMatrix);
	}

	XSceneNode::destroy();
	return X_OK;
}


XU32 XLight::getPriority()
{
	return m_priority;
}


XStatus XLight::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	m_boundingSphere.center = XVector3D(m_worldTransform.row4);
	m_boundingSphere.radius = m_params.x;

	if (m_pOccScene)
	{
		m_pOccCam->setModelTransform(m_modelTransform);
		m_pOccCam->update(dt,pScene,pMatrixStack);
		(*m_pOccMatrix) = m_pOccCam->getViewMatrix() * m_pOccCam->getProjMatrix();

		XMatrix4x4 biasMat;
		biasMat._11 = .5f;
		biasMat._22 = -.5f;
		biasMat._41 = .5f;
		biasMat._42 = .5f;

		(*m_pOccMatrix) *= biasMat;

	}
	

	return X_OK;
}
//*****************************************************************************
//*****************************************************************************
XStatus XLight::prepForRender(XF32 dt,XScene* pScene)
{
	//Render only if render is allowed
	//if (X_SNP_RENDER & m_propertyFlags)
	//	pScene->addRenderEvent(this,NULL,XDUL::getSphereMaterial(),0);
	X_DUL
	(
		XDUL::drawSpherePoints(pScene,m_boundingSphere.center,m_boundingSphere.radius,XColor(1,1,1,1));
	)

	return X_OK;
}



//*****************************************************************************
/**
Renders the model object.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XLight::render(XRenderEvent* pRenderEvent)
{

	//XDUL::drawSphere(pRenderEvent->pScene,XVector3D(m_worldTransform.row4),m_params.x,XColor(0,0,0,1));
	
	return X_OK;
}

XStatus XLight::enableOcclusion(XBOOL enable)
{
	//X_ASSERTM(m_lightType != X_LT_POINT,"Point lights do not currently support occlusion");

	

	if ((enable) && (!m_pOccScene))
	{
		//Allocate a new occlusion data
		XNew(m_pOccScene,XScene(),XOSMgr::getCameraHeap());
		XNew(m_pOccCam,XCamera(),XOSMgr::getCameraHeap());
		XNew(m_pOccSC,XShadowCasterSSMSC(),XOSMgr::getCameraHeap());
		XNew(m_pOccMatrix,XMatrix4x4(),XOSMgr::getCameraHeap());

		m_pOccCam->init();

		m_pOccSC->init();
		XMaterial* pMaterial = NULL;
		XVP::GraphicsMgr.createMaterial(NULL,m_pOccMat);
		pMaterial = XVP::GraphicsMgr.lockMaterial(m_pOccMat);
		XVP::GraphicsMgr.unlockMaterial(m_pOccMat);
		pMaterial->attachShaderController(m_pOccSC);

		XSceneDesc sceneDesc;
		strcpy(sceneDesc.name,"Occlusion Scene");
		sceneDesc.numUpdateLevels = 0;
		sceneDesc.viewportArrayDesc.numViewports = 1;
		sceneDesc.renderTargetArrayDesc.numRenderTargets = 1;
		sceneDesc.renderTargetArrayDesc.width = 2048;
		sceneDesc.renderTargetArrayDesc.height = 2048;
		sceneDesc.viewportArrayDesc.viewports[0].topLeftX = 0;
		sceneDesc.viewportArrayDesc.viewports[0].topLeftY = 0;
		sceneDesc.viewportArrayDesc.viewports[0].width = 2048;
		sceneDesc.viewportArrayDesc.viewports[0].height = 2048;
		sceneDesc.viewportArrayDesc.enablePostEffects[0] = false;
		sceneDesc.renderTargetArrayDesc.formats[0] = X_DFT_R32_FLOAT;
		sceneDesc.renderTargetArrayDesc.useDepthStencil = true;
		sceneDesc.renderTargetArrayDesc.clearColors[0] = XColor(1,1,1,1);
		sceneDesc.enableResizeEvents = false;

		sceneDesc.numRenderLevels = 1;
		strcpy(sceneDesc.renderLevelDescs[0].name,"DEFAULT");
		sceneDesc.renderLevelDescs[0].sortType = X_ST_NONE;
		sceneDesc.renderLevelDescs[0].maxRenderEvents = 2048;
		sceneDesc.renderLevelDescs[0].autoAlphaManagement = false;

		X_ASSERT(m_pOccScene->init(sceneDesc) == X_OK);

		//Hookup scene 
		m_pOccScene->setMaterialOverride(m_pOccMat);
		m_pOccScene->setViewportCamera(0,m_pOccCam);
	}
	else if ((!enable) && (m_pOccScene))
	{
		//Destroy occlusion data
		m_pOccSC->destroy();
		XVP::GraphicsMgr.destroyMaterial(m_pOccMat);
		m_pOccScene->destroy();
		m_pOccCam->destroy();

		XDelete(m_pOccScene);
		XDelete(m_pOccCam);
		XDelete(m_pOccSC);
		XDelete(m_pOccMatrix);
	}

	return X_OK;
}

XScene* XLight::getOcclusionScene()
{
	return m_pOccScene;
}

XStatus XLight::addOccluder(XSceneNode* pOccluder)
{
	X_ASSERTM(m_pOccScene,"Occlusion has not been enabled for this light");
	X_ASSERT(pOccluder);
	
	return m_pOccScene->addRenderNode(pOccluder);
}

XStatus XLight::removeOccluder(XSceneNode* pOccluder)
{
	//X_ASSERTM(m_pOccScene,"Occlusion has not been enabled for this light");
	X_ASSERT(pOccluder);
	if (m_pOccScene)
		return m_pOccScene->removeRenderNode(pOccluder);
	else
		return X_ERR_NO_SUCH_OBJECT;
}

XBOOL XLight::containsOccluder(XSceneNode* pOccluder)
{
	//X_ASSERTM(m_pOccScene,"Occlusion has not been enabled for this light");
	X_ASSERT(pOccluder);
	
	if (m_pOccScene)
		return m_pOccScene->containsRenderNode(pOccluder);
	else
		return false;
}

XMatrix4x4* XLight::getOcclusionMatrix()
{
	return m_pOccMatrix;
}


XBOOL XLight::isOccludersEnabled()
{
	return m_pOccScene->isEnabled();
}

XStatus XLight::enableOccluders(XBOOL enable)
{
	for (XU32 i = 0;i < m_pOccScene->getNumUpdateLevels();++i)
		m_pOccScene->enableUpdateLevel(i,enable);
	for (XU32 i = 0;i < m_pOccScene->getNumRenderLevels();++i)
		m_pOccScene->enableRenderLevel(i,enable);
	return X_OK;
}

XStatus XLight::setFloatProperty(XS8* pPropertyName,XF32 value)
{
	PB_MAKE_PROPERTY("CutoffDistance",m_params.x = value)
	PB_MAKE_PROPERTY("HotSpotDistance",m_params.y = value)
	PB_MAKE_PROPERTY("AngleCutoff",m_params.z = value)
	PB_MAKE_PROPERTY("AngleDropoff",m_params.w = value)

	return X_ERR_NO_SUCH_OBJECT;
}


XStatus XLight::setUIntProperty(XS8* pPropertyName,XU32 value)
{
	PB_MAKE_PROPERTY("Priority",m_priority = value)

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XLight::setColorProperty(XS8* pPropertyName,XColor& value)
{
	PB_MAKE_PROPERTY("DiffuseColor",m_diffuse = value)
	PB_MAKE_PROPERTY("SpecularColor",m_specular = value)

	return X_ERR_NO_SUCH_OBJECT;
}

XStatus XLight::setEnumProperty(XS8* pPropertyName,XS32 value)
{
	PB_MAKE_PROPERTY("Type",m_lightType = (X_LIGHT_TYPE)value)

	return X_ERR_NO_SUCH_OBJECT;
}