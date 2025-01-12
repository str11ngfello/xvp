
#pragma once

using namespace System;

namespace XVPManaged
{
	public class Light : public ProxyObject
	{
	protected:
		XLight* m_pLight;
		XHandle m_samplerStateOverride;

	public:
		Light();
		~Light();
		XStatus init(XScene* pScene);
		XLight* getLight();
		XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XStatus setColorProperty(XS8* pPropertyName,XColor& color);
		XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
	};
}