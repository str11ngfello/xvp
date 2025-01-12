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
//	Module Name:	XModelObjectTemplate.cpp
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
Constructor for the model object template.  
*/
//*****************************************************************************
XModelObjectTemplate::XModelObjectTemplate()
{
	/*for (XU32 i = 0;i < X_MAX_MODEL_OBJECT_LODS;++i)
	{
		m_vbHandle[i] = X_INVALID_HANDLE;
		m_ibHandle[i] = X_INVALID_HANDLE;
		//m_texHandle[i] = X_INVALID_HANDLE;
		//m_matHandle[i] = X_INVALID_HANDLE;
	}
	m_texHandle = X_INVALID_HANDLE;
	m_matHandle = X_INVALID_HANDLE;
	m_matHandle = X_INVALID_HANDLE;*/
	m_lastLODIndex = 0;

	//There will be at least one LOD, so make space for it.
	vector<XGeometry*> geomList;
	m_pGeometryLists.push_back(geomList);
	m_isBone = false;
}



//*****************************************************************************
/**
Destructor for the model object template.  Destroys the vertex buffer,
index buffer and render state.
*/
//*****************************************************************************
XModelObjectTemplate::~XModelObjectTemplate()
{	
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObjectTemplate::destroy()
{
	//Destroy all geometry lists
	vector<vector<XGeometry*>>::iterator i = m_pGeometryLists.begin();
	for (;i != m_pGeometryLists.end();++i)
	{
		///For each piece of geometry in this list
		vector<XGeometry*>::iterator j = (*i).begin();
		for (;j != (*i).end();++j)
			XDelete(*j);
		(*i).clear();
	}
	m_pGeometryLists.clear();

	return X_OK;
}



#if 0
//*****************************************************************************
//*****************************************************************************
XStatus XModelObjectTemplate::setLODInfo(XU32 LODIndex,XHandle vbHandle,XHandle ibHandle/*,
										 XHandle texHandle,XHandle matHandle*/)
{
	if ((LODIndex > 0) && (LODIndex < X_MAX_MODEL_OBJECT_LODS))
	{
		m_vbHandle[LODIndex] = vbHandle;
		m_ibHandle[LODIndex] = ibHandle;
		//m_texHandle[LODIndex] = texHandle;
		//m_matHandle[LODIndex] = matHandle;
		m_lastLODIndex = LODIndex;
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}
#endif


//*****************************************************************************
/**
Sets the model object's hierarchy ID.

@param The new hierarchy ID.
@return None.
*/
//*****************************************************************************
XStatus XModelObjectTemplate::setHierarchyID(XU16 ID)
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
XU16 XModelObjectTemplate::getHierarchyID()
{
	return m_hierarchyID;
}


//*****************************************************************************
//*****************************************************************************
XStatus XModelObjectTemplate::addGeometry(XU32 LODIndex,XHandle vbHandle,XHandle ibHandle,
										  XVector3D* pPositionCache,XVector3D* pNormalCache,XVector2D* pUVCache,XU16* pIndexCache,
										  XHandle matHandle)
{
	XGeometry* pGeometry = NULL;
	XNew(pGeometry,XGeometry,XOSMgr::getDefaultHeap());
	pGeometry->init(vbHandle,ibHandle,pPositionCache,pNormalCache,pUVCache,pIndexCache,matHandle);

	//Add room for LOD
	if (m_pGeometryLists.size() <= LODIndex)
	{
		vector<XGeometry*> geomList;
		m_pGeometryLists.push_back(geomList);
		X_ASSERTM(m_pGeometryLists.size() == LODIndex+1,"Can't skip LODs when specifying geometry");

		//Record max LOD
		m_lastLODIndex = LODIndex;
	}

	m_pGeometryLists[LODIndex].push_back(pGeometry);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XModelObjectTemplate::getNumLODs()
{
	return m_pGeometryLists.size();
}



//*****************************************************************************
//*****************************************************************************
XStatus XModelObjectTemplate::getGeometryList(XU32 LODIndex,vector<XGeometry*>& geometryList)
{
	//If the LODIndex exists, then return the geometry list representing the LOD
	if (LODIndex < m_pGeometryLists.size())
	{
		geometryList = m_pGeometryLists[LODIndex];
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Gets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
/*XU32 XModelObjectTemplate::getMaterialHandle()
{
	//return m_matHandle[0];
	return m_matHandle;
}*/



//*****************************************************************************
/**
Gets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
/*XHandle XModelObjectTemplate::getVertexBufferHandle()
{
	return m_vbHandle[0];
}
*/


//*****************************************************************************
/**
Gets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
/*XHandle XModelObjectTemplate::getIndexBufferHandle()
{
	return m_ibHandle[0];
}
*/


//*****************************************************************************
/**
Sets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
/*XStatus XModelObjectTemplate::setMaterialHandle(XHandle materialHandle)
{
	//m_matHandle[0] = renderStateHandle;
	m_matHandle = materialHandle;
	return X_OK;
}
*/


//*****************************************************************************
/**
Sets the node's vertex buffer handle.

@return The vertex buffer handle of the node.
*/
//*****************************************************************************
/*XStatus XModelObjectTemplate::setVertexBufferHandle(XHandle vertexBufferHandle)
{
	m_vbHandle[0] = vertexBufferHandle;
	return X_OK;
}
*/


//*****************************************************************************
/**
Sets the node's index buffer handle.

@return The index buffer handle of the node.
*/
//*****************************************************************************
/*XStatus XModelObjectTemplate::setIndexBufferHandle(XHandle indexBufferHandle)
{
	m_ibHandle[0] = indexBufferHandle;
	return X_OK;
}
*/


