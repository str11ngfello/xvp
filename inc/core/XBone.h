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
//	Module Name:	XBone.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BONE_H__
#define __X_BONE_H__




//*****************************************************************************
//! Bone objects used in skin mesh implementation.
/**
XBone objects are used in the creation of skin meshes and are responsible for
deforming the skin at runtime.
*/
//*****************************************************************************
class XBone : public XModelObject
{							        
public:
	//XSkinMesh*				m_pParentSkinMesh;
	XMatrix4x4				m_skinTransform;
	XMatrix4x4				m_invBoneTransform;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBone();
	~XBone();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus prepForRender(XF32 dt,XScene* pScene);
	XMatrix4x4& getSkinTransform();
	XMatrix4x4& getInvBoneTransform();
	void setInvBoneTransform(XMatrix4x4& invBoneTransform);
	XStatus getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared);
	XStatus setLODIndex(XU8 LODIndex);
};


#endif
