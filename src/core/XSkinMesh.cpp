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
//	Module Name:	XSkinMesh.cpp
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
Constructor for the skin mesh.  
*/
//*****************************************************************************
XSkinMesh::XSkinMesh()
{
}



//*****************************************************************************
/**
Destructor for the skin mesh.  
*/
//*****************************************************************************
XSkinMesh::~XSkinMesh()
{
}



//*****************************************************************************
//*****************************************************************************
X_SCENE_NODE_TYPE XSkinMesh::getType()
{
	static X_SCENE_NODE_TYPE type = X_SNT_MAKE(X_SNID_MODEL,X_SNID_SKIN_MESH);
	return type;
}



//*****************************************************************************
/**
Returns a string description of the skin mesh.  This is the
description that appears in log files and other debug output.

@return A string description of the skin mesh.
*/
//*****************************************************************************
XS8* XSkinMesh::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XSkinMesh";
	return description; 
}



//*****************************************************************************
/**
Gets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
/*XHandle XSkinMesh::getIndexBufferHandle()
{
	return m_ibHandle;
}*/



//*****************************************************************************
/**
Sets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
/*XStatus XSkinMesh::setIndexBufferHandle(XHandle indexBufferHandle)
{
	m_ibHandle = indexBufferHandle;
	return X_OK;
}
*/


//*****************************************************************************
//*****************************************************************************
XStatus XSkinMesh::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//Update the model object tree.
	m_nodeTree.update(dt,pScene,pMatrixStack);
	
	//Update the bounding sphere
	XMatrix4x4& rootBoneWorldTransform = m_pNodeArray[0]->getWorldTransform();
	m_boundingSphere.center.x = rootBoneWorldTransform._41;
	m_boundingSphere.center.y = rootBoneWorldTransform._42;
	m_boundingSphere.center.z = rootBoneWorldTransform._43;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinMesh::prepForRender(XF32 dt,XScene* pScene)
{
	X_DEBUG
	(
		if (X_TEST_ULONG_BIT(m_propertyFlags,X_SNP_RENDER_BOUNDING_SPHERE))
			XDUL::drawSphere(pScene,m_boundingSphere.center,m_boundingSphere.radius,XColor(0.0f,0.0f,0.0f,0.0f));
	)


	//Only if the skin mesh bounding sphere intersects the camera's frustum
	//do we actually render it
	if (X_TEST_ULONG_BIT(m_propertyFlags,X_SNP_FRUSTUM_CULL))
	{
		XCamera* pCamera = pScene->getActiveCamera();
		if (pCamera)
		{
			if (!pCamera->sphereInFrustumQuick(m_boundingSphere))
			{
				X_STATS(XVP::stats.numFrustumCullsPerFrame++;)
				return X_OK;
			}
		}
	}

	//Render only if render is allowed
	if (X_SNP_RENDER & m_propertyFlags)
		pScene->addRenderEvent(this,&((XSkinMeshTemplate*)m_pDataTemplate)->m_geometry,
		((XSkinMeshTemplate*)m_pDataTemplate)->m_geometry.m_matHandle	);

	//Add this node as a render client. 
	//SHADERCONVERT XVP::addRenderClient(m_matHandle,this);

	return X_OK;
}



//*****************************************************************************
/**
Renders a skin mesh.  This method is called by the scene graph
during the render pass.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XSkinMesh::render(XRenderEvent* pRenderEvent)
{
	XVP::GraphicsMgr.renderVertexBuffer(((XSkinMeshTemplate*)m_pDataTemplate)->m_geometry.m_vbHandle,
										((XSkinMeshTemplate*)m_pDataTemplate)->m_geometry.m_ibHandle);

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XSkinMesh::getNumBones()
{
	return ((XSkinMeshTemplate*)m_pDataTemplate)->m_numBones;
}



//*****************************************************************************
//*****************************************************************************
vector<XBone*>* XSkinMesh::getBones()
{
	return &m_boneIDIndexArray;
}
