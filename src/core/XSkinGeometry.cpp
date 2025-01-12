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
//	Module Name:	X2DMovie.h
//	Author:			Lynn Duke
//	Creation Date:	4-29-06
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************


#include <xvpsdk.h>


	XSkinGeometry::XSkinGeometry()
	{
		m_pOriginalVB = NULL;
		m_ibHandle = X_INVALID_HANDLE;
		m_matHandle = X_INVALID_HANDLE;
		m_numVerts = 0;
		m_pSkinnedVertCache = NULL;
		m_pIndexCache = NULL;
	}

	XSkinGeometry::~XSkinGeometry()
	{
		destroy();
	}

	XStatus XSkinGeometry::init(XVertexDesc& desc,X_PRIMITIVE_TYPE pt,SkinVert* pOriginalVB,XHandle ibHandle,XHandle matHandle,XU32 numVerts,
								XVector3D* pSkinnedVertCache,XU16* pIndexCache)
	{
		destroy();
		m_vertexDesc = desc;
		m_primType = pt;
		m_pOriginalVB = pOriginalVB;
		m_ibHandle = ibHandle;
		m_matHandle = matHandle;
		m_numVerts = numVerts;
		m_pSkinnedVertCache = pSkinnedVertCache;
		m_pIndexCache = pIndexCache;
		return X_OK;
	}

	XStatus XSkinGeometry::destroy()
	{
		XDeleteArray(m_pOriginalVB);
		XVP::GraphicsMgr.destroyIndexBuffer(m_ibHandle);
		m_ibHandle = X_INVALID_HANDLE;
		XVP::GraphicsMgr.destroyMaterial(m_matHandle);
		m_matHandle = X_INVALID_HANDLE;

		//If we have cached geometry info, delete it
		if (m_pSkinnedVertCache)
			XDeleteArray(m_pSkinnedVertCache);
		if (m_pIndexCache)
			XDeleteArray(m_pIndexCache);

		return X_OK;
	}
