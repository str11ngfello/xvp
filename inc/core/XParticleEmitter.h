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
//	Module Name:	XParticleEmitter.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_PARTICLE_SYSTEM_OBJECT_H__
#define __X_PARTICLE_SYSTEM_OBJECT_H__





//*****************************************************************************
//! Particle system emitter types.
/**
These defines specify various types of emitters for particle systems.  
*/
//*****************************************************************************
enum X_EMITTER_TYPE
{
	X_ET_BOX,
	X_ET_SPHERE,
	X_ET_CYLINDER
};


//*****************************************************************************
//! Particle System objects used in the camera implementation.
/**
XParticleEmitter objects are used in the creation of particle systems and
provide movement functionality.
*/
//*****************************************************************************
class XParticleEmitter : public XModelObject
{				
protected:
	XHandle						m_matHandle;
	//static XU32					m_maxVertexBufferSize;
	//static XParticleVertex*		m_pParticleVertexBuffer;
	XHandle						m_vbHandle;
	static XHandle				m_vertexDeclarationHandle;
	XFreeList<XParticle>		m_particleFreeList;
	XParticle**					m_pParticlesInUse;
	XU32						m_maxParticles;
	XU32						m_numActiveParticles;
	X_EMITTER_TYPE				m_emitterType;
	XF32						m_emitterLength;
	XF32						m_emitterWidth;
	XF32						m_emitterHeight;
	XF32						m_birthRate;
	XF32						m_particleGrowTime;
	XF32						m_particleShrinkTime;
	XF32						m_particleSpeed;
	XF32						m_particleSpeedVariation;
	XF32						m_particleLife;
	XF32						m_particleLifeVariation;
	XF32						m_particleSize;
	XF32						m_particleSizeVariation;
	XF32						m_particleRotSpeed;
	XF32						m_particleRotSpeedVariation;
	XVector3D					m_directionVec;
	XVector3D					m_translatedDirectionVec;
	XF32						m_directionVecVariation;
	XF32						m_time;
	XF32						m_particleResidue;
	XColor						m_startColor;
	XColor						m_endColor;
	XU32						m_totalParticlesEmitted;
	XBOOL						m_oneShot;
	XBOOL						m_running;

private:
	//*************************************************************************
	//Private Interface
	//*************************************************************************
	XVector3D varyVector(XVector3D& vector,XF32 percent,XF32 maxAngle);
	void rotatePoint(XVector3D& point,XVector3D& q,XVector3D& w,XF32 theta);
	void setUpRotation(XVector3D& q,XVector3D& w,XF32 theta,XMatrix4x4& rq);
	friend class XVP;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XParticleEmitter();
	~XParticleEmitter();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XStatus destroy();
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus prepForRender(XF32 dt,XScene* pScene);
	XStatus render(XRenderEvent* pRenderEvent);
	XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
	XHandle getMaterialHandle();
	XStatus setMaterialHandle(XHandle matHandle);
	void pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits);
	XStatus getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared);
	XStatus setLODIndex(XU8 LODIndex);
	XU32 getMaxParticles();
	XStatus setMaxParticles(XU32 maxParticles);
	virtual XParticle* generateParticle();
	virtual void destroyParticle(XParticle* pParticle);
	void start();
	void restart();
	void stop();
	XBOOL isRunning();
	XU32 getNumParticles();
	void setEmitterType(X_EMITTER_TYPE emitterType);
	X_EMITTER_TYPE getEmitterType();
	void setEmitterLength(XF32 emitterLength);
	XF32 getEmitterLength();
	void setEmitterWidth(XF32 emitterWidth);
	XF32 getEmitterWidth();
	void setEmitterHeight(XF32 emitterHeight);
	XF32 getEmitterHeight();
	void setBirthRate(XF32 birthRate);
	XF32 getBirthRate();
	void setParticleGrowTime(XF32 particleGrowTime);
	XF32 getParticleGrowTime();
	void setParticleShrinkTime(XF32 particleShrinkTime);
	XF32 getParticleShrinkTime();
	void setParticleSpeed(XF32 particleSpeed);
	XF32 getParticleSpeed();
	void setParticleSpeedVariation(XF32 particleSpeedVariation);
	XF32 getParticleSpeedVariation();
	void setParticleLife(XF32 particleLife);
	XF32 getParticleLife();
	void setParticleLifeVariation(XF32 particleLifeVariation);
	XF32 getParticleLifeVariation();
	void setParticleSize(XF32 particleSize);
	XF32 getParticleSize();
	void setParticleSizeVariation(XF32 particleSizeVariation);
	XF32 getParticleSizeVariation();
	void setParticleRotSpeed(XF32 particleRotSpeed);
	XF32 getParticleRotSpeed();
	void setParticleRotSpeedVariation(XF32 particleRotSpeedVariartion);
	XF32 getParticleRotSpeedVariation();
	void setParticleStartColor(XColor& startColor);
	XColor getParticleStartColor();
	void setParticleEndColor(XColor& endColor);
	XColor getParticleEndColor();
	void setDirectionVector(XVector3D directionVector);
	XVector3D getDirectionVec();
	void setDirectionVecVariation(XF32 directionVecVariation);
	XF32 getDirectionVecVariation();
	void enableOneShot(XBOOL enable);
	XBOOL isOneShotEnabled();
	void flushParticles();

	//Property Bag Methods
	XStatus setBoolProperty(XS8* pPropertyName,XBOOL value);
	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
	XStatus setUIntProperty(XS8* pPropertyName,XU32 value);
	XStatus setColorProperty(XS8* pPropertyName,XColor& value);
	XStatus setEnumProperty(XS8* pPropertyName,XS32 value);
	XStatus setVector3DProperty(XS8* pPropertyName,XVector3D& value);

};


#endif
