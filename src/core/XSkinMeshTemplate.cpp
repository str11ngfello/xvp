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
//	Module Name:	XSkinMeshTemplate.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

XBOOL XSkinMeshTemplate::m_determineCaps = true;


//*****************************************************************************
/**
Constructor for the skin mesh template.  
*/
//*****************************************************************************
XSkinMeshTemplate::XSkinMeshTemplate()
{
	m_refCount = 0;
	//m_vbHandle = X_INVALID_HANDLE;
	//m_matHandle = X_INVALID_HANDLE;
	//m_ibHandle = X_INVALID_HANDLE;
	//m_renderVBHandle = X_INVALID_HANDLE;
	//m_matHandle = X_INVALID_HANDLE;
	m_numVerts = 0;
	m_inited = false;
}



//*****************************************************************************
/**
Destructor for the skin mesh template.  
*/
//*****************************************************************************
XSkinMeshTemplate::~XSkinMeshTemplate()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinMeshTemplate::destroy()
{
	if (!m_inited)
		return X_OK;

	//Destroy resources.
	m_geometry.destroy();
	//XVP::GraphicsMgr.destroyVertexBuffer(m_renderVBHandle);
	//m_renderVBHandle = X_INVALID_HANDLE;
	//XVP::GraphicsMgr.destroyIndexBuffer(m_ibHandle);
	//m_ibHandle = X_INVALID_HANDLE;
	//XVP::destroyMaterial(m_matHandle);
	//m_matHandle = X_INVALID_HANDLE;

	//Destroy the bones.
	for (XU32 i = 0;i < m_nodeArray.size();i++)
	{
		m_nodeArray[i]->destroy();
		XDelete(m_nodeArray[i])
	}
	m_nodeTree.destroy();
	m_nodeArray.~vector();

	for (XU32 i = 0;i < m_parentChildList.size();i++)
		m_parentChildList[i].destroy();
	m_parentChildList.clear();

	m_defaultPose.destroy();
	m_inited = false;
	return X_OK;
}



//*****************************************************************************
/**
Initializes the skin mesh with a .xsm file.  This function can be called
more than once.  Each time it is called it flushes any previous template data
so care should be taken to ensure that no skin mesh instances are using the 
data before initializing it with new data.

@param	pFilename	The path to the .xsm file to load.  This file must end
					with a .xsm extension or this function will fail.
@return	Returns a status code of type XStatus.
*/
//*****************************************************************************
XStatus XSkinMeshTemplate::init(XS8* pFilename)
{
	//Let's see if the hardware supports skinning if this is the first skin
	//mesh to be loaded.
	/*if (m_determineCaps)
	{
		if (!(XVP::GraphicsMgr.getMaxVertexShaderVersion() >= X_VSV_2_0))
		{
			X_ASSERT_FORCED("Vertex shader version 2.0 or higher is required for Skin Mesh support");
			return X_ERR_NOT_SUPPORTED;
		}
	}*/

	//Initialize the skin mesh
	destroy();
	XSkinMeshTemplate();


	//This reference count only goes up when the user creates
	//a skin mesh instance that references this template.
	m_refCount = 0;








	XBSChunkHeader inHeader;
	FILE* inFile = fopen(pFilename,"rb");
	
	//Get the file size
	int lSize = XOSMgr::fileGetSize(pFilename);
	
	XBSMaterialChunk commonMaterialChunk;
	XBSModelObjectChunk modelObjectChunk;
	XBSBoneChunk boneChunk;
	//XU8 s[XBS_MAX_NAME_LEN];
	XMatrix4x4 matrix;
	XHandle matHandle = X_INVALID_HANDLE;
	XHandle texHandle = X_INVALID_HANDLE;
	XHandle vbHandle = X_INVALID_HANDLE;
	XHandle ibHandle = X_INVALID_HANDLE;
	XMaterial* pMat = NULL;
	XParentChildCombo pcCombo;
	XSceneGraphNode* pSceneGraphNode = NULL;
	XU32 ID = 0;
	XVertexBuffer* pVB = NULL;
	WORD* pIB = NULL;
	XBone* pNewBone = NULL;
	XU32 numBones = 0;
	XU32 numVerts = 0;
	
	XVertexDesc desc;
	desc.numNormals = 1;
	desc.numUVs = 2;
	desc.numColors = 1;
	desc.numBoneIndices4Tuplets = 1;
	desc.numBoneWeights4Tuplets = 1;

	while (ftell(inFile) < lSize)
	{
		fread(&inHeader,sizeof(inHeader),1,inFile);
			
			XU32 numTotalBones = 0;
				fread(&numTotalBones,sizeof(numTotalBones),1,inFile);
				m_defaultPose.init(numTotalBones);

		switch (inHeader.chunkID)
		{
		case XBS_SKIN_MESH:
			while (ftell(inFile) < lSize)
			{
				fread(&inHeader,sizeof(inHeader),1,inFile);
				
			

				switch (inHeader.chunkID)
				{
				case XBS_MATERIAL:
					fread(&commonMaterialChunk,sizeof(commonMaterialChunk),1,inFile);
					
					XVP::GraphicsMgr.createMaterial(commonMaterialChunk.name,matHandle);
					
					pMat = XVP::GraphicsMgr.getMaterialInterface(matHandle);					
					X_ASSERTM(pMat,"Could not create material for skin mesh.");
					
					/*if (strlen(commonMaterialChunk.diffuseMapName))
					{
						XVP::GraphicsMgr.createTexture(commonMaterialChunk.diffuseMapName,X_TT_2D,texHandle);
						pMat->setTextureHandle(0,texHandle);
						XVP::GraphicsMgr.destroyTexture(texHandle);
					}	*/			

					//pMat->m_diffuse = commonMaterialChunk.diffuseColor;
					//pMat->m_specular = commonMaterialChunk.specularColor;
					//pMat->m_ambient = commonMaterialChunk.ambientColor;
					//pMat->m_specPower = commonMaterialChunk.shininess;

					break;
				case XBS_MODEL_OBJECT:
					fread(&modelObjectChunk,sizeof(modelObjectChunk),1,inFile);
					
					for (XU32 i = 0;i < modelObjectChunk.numVertexBuffers;++i)
					{
						X_PRIMITIVE_TYPE pt;
						fread(&pt,sizeof(pt),1,inFile);

						X_VERTEX_TYPE vt;
						fread(&vt,sizeof(vt),1,inFile);
						
						XS8 materialName[XBS_MAX_NAME_LEN];
						fread(materialName,sizeof(XU8),XBS_MAX_NAME_LEN,inFile);

						fread(&numVerts,sizeof(numVerts),1,inFile);
						m_numVerts = numVerts;

						XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_LIST,
														numVerts,true,vbHandle);
						pVB = XVP::GraphicsMgr.lockVertexBuffer(vbHandle,X_LT_DISCARD);
						
						for (XU32 j = 0;j < numVerts;++j)
						{
							XSkinVertex v;
							fread(&v,sizeof(v),1,inFile);

							//pVB[j] = v;
							pVB->setPosition(j,v.pos);
							pVB->setNormal(j,0,v.normal);
							pVB->setColor(j,0,v.diffuse);
							pVB->setUV(j,0,v.texCoords);
							pVB->setUV(j,1,v.numBones);
							pVB->setBlendIndices(j,0,v.blendIndices);
							pVB->setBlendWeights(j,0,v.blendWeights);
						}

						XU32 numIndicies;
						fread(&numIndicies,sizeof(numIndicies),1,inFile);
						XVP::GraphicsMgr.createIndexBuffer(numIndicies,true,ibHandle);
						pIB = XVP::GraphicsMgr.lockIndexBuffer(ibHandle,X_LT_DISCARD);
	
						for (XU32 j = 0;j < numIndicies;++j)
						{
							XU16 i;
							fread(&i,sizeof(i),1,inFile);

							pIB[j] =  i;
						}
						
						XIGenBoundingSphere(m_boundingSphere,numVerts,pVB->getPositionStream(),sizeof(XVector3D));

						//Release vertex buffer and index buffer pointers.
						XVP::GraphicsMgr.unlockVertexBuffer(vbHandle);
						XVP::GraphicsMgr.unlockIndexBuffer(ibHandle);

						//Finally, create geometry object
						m_geometry.init(vbHandle,ibHandle,NULL,NULL,NULL,NULL,matHandle);
					}
					break;
				case XBS_BONE:
					fread(&boneChunk,sizeof(boneChunk),1,inFile);
					
					++numBones;
					XNew(pNewBone,XBone,XOSMgr::getDefaultHeap());
					m_nodeArray.push_back(pNewBone);
					pSceneGraphNode = m_nodeTree.createNode(pNewBone,boneChunk.parentName,boneChunk.name);
					m_parentChildList.push_back(pcCombo);
					m_parentChildList[m_parentChildList.size()-1].setChildName(boneChunk.name);
					if (strlen(boneChunk.parentName))
						m_parentChildList[m_parentChildList.size()-1].setParentName(boneChunk.parentName);


					pNewBone->setInvBoneTransform(boneChunk.invBoneTransform);
					m_defaultPose.addTransform(boneChunk.modelTransform);
					pNewBone->setHierarchyID(ID);
					++ID;

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



	
	//Save the total number of bones and verts
	m_numBones = numBones;
	m_numVerts = numVerts;





/*

	XModelLoader loader;
		
		
	if (loader.load(pFilename) != X_OK)
	{
		X_DEBUG_LOG_LN("Could not load skin mesh!",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}

	//Okay, at this point the skin mesh data has been loaded from the file,
	//so it's time to chop it up.  			
	XBone* pBone = NULL;
	XSceneGraphNode* pSceneGraphNode = NULL;
	XParentChildCombo pcCombo;
	XmfMaterial* pMat = NULL;
	XmfGeometry* pObj = NULL;
	XsmBone* pOriginalBone = NULL;
	XMaterial* pRs;

	//Make sure that there is at least one material and one geometry object.
	//Currently the skin mesh will use the material in the first slot and the
	//geometry object in the first slot.
	if ((loader.m_model.materialList.size() < 1) || (loader.m_model.objectList.size() < 1))
	{
		X_DEBUG_LOG_LN("Only one material and one object are supported.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}

	//Get pointers to the first material and first object
	pMat = &loader.m_model.materialList[0];
	pObj = &loader.m_model.objectList[0];
	
	//Create the render state for skin mesh
	if (XVP::createMaterial(NULL,m_matHandle) != X_OK)
	{
		X_DEBUG_LOG_LN("Could not create the render state.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}

	//Grab the interface to the newly created render state.
	pRs = XVP::getMaterialInterface(m_matHandle);
	if (!pRs)
	{
		X_DEBUG_LOG_LN("Could not get render state interface.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}
	
	//Set render state properties.
	if (pMat->fillMode == 0)
		pRs->setFillMode(X_FMT_SOLID);
	else
		pRs->setFillMode(X_FMT_WIRE);

	//Load textures.
	if  ((pMat->texFilename != NULL) && (strcmp(pMat->texFilename,"") != 0))
	{
		if(XVP::GraphicsMgr.createTexture(pMat->texFilename,X_TT_2D,m_texHandle) != X_OK)
		{
			X_DEBUG_LOG_LN("Could not create texture.",X_DLL_ERROR);
			return X_ERR_OPERATION_FAILED;
		}
		else
			pRs->setTextureHandle(0,m_texHandle);
	}

	//Create material.
	XColor diffuse(pMat->diffuseColor.x,
					pMat->diffuseColor.y,
					pMat->diffuseColor.z,
					pMat->diffuseColor.w);
	XColor ambient(pMat->ambientColor.x,
					pMat->ambientColor.y,
					pMat->ambientColor.z,
					pMat->ambientColor.w);
	XColor specular(pMat->specularColor.x,
					pMat->specularColor.y,
					pMat->specularColor.z,
					pMat->specularColor.w);
		
	pRs->m_ambient = ambient;
	pRs->m_diffuse = diffuse;
	pRs->m_specular = specular;
	pRs->m_specPower = pMat->specPower;
	
	//Create and fill the index buffer.
	if (XVP::GraphicsMgr.createIndexBuffer(pObj->numPolyFaces * 3,false,m_ibHandle))
	{
		X_DEBUG_LOG_LN("Could not create index buffer.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}
	else
	{
		pIB = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle);
		if (!pIB)
		{
			X_DEBUG_LOG_LN("Could not lock index buffer.",X_DLL_ERROR);
			return X_ERR_OPERATION_FAILED;
		}
		else
		{
			//Fill in the index buffer
			for (XU32 k = 0;k < pObj->polyFaceList.size();k++)
			{
				pIB[k*3] = pObj->polyFaceList[k].x;
				pIB[(k*3)+1] = pObj->polyFaceList[k].y;
				pIB[(k*3)+2] = pObj->polyFaceList[k].z;
			}

			XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);
		}
	}
			
	//Get the bounding sphere 
	m_boundingSphere = pObj->boundingSphere;

	//Create the vertex buffer.  Since we are in hardware we're going to
	//create skin verts.
	if (XVP::GraphicsMgr.createVertexBuffer(X_VT_SKIN,X_PT_TRIANGLE_LIST,pObj->numVerts,false,m_renderVBHandle) != X_OK)
	{
		X_DEBUG_LOG_LN("Could not create render vertex buffer.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}
		
	//Lock main vertex buffer.
	XSkinVertex* pRenderVB = (XSkinVertex*)XVP::GraphicsMgr.lockVertexBuffer(m_renderVBHandle);
	if (!pRenderVB)
	{
		X_DEBUG_LOG_LN("Could not lock render vertex buffer.",X_DLL_ERROR);
		return X_ERR_OPERATION_FAILED;
	}
		
	//Now fill in the vertex position data.
	for (XU32 k = 0;k < pObj->vertexList.size();k++)
	{
		pRenderVB[k].pos = pObj->vertexList[k];
		pRenderVB[k].diffuse = diffuse.getARGB32Bit();
	}

	//If there are texture vertices then load them.
	if ((pObj->texCoordList.size() > 0) &&
		(pObj->texFaceList.size() > 0))
	{
		for (XU32 k = 0;k < pObj->polyFaceList.size();k++)
		{
			pRenderVB[pObj->polyFaceList[k].x].texCoords = pObj->texCoordList[pObj->texFaceList[k].x];
			pRenderVB[pObj->polyFaceList[k].y].texCoords = pObj->texCoordList[pObj->texFaceList[k].y];
			pRenderVB[pObj->polyFaceList[k].z].texCoords = pObj->texCoordList[pObj->texFaceList[k].z];
		}
	}

	//Read in vertex normals.
	for (XU32 k = 0;k < pObj->vNormalList.size();k++)
		pRenderVB[k].normal = pObj->vNormalList[k];
			
	
	//Load skin data and apply a limit of 4 bones per vertex.
	if (pObj->skinVertList.size() > 0)
	{
		for (XU32 i = 0;i < pObj->skinVertList.size();i++)
		{
			pRenderVB[i].numBones.x = (XF32)pObj->skinVertList[i].boneIDList.size();
			if (pRenderVB[i].numBones.x > 4)
				pRenderVB[i].numBones.x = 4;
			for (XU32 j = 0;((j < pObj->skinVertList[i].boneIDList.size()) && (j != 4));++j)
			{
				pRenderVB[i].blendIndices.array[j] = (XF32)pObj->skinVertList[i].boneIDList[j];
				pRenderVB[i].blendWeights.array[j] = (XF32)pObj->skinVertList[i].boneWeightList[j];
			}
		}
	}

	//Release vertex buffer pointer.
	XVP::GraphicsMgr.unlockVertexBuffer(m_renderVBHandle);

	//Spin through the bone list and create the bone hierarchy.
	X_DEBUG_LOG_LN("Creating Bones...",X_DLL_INFO);
	for (XU32 j = 0;j < loader.m_model.boneList.size();j++)
	{
		//Get pointers to the data we are interested in during this iteration
		pOriginalBone = &loader.m_model.boneList[j]; 

		//First we need to insert a new node into the skin mesh template's bone tree. 
		XBone* pNewBone = new XBone();
		m_nodeArray.push_back(pNewBone);
			
		pSceneGraphNode = m_nodeTree.createNode(pNewBone,pOriginalBone->parentName,pOriginalBone->name);
		X_DEBUG_LOG_LN(pOriginalBone->name,X_DLL_INFO);

		m_parentChildList.push_back(pcCombo);
		if (pOriginalBone->name != NULL)
			m_parentChildList[j].setChildName(pOriginalBone->name);
		if (pOriginalBone->parentName)
			m_parentChildList[j].setParentName(pOriginalBone->parentName);

		//Copy over the bone members.
		pNewBone->setHierarchyID(pOriginalBone->ID);
		pNewBone->setInvBoneTransform(pOriginalBone->invBoneTransform);
		
		//pNewBone->setModelTransform(pOriginalBone->modelTransform);
		m_defaultPose.addTransform(pOriginalBone->modelTransform);
	}
		
	//Save the total number of bones.
	m_numBones = loader.m_model.boneList.size();

	//Save the total number of verts.
	m_numVerts = pObj->vertexList.size();
			 
	//Everything was created okay. Release model data and return.
	loader.destroyModelData();
*/

	m_inited = true;

	return X_OK;
}



//*****************************************************************************
/**
Generates a skin mesh instance from this skin mesh template.  This
function can be used to generate any number of skin mesh instances from
this template.  Before this function can be called however, the skin mesh
template has to initialized with a call to init().

It's important to note that as long as the skin mesh instances that are
created from this template exist, the template must also exist.  If a template, that
was used to create instances, goes out of scope while instances still exist,
the application will become unstable.  Therefore, it's important to know when
it's safe to deallocate the template.  A good rule of thumb is to destoy all 
instances of the object before destroying the template used to create them.

@param	skinMesh	An XSkinMesh object to hold the newly generated 
					skin mesh.
@return	Returns a status code of type XStatus.
*/
//*****************************************************************************
XStatus XSkinMeshTemplate::generateSkinMesh(XSkinMesh& skinMesh)
{
	//Re-initialize the skin mesh in case it contains data.
	skinMesh.destroy();
	skinMesh.init();

	XBone* pBone;
	
	//SHADERCONVERT skinMesh.setMaterialHandle(m_matHandle);
		
	//skinMesh.setVertexBufferHandle(m_vbHandle);
	//skinMesh.setIndexBufferHandle(m_ibHandle);
	skinMesh.setBoundingSphere(m_boundingSphere);
	//skinMesh.m_renderVBHandle = m_renderVBHandle;
	skinMesh.m_numVerts = m_numVerts;
	skinMesh.m_pDataTemplate = this;
	
	//Build a bone ID list that will allow us to quickly access bones
	//in the tree by bone ID instead of recursing through the tree. This
	//will be used to apply bone influences to each vertex.  For now,
	//just create enough slots.  Later we will fill the slots as we
	//create bones.
	for (XU32 i = 0;i < m_numBones;i++)
	{
		XBone* pOriginalBone = NULL;
		skinMesh.m_boneIDIndexArray.push_back(pOriginalBone);
	}

	//We need to create the appropriate nodes in the new tree.
	//The parent/child names for creating the new tree can be found in 
	//the skin mesh template's m_parentChildList.  We will use these to create
	//the new nodes.
	XNewArray(skinMesh.m_pNodeArray,XSceneNode*,m_parentChildList.size(),XOSMgr::getModelHeap())
	for (XU32 i = 0;i < m_parentChildList.size();i++)
	{		  
		//Insert a new node in to the instance tree
		XBone* pNewBone = NULL;
		XNew(pNewBone,XBone,XOSMgr::getDefaultHeap());
		skinMesh.m_pNodeArray[i] = pNewBone;
		skinMesh.m_nodeTree.createNode(pNewBone,m_parentChildList[i].getParentName(),
									   m_parentChildList[i].getChildName());

		//Find the node in the skin mesh template's bone tree that matches the
		//one we just created for the new skin mesh instance.
		pBone = (XBone*)(m_nodeTree.getNode(m_parentChildList[i].getChildName()))->m_pSceneNode;
		X_DEBUG_LOG_LN(m_parentChildList[i].getChildName(),X_DLL_INFO);

		//Copy the relevant bone data from skinMeshTemplate to skinMesh		
		pNewBone->setInvBoneTransform(pBone->getInvBoneTransform());
		pNewBone->setModelTransform(pBone->getModelTransform());

		pNewBone->setHierarchyID(pBone->getHierarchyID());
		//pNewBone->m_pParentSkinMesh = &skinMesh;
			   		
		pNewBone->m_pParentModel = &skinMesh;
		//pNewBone->m_pModelObjectTemplate = pBone;

		//Save this new bone's pointer to the m_boneIDIndexArray in order to provide
		//quick and easy access to it for skinning during rendering.
		skinMesh.m_boneIDIndexArray[pBone->getHierarchyID()] = pNewBone;   
	}

	skinMesh.setPose(&m_defaultPose);
	skinMesh.m_numNodes = m_parentChildList.size();

	//Increment the skin mesh template refCount.
	++m_refCount;

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XSkinMeshTemplate::attachShaderController(XS8* pMaterialName,
													XShaderController* pMatController)
{
	//If we are looking for a specific material name
	if (pMaterialName)
	{
		XS8* matName = XVP::GraphicsMgr.getMaterialName(m_geometry.m_matHandle);
		if (matName)
			if (strcmp(matName,pMaterialName) == 0)
				XVP::GraphicsMgr.getMaterialInterface(m_geometry.m_matHandle)->attachShaderController(pMatController);					
	}
	//else we are applying the controller to all materials
	else
		XVP::GraphicsMgr.getMaterialInterface(m_geometry.m_matHandle)->attachShaderController(pMatController);
	
	return X_OK;
}
