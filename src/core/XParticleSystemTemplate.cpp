//*****************************************************************************
//*****************************************************************************
//     _  __         _    ______ 
//    | |/ /        | |  / / __ \
//    |   / ______  | | / / /_/ /
//   /   | /_____/  | |/ / ____/ 
//  /_/|_|          |___/_/       
//
//	Extensible Visualization Platform (X-VP) (c) 
//	Copyright 2000. All Rights Reserved.
// 
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//	Module Name:	XParticleSystemTemplate.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-2-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the particle system template.  
*/
//*****************************************************************************
XParticleSystemTemplate::XParticleSystemTemplate()
{
	m_refCount = 0;
	m_numNodes = 0;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the particle system template.
*/
//*****************************************************************************
XParticleSystemTemplate::~XParticleSystemTemplate()
{
}



//*****************************************************************************
/**
Destroys the particle system template by freeing all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XParticleSystemTemplate::destroy()
{
	if (!m_inited)
		return X_OK;

	//Destroy the objects
	for (XU32 i = 0;i < m_nodeArray.size();i++)
	{
		m_nodeArray[i].pNode->destroy();
		XDelete(m_nodeArray[i].pNode)
	}
	m_nodeTree.destroy();
	m_nodeArray.~vector();

	//for (XU32 i = 0;i < m_parentChildList.size();i++)
	//	m_parentChildList[i].destroy();
	//m_parentChildList.clear();

	m_defaultPose.destroy();
	m_inited = false;
	return X_OK;
}



//*****************************************************************************
/**
Initializes the particle system template with a .xps file.  This function can 
be called more than once.  Each time it is called it flushes any previous
template data so care should be taken to ensure that no particle system
instances are using the data before initializing it with new data.

@param	pFilename	The path to the .xps file to load.  This file must end
					with a .xps extension or this function will fail.
@return	Returns a status code of type XStatus.
*/
//*****************************************************************************
/*XStatus XParticleSystemTemplate::init(XS8* pFilename)
{
	//Initialize the particle system template
	destroy();
	XParticleSystemTemplate();

	XModelLoader loader;
	
	//This reference count only goes up when the user creates
	//a particle system instance that references this template.
	m_refCount = 0;

	//Now, attempt to load the file passed in.
	if (loader.load(pFilename) != X_OK)
		return X_ERR_OPERATION_FAILED;
		
	//Make sure that there is at least one material.  Currently the particle system
	//will use the material in the first slot.
	if ((loader.m_model.materialList.size() < 1))
		return X_ERR_OPERATION_FAILED;

	//Get pointers to the first material and particle system
	XmfMaterial* pMaterial = &loader.m_model.materialList[0];
	XmfParticleSystem* pParticleSystem = &loader.m_model.particleSystem;

	XParentChildCombo pcCombo;
	m_parentChildList.push_back(pcCombo);
	m_parentChildList[0].setChildName(pParticleSystem->name);
	
	//Okay, at this point the particle system data has been loaded from the file,
	//so it's time to chop it up.  	
	m_emitterType = (X_EMITTER_TYPE)pParticleSystem->emitterType;
	m_emitterLength = pParticleSystem->emitterLength;
	m_emitterWidth = pParticleSystem->emitterWidth;
	m_emitterHeight = pParticleSystem->emitterHeight;
	m_birthRate = pParticleSystem->birthRate;
	m_startAlpha = pParticleSystem->startAlpha;
	m_endAlpha = pParticleSystem->endAlpha;
	m_startColor = pParticleSystem->startColor;
	m_endColor = pParticleSystem->endColor;	
	m_particleGrowTime = pParticleSystem->particleGrowTime;

	//If the particle shrink time is 1 then we need to shrink from the
	//beginning of the particle's life.  So we adjust the start time
	//to a non-zero value;
	if (pParticleSystem->particleShrinkTime == 1)
		m_particleShrinkTime = .00001f;
	else
		m_particleShrinkTime = 1 - pParticleSystem->particleShrinkTime;

	m_particleSpeed = pParticleSystem->particleSpeed;
	m_particleSpeedVariation = pParticleSystem->particleSpeedVariation;
	m_particleLife = pParticleSystem->particleLife;
	m_particleLifeVariation = pParticleSystem->particleLifeVariation;
	m_particleSize = pParticleSystem->particleSize;
	m_particleSizeVariation = pParticleSystem->particleSizeVariation;
	m_directionVec = pParticleSystem->directionVec;
	m_directionVecVariation = pParticleSystem->directionVecVariation;
	
	//Copy over the inital model transform matrix but leave the inverse
	//as the identity matrix.  Particle Systems are different than geometry because
	//they begin transformation from the world identity position whereas
	//geometry could have vertices with positions anywhere in world space.
	
	
	//pNewEmitter->setModelTransform(pParticleSystem->modelTransform);
	m_defaultPose.init(1);
	m_defaultPose.addTransform(pParticleSystem->modelTransform);

	//Create the render state for the particle system
	//XHandle matHandle = X_INVALID_HANDLE;
	XHandle texHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.createMaterial(NULL,m_matHandle);
	XMaterial* pRs = XVP::GraphicsMgr.getMaterialInterface(m_matHandle);
	if  ((pMaterial->texFilename != NULL) && (strcmp(pMaterial->texFilename,"") != 0))	
	{
		if (XVP::GraphicsMgr.createTexture(pMaterial->texFilename,X_TT_2D,texHandle) == X_OK)
		{
			pRs->setTextureHandle(0,texHandle);
			XVP::GraphicsMgr.destroyTexture(texHandle);
		}
	}

	if (pMaterial->fillMode == 0)
		pRs->setFillMode(X_FMT_SOLID);
	else
		pRs->setFillMode(X_FMT_WIRE);

	XColor diffuse(pMaterial->diffuseColor.x,
					pMaterial->diffuseColor.y,
					pMaterial->diffuseColor.z,
					pMaterial->diffuseColor.w);
	XColor ambient(pMaterial->ambientColor.x,
					pMaterial->ambientColor.y,
					pMaterial->ambientColor.z,
					pMaterial->ambientColor.w);
	XColor specular(pMaterial->specularColor.x,
					pMaterial->specularColor.y,
					pMaterial->specularColor.z,
					pMaterial->specularColor.w);

	pRs->m_ambient = ambient;
	pRs->m_diffuse = diffuse;
	pRs->m_specular = specular;
	pRs->m_specPower = pMaterial->specPower;

	//Create the one and only model object for the particle system.
	XParticleEmitter* pNewEmitter = NULL;
	XNew(pNewEmitter,XParticleEmitter,XOSMgr::getDefaultHeap());
	pNewEmitter->setMaterialHandle(m_matHandle);
	m_nodeArray.push_back(pNewEmitter);
	m_nodeTree.createNode(pNewEmitter,NULL,pParticleSystem->name);

	//Everything was created okay. Release model data and return.
	loader.destroyModelData();

	m_inited = true;

	return X_OK;
}*/


//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemTemplate::init(XU32 numEmitters)
{
	X_ASSERTM(numEmitters > 0,"Can't create a particles system with 0 emitters");

	//Initialize the particle system template
	destroy();
	XParticleSystemTemplate();

	//This reference count only goes up when the user creates
	//a particle system instance that references this template.
	m_refCount = 0;

	m_numNodes = numEmitters;
	m_defaultPose.init(numEmitters);
	
	XSceneGraphNode* pLastNode = NULL;
	XS8 buf[25] = "Emitter";
	XS8 newName[25];
	for (XU32 i = 0;i < numEmitters;++i)
	{
		//Create new name
		sprintf(newName,"%s%d",buf,i);

		XParticleEmitter* pNewEmitter = NULL;
		XNew(pNewEmitter,XParticleEmitter,XOSMgr::getDefaultHeap());
		if (!pLastNode)
			pLastNode = m_nodeTree.createNode(pNewEmitter,NULL,newName);
		else
			pLastNode = m_nodeTree.createNode(pNewEmitter,pLastNode->m_pName,newName);

		tempSGN n;
		n.name = pLastNode->m_pName;
		if (pLastNode->m_pParentNode)
			n.pname = pLastNode->m_pParentNode->m_pName;
		else
			n.pname = NULL;
		n.pNode = pLastNode->m_pSceneNode;

		m_nodeArray.push_back(n);

		pNewEmitter->setHierarchyID(i);
				
		//Add a transform for this emitter
		m_defaultPose.addTransform(XMatrix4x4());

		//Create material for emitter
		XHandle matHandle = X_INVALID_HANDLE;
		XVP::GraphicsMgr.createMaterial(NULL,matHandle);
		X_ASSERTM(matHandle != X_INVALID_HANDLE,"Could not create emitter material");
		pNewEmitter->setMaterialHandle(matHandle);
	}

	m_inited = true;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemTemplate::initFromMemory(XS8* pMemoryChunk)
{
	X_ASSERTM(pMemoryChunk,"Invalid memory chunk");

	//Initialize the particle system template
	destroy();
	XParticleSystemTemplate();

	//This reference count only goes up when the user creates
	//a particle system instance that references this template.
	m_refCount = 0;

	//Initialize from memory stream
	XU32 numEmitters = *(XU32*)pMemoryChunk;
	pMemoryChunk += sizeof(XU32);

	m_numNodes = numEmitters;
	m_defaultPose.init(numEmitters);
	
	XSceneGraphNode* pLastNode = NULL;
	for (XU32 i = 0;i < numEmitters;++i)
	{
		//Get emitter name and create a new emitter
		XU32 nameLength = *(XU32*)pMemoryChunk;
		pMemoryChunk += sizeof(XU32);

		//XParentChildCombo pcCombo;
		//m_parentChildList.push_back(pcCombo);
		//if (pLastNode)
		//	m_parentChildList[i].setParentName(pLastNode->m_pName);		
		//m_parentChildList[i].setChildName(pMemoryChunk);

		
		XParticleEmitter* pNewEmitter = NULL;
		XNew(pNewEmitter,XParticleEmitter,XOSMgr::getDefaultHeap());
		//m_nodeArray.push_back(pNewEmitter);
		if (!pLastNode)
			pLastNode = m_nodeTree.createNode(pNewEmitter,NULL,pMemoryChunk);
		else
			pLastNode = m_nodeTree.createNode(pNewEmitter,pLastNode->m_pName,pMemoryChunk);

		tempSGN n;
		n.name = pLastNode->m_pName;
		if (pLastNode->m_pParentNode)
			n.pname = pLastNode->m_pParentNode->m_pName;
		else
			n.pname = NULL;
		n.pNode = pLastNode->m_pSceneNode;

		m_nodeArray.push_back(n);

		pNewEmitter->setHierarchyID(i);
		pMemoryChunk += nameLength;
		
		//Add a transform for this emitter
		m_defaultPose.addTransform(XMatrix4x4());

		//EmitterProperties
		pNewEmitter->setMaxParticles(*(XU32*)pMemoryChunk);	pMemoryChunk += sizeof(XU32);
		pNewEmitter->setEmitterType(X_EMITTER_TYPE(*(XU32*)pMemoryChunk)); pMemoryChunk += sizeof(XU32);
		pNewEmitter->setEmitterLength(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setEmitterWidth(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setEmitterHeight(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setBirthRate(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleGrowTime(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleShrinkTime(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleSpeed(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleSpeedVariation(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleLife(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleLifeVariation(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleRotSpeed(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleRotSpeedVariation(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleSize(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleSizeVariation(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		
		XVector3D dirVec;
		dirVec.x = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
		dirVec.y = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
		dirVec.z = *(XF32*)pMemoryChunk; pMemoryChunk += sizeof(XF32);
		pNewEmitter->setDirectionVector(dirVec);
		
		pNewEmitter->setDirectionVecVariation(*(XF32*)pMemoryChunk); pMemoryChunk += sizeof(XF32);
		pNewEmitter->setParticleStartColor(*(XColor*)pMemoryChunk); pMemoryChunk += sizeof(XColor);
		pNewEmitter->setParticleEndColor(*(XColor*)pMemoryChunk); pMemoryChunk += sizeof(XColor);

		pNewEmitter->enableOneShot(*(XBOOL*)pMemoryChunk); pMemoryChunk += sizeof(XBOOL);

		//Get material for emitter
		XHandle matHandle = XVP::GraphicsMgr.getMaterial(pMemoryChunk);
		XMaterial* pMat = XVP::GraphicsMgr.getMaterialInterface(matHandle);	
		X_ASSERTM(pMat,"Could not find particle system material.  Expected material to already exist.");
		pNewEmitter->setMaterialHandle(matHandle);
		pMemoryChunk += sizeof(XBSMaterialChunk);
	}


	m_inited = true;

	return X_OK;
}



//*****************************************************************************
/**
Generates a particle system instance from this particle system template.  This
function can be used to generate any number of particle system instances from
this template.  Before this function can be called however, the particle system
template has to initialized with a call to init().

It's important to note that as long as the particle system instances that are
created from this template exist, the template must also exist.  If a template, that
was used to create instances, goes out of scope while instances still exist,
the application will become unstable.  Therefore, it's important to know when
it's safe to deallocate the template.  A good rule of thumb is to destoy all 
instances of the object before destroying the template used to create them.

@param	particleSystem	An XParticleSystem object to hold the newly generated 
						particle system.
@return	Returns a status code of type XStatus.
*/
//*****************************************************************************
XStatus XParticleSystemTemplate::generateParticleSystem(XParticleSystem& particleSystem)
{	
	//Re-initialize the particle system in case it contains data.
	particleSystem.destroy();
	particleSystem.init();


	//First we need to create the appropriate nodes in the new tree.
	//The parent/child names for creating the new tree can be found in 
	//the modelObjectTree's m_parentChildList.  We will use these to create
	//the new nodes.
	XNewArray(particleSystem.m_pNodeArray,XSceneNode*,m_numNodes,XOSMgr::getModelHeap())
	for (XU32 i = 0;i < m_numNodes;i++)
	{
		//Insert a new node in to the instance tree
		XParticleEmitter* pOrgEmitter = NULL;
		XParticleEmitter* pNewEmitter = NULL;
		XNew(pNewEmitter,XParticleEmitter,XOSMgr::getDefaultHeap());
		particleSystem.m_pNodeArray[i] = pNewEmitter;

		if (m_nodeArray[i].pname)
			particleSystem.m_nodeTree.createNode(pNewEmitter,
												 m_nodeArray[i].pname,
												 m_nodeArray[i].name);
		else
		{
			particleSystem.m_nodeTree.createNode(pNewEmitter,
												 NULL,
												 m_nodeArray[i].name);

		}
		
		//Find the node in the model tree that matches the one we just created.
		pOrgEmitter = (XParticleEmitter*)m_nodeArray[i].pNode;//(m_nodeTree.getNode(m_parentChildList[i].getChildName()))->m_pSceneNode;

		//Copy the relevant data from template to instance
		pNewEmitter->setHierarchyID(pOrgEmitter->getHierarchyID());
		pNewEmitter->setMaterialHandle(pOrgEmitter->getMaterialHandle());

		//Copy over the inital model transform matrix.
		pNewEmitter->setModelTransform(pOrgEmitter->getModelTransform());
		pNewEmitter->m_pParentModel = &particleSystem;
		pNewEmitter->m_pDataTemplate = (void*)pOrgEmitter;

		pNewEmitter->setMaxParticles(pOrgEmitter->getMaxParticles());
		pNewEmitter->setEmitterType(pOrgEmitter->getEmitterType());
		pNewEmitter->setEmitterLength(pOrgEmitter->getEmitterLength());
		pNewEmitter->setEmitterWidth(pOrgEmitter->getEmitterWidth());
		pNewEmitter->setEmitterHeight(pOrgEmitter->getEmitterHeight());
		pNewEmitter->setBirthRate(pOrgEmitter->getBirthRate());
		pNewEmitter->setParticleStartColor(pOrgEmitter->getParticleStartColor());
		pNewEmitter->setParticleEndColor(pOrgEmitter->getParticleEndColor());
		pNewEmitter->setParticleGrowTime(pOrgEmitter->getParticleGrowTime());
		pNewEmitter->setParticleShrinkTime(pOrgEmitter->getParticleShrinkTime());
		pNewEmitter->setParticleSpeed(pOrgEmitter->getParticleSpeed());
		pNewEmitter->setParticleSpeedVariation(pOrgEmitter->getParticleSpeedVariation());
		pNewEmitter->setParticleLife(pOrgEmitter->getParticleLife());
		pNewEmitter->setParticleLifeVariation(pOrgEmitter->getParticleLifeVariation());
		pNewEmitter->setParticleRotSpeed(pOrgEmitter->getParticleRotSpeed());
		pNewEmitter->setParticleRotSpeedVariation(pOrgEmitter->getParticleRotSpeedVariation());
		pNewEmitter->setParticleSize(pOrgEmitter->getParticleSize());
		pNewEmitter->setParticleSizeVariation(pOrgEmitter->getParticleSizeVariation());
		pNewEmitter->setDirectionVector(pOrgEmitter->getDirectionVec());
		pNewEmitter->setDirectionVecVariation(pOrgEmitter->getDirectionVecVariation());
		pNewEmitter->enableOneShot(pOrgEmitter->isOneShotEnabled());
		

	}
	

	//Insert a new node in to the instance tree
	/*XParticleEmitter* pEmitter = NULL;
	XNew(pEmitter,XParticleEmitter,XOSMgr::getDefaultHeap());

	XNewArray(particleSystem.m_pNodeArray,XSceneNode*,1,XOSMgr::getModelHeap())
	particleSystem.m_pNodeArray[0] = pEmitter;
	particleSystem.m_nodeTree.createNode(pEmitter,m_parentChildList[0].getParentName(), m_parentChildList[0].getChildName());
		
	//Find the node in the model tree that matches the one we just created.
	pOrgEmitter = (XParticleEmitter*)(m_nodeTree.getNode(m_parentChildList[0].getChildName()))->m_pSceneNode;
	
*/
	
	particleSystem.m_numNodes = m_numNodes;
	particleSystem.setPose(&m_defaultPose);
	
	//Increment the particle system template refCount.
	++m_refCount;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XParticleSystemTemplate::attachShaderController(XS8* pMaterialName,
														  XShaderController* pMatController)
{
	for (XU32 i = 0;i < m_nodeArray.size();++i)
		((XParticleEmitter*)m_nodeArray[i].pNode)->attachShaderController(pMaterialName,pMatController);

	return X_OK;
}