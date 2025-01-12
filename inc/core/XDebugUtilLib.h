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
//	Module Name:	XDebugUtilLib.h
//	Author:			Lynn Duke
//	Creation Date:	7-28-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_DEBUG_UTIL_LIB_H__
#define __X_DEBUG_UTIL_LIB_H__



#ifdef XVP_XDUL

#define X_DUL_NUM_CYLINDER_FLOATS 156
#define X_DUL_NUM_CYLINDER_VERTS (X_DUL_NUM_CYLINDER_FLOATS/6)

#define X_DUL_NUM_CYLINDER_CAP_FLOATS 216
#define X_DUL_NUM_CYLINDER_CAP_VERTS (X_DUL_NUM_CYLINDER_CAP_FLOATS/6)

#define X_DUL_NUM_SPHERE_VERTS 92
#define X_DUL_NUM_SPHERE_INDICES 540

#define X_DUL_NUM_SPHERE_POINTS_VERTS 512

#define X_DUL_NUM_BOX_VERTS	8
#define X_DUL_NUM_BOX_INDICES 24

#define X_DUL_NUM_TRANSFORM_VERTS	6

enum X_VIS_MODE_TYPE
{
	X_VMT_NONE,
	X_VMT_OVERDRAW,
	X_VMT_DRAW_CALLS,
	X_VMT_TEXTURE_SIZE,
	X_VMT_TRI_COMPLEXITY,
	X_VMT_SHADER_COMPLEXITY,
	X_VMT_BUDGETS
};
#define COLOR_ARRAY_SIZE 8

//*****************************************************************************
//*****************************************************************************
class XDUL
{
public:
	struct XPlainVertex
	{
		XVector3D	pos;		/*!< Vertex position */
		XU32		diffuse;	/*!< The vertex diffuse color. */
	};

private:

	struct VSConstants
	{
		XMatrix4x4 worldViewProjMatrix;
		XVector4D depthBias;

		VSConstants()
		{
			depthBias.x = 0.0f;
		}
	};

	struct PSConstants
	{
		XColor color;
	};

	static XPlainVertex		m_sphereVertices[X_DUL_NUM_SPHERE_VERTS];
	static XU16				m_sphereIndices[X_DUL_NUM_SPHERE_INDICES];
	static XPlainVertex		m_boxVertices[X_DUL_NUM_BOX_VERTS];
    static XU16				m_boxIndices[X_DUL_NUM_BOX_INDICES];
	static XPlainVertex		m_transformVertices[X_DUL_NUM_TRANSFORM_VERTS];
	//static XPlainVertex		m_cylinder[X_DUL_NUM_BOX_VERTS];


	static XHandle			m_vsHandle;
	static XHandle			m_psHandle;
	static XHandle			m_simpleVertColorVS;
	static XHandle			m_simpleVertColorPS;

	//Debug volumes 
	static XHandle			m_VSConstants;
	static XHandle			m_PSConstants;
	static XHandle			m_wireframeMat;
	static XHandle			m_noZMat;
	static XHandle			m_cylinderVBHandle;
	static XHandle			m_cylinderTopVBHandle;
	static XHandle			m_cylinderBottomVBHandle;
	static XHandle			m_sphereVBHandle;
	static XHandle			m_sphereIBHandle;
	static XHandle			m_spherePointsVBHandle;
	static XHandle			m_boxVBHandle;
	static XHandle			m_boxIBHandle;
	static XHandle			m_transformVBHandle;

	//Visualization
	static X_VIS_MODE_TYPE	m_visMode;
	static XMaterialColorSC m_visSC;
	static XHandle			m_overdrawMat;
	static XF32				m_overdrawThresh;
	static XHandle			m_solidColorMat;
	static XColor			m_colorArray[COLOR_ARRAY_SIZE];

public:
	static XStatus init();
	static XStatus destroy();
	static XStatus drawSphere(XScene* pScene,XVector3D& center,XF32 radius,XColor& color);
	static XStatus drawSpherePoints(XScene* pScene,XVector3D& center,XF32 radius,XColor& color);
	static XHandle getSphereMaterial();
	static XStatus drawSphere(XScene* pScene,XMatrix4x4 transform,XF32 radius,XColor& color);
	static XStatus drawSpherePoints(XScene* pScene,XMatrix4x4 transform,XF32 radius,XColor& color);
	static XStatus drawCylinder(XScene* pScene,XMatrix4x4 transform,XColor& color);
	static XStatus drawTransform(XScene* pScene,XMatrix4x4 transform,XBOOL depthTest = true);
	
	static XStatus drawAABB(XScene* pScene,XVector3D& min,XVector3D& max,XColor& color);
	static XStatus drawBox(XScene* pScene,XMatrix4x4 transform,XColor& color);

	static XStatus setVisualizationMode(X_VIS_MODE_TYPE visMode);
	static XStatus fixupRenderEventForVisualization(XRenderEvent& event);
	static XF32 getOverdrawThreshold();
	static XStatus setOverdrawThreshold(XF32 threshold);
};



#endif

#endif
