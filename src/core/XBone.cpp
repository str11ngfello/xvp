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
//	Module Name:	XBone.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the bone.
*/
//*****************************************************************************
XBone::XBone()
{
	//m_parentSkinMeshPtr = NULL;		//Only valid when a bone is a member 
									//of a skin mesh instance, not a skin
									//mesh template	
}



//*****************************************************************************
/**
Destructor for the bone.
*/
//*****************************************************************************
XBone::~XBone()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XBone::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL_OBJECT,X_SNID_BONE);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the bone.  This is the
description that appears in log files and other debug output.

@return A string description of the bone.
*/
//*****************************************************************************
XS8* XBone::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XBone";
	return description; 
}



//*****************************************************************************
/**
Updates the bones world space transform and updates the skin transform used
during the skinning process.

@param dt	The amount of time, in seconds, that has elapsed since the last
					update.
@param pMatrixStack	The scene graph matrix stack.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XBone::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)	
{
	//We premultiply the bone transform with the parent's transform.
	//This causes the bone to inherit all motion of the parent in addition
	//to it's own.
	//m_worldTransform = m_modelTransform * (*pMatrixStack).back();

	//Also, we push this node's matrix on the stack so that children may
	//access it in the same manner.
	//pMatrixStack->push_back(m_worldTransform);

	//We need to update the skin transform.  It will be used to 
	//skin vertices later.  Here we'll use the inverse bone transform to
	//get from skin space to bone space.
	m_skinTransform = m_invBoneTransform * m_worldTransform;

	
	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XBone::prepForRender(XF32 dt,XScene* pScene)
{
	//Render only if render is allowed
	//if (X_SNP_RENDER & m_propertyFlags)
	//	pScene->addRenderEvent(this,NULL,XDUL::getSphereMaterial(),0);
	/*X_DUL
	(
		XDUL::drawSphere(pScene,XVector3D(m_worldTransform.row4),1.0f,XColor(1,1,1,1));
	)*/

	return X_OK;
}

//*****************************************************************************
/**
Returns the skin transform of the bone.  This transform represents the
transformations necessary to get vertices into skin space.

@return The bone's skin transform.
*/
//*****************************************************************************
XMatrix4x4& XBone::getSkinTransform()
{
	return m_skinTransform;
}



//*****************************************************************************
/**
Returns the inverse bone transform.  This transform represents the transformations
necessary to get vertices from object space into bone space.

@return The bone's inverse transform.
*/
//*****************************************************************************
XMatrix4x4& XBone::getInvBoneTransform()
{
	return m_invBoneTransform;
}



//*****************************************************************************
/**
Returns the inverse bone transform.  This transform represents the transformations
necessary to get vertices from object space into bone space.

@return The bone's inverse transform.
*/
//*****************************************************************************
void XBone::setInvBoneTransform(XMatrix4x4& invBoneTransform)
{
	m_invBoneTransform = invBoneTransform;
}



//*****************************************************************************
//*****************************************************************************
XStatus XBone::getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared)
{
	return X_ERR_NOT_SUPPORTED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XBone::setLODIndex(XU8 LODIndex)
{
	return X_ERR_NOT_SUPPORTED;
}