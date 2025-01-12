
#pragma once

using namespace System;
#include "XPhysXController.h"

namespace XVPManaged
{
	public class PhysXController : public BaseController
	{
	protected:
		XPhysXController* m_pPhysX;

	public:
		PhysXController();
		~PhysXController();
		XStatus init(XScene* pScene);
		XStatus destroy();
		XController* getController();
		void addForce(XVector3D& forceVec);
	};
}