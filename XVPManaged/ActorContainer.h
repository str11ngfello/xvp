#pragma once

using namespace System;

namespace XVPManaged
{
	public class ActorContainer : public ProxyObject
	{
	protected:
		XActorContainer* m_pActorContainer;

	public:
		ActorContainer();
		~ActorContainer();
		XStatus init(XScene* pScene);
		XActorContainer* getActorContainer();
		XStatus destroy();
	};
}