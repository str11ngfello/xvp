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
//	Module Name:	XModelTemplate.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-22-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

struct XMatMap
{
	XHandle oldMat;
	XHandle newMat;
};

//*****************************************************************************
/**
Constructor for the model template.  
*/
//*****************************************************************************
XModelTemplate::XModelTemplate()
{
	m_refCount = 0;
	m_pLODRangesSquared = NULL;
	m_numLODRanges = 0;
	m_maxLODRanges = 0;
	m_nextLODIndexToMerge = 1;
	m_numNodes = 0;
	m_pNodeArray = NULL;
	m_pParentChildList = NULL;
	m_inited = false;
	
}					  



//*****************************************************************************
/**
Destructor for the model template.
*/
//*****************************************************************************
XModelTemplate::~XModelTemplate()
{
}



//*****************************************************************************
/**
Destroys the model template by freeing all allocated resources.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModelTemplate::destroy()
{
	if (!m_inited)
		return X_OK;

	//Destroy the objects
	XMemBlockList<XSkinObjectTemplate*>::iterator i(m_skins.begin());
	while (i.isValid())
	{
		XDelete(i.getValue());
		i.getNext();
	}
	m_skins.clear();
	for (XU32 i = 0;i < m_numNodes;++i)
	{
		m_pNodeArray[i]->destroy();
		m_pParentChildList[i].destroy();
		XDelete(m_pNodeArray[i])
	}
	XDeleteArray(m_pNodeArray)	
	XDeleteArray(m_pParentChildList)
	m_nodeTree.destroy();
	m_defaultPose.destroy();

	XDelete(m_pLODRangesSquared);

	m_numNodes = 0;
	m_refCount = 0;
	m_pLODRangesSquared = NULL;
	m_numLODRanges = 0;
	m_maxLODRanges = 0;
	m_nextLODIndexToMerge = 1;
	m_inited = false;

	return X_OK;
}



//*****************************************************************************
/**
Initializes the model template with a .xmf file.  This function can be called
more than once.  Each time it is called it flushes any previous template data
so care should be taken to ensure that no model instances are using the 
data before initializing it with new data.

@param	pFilename	The path to the .xmf file to load.  This file must end
					with a .xmf extension or this function will fail.
@return	Returns a status code of type XStatus.
*/
//*****************************************************************************
XStatus XModelTemplate::init(XS8* pFilename,XU32 maxLODs )
{
	//Make sure we have a valid filename and maxLODs
	X_ASSERTM(pFilename,"Invalid filename passed to init");
	X_ASSERTF(XOSMgr::fileExists(pFilename),"Could not find %s",pFilename);

	//Destroy just in case.
	destroy();
	
	XBSChunkHeader inHeader;
	FILE* inFile = fopen(pFilename,"rb");
	
	//Get the file size
	int lSize = XOSMgr::fileGetSize(pFilename);
	
	XBSMaterialChunk commonMaterialChunk;
	XBSModelObjectChunk modelObjectChunk;
	XBSSkinChunk skinChunk;
	XU8 s[XBS_MAX_NAME_LEN] = {'\0'};
	XMatrix4x4 matrix;
	XMemBlockList<XHandle> matHandles;
	XHandle matHandle = X_INVALID_HANDLE;
	XHandle texHandle = X_INVALID_HANDLE;
	XHandle vbHandle = X_INVALID_HANDLE;
	XHandle ibHandle = X_INVALID_HANDLE;
	XMaterial* pMat = NULL;
	XModelObjectTemplate* pNewNode = NULL;
	XSkinObjectTemplate* pNewSkinNode = NULL;
	XSceneGraphNode* pSceneGraphNode = NULL;
	XU32 ID = 0;
	XVertexBuffer* pVB = NULL;
	XIndexBuffer* pIB = NULL;
	XVector4D tempV4d;
	XVector3D tempV3d;
	XVector2D tempV2d;
	XU32 tempXU32;
	XSphere boundingSphere;
	XVector3D* pPositionCache = NULL;
	XVector3D* pNormalCache = NULL;
	XVector2D* pUVCache = NULL;
	XU16* pIndexCache = NULL;


	while (ftell(inFile) < lSize)
	{
		fread(&inHeader,sizeof(inHeader),1,inFile);
	
		switch (inHeader.chunkID)
		{
		case XBS_MODEL:

			//Allocate space for nodes
			XU32 numNodes = 0;
			fread(&numNodes,sizeof(numNodes),1,inFile);
			XNewArray(m_pNodeArray,XModelObjectTemplate*,numNodes,XOSMgr::getModelHeap());
			XNewArray(m_pParentChildList,XParentChildCombo,numNodes,XOSMgr::getModelHeap());
			m_defaultPose.init(numNodes);

			while (ftell(inFile) < lSize)
			{
				fread(&inHeader,sizeof(inHeader),1,inFile);

				switch (inHeader.chunkID)
				{
				case XBS_MATERIAL:
					fread(&commonMaterialChunk,sizeof(commonMaterialChunk),1,inFile);
					XVP::GraphicsMgr.createMaterial(commonMaterialChunk.name,matHandle);
					
					pMat = XVP::GraphicsMgr.getMaterialInterface(matHandle);	
					X_ASSERTM(pMat,"Could not create material for model.");
					matHandles.insertBack(matHandle);
					break;
				case XBS_MODEL_OBJECT:
					fread(&modelObjectChunk,sizeof(modelObjectChunk),1,inFile);

					//Create new model object template and insert it to the tree.  If it's really a bone
					//create a bone object instead
					XNew(pNewNode,XModelObjectTemplate,XOSMgr::getDefaultHeap());
					pNewNode->m_isBone = modelObjectChunk.isBone;
					if (modelObjectChunk.isBone)
						pNewNode->m_invBoneTransform = modelObjectChunk.invBoneTransform;
					pSceneGraphNode = m_nodeTree.createNode(pNewNode,modelObjectChunk.parentName,modelObjectChunk.name);

					//Add objects and increment node count
					m_pNodeArray[m_numNodes] = pNewNode;
					m_pParentChildList[m_numNodes].setChildName(modelObjectChunk.name);
					if (strlen(modelObjectChunk.parentName))
						m_pParentChildList[m_numNodes].setParentName(modelObjectChunk.parentName);
					++m_numNodes;
					
					m_defaultPose.addTransform(modelObjectChunk.modelTransform);
					pNewNode->setHierarchyID(ID);
					++ID;

					//If this is not a bone, fill in geometry information
					if (!modelObjectChunk.isBone)
					{
						for (XU32 i = 0;i < modelObjectChunk.numVertexBuffers;++i)
						{
							X_PRIMITIVE_TYPE pt;
							fread(&pt,sizeof(pt),1,inFile);
							fread(&m_vertexDesc,sizeof(m_vertexDesc),1,inFile);
							
							XS8 materialName[XBS_MAX_NAME_LEN];
							fread(materialName,sizeof(XU8),XBS_MAX_NAME_LEN,inFile);

							XU32 numVerts;
							fread(&numVerts,sizeof(numVerts),1,inFile);

							XVP::GraphicsMgr.createVertexBuffer(m_vertexDesc,pt,numVerts,false,vbHandle);
							pVB = XVP::GraphicsMgr.getVertexBuffer(vbHandle);

							//Position
	#ifdef XVP_EDITOR
							XNewArray(pPositionCache,XVector3D,numVerts,XOSMgr::getDebugHeap());
	#endif
							for (XU32 j = 0;j < numVerts;++j)
							{
								fread(&tempV3d,sizeof(tempV3d),1,inFile);
								pVB->setPositionData(&tempV3d,j*sizeof(XVector3D),sizeof(XVector3D));
	#ifdef XVP_EDITOR
								pPositionCache[j] = tempV3d;
	#endif
							}

							//Normals
							for (XU32 j = 0;j < m_vertexDesc.numNormals;++j)
							{
	#ifdef XVP_EDITOR
#pragma message("Only taking the first layer of normals here.. FIX")
								if (j == 0)
									XNewArray(pNormalCache,XVector3D,numVerts,XOSMgr::getDebugHeap());
	#endif

								for (XU32 k = 0;k < numVerts;++k)
								{
									fread(&tempV3d,sizeof(tempV3d),1,inFile);
									X_ASSERTM(!IsInfinityOrNAN(tempV3d),"Normal Arg!");
									pVB->setNormalData(j,&tempV3d,k*sizeof(XVector3D),sizeof(XVector3D));
	#ifdef XVP_EDITOR
									if (j == 0)
										pNormalCache[k] = tempV3d;
	#endif
								}
							}
							//Colors
							for (XU32 j = 0;j < m_vertexDesc.numColors;++j)
							{
								for (XU32 k = 0;k < numVerts;++k)
								{
									fread(&tempXU32,sizeof(tempXU32),1,inFile);
									pVB->setColorData(j,&tempXU32,k*sizeof(tempXU32),sizeof(tempXU32));
								}
							}
							//UVs
							for (XU32 j = 0;j < m_vertexDesc.numUVs;++j)
							{
	#ifdef XVP_EDITOR
#pragma message("Only taking the first layer of UVs here.. FIX")
								if (j == 0)
									XNewArray(pUVCache,XVector2D,numVerts,XOSMgr::getDebugHeap());
	#endif

								for (XU32 k = 0;k < numVerts;++k)
								{
									fread(&tempV2d,sizeof(tempV2d),1,inFile);
									pVB->setUVData(j,&tempV2d,k*sizeof(tempV2d),sizeof(tempV2d));
	#ifdef XVP_EDITOR
									if (j == 0)
										pUVCache[k] = tempV2d;
	#endif
								}
							}

							//Tangents
							for (XU32 j = 0;j < m_vertexDesc.numTangents;++j)
							{
								for (XU32 k = 0;k < numVerts;++k)
								{
									fread(&tempV3d,sizeof(tempV3d),1,inFile);
									X_ASSERTM(!IsInfinityOrNAN(tempV3d),"Tangent Arg!");
									pVB->setTangentData(j,&tempV3d,k*sizeof(tempV3d),sizeof(tempV3d));
								}
							}
							
							//Binormals
							for (XU32 j = 0;j < m_vertexDesc.numBinormals;++j)
							{
								for (XU32 k = 0;k < numVerts;++k)
								{
									fread(&tempV3d,sizeof(tempV3d),1,inFile);
									X_ASSERTM(!IsInfinityOrNAN(tempV3d),"Binormal Arg!");
									pVB->setBinormalData(j,&tempV3d,k*sizeof(tempV3d),sizeof(tempV3d));
								}
							}			

							XU32 numIndicies;
							fread(&numIndicies,sizeof(numIndicies),1,inFile);
							XVP::GraphicsMgr.createIndexBuffer(numIndicies,false,ibHandle);
							pIB = XVP::GraphicsMgr.getIndexBuffer(ibHandle);
	#ifdef XVP_EDITOR
							XNewArray(pIndexCache,XU16,numIndicies,XOSMgr::getDebugHeap());
	#endif
							for (XU32 j = 0;j < numIndicies;++j)
							{
								XU16 i;
								fread(&i,sizeof(i),1,inFile);

								//pIB[j] =  i;
								pIB->setData(&i,j * sizeof(XU16),sizeof(XU16));
	#ifdef XVP_EDITOR
								pIndexCache[j] = i;
	#endif
							}

							//Finally, add a new geometry object to this model object
							XHandle finalMatHandle = X_INVALID_HANDLE;
							XMemBlockList<XHandle>::iterator m(matHandles.begin());
							while (m.isValid())
							{
								if (strcmp(XVP::GraphicsMgr.getMaterialName(m.getValue()),materialName) == 0)
								{
									finalMatHandle = m.getValue();
									break;
								}
								m.getNext();
							}
							X_DEBUG
							(
								XS8 pBuf[255];
								sprintf(pBuf,"Couldn't find material %s during model load",materialName);
								X_ASSERTM(finalMatHandle != X_INVALID_HANDLE,pBuf);
							)
								
							pNewNode->addGeometry(0,vbHandle,ibHandle,pPositionCache,pNormalCache,pUVCache,pIndexCache,finalMatHandle);
						}
					}
					
					fread(&boundingSphere.center,sizeof(XVector3D),1,inFile);
					fread(&boundingSphere.radius,sizeof(XF32),1,inFile);
					
					pNewNode->setBoundingSphere(boundingSphere);

					break;
				case XBS_SKIN_MESH:
					fread(&skinChunk,sizeof(skinChunk),1,inFile);
					XNew(pNewSkinNode,XSkinObjectTemplate,XOSMgr::getDefaultHeap());
					pNewSkinNode->setName(skinChunk.name);

					for (XU32 i = 0;i < skinChunk.numVertexBuffers;++i)
					{

						X_PRIMITIVE_TYPE pt;
						fread(&pt,sizeof(pt),1,inFile);
						fread(&m_vertexDesc,sizeof(m_vertexDesc),1,inFile);
						
						XS8 materialName[XBS_MAX_NAME_LEN];
						fread(materialName,sizeof(XU8),XBS_MAX_NAME_LEN,inFile);

						XU32 numVerts;
						fread(&numVerts,sizeof(numVerts),1,inFile);
#ifdef XVP_EDITOR
						XNewArray(pPositionCache,XVector3D,numVerts,XOSMgr::getDebugHeap());
#endif
						SkinVert* pSV = new SkinVert[numVerts];
						for (XU32 j = 0;j < numVerts;++j)
							fread(&pSV[j].position,sizeof(pSV[j].position),1,inFile);
						//Normals
						for (XU32 k = 0;k < numVerts;++k)
						{
							fread(&pSV[k].normal,sizeof(pSV[k].normal),1,inFile);
							X_ASSERTM(!IsInfinityOrNAN(pSV[k].normal),"Normal Arg!");
						}
						//Colors
						if (m_vertexDesc.numColors > 0)
						{
							X_ASSERTF(m_vertexDesc.numColors == 1,"More than one vertex color layer not expected")
							for (XU32 k = 0;k < numVerts;++k)
								fread(&pSV[k].color,sizeof(pSV[k].color),1,inFile);
						}
						//UVs
						if (m_vertexDesc.numUVs > 0)
						{
							X_ASSERTF(m_vertexDesc.numUVs == 1,"More than one uv layer not expected")
							for (XU32 k = 0;k < numVerts;++k)
								fread(&pSV[k].uv,sizeof(pSV[k].uv),1,inFile);
						}
						//Tangents
						if (m_vertexDesc.numTangents > 0)
						{
							X_ASSERTF(m_vertexDesc.numTangents == 1,"More than one tangent layer not expected")
							for (XU32 k = 0;k < numVerts;++k)
							{
								fread(&pSV[k].tangent,sizeof(pSV[k].tangent),1,inFile);
								X_ASSERTM(!IsInfinityOrNAN(pSV[k].tangent),"Tangent Arg!");
							}
						}
						//Binormals
						if (m_vertexDesc.numBinormals > 0)
						{
							X_ASSERTF(m_vertexDesc.numBinormals == 1,"More than one binormal layer not expected")
							for (XU32 k = 0;k < numVerts;++k)
							{
								fread(&pSV[k].binormal,sizeof(pSV[k].binormal),1,inFile);
								X_ASSERTM(!IsInfinityOrNAN(pSV[k].binormal),"Binormal Arg!");
							}		
						}

						//Num Bones
						for (XU32 k = 0;k < numVerts;++k)
						{
							fread(&pSV[k].numBones,sizeof(pSV[k].numBones),1,inFile);
						}

						//Bone Weights
						for (XU32 j = 0;j < m_vertexDesc.numBoneWeights4Tuplets;++j)
						{
							for (XU32 k = 0;k < numVerts;++k)
							{
								fread(&pSV[k].boneWeights,sizeof(pSV[k].boneWeights),1,inFile);
								//X_ASSERTM(!IsInfinityOrNAN(pSV[k].boneWeights),"numBoneWeights4Tuplets Arg!");
								//pVB->setBinormalData(j,&tempV3d,k*sizeof(tempV3d),sizeof(tempV3d));
							}
						}		



						//Bone Indices
						for (XU32 j = 0;j < m_vertexDesc.numBoneIndices4Tuplets;++j)
						{
							for (XU32 k = 0;k < numVerts;++k)
							{
								fread(&pSV[k].boneIndices,sizeof(pSV[k].boneIndices),1,inFile);
								//X_ASSERTM(!IsInfinityOrNAN(pSV[k].boneIndices),"numBoneIndices4Tuplets Arg!");
								//pVB->setBinormalData(j,&tempV3d,k*sizeof(tempV3d),sizeof(tempV3d));
							}
						}	

						XU32 numIndicies;
						fread(&numIndicies,sizeof(numIndicies),1,inFile);
						XVP::GraphicsMgr.createIndexBuffer(numIndicies,false,ibHandle);
						pIB = XVP::GraphicsMgr.getIndexBuffer(ibHandle);
#ifdef XVP_EDITOR
						XNewArray(pIndexCache,XU16,numIndicies,XOSMgr::getDebugHeap());
#endif
						for (XU32 j = 0;j < numIndicies;++j)
						{
							XU16 i;
							fread(&i,sizeof(i),1,inFile);

							//pIB[j] =  i;
							pIB->setData(&i,j * sizeof(XU16),sizeof(XU16));
#ifdef XVP_EDITOR
							pIndexCache[j] = i;
#endif
						}

						XHandle finalMatHandle = X_INVALID_HANDLE;
						XMemBlockList<XHandle>::iterator m(matHandles.begin());
						while (m.isValid())
						{
							if (strcmp(XVP::GraphicsMgr.getMaterialName(m.getValue()),materialName) == 0)
							{
								finalMatHandle = m.getValue();
								break;
							}
							m.getNext();
						}
						X_DEBUG
						(
							XS8 pBuf[255];
							sprintf(pBuf,"Couldn't find material %s during model load",materialName);
							X_ASSERTM(finalMatHandle != X_INVALID_HANDLE,pBuf);
						)
							
						pNewSkinNode->addGeometry(0,m_vertexDesc,pt,pSV,ibHandle,finalMatHandle,numVerts,pPositionCache,pIndexCache);
					}
					fread(&boundingSphere.center,sizeof(XVector3D),1,inFile);
					fread(&boundingSphere.radius,sizeof(XF32),1,inFile);
					pNewSkinNode->setBoundingSphere(boundingSphere);
					m_skins.insertBack(pNewSkinNode);
					break;
				default:
					fseek(inFile,inHeader.sizeInBytes,SEEK_CUR);
					break;
				}

			}
			break;
		}

		fseek(inFile,inHeader.sizeInBytes,SEEK_CUR);		
	}

	fclose(inFile);

	matHandles.clear();

	//Make space for LODs
	m_maxLODRanges = maxLODs;
	if (m_maxLODRanges != 0)
		XNewArray(m_pLODRangesSquared,XF32,m_maxLODRanges,XOSMgr::getModelHeap());
	
/*
	//Load LODs if we have some LOD ranges
	if ((pLODRanges) && (numLODRanges != 0))
	{
		//First, clamp the ranges if too many were passed in.
		XU8 clampedLODRanges = 0;
		if (numLODRanges >= X_MAX_MODEL_OBJECT_LODS)
			clampedLODRanges = X_MAX_MODEL_OBJECT_LODS-1;
		else
			clampedLODRanges = numLODRanges;

		//Store the square of the LOD ranges for faster evaluation at run-time
		for (XU32 i = 0;i < clampedLODRanges;++i)
			m_pLODRangesSquared[i] = pLODRanges[i]*pLODRanges[i];
		m_numLODRanges = clampedLODRanges;
		loadLODs(pFilename);
	}
*/

	m_inited = true;

	return X_OK;
}


XStatus XModelTemplate::initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize,XU32 maxLODs)
{
	//Make sure we have a valid mem chunk
	X_ASSERTM(pMemoryChunk,"Invalid mem chunk passed to init");

	XS8* pCurrent = pMemoryChunk;

	//Destroy just in case.
	destroy();
	
	XBSChunkHeader inHeader;
	XBSMaterialChunk commonMaterialChunk;
	XBSModelObjectChunk modelObjectChunk;
	XU8 s[XBS_MAX_NAME_LEN] = {'\0'};
	XMatrix4x4 matrix;
	XMemBlockList<XHandle> matHandles;
	XHandle matHandle = X_INVALID_HANDLE;
	XHandle texHandle = X_INVALID_HANDLE;
	XHandle vbHandle = X_INVALID_HANDLE;
	XHandle ibHandle = X_INVALID_HANDLE;
	XMaterial* pMat = NULL;
	XModelObjectTemplate* newMot = NULL;
	XSceneGraphNode* pSceneGraphNode = NULL;
	XU32 ID = 0;
	XVertexBuffer* pVB = NULL;
	XIndexBuffer* pIB = NULL;
	XVector3D tempV3d;
	XVector2D tempV2d;
	XU32 tempXU32;
	XVertexDesc desc;
	XSphere boundingSphere;
	XVector3D* pPositionCache = NULL;
	XU16* pIndexCache = NULL;

	while (XU32(pCurrent - pMemoryChunk) < memChunkSize)
	{
		inHeader = *(XBSChunkHeader*)pCurrent; pCurrent += sizeof(inHeader);
		//fread(&inHeader,sizeof(inHeader),1,inFile);
	
		switch (inHeader.chunkID)
		{
		case XBS_MODEL:

			//Allocate space for nodes
			XU32 numNodes = 0;
			numNodes = *(XU32*)pCurrent; pCurrent += sizeof(numNodes);
			//fread(&numNodes,sizeof(numNodes),1,inFile);
			XNewArray(m_pNodeArray,XModelObjectTemplate*,numNodes,XOSMgr::getModelHeap());
			XNewArray(m_pParentChildList,XParentChildCombo,numNodes,XOSMgr::getModelHeap());
			m_defaultPose.init(numNodes);

			while (XU32(pCurrent - pMemoryChunk) < memChunkSize)
			{
				inHeader = *(XBSChunkHeader*)pCurrent; pCurrent += sizeof(inHeader);
				//fread(&inHeader,sizeof(inHeader),1,inFile);

				switch (inHeader.chunkID)
				{
				case XBS_MATERIAL:
					commonMaterialChunk = *(XBSMaterialChunk*)pCurrent; pCurrent += sizeof(commonMaterialChunk);
					//fread(&commonMaterialChunk,sizeof(commonMaterialChunk),1,inFile);
					//XVP::GraphicsMgr.createMaterial(commonMaterialChunk.name,matHandle);
					matHandle = XVP::GraphicsMgr.getMaterial(commonMaterialChunk.name);
					
					pMat = XVP::GraphicsMgr.getMaterialInterface(matHandle);	
					X_ASSERTM(pMat,"Could not find model material.  Expected material to already exist.");
					
					//for (XU32 i = 0;i < commonMaterialChunk.numTextures;++i)
					//{
						//XVP::GraphicsMgr.createTexture(commonMaterialChunk.diffuseMapName,X_TT_2D,texHandle);
						//pMat->setTextureHandle(0,texHandle);
						//XVP::GraphicsMgr.destroyTexture(texHandle);
					//}

					//pMat->m_diffuse = commonMaterialChunk.diffuseColor;
					//pMat->m_specular = commonMaterialChunk.specularColor;
					//pMat->m_ambient = commonMaterialChunk.ambientColor;
					//pMat->m_specPower = commonMaterialChunk.shininess;

					matHandles.insertBack(matHandle);

					break;
				case XBS_MODEL_OBJECT:
					modelObjectChunk = *(XBSModelObjectChunk*)pCurrent; pCurrent += sizeof(modelObjectChunk);
					//fread(&modelObjectChunk,sizeof(modelObjectChunk),1,inFile);

					//Create new model object template and insert it to the tree
					XNew(newMot,XModelObjectTemplate,XOSMgr::getDefaultHeap());
					pSceneGraphNode = m_nodeTree.createNode(newMot,modelObjectChunk.parentName,modelObjectChunk.name);

					//Add objects and increment node count
					m_pNodeArray[m_numNodes] = newMot;
					m_pParentChildList[m_numNodes].setChildName(modelObjectChunk.name);
					if (strlen(modelObjectChunk.parentName))
						m_pParentChildList[m_numNodes].setParentName(modelObjectChunk.parentName);
					++m_numNodes;
					
					m_defaultPose.addTransform(modelObjectChunk.modelTransform);
					newMot->setHierarchyID(ID);
					++ID;

					for (XU32 i = 0;i < modelObjectChunk.numVertexBuffers;++i)
					{
						X_PRIMITIVE_TYPE pt;
						pt = *(X_PRIMITIVE_TYPE*)pCurrent; pCurrent += sizeof(pt);
						//fread(&pt,sizeof(pt),1,inFile);

						desc = *(XVertexDesc*)pCurrent; pCurrent += sizeof(desc);
						//fread(&desc,sizeof(desc),1,inFile);
						
						XS8 materialName[XBS_MAX_NAME_LEN];
						strncpy(materialName,pCurrent,XBS_MAX_NAME_LEN);pCurrent += XBS_MAX_NAME_LEN;
						//fread(materialName,sizeof(XU8),XBS_MAX_NAME_LEN,inFile);

						XU32 numVerts;
						numVerts = *(XU32*)pCurrent; pCurrent += sizeof(numVerts);
						//fread(&numVerts,sizeof(numVerts),1,inFile);

						XVP::GraphicsMgr.createVertexBuffer(desc,pt,numVerts,false,vbHandle);
						//pVB = XVP::GraphicsMgr.lockVertexBuffer(vbHandle,X_LT_WRITE_ONLY);
						pVB = XVP::GraphicsMgr.getVertexBuffer(vbHandle);

						//Position
						/*for (XU32 j = 0;j < numVerts;++j)
						{
							tempV3d = *(XVector3D*)pCurrent; pCurrent += sizeof(tempV3d);
							//fread(&tempV3d,sizeof(tempV3d),1,inFile);
							pVB->setPosition(j,tempV3d);
						}*/
						
						pVB->setPositionData(pCurrent,0,numVerts * sizeof(XVector3D));
#ifdef XVP_EDITOR
						XNewArray(pPositionCache,XVector3D,numVerts,XOSMgr::getDebugHeap());
						memcpy(pPositionCache,pCurrent,sizeof(XVector3D) * numVerts);
#endif
						pCurrent += numVerts * sizeof(XVector3D);

						//Normals
						/*for (XU32 j = 0;j < numVerts;++j)
						{
							tempV3d = *(XVector3D*)pCurrent; pCurrent += sizeof(tempV3d);
							//fread(&tempV3d,sizeof(tempV3d),1,inFile);
							pVB->setNormal(j,0,tempV3d);
						}*/
						for (XU32 j = 0;j < desc.numNormals;++j)
						{
							pVB->setNormalData(j,pCurrent,0,numVerts * sizeof(XVector3D));
							pCurrent += numVerts * sizeof(XVector3D);
						}

						//Colors
						/*for (XU32 j = 0;j < numVerts;++j)
						{
							tempXU32 = *(XU32*)pCurrent; pCurrent += sizeof(tempXU32);
							//fread(&tempXU32,sizeof(tempXU32),1,inFile);
							pVB->setColor(j,0,tempXU32);
						}*/
						for (XU32 j = 0;j < desc.numColors;++j)
						{
							pVB->setColorData(j,pCurrent,0,numVerts * sizeof(tempXU32));
							pCurrent += numVerts * sizeof(tempXU32);
						}

						//UVs
						/*for (XU32 j = 0;j < desc.numUVs;++j)
						{
							for (XU32 k = 0;k < numVerts;++k)
							{
								tempV2d = *(XVector2D*)pCurrent; pCurrent += sizeof(tempV2d);
								//fread(&tempV2d,sizeof(tempV2d),1,inFile);
								pVB->setUV(k,j,tempV2d);
							}
						}*/
						for (XU32 j = 0;j < desc.numUVs;++j)
						{
							pVB->setUVData(j,pCurrent,0,numVerts * sizeof(XVector2D));
							pCurrent += numVerts * sizeof(XVector2D);
						}

						//Tangents
						/*for (XU32 j = 0;j < desc.numTangents;++j)
						{
							for (XU32 k = 0;k < numVerts;++k)
							{
								tempV3d = *(XVector3D*)pCurrent; pCurrent += sizeof(tempV3d);
								//fread(&tempV3d,sizeof(tempV3d),1,inFile);
								pVB->setTangent(k,j,tempV3d);
							}
						}*/
						for (XU32 j = 0;j < desc.numTangents;++j)
						{
							pVB->setTangentData(j,pCurrent,0,numVerts * sizeof(XVector3D));
							pCurrent += numVerts * sizeof(XVector3D);
						}
						
						//Binormals
						/*for (XU32 j = 0;j < desc.numBinormals;++j)
						{
							for (XU32 k = 0;k < numVerts;++k)
							{
								tempV3d = *(XVector3D*)pCurrent; pCurrent += sizeof(tempV3d);
								//fread(&tempV3d,sizeof(tempV3d),1,inFile);
								pVB->setBinormal(k,j,tempV3d);
							}
						}*/
						for (XU32 j = 0;j < desc.numBinormals;++j)
						{
							pVB->setBinormalData(j,pCurrent,0,numVerts * sizeof(XVector3D));
							pCurrent += numVerts * sizeof(XVector3D);
						}

						//Tangents
						//for (XU32 j = 0;j < numVerts;++j)
						//	fwrite(&vertexBuffer[j].tangent,sizeof(vertexBuffer[j].tangent),1,outFile);
						//Binormals
						//for (XU32 j = 0;j < numVerts;++j)
						//	fwrite(&vertexBuffer[j].binormal,sizeof(vertexBuffer[j].binormal),1,outFile);
	
						
						
						
						/*for (XU32 j = 0;j < numVerts;++j)
						{
							XModelVertex v;
							fread(&v,sizeof(v),1,inFile);

							//pVB[j] = v;
							pVB->setPosition(j,0,v.pos);
							pVB->setNormal(j,0,v.normal);
							pVB->setColor(j,0,v.diffuse);
							pVB->setUV(j,0,v.texCoords);
						}*/

						XU32 numIndicies;
						numIndicies = *(XU32*)pCurrent; pCurrent += sizeof(numIndicies);
						//fread(&numIndicies,sizeof(numIndicies),1,inFile);
						XVP::GraphicsMgr.createIndexBuffer(numIndicies,false,ibHandle);
						//pIB = XVP::GraphicsMgr.lockIndexBuffer(ibHandle);
						pIB = XVP::GraphicsMgr.getIndexBuffer(ibHandle);
						pIB->setData(pCurrent,0,sizeof(XU16) * numIndicies);

#ifdef XVP_EDITOR
						XNewArray(pIndexCache,XU16,numIndicies,XOSMgr::getDebugHeap());
						memcpy(pIndexCache,pCurrent,sizeof(XU16) * numIndicies);
#endif

						pCurrent += sizeof(XU16) * numIndicies;
						/*for (XU32 j = 0;j < numIndicies;++j)
						{
							XU16 i;
							i = *(XU16*)pCurrent; pCurrent += sizeof(i);
							//fread(&i,sizeof(i),1,inFile);

							pIB[j] =  i;
						}*/
						

						/*XSphere sphere;
						XIGenBoundingSphere(sphere,numVerts,pVB->getPositionStream(),sizeof(XVector3D));
						if (i > 0)
							XISphereEnclosingSpheres(lastSphere,lastSphere,sphere);
						else
							lastSphere = sphere;
						XIGenBoundingAABB(newMot->m_aabb,numVerts,pVB->getPositionStream(),sizeof(XVector3D));*/

						//Release vertex buffer and index buffer pointers.
						//XVP::GraphicsMgr.unlockVertexBuffer(vbHandle);
						//XVP::GraphicsMgr.unlockIndexBuffer(ibHandle);

						//Finally, add a new geometry object to this model object
						XHandle finalMatHandle = X_INVALID_HANDLE;
						XMemBlockList<XHandle>::iterator m(matHandles.begin());
						while (m.isValid())
						{
							if (strcmp(XVP::GraphicsMgr.getMaterialName(m.getValue()),materialName) == 0)
							{
								finalMatHandle = m.getValue();
								break;
							}
							m.getNext();
						}
						X_ASSERTM(finalMatHandle != X_INVALID_HANDLE,"Couldn't find material during model load");
							
						newMot->addGeometry(0,vbHandle,ibHandle,pPositionCache,NULL,NULL,pIndexCache,finalMatHandle);
					}

					boundingSphere.center = *(XVector3D*)pCurrent; pCurrent += sizeof(boundingSphere.center);
					boundingSphere.radius = *(XF32*)pCurrent; pCurrent += sizeof(boundingSphere.radius);
					newMot->setBoundingSphere(boundingSphere);

					break;
				default:
					pCurrent += inHeader.sizeInBytes;
					//fseek(inFile,inHeader.sizeInBytes,SEEK_CUR);
					break;
				}

			}
			break;
		}

		pCurrent += inHeader.sizeInBytes;
		//fseek(inFile,inHeader.sizeInBytes,SEEK_CUR);		
	}

	//fclose(inFile);

	matHandles.clear();

	//Make space for LODs
	m_maxLODRanges = maxLODs;
	if (m_maxLODRanges != 0)
		XNewArray(m_pLODRangesSquared,XF32,m_maxLODRanges,XOSMgr::getModelHeap());
	
/*
	//Load LODs if we have some LOD ranges
	if ((pLODRanges) && (numLODRanges != 0))
	{
		//First, clamp the ranges if too many were passed in.
		XU8 clampedLODRanges = 0;
		if (numLODRanges >= X_MAX_MODEL_OBJECT_LODS)
			clampedLODRanges = X_MAX_MODEL_OBJECT_LODS-1;
		else
			clampedLODRanges = numLODRanges;

		//Store the square of the LOD ranges for faster evaluation at run-time
		for (XU32 i = 0;i < clampedLODRanges;++i)
			m_pLODRangesSquared[i] = pLODRanges[i]*pLODRanges[i];
		m_numLODRanges = clampedLODRanges;
		loadLODs(pFilename);
	}
*/

	m_inited = true;

	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XModelTemplate::mergeLOD(XS8* pFilename,XF32 LODRange)
{
	X_ASSERTM(pFilename,"Invalid filename passed to mergeLOD()");
	X_ASSERTM(getNumMergedLODs() < m_maxLODRanges,"max LODs prevented new LOD merge");

	XModelTemplate mt;
	mt.init(pFilename,0);

	return findLODParts(mt,LODRange);
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelTemplate::mergeLODFromMemory(XS8* pMemoryChunk,XU32 memChunkSize,XF32 LODRange)
{
	X_ASSERTM(pMemoryChunk,"Invalid memory chunk passed to mergeLOD()");
	X_ASSERTM(getNumMergedLODs() < m_maxLODRanges,"max LODs prevented new LOD merge");

	XModelTemplate mt;
	mt.initFromMemory(pMemoryChunk,memChunkSize);

	return findLODParts(mt,LODRange);
}


//*****************************************************************************
//*****************************************************************************
XStatus XModelTemplate::findLODParts(XModelTemplate& mt,XF32 LODRange)
{
	XMemBlockList<XSceneGraphNode*> nodeList;
	XSceneGraphNode* pNode;
	XModelObjectTemplate* pMot;
	XModelObjectTemplate* pLODMot;
	XBOOL LODfound = false;

	mt.m_nodeTree.getNodes(NULL,nodeList);
	XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
	while (i.isValid())
	{
		pNode = m_nodeTree.getNode(i.getValue()->m_pName);
		if (pNode)
		{
			pMot = (XModelObjectTemplate*)pNode->m_pSceneNode;
			pLODMot = (XModelObjectTemplate*)i.getValue()->m_pSceneNode;

			//We need to bump the ref counts for each resource we plan on keeping after
			//we destroy the lod template.  The LOD template must have at least one LOD.
			vector<XGeometry*> geomList;
			pLODMot->getGeometryList(0,geomList);
			for (XU32 k = 0;k < geomList.size();++k)
			{
				XVP::GraphicsMgr.addVertexBufferRef(geomList[k]->m_vbHandle);
				XVP::GraphicsMgr.addIndexBufferRef(geomList[k]->m_ibHandle);
				

				//If the material name does not match the base LOD, then use new material, otherwise
				//use the base lod material
				XHandle finalMat = X_INVALID_HANDLE;
				XMaterial* pOrgMat = XVP::GraphicsMgr.getMaterialInterface(pMot->m_pGeometryLists[0][k]->m_matHandle);
				XMaterial* pNewMat = XVP::GraphicsMgr.getMaterialInterface(geomList[k]->m_matHandle);
				if (strcmp(pOrgMat->getName(),pNewMat->getName()) == 0)
				{
					XVP::GraphicsMgr.addMaterialRef(pMot->m_pGeometryLists[0][k]->m_matHandle);
					finalMat = pMot->m_pGeometryLists[0][k]->m_matHandle;
				}
				else
				{
					XVP::GraphicsMgr.addMaterialRef(geomList[k]->m_matHandle);
					finalMat = geomList[k]->m_matHandle;
				}
				
				LODfound = true;
				pMot->addGeometry(m_nextLODIndexToMerge,geomList[k]->m_vbHandle,geomList[k]->m_ibHandle,NULL,NULL,NULL,NULL,finalMat/*geomList[k]->m_matHandle*/);
			}
			geomList.clear();
		}
		i.getNext();
	}
	
	nodeList.clear();
	mt.destroy();

	if (LODfound)
	{ 
		m_pLODRangesSquared[m_numLODRanges++] = LODRange*LODRange;
		++m_nextLODIndexToMerge;
		return X_OK;
	}

	return X_ERR_NO_EFFECT;
}



//*****************************************************************************
//*****************************************************************************
XU32 XModelTemplate::getNumMergedLODs()
{
	return m_numLODRanges;
}



//*****************************************************************************
//*****************************************************************************
XU32 XModelTemplate::getMaxLODs()
{
	return m_maxLODRanges;
}


/*
//*****************************************************************************
//*****************************************************************************
XStatus XModelTemplate::loadLODs(XS8* pFilename)
{
	XModelTemplate mt;
	XMemBlockList<XSceneGraphNode*> nodeList;
	XSceneGraphNode* pNode;
	XModelObjectTemplate* pMot;
	XModelObjectTemplate* pLODMot;
	char pLODFilename[X_MAX_PATH_SIZE];
	char pBaseLODFilename[X_MAX_PATH_SIZE];

	//Build a base filename without extension.
	strcpy(pBaseLODFilename,pFilename);
	XS8* pExt = strrchr(pBaseLODFilename,'.');
	if (pExt)
		*pExt = '\0';

	for (XU32 j = 1;j <= m_numLODRanges;++j)
	{
		//Build the next filename and see if it exists.
		sprintf(pLODFilename,"%s_lod%d.xmf",pBaseLODFilename,j);
		
		//See if the file exists, if not bail and don't look any
		//further.
		if (!XOSMgr::fileExists(pLODFilename))
			break;

		mt.init(pLODFilename,0,NULL);
		mt.m_nodeTree.getNodes(NULL,nodeList);
		XMemBlockList<XSceneGraphNode*>::iterator i(nodeList.begin());
		while (i.isValid())
		{
			pNode = m_nodeTree.getNode(i.getValue()->m_pName);
			if (pNode)
			{
				pMot = (XModelObjectTemplate*)pNode->m_pSceneNode;
				pLODMot = (XModelObjectTemplate*)i.getValue()->m_pSceneNode;

				//We need to bump the ref counts for each resource we plan on keeping after
				//we destroy the lod template.  The LOD template must have at least one LOD.
				vector<XGeometry*> geomList;
				pLODMot->getGeometryList(0,geomList);
				for (XU32 k = 0;k < geomList.size();++k)
				{
					XVP::GraphicsMgr.addVertexBufferRef(geomList[k]->m_vbHandle);
					XVP::GraphicsMgr.addIndexBufferRef(geomList[k]->m_ibHandle);
					XVP::GraphicsMgr.addMaterialRef(geomList[k]->m_matHandle);
					
					pMot->addGeometry(j,geomList[k]->m_vbHandle,geomList[k]->m_ibHandle,geomList[k]->m_matHandle);
				}
				geomList.clear();
			}
			i.getNext();
		}
		

		nodeList.clear();
		mt.destroy();
	}

	return X_OK;
	
}
*/

//*****************************************************************************
//*****************************************************************************
/*XU8 XModelTemplate::getLODIndex(float distanceSquared)
{
	for (XS32 i = m_numLODRanges-1;i >= 0;--i)
	{
		if (distanceSquared >= m_pLODRangesSquared[i])
			return i+1;
	}

	return 0;
}*/


//*****************************************************************************
//*****************************************************************************
XStatus XModelTemplate::getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared)
{
	numLODRanges = m_numLODRanges;
	(*pLODRangesSquared) = m_pLODRangesSquared;
	return X_OK;
}

XStatus XModelTemplate::setLODRange(XU32 LODIndex,XF32 LODRange)
{
	X_ASSERTM(LODIndex < m_maxLODRanges,"LOD index out of range");
	m_pLODRangesSquared[LODIndex] = LODRange * LODRange;
	return X_OK;
}

//*****************************************************************************
/**
Generates a model instance from this model template.  This function can be
used to generate any number of model instances from this template.  Before this
function can be called however, the model template has to initialized with
a call to init().

It's important to note that as long as the model instances that are created
from this template exist, the template must also exist.  If a template, that
was used to create instances, goes out of scope while instances still exist,
the application will become unstable.  Therefore, it's important to know when
it's safe to deallocate the template.  A good rule of thumb is to destoy all 
instances of the object before destroying the template used to create them.

@param	model		An XModel object to hold the newly generated model.
@return	Returns a status code of type XStatus.
*/
//*****************************************************************************
XStatus XModelTemplate::generateModel(XModel& model,XBOOL instanceMaterials)
{
	//Re-initialize the model in case it contains data.
	model.destroy();
	model.init();

	XModelObjectTemplate* pOriginalModelObject;

	XMemBlockList<XMatMap> uniqueMaterials;

	//First we need to create the appropriate nodes in the new tree.
	//The parent/child names for creating the new tree can be found in 
	//the modelObjectTree's m_parentChildList.  We will use these to create
	//the new nodes.
	XNewArray(model.m_pNodeArray,XSceneNode*,m_numNodes,XOSMgr::getModelHeap())
	for (XU32 i = 0;i < m_numNodes;i++)
	{
		//Find the node in the template model tree we want to create for the instance
		pOriginalModelObject = (XModelObjectTemplate*)(m_nodeTree.getNode(m_pParentChildList[i].getChildName()))->m_pSceneNode;

		//This could be a model object or a bone
		if (pOriginalModelObject->m_isBone)
		{
			//Insert a new node in to the instance tree
			XBone* pBone = NULL;
			XNew(pBone,XBone,XOSMgr::getModelHeap());
			model.m_pNodeArray[i] = pBone;
			model.m_nodeTree.createNode(pBone,m_pParentChildList[i].getParentName(),m_pParentChildList[i].getChildName());

			//Copy the relevant data from template to instance
			pBone->setHierarchyID(pOriginalModelObject->getHierarchyID());
			pBone->setModelTransform(pOriginalModelObject->getModelTransform());
			pBone->setInvBoneTransform(pOriginalModelObject->m_invBoneTransform);
			pBone->setBoundingSphere(pOriginalModelObject->getBoundingSphere());
			//pBone->m_aabb = pOriginalModelObject->m_aabb;
			//pBone->m_modelSpaceAABB = pOriginalModelObject->m_aabb;
			//pBone->setBoundingSphere(pOriginalModelObject->getBoundingSphere());
			pBone->m_pParentModel = &model;
			pBone->m_pDataTemplate = pOriginalModelObject;

			//Save this new bone's pointer to the m_boneIDIndexArray in order to provide
			//quick and easy access to it for skinning during rendering.
			//model.m_boneIDIndexArray[pBone->getHierarchyID()] = pNewBone;   
		}
		else
		{
			//Insert a new node in to the instance tree
			XModelObject* pMo = NULL;
			XNew(pMo,XModelObject,XOSMgr::getModelHeap());
			model.m_pNodeArray[i] = pMo;
			model.m_nodeTree.createNode(pMo,m_pParentChildList[i].getParentName(),m_pParentChildList[i].getChildName());

			//Copy the relevant data from template to instance
			pMo->setHierarchyID(pOriginalModelObject->getHierarchyID());
			pMo->setModelTransform(pOriginalModelObject->getModelTransform());
			pMo->setBoundingSphere(pOriginalModelObject->getBoundingSphere());
			//pMo->m_aabb = pOriginalModelObject->m_aabb;
			//pMo->m_modelSpaceAABB = pOriginalModelObject->m_aabb;
			//pMo->setBoundingSphere(pOriginalModelObject->getBoundingSphere());
			pMo->m_pParentModel = &model;
			pMo->m_pDataTemplate = pOriginalModelObject;

			//Create unique materials if needed
			if (instanceMaterials)
			{
				XHandle** ppMats = NULL;
				XNewArray(ppMats,XHandle*,pOriginalModelObject->getNumLODs(),XOSMgr::getModelHeap())
				vector<XGeometry*> geomList;
				for (XU32 i = 0;i < pOriginalModelObject->getNumLODs();++i)
				{
					pOriginalModelObject->getGeometryList(i,geomList);
					XNewArray(ppMats[i],XHandle,geomList.size(),XOSMgr::getModelHeap());
					for (XU32 j = 0;j < geomList.size();++j)
					{
						//Clone material if and only if we haven't already cloned it for
						//this model object.  Materals should still be shared in the new model
						//just not shared with the model template
						XMemBlockList<XMatMap>::iterator itr(uniqueMaterials.begin());
						XHandle foundMat = X_INVALID_HANDLE;
						while (itr.isValid())
						{
							if (itr.getValue().oldMat == geomList[j]->m_matHandle)
							{
								foundMat = itr.getValue().newMat;
								break;
							}
							itr.getNext();
						}
						if (foundMat == X_INVALID_HANDLE)
						{
							XVP::GraphicsMgr.cloneMaterial(geomList[j]->m_matHandle,ppMats[i][j]);
							XMatMap mm;
							mm.oldMat = geomList[j]->m_matHandle;
							mm.newMat = ppMats[i][j];
							uniqueMaterials.insertBack(mm);
						}
						else
							ppMats[i][j] = foundMat;
					}
				}
				pMo->m_ppInstanceMats = ppMats;
			}
		}
	}
	
	uniqueMaterials.clear();

	model.m_numNodes = m_numNodes;
	model.m_pDataTemplate = this;
	model.setPose(&m_defaultPose);
	

	XMemBlockList<XSkinObjectTemplate*>::iterator i(m_skins.begin());
	while (i.isValid())
	{
		//Insert a new skin into the model
		XSkinObject* pSkin = NULL;
		XNew(pSkin,XSkinObject,XOSMgr::getModelHeap());

		//Copy the relevant data from template to instance,
		//We are only supporting one LOD for skins at the moment
		//XHandle vbHandle;
		pSkin->setName(i.getValue()->getName());
		vector<XSkinGeometry*> geometryList;
		i.getValue()->getGeometryList(0,geometryList);
		pSkin->setGeometryList(geometryList);
		//XVP::GraphicsMgr.createVertexBuffer(m_vertexDesc,X_PT_TRIANGLE_STRIP,i.getValue()->getNumVerts(),false,vbHandle);
		//pSkin->setSkinnedVertexBuffer(vbHandle);
		pSkin->setHierarchyID(0);
		pSkin->setModelTransform(i.getValue()->getModelTransform());
		pSkin->setBoundingSphere(i.getValue()->getBoundingSphere());
		//pSkin->m_aabb = pOriginalModelObject->m_aabb;
		//pSkin->m_modelSpaceAABB = pOriginalModelObject->m_aabb;
		//pSkin->setBoundingSphere(pOriginalModelObject->getBoundingSphere());
		pSkin->m_pParentModel = &model;
		pSkin->m_pDataTemplate = i.getValue();

		//Create unique materials if needed
		if (instanceMaterials)
		{
		}

		model.m_skins.insertBack(pSkin);
		i.getNext();
	}

	++m_refCount;
	
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelTemplate::attachShaderController(XS8* pMaterialName,
												 XShaderController* pMatController)
{
	vector<XGeometry*> geomList;
	for (XU32 i = 0;i < m_numNodes;++i)
	{
		for (XU32 j = 0;j < m_pNodeArray[i]->getNumLODs();++j)
		{
			m_pNodeArray[i]->getGeometryList(j,geomList);
			for (XU32 k = 0;k < geomList.size();++k)
			{
				//If we are looking for a specific material name
				if (pMaterialName)
				{
					if (strcmp(XVP::GraphicsMgr.getMaterialName(geomList[k]->m_matHandle),pMaterialName) == 0)
						XVP::GraphicsMgr.getMaterialInterface(geomList[k]->m_matHandle)->attachShaderController(pMatController);
				}
				//else we are applying the controller to all materials
				else
					XVP::GraphicsMgr.getMaterialInterface(geomList[k]->m_matHandle)->attachShaderController(pMatController);	
			}
			geomList.clear();
		}
	}

	/*if (!pMaterialName)
	{
		XMemBlockList<XSkinObjectTemplate*>::iterator i(m_skins.begin());
		while (i.isValid())
		{
			XVP::GraphicsMgr.getMaterialInterface(i.getValue()->getMaterial())->attachShaderController(pMatController);	
			i.getNext();
		}
	}*/

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XHandle XModelTemplate::getMaterial(XS8* pMaterialName)
{
	X_ASSERTM(pMaterialName,"Material name cannot be null");

	vector<XGeometry*> geomList;
	for (XU32 i = 0;i < m_numNodes;++i)
	{
		for (XU32 j = 0;j < m_pNodeArray[i]->getNumLODs();++j)
		{
			m_pNodeArray[i]->getGeometryList(j,geomList);
			for (XU32 k = 0;k < geomList.size();++k)
			{
				if (strcmp(XVP::GraphicsMgr.getMaterialName(geomList[k]->m_matHandle),pMaterialName) == 0)
					return geomList[k]->m_matHandle;
			}
			geomList.clear();
		}
	}

	//Look through skins
	XMemBlockList<XSkinObjectTemplate*>::iterator i(m_skins.begin());
	while (i.isValid())
	{
		vector<XSkinGeometry*> geomList;
		i.getValue()->getGeometryList(0,geomList);
		for (XU32 j = 0;j < geomList.size();++j)
		{
			XHandle matHandle = geomList[j]->m_matHandle;
			XS8* pMatName = XVP::GraphicsMgr.getMaterialName(matHandle);
			if (pMatName)
				if (strcmp(pMatName,pMaterialName) == 0)
					return matHandle;
		}
		/*vector<XSkinGeometry*>::iterator j(geomList.begin());
		while (j.isValid())
		{
			XHandle matHandle = j.getValue()->m_matHandle;
			XS8* pMatName = XVP::GraphicsMgr.getMaterialName(matHandle);
			if (pMatName)
				if (strcmp(pMatName,pMaterialName) == 0)
					return matHandle;

			j.getNext();
		}
		i.getNext();
		*/
	}

	return X_INVALID_HANDLE;
}