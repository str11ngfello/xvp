#include "xvpsdk.h"
#include "ProxyObject.h"
#include "Model.h"


namespace XVPManaged
{
	Model::Model()
	{
		m_pMT = NULL;
		m_pM = NULL;
	}

	Model::~Model()
	{
	}

	XStatus Model::init(XScene* pScene,XS8* pFilename,XU32 numLODs)
	{
		XNew(m_pMT,XModelTemplate(),XOSMgr::getDefaultHeap());
		XNew(m_pM,XModel(),XOSMgr::getDefaultHeap());
		
		m_pMT->init(pFilename,numLODs);
		//m_pMT->generateModel(*m_pM);
		//pScene->addSceneNode(m_pM);

		m_pActorContainerRoot = m_pM;
		m_pScene = pScene;


		return X_OK;
	}

	XStatus Model::postInit()
	{
		m_pMT->generateModel(*m_pM);
		m_pScene->addUpdateNode(m_pM);
		m_pScene->addRenderNode(m_pM);
		
		return X_OK;
	}

	XStatus Model::mergeLOD(XS8* pFilename,XF32 LODDistance)
	{
		m_pMT->mergeLOD(pFilename,LODDistance);
		return X_OK;
	}

	XStatus Model::setLODDist(XU32 LODIndex,XF32 LODDist)
	{
		m_pMT->setLODRange(LODIndex,LODDist);
		return X_OK;
	}

	XMaterial* Model::getMaterial(XS8* pMaterialName)
	{
		XHandle matHandle = m_pMT->getMaterial(pMaterialName);
		X_ASSERTM(matHandle != X_INVALID_HANDLE,"Material could not be found in model LODs");
		return XVP::GraphicsMgr.getMaterialInterface(matHandle);
	}

	XStatus Model::destroy()
	{
		//Remove from scene
		m_pScene->removeUpdateNode(m_pM);
		m_pScene->removeRenderNode(m_pM);

		//Destroy template and instance
		m_pM->destroy();
		m_pMT->destroy();

		XDelete(m_pMT);
		XDelete(m_pM);

		return X_OK;
	}

	XStatus Model::setScene(XScene* pScene)
	{
		m_pScene->removeUpdateNode(m_pM);
		m_pScene->removeRenderNode(m_pM);
		pScene->addUpdateNode(m_pM);
		pScene->addRenderNode(m_pM);
		m_pScene = pScene;

		return X_OK;
	}
	
	void Model::attachController(XController* pController,XS8* pNodeName)
	{
		m_pM->attachController(pNodeName,pController,false);
	}

	void Model::detachController(XController* pController,XS8* pNodeName)
	{
		m_pM->detachController(pNodeName,pController,false);
	}

	
	XModel* Model::getModel()
	{
		return m_pM;
	}

	void Model::generateLightmap(XU32 numModels,XModel** ppModels,XU32 numLights,XLight** ppLights,XS8* pLMFilename,XU32 maxTexSize,XU32 pixelsPerSurfAreaUnit,XU32 forcedTexSize,XF32 blurSize)
	{
		XU32 texSize = 32;
		XF32 surfaceArea = 0.0f;
		if (forcedTexSize == 0)
		{
			//Guess an appropriate sized texture based on the surface area of the
			//mesh(s) to be light mapped
			surfaceArea = m_pM->getSurfaceArea();
			XF32 resolution = surfaceArea;
			while (texSize * texSize < resolution)
			{
				if (texSize >= maxTexSize)
				{
					texSize = maxTexSize;
					break;
				}

				texSize *= 2;
			}

			//Now bump the resolution up by the quality count but still check for the 
			//max
			for (XU32 i = 0;i < pixelsPerSurfAreaUnit;++i)
			{
				if (texSize >= maxTexSize)
				{
					texSize = maxTexSize;
					break;
				}

				texSize *= 2;
			}

		}
		else
			texSize = forcedTexSize;
		m_pM->generateLightmap(numModels,ppModels,numLights,ppLights,pLMFilename,texSize,blurSize);
	}

	void Model::generateAOMap(XU32 numModels,XModel** ppModels,XU32 numRays,XVector3D* pRays,XS8* pAOMapFilename,XU32 maxTexSize,XU32 pixelsPerSurfAreaUnit,XU32 forcedTexSize,XF32 blurSize)
	{
		XU32 texSize = 32;
		XF32 surfaceArea = 0.0f;
		if (forcedTexSize == 0)
		{
			//Guess an appropriate sized texture based on the surface area of the
			//mesh(s) to be light mapped
			surfaceArea = m_pM->getSurfaceArea();
			XF32 resolution = surfaceArea;
			while (texSize * texSize < resolution)
			{
				if (texSize >= maxTexSize)
				{
					texSize = maxTexSize;
					break;
				}

				texSize *= 2;
			}

			//Now bump the resolution up by the quality count but still check for the 
			//max
			for (XU32 i = 0;i < pixelsPerSurfAreaUnit;++i)
			{
				if (texSize >= maxTexSize)
				{
					texSize = maxTexSize;
					break;
				}

				texSize *= 2;
			}

		}
		else
			texSize = forcedTexSize;

		m_pM->generateAOMap(numModels,ppModels,numRays,pRays,pAOMapFilename,texSize,blurSize);
	}
}
