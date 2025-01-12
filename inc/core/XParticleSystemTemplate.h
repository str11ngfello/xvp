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
//	Module Name:	XParticleSystemTemplate.h
//	Author:			Lynn Duke
//	Creation Date:	1-2-2004
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PARTICLE_SYSTEM_TEMPLATE_H__
#define __X_PARTICLE_SYSTEM_TEMPLATE_H__





//*****************************************************************************
//! Loads in a particle system from a .xps file.
/**
The XParticleSystemTemplate object is used to hold data for a particle
system in memory. The XParticleSystemTemplate can load in a predefined
particle system from a .xps file.  Once a particle system template has been
loaded, you can then create any number of particle system instances from this
template by using one or more XParticleSystem objects.  

The steps below illustrate typical usage of an XParticleSystemTemplate object
in conjunction with XParticleSystem objects:

1.  Initialize an XParticleSystemTemplate with a .xps file. <br>
2.  Generate any number of XParticleSystem objects from the 
	XParticleSystemTemplate object.<br>
3.  Add the XParticleSystem objects to the scene.

The XParticleSystemTemplate object by itself is not enough to render the
particle system. It only provides a way to generate particle system instances. 
You must create at least one particle system instance from the template to
actually start rendering the particle system.
*/
//*****************************************************************************
class XParticleSystemTemplate
{
private:
	struct tempSGN
	{
		XS8* name;
		XS8* pname;
		XSceneNode* pNode;
	};
	XSceneGraph							m_nodeTree;
	vector<tempSGN>						m_nodeArray;
	//vector<XParentChildCombo>			m_parentChildList;
	XU32								m_refCount;
	XU32								m_numNodes;

	/*
	X_EMITTER_TYPE						m_emitterType;
	XF32								m_emitterLength;
	XF32								m_emitterWidth;
	XF32								m_emitterHeight;
	XF32								m_birthRate;
	XF32								m_startAlpha;
	XF32								m_endAlpha;
	XVector3D							m_startColor;
	XVector3D							m_endColor;
	XF32								m_particleGrowTime;
	XF32								m_particleShrinkTime;
	XF32								m_particleSpeed;
	XF32								m_particleSpeedVariation;
	XF32								m_particleLife;
	XF32								m_particleLifeVariation;
	XF32								m_particleSize;
	XF32								m_particleSizeVariation;
	XVector3D							m_directionVec;
	XF32								m_directionVecVariation;
	*/
	XPose								m_defaultPose;
	XBOOL								m_inited;

public:
	//*************************************************************************
	//Constructors/Destructors
	//*************************************************************************
	XParticleSystemTemplate();
	~XParticleSystemTemplate();

	//*************************************************************************
	//Public Interface
	//*************************************************************************
	XStatus init(XU32 numEmitters);
	XStatus initFromMemory(XS8* pMemoryChunk);
	XStatus destroy();
	XStatus generateParticleSystem(XParticleSystem& particleSystem);
	XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
};


#endif
