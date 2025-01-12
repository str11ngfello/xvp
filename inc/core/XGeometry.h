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

#ifndef __X_GEOMETRY_H__
#define __X_GEOMETRY_H__





//*****************************************************************************
//*****************************************************************************
class XGeometry
{
public:
	XHandle					m_vbHandle;
	XHandle					m_ibHandle;
	XHandle					m_matHandle;
	XVector3D*				m_pPositionCache;
	XVector3D*				m_pNormalCache;
	XVector2D*				m_pUVCache;
	XU16*					m_pIndexCache;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGeometry();
	~XGeometry();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XHandle vbHandle,XHandle ibHandle,XVector3D* pPositionCache,XVector3D* pNormalCache,XVector2D* pUVCache,
						XU16* pIndexCache,XHandle matHandle);
	XStatus destroy();
};

#endif