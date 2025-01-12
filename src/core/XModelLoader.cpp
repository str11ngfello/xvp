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
//	Module Name:	XModelLoader.cpp
//	Author:			Lynn Duke
//	Creation Date:	10-7-02
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


//*****************************************************************************
//*****************************************************************************
XModelLoader::XModelLoader()
{
}



//*****************************************************************************
//*****************************************************************************
XModelLoader::~XModelLoader()
{
	destroyModelData();
}



//*****************************************************************************
//*****************************************************************************
void XModelLoader::destroyModelData()
{
	//Clean up dynamic data
	for (XU32 i = 0;i < m_model.materialList.size();i++)
		XDeleteArray(m_model.materialList[i].texFilename);
	m_model.materialList.clear();
	m_model.poseTransforms.clear();
	for (XU32 i = 0;i < m_model.objectList.size();i++)
	{
		XDeleteArray(m_model.objectList[i].objectName)
		XDeleteArray(m_model.objectList[i].parentName)
		m_model.objectList[i].cpvVertList.clear();
		m_model.objectList[i].polyFaceList.clear();

		for (XU32 j = 0;j < m_model.objectList[i].skinVertList.size();j++)
		{		
			m_model.objectList[i].skinVertList[j].boneIDList.clear();
			m_model.objectList[i].skinVertList[j].boneWeightList.clear();
		}
		m_model.objectList[i].skinVertList.clear();

		for (XU32 j = 0;j < m_model.boneList.size();j++)
		{
			XDeleteArray(m_model.boneList[j].name);
			XDeleteArray(m_model.boneList[j].parentName);
		}

		m_model.objectList[i].texCoordList.clear();
		m_model.objectList[i].texFaceList.clear();
		m_model.objectList[i].vertexList.clear();
		m_model.objectList[i].vNormalList.clear();

	}
	m_model.objectList.clear();

	m_model.boneList.clear();

	for (XU32 i = 0;i < m_model.m_animationTrack.keysList.size();i++)
	{
		m_model.m_animationTrack.keysList[i].posKeyList.clear();
		m_model.m_animationTrack.keysList[i].rotKeyList.clear();
		m_model.m_animationTrack.keysList[i].scaleKeyList.clear();
	}
	m_model.m_animationTrack.keysList.clear();

	m_model.m_matAnimationTrack.uOffsetKeyList.clear();
	m_model.m_matAnimationTrack.vOffsetKeyList.clear();
	m_model.m_matAnimationTrack.wAngleKeyList.clear();

	XDeleteArray(m_model.particleSystem.name);
	XDeleteArray(m_model.particleSystem.parentName);

	XDeleteArray(m_model.camera.pName);
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelLoader::load(XS8* filename)
{
	if (!filename)
		return X_ERR_INVALID_PARAMS;

	//First, make sure that any previous data has been destroyed.
	destroyModelData();

	FILE* f;
	f = fopen(filename,"r");
	
	//Make sure the file was openend.
	if (!f)
	{
		X_DEBUG
		(	
			std::string s;
			s = "MODEL LOADER: Could not find file -> ";
			s += filename;
			X_ASSERT_FORCED(s.c_str());
		)

		return X_ERR_INVALID_PARAMS;
	}

	int c;
	char chunkName[MAX_CHUNK_NAME_LENGTH];

	//Determine what extension the file has
	XS8* ext = &filename[strlen(filename) - 3];
	XU8 fileType;
	if ((strcmp(ext,"xmf") == 0) || (strcmp(ext,"xof") == 0) || (strcmp(ext,"xsm") == 0))
		fileType = 0;
	else if (strcmp(ext,"xaf") == 0)
		fileType = 1;
	else if (strcmp(ext,"xlf") == 0)
		fileType = 2;
	else if (strcmp(ext,"xcf") == 0)
		fileType = 3;
	else if (strcmp(ext,"xps") == 0)
		fileType = 4;
	else if (strcmp(ext,"xpo") == 0)
		fileType = 5;
	else if (strcmp(ext,"uva") == 0)
		fileType = 6;
	else
		fileType = 255;

	while (!feof(f))
	{
		c = fgetc(f);
		if (c == '@')
		{
			if (fscanf(f,"%s",chunkName) != EOF)
				switch (fileType)
				{
				case 0:
					if (!xmfProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				case 1:
					if (!xafProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				case 2:
					if (!xlfProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				case 3:
					if (!xcfProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				case 4:
					if (!xpsProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				case 5:
					if (!xpoProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				case 6:
					if (!uvaProcessChunk(chunkName,f))
						return X_ERR_OPERATION_FAILED;
					break;
				default:
					return X_ERR_OPERATION_FAILED;
					break;
				}
		}		
	}

	fclose(f);

	//If we make it here then everything was ok.
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xmfProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"MATERIAL") == 0)
		return xmfProcessMaterialObject(f);
	if (strcmp(chunkName,"GEOMETRY") == 0 || strcmp(chunkName,"DUMMY") == 0)
		return xmfProcessGeometryObject(f);
	if (strcmp(chunkName,"BONE") == 0)
		return xmfProcessBone(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xmfProcessMaterialObject(FILE* f)
{
	XmfMaterial material;
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];
	char tmpBuf[MAX_TEMP_BUF_LENGTH];

	while (true)
	{
		fscanf(f,"%s",chunkValueName);
		
		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;
		
		if (strcmp(chunkValueName,"DIFFUSE_COLOR") == 0)
		{
			fscanf(f,"%f %f %f %f",&material.diffuseColor.x,&material.diffuseColor.y,&material.diffuseColor.z,&material.diffuseColor.w);
		}
		else if (strcmp(chunkValueName,"AMBIENT_COLOR") == 0)
		{
			fscanf(f,"%f %f %f %f",&material.ambientColor.x,&material.ambientColor.y,&material.ambientColor.z,&material.ambientColor.w);
		}
		else if (strcmp(chunkValueName,"SPECULAR_COLOR") == 0)
		{
			fscanf(f,"%f %f %f %f",&material.specularColor.x,&material.specularColor.y,&material.specularColor.z,&material.specularColor.w);
		}
		else if (strcmp(chunkValueName,"SPECULAR_POWER") == 0)
		{
			fscanf(f,"%f",&material.specPower);
		}
		else if (strcmp(chunkValueName,"FILL_MODE") == 0)
		{
			fscanf(f,"%u",&material.fillMode);
		}
		else if (strcmp(chunkValueName,"DIFFUSE_MAP") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
			
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(material.texFilename,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(material.texFilename,strippedString);
			}
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	//Push the newly created material onto the object list.
	m_model.materialList.push_back(material);

	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xpoProcessTransform(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];
	XMatrix4x4 transform;

	while (true)
	{
		fscanf(f,"%s",chunkValueName);
		
		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;
		
		if (strcmp(chunkValueName,"MX_R1") == 0)
		{
			fscanf(f,"%f %f %f",&transform._11,
								&transform._12,
								&transform._13);
		}
		else if (strcmp(chunkValueName,"MX_R2") == 0)
		{
			fscanf(f,"%f %f %f",&transform._21,
								&transform._22,
								&transform._23);
		}
		else if (strcmp(chunkValueName,"MX_R3") == 0)
		{
			fscanf(f,"%f %f %f",&transform._31,
								&transform._32,
								&transform._33);
		}
		else if (strcmp(chunkValueName,"MX_R4") == 0)
		{
			fscanf(f,"%f %f %f",&transform._41,
								&transform._42,
								&transform._43);
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	//Push the newly created material onto the object list.
	m_model.poseTransforms.push_back(transform);
	
	return true;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xmfProcessGeometryObject(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];
	char tmpBuf[MAX_TEMP_BUF_LENGTH];
	XF32 tempFloat1,tempFloat2,tempFloat3;

	XmfGeometry object;

	while(true)
	{
		fscanf(f,"%s",chunkValueName);
		
		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		//If we make it here then we have a valid chunk value to process
		//so we check for the specific value.
		if (strcmp(chunkValueName,"NAME") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
			
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(object.objectName,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(object.objectName,strippedString);
			}
		}
		else if (strcmp(chunkValueName,"PARENT_NAME") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
			
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(object.parentName,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(object.parentName,strippedString);
			}
		}
		else if (strcmp(chunkValueName,"MX_R1") == 0)
		{
			fscanf(f,"%f %f %f",&object.modelTransform._11,
								&object.modelTransform._12,
								&object.modelTransform._13);
		}
		else if (strcmp(chunkValueName,"MX_R2") == 0)
		{
			fscanf(f,"%f %f %f",&object.modelTransform._21,
								&object.modelTransform._22,
								&object.modelTransform._23);
		}
		else if (strcmp(chunkValueName,"MX_R3") == 0)
		{
			fscanf(f,"%f %f %f",&object.modelTransform._31,
								&object.modelTransform._32,
								&object.modelTransform._33);
		}
		else if (strcmp(chunkValueName,"MX_R4") == 0)
		{
			fscanf(f,"%f %f %f",&object.modelTransform._41,
								&object.modelTransform._42,
								&object.modelTransform._43);
		}
		else if (strcmp(chunkValueName,"SPHERE_CENTER") == 0)
		{
			fscanf(f,"%f %f %f",&object.boundingSphere.center.x,
								&object.boundingSphere.center.y,
								&object.boundingSphere.center.z);
		}
		else if (strcmp(chunkValueName,"SPHERE_RADIUS") == 0)
		{
			fscanf(f,"%f",&object.boundingSphere.radius);
		}
		else if (strcmp(chunkValueName,"MATERIAL_ID") == 0)
		{
			fscanf(f,"%i",&object.materialID);
		}
		else if (strcmp(chunkValueName,"NUM_VERTS") == 0)
		{
			fscanf(f,"%u",&object.numVerts);
			//Allocate the space for the vertices.
			if (object.numVerts > 0)
			{
				//Now read in the data
				XVector3D tempVec;
				for (XU32 i = 0;i < object.numVerts;i++)
				{
					fscanf(f,"%*d %f %f %f",&tempVec.x,&tempVec.y,&tempVec.z);
					object.vertexList.push_back(tempVec);
				}
			}
				
		}
		else if (strcmp(chunkValueName,"NUM_TVERTS") == 0)
		{
			fscanf(f,"%u",&object.numTexCoordPairs);

			//Allocate the space for the vertices.
			if (object.numTexCoordPairs > 0)
			{
				//Now read in the data
				XVector2D tempVec;
				for (XU32 i = 0;i < object.numTexCoordPairs;i++)
				{
					fscanf(f,"%*d %f %f",&tempVec.x,&tempVec.y);
					object.texCoordList.push_back(tempVec);
				}
			}
		}
		else if (strcmp(chunkValueName,"NUM_VNORMALS") == 0)
		{
			fscanf(f,"%u",&object.numVNormals);

			//Allocate the space for the vertex normals.
			if (object.numVNormals > 0)
			{
				//Now read in the data
				XVector3D tempVec;
				for (XU32 i = 0;i < object.numVNormals;i++)
				{
					fscanf(f,"%*d %f %f %f",&tempVec.x,&tempVec.y,&tempVec.z);
					object.vNormalList.push_back(tempVec);
				}
			}
		}
		else if (strcmp(chunkValueName,"NUM_MFACES") == 0)
		{
			fscanf(f,"%u",&object.numPolyFaces);

			//Allocate the space for the vertices.
			if (object.numPolyFaces > 0)
			{
				//Now read in the data
				XmfFace tempVec;
				for (XU32 i = 0;i < object.numPolyFaces;i++)
				{
					fscanf(f,"%*d %f %f %f",&tempFloat1,&tempFloat2,&tempFloat3);
					tempVec.x = (XU32)tempFloat1;
					tempVec.y = (XU32)tempFloat2;
					tempVec.z = (XU32)tempFloat3;
					object.polyFaceList.push_back(tempVec);
				}
			}
		}
		else if (strcmp(chunkValueName,"NUM_TFACES") == 0)
		{
			fscanf(f,"%u",&object.numTexFaces);

			//Allocate the space for the vertices.
			if (object.numTexFaces > 0)
			{
				//Now read in the data
				XmfFace tempVec;
				for (XU32 i = 0;i < object.numTexFaces;i++)
				{
					fscanf(f,"%*d %f %f %f",&tempFloat1,&tempFloat2,&tempFloat3);
					tempVec.x = (XU32)tempFloat1;
					tempVec.y = (XU32)tempFloat2;
					tempVec.z = (XU32)tempFloat3;
					object.texFaceList.push_back(tempVec);
				}
			}
		}
		else if (strcmp(chunkValueName,"NUM_CPV_VERTS") == 0)
		{
			fscanf(f,"%u",&object.numCpvVerts);
			//Allocate the space for the vertices.
			if (object.numCpvVerts > 0)
			{
				//Now read in the data
				XmfColor color;
				for (XU32 i = 0;i < object.numCpvVerts;i++)
				{					
					fscanf(f,"%*d %f %f %f %f",&color.a,&color.r,&color.g,&color.b);
					object.cpvVertList.push_back(color);					
				}
			}				
		}
		else if (strcmp(chunkValueName,"NUM_SKIN_VERTS") == 0)
		{
			fscanf(f,"%u",&object.numSkinVerts);

			//Allocate the space for the skin vertices.
			if (object.numSkinVerts > 0)
			{	
				//Now, for each skin vertex, loop through the
				//number of bones/weights
				for (XU32 i = 0;i < object.numSkinVerts;i++)
				{
					XU32 numWeights = 0;
					fscanf(f,"%*d %u",&numWeights);
					
					//Push an empty skin vertex onto the end of the list.
					XmfSkinVertex sVert;					
					object.skinVertList.push_back(sVert);
								
					for (XU32 j = 0;j < numWeights;j++)
					{	
						//Push a new bone and weight onto the list
						XU32 boneID;
						XF32 weight;						
						fscanf(f,"%*d %u %f",&boneID,&weight);
						object.skinVertList[i].boneIDList.push_back(boneID);
						object.skinVertList[i].boneWeightList.push_back(weight);
					}
				}
			}
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	//Optimize the vertex and texture coordinate pair relationships only if this
	//object contains texture coordinates
	if ((object.texCoordList.size() > 0) && (object.texFaceList.size() > 0))
		xmfWeldVertices(&object);
/*
	//NVTriStrip setup code
	SetCacheSize(CACHESIZE_GEFORCE3);
	SetStitchStrips(true);
	SetMinStripSize(0);
	SetListsOnly(true);

	XU16 pIndices = (XU16)&object.polyFaceList[0].x;
	PrimitiveGroup* pg;
	XU16 numSections;
	int ctr = 0;
	ofstream ff;
	ff.open("c:/test.txt",ios::app);

	XU16* converted = new XU16[object.polyFaceList.size() * 3];

	for (int i = 0;i < object.polyFaceList.size();++i)
	{
		converted[ctr++] = object.polyFaceList[i].x;
		converted[ctr++] = object.polyFaceList[i].y;
		converted[ctr++] = object.polyFaceList[i].z;

		//ff << object.polyFaceList[ctr].x << " " ;
		//ff << object.polyFaceList[ctr].y << " " ;
		//ff << object.polyFaceList[ctr++].z << " " ;
	}

	GenerateStrips(converted,object.numPolyFaces * 3,&pg,&numSections);
	ff << object.numPolyFaces * 3 << " " << pg[0].numIndices << endl;
	//	memcpy(&object.polyFaceList[0].x,pg[0].indices, pg[0].numIndices * sizeof(XU16));
	object.polyFaceList.clear();
	ctr = 0;
	for (int i = 0;i < pg[0].numIndices /3;++i)
	{
		XmfFace face;
		//ff << pg[0].indices[ctr] << " " ;
		face.x = pg[0].indices[ctr++];
		//ff << pg[0].indices[ctr] << " ";
		face.y = pg[0].indices[ctr++];
		//ff << pg[0].indices[ctr] << " ";
		face.z = pg[0].indices[ctr++];
		object.polyFaceList.push_back(face);
	}
	object.numPolyFaces = pg[0].numIndices /3;*/
/*

/////////////////////////////////////////////////////////////////////////
	// NVTriStrip setup code
	SetCacheSize(CACHESIZE_GEFORCE3);
	SetStitchStrips(true);
	SetMinStripSize(0);

	//First, we create the optimized list indices
	SetListsOnly(true);

	unsigned short* pIndices = NULL;
	hr = tempMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);
	if (FAILED(hr))
		return hr;

	unsigned short numSections;
	GenerateStrips(pIndices, g_dwNumFacesList * 3, &g_pPrimitiveGroupsList, &numSections);
	tempMesh->UnlockIndexBuffer();

	//copy optimized data to index buffer
	pd3dDevice->CreateIndexBuffer(g_pPrimitiveGroupsList[0].numIndices * sizeof(unsigned short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
		D3DPOOL_DEFAULT, &g_pIndexBufferOptList, NULL);
	unsigned short* pDest;
	g_pIndexBufferOptList->Lock(0, g_pPrimitiveGroupsList[0].numIndices * sizeof(unsigned short), (void**)&pDest, 0);
	memcpy(pDest, g_pPrimitiveGroupsList[0].indices, g_pPrimitiveGroupsList[0].numIndices * sizeof(unsigned short));
	g_pIndexBufferOptList->Unlock();

	//Next, we create the optimized strip indices
	SetListsOnly(false);

	pIndices = NULL;
	hr = tempMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndices);
	if (FAILED(hr))
		return hr;

	GenerateStrips(pIndices, g_dwNumFacesList * 3, &g_pPrimitiveGroupsStrip, &numSections);
	tempMesh->UnlockIndexBuffer();

	//copy optimized data to index buffer
	pd3dDevice->CreateIndexBuffer(g_pPrimitiveGroupsStrip[0].numIndices * sizeof(unsigned short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
		D3DPOOL_DEFAULT, &g_pIndexBufferOptStrip, NULL);
	g_pIndexBufferOptStrip->Lock(0, g_pPrimitiveGroupsStrip[0].numIndices * sizeof(unsigned short), (void**)&pDest, 0);
	memcpy(pDest, g_pPrimitiveGroupsStrip[0].indices, g_pPrimitiveGroupsStrip[0].numIndices * sizeof(unsigned short));
	g_pIndexBufferOptStrip->Unlock();

	g_dwNumFacesStrip = g_pPrimitiveGroupsStrip[0].numIndices - 2;

	//convert to our format
	hr = tempMesh->CloneMeshFVF(D3DXMESH_WRITEONLY, dwFVF, pd3dDevice, &tempMeshFVF);

	tempMeshFVF->GetVertexBuffer(&g_pVertexBuffer);
	tempMeshFVF->GetIndexBuffer(&g_pIndexBufferUnOpt);

    SAFE_RELEASE(tempMesh);
    SAFE_RELEASE(tempMeshFVF);
    
    hr = pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof( VertexType ) );
    if (FAILED(hr))
        return hr;

    return S_OK;
*/


	//Push the newly created object onto the object list.
	m_model.objectList.push_back(object);

	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xmfProcessBone(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];
	char tmpBuf[MAX_TEMP_BUF_LENGTH];
	
	XsmBone bone;

	while(true)
	{
		fscanf(f,"%s",chunkValueName);

		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		//If we make it here then we have a valid chunk value to process
		//so we check for the specific value.
		if (strcmp(chunkValueName,"ID") == 0)
		{
			fscanf(f,"%u",&bone.ID);
		}
		else if (strcmp(chunkValueName,"NAME") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
				
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(bone.name,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(bone.name,strippedString);
			}
		}
		else if (strcmp(chunkValueName,"PARENT_NAME") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
				
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(bone.parentName,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(bone.parentName,strippedString);
			}
		}
		else if (strcmp(chunkValueName,"MX_R1") == 0)
		{
			fscanf(f,"%f %f %f",&bone.modelTransform._11,
								&bone.modelTransform._12,
								&bone.modelTransform._13);
		}
		else if (strcmp(chunkValueName,"MX_R2") == 0)
		{
			fscanf(f,"%f %f %f",&bone.modelTransform._21,
								&bone.modelTransform._22,
								&bone.modelTransform._23);
		}
		else if (strcmp(chunkValueName,"MX_R3") == 0)
		{
			fscanf(f,"%f %f %f",&bone.modelTransform._31,
								&bone.modelTransform._32,
								&bone.modelTransform._33);
		}
		else if (strcmp(chunkValueName,"MX_R4") == 0)
		{
			fscanf(f,"%f %f %f",&bone.modelTransform._41,
								&bone.modelTransform._42,
								&bone.modelTransform._43);
		}
		else if (strcmp(chunkValueName,"IBX_R1") == 0)
		{
			fscanf(f,"%f %f %f",&bone.invBoneTransform._11,
								&bone.invBoneTransform._12,
								&bone.invBoneTransform._13);
		}
		else if (strcmp(chunkValueName,"IBX_R2") == 0)
		{
			fscanf(f,"%f %f %f",&bone.invBoneTransform._21,
								&bone.invBoneTransform._22,
								&bone.invBoneTransform._23);
		}
		else if (strcmp(chunkValueName,"IBX_R3") == 0)
		{
			fscanf(f,"%f %f %f",&bone.invBoneTransform._31,
								&bone.invBoneTransform._32,
								&bone.invBoneTransform._33);
		}
		else if (strcmp(chunkValueName,"IBX_R4") == 0)
		{
			fscanf(f,"%f %f %f",&bone.invBoneTransform._41,
								&bone.invBoneTransform._42,
								&bone.invBoneTransform._43);
		}

		 
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	//Push the newly created bone onto the list.
	m_model.boneList.push_back(bone);

	return true;

}



//*****************************************************************************
//*****************************************************************************
void XModelLoader::xmfWeldVertices(XmfGeometry* object)
{
	MlVertexTexInfo* vtBlockPtr;
	vector<MlVertexTexInfo> vertexTexList;
	vector<MlPolyFaceListChange> changeList;
	
	//Make sure that the object has texture coordinates
	if ((object->texCoordList.size() <= 0) || (object->texFaceList.size() <= 0))
		return;

	//For each poly face index
	for (XU32 i = 0;i < object->numPolyFaces;i++)
	{
		//**************
		//1st Poly Index
		//**************

		//If the vertex has not been accounted for, then it needs to be
		//added to the list along with the tex coordinate in the 
		//corresponding tex coord list. We also store the value of the polyFaceList
		//in the vertexTexInfoBlock so that later we can re-use when
		//coming across this vertex and tex coord pair again.
		vtBlockPtr = getVertexTexInfoBlock(&vertexTexList,object->polyFaceList[i].x);
		if (vtBlockPtr == NULL)
		{
			MlVertexTexInfo newVTBlock;
			newVTBlock.vertexIndex = object->polyFaceList[i].x;
			newVTBlock.texIndexList.push_back(object->texFaceList[i].x);
			vertexTexList.push_back(newVTBlock);
		}
		//Else the vertex has been seen before, so we should look deeper into the
		//tex coord list to see what should be done at this point.
		else
		{
			//if the vertex has already been seen before then make sure that we
			//check the associated tex coord index to see if it has already
			//been associated with this vertex index before. If it hasn't, then
			//we need to add it to the "already seen" list and create a new
			//vertex.  Also we must update the polyFaceList to point to the
			//new vertex information.
			if (!texCoordIndexExists(vtBlockPtr,object->texFaceList[i].x))
			{
				//Since this tex coord index has not been seen paired with this
				//particular vertex buffer, we must create a new vertex
				//and append it to the vertex buffer list.
				object->vertexList.push_back(object->vertexList[object->polyFaceList[i].x]);

				//Next, modify the poly face list to point to the new vertex index. Push this
				//change onto the change list for processing later.
				MlPolyFaceListChange change;
				change.fromPolyIndex = object->polyFaceList[i].x;
				change.toPolyIndex = object->vertexList.size() - 1;
				change.texIndex = object->texFaceList[i].x;
				changeList.push_back(change);

				//Also, duplicate the skin vertex index for this vertex if skin vertices
				//exist
				if (object->skinVertList.size() > 0)
				{
					//Push a blank skin vertex on the vector.  We do this early because
					//the xmfAppendSkinVertex call takes pointers.  Since the "push_back"
					//call of an STL vector can potentially change addresses, therefore
					//invalidating pointers, we go ahead and allocate a new skin vertex
					//right off the bat.  This ensures that any pointers to elements
					//in our vector are valid at least until the next push_back call.
					XmfSkinVertex sv;
					object->skinVertList.push_back(sv);
					xmfAppendSkinVertex(&object->skinVertList,
										&object->skinVertList[object->polyFaceList[i].x]);
				}

				//Also, duplicate the Color-Per-Vertex information for this vertex if
				//this information exists.
				if (object->cpvVertList.size() > 0)
					object->cpvVertList.push_back(object->cpvVertList[object->polyFaceList[i].x]);

				//Also, duplicate the vertex normal if this information exists
				if (object->vNormalList.size() > 0)
					object->vNormalList.push_back(object->vNormalList[object->polyFaceList[i].x]);
				
				//Push the tex coord index on the "already seen" list to avoid
				//adding another vertex in response to this tex coord.
				vtBlockPtr->texIndexList.push_back(object->texFaceList[i].x);
			}
		}


		//**************
		//2nd Poly Index
		//**************

		//If the vertex has not been accounted for, then it needs to be
		//added to the list along with the tex coordinate in the 
		//corresponding tex coord list. 
		vtBlockPtr = getVertexTexInfoBlock(&vertexTexList,object->polyFaceList[i].y);
		if (vtBlockPtr == NULL)
		{
			MlVertexTexInfo newVTBlock;
			newVTBlock.vertexIndex = object->polyFaceList[i].y;
			newVTBlock.texIndexList.push_back(object->texFaceList[i].y);
			vertexTexList.push_back(newVTBlock);
		}
		//Else the vertex has been seen before, so we should look deeper into the
		//tex coord list to see what should be done at this point.
		else
		{
			//if the vertex has already been seen before then make sure that we
			//check the associated tex coord index to see if it has already
			//been associated with this vertex index before. If it hasn't, then
			//we need to add it to the "already seen" list and create a new
			//vertex.  Also we must update the polyFaceList to point to the
			//new vertex information.
			if (!texCoordIndexExists(vtBlockPtr,object->texFaceList[i].y))
			{
				//Since this tex coord index has not been seen paired with this
				//particular vertex buffer, we must create a new vertex
				//and append it to the vertex buffer list.
				object->vertexList.push_back(object->vertexList[object->polyFaceList[i].y]);

				//Next, modify the poly face list to point to the new vertex index. Push this
				//change onto the change list for processing later.
				MlPolyFaceListChange change;
				change.fromPolyIndex = object->polyFaceList[i].y;
				change.toPolyIndex = object->vertexList.size() - 1;
				change.texIndex = object->texFaceList[i].y;
				changeList.push_back(change);

				//Also, duplicate the skin vertex index for this vertex if skin vertices
				//exist
				if (object->skinVertList.size() > 0)
				{
					//Push a blank skin vertex on the vector.  We do this early because
					//the xmfAppendSkinVertex call takes pointers.  Since the "push_back"
					//call of an STL vector can potentially change addresses, therefore
					//invalidating pointers, we go ahead and allocate a new skin vertex
					//right off the bat.  This ensures that any pointers to elements
					//in our vector are valid at least until the next push_back call.
					XmfSkinVertex sv;
					object->skinVertList.push_back(sv);
					xmfAppendSkinVertex(&object->skinVertList,
										&object->skinVertList[object->polyFaceList[i].y]);
				}

				//Also, duplicate the Color-Per-Vertex information for this vertex if
				//this information exists.
				if (object->cpvVertList.size() > 0)
					object->cpvVertList.push_back(object->cpvVertList[object->polyFaceList[i].y]);
				 				
				//Also, duplicate the vertex normal if this information exists
				if (object->vNormalList.size() > 0)
					object->vNormalList.push_back(object->vNormalList[object->polyFaceList[i].y]);


				//Push the tex coord index on the "already seen" list to avoid
				//adding another vertex in response to this tex coord.
				vtBlockPtr->texIndexList.push_back(object->texFaceList[i].y);
			}
		}


		//**************
		//3rd Poly Index
		//**************

		//If the vertex has not been accounted for, then it needs to be
		//added to the list along with the tex coordinate in the 
		//corresponding tex coord list. 
		vtBlockPtr = getVertexTexInfoBlock(&vertexTexList,object->polyFaceList[i].z);
		if (vtBlockPtr == NULL)
		{
			MlVertexTexInfo newVTBlock;
			newVTBlock.vertexIndex = object->polyFaceList[i].z;
			newVTBlock.texIndexList.push_back(object->texFaceList[i].z);
			vertexTexList.push_back(newVTBlock);
		}
		//Else the vertex has been seen before, so we should look deeper into the
		//tex coord list to see what should be done at this point.
		else
		{
			//if the vertex has already been seen before then make sure that we
			//check the associated tex coord index to see if it has already
			//been associated with this vertex index before. If it hasn't, then
			//we need to add it to the "already seen" list and create a new
			//vertex.  Also we must update the polyFaceList to point to the
			//new vertex information.
			if (!texCoordIndexExists(vtBlockPtr,object->texFaceList[i].z))
			{
				//Since this tex coord index has not been seen paired with this
				//particular vertex buffer, we must create a new vertex
				//and append it to the vertex buffer list.
				object->vertexList.push_back(object->vertexList[object->polyFaceList[i].z]);

				//Next, modify the poly face list to point to the new vertex index. Push this
				//change onto the change list for processing later.
				MlPolyFaceListChange change;
				change.fromPolyIndex = object->polyFaceList[i].z;
				change.toPolyIndex = object->vertexList.size() - 1;
				change.texIndex = object->texFaceList[i].z;
				changeList.push_back(change);

				//Also, duplicate the skin vertex index for this vertex if skin vertices
				//exist
				if (object->skinVertList.size() > 0)
				{
					//Push a blank skin vertex on the vector.  We do this early because
					//the xmfAppendSkinVertex call takes pointers.  Since the "push_back"
					//call of an STL vector can potentially change addresses, therefore
					//invalidating pointers, we go ahead and allocate a new skin vertex
					//right off the bat.  This ensures that any pointers to elements
					//in our vector are valid at least until the next push_back call.
					XmfSkinVertex sv;
					object->skinVertList.push_back(sv);
					xmfAppendSkinVertex(&object->skinVertList,
										&object->skinVertList[object->polyFaceList[i].z]);
				}

				//Also, duplicate the Color-Per-Vertex information for this vertex if
				//this information exists.
				if (object->cpvVertList.size() > 0)
					object->cpvVertList.push_back(object->cpvVertList[object->polyFaceList[i].z]);

				//Also, duplicate the vertex normal if this information exists
				if (object->vNormalList.size() > 0)
					object->vNormalList.push_back(object->vNormalList[object->polyFaceList[i].z]);

				//Push the tex coord index on the "already seen" list to avoid
				//adding another vertex in response to this tex coord.
				vtBlockPtr->texIndexList.push_back(object->texFaceList[i].z);
			}
		}
	}

	//At this point, we have duplicated all the vertices that were sharing texture
	//coordinates so that there is a one-to-one mapping between vertex and tex coord.
	//So, now we run through the change list that was just built above and apply
	//the changes therein.
	for (XU32 ctr = 0;ctr < changeList.size();ctr++)
	{
		for (XU32 ctr2 = 0;ctr2 < object->polyFaceList.size();ctr2++)
		{
			if ((object->polyFaceList[ctr2].x == changeList[ctr].fromPolyIndex) &&
			   (object->texFaceList[ctr2].x == changeList[ctr].texIndex))
			   object->polyFaceList[ctr2].x = changeList[ctr].toPolyIndex;
			if ((object->polyFaceList[ctr2].y == changeList[ctr].fromPolyIndex) &&
			   (object->texFaceList[ctr2].y == changeList[ctr].texIndex))
			   object->polyFaceList[ctr2].y = changeList[ctr].toPolyIndex;
			if ((object->polyFaceList[ctr2].z == changeList[ctr].fromPolyIndex) &&
			   (object->texFaceList[ctr2].z == changeList[ctr].texIndex))
			   object->polyFaceList[ctr2].z = changeList[ctr].toPolyIndex;
		}
	}

	//Don't forget to update the vertex list size in case we have added new
	//vertices or any other type of information
	object->numVerts = object->vertexList.size();
	object->numCpvVerts = object->cpvVertList.size();
	object->numVNormals = object->vNormalList.size();
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xafProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"KEY_FRAME_SET") == 0)
		return xafProcessKeyFrameSet(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::uvaProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"MAT_KEY_FRAME_SET") == 0)
		return uvaProcessMatAnimationTrack(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xlfProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"LIGHT") == 0)
		return xlfProcessLight(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xcfProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"CAMERA") == 0)
		return xcfProcessCamera(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xpsProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"MATERIAL") == 0)
		return xmfProcessMaterialObject(f);
	if (strcmp(chunkName,"PARTICLE_SYSTEM") == 0)
		return xpsProcessParticleSystem(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xpoProcessChunk(XS8* chunkName,FILE* f)
{
	if (strcmp(chunkName,"MX") == 0)
		return xpoProcessTransform(f);

	//Didn't recognize the chunk name.
	return false;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xafProcessKeyFrameSet(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];	
	XafKeyFrameSet boneKeyFrameSet;

	while(true)
	{
		fscanf(f,"%s",chunkValueName);

		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		//If we make it here then we have a valid chunk value to process
		//so we check for the specific value.
		if (strcmp(chunkValueName,"ID") == 0)
		{
			fscanf(f,"%u",&boneKeyFrameSet.ID);
		}
		/*else if (strcmp(chunkValueName,"REF_COORD_SYS_ROW_1") == 0)
		{
			fscanf(f,"%f %f %f",&boneKeyFrameSet.refCoordSys._11,
								&boneKeyFrameSet.refCoordSys._12,
								&boneKeyFrameSet.refCoordSys._13);
		}
		else if (strcmp(chunkValueName,"REF_COORD_SYS_ROW_2") == 0)
		{
			fscanf(f,"%f %f %f",&boneKeyFrameSet.refCoordSys._21,
								&boneKeyFrameSet.refCoordSys._22,
								&boneKeyFrameSet.refCoordSys._23);
		}
		else if (strcmp(chunkValueName,"REF_COORD_SYS_ROW_3") == 0)
		{
			fscanf(f,"%f %f %f",&boneKeyFrameSet.refCoordSys._31,
								&boneKeyFrameSet.refCoordSys._32,
								&boneKeyFrameSet.refCoordSys._33);
		}
		else if (strcmp(chunkValueName,"REF_COORD_SYS_ROW_4") == 0)
		{
			fscanf(f,"%f %f %f",&boneKeyFrameSet.refCoordSys._41,
								&boneKeyFrameSet.refCoordSys._42,
								&boneKeyFrameSet.refCoordSys._43);
		}*/
		else if (strcmp(chunkValueName,"NUM_ROT_KF") == 0)
		{
			XU32 numFrames;
			fscanf(f,"%u",&numFrames);
			if (numFrames > 0)
			{
				//Now read in the data
				XafRotationKey rotKey;
				for (XU32 i = 0;i < numFrames;i++)
				{					
					fscanf(f,"%*f %u %f %f %f %f",&rotKey.time,&rotKey.quat.x,&rotKey.quat.y,&rotKey.quat.z,&rotKey.quat.w);
					boneKeyFrameSet.rotKeyList.push_back(rotKey);
				}
			}
				
		}
		else if (strcmp(chunkValueName,"NUM_POS_KF") == 0)
		{
			XU32 numFrames;
			fscanf(f,"%u",&numFrames);
			if (numFrames > 0)
			{
				//Now read in the data
				XafPositionKey posKey;
				for (XU32 i = 0;i < numFrames;i++)
				{					
					fscanf(f,"%*f %u %f %f %f",&posKey.time,&posKey.vec.x,&posKey.vec.y,&posKey.vec.z);
					boneKeyFrameSet.posKeyList.push_back(posKey);
				}
			}	
		}
		else if (strcmp(chunkValueName,"NUM_SCA_KF") == 0)
		{
			XU32 numFrames;
			fscanf(f,"%u",&numFrames);
			if (numFrames > 0)
			{
				//Now read in the data
				XafScaleKey scaleKey;
				for (XU32 i = 0;i < numFrames;i++)
				{					
					fscanf(f,"%*f %u %f %f %f",&scaleKey.time,&scaleKey.vec.x,&scaleKey.vec.y,&scaleKey.vec.z);
					boneKeyFrameSet.scaleKeyList.push_back(scaleKey);
				}
			}	
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	//Push the new bone key frame list onto the animation track.
	m_model.m_animationTrack.keysList.push_back(boneKeyFrameSet);

	return true;
}


//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::uvaProcessMatAnimationTrack(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];	

	while(true)
	{
		fscanf(f,"%s",chunkValueName);

		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		//If we make it here then we have a valid chunk value to process
		//so we check for the specific value.
		if (strcmp(chunkValueName,"NUM_U_OFFSET_KF") == 0)
		{
			XU32 numFrames;
			fscanf(f,"%u",&numFrames);
			if (numFrames > 0)
			{
				//Now read in the data
				UvaFloatKey floatKey;
				for (XU32 i = 0;i < numFrames;i++)
				{					
					fscanf(f,"%*f %u %f",&floatKey.time,&floatKey.value);
					m_model.m_matAnimationTrack.uOffsetKeyList.push_back(floatKey);
				}
			}
				
		}
		else if (strcmp(chunkValueName,"NUM_V_OFFSET_KF") == 0)
		{
			XU32 numFrames;
			fscanf(f,"%u",&numFrames);
			if (numFrames > 0)
			{
				//Now read in the data
				UvaFloatKey floatKey;
				for (XU32 i = 0;i < numFrames;i++)
				{					
					fscanf(f,"%*f %u %f",&floatKey.time,&floatKey.value);
					m_model.m_matAnimationTrack.vOffsetKeyList.push_back(floatKey);
				}
			}	
		}
		else if (strcmp(chunkValueName,"NUM_W_ANGLE_KF") == 0)
		{
			XU32 numFrames;
			fscanf(f,"%u",&numFrames);
			if (numFrames > 0)
			{
				//Now read in the data
				UvaFloatKey floatKey;
				for (XU32 i = 0;i < numFrames;i++)
				{					
					fscanf(f,"%*f %u %f",&floatKey.time,&floatKey.value);
					m_model.m_matAnimationTrack.wAngleKeyList.push_back(floatKey);
				}
			}	
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xlfProcessLight(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];	
	  
	while(true)
	{
		fscanf(f,"%s",chunkValueName);

		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		//If we make it here then we have a valid chunk value to process
		//so we check for the specific value.
		if (strcmp(chunkValueName,"TYPE") == 0)
		{	  
			char buf[100];
			fscanf(f,"%s",buf);
			if (strcmp(buf,"SPOT") == 0)
				m_model.light.type = X_LT_SPOT;
			else if (strcmp(buf,"OMNI") == 0)
				m_model.light.type = X_LT_POINT;
			else if (strcmp(buf,"DIRECTIONAL") == 0)
				m_model.light.type = X_LT_DIRECTIONAL;
			else
				m_model.light.type = X_LT_POINT;
		}
		else if (strcmp(chunkValueName,"POSITION") == 0)
		{	  
			fscanf(f,"%f %f %f",&m_model.light.position.x,&m_model.light.position.y,&m_model.light.position.z);
		}
		else if (strcmp(chunkValueName,"DIRECTION") == 0)
		{		  
			fscanf(f,"%f %f %f",&m_model.light.direction.x,&m_model.light.direction.y,&m_model.light.direction.z);
		}
		else if (strcmp(chunkValueName,"DIFFUSE_COLOR") == 0)
		{				
			XF32 r,g,b;
			fscanf(f,"%f %f %f",&r,&g,&b);
			XColor color(r,g,b,1.0);
			m_model.light.diffuse = color;
		}
		else if (strcmp(chunkValueName,"AMBIENT_COLOR") == 0)
		{				 
			XF32 r,g,b;
			fscanf(f,"%f %f %f",&r,&g,&b);
			XColor color(r,g,b,1.0);
			m_model.light.ambient = color;
		}
		else if (strcmp(chunkValueName,"SPECULAR_COLOR") == 0)
		{					   
			XF32 r,g,b;
			fscanf(f,"%f %f %f",&r,&g,&b);
			XColor color(r,g,b,1.0);
			m_model.light.specular = color;
		}
		else if (strcmp(chunkValueName,"FALLOFF") == 0)
		{
			fscanf(f,"%f",&m_model.light.falloff);
		}
		else if (strcmp(chunkValueName,"INNER_CONE_ANGLE") == 0)
		{
			fscanf(f,"%f",&m_model.light.innerConeAngle);
		}
		else if (strcmp(chunkValueName,"OUTER_CONE_ANGLE") == 0)
		{
			fscanf(f,"%f",&m_model.light.outerConeAngle);
		}
		else if (strcmp(chunkValueName,"RANGE") == 0)
		{
			fscanf(f,"%f",&m_model.light.range);
		}
		else if (strcmp(chunkValueName,"ATTENUATION") == 0)
		{
			fscanf(f,"%f",&m_model.light.attenuation);	 
		}

		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xcfProcessCamera(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];
	char tmpBuf[MAX_TEMP_BUF_LENGTH];

	while(true)
	{
		fscanf(f,"%s",chunkValueName);
		
		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		//If we make it here then we have a valid chunk value to process
		//so we check for the specific value.
		if (strcmp(chunkValueName,"NAME") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
			
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(m_model.camera.pName,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(m_model.camera.pName,strippedString);
			}
		}
		else if (strcmp(chunkValueName,"MX_R1") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.camera.modelTransform._11,
								&m_model.camera.modelTransform._12,
								&m_model.camera.modelTransform._13);
		}
		else if (strcmp(chunkValueName,"MX_R2") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.camera.modelTransform._21,
								&m_model.camera.modelTransform._22,
								&m_model.camera.modelTransform._23);
		}
		else if (strcmp(chunkValueName,"MX_R3") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.camera.modelTransform._31,
								&m_model.camera.modelTransform._32,
								&m_model.camera.modelTransform._33);
		}
		else if (strcmp(chunkValueName,"MX_R4") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.camera.modelTransform._41,
								&m_model.camera.modelTransform._42,
								&m_model.camera.modelTransform._43);
		}
		else if (strcmp(chunkValueName,"HORIZONTAL_FOV") == 0)
		{
			fscanf(f,"%f",&m_model.camera.horizontalFOV);
		}
		else if (strcmp(chunkValueName,"NEAR_CLIP_PLANE") == 0)
		{
			fscanf(f,"%f",&m_model.camera.nearClipPlane);
		}
		else if (strcmp(chunkValueName,"FAR_CLIP_PLANE") == 0)
		{
			fscanf(f,"%f",&m_model.camera.farClipPlane);
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::xpsProcessParticleSystem(FILE* f)
{
	char chunkValueName[MAX_CHUNK_VALUE_NAME_LENGTH];
	char tmpBuf[MAX_TEMP_BUF_LENGTH];

	while(true)
	{
		fscanf(f,"%s",chunkValueName);
		
		//If we have reached a '@' that signifies the end of the chunk
		//or if we are at the end of the file then break;
		if ((strcmp(chunkValueName,"@") == 0) || (feof(f)))
			break;

		if (strcmp(chunkValueName,"NAME") == 0)
		{
			fgets(tmpBuf,MAX_TEMP_BUF_LENGTH,f);
			
			//Ingore the XF64 quotes that appear at the front and end
			//of the string. Also, ignore the space at the front of the string.
			tmpBuf[strlen(tmpBuf)-2] = '\0';
			XS8* strippedString = &tmpBuf[2];

			//If the string is "NULL" then there is no valid string
			if (strcmp("NULL",strippedString) != 0)
			{
				XNewArray(m_model.particleSystem.name,XS8,strlen(strippedString)+1,XOSMgr::getDefaultHeap());
				strcpy(m_model.particleSystem.name,strippedString);
			}
		}
		else if (strcmp(chunkValueName,"MX_R1") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.modelTransform._11,
								&m_model.particleSystem.modelTransform._12,
								&m_model.particleSystem.modelTransform._13);
		}
		else if (strcmp(chunkValueName,"MX_R2") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.modelTransform._21,
								&m_model.particleSystem.modelTransform._22,
								&m_model.particleSystem.modelTransform._23);
		}
		else if (strcmp(chunkValueName,"MX_R3") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.modelTransform._31,
								&m_model.particleSystem.modelTransform._32,
								&m_model.particleSystem.modelTransform._33);
		}
		else if (strcmp(chunkValueName,"MX_R4") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.modelTransform._41,
								&m_model.particleSystem.modelTransform._42,
								&m_model.particleSystem.modelTransform._43);
		}
		else if (strcmp(chunkValueName,"MATERIAL_ID") == 0)
		{
			fscanf(f,"%i",&m_model.particleSystem.materialID);
		}
		else if (strcmp(chunkValueName,"EMITTER_TYPE") == 0)
		{
			fscanf(f,"%u",&m_model.particleSystem.emitterType);
		}
		else if (strcmp(chunkValueName,"EMITTER_LENGTH") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.emitterLength);
		}
		else if (strcmp(chunkValueName,"EMITTER_WIDTH") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.emitterWidth);
		}
		else if (strcmp(chunkValueName,"EMITTER_HEIGHT") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.emitterHeight);
		}
		else if (strcmp(chunkValueName,"BIRTH_RATE") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.birthRate);
		}
		else if (strcmp(chunkValueName,"PARTICLE_START_ALPHA") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.startAlpha);
		}
		else if (strcmp(chunkValueName,"PARTICLE_END_ALPHA") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.endAlpha);
		}
		else if (strcmp(chunkValueName,"PARTICLE_START_COLOR") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.startColor.x,
								&m_model.particleSystem.startColor.y,
								&m_model.particleSystem.startColor.z);
		}
		else if (strcmp(chunkValueName,"PARTICLE_END_COLOR") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.endColor.x,
								&m_model.particleSystem.endColor.y,
								&m_model.particleSystem.endColor.z);
		}
		else if (strcmp(chunkValueName,"PARTICLE_GROW_TIME") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleGrowTime);
		}
		else if (strcmp(chunkValueName,"PARTICLE_SHRINK_TIME") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleShrinkTime);
		}
		else if (strcmp(chunkValueName,"PARTICLE_SPEED") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleSpeed);
		}
		else if (strcmp(chunkValueName,"PARTICLE_SPEED_VARIATION") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleSpeedVariation);
		}
		else if (strcmp(chunkValueName,"PARTICLE_LIFE") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleLife);
		}
		else if (strcmp(chunkValueName,"PARTICLE_LIFE_VARIATION") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleLifeVariation);
		}
		else if (strcmp(chunkValueName,"PARTICLE_SIZE") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleSize);
		}
		else if (strcmp(chunkValueName,"PARTICLE_SIZE_VARIATION") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.particleSizeVariation);
		}
		else if (strcmp(chunkValueName,"DIRECTION_VECTOR") == 0)
		{
			fscanf(f,"%f %f %f",&m_model.particleSystem.directionVec.x,
								&m_model.particleSystem.directionVec.y,
								&m_model.particleSystem.directionVec.z);
		}
		else if (strcmp(chunkValueName,"DIRECTION_VECTOR_VARIATION") == 0)
		{
			fscanf(f,"%f",&m_model.particleSystem.directionVecVariation);
		}
		//else we don't know the word so skip the rest of the line.
		else
			fgets(chunkValueName,MAX_CHUNK_VALUE_NAME_LENGTH,f);
	}

	return true;
}



//*****************************************************************************
//*****************************************************************************
MlVertexTexInfo* XModelLoader::getVertexTexInfoBlock(vector<MlVertexTexInfo>* list,XU32 index)
{
	for (XU32 i = 0;i < list->size();i++)
		if ((*list)[i].vertexIndex == index)
			return &(*list)[i];

	return NULL;	
}



//*****************************************************************************
//*****************************************************************************
XBOOL XModelLoader::texCoordIndexExists(MlVertexTexInfo* vtBlockPtr,XU32 index)
{
	for (XU32 i = 0;i < vtBlockPtr->texIndexList.size();i++)
	{
		if (vtBlockPtr->texIndexList[i] == index)
			return true;
	}

	return false;
}



//*****************************************************************************
//*****************************************************************************
void XModelLoader::xmfAppendSkinVertex(vector<XmfSkinVertex>* destList,XmfSkinVertex* data)
{
	//This function assumes that a blank skin vertex has already been pushed onto
	//the end of the vector.  Therefore we use the last element in the
	//vector.  

	//Now build the blank skin vertex in the last element in this list
	for (XU32 i = 0;i < data->boneIDList.size();i++)
	{
		(*destList)[destList->size()-1].boneIDList.push_back(data->boneIDList[i]);
		(*destList)[destList->size()-1].boneWeightList.push_back(data->boneWeightList[i]);
	}
}
