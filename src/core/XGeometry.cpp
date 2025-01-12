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


	XGeometry::XGeometry()
	{
		m_vbHandle = X_INVALID_HANDLE;
		m_ibHandle = X_INVALID_HANDLE;
		m_matHandle = X_INVALID_HANDLE;
		m_pPositionCache = NULL;
		m_pNormalCache = NULL;
		m_pUVCache = NULL;
		m_pIndexCache = NULL;
	}

	XGeometry::~XGeometry()
	{
		destroy();
	}

	XStatus XGeometry::init(XHandle vbHandle,XHandle ibHandle,XVector3D* pPositionCache,XVector3D* pNormalCache,XVector2D* pUVCache,
						XU16* pIndexCache,XHandle matHandle)
	{
		destroy();
		m_vbHandle = vbHandle;
		m_ibHandle = ibHandle;
		m_matHandle = matHandle;
		m_pPositionCache = pPositionCache;
		m_pNormalCache = pNormalCache;
		m_pUVCache = pUVCache;
		m_pIndexCache = pIndexCache;
		return X_OK;
	}

	XStatus XGeometry::destroy()
	{
		if (m_vbHandle != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.destroyVertexBuffer(m_vbHandle);
			m_vbHandle = X_INVALID_HANDLE;
		}
		if (m_ibHandle != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.destroyIndexBuffer(m_ibHandle);
			m_ibHandle = X_INVALID_HANDLE;
		}
		if (m_matHandle != X_INVALID_HANDLE)
		{
			XVP::GraphicsMgr.destroyMaterial(m_matHandle);
			m_matHandle = X_INVALID_HANDLE;
		}

		//If we have cached geometry info, delete it
		if (m_pPositionCache)
			XDeleteArray(m_pPositionCache);
		if (m_pNormalCache)
			XDeleteArray(m_pNormalCache);
		if (m_pUVCache)
			XDeleteArray(m_pUVCache);
		if (m_pIndexCache)
			XDeleteArray(m_pIndexCache);

		return X_OK;
	}
