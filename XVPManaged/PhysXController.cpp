#include "xvpsdk.h"
#include "ProxyObject.h"
#include "PhysXController.h"

namespace XVPManaged
{
	PhysXController::PhysXController()
	{
		m_pPhysX = NULL;
	}

	PhysXController::~PhysXController()
	{
	}

	XStatus PhysXController::init(XScene* pScene)
	{
		XNew(m_pPhysX,XPhysXController(),XOSMgr::getDefaultHeap());
		m_pPhysX->init();
		m_pScene = pScene;

		return X_OK;
	}

	XStatus PhysXController::destroy()
	{
		m_pPhysX->destroy();
		XDelete(m_pPhysX);

		return X_OK;
	}

	XController* PhysXController::getController()
	{
		return m_pPhysX;
	}

	void PhysXController::addForce(XVector3D& forceVec)
	{
		m_pPhysX->addForce(forceVec);
	}
}
