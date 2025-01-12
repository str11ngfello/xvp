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
//	Module Name:	XSkinObjectTemplate.cpp
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
XSkinObjectTemplate::XSkinObjectTemplate()
{
	m_pName = NULL;
	m_lastLODIndex = 0;

	//There will be at least one LOD, so make space for it.
	//vector<XGeometry*> geomList;
	//m_pGeometryLists.push_back(geomList);
}



//*****************************************************************************
/**
Destructor for the model object template.  Destroys the vertex buffer,
index buffer and render state.
*/
//*****************************************************************************
XSkinObjectTemplate::~XSkinObjectTemplate()
{	
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObjectTemplate::destroy()
{
	//Destroy all geometry lists
	/*vector<vector<XGeometry*>>::iterator i = m_pGeometryLists.begin();
	while (i.isValid())
	{
		XMemBlockList<XSkinGeometry*>::iterator j = i.getValue().begin();
		while (j.isValid())
		{
			XDelete(j.getValue());
			j.getNext();
		}
		i.getValue().clear();
	}
	m_pGeometryLists.clear();*/
	vector<vector<XSkinGeometry*>>::iterator i = m_pGeometryLists.begin();
	for (;i != m_pGeometryLists.end();++i)
	{
		///For each piece of geometry in this list
		vector<XSkinGeometry*>::iterator j = (*i).begin();
		for (;j != (*i).end();++j)
			XDelete(*j);
		(*i).clear();
	}
	m_pGeometryLists.clear();


	XOSMgr::destroyString(&m_pName);
	return X_OK;
}



XS8* XSkinObjectTemplate::getName()
{
	return m_pName;
}
void XSkinObjectTemplate::setName(XS8* pName)
{
	XOSMgr::destroyString(&m_pName);
	m_pName = XOSMgr::createString(pName);
}

//*****************************************************************************
//*****************************************************************************
XStatus XSkinObjectTemplate::addGeometry(XU32 LODIndex,XVertexDesc& desc,X_PRIMITIVE_TYPE pt,SkinVert* pOriginalVB,XHandle ibHandle,
										  XHandle matHandle,XU32 numVerts,
								XVector3D* pSkinnedVertCache,XU16* pIndexCache)
{
	XSkinGeometry* pGeometry = NULL;
	XNew(pGeometry,XSkinGeometry,XOSMgr::getDefaultHeap());
	pGeometry->init(desc,pt,pOriginalVB,ibHandle,matHandle,numVerts,pSkinnedVertCache,pIndexCache);

	//Add room for LOD
	if (m_pGeometryLists.size() <= LODIndex)
	{
		vector<XSkinGeometry*> geomList;
		m_pGeometryLists.push_back(geomList);
		X_ASSERTM(m_pGeometryLists.size() == LODIndex+1,"Can't skip LODs when specifying skin geometry");

		//Record max LOD
		m_lastLODIndex = LODIndex;
	}

	m_pGeometryLists.front().push_back(pGeometry);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XSkinObjectTemplate::getNumLODs()
{
	return m_pGeometryLists.size();
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkinObjectTemplate::getGeometryList(XU32 LODIndex,vector<XSkinGeometry*>& geometryList)
{
	//If the LODIndex exists, then return the geometry list representing the LOD
	if (LODIndex < m_pGeometryLists.size())
	{
		geometryList = m_pGeometryLists.front();
		return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}

#if 0
//*****************************************************************************
//*****************************************************************************
XStatus XSkinObjectTemplate::setLODInfo(XU32 LODIndex,XHandle vbHandle,XHandle ibHandle/*,
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



//*****************************************************************************
/**
Sets the model object's hierarchy ID.

@param The new hierarchy ID.
@return None.
*/
//*****************************************************************************
XStatus XSkinObjectTemplate::setHierarchyID(XU16 ID)
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
XU16 XSkinObjectTemplate::getHierarchyID()
{
	return m_hierarchyID;
}






//*****************************************************************************
/**
Gets the node's render state handle.

@return The render state handle of the node.
*/
//*****************************************************************************
/*XU32 XSkinObjectTemplate::getMaterialHandle()
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
/*XHandle XSkinObjectTemplate::getVertexBufferHandle()
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
/*XHandle XSkinObjectTemplate::getIndexBufferHandle()
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
/*XStatus XSkinObjectTemplate::setMaterialHandle(XHandle materialHandle)
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
/*XStatus XSkinObjectTemplate::setVertexBufferHandle(XHandle vertexBufferHandle)
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
/*XStatus XSkinObjectTemplate::setIndexBufferHandle(XHandle indexBufferHandle)
{
	m_ibHandle[0] = indexBufferHandle;
	return X_OK;
}
*/



#endif