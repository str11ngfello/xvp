#include "xvpsdk.h"
#include "ProxyObject.h"
#include "MaterialNP.h"



namespace XVPManaged
{
	MaterialNP::MaterialNP()
	{
		for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
			m_texHandles[i] = X_INVALID_HANDLE;
		m_pShaderController = NULL;
		m_pMaterial = NULL;
	}

	MaterialNP::~MaterialNP()
	{

	}

	XStatus MaterialNP::init(XMaterial* pMaterial)
	{
		m_pMaterial = pMaterial;

		m_pShaderController = XVP::ShaderFactory.createObject("White.fx");
		m_pShaderController->init();
		m_pMaterial->attachShaderController(m_pShaderController);

		return X_OK;
	}

	XStatus MaterialNP::destroy()
	{
		for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		{
			if (m_texHandles[i] != X_INVALID_HANDLE)
				XVP::GraphicsMgr.destroyTexture(m_texHandles[i]);
			m_texHandles[i] = X_INVALID_HANDLE;
		}

		if (m_pShaderController)
		{
			m_pShaderController->destroy();
			XDelete(m_pShaderController);
		}

		return X_OK;
	}

	XStatus MaterialNP::setShaderName(XS8* pShaderName)
	{
		XShaderController* pNewSC = NULL;
		
		//Let's determine if this is a shader template by looking for a '#'.  For example,
		//Eyebal.fx#Model-BlinnPhong.fx
		XS8* pDerivedName = pShaderName;
		XS8* pTemplateName = pShaderName;
		if (strstr(pShaderName,"#"))
		{
			XS8* nextLineToken = NULL;
			XS8* pDerivedName = strtok_s(pShaderName,"#",&pTemplateName);
		}

		//Check to make sure the object has been registered.  If not,
		//we need to replace to default to a known shader.  
		if (!XVP::ShaderFactory.isObjectRegistered(pTemplateName))
		{
			pNewSC = XVP::ShaderFactory.createObject("White.fx");
			char buf[512];
			sprintf(buf,"A material references an unknown shader named \"%s\".  The shader will be reset to default.  Save the file to avoid this message in the future.",pTemplateName);
			MessageBox(NULL,buf,"Unknown Shader Reference",MB_OK);
		}
		else
			pNewSC = XVP::ShaderFactory.createObject(pTemplateName);

		pNewSC->init(pDerivedName);
		m_pMaterial->attachShaderController(pNewSC);
		
		for (XU32 i = 0;i < X_MAX_SAMPLERS;++i)
		{
			if (m_texHandles[i] != X_INVALID_HANDLE)
				XVP::GraphicsMgr.destroyTexture(m_texHandles[i]);
			m_texHandles[i] = X_INVALID_HANDLE;
		}
		
		//Now that we attacheded a new shader controller, we can delete the old one
		//and store a the new one
		XDelete(m_pShaderController);
		m_pShaderController = pNewSC;
		

		return X_OK;
	}

	XStatus MaterialNP::setTexture(XU32 samplerIndex,XS8* pFilename)
	{
		//Destroy current texture if one exists at this sampler index
		if (m_texHandles[samplerIndex] != X_INVALID_HANDLE)
		{
			m_pMaterial->setTextureHandle(samplerIndex,NULL);
			XVP::GraphicsMgr.destroyTexture(m_texHandles[samplerIndex]);
		}

		//Create texture and assign to material.  
		if (pFilename && pFilename[0] != '\0')
		{
			XVP::GraphicsMgr.createTextureFromFile(pFilename,m_texHandles[samplerIndex],true);
			m_pMaterial->setTextureHandle(samplerIndex,m_texHandles[samplerIndex]);
		}
		else
			m_pMaterial->setTextureHandle(samplerIndex,X_INVALID_HANDLE);
		
		return X_OK;
	}

	XStatus MaterialNP::setSamplerAddressU(XU32 samplerIndex,XS32 addressType)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerAddressU(samplerIndex,(X_TEXTURE_ADDRESS_TYPE)addressType);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerAddressV(XU32 samplerIndex,XS32 addressType)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerAddressV(samplerIndex,(X_TEXTURE_ADDRESS_TYPE)addressType);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerAddressW(XU32 samplerIndex,XS32 addressType)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerAddressW(samplerIndex,(X_TEXTURE_ADDRESS_TYPE)addressType);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerMipLODBias(XU32 samplerIndex,XF32 bias)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerMipLODBias(samplerIndex,bias);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerMaxAnisotropy(XU32 samplerIndex,XU32 maxAnisotropy)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerMaxAnisotropy(samplerIndex,maxAnisotropy);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerMinLOD(XU32 samplerIndex,XF32 minLOD)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerMinLOD(samplerIndex,minLOD);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerMaxLOD(XU32 samplerIndex,XF32 maxLOD)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerMaxLOD(samplerIndex,maxLOD);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerBorderColor(XU32 samplerIndex,XF32 r,XF32 g,XF32 b,XF32 a)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerBorderColor(samplerIndex,XColor(r,g,b,a));
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerFilter(XU32 samplerIndex,XS32 filterType)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerFilter(samplerIndex,(X_TEXTURE_FILTER_TYPE)filterType);
		return m_pMaterial->unlock();
	}

	XStatus MaterialNP::setSamplerFunc(XU32 samplerIndex,XS32 comparisonFunc)
	{
		m_pMaterial->lock();
		m_pMaterial->setSamplerFunc(samplerIndex,(X_CMP_FUNC_TYPE)comparisonFunc);
		return m_pMaterial->unlock();
	}
		

	/*XStatus MaterialNP::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pMaterial->setFloatProperty(pPropertyName,value);
	}

	XStatus MaterialNP::setUIntProperty(XS8* pPropertyName,XU32 value)
	{
		return m_pMaterial->setUIntProperty(pPropertyName,value);
	}

	XStatus MaterialNP::setColorProperty(XS8* pPropertyName,XColor& value)
	{
		return m_pMaterial->setColorProperty(pPropertyName,value);
	}	*/

	XStatus MaterialNP::setEnumProperty(XS8* pPropertyName,XS32 value)
	{
		m_pMaterial->getShaderController()->setEnumProperty(pPropertyName,value);
		m_pMaterial->setEnumProperty(pPropertyName,value);
		return X_OK;
	}	

	XStatus MaterialNP::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		m_pMaterial->getShaderController()->setFloatProperty(pPropertyName,value);
		m_pMaterial->setFloatProperty(pPropertyName,value);
		return X_OK;
	}		

	XStatus MaterialNP::setDoubleProperty(XS8* pPropertyName,XF64 value)
	{
		m_pMaterial->getShaderController()->setDoubleProperty(pPropertyName,value);
		m_pMaterial->setDoubleProperty(pPropertyName,value);
		return X_OK;
	}	
	
	XStatus MaterialNP::setIntProperty(XS8* pPropertyName,XS32 value)
	{
		m_pMaterial->getShaderController()->setIntProperty(pPropertyName,value);
		m_pMaterial->setIntProperty(pPropertyName,value);
		return X_OK;
	}		

	XStatus MaterialNP::setUIntProperty(XS8* pPropertyName,XU32 value)
	{
		m_pMaterial->getShaderController()->setUIntProperty(pPropertyName,value);
		m_pMaterial->setUIntProperty(pPropertyName,value);
		return X_OK;
	}
	
	XStatus MaterialNP::setBoolProperty(XS8* pPropertyName,XBOOL value)
	{
		m_pMaterial->getShaderController()->setBoolProperty(pPropertyName,value);
		m_pMaterial->setBoolProperty(pPropertyName,value);
		return X_OK;
	}		

	XStatus MaterialNP::setColorProperty(XS8* pPropertyName,XColor& color)
	{
		m_pMaterial->getShaderController()->setColorProperty(pPropertyName,color);
		m_pMaterial->setColorProperty(pPropertyName,color);
		return X_OK;
	}

	XStatus MaterialNP::setVector2DProperty(XS8* pPropertyName,XVector2D& value)
	{
		m_pMaterial->getShaderController()->setVector2DProperty(pPropertyName,value);
		m_pMaterial->setVector2DProperty(pPropertyName,value);
		return X_OK;
	}

	XStatus MaterialNP::setVector3DProperty(XS8* pPropertyName,XVector3D& value)
	{
		m_pMaterial->getShaderController()->setVector3DProperty(pPropertyName,value);
		m_pMaterial->setVector3DProperty(pPropertyName,value);
		return X_OK;
	}

	XStatus MaterialNP::setVector4DProperty(XS8* pPropertyName,XVector4D& value)
	{
		m_pMaterial->getShaderController()->setVector4DProperty(pPropertyName,value);
		m_pMaterial->setVector4DProperty(pPropertyName,value);
		return X_OK;
	}
}
