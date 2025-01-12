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
//	Module Name:	XParticle.cpp
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
Constructor for the particle.  
*/
//*****************************************************************************
XParticle::XParticle() 
{
	m_age = 0;
	m_lifetime = 0;
	m_currentSize = 0;
	m_maxSize = 0;
	m_alphaDelta = 0;
	m_redDelta = 0;
	m_blueDelta = 0;
	m_greenDelta = 0;
	m_growTime = 0;
	m_shrinkTime = 0;
	m_rotationSpeed = 0.0f;
	m_pParentEmitter = NULL;
}



//*****************************************************************************
/**
Updates a particle.  This function gives the particle a chance to update
any internal, time-dependent data.

@param dt	The amount of time, in seconds, that has passed since the
					last update.
@return True if the particle is still alive, false otherwise.
*/
//*****************************************************************************
XBOOL XParticle::update(XF32 dt)
{
	//First update the age of the particle.  If it has died,
	//then return false to indicate the particle has died.
	if (m_age + dt > m_lifetime)
		return false;

	//If we make if here then this particular particle is alive so we
	//should update it.  Start with the age and position.
	m_age += dt;
	m_prevPosition = m_position;
	m_position += m_velocity * dt;

	//Calculate new diffuse color
	m_diffuseColor.m_A += (m_alphaDelta * dt);
	m_diffuseColor.m_R += (m_redDelta * dt);
	m_diffuseColor.m_G += (m_greenDelta * dt);
	m_diffuseColor.m_B += (m_blueDelta * dt);

	//Calculate new size
	/*XF32 progress = m_age/m_lifetime;
	if (progress < m_pParentEmitter->getParticleGrowTime())
		m_currentSize = m_maxSize * (m_age/m_growTime);
	else if (progress > m_pParentEmitter->getParticleShrinkTime())
		m_currentSize = m_maxSize * ((m_lifetime-m_age)/(m_lifetime-m_shrinkTime));
	else
		m_currentSize = m_maxSize;*/
	

	return true;
}