
#pragma once

using namespace System;
#include "ActorContainer.h"

namespace XVPManaged
{
	public class Model : public ProxyObject
	{
	protected:
		XModelTemplate* m_pMT;
		XModel* m_pM;

	public:
		Model();
		~Model();
		XStatus init(XScene* pScene,XS8* pFilename,XU32 numLODs);
		XStatus postInit();
		XStatus mergeLOD(XS8* pFilename,XF32 LODDistance);
		XStatus setLODDist(XU32 LODIndex,XF32 LODDist);
		XMaterial* getMaterial(XS8* pMaterialName);
		XStatus destroy();
		XStatus setScene(XScene* pScene);
		void attachController(XController* pController,XS8* pNodeName);
		void detachController(XController* pController,XS8* pNodeName);
		XModel* getModel();
		void generateLightmap(XU32 numModels,XModel** ppModels,XU32 numLights,XLight** ppLights,XS8* pLMFilename,XU32 maxTexSize,XU32 pixelsPerSurfAreaUnit,XU32 forcedSize,XF32 blurSize);
		void generateAOMap(XU32 numModels,XModel** ppModels,XU32 numRays,XVector3D* pRays,XS8* pAOMapFilename,XU32 maxTexSize,XU32 pixelsPerSurfAreaUnit,XU32 forcedTexSize,XF32 blurSize);
	};

	/*public class Emitter : public ProxyObject
	{
	protected:
		XParticleEmitter* m_pEmitter;

	public:
		Emitter();
		~Emitter();
		XStatus init(XParticleEmitter* pEmitter);
		XMaterial* createMaterial();
		//XStatus destroy();
		XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
		XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
		XStatus setColorProperty(XS8* pPropertyName,XColor& value);
		XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
		//void setBirthRate(XF32 birthRate);
	};*/

}