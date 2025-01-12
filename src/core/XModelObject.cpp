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
//	Module Name:	XModelObject.cpp
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
XModelObject::XModelObject()
{
	m_ppInstanceMats = NULL;
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
XModelObject::~XModelObject()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::destroy()
{
	if (m_pDataTemplate && m_ppInstanceMats)
	{
		vector<XGeometry*> geomList;
		for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->getNumLODs();++i)
		{
			((XModelObjectTemplate*)m_pDataTemplate)->getGeometryList(i,geomList);
			for (XU32 j = 0;j < geomList.size();++j)
			{
				//Destroy instanced material
				XVP::GraphicsMgr.destroyMaterial(m_ppInstanceMats[i][j]);
			}
			XDeleteArray(m_ppInstanceMats[i]);
		}
		XDeleteArray(m_ppInstanceMats)
	}

	return XSceneNode::destroy();
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XModelObject::getType()
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
XS8* XModelObject::getTypeDesc()
{
	//Declare a static description string for this object.  The base
	//object class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XModelObject";
	return description; 
}



//*****************************************************************************
/**
Sets the model object's hierarchy ID.

@param The new hierarchy ID.
@return None.
*/
//*****************************************************************************
XStatus XModelObject::setHierarchyID(XU16 ID)
{
	//Set the passed in ID
	m_hierarchyID = ID;
	return X_OK;
}



//*****************************************************************************
/**
Gets the model object's hierarchy ID.

@return The model object's hierarchy ID.
*/
//*****************************************************************************
XU16 XModelObject::getHierarchyID()
{
	return m_hierarchyID;
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::setTexMatrix(XMatrix4x4& texMatrix)
{
	m_texMatrix = texMatrix;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XModelObject::getTexMatrix()
{
	return m_texMatrix;
}



//*****************************************************************************
/**
Gets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
/*XU32 XModelObject::getMaterialHandle()
{
	return m_matHandle;
}*/



//*****************************************************************************
//*****************************************************************************
XModel* XModelObject::getParentModel()
{
	return m_pParentModel;
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::getLODRanges(XU32& numLODRanges,XF32** pLODRangesSquared)
{
	return ((XModelTemplate*)(m_pParentModel->m_pDataTemplate))->getLODRanges(numLODRanges,pLODRangesSquared);
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::setLODIndex(XU8 LODIndex)
{
	XU8 lastLODIndex = ((XModelObjectTemplate*)m_pDataTemplate)->m_lastLODIndex;
	if (LODIndex > lastLODIndex)
		m_LODIndex = lastLODIndex;
	else
		m_LODIndex = LODIndex;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU8 XModelObject::getLODIndex()
{
	return m_LODIndex;
}

/*
//*****************************************************************************
//*****************************************************************************
XU32 XModelObject::getNumLODs()
{
	return ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size();
}*/


/*
//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::getGeometryList(XU32 LODIndex,vector<XGeometry*>& geometryList)
{
	//If the LODIndex exists, then return the geometry list representing the LOD
	if (LODIndex < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size())
	{
		geometryList = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[LODIndex];
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}*/


//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::attachShaderController(XS8* pMaterialName,XShaderController* pMatController)
{
	X_ASSERTM(m_ppInstanceMats,"Cannot attach shader controller because this model object was not generated with instanced materials.  Attach the shader controller to the model template instead.");
		XModelObjectTemplate* pMot = ((XModelObjectTemplate*)m_pDataTemplate);
		for (XU32 j = 0;j < pMot->m_pGeometryLists.size();++j)
		{
			for (XU32 k = 0;k < pMot->m_pGeometryLists[j].size();++k)
			{
				//If we are looking for a specific material name
				if (pMaterialName)
				{
					XS8* matName = XVP::GraphicsMgr.getMaterialName(m_ppInstanceMats[j][k]);
					if (matName)
						if (strcmp(XVP::GraphicsMgr.getMaterialName(m_ppInstanceMats[j][k]),pMaterialName) == 0)
							XVP::GraphicsMgr.getMaterialInterface(m_ppInstanceMats[j][k])->attachShaderController(pMatController);					
				}
				//else we are applying the controller to all materials
				else
					XVP::GraphicsMgr.getMaterialInterface(m_ppInstanceMats[j][k])->attachShaderController(pMatController);					
			}
		}
	/*
	else
	{
		XModelObjectTemplate* pMot = ((XModelObjectTemplate*)m_pDataTemplate);
		for (XU32 j = 0;j < pMot->m_pGeometryLists.size();++j)
		{
			for (XU32 k = 0;k < pMot->m_pGeometryLists[j].size();++k)
			{
				//If we are looking for a specific material name
				if (pMaterialName)
				{
					XS8* matName = XVP::GraphicsMgr.getMaterialName(pMot->m_pGeometryLists[j][k]->m_matHandle);
					if (matName)
						if (strcmp(XVP::GraphicsMgr.getMaterialName(pMot->m_pGeometryLists[j][k]->m_matHandle),pMaterialName) == 0)
							XVP::GraphicsMgr.getMaterialInterface(pMot->m_pGeometryLists[j][k]->m_matHandle)->attachShaderController(pMatController);					
				}
				//else we are applying the controller to all materials
				else
					XVP::GraphicsMgr.getMaterialInterface(pMot->m_pGeometryLists[j][k]->m_matHandle)->attachShaderController(pMatController);					
			}
		}
	}*/
	return X_OK;
}


#if 0
//*****************************************************************************
/**
Gets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
XHandle XModelObject::getVertexBufferHandle()
{
	return m_vbHandle;
}



//*****************************************************************************
/**
Gets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
XHandle XModelObject::getIndexBufferHandle()
{
	return m_ibHandle;
}


//*****************************************************************************
/**
Sets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
XStatus XModelObject::setMaterialHandle(XHandle materialHandle)
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
XStatus XModelObject::setVertexBufferHandle(XHandle vertexBufferHandle)
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
XStatus XModelObject::setIndexBufferHandle(XHandle indexBufferHandle)
{
	m_ibHandle = indexBufferHandle;
	return X_OK;
}
#endif


//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::setBoundingSphere(XSphere& sphere)
{
	XSceneNode::setBoundingSphere(sphere);
	m_modelSpaceBoundingSphere = sphere;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObject::updateTransforms(XF32 dt, XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//We premultiply the model transform with the parent's transform.
	//This causes the node to inherit all motion of the parent in addition
	//to it's own.
	m_prevWorldTransform = m_worldTransform;
	m_worldTransform = m_modelTransform * 
		(m_pParentModel->m_pPose->getPoseTransform(m_hierarchyID)) *
		(*pMatrixStack->getBack());

	//Also, we push this nodes matrix on the stack so that children may
	//access it in the same manner.
	XMatrix4x4* pMat = pMatrixStack->insertBack();
	(*pMat) = m_worldTransform;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XMatrix4x4& XModelObject::getPoseTransform()
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
XStatus XModelObject::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	
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
XStatus XModelObject::prepForRender(XF32 dt,XScene* pScene)
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

	
	XU32 numLODRanges = 0;
	XF32* pLODRangesSquared = NULL;
	XU8 finalLODIndex = 0;
	XF32 distanceSquared = 0;

	//First, get the object's LOD ranges
	getLODRanges(numLODRanges,&pLODRangesSquared);

	//If it has LOD ranges...
	if (numLODRanges > 0)
	{
		XVector3D camPos,worldPos;
		pCamera->getWorldPosition(camPos);
		getWorldPosition(worldPos);
		distanceSquared = camPos.distanceSquared(worldPos);

		for (XS32 i = numLODRanges-1;i >= 0;--i)
		{
			if (distanceSquared >= pLODRangesSquared[i])
			{
				finalLODIndex = i+1;
				break;
			}
		}
				
		setLODIndex(finalLODIndex);
	}


	//XVP::addRenderClient(m_matHandle,this);
	XU32 size = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[m_LODIndex].size();
	if (m_ppInstanceMats)
	{
		for (XU32 i = 0; i < size;++i)
		{
			//X_DEBUG_LOGF("prepForRender %s\n",X_DLL_NONE,XVP::GraphicsMgr.getMaterialName(m_ppInstanceMats[m_LODIndex][i]));
				pScene->addRenderEvent(this,((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[m_LODIndex][i],m_ppInstanceMats[m_LODIndex][i]);			
		}
	}
	else
	{
		for (XU32 i = 0; i < size;++i)
		{
				pScene->addRenderEvent(this,((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[m_LODIndex][i],
									((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[m_LODIndex][i]->m_matHandle);			
		}
	}
	

	return X_OK;
}



//*****************************************************************************
/**
Renders the model object.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XModelObject::render(XRenderEvent* pRenderEvent)
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

	

	XVP::GraphicsMgr.renderVertexBuffer(((XGeometry*)pRenderEvent->pEventData)->m_vbHandle,
									((XGeometry*)pRenderEvent->pEventData)->m_ibHandle);
	
	//XVP::GraphicsMgr.renderVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_vbHandle[m_LODIndex],
	//									 ((XModelObjectTemplate*)m_pDataTemplate)->m_ibHandle[m_LODIndex]);
	
	
	
	
	
	//}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XModelObject::pick(XRay3D& pickRay,XMemBlockList<HitInfo>& hits)
{	
	if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size() == 0)
		return;

	//First, test the bounding sphere, if we intersect the bounding sphere then
	//we'll do a triangle check
	if (m_boundingSphere.radius > 0)
	{
		if (!XIRaySphereQuick(pickRay.origin,pickRay.normal,m_boundingSphere.center,m_boundingSphere.radius))
			return;
	}
	else
	{
		return;
	}

	//Transform pick ray from world space to model space
	XMatrix4x4 invWorld = m_worldTransform.getInverse();
	XVector4D v(pickRay.normal.x,pickRay.normal.y,pickRay.normal.z,0);
	v = v * invWorld;
	XVector4D v2(pickRay.origin.x,pickRay.origin.y,pickRay.origin.z,1);
	v2 = v2 * invWorld;
	XRay3D newRay(XVector3D(v2.x,v2.y,v2.z),XVector3D(v.x,v.y,v.z));

	XF32 baryU,baryV,baryW,time;

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle);
		if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during pick");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
		XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
		{
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
			X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");

			//Test the pick ray agains all triangles in the model object.
			XBOOL hitDetected = false;
			for (XU32 j = 0;j < numTris*3;j+=3)
			{
				//Check both front and back facing polys
				if (XIRayTriangle(newRay.origin,newRay.normal,pPos[pIdx[j]],pPos[pIdx[j+1]],pPos[pIdx[j+2]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j+2]]);
					hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
					hits.insertBack(hi);
				}
				if (XIRayTriangle(newRay.origin,newRay.normal,pPos[pIdx[j+2]],pPos[pIdx[j+1]],pPos[pIdx[j]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j+2]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j]]);
					hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
					hits.insertBack(hi);
				}
			}
		}
		else
		{
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
			X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");

			//Test the pick ray agains all triangles in the model object.
			XBOOL hitDetected = false;
			for (XU32 j = 0;j < numTris;++j)
			{
				//Check both front and back facing polys
				if (XIRayTriangle(newRay.origin,newRay.normal,pPos[pIdx[j]],pPos[pIdx[j+1]],pPos[pIdx[j+2]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j+2]]);
					hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
					hits.insertBack(hi);
				}
				if (XIRayTriangle(newRay.origin,newRay.normal,pPos[pIdx[j+2]],pPos[pIdx[j+1]],pPos[pIdx[j]],baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*pPos[pIdx[j+2]]) + (baryV*pPos[pIdx[j+1]]) + (baryW*pPos[pIdx[j]]);
					hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
					hits.insertBack(hi);
				}
			}
		}
			
		
	}
}



void XModelObject::pickLine(XVector3D& lineStart,XVector3D& lineEnd,XMemBlockList<HitInfo>& hits)
{	
	if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists.size() == 0)
		return;

	//First, test the bounding sphere, if we intersect the bounding sphere then
	//we'll do a triangle check
	if (m_boundingSphere.radius > 0)
	{
		XVector3D intersectionPoint;
		XF32 time;
		if (!XILineSphere(lineStart,lineEnd,m_boundingSphere.center,m_boundingSphere.radius,intersectionPoint,time))
			return;
	}
	else
	{
		return;
	}

	//Transform line from world space to model space
	XMatrix4x4 invWorld = m_worldTransform.getInverse();
	XVector3D xLineStart = invWorld * lineStart;
	XVector3D xLineEnd = invWorld * lineEnd;

	XF32 baryU,baryV,baryW,time;

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle);
		if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during pick");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
					XBOOL hitDetected = false;
			XVector3D a;
			XVector3D b;
			XVector3D c;
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
		{
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
			X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");
			XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
			XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

			//Test the pick ray agains all triangles in the model object.
			for (XU32 j = 0;j < numTris*3;j+=3)
			{
				//Check both front and back facing polys
				a = pPos[pIdx[j]];
				b = pPos[pIdx[j+1]];
				c = pPos[pIdx[j+2]];

				if (XILineTriangle(xLineStart,xLineEnd,a,b,c,baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*a) + (baryV*b) + (baryW*c);
					hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
					hits.insertBack(hi);
				}
				if (XILineTriangle(xLineStart,xLineEnd,c,b,a,baryU,baryV,baryW,time))
				{
					HitInfo hi;
					hi.pSceneNode = this;
					hi.intersectionPoint = (baryU*c) + (baryV*b) + (baryW*a);
					hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
					hits.insertBack(hi);
				}
			}
		}
		else
		{
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
			X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during pick");
			XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
			XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

			//Test the pick ray agains all triangles in the model object.
			for (XU32 j = 0;j < numTris;++j)
			{
				//Check both front and back facing polys
				a = pPos[pIdx[j]];
				b = pPos[pIdx[j+1]];
				c = pPos[pIdx[j+2]];
				if (XILineTriangle(xLineStart,xLineEnd,a,b,c,baryU,baryV,baryW,time))
				{
						HitInfo hi;
						hi.pSceneNode = this;
						hi.intersectionPoint = (baryU*a) + (baryV*b) + (baryW*c);
						hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
						hits.insertBack(hi);
				}
				if (XILineTriangle(xLineStart,xLineEnd,c,b,a,baryU,baryV,baryW,time))
				{
						HitInfo hi;
						hi.pSceneNode = this;
						hi.intersectionPoint = (baryU*c) + (baryV*b) + (baryW*a);
						hi.intersectionPoint = m_worldTransform * hi.intersectionPoint;
						hits.insertBack(hi);
			
				}
			}
		}
	}
}

int GetPixelCoordinate(float TextureCoord,XU32 texSize)
{
	int iPixel = (int)(TextureCoord * texSize);
	if (iPixel < 0)
		iPixel = 0;
	if (iPixel >= (int)texSize)
		iPixel = texSize-1;
	return iPixel;
}

float GetTextureCoordinate(int iPixelCoord,XU32 texSize)
{
	return (float(iPixelCoord)+0.5f)/float(texSize);
}

XVector3D GetBarycentricCoordinates(const XVector2D &P1, const XVector2D &P2, const XVector2D &P3, const XVector2D &P)
{
	XVector3D Coordinates(0.0f,0.0f,0.0f);
	float denom = (-P1.x * P3.y - P2.x * P1.y + P2.x * P3.y + P1.y * P3.x + P2.y * P1.x - P2.y * P3.x);

	if (fabs(denom) >= 1e-6)
	{
		Coordinates.x = (P2.x * P3.y - P2.y * P3.x - P.x * P3.y + P3.x * P.y - P2.x * P.y + P2.y * P.x) / denom;
		Coordinates.y = -(-P1.x * P.y + P1.x * P3.y + P1.y * P.x - P.x * P3.y + P3.x * P.y - P1.y * P3.x) / denom;
		Coordinates.z = (-P1.x * P.y + P2.y * P1.x + P2.x * P.y - P2.x * P1.y - P2.y * P.x + P1.y * P.x) / denom;
	}
	//Coordinates.z = 1 - Coordinates.x - Coordinates.y;

	return Coordinates;
}



void XModelObject::GetLightIntensity(XVector3D &Position, XVector3D &Normal,XColor& lightIntensity,XMemBlockList<XSceneNode*>& nodeList,XU32 numLights,XLight** ppLights)
{
	float Distance = 500000.0f;
	lightIntensity += XColor(.2f,.2f,.2f,1);
	XColor LightColor(1,1,1,1);
 
	XVector3D LightDirection(0.4f, -1.0f, -0.8f);
	LightDirection.normalize();

	LightColor *= X_MATH_MAX((-LightDirection).dot(Normal),0.0f);

	XVector3D Origin = Position;
	//(Distance * -LightDirection) - Position;

	//XF32 litPixelDepth = Origin.distance(Position);

	XMemBlockList<XSceneNode*>::iterator i(nodeList.begin());
	XMemBlockList<HitInfo> hits;
	XMemBlockList<HitInfo>::iterator hitItr;

	XBOOL occluded = false; 
	XRay3D pickRay(Origin,LightDirection);

	XVector3D lineStart = Position + (-LightDirection * .01f);
	XVector3D lineEnd = Position + (-LightDirection * (Distance));// - .01f));
	while (i.isValid())
	{
		i.getValue()->pickLine(lineStart,lineEnd,hits);
		
		if (hits.getNumItems() != 0)
		{
			occluded = true;
		}
		hits.clear();
		
		if (occluded)
			break;

		i.getNext();
	}

	if (!occluded)
		lightIntensity += LightColor;
	
	//Point Lights
	for (XU32 i = 0; i < numLights;++i)
	{
		if (ppLights[i]->m_lightType == X_LT_POINT)
		{
			XVector3D lightPos;
			ppLights[i]->getWorldPosition(lightPos);
			XVector3D lightVec = lightPos - Position;
			XVector3D lightDir = lightVec;
			lightDir.normalize();
			XF32 pointIntensity = X_MATH_MAX(lightDir.dot(Normal),0.0f);
			
			lightIntensity += ppLights[i]->m_diffuse * (1.0f - X_MATH_MIN((lightVec.mag()/ppLights[i]->m_params.x),1.0f)) * pointIntensity;	
		}
	}
}

void XModelObject::GetAmbientLightIntensity(XVector3D &Position, XVector3D &Normal,XColor& lightIntensity,XMemBlockList<XSceneNode*>& nodeList,XU32 numRays,XVector3D* pRays)
{
	//For each ray see if it is occluded by anything within a certain distance
	XU32 numRaysCollided = 0;
	for (XU32 i = 0;i < numRays;++i)
	{
		XMemBlockList<XSceneNode*>::iterator itr(nodeList.begin());
		XMemBlockList<HitInfo> hits;
		XMemBlockList<HitInfo>::iterator hitItr;

		XBOOL occluded = false; 

		XVector3D lineStart = Position + (pRays[i] * .01f);
		XVector3D lineEnd = Position + (pRays[i] * (1));
		while (itr.isValid())
		{
			itr.getValue()->pickLine(lineStart,lineEnd,hits);
			
			if (hits.getNumItems() != 0)
				++numRaysCollided;
		
			hits.clear();
			itr.getNext();
		}
		lightIntensity.m_R = 
		lightIntensity.m_G = 
		lightIntensity.m_B = 
		lightIntensity.m_A = 1.0f - (XF32)numRaysCollided/(XF32)numRays;
	}

	/*float Distance = 500000.0f;
	lightIntensity += XColor(.2f,.2f,.2f,1);
	XColor LightColor(1,1,1,1);
 
	XVector3D LightDirection(0.4f, -1.0f, -0.8f);
	LightDirection.normalize();

	LightColor *= X_MATH_MAX((-LightDirection).dot(Normal),0.0f);

	XVector3D Origin = Position;
	//(Distance * -LightDirection) - Position;

	//XF32 litPixelDepth = Origin.distance(Position);

	XMemBlockList<XSceneNode*>::iterator i(nodeList.begin());
	XMemBlockList<HitInfo> hits;
	XMemBlockList<HitInfo>::iterator hitItr;

	XBOOL occluded = false; 
	XRay3D pickRay(Origin,LightDirection);

	XVector3D lineStart = Position + (-LightDirection * .01f);
	XVector3D lineEnd = Position + (-LightDirection * (Distance));// - .01f));
	while (i.isValid())
	{
		i.getValue()->pickLine(lineStart,lineEnd,hits);
		
		if (hits.getNumItems() != 0)
		{
			occluded = true;
		}
		hits.clear();
		
		if (occluded)
			break;

		i.getNext();
	}

	if (!occluded)
		lightIntensity += LightColor;
	
	//Point Lights
	for (XU32 i = 0; i < numLights;++i)
	{
		if (ppLights[i]->m_lightType == X_LT_POINT)
		{
			XVector3D lightPos;
			ppLights[i]->getWorldPosition(lightPos);
			XVector3D lightVec = lightPos - Position;
			XVector3D lightDir = lightVec;
			lightDir.normalize();
			XF32 pointIntensity = X_MATH_MAX(lightDir.dot(Normal),0.0f);
			
			lightIntensity += ppLights[i]->m_diffuse * (1.0f - X_MATH_MIN((lightVec.mag()/ppLights[i]->m_params.x),1.0f)) * pointIntensity;	
		}
	}*/
}

void XModelObject::fillLightmap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numLights,XLight** ppLights)
{
	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle);
		if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during fillLightmap");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
		else
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
		X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during fillLightmap");
			
		XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
		XVector3D* pNormals = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pNormalCache;
		XVector2D* pUVs = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pUVCache;
		XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

		//Light each triangle
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
		{
			for (XU32 j = 0;j < numTris*3;j+=3)
			{
				XVector3D P1 = pPos[pIdx[j]];
				XVector3D P2 = pPos[pIdx[j+1]];
				XVector3D P3 = pPos[pIdx[j+2]];

				XVector3D N1 = pNormals[pIdx[j]];
				XVector3D N2 = pNormals[pIdx[j+1]];
				XVector3D N3 = pNormals[pIdx[j+2]];

				XVector2D T1 = pUVs[pIdx[j]];
				XVector2D T2 = pUVs[pIdx[j+1]];
				XVector2D T3 = pUVs[pIdx[j+2]];

				XVector2D TMin = T1, TMax = T1;
				TMin.min(T2);
				TMin.min(T3);
				TMax.max(T2);
				TMax.max(T3);
				int iMinX = GetPixelCoordinate(TMin.x,lm.width);
				int iMinY = GetPixelCoordinate(TMin.y,lm.width);
				int iMaxX = GetPixelCoordinate(TMax.x,lm.width);
				int iMaxY = GetPixelCoordinate(TMax.y,lm.width);
				int x, y;
				XVector2D TextureCoord;
				XVector3D BarycentricCoords;
				XVector3D Pos;
				XVector3D Normal;
				for (x=iMinX; x<=iMaxX; ++x)
				{
					for (y=iMinY; y<=iMaxY; ++y)
					{
						//bail if this pixel has already received light
						if (mask(x,y,0,0) == 1)
							continue;

						TextureCoord.x = GetTextureCoordinate(x,lm.width);
						TextureCoord.y = GetTextureCoordinate(y,lm.width);
						BarycentricCoords = GetBarycentricCoordinates(T1, T2, T3, TextureCoord);
						if (BarycentricCoords.x < 0 || BarycentricCoords.y < 0 || BarycentricCoords.z < 0)
							continue;
						if (BarycentricCoords.isZero())
							continue;
						Pos = BarycentricCoords.x * P1 + BarycentricCoords.y * P2 + BarycentricCoords.z * P3;
						Normal = BarycentricCoords.x * N1 + BarycentricCoords.y * N2 + BarycentricCoords.z * N3;
						Normal.normalize();
						XColor lightIntensity;
						GetLightIntensity(m_worldTransform * Pos, m_worldTransform.getInverse().getTranspose().transformVector3D(Normal).normalize(),lightIntensity,nodeList,numLights,ppLights);
						lm(x, y,0,0) = (unsigned char)(lightIntensity.m_R * 255);
						lm(x, y,0,1) = (unsigned char)(lightIntensity.m_G * 255);
						lm(x, y,0,2) = (unsigned char)(lightIntensity.m_B * 255);
						mask(x, y, 0, 0) = 1;
					}
				}
			}
		}
		else
		{
			for (XU32 j = 0;j < numTris;++j)
			{
				XVector3D P1 = pPos[pIdx[j]];
				XVector3D P2 = pPos[pIdx[j+1]];
				XVector3D P3 = pPos[pIdx[j+2]];

				XVector3D N1 = pNormals[pIdx[j]];
				XVector3D N2 = pNormals[pIdx[j+1]];
				XVector3D N3 = pNormals[pIdx[j+2]];

				XVector2D T1 = pUVs[pIdx[j]];
				XVector2D T2 = pUVs[pIdx[j+1]];
				XVector2D T3 = pUVs[pIdx[j+2]];

				XVector2D TMin = T1, TMax = T1;
				TMin.min(T2);
				TMin.min(T3);
				TMax.max(T2);
				TMax.max(T3);
				int iMinX = GetPixelCoordinate(TMin.x,lm.width);
				int iMinY = GetPixelCoordinate(TMin.y,lm.width);
				int iMaxX = GetPixelCoordinate(TMax.x,lm.width);
				int iMaxY = GetPixelCoordinate(TMax.y,lm.width);
				int x, y;
				XVector2D TextureCoord;
				XVector3D BarycentricCoords;
				XVector3D Pos;
				XVector3D Normal;
				for (x=iMinX; x<=iMaxX; ++x)
				{
					for (y=iMinY; y<=iMaxY; ++y)
					{
						//bail if this pixel has already received light
						if (mask(x,y,0,0) == 1)
							continue;

						TextureCoord.x = GetTextureCoordinate(x,lm.width);
						TextureCoord.y = GetTextureCoordinate(y,lm.width);
						BarycentricCoords = GetBarycentricCoordinates(T1, T2, T3, TextureCoord);
						if (BarycentricCoords.x < 0 || BarycentricCoords.y < 0 || BarycentricCoords.z < 0)
							continue;
						if (BarycentricCoords.isZero())
							continue;
						Pos = BarycentricCoords.x * P1 + BarycentricCoords.y * P2 + BarycentricCoords.z * P3;
						Normal = BarycentricCoords.x * N1 + BarycentricCoords.y * N2 + BarycentricCoords.z * N3;
						Normal.normalize();
						XColor lightIntensity;
						GetLightIntensity(m_worldTransform * Pos, m_worldTransform.getInverse().getTranspose().transformVector3D(Normal).normalize(),lightIntensity,nodeList,numLights,ppLights);
						lm(x, y,0,0) = (unsigned char)(lightIntensity.m_R * 255);
						lm(x, y,0,1) = (unsigned char)(lightIntensity.m_G * 255);
						lm(x, y,0,2) = (unsigned char)(lightIntensity.m_B * 255);
						mask(x, y, 0, 0) = 1;
					}
				}
			}
		}
	}
}

void XModelObject::fillAOMap(cimg_library::CImg<unsigned char>& lm,cimg_library::CImg<unsigned char>& mask,XMemBlockList<XSceneNode*>& nodeList,XU32 numRays,XVector3D* pRays)
{
	XVector3D transformedRays[2048];

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle);
		if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during fillLightmap");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
		else
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
		X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during fillLightmap");
			
		XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
		XVector3D* pNormals = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pNormalCache;
		XVector2D* pUVs = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pUVCache;
		XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

		//Light each triangle
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
		{
			for (XU32 j = 0;j < numTris*3;j+=3)
			{
				XVector3D P1 = pPos[pIdx[j]];
				XVector3D P2 = pPos[pIdx[j+1]];
				XVector3D P3 = pPos[pIdx[j+2]];

				XVector3D N1 = pNormals[pIdx[j]];
				XVector3D N2 = pNormals[pIdx[j+1]];
				XVector3D N3 = pNormals[pIdx[j+2]];

				XVector2D T1 = pUVs[pIdx[j]];
				XVector2D T2 = pUVs[pIdx[j+1]];
				XVector2D T3 = pUVs[pIdx[j+2]];

				XVector2D TMin = T1, TMax = T1;
				TMin.min(T2);
				TMin.min(T3);
				TMax.max(T2);
				TMax.max(T3);
				int iMinX = GetPixelCoordinate(TMin.x,lm.width);
				int iMinY = GetPixelCoordinate(TMin.y,lm.width);
				int iMaxX = GetPixelCoordinate(TMax.x,lm.width);
				int iMaxY = GetPixelCoordinate(TMax.y,lm.width);
				int x, y;
				XVector2D TextureCoord;
				XVector3D BarycentricCoords;
				XVector3D Pos;
				XVector3D Normal;
				for (x=iMinX; x<=iMaxX; ++x)
				{
					for (y=iMinY; y<=iMaxY; ++y)
					{
						//bail if this pixel has already received light
						if (mask(x,y,0,0) == 1)
							continue;

						TextureCoord.x = GetTextureCoordinate(x,lm.width);
						TextureCoord.y = GetTextureCoordinate(y,lm.width);
						BarycentricCoords = GetBarycentricCoordinates(T1, T2, T3, TextureCoord);
						if (BarycentricCoords.x < 0 || BarycentricCoords.y < 0 || BarycentricCoords.z < 0)
							continue;
						if (BarycentricCoords.isZero())
							continue;
						Pos = BarycentricCoords.x * P1 + BarycentricCoords.y * P2 + BarycentricCoords.z * P3;
						Normal = BarycentricCoords.x * N1 + BarycentricCoords.y * N2 + BarycentricCoords.z * N3;
						Normal.normalize();
						XColor lightIntensity;

						XMatrix4x4 normalTransformMatrix = m_worldTransform.getInverse().getTranspose();
						XVector3D transformedNormal = normalTransformMatrix.transformVector3D(Normal).normalize();

						//Build transformed rays by building a tangent frame
						XMatrix4x4 rayTransformMatrix;
						if (transformedNormal == XVector3D(0.0f,-1.0f,0.0f))
						{
							rayTransformMatrix.createXRotation(X_MATH_PI);
						}
						else if (transformedNormal != XVector3D(0.0f,1.0f,0.0f))
						{
							XVector3D right,look;
							right = transformedNormal.cross(XVector3D::YAxis).normalize();
							look = transformedNormal.cross(right).normalize();

							rayTransformMatrix._11 = right.x;
							rayTransformMatrix._12 = right.y;
							rayTransformMatrix._13 = right.z;
							rayTransformMatrix._21 = transformedNormal.x;
							rayTransformMatrix._22 = transformedNormal.y;
							rayTransformMatrix._23 = transformedNormal.z;
							rayTransformMatrix._31 = -look.x;
							rayTransformMatrix._32 = -look.y;
							rayTransformMatrix._33 = -look.z;


						}
						rayTransformMatrix.invert().transpose();
						
						for (XU32 ray = 0;ray <= numRays;++ray)
							transformedRays[ray] = rayTransformMatrix.transformVector3D(pRays[ray]);

						GetAmbientLightIntensity(m_worldTransform * Pos,transformedNormal ,lightIntensity,nodeList,numRays,transformedRays);
						lm(x, y,0,0) = (unsigned char)(lightIntensity.m_R * 255);
						lm(x, y,0,1) = (unsigned char)(lightIntensity.m_G * 255);
						lm(x, y,0,2) = (unsigned char)(lightIntensity.m_B * 255);
						mask(x, y, 0, 0) = 1;
					}
				}
			}
		}
		else
		{
			for (XU32 j = 0;j < numTris;++j)
			{
				XVector3D P1 = pPos[pIdx[j]];
				XVector3D P2 = pPos[pIdx[j+1]];
				XVector3D P3 = pPos[pIdx[j+2]];

				XVector3D N1 = pNormals[pIdx[j]];
				XVector3D N2 = pNormals[pIdx[j+1]];
				XVector3D N3 = pNormals[pIdx[j+2]];

				XVector2D T1 = pUVs[pIdx[j]];
				XVector2D T2 = pUVs[pIdx[j+1]];
				XVector2D T3 = pUVs[pIdx[j+2]];

				XVector2D TMin = T1, TMax = T1;
				TMin.min(T2);
				TMin.min(T3);
				TMax.max(T2);
				TMax.max(T3);
				int iMinX = GetPixelCoordinate(TMin.x,lm.width);
				int iMinY = GetPixelCoordinate(TMin.y,lm.width);
				int iMaxX = GetPixelCoordinate(TMax.x,lm.width);
				int iMaxY = GetPixelCoordinate(TMax.y,lm.width);
				int x, y;
				XVector2D TextureCoord;
				XVector3D BarycentricCoords;
				XVector3D Pos;
				XVector3D Normal;
				for (x=iMinX; x<=iMaxX; ++x)
				{
					for (y=iMinY; y<=iMaxY; ++y)
					{
						//bail if this pixel has already received light
						if (mask(x,y,0,0) == 1)
							continue;

						TextureCoord.x = GetTextureCoordinate(x,lm.width);
						TextureCoord.y = GetTextureCoordinate(y,lm.width);
						BarycentricCoords = GetBarycentricCoordinates(T1, T2, T3, TextureCoord);
						if (BarycentricCoords.x < 0 || BarycentricCoords.y < 0 || BarycentricCoords.z < 0)
							continue;
						if (BarycentricCoords.isZero())
							continue;
						Pos = BarycentricCoords.x * P1 + BarycentricCoords.y * P2 + BarycentricCoords.z * P3;
						Normal = BarycentricCoords.x * N1 + BarycentricCoords.y * N2 + BarycentricCoords.z * N3;
						Normal.normalize();
						XColor lightIntensity;

						XMatrix4x4 normalTransformMatrix = m_worldTransform.getInverse().getTranspose();
						XVector3D transformedNormal = normalTransformMatrix.transformVector3D(Normal).normalize();

						//Build transformed rays by building a tangent frame
						XMatrix4x4 rayTransformMatrix;
						if (transformedNormal != XVector3D(0.0f,1.0f,0.0f))
						{
							XVector3D right,look;
							right = transformedNormal.cross(XVector3D::YAxis);
							look = transformedNormal.cross(right);

							rayTransformMatrix._11 = right.x;
							rayTransformMatrix._12 = right.y;
							rayTransformMatrix._13 = right.z;
							rayTransformMatrix._21 = transformedNormal.x;
							rayTransformMatrix._22 = transformedNormal.y;
							rayTransformMatrix._23 = transformedNormal.z;
							rayTransformMatrix._31 = look.x;
							rayTransformMatrix._32 = look.y;
							rayTransformMatrix._33 = look.z;

							rayTransformMatrix.invert().transpose();
						}

						for (XU32 ray = 0;ray <= numRays;++ray)
							transformedRays[ray] = rayTransformMatrix.transformVector3D(pRays[ray]);

						GetAmbientLightIntensity(m_worldTransform * Pos,transformedNormal ,lightIntensity,nodeList,numRays,transformedRays);
						lm(x, y,0,0) = (unsigned char)(lightIntensity.m_R * 255);
						lm(x, y,0,1) = (unsigned char)(lightIntensity.m_G * 255);
						lm(x, y,0,2) = (unsigned char)(lightIntensity.m_B * 255);
						mask(x, y, 0, 0) = 1;
					}
				}
			}
		}
	}
}

XF32 XModelObject::getSurfaceArea()
{
	XF32 surfaceArea = 0;

	//For each geometry primitive in lod 0
	for (XU32 i = 0;i < ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0].size();++i)
	{
		//Discover what type of primitive makes up our vertex buffer then calculate the
		//number of triangles
		XVertexBuffer* pVB = XVP::GraphicsMgr.getVertexBuffer(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_vbHandle);
		if (((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache == NULL)
			continue;

		XU32 numTris = 0;
		X_ASSERTM(pVB->getNumVerts() != 0,"0 length vertex buffer found during getSurfaceArea");
		X_ASSERTM(pVB->getPrimType() == X_PT_TRIANGLE_LIST || pVB->getPrimType() == X_PT_TRIANGLE_STRIP,"Vertex buffer needs to be triangle list or triangle strip");
		if (pVB->getPrimType() == X_PT_TRIANGLE_LIST)
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) / 3;
		else
			numTris = XVP::GraphicsMgr.getIndexBufferLength(((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_ibHandle) - 2;
		X_ASSERTM(numTris != 0,"No triangles found in vertex buffer during getSurfaceArea");
			
		XVector3D* pPos = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pPositionCache;
		XU16* pIdx = ((XModelObjectTemplate*)m_pDataTemplate)->m_pGeometryLists[0][i]->m_pIndexCache;

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

XActorContainer* XModelObject::getActorContainer()
{
	return m_pParentModel->getActorContainer();
}

