
#pragma once

#include "xvpsdk.h"

using namespace System;

namespace XVPManaged
{
	public class ProxyObject
	{
	protected:
		XSceneNode* m_pActorContainerRoot;
		XScene* m_pScene;

	public:
		ProxyObject();
		virtual ~ProxyObject();
		virtual XStatus destroy();

		virtual XStatus setScene(XScene* pScene);
		virtual XStatus setActorContainer(void* pActorContainer);
		virtual XStatus setIntProperty(XS8* pPropertyName,XS32 value);
		virtual XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
		virtual XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		virtual XStatus setDoubleProperty(XS8* pPropertyName,XF64 value);
		virtual XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
		virtual XStatus setColorProperty(XS8* pPropertyName,XColor& color);
		virtual XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
		virtual XStatus setVector2DProperty(XS8* pPropertyName,XVector2D& value);
		virtual XStatus setVector3DProperty(XS8* pPropertyName,XVector3D& value);
		virtual XStatus setVector4DProperty(XS8* pPropertyName,XVector4D& value);
	};

	public class BaseController : public ProxyObject
	{
	public:
		virtual XController* getController() = 0;
	};

}