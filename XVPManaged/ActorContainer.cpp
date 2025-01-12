#include "xvpsdk.h"
#include "ProxyObject.h"
#include "ActorContainer.h"

namespace XVPManaged
{
	ActorContainer::ActorContainer()
	{
		m_pActorContainer = NULL;
	}

	ActorContainer::~ActorContainer()
	{
	}

	XStatus ActorContainer::init(XScene* pScene)
	{
		XNew(m_pActorContainer,XActorContainer(),XOSMgr::getDefaultHeap());
		pScene->addUpdateNode(m_pActorContainer);
		pScene->addRenderNode(m_pActorContainer);
		m_pScene = pScene;

		return X_OK;
	}

	XActorContainer* ActorContainer::getActorContainer()
	{
		return m_pActorContainer;
	}

	XStatus ActorContainer::destroy()
	{
		//Remove from scene
		m_pScene->removeUpdateNode(m_pActorContainer);
		m_pScene->removeRenderNode(m_pActorContainer);

		XDelete(m_pActorContainer);
		
		return X_OK;
	}

}