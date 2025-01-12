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
//	Module Name:	XStage.h
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_STAGE_H__
#define __X_STAGE_H__




//*****************************************************************************
//*****************************************************************************
class XStage
{					  
protected:
	
	struct ActorInfo
	{
		XS8*			pManifest;
		XMatrix4x4		transform;
	};

	XStringMap<ActorInfo>		m_actorMap;
	XWad						m_wad;
	
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XStage();
	virtual ~XStage();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus initFromWadFile(XS8* pFilename);
	virtual XStatus generateActor(XS8* pActorName,XActor& actor);
	virtual XStatus destroy();
};

#endif
