#include "xvpsdk.h"
#include "ProxyObject.h"
#include "Light.h"

namespace XVPManaged
{
	Light::Light()
	{
		m_pLight = NULL;
		m_samplerStateOverride = X_INVALID_HANDLE;
	}

	Light::~Light()
	{
	}

	XStatus Light::init(XScene* pScene)
	{
		XNew(m_pLight,XLight(),XOSMgr::getDefaultHeap());
		XLightDesc desc;
		desc.enableOcclusion = true;
		m_pLight->init(desc);

		pScene->addUpdateNode(m_pLight);
		pScene->addRenderNode(m_pLight);
		m_pScene = pScene;

		XSamplerDesc samplerDesc;
		samplerDesc.addressU = X_TAT_CLAMP;
		samplerDesc.addressW = X_TAT_CLAMP;
		samplerDesc.addressV = X_TAT_CLAMP;
		samplerDesc.filter = X_TFT_MIN_MAG_MIP_POINT;
		XVP::GraphicsMgr.createSamplerState(samplerDesc,m_samplerStateOverride);
		pScene->setTextureOverrideFromRTA(12,m_samplerStateOverride,m_pLight->getOcclusionScene()->getRenderTargetArray(),0);

		m_pActorContainerRoot = m_pLight;

		return X_OK;
	}

	XLight* Light::getLight()
	{
		return m_pLight;
	}

	XStatus Light::destroy()
	{
		//Remove from scene
		m_pScene->removeUpdateNode(m_pLight);
		m_pScene->removeRenderNode(m_pLight);
		m_pScene->removeTextureOverride(12);
		XVP::GraphicsMgr.destroySamplerState(m_samplerStateOverride);

		//Destroy template and instance
		m_pLight->destroy();

		XDelete(m_pLight);
		
		return X_OK;
	}

	XStatus Light::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pLight->setFloatProperty(pPropertyName,value);
	}

	XStatus Light::setColorProperty(XS8* pPropertyName,XColor& color)
	{
		return m_pLight->setColorProperty(pPropertyName,color);
	}
	
	XStatus Light::setEnumProperty(XS8* pPropertyName,XS32 value)
	{
		return m_pLight->setEnumProperty(pPropertyName,value);
	}
}
