
#pragma once

using namespace System;

namespace XVPManaged
{
	public class Camera : public ProxyObject
	{
	protected:
		XCamera* m_pCam;

	public:
		Camera();
		~Camera();
		XStatus init(XScene* pScene);
		XCamera* getCamera();
		XStatus destroy();
		XStatus setToViewport(XU32 viewportIndex,XController* pController);
		XStatus removeFromViewport(XController* pController);
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XStatus setEnumProperty(XS8* pPropertyName,XS32 value);

	};

	public class BasePostEffect : public ProxyObject
	{
	public:
		virtual XPostEffect* getPostEffect() = 0;
	};

	public class HDRPostEffect : public BasePostEffect
	{
	protected:
		XHDRPostEffect* m_pHDR;

	public:
		HDRPostEffect();
		~HDRPostEffect();
		XStatus init(XCamera* pCamera);
		XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
		XPostEffect* getPostEffect() { return m_pHDR; }
	};

	public class DOFPostEffect : public BasePostEffect
	{
	protected:
		XDOFPostEffect* m_pDOF;
		XCamera* m_pCamera;

	public:
		DOFPostEffect();
		~DOFPostEffect();
		XStatus init(XCamera* pCamera);
		XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XPostEffect* getPostEffect() { return m_pDOF; }
	};

	public class EdgePostEffect : public BasePostEffect
	{
	protected:
		XEdgePostEffect* m_pEdge;
		XCamera* m_pCamera;

	public:
		EdgePostEffect();
		~EdgePostEffect();
		XStatus init(XCamera* pCamera);
		XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XPostEffect* getPostEffect() { return m_pEdge; }
	};

	public class DistortionPostEffect : public BasePostEffect
	{
	protected:
		XDistortionPostEffect* m_pDistort;
		XCamera* m_pCamera;

	public:
		DistortionPostEffect();
		~DistortionPostEffect();
		XStatus init(XScene* pScene,XCamera* pCamera);
		XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XPostEffect* getPostEffect() { return m_pDistort; }
	};

	public class MotionBlurPostEffect : public BasePostEffect
	{
	protected:
		XMotionBlurPostEffect* m_pMotionBlur;
		XCamera* m_pCamera;

	public:
		MotionBlurPostEffect();
		~MotionBlurPostEffect();
		XStatus init(XCamera* pCamera);
		XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XPostEffect* getPostEffect() { return m_pMotionBlur; }
	};
}