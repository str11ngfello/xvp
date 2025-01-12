#include "xvpsdk.h"
#include "ProxyObject.h"
#include "Camera.h"

namespace XVPManaged
{
	Camera::Camera()
	{
		m_pCam = NULL;
	}

	Camera::~Camera()
	{
	}

	XStatus Camera::init(XScene* pScene)
	{
		XNew(m_pCam,XCamera(),XOSMgr::getDefaultHeap());
		m_pCam->init();
		pScene->addUpdateNode(m_pCam);
		//m_pCam->setDebugScene(pScene);

		m_pScene = pScene;

		return X_OK;
	}

	XCamera* Camera::getCamera()
	{
		return m_pCam;
	}

	XStatus Camera::destroy()
	{
		//Remove from scene
		m_pScene->removeUpdateNode(m_pCam);

		m_pCam->setDebugScene(NULL);

		//Destroy instance
		m_pCam->destroy();
		XDelete(m_pCam);

		return X_OK;
	}

	XStatus Camera::setToViewport(XU32 viewportIndex,XController* pController)
	{
		m_pCam->setID(viewportIndex);
		m_pCam->attachController(pController);
		//m_pCam->setModelPosition(XVector3D(0,0,-150));
		m_pScene->setViewportCamera(viewportIndex,m_pCam);
		
		return X_OK;
	}

	XStatus Camera::removeFromViewport(XController* pController)
	{
		m_pCam->detachController(pController);
		return X_OK;
	}

	XStatus Camera::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pCam->setFloatProperty(pPropertyName,value);
	}

	XStatus Camera::setEnumProperty(XS8* pPropertyName,XS32 value)
	{
		return m_pCam->setEnumProperty(pPropertyName,value);
	}


	HDRPostEffect::HDRPostEffect()
	{
		m_pHDR = NULL;
	}

	HDRPostEffect::~HDRPostEffect()
	{
	}

	XStatus HDRPostEffect::init(XCamera* pCamera)
	{
		XNew(m_pHDR,XHDRPostEffect(),XOSMgr::getDefaultHeap());
			
		m_pHDR->init(pCamera->getViewportWidth(),pCamera->getViewportHeight());
		pCamera->addPostEffect(m_pHDR);

		return X_OK;
	}


	XStatus HDRPostEffect::destroy()
	{
		m_pHDR->destroy();		
		XDelete(m_pHDR);

		return X_OK;
	}

	XStatus HDRPostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pHDR->setFloatProperty(pPropertyName,value);
	}

	XStatus HDRPostEffect::setBoolProperty(XS8* pPropertyName,XBOOL value)
	{
		return m_pHDR->setBoolProperty(pPropertyName,value);
	}


	DOFPostEffect::DOFPostEffect()
	{
		m_pDOF = NULL;
		m_pCamera = NULL;
	}

	DOFPostEffect::~DOFPostEffect()
	{
	}

	XStatus DOFPostEffect::init(XCamera* pCamera)
	{
		m_pCamera = pCamera;
		XNew(m_pDOF,XDOFPostEffect(),XOSMgr::getDefaultHeap());
			
		m_pDOF->init(pCamera->getViewportWidth(),pCamera->getViewportHeight());
		pCamera->addPostEffect(m_pDOF);

		return X_OK;
	}


	XStatus DOFPostEffect::destroy()
	{
		m_pCamera->removePostEffect(m_pDOF);
		m_pDOF->destroy();		
		XDelete(m_pDOF);

		return X_OK;
	}

	XStatus DOFPostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pDOF->setFloatProperty(pPropertyName,value);
	}

	EdgePostEffect::EdgePostEffect()
	{
		m_pEdge = NULL;
		m_pCamera = NULL;
	}

	EdgePostEffect::~EdgePostEffect()
	{
	}

	XStatus EdgePostEffect::init(XCamera* pCamera)
	{
		m_pCamera = pCamera;
		XNew(m_pEdge,XEdgePostEffect(),XOSMgr::getDefaultHeap());
			
		m_pEdge->init(pCamera->getViewportWidth(),pCamera->getViewportHeight());
		pCamera->addPostEffect(m_pEdge);

		return X_OK;
	}


	XStatus EdgePostEffect::destroy()
	{
		m_pCamera->removePostEffect(m_pEdge);
		m_pEdge->destroy();		
		XDelete(m_pEdge);

		return X_OK;
	}

	XStatus EdgePostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pEdge->setFloatProperty(pPropertyName,value);
	}

	DistortionPostEffect::DistortionPostEffect()
	{
		m_pDistort = NULL;
		m_pCamera = NULL;
	}

	DistortionPostEffect::~DistortionPostEffect()
	{
	}

	XStatus DistortionPostEffect::init(XScene* pScene,XCamera* pCamera)
	{
		m_pCamera = pCamera;
		XNew(m_pDistort,XDistortionPostEffect(),XOSMgr::getDefaultHeap());
			
		m_pDistort->init();
		m_pDistort->setSourceRTA(pScene->getRenderTargetArray());
		pCamera->addPostEffect(m_pDistort);

		return X_OK;
	}


	XStatus DistortionPostEffect::destroy()
	{
		m_pCamera->removePostEffect(m_pDistort);
		m_pDistort->destroy();		
		XDelete(m_pDistort);

		return X_OK;
	}

	XStatus DistortionPostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pDistort->setFloatProperty(pPropertyName,value);
	}

	MotionBlurPostEffect::MotionBlurPostEffect()
	{
		m_pMotionBlur = NULL;
		m_pCamera = NULL;
	}

	MotionBlurPostEffect::~MotionBlurPostEffect()
	{
	}

	XStatus MotionBlurPostEffect::init(XCamera* pCamera)
	{
		m_pCamera = pCamera;
		XNew(m_pMotionBlur,XMotionBlurPostEffect(),XOSMgr::getDefaultHeap());
			
		m_pMotionBlur->init();
		pCamera->addPostEffect(m_pMotionBlur);

		return X_OK;
	}


	XStatus MotionBlurPostEffect::destroy()
	{
		m_pCamera->removePostEffect(m_pMotionBlur);
		m_pMotionBlur->destroy();		
		XDelete(m_pMotionBlur);

		return X_OK;
	}

	XStatus MotionBlurPostEffect::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pMotionBlur->setFloatProperty(pPropertyName,value);
	}
}
