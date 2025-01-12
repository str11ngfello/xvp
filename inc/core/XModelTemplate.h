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
//	Module Name:	XModelTemplate.h
//	Author:			Lynn Duke
//	Creation Date:	5-22-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_TEMPLATE_H__
#define __X_MODEL_TEMPLATE_H__





//*****************************************************************************
//! Loads in a model from a .xmf file.
/**
The XModelTemplate object is used to hold data for a model in memory.
The XModelTemplate can load in a predefined model from a .xmf file.
Once a model template has been loaded, you can then create any number of 
model instances from this template by using one or more XModel objects.  

The steps below illustrate typical usage of an XModelTemplate object in 
conjunction with XModel objects:

1.  Initialize an XModelTemplate with a .xmf file. <br>
2.  Generate any number of XModel objects from the XModelTemplate
	object.<br>
3.  Add the XModel objects to the scene.

The XModelTemplate object by itself is not enough to render the model.
It only provides a way to generate model instances.  You must create
at least one model instance from the template to actually start rendering
the model.
*/
//*****************************************************************************
class XModelTemplate 
{
private:
	XSceneGraph						m_nodeTree;
	XModelObjectTemplate**			m_pNodeArray;
	XU32							m_numNodes;
	XU32							m_refCount;
	XParentChildCombo*				m_pParentChildList;
	XPose							m_defaultPose;
	XF32*							m_pLODRangesSquared;
	XU8								m_numLODRanges;
	XU8								m_maxLODRanges;
	XU8								m_nextLODIndexToMerge;
	XBOOL							m_inited;
	XVertexDesc						m_vertexDesc;


	XStatus findLODParts(XModelTemplate& mt,XF32 LODRange);

public:	
	XMemBlockList<XSkinObjectTemplate*> m_skins;
	//*************************************************************************
	//Constructors/Destructors
	//*************************************************************************
	XModelTemplate();
	~XModelTemplate();

	//*************************************************************************
	//Public Interface
	//*************************************************************************
	XStatus init(XS8* pFilename,XU32 maxLODs = 0);
	XStatus initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize,XU32 maxLODs = 0);
	XStatus mergeLOD(XS8* pFilename,XF32 LODRange);
	XStatus mergeLODFromMemory(XS8* pMemoryChunk,XU32 memChunkSize,XF32 LODRange);
	XU8 getLODIndex(float distanceSquared);
	XStatus getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared);
	XStatus setLODRange(XU32 LODIndex,XF32 LODRange);
	XU32 getNumMergedLODs();
	XU32 getMaxLODs();
	XStatus destroy();
	XStatus generateModel(XModel& model,XBOOL instanceMaterials = false);
	XStatus attachShaderController(XS8* pMaterialName,XShaderController* pMatController);
	XHandle getMaterial(XS8* pMaterialName);
	//friend XModel;
	//friend XModelObject;
};



#endif