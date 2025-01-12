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
//	Module Name:	XSkinObject.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-10-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructor for the model object.  
*/
//*****************************************************************************
XSkinObject::XSkinObject()
{
	m_pName = NULL;
	m_hierarchyID = 0;
	m_LODIndex = 0;
	m_pDataTemplate = NULL;
	m_pParentModel = NULL; 
}



//*****************************************************************************
/**
Destructor for the model object. 
*/
//*****************************************************************************
XSkinObject::~XSkinObject()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::destroy()
{
	/*XMemBlockList<SkinInstance>::iterator i(m_skinInstances.begin());
	while(i.isValid())
	{
		XVP::GraphicsMgr.destroyVertexBuffer(i.getValue().skinnedVB);
		i.getNext();
	}
	m_skinInstances.clear();*/
	vector<SkinInstance>::iterator i = m_skinInstances.begin();
	for (;i != m_skinInstances.end();++i)
	{
		XVP::GraphicsMgr.destroyVertexBuffer((*i).skinnedVB);
	}

	XOSMgr::destroyString(&m_pName);

	return XSceneNode::destroy();
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XSkinObject::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL_OBJECT,X_SNID_MODEL_OBJECT);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the model object.  This is the
description that appears in log files and other debug output.

@return A string description of the model object.
*/
//*****************************************************************************
XS8* XSkinObject::getTypeDesc()
{
	//Declare a static description string for this object.  The base
	//object class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XSkinObject";
	return description; 
}



//*****************************************************************************
/**
Sets the model object's hierarchy ID.

@param The new hierarchy ID.
@return None.
*/
//*****************************************************************************
XStatus XSkinObject::setHierarchyID(XU16 ID)
{
	//Set the passed in ID
	m_hierarchyID = ID;
	return X_OK;
}

XStatus XSkinObject::setGeometryList(vector<XSkinGeometry*>& geometryList)
{
	//Create instance vertex buffers
	XHandle vbHandle;
	//vector<XSkinGeometry*>::iterator i(geometryList.begin());
	XSkinGeometry* pSkinGeom = NULL;
	for (XU32 i = 0;i < geometryList.size();++i)//while (i.isValid())
	{
		pSkinGeom = geometryList[i];//i.getValue();
		XVP::GraphicsMgr.createVertexBuffer(pSkinGeom->m_vertexDesc,pSkinGeom->m_primType,pSkinGeom->m_numVerts,true,vbHandle);
		SkinInstance si;
		si.skinnedVB = vbHandle;
		si.ibHandle = pSkinGeom->m_ibHandle;
		m_skinInstances.push_back(si);

		//Initialize the instance vertex buffer
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(vbHandle);
		pVB->lockAllStreams(X_LT_DISCARD);

		//Positions
		for (XU32 k = 0;k < pSkinGeom->m_numVerts;++k)
			pVB->setPosition(k,pSkinGeom->m_pOriginalVB[k].position);//,k*sizeof(XVector3D),sizeof(XVector3D));
		//Normals
		for (XU32 j = 0;j < pSkinGeom->m_vertexDesc.numNormals;++j)
			for (XU32 k = 0;k < pSkinGeom->m_numVerts;++k)
				pVB->setNormal(k,j,pSkinGeom->m_pOriginalVB[k].normal);//.normal,k*sizeof(XVector3D),sizeof(XVector3D));		
		//Colors
		for (XU32 j = 0;j < pSkinGeom->m_vertexDesc.numColors;++j)
			for (XU32 k = 0;k < pSkinGeom->m_numVerts;++k)
				pVB->setColor(k,j,pSkinGeom->m_pOriginalVB[k].color);//,k*sizeof(XU32),sizeof(XU32));
		//UVs
		for (XU32 j = 0;j < pSkinGeom->m_vertexDesc.numUVs;++j)
			for (XU32 k = 0;k < pSkinGeom->m_numVerts;++k)
				pVB->setUV(k,j,pSkinGeom->m_pOriginalVB[k].uv);//,k*sizeof(XVector2D),sizeof(XVector2D));
		//Tangents
		for (XU32 j = 0;j < pSkinGeom->m_vertexDesc.numTangents;++j)
			for (XU32 k = 0;k < pSkinGeom->m_numVerts;++k)
				pVB->setTangent(k,j,pSkinGeom->m_pOriginalVB[k].tangent);//k*sizeof(XVector3D),sizeof(XVector3D));
		//Binormals
		for (XU32 j = 0;j < pSkinGeom->m_vertexDesc.numBinormals;++j)
			for (XU32 k = 0;k < pSkinGeom->m_numVerts;++k)
				pVB->setBinormal(k,j,pSkinGeom->m_pOriginalVB[k].binormal);//,k*sizeof(XVector3D),sizeof(XVector3D));

		pVB->unlockAllStreams();
		//i.getNext();
	}
	return X_OK;
}

//*****************************************************************************
/**
Gets the model object's hierarchy ID.

@return The model object's hierarchy ID.
*/
//*****************************************************************************
XU16 XSkinObject::getHierarchyID()
{
	return m_hierarchyID;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::setTexMatrix(XMatrix4x4& texMatrix)
{
	m_texMatrix = texMatrix;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XSkinObject::getTexMatrix()
{
	return m_texMatrix;
}



//*****************************************************************************
/**
Gets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
/*XU32 XSkinObject::getMaterialHandle()
{
	return m_matHandle;
}*/



//*****************************************************************************
//*****************************************************************************
XModel* XSkinObject::getParentModel()
{
	return m_pParentModel;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared)
{
	return ((XModelTemplate*)(m_pParentModel->m_pDataTemplate))->getLODRanges(numLODRanges,pLODRangesSquared);
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::setLODIndex(XU8 LODIndex)
{
	/*XU8 lastLODIndex = ((XSkinObjectTemplate*)m_pDataTemplate)->m_lastLODIndex;
	if (LODIndex > lastLODIndex)
		m_LODIndex = lastLODIndex;
	else
		m_LODIndex = LODIndex;
*/
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU8 XSkinObject::getLODIndex()
{
	return m_LODIndex;
}

/*
//*****************************************************************************
//*****************************************************************************
XU32 XSkinObject::getNumLODs()
{
	return ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size();
}*/


/*
//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::getGeometryList(XU32 LODIndex,vector<XGeometry*>& geometryList)
{
	//If the LODIndex exists, then return the geometry list representing the LOD
	if (LODIndex < ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size())
	{
		geometryList = ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[LODIndex];
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}*/


//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::attachShaderController(XS8* pMaterialName,XShaderController* pMatController)
{
	//If we are looking for a specific material name
	XSkinObjectTemplate* pMot = ((XSkinObjectTemplate*)m_pDataTemplate);
	vector<XSkinGeometry*>::iterator i(pMot->m_pGeometryLists.front().begin());
	for (;i != pMot->m_pGeometryLists.front().end();++i)
	{
		XHandle matHandle = (*i)->m_matHandle;
		XS8* pMatName = XVP::GraphicsMgr.getMaterialName(matHandle);
		if (pMaterialName)
		{
			if (pMatName)
				if (strcmp(pMatName,pMaterialName) == 0)
					XVP::GraphicsMgr.getMaterialInterface(matHandle)->attachShaderController(pMatController);					
		}
		//else we are applying the controller to all materials
		else
			XVP::GraphicsMgr.getMaterialInterface(matHandle)->attachShaderController(pMatController);					
		//i.getNext();
	}
	
	return X_OK;
}

XS8* XSkinObject::getName()
{
	return m_pName;
}
void XSkinObject::setName(XS8* pName)
{
	XOSMgr::destroyString(&m_pName);
	m_pName = XOSMgr::createString(pName);
}
#if 0
//*****************************************************************************
/**
Gets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
XHandle XSkinObject::getVertexBufferHandle()
{
	return m_vbHandle;
}



//*****************************************************************************
/**
Gets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
XHandle XSkinObject::getIndexBufferHandle()
{
	return m_ibHandle;
}


//*****************************************************************************
/**
Sets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
XStatus XSkinObject::setMaterialHandle(XHandle materialHandle)
{
	m_matHandle = materialHandle;
	return X_OK;
}



//*****************************************************************************
/**
Sets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
XStatus XSkinObject::setVertexBufferHandle(XHandle vertexBufferHandle)
{
	m_vbHandle = vertexBufferHandle;
	return X_OK;
}



//*****************************************************************************
/**
Sets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
XStatus XSkinObject::setIndexBufferHandle(XHandle indexBufferHandle)
{
	m_ibHandle = indexBufferHandle;
	return X_OK;
}
#endif


//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::setBoundingSphere(XSphere& sphere)
{
	XSceneNode::setBoundingSphere(sphere);
	m_modelSpaceBoundingSphere = sphere;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::updateTransforms(XF32 dt, XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//We premultiply the model transform with the parent's transform.
	//This causes the node to inherit all motion of the parent in addition
	//to it's own.
	/*m_prevWorldTransform = m_worldTransform;
	m_worldTransform = m_modelTransform * 
		(m_pParentModel->m_pPose->getPoseTransform(m_hierarchyID)) *
		(*pMatrixStack->getBack());

	//Also, we push this nodes matrix on the stack so that children may
	//access it in the same manner.
	XMatrix4x4* pMat = pMatrixStack->insertBack();
	(*pMat) = m_worldTransform;*/

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XSkinObject::getPoseTransform()
{
	return m_pParentModel->m_pPose->getPoseTransform(m_hierarchyID);
}



//*****************************************************************************
/**
Updates the model object.  This gives the model object a chance to update
any internal, time-dependent data.  

@param dt	The amount of time, in seconds, that has passed since
					the last update.
@param pMatrixStack	The scene graph's matrix stack.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSkinObject::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	vector<XSkinGeometry*> geometryList;
	((XSkinObjectTemplate*)m_pDataTemplate)->getGeometryList(0,geometryList);
	//vector<XSkinGeometry*>::iterator itr(geometryList.begin());
	vector<SkinInstance>::iterator itr2(m_skinInstances.begin());

	XBOOL loop1 = true;
	XVector3D finalMin,finalMax;
	XVector3D min,max;
	
	for (XU32 itr = 0;itr < geometryList.size();++itr)//while (itr.isValid())
	{
		SkinVert* pSV = geometryList[itr]->m_pOriginalVB;//itr.getValue()->m_pOriginalVB;
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer((*itr2).skinnedVB);
		pVB->lockStream(X_ST_POSITION,0,X_LT_DISCARD);
		pVB->lockStream(X_ST_NORMAL,0,X_LT_DISCARD);
	
		XU32 numVerts = geometryList[itr]->m_numVerts;//itr.getValue()->m_numVerts;
		//Now start the actual skinning process.
		XVector3D position;
		XVector3D normal;
		XVector3D tangent;
		XVector3D binormal;

		if (pVB->getVertexDesc()->numTangents > 0)
		{
			pVB->lockStream(X_ST_TANGENT,0,X_LT_DISCARD);
			pVB->lockStream(X_ST_BINORMAL,0,X_LT_DISCARD);
			
			for(ULONG i = 0;i < numVerts;i++)
			{
				XVector3D skinnedPos(0,0,0);
				XVector3D skinnedNormal(0,0,0);
				XVector3D skinnedTangent(0,0,0);
				XVector3D skinnedBinormal(0,0,0);
				XMatrix4x4 accumulatedMat;
				accumulatedMat.setZero();

				//Run through the weight list for each vertex and update the final vertex
				//pSv = &m_pSkinMeshTemplate->m_skinVertexList[i];
				for(ULONG j = 0;j < pSV[i].numBones;++j)
				{
					//transform vertex with current state of the bone
					XBone* pBone = dynamic_cast<XBone*>(m_pParentModel->m_pNodeArray[(XU32)pSV[i].boneIndices[j]]);
					X_ASSERTM(pBone,"Bone index did not correspond to a valid bone object!");

					accumulatedMat += pBone->getSkinTransform() * pSV[i].boneWeights[j];
				}

				skinnedPos = accumulatedMat * pSV[i].position;

				//Multiple normals by inverse transpose then normalize them
				accumulatedMat.invert().transpose();
				skinnedNormal = accumulatedMat.transformVector3D(pSV[i].normal);
				skinnedTangent = accumulatedMat.transformVector3D(pSV[i].tangent);
				skinnedBinormal = accumulatedMat.transformVector3D(pSV[i].binormal);
				skinnedNormal.normalize();
				skinnedTangent.normalize();
				skinnedBinormal.normalize();

				//Set the new vertex position 
				pVB->setPosition(i,skinnedPos);//,i*sizeof(XVector3D),sizeof(XVector3D));
				pVB->setNormal(i,0,skinnedNormal);//,i*sizeof(XVector3D),sizeof(XVector3D));
				pVB->setTangent(i,0,skinnedTangent);//,i*sizeof(XVector3D),sizeof(XVector3D));
				pVB->setBinormal(i,0,skinnedBinormal);//,i*sizeof(XVector3D),sizeof(XVector3D));

	#ifdef XVP_EDITOR
				geometryList[itr]->m_pSkinnedVertCache[i] = skinnedPos;//itr.getValue()->m_pSkinnedVertCache[i] = skinnedPos;
	#endif

				//Update the aabb
				if (i == 0)
					min = max = skinnedPos;
				else
				{
					min.x = (min.x < skinnedPos.x) ? min.x : skinnedPos.x;
					min.y = (min.y < skinnedPos.y) ? min.y : skinnedPos.y;
					min.z = (min.z < skinnedPos.z) ? min.z : skinnedPos.z;
					max.x = (max.x > skinnedPos.x) ? max.x : skinnedPos.x;
					max.y = (max.y > skinnedPos.y) ? max.y : skinnedPos.y;
					max.z = (max.z > skinnedPos.z) ? max.z : skinnedPos.z;
				}
			}
		}
		else
		{
			for(ULONG i = 0;i < numVerts;i++)
			{
				XVector3D skinnedPos(0,0,0);
				XVector3D skinnedNormal(0,0,0);
				XMatrix4x4 accumulatedMat;
				accumulatedMat.setZero();

				//Run through the weight list for each vertex and update the final vertex
				//pSv = &m_pSkinMeshTemplate->m_skinVertexList[i];
				for(ULONG j = 0;j < pSV[i].numBones;++j)
				{
					//transform vertex with current state of the bone
					XBone* pBone = dynamic_cast<XBone*>(m_pParentModel->m_pNodeArray[(XU32)pSV[i].boneIndices[j]]);
					X_ASSERTM(pBone,"Bone index did not correspond to a valid bone object!");

					accumulatedMat += pBone->getSkinTransform() * pSV[i].boneWeights[j];
				}
				skinnedPos = accumulatedMat * pSV[i].position;

				//Multiple normals by inverse transpose then normalize them
				accumulatedMat.invert().transpose();
				skinnedNormal = accumulatedMat.transformVector3D(pSV[i].normal);
				skinnedNormal.normalize();

				//Set the new vertex position 
				pVB->setPosition(i,skinnedPos);//,i*sizeof(XVector3D),sizeof(XVector3D));
				pVB->setNormal(i,0,skinnedNormal);//,i*sizeof(XVector3D),sizeof(XVector3D));

	#ifdef XVP_EDITOR
				geometryList[itr]->m_pSkinnedVertCache[i] = skinnedPos;//itr.getValue()->m_pSkinnedVertCache[i] = skinnedPos;
	#endif

				//Update the aabb
				if (i == 0)
					min = max = skinnedPos;
				else
				{
					min.x = (min.x < skinnedPos.x) ? min.x : skinnedPos.x;
					min.y = (min.y < skinnedPos.y) ? min.y : skinnedPos.y;
					min.z = (min.z < skinnedPos.z) ? min.z : skinnedPos.z;
					max.x = (max.x > skinnedPos.x) ? max.x : skinnedPos.x;
					max.y = (max.y > skinnedPos.y) ? max.y : skinnedPos.y;
					max.z = (max.z > skinnedPos.z) ? max.z : skinnedPos.z;
				}
			}
		}
		
		if (loop1)
		{
			finalMin = min;
			finalMax = max;
		}
		else
		{
			finalMin.x = (finalMin.x < min.x) ? finalMin.x : min.x;
			finalMin.y = (finalMin.y < min.y) ? finalMin.y : min.y;
			finalMin.z = (finalMin.z < min.z) ? finalMin.z : min.z;
			finalMax.x = (finalMax.x > max.x) ? finalMax.x : max.x;
			finalMax.y = (finalMax.y > max.y) ? finalMax.y : max.y;
			finalMax.z = (finalMax.z > max.z) ? finalMax.z : max.z;
		}

		//itr.getNext();
		itr2++;//itr2.getNext();
		loop1 = false;

		pVB->unlockAllStreams();
	}

	m_modelSpaceBoundingSphere.center = (finalMax + finalMin) *.5f;
	m_modelSpaceBoundingSphere.radius = (finalMax - finalMin).mag() *.5f;
	m_boundingSphere = m_modelSpaceBoundingSphere;

	/*for (XU32 i = 0;i < numVerts;++i)
	{
		pVB->setPositionData(&pSV[i].position,i*sizeof(XVector3D),sizeof(XVector3D));
	}*/
	
	//Calculate new bounding sphere position	
	//m_boundingSphere.center = m_worldTransform * m_modelSpaceBSphCenter;
	XITransformSphere(m_boundingSphere,m_modelSpaceBoundingSphere,m_worldTransform);
	//XITransformAABB(m_aabb,m_modelSpaceAABB,m_worldTransform);
	//TransformOrientedBox(&m_obb,&m_modelSpaceOBB,m_worldTransform);

	
	//pScene->addPreRenderEvent(this);

	

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObject::prepForRender(XF32 dt,XScene* pScene)
{

	//Render only if render is allowed
	if (!(X_SNP_RENDER & m_propertyFlags) || (!(m_viewportMask & (1 << pScene->getActiveViewportIndex()))))
		return X_OK;	

	//Only if the model objects bounding sphere intersects the camera's frustum
	//do we actually render it
	XCamera* pCamera = pScene->getActiveCamera();
	if (X_TEST_ULONG_BIT(m_propertyFlags,X_SNP_FRUSTUM_CULL))
	{
		
		if (pCamera)
		{
			if (!pCamera->sphereInFrustumQuick(m_boundingSphere))
			{
				X_STATS(XVP::stats.numFrustumCullsPerFrame++;)
				return X_OK;
			}
		}
	}

	X_DEBUG
	(
		if (X_TEST_ULONG_BIT(m_propertyFlags,X_SNP_RENDER_BOUNDING_SPHERE))
			XDUL::drawSphere(pScene,m_boundingSphere.center,m_boundingSphere.radius,XColor(0.0f,0.0f,0.0f,1.0f));
	)
	
	vector<XSkinGeometry*> geometryList;
	((XSkinObjectTemplate*)m_pDataTemplate)->getGeometryList(0,geometryList);
	vector<XSkinGeometry*>::iterator itr(geometryList.begin());
	vector<SkinInstance>::iterator itr2(m_skinInstances.begin());
	for (XU32 i = 0;i < geometryList.size();++i)//while (itr.isValid())
	{
		pScene->addRenderEvent(this,&(*itr2),geometryList[i]->m_matHandle);
		//itr.getNext();
		itr2++;//itr2.getNext();
	}

	return X_OK;
}



//*****************************************************************************
/**
Renders the model object.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSkinObject::render(XRenderEvent* pRenderEvent)
{
	//Set world matrix for this node
	//XVP::GraphicsMgr.setWorldMatrix(m_worldTransform);

	//Now render this object with the appropriate LOD
	//XCamera* pCam = XVP::getActiveCamera();
	//if (pCam)
	//{
	//	XVector3D camPos,worldPos;
//		pCam->getWorldPosition(camPos);
//		getWorldPosition(worldPos);
//		XU32 LODIndex = m_pParentModel->m_pModelTemplate->getLODIndex(camPos.distanceSquared(worldPos));

	
	SkinInstance* pSI = ((SkinInstance*)pRenderEvent->pEventData);
	XVP::GraphicsMgr.renderVertexBuffer(pSI->skinnedVB,pSI->ibHandle);
	
	//XVP::GraphicsMgr.renderVertexBuffer(((XSkinObjectTemplate*)m_pDataTemplate)->m_vbHandle[m_LODIndex],
	//									 ((XSkinObjectTemplate*)m_pDataTemplate)->m_ibHandle[m_LODIndex]);
	
	
	
	
	
	//}

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
void XSkinObject::pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits)
{	
	if (((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size() == 0)
		return;

	//First, test the bounding sphere, if we intersect the bounding sphere then
	//we'll do a triangle check
	if (m_boundingSphere.radius > 0)
	{
		if (!XIRaySphereQuick(pickRay.origin,pickRay.normal,m_boundingSphere.center,m_boundingSphere.radius))
			return;
	}
	else
		return;

	//Transform pick ray from world space to model space
	XMatrix4x4 invWorld = m_worldTransform.getInverse();
	XVector4D v(pickRay.normal.x,pickRay.normal.y,pickRay.normal.z,0);
	v = v * invWorld;
	XVector4D v2(pickRay.origin.x,pickRay.origin.y,pickRay.origin.z,1);
	v2 = v2 * invWorld;
	XRay3D newRay(XVector3D(v2.x,v2.y,v2.z),XVector3D(v.x,v.y,v.z));

	XF32 baryU,baryV,baryW,time;

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(m_skinInstances[i].skinnedVB/*((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle*/);
		if (((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pSkinnedVertCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during pick");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		XVector3D* pPos = ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pSkinnedVertCache;
		XU16* pIdx = ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
		{
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
			X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");
	
			//Test the pick ray agains all triangles in the model object.
			XBOOL hitDetected = false;
			for (XU32 j = 0;j < numTris*3;j+=3)
			{
				//Check both front and back facing polys
				if (XIRayTriangle(pickRay.origin,pickRay.normal,pPos[pIdx[j]],pPos[pIdx[j+1]],pPos[pIdx[j+2]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j+2]]);
					hi.intersectionPoint = hi.intersectionPoint;
					hits.insertBack(hi);
				}
				if (XIRayTriangle(pickRay.origin,pickRay.normal,pPos[pIdx[j+2]],pPos[pIdx[j+1]],pPos[pIdx[j]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j+2]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j]]);
					hi.intersectionPoint = hi.intersectionPoint;
					hits.insertBack(hi);
				}
			}
		}
		else
		{
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
			X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");
	
			//Test the pick ray agains all triangles in the model object.
			XBOOL hitDetected = false;
			for (XU32 j = 0;j < numTris;++j)
			{
				//Check both front and back facing polys
				if (XIRayTriangle(pickRay.origin,pickRay.normal,pPos[pIdx[j]],pPos[pIdx[j+1]],pPos[pIdx[j+2]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j+2]]);
					hi.intersectionPoint = hi.intersectionPoint;
					hits.insertBack(hi);
				}
				if (XIRayTriangle(pickRay.origin,pickRay.normal,pPos[pIdx[j+2]],pPos[pIdx[j+1]],pPos[pIdx[j]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j+2]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j]]);
					hi.intersectionPoint = hi.intersectionPoint;
					hits.insertBack(hi);
				}
			}
		}
	}
}

XF32 XSkinObject::getSurfaceArea()
{
	XF32 surfaceArea = 0;

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(m_skinInstances[i].skinnedVB/*((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle*/);
		if (((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pSkinnedVertCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during getSurfaceArea");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
		else
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
		X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during getSurfaceArea");
			
		XVector3D* pPos = ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pSkinnedVertCache;
		XU16* pIdx = ((XSkinObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
		{
			for (XU32 j = 0;j < numTris*3;j+=3)
			{
				surfaceArea += XITriArea3D(m_worldTransform * pPos[pIdx[j]],
										   m_worldTransform * pPos[pIdx[j+1]],
										   m_worldTransform * pPos[pIdx[j+2]]);
			}
		}
		else
		{
			for (XU32 j = 0;j < numTris;++j)
			{
				surfaceArea += XITriArea3D(m_worldTransform * pPos[pIdx[j]],
										   m_worldTransform * pPos[pIdx[j+1]],
										   m_worldTransform * pPos[pIdx[j+2]]);
			}
		}
	}

	return surfaceArea;
}


XActorContainer* XSkinObject::getActorContainer()
{
	return m_pParentModel->getActorContainer();
}

