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
//	Module Name:	XParticleSystem.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the particle system.  
*/
//*****************************************************************************
XParticleSystem::XParticleSystem()
{
}



//*****************************************************************************
/**
Destructor for the particle system.  Destroys all remaining particles.
*/
//*****************************************************************************
XParticleSystem::~XParticleSystem()
{
	
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XParticleSystem::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL,X_SNID_PARTICLE_SYSTEM);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the particle system.  This is the
description that appears in log files and other debug output.

@return A string description of the particle system.
*/
//*****************************************************************************
XS8* XParticleSystem::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XParticleSystem";
	return description; 
}


//*****************************************************************************
//*****************************************************************************
XU32 XParticleSystem::getNumEmitters()
{
	return m_numNodes;
}



//*****************************************************************************
//*****************************************************************************
XParticleEmitter* XParticleSystem::getEmitter(XU32 emitterIndex)
{
	X_ASSERTM(emitterIndex < m_numNodes,"No such emitter index in this particle system");
	return (XParticleEmitter*)m_pNodeArray[emitterIndex];
}


XStatus XParticleSystem::addEmitter(XS8* pParentEmitterName,XS8* pEmitterName)
{
	//First we need to create a new node in the tree.
	//Recreate the array of emitters
	XS8 buf[25] = "Emitter";
	XS8 buf2[25] = "Emitter";
	XS8 parentName[25];
	XS8 newName[25];
	sprintf(newName,"%s%d",buf,m_numNodes);
	sprintf(parentName,"%s%d",buf,m_numNodes-1);

	XSceneNode** pTempNodes = NULL;
	XNewArray(pTempNodes,XSceneNode*,m_numNodes+1,XOSMgr::getModelHeap())
	for (XU32 i = 0;i < m_numNodes;i++)
		pTempNodes[i] = m_pNodeArray[i];

	//Insert a new emitter at the end
	XParticleEmitter* pNewEmitter = NULL;
	XNew(pNewEmitter,XParticleEmitter,XOSMgr::getDefaultHeap());
	pTempNodes[m_numNodes] = pNewEmitter;

	m_nodeTree.createNode(pNewEmitter,
						 parentName,
						 newName);
		
	pNewEmitter->setHierarchyID(0);

	//Create material for emitter
	XHandle matHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.createMaterial(NULL,matHandle);
	X_ASSERTM(matHandle != X_INVALID_HANDLE,"Could not create emitter material");
	pNewEmitter->setMaterialHandle(matHandle);

	//Copy over the inital model transform matrix.
	pNewEmitter->m_pParentModel = this;
	//pNewEmitter->m_pDataTemplate = (void*)pOrgEmitter;

	++m_numNodes;
	
	XDeleteArray(m_pNodeArray)
	m_pNodeArray = pTempNodes;

	return X_OK;
}

XStatus XParticleSystem::start()
{
	for (XU32 i = 0;i < m_numNodes;++i)
		((XParticleEmitter*)m_pNodeArray[i])->start();

	return X_OK;
}
XStatus XParticleSystem::stop()
{
	for (XU32 i = 0;i < m_numNodes;++i)
		((XParticleEmitter*)m_pNodeArray[i])->stop();
	return X_OK;
}
XStatus XParticleSystem::restart()
{
	for (XU32 i = 0;i < m_numNodes;++i)
		((XParticleEmitter*)m_pNodeArray[i])->restart();
	return X_OK;
}