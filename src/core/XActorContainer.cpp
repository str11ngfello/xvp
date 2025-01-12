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
//	Module Name:	XActorContainer.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-19-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the WAD file.  
*/
//*****************************************************************************
XActorContainer::XActorContainer()
{

}



//*****************************************************************************
/**
Destructor for the WAD file.  
*/
//*****************************************************************************
XActorContainer::~XActorContainer()
{
}


//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XActorContainer::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_ACTOR_CONTAINER,X_SNID_ACTOR_CONTAINER);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the bone.  This is the
description that appears in log files and other debug output.

@return A string description of the bone.
*/
//*****************************************************************************
XS8* XActorContainer::getTypeDesc()
{
	static char description[X_TYPE_DESC_LEN] = "XActorContainer";
	return description; 
}


XStatus XActorContainer::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//If no scene nodes, we're done...
	if (m_sceneNodes.getNumItems() == 0)
		return X_OK;

	//Find a average center for the bounding sphere using all the actor nodes
	XMemBlockList<XSceneNode*>::iterator i(m_sceneNodes.begin());
	XVector3D newCenter;
	XSphere sphere;
	while (i.isValid())
	{
		newCenter += i.getValue()->getActorContainerBoundingSphere().center;
		i.getNext();
	}
	sphere.center = newCenter / (XF32)m_sceneNodes.getNumItems();

	//set model transforms and update the actor container's bounding sphere
	i.set(m_sceneNodes.begin());
	while (i.isValid())
	{
		i.getValue()->setModelTransform(m_modelTransform);
		XISphereEnclosingSpheres(sphere,sphere,i.getValue()->getActorContainerBoundingSphere());
		i.getNext();
	}
	m_boundingSphere = sphere;
	

	return X_OK;
}

void XActorContainer::pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits)
{
	//Actor Containers don't react to picks so they don't get selected in the
	//editor
	return;
}
