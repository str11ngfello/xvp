
#include "ProxyObject.h"



namespace XVPManaged
{
	ProxyObject::ProxyObject()
	{
		m_pActorContainerRoot = NULL;
		m_pScene = NULL;
	}

	ProxyObject::~ProxyObject()
	{
	}

	XStatus ProxyObject::destroy()
	{
		return X_OK;
	}

	XStatus ProxyObject::setScene(XScene* pScene)
	{
		return X_OK;
	}

	XStatus ProxyObject::setActorContainer(void* pActorContainer)
	{
		m_pActorContainerRoot->setActorContainer(pActorContainer);
		return X_OK;
	}

	XStatus ProxyObject::setIntProperty(XS8* pPropertyName,XS32 value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setUIntProperty(XS8* pPropertyName,XU32 value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setDoubleProperty(XS8* pPropertyName,XF64 value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setBoolProperty(XS8* pPropertyName,XBOOL value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setColorProperty(XS8* pPropertyName,XColor& color)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setEnumProperty(XS8* pPropertyName,XS32 value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setVector2DProperty(XS8* pPropertyName,XVector2D& value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setVector3DProperty(XS8* pPropertyName,XVector3D& value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

	XStatus ProxyObject::setVector4DProperty(XS8* pPropertyName,XVector4D& value)
	{
		return X_ERR_NO_SUCH_OBJECT;
	}

}
