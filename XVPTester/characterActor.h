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
//	Module Name:	XActor.h
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __CHARACTER_ACTOR_H__
#define __CHARACTER_ACTOR_H__





//*****************************************************************************
//*****************************************************************************
class CharacterActor : public XActor
{					  
private:
	XStringMap<XModelTemplate*>				m_modelTemplates;
	XStringMap<XModel*>						m_models;
	XStringMap<XParticleSystemTemplate*>	m_particleSystemTemplates;
	XStringMap<XParticleSystem*>			m_particleSystems;
	XStringMap<XHandle>						m_materials;
	XStringMap<XCamera*>					m_cameras;
	XStringMap<XLight*>						m_lights;
	XStringMap<XController*>				m_controllers;

	XMemBlockList<XAnimTrack*>				m_animTracks;

	XAnimController*	m_pAnimController;
	XAnimTrack			m_animTrack;
	XHandle				m_atHandle;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	CharacterActor();
	virtual ~CharacterActor();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus handleInit();
	virtual XStatus destroy();

	virtual XStatus addToScene(XScene& scene,XBOOL update,XBOOL render,XU32 updateLevel,XU32 renderLevel);
	virtual XStatus setWorldPosition(XVector3D position);

	XStatus loadModels();
	XStatus destroyModels();
	XStatus loadParticleSystems();
	XStatus destroyParticleSystems();
	XStatus loadMaterials();
	XStatus destroyMaterials();
	XStatus loadCameras();
	XStatus destroyCameras();
	XStatus loadLights();
	XStatus destroyLights();
	XStatus loadControllers();
	XStatus destroyControllers();
};

#endif
