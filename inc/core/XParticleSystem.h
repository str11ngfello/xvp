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

#ifndef __X_PARTICLE_SYSTEM_H_
#define __X_PARTICLE_SYSTEM_H_





//*****************************************************************************
//! Represents a system of particles used to model certain types of phenomena.
/** 
The XParticleSystem object is used to represent a system of particles used 
to model certain types of phenomena.  As time progresses, individual particles
are "spawned" from the particle system's emitter.  An emitter is simply a 3D
volume where particles start their life. The particle system has many different
settings that affect the overall behavior of the system. 

To initialize an XParticleSystem object, you must first load in an
XParticleSystemTemplate object.  Once you have a template, you can create any
number of XParticleSystem objects from it.  See XParticleSystemTemplate for
details. All particle system instances generated from the same template will
share the same geometry, render state, and other memory intensive data, but will
contain their own instance data such as world space transforms, controller
attachements, etc.
*/
//*****************************************************************************
class XParticleSystem : public XModel
{

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XParticleSystem();
	virtual ~XParticleSystem();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XU32 getNumEmitters();
	XParticleEmitter* getEmitter(XU32 emitterIndex);
	XStatus addEmitter(XS8* pParentEmitterName,XS8* pEmitterName);
	XStatus start();
	XStatus stop();
	XStatus restart();
};

#endif
