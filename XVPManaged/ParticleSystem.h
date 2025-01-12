
#pragma once

using namespace System;
#include "ActorContainer.h"

namespace XVPManaged
{
	public class ParticleSystem : public ProxyObject
	{
	protected:
		XParticleSystemTemplate* m_pPST;
		XParticleSystem* m_pPS;

	public:
		ParticleSystem();
		~ParticleSystem();
		XStatus init(XScene* pScene,XU32 numEmitters);
		XStatus destroy();
		XParticleEmitter* getEmitter(XU32 index);
		void start();
		void stop();
		void restart();
		void addEmitter();
		//void setActorContainer(ActorContainer* pActorContainer);
	};

	public class Emitter : public ProxyObject
	{
	protected:
		XParticleEmitter* m_pEmitter;

	public:
		Emitter();
		~Emitter();
		XStatus init(XParticleEmitter* pEmitter);
		XMaterial* createMaterial();
		//XStatus destroy();
		XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
		XStatus setColorProperty(XS8* pPropertyName,XColor& value);
		XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
		XStatus setVector3DProperty(XS8* pPropertyName,XVector3D& value);

		//void setBirthRate(XF32 birthRate);
	};

}