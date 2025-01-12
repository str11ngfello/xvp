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
//	Module Name:	XPose.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-23-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XPose::XPose()
{
	m_pTransforms = NULL;
	m_numTransforms = 0;
	m_maxTransforms = 0;
}



//*****************************************************************************
//*****************************************************************************
XPose::~XPose()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XPose::init(XS8* pFilename)
{
	//Validate filename
	X_ASSERTM(pFilename,"Invalid filename");
	destroy();
	XModelLoader loader;
		
	if (loader.load(pFilename) == X_OK)
	{
		//Allocate space for transforms
		XNewArray(m_pTransforms,XMatrix4x4,loader.m_model.poseTransforms.size(),XOSMgr::getModelHeap());
		m_maxTransforms = loader.m_model.poseTransforms.size();
		for (XU32 i = 0;i < m_maxTransforms;++i)
			m_pTransforms[m_numTransforms++] = loader.m_model.poseTransforms[i];
	}


	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XPose::init(XU32 maxTransforms)
{
	X_ASSERTM(maxTransforms != 0,"num transforms must be greater than 0");
	
	destroy();
	XNewArray(m_pTransforms,XMatrix4x4,maxTransforms,XOSMgr::getModelHeap());
	m_maxTransforms = maxTransforms;

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XPose::addTransform(XMatrix4x4& transform)
{
	X_ASSERTM(m_numTransforms < m_maxTransforms,"Exceeded max transforms");
	m_pTransforms[m_numTransforms++] = transform;

	return X_OK;
}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XPose::destroy()
{
	XDeleteArray(m_pTransforms);
	m_pTransforms = NULL;
	m_numTransforms = 0;
	m_maxTransforms = 0;


	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XPose::getPoseTransform(XU32 id)
{
	X_ASSERTM(id < m_maxTransforms,"No such ID in pose.");
	return m_pTransforms[id];
}



//*****************************************************************************
//*****************************************************************************
XU32 XPose::getNumTransforms()
{
	return m_numTransforms;
}