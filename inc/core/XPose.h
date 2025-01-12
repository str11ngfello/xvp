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
//	Module Name:	XPose.h
//	Author:			Lynn Duke
//	Creation Date:	7-23-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************


#ifndef __X_POSE_H__
#define __X_POSE_H__




class XPose
{
private:
	XU32			m_numTransforms;
	XU32			m_maxTransforms;
	XMatrix4x4*		m_pTransforms;

public:
	XPose();
	~XPose();
	XStatus init(XS8* pFilename);
	XStatus init(XU32 m_maxTransforms);
	XStatus destroy();
	XStatus addTransform(XMatrix4x4& transform);
	XMatrix4x4& getPoseTransform(XU32 id);
	XU32 getNumTransforms();
};


#endif
