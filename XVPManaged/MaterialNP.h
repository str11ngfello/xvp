
#pragma once

using namespace System;

namespace XVPManaged
{
	public class MaterialNP : public ProxyObject
	{
	protected:
		XHandle m_texHandles[X_MAX_SAMPLERS];
		XShaderController* m_pShaderController;
		XMaterial* m_pMaterial;

	public:
		MaterialNP();
		~MaterialNP();
		XStatus init(XMaterial* pMaterial);
		XStatus destroy();
		XStatus setShaderName(XS8* pShaderName);
		XStatus setTexture(XU32 samplerIndex,XS8* pFilename);
		XStatus setSamplerAddressU(XU32 samplerIndex,XS32 addressType);
		XStatus setSamplerAddressV(XU32 samplerIndex,XS32 addressType);
		XStatus setSamplerAddressW(XU32 samplerIndex,XS32 addressType);
		XStatus setSamplerMipLODBias(XU32 samplerIndex,XF32 bias);
		XStatus setSamplerMaxAnisotropy(XU32 samplerIndex,XU32 maxAnisotropy);
		XStatus setSamplerMinLOD(XU32 samplerIndex,XF32 minLOD);
		XStatus setSamplerMaxLOD(XU32 samplerIndex,XF32 maxLOD);
		XStatus setSamplerBorderColor(XU32 samplerIndex,XF32 r,XF32 g,XF32 b,XF32 a);
		XStatus setSamplerFilter(XU32 samplerIndex,XS32 filterType);
		XStatus setSamplerFunc(XU32 samplerIndex,XS32 comparisonFunc);
		
		XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XStatus setDoubleProperty(XS8* pPropertyName,XF64 value);
		XStatus setIntProperty(XS8* pPropertyName,XS32 value);
		XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
		XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
		XStatus setColorProperty(XS8* pPropertyName,XColor& color);
		XStatus setVector2DProperty(XS8* pPropertyName,XVector2D& value);
		XStatus setVector3DProperty(XS8* pPropertyName,XVector3D& value);
		XStatus setVector4DProperty(XS8* pPropertyName,XVector4D& value);
	};

}