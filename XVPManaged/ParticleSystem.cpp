#include "xvpsdk.h"
#include "ProxyObject.h"
#include "ParticleSystem.h"


namespace XVPManaged
{
	ParticleSystem::ParticleSystem()
	{
		m_pPST = NULL;
		m_pPS = NULL;
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	XStatus ParticleSystem::init(XScene* pScene,XU32 numEmitters)
	{
		/*XShaderController* pSC;

		XNew(m_pPST,XParticleSystemTemplate(),XOSMgr::getDefaultHeap());
		XNew(m_pPS,XParticleSystem(),XOSMgr::getDefaultHeap());
		
		pSC = XVP::ShaderFactory.createObject("Particle System.fx");

		XS8 pMem[15000];
		XS8* p = pMem;
		FILE* f = fopen("c:/Particle System.xps","rb");
		 
		// obtain file size.
		fseek (f , 0 , SEEK_END);
		int size = ftell (f);
		rewind (f);
		fread(pMem,size,1,f);
		m_pPST->init(p);
		fclose(f);

		pSC->init();

		m_pPST->generateParticleSystem(*m_pPS);
		m_pPS->attachShaderController(NULL,pSC);
		pScene->addSceneNode(m_pPS);
		for (int x = 0;x < m_pPS->getNumEmitters();++x)
		{
			m_pPS->getEmitter(x)->setBirthRate(.0001f);
			m_pPS->getEmitter(x)->setParticleLife(3);
			m_pPS->getEmitter(x)->setParticleLifeVariation(1);
		}*/

		

		XNew(m_pPST,XParticleSystemTemplate(),XOSMgr::getDefaultHeap());
		XNew(m_pPS,XParticleSystem(),XOSMgr::getDefaultHeap());
		
		m_pPST->init(numEmitters);
		m_pPST->generateParticleSystem(*m_pPS);
		//m_pPS->attachShaderController(NULL,pSC);
		pScene->addUpdateNode(m_pPS);
		pScene->addRenderNode(m_pPS,3);

		/*for (int x = 0;x < m_pPS->getNumEmitters();++x)
		{
			m_pPS->getEmitter(x)->setBirthRate(.0001f);
			m_pPS->getEmitter(x)->setParticleLife(3);
			//m_pPS->getEmitter(x)->setParticleLifeVariation(1);
			m_pPS->getEmitter(x)->setParticleSize(2);
		}*/

		m_pActorContainerRoot = m_pPS;
		m_pScene = pScene;

		return X_OK;
	}

	XStatus ParticleSystem::destroy()
	{
		//Remove from scene
		m_pScene->removeUpdateNode(m_pPS);
		m_pScene->removeRenderNode(m_pPS);

		//Destroy template and instance
		m_pPS->destroy();
		m_pPST->destroy();

		XDelete(m_pPST);
		XDelete(m_pPS);

		return X_OK;
	}

	XParticleEmitter* ParticleSystem::getEmitter(XU32 index)
	{
		//return (XParticleEmitter*)m_pPS->getNode(pEmitterName);
		
		//HACK!!!!!!!!!!!!!!
		return m_pPS->getEmitter(index);
		/*XU32 numEmitters = m_pPS->getNumEmitters();
		destroy();
		init(m_pScene,numEmitters+1);
		return m_pPS->getEmitter(numEmitters);*/
	}

	/*void ParticleSystem::setActorContainer(ActorContainer* pActorContainer)
	{
		m_pPS->setActorContainer(pActorContainer->getActorContainer());
	}*/


	Emitter::Emitter()
	{
		m_pEmitter = NULL;
	}

	Emitter::~Emitter()
	{
	}

	XStatus Emitter::init(XParticleEmitter* pEmitter)
	{
		m_pEmitter = pEmitter;
		return X_OK;
	}

	/*XStatus Emitter::destroy()
	{
		return X_OK;
	}*/

	XMaterial* Emitter::createMaterial()
	{
		return XVP::GraphicsMgr.getMaterialInterface(m_pEmitter->getMaterialHandle());
	}

	
	void ParticleSystem::start()
	{
		m_pPS->start();
	}
	void ParticleSystem::stop()
	{
		m_pPS->stop();
	}
	void ParticleSystem::restart()
	{
		m_pPS->restart();
	}

	void ParticleSystem::addEmitter()
	{
		m_pPS->addEmitter(NULL,"Emitter");
	}

	XStatus Emitter::setBoolProperty(XS8* pPropertyName,XBOOL value)
	{
		return m_pEmitter->setBoolProperty(pPropertyName,value);
	}

	XStatus Emitter::setFloatProperty(XS8* pPropertyName,XF32 value)
	{
		return m_pEmitter->setFloatProperty(pPropertyName,value);
	}

	XStatus Emitter::setUIntProperty(XS8* pPropertyName,XU32 value)
	{
		return m_pEmitter->setUIntProperty(pPropertyName,value);
	}

	XStatus Emitter::setColorProperty(XS8* pPropertyName,XColor& value)
	{
		return m_pEmitter->setColorProperty(pPropertyName,value);
	}	

	XStatus Emitter::setEnumProperty(XS8* pPropertyName,XS32 value)
	{
		return m_pEmitter->setEnumProperty(pPropertyName,value);
	}		

	XStatus Emitter::setVector3DProperty(XS8* pPropertyName,XVector3D& value)
	{
		return m_pEmitter->setVector3DProperty(pPropertyName,value);
	}		
}
