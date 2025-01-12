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
//	Module Name:	XParticle.h
//	Author:			Lynn Duke
//	Creation Date:	4-11-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PARTICLE_H__
#define __X_PARTICLE_H__


class XParticleEmitter;

//*****************************************************************************
//! Particle object used to build particle systems.
/** 
The XParticle object is used to build systems of particles.  Each particle has
properties that are unique to the particle as well as some that are unique
to the system in which the particle lives.
*/
//*****************************************************************************
class XParticle
{
public:
	XVector3D			m_position;
	XVector3D			m_prevPosition;
	XVector3D			m_velocity;
	XF32				m_age;
	XF32				m_lifetime;
	XF32				m_currentSize;
	XF32				m_maxSize;
	XColor				m_diffuseColor;
	XF32				m_alphaDelta;
	XF32				m_redDelta;
	XF32				m_blueDelta;
	XF32				m_greenDelta;
	XF32				m_growTime;
	XF32				m_shrinkTime;
	XF32				m_rotationSpeed;
	XParticleEmitter*	m_pParentEmitter;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XParticle();
	//virtual ~XParticle();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XBOOL update(XF32 dt);
};

#endif