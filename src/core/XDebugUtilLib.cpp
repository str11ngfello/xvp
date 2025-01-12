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
//	Module Name:	XDebugUtilLib.cpp
//	Author:			Lynn Duke
//	Creation Date:	7-28-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

#ifdef XVP_XDUL


XHandle			XDUL::m_VSConstants = X_INVALID_HANDLE;
XHandle			XDUL::m_PSConstants = X_INVALID_HANDLE;
XHandle			XDUL::m_noZMat = X_INVALID_HANDLE;
XHandle			XDUL::m_wireframeMat = X_INVALID_HANDLE;
XHandle			XDUL::m_vsHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_psHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_simpleVertColorVS = X_INVALID_HANDLE;
XHandle			XDUL::m_simpleVertColorPS = X_INVALID_HANDLE;
XHandle			XDUL::m_cylinderVBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_cylinderTopVBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_cylinderBottomVBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_sphereVBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_sphereIBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_spherePointsVBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_boxVBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_boxIBHandle = X_INVALID_HANDLE;
XHandle			XDUL::m_transformVBHandle = X_INVALID_HANDLE;

X_VIS_MODE_TYPE XDUL::m_visMode = X_VMT_NONE;
XMaterialColorSC XDUL::m_visSC;
XHandle			XDUL::m_overdrawMat = X_INVALID_HANDLE;
XF32			XDUL::m_overdrawThresh = 8.0f;
XHandle			XDUL::m_solidColorMat = X_INVALID_HANDLE;



XDUL::XPlainVertex XDUL::m_sphereVertices[X_DUL_NUM_SPHERE_VERTS] =
{
	{XVector3D(0, 1, 0),0xFFFFFFFF},
	{XVector3D(0, 0.913545f,0.406737f),0xFFFFFFFF},
	{XVector3D(-0.165435f, 0.913545f,0.371572f),0xFFFFFFFF},
	{XVector3D(-0.302264f, 0.913545f,0.27216f),0xFFFFFFFF},
	{XVector3D(-0.38683f, 0.913545f,0.125689f),0xFFFFFFFF},
	{XVector3D(-0.404509f, 0.913545f,-0.0425156f),0xFFFFFFFF},
	{XVector3D(-0.352244f, 0.913545f,-0.203368f),0xFFFFFFFF},
	{XVector3D(-0.239074f, 0.913545f,-0.329057f),0xFFFFFFFF},
	{XVector3D(-0.0845653f, 0.913545f,-0.397848f),0xFFFFFFFF},
	{XVector3D(0.0845654f, 0.913545f,-0.397848f),0xFFFFFFFF},
	{XVector3D(0.239074f, 0.913545f,-0.329057f),0xFFFFFFFF},
	{XVector3D(0.352244f, 0.913545f,-0.203368f),0xFFFFFFFF},
	{XVector3D(0.404509f, 0.913545f,-0.0425155f),0xFFFFFFFF},
	{XVector3D(0.38683f, 0.913545f,0.125689f),0xFFFFFFFF},
	{XVector3D(0.302264f, 0.913545f,0.27216f),0xFFFFFFFF},
	{XVector3D(0.165435f, 0.913545f,0.371572f),0xFFFFFFFF},
	{XVector3D(0, 0.669131f,0.743145f),0xFFFFFFFF},
	{XVector3D(-0.302264f, 0.669131f,0.678897f),0xFFFFFFFF},
	{XVector3D(-0.552264f, 0.669131f,0.497261f),0xFFFFFFFF},
	{XVector3D(-0.706773f, 0.669131f,0.229644f),0xFFFFFFFF},
	{XVector3D(-0.739074f, 0.669131f,-0.0776799f),0xFFFFFFFF},
	{XVector3D(-0.643582f, 0.669131f,-0.371572f),0xFFFFFFFF},
	{XVector3D(-0.43681f, 0.669131f,-0.601217f),0xFFFFFFFF},
	{XVector3D(-0.154508f, 0.669131f,-0.726905f),0xFFFFFFFF},
	{XVector3D(0.154509f, 0.669131f,-0.726905f),0xFFFFFFFF},
	{XVector3D(0.43681f, 0.669131f,-0.601217f),0xFFFFFFFF},
	{XVector3D(0.643582f, 0.669131f,-0.371572f),0xFFFFFFFF},
	{XVector3D(0.739074f, 0.669131f,-0.0776797f),0xFFFFFFFF},
	{XVector3D(0.706773f, 0.669131f,0.229645f),0xFFFFFFFF},
	{XVector3D(0.552264f, 0.669131f,0.497261f),0xFFFFFFFF},
	{XVector3D(0.302264f, 0.669131f,0.678897f),0xFFFFFFFF},
	{XVector3D(0, 0.309017f,0.951057f),0xFFFFFFFF},
	{XVector3D(-0.38683f, 0.309017f,0.868833f),0xFFFFFFFF},
	{XVector3D(-0.706773f, 0.309017f,0.636381f),0xFFFFFFFF},
	{XVector3D(-0.904509f, 0.309017f,0.293893f),0xFFFFFFFF},
	{XVector3D(-0.945847f, 0.309017f,-0.0994126f),0xFFFFFFFF},
	{XVector3D(-0.823639f, 0.309017f,-0.475528f),0xFFFFFFFF},
	{XVector3D(-0.559017f, 0.309017f,-0.769421f),0xFFFFFFFF},
	{XVector3D(-0.197736f, 0.309017f,-0.930274f),0xFFFFFFFF},
	{XVector3D(0.197736f, 0.309017f,-0.930274f),0xFFFFFFFF},
	{XVector3D(0.559017f, 0.309017f,-0.769421f),0xFFFFFFFF},
	{XVector3D(0.823639f, 0.309017f,-0.475528f),0xFFFFFFFF},
	{XVector3D(0.945847f, 0.309017f,-0.0994123f),0xFFFFFFFF},
	{XVector3D(0.904508f, 0.309017f,0.293893f),0xFFFFFFFF},
	{XVector3D(0.706773f, 0.309017f,0.636381f),0xFFFFFFFF},
	{XVector3D(0.386829f, 0.309017f,0.868833f),0xFFFFFFFF},
	{XVector3D(0, -0.104529f,0.994522f),0xFFFFFFFF},
	{XVector3D(-0.404509f, -0.104529f,0.908541f),0xFFFFFFFF},
	{XVector3D(-0.739074f,-0.104529f,0.665465f),0xFFFFFFFF},
	{XVector3D(-0.945847f,-0.104529f,0.307324f),0xFFFFFFFF},
	{XVector3D(-0.989074f,-0.104529f,-0.103956f),0xFFFFFFFF},
	{XVector3D(-0.861281f,-0.104529f,-0.497261f),0xFFFFFFFF},
	{XVector3D(-0.584565f,-0.104529f,-0.804585f),0xFFFFFFFF},
	{XVector3D(-0.206773f,-0.104529f,-0.972789f),0xFFFFFFFF},
	{XVector3D(0.206773f,-0.104529f,-0.972789f),0xFFFFFFFF},
	{XVector3D(0.584565f,-0.104529f,-0.804585f),0xFFFFFFFF},
	{XVector3D(0.861281f,-0.104529f,-0.497261f),0xFFFFFFFF},
	{XVector3D(0.989074f,-0.104529f,-0.103956f),0xFFFFFFFF},
	{XVector3D(0.945846f,-0.104529f,0.307324f),0xFFFFFFFF},
	{XVector3D(0.739074f,-0.104529f,0.665465f),0xFFFFFFFF},
	{XVector3D(0.404508f,-0.104529f,0.908541f),0xFFFFFFFF},
	{XVector3D(0,-0.5f,0.866025f),0xFFFFFFFF},
	{XVector3D(-0.352244f,-0.5f,0.791154f),0xFFFFFFFF},
	{XVector3D(-0.643582f,-0.5f,0.579484f),0xFFFFFFFF},
	{XVector3D(-0.823639f,-0.5f,0.267617f),0xFFFFFFFF},
	{XVector3D(-0.861281f,-0.5f,-0.0905244f),0xFFFFFFFF},
	{XVector3D(-0.75f,-0.5f,-0.433013f),0xFFFFFFFF},
	{XVector3D(-0.509037f,-0.5f,-0.700629f),0xFFFFFFFF},
	{XVector3D(-0.180057f,-0.5f,-0.847101f),0xFFFFFFFF},
	{XVector3D(0.180057f,-0.5f,-0.847101f),0xFFFFFFFF},
	{XVector3D(0.509037f,-0.5f,-0.700629f),0xFFFFFFFF},
	{XVector3D(0.75f,-0.5f,-0.433013f),0xFFFFFFFF},
	{XVector3D(0.861281f,-0.5f,-0.0905242f),0xFFFFFFFF},
	{XVector3D(0.823639f,-0.5f,0.267617f),0xFFFFFFFF},
	{XVector3D(0.643582f,-0.5f,0.579484f),0xFFFFFFFF},
	{XVector3D(0.352244f,-0.5f,0.791154f),0xFFFFFFFF},
	{XVector3D(0,-0.809017f,0.587785f),0xFFFFFFFF},
	{XVector3D(-0.239074f,-0.809017f,0.536968f),0xFFFFFFFF},
	{XVector3D(-0.43681f,-0.809017f,0.393305f),0xFFFFFFFF},
	{XVector3D(-0.559017f,-0.809017f,0.181636f),0xFFFFFFFF},
	{XVector3D(-0.584565f,-0.809017f,-0.0614403f),0xFFFFFFFF},
	{XVector3D(-0.509037f,-0.809017f,-0.293893f),0xFFFFFFFF},
	{XVector3D(-0.345491f,-0.809017f,-0.475528f),0xFFFFFFFF},
	{XVector3D(-0.122207f,-0.809017f,-0.574941f),0xFFFFFFFF},
	{XVector3D(0.122207f,-0.809017f,-0.574941f),0xFFFFFFFF},
	{XVector3D(0.345492f,-0.809017f,-0.475528f),0xFFFFFFFF},
	{XVector3D(0.509037f,-0.809017f,-0.293893f),0xFFFFFFFF},
	{XVector3D(0.584565f,-0.809017f,-0.0614402f),0xFFFFFFFF},
	{XVector3D(0.559017f,-0.809017f,0.181636f),0xFFFFFFFF},
	{XVector3D(0.436809f,-0.809017f,0.393305f),0xFFFFFFFF},
	{XVector3D(0.239074f,-0.809017f,0.536969f),0xFFFFFFFF},
	{XVector3D(0,-1,0),0xFFFFFFFF}
};


XU16 XDUL::m_sphereIndices[X_DUL_NUM_SPHERE_INDICES] =
{ 
	2,1,0,3,2,0,4,3,0,5,4,0,6,5,0,7,6,0,8,7,0,9,8,0,10,9,0,11,10,0,12,11,0,13,12,0,14,13,0
	,15,14,0,1,15,0,17,16,1,2,17,1,18,17,2,3,18,2,19,18,3,4,19,3,20,19,4,5,20,4,21,20,5
	,6,21,5,22,21,6,7,22,6,23,22,7,8,23,7,24,23,8,9,24,8,25,24,9,10,25,9,26,25,10,11,26,10
	,27,26,11,12,27,11,28,27,12,13,28,12,29,28,13,14,29,13,30,29,14,15,30,14,16,30,15
	,1,16,15,32,31,16,17,32,16,33,32,17,18,33,17,34,33,18,19,34,18,35,34,19,20,35,19
	,36,35,20,21,36,20,37,36,21,22,37,21,38,37,22,23,38,22,39,38,23,24,39,23,40,39,24
	,25,40,24,41,40,25,26,41,25,42,41,26,27,42,26,43,42,27,28,43,27,44,43,28,29,44,28
	,45,44,29,30,45,29,31,45,30,16,31,30,47,46,31,32,47,31,48,47,32,33,48,32,49,48,33
	,34,49,33,50,49,34,35,50,34,51,50,35,36,51,35,52,51,36,37,52,36,53,52,37,38,53,37
	,54,53,38,39,54,38,55,54,39,40,55,39,56,55,40,41,56,40,57,56,41,42,57,41,58,57,42
	,43,58,42,59,58,43,44,59,43,60,59,44,45,60,44,46,60,45,31,46,45,62,61,46,47,62,46
	,63,62,47,48,63,47,64,63,48,49,64,48,65,64,49,50,65,49,66,65,50,51,66,50,67,66,51
	,52,67,51,68,67,52,53,68,52,69,68,53,54,69,53,70,69,54,55,70,54,71,70,55,56,71,55
	,72,71,56,57,72,56,73,72,57,58,73,57,74,73,58,59,74,58,75,74,59,60,75,59,61,75,60
	,46,61,60,77,76,61,62,77,61,78,77,62,63,78,62,79,78,63,64,79,63,80,79,64,65,80,64
	,81,80,65,66,81,65,82,81,66,67,82,66,83,82,67,68,83,67,84,83,68,69,84,68,85,84,69
	,70,85,69,86,85,70,71,86,70,87,86,71,72,87,71,88,87,72,73,88,72,89,88,73,74,89,73
	,90,89,74,75,90,74,76,90,75,61,76,75,76,77,91,77,78,91,78,79,91,79,80,91,80,81,91
	,81,82,91,82,83,91,83,84,91,84,85,91,85,86,91,86,87,91,87,88,91,88,89,91,89,90,91
	,90,76,91
};



XDUL::XPlainVertex XDUL::m_boxVertices[] = 
    {
		{XVector3D( -1, -1, -1 ),0xFFFFFFFF},
        {XVector3D(  1, -1, -1 ),0xFFFFFFFF},
        {XVector3D(  1, -1,  1 ),0xFFFFFFFF},
        {XVector3D( -1, -1,  1 ),0xFFFFFFFF},
        {XVector3D( -1,  1, -1 ),0xFFFFFFFF},
        {XVector3D(  1,  1, -1 ),0xFFFFFFFF},
        {XVector3D(  1,  1,  1 ),0xFFFFFFFF},
        {XVector3D( -1,  1,  1 ),0xFFFFFFFF}
    };

WORD XDUL::m_boxIndices[] =
    {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };

static float gCylinderData[X_DUL_NUM_CYLINDER_FLOATS]=
{
	1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
	0.866025f,0.500000f,1.0f,0.866025f,0.500000f,1.0f,0.866025f,0.500000f,0.0f,0.866025f,0.500000f,0.0f,
	0.500000f,0.866025f,1.0f,0.500000f,0.866025f,1.0f,0.500000f,0.866025f,0.0f,0.500000f,0.866025f,0.0f,
	-0.0f,1.0f,1.0f,-0.0f,1.0f,1.0f,-0.0f,1.0f,0.0f,-0.0f,1.0f,0.0f,
	-0.500000f,0.866025f,1.0f,-0.500000f,0.866025f,1.0f,-0.500000f,0.866025f,0.0f,-0.500000f,0.866025f,0.0f,
	-0.866025f,0.500000f,1.0f,-0.866025f,0.500000f,1.0f,-0.866025f,0.500000f,0.0f,-0.866025f,0.500000f,0.0f,
	-1.0f,-0.0f,1.0f,-1.0f,-0.0f,1.0f,-1.0f,-0.0f,0.0f,-1.0f,-0.0f,0.0f,
	-0.866025f,-0.500000f,1.0f,-0.866025f,-0.500000f,1.0f,-0.866025f,-0.500000f,0.0f,-0.866025f,-0.500000f,0.0f,
	-0.500000f,-0.866025f,1.0f,-0.500000f,-0.866025f,1.0f,-0.500000f,-0.866025f,0.0f,-0.500000f,-0.866025f,0.0f,
	0.0f,-1.0f,1.0f,0.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,
	0.500000f,-0.866025f,1.0f,0.500000f,-0.866025f,1.0f,0.500000f,-0.866025f,0.0f,0.500000f,-0.866025f,0.0f,
	0.866026f,-0.500000f,1.0f,0.866026f,-0.500000f,1.0f,0.866026f,-0.500000f,0.0f,0.866026f,-0.500000f,0.0f,
	1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f
};

static float gCylinderDataCapsTop[]={
	0.866026f,-0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.500000f,-0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.500000f,-0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,-1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,-1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.500000f,-0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.500000f,-0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.866025f,-0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.866025f,-0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-1.000000f,-0.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-1.000000f,-0.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.866025f,0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.866025f,0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.500000f,0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.500000f,0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	-0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.500000f,0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.500000f,0.866025f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.866025f,0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.866025f,0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	1.000000f,0.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	1.000000f,0.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.000000f,1.000000f,1.000000f,0.000000f,1.000000f,1.000000f,
	0.866026f,-0.500000f,1.000000f,0.000000f,1.000000f,1.000000f,
};

static float gCylinderDataCapsBottom[]={
	1.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.866025f,0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.866025f,0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.500000f,0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.500000f,0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.000000f,1.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.000000f,1.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.500000f,0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.500000f,0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.866025f,0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.866025f,0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-1.000000f,-0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-1.000000f,-0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.866025f,-0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.866025f,-0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.500000f,-0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	-0.500000f,-0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,-1.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,-1.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.500000f,-0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.500000f,-0.866025f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.866026f,-0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.866026f,-0.500000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	0.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
	1.000000f,0.000000f,0.000000f,0.000000f,-1.000000f,0.000000f,
};

XDUL::XPlainVertex XDUL::m_transformVertices[] = 
    {
		{XVector3D( 0, 0, 0 ),0xFF0000FF},
        {XVector3D( 5, 0, 0 ),0xFF0000FF},
        {XVector3D( 0, 0, 0 ),0xFF00FF00},
        {XVector3D( 0, 5, 0 ),0xFF00FF00},
        {XVector3D( 0, 0, 0 ),0xFFFF0000},
        {XVector3D( 0, 0, 5 ),0xFFFF0000}
    };

XColor XDUL::m_colorArray[COLOR_ARRAY_SIZE] = 
{
	XColor(0,0,0,1),
	XColor(0,0,1,1),
	XColor::Green,
	XColor(0,1,1,1),
	XColor::Red,
	XColor(1,0,1,1),
	XColor(1,1,0,1),
	XColor(1,1,1,1)
};

//*****************************************************************************
//*****************************************************************************
XStatus XDUL::init()
{
	//Create debug volume vertex and index buffers
	XVertexDesc desc;
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_STRIP,X_DUL_NUM_CYLINDER_VERTS,true,m_cylinderVBHandle);
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_LIST,X_DUL_NUM_CYLINDER_CAP_VERTS,true,m_cylinderTopVBHandle);
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_LIST,X_DUL_NUM_CYLINDER_CAP_VERTS,true,m_cylinderBottomVBHandle);
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_LIST,X_DUL_NUM_SPHERE_VERTS,true,m_sphereVBHandle);
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_POINT_LIST,X_DUL_NUM_SPHERE_POINTS_VERTS,true,m_spherePointsVBHandle);
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_LINE_LIST,X_DUL_NUM_BOX_VERTS,true,m_boxVBHandle);

	desc.numColors = 1;
	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_LINE_LIST,X_DUL_NUM_TRANSFORM_VERTS,true,m_transformVBHandle);
	XVP::GraphicsMgr.createIndexBuffer(X_DUL_NUM_SPHERE_INDICES,true,m_sphereIBHandle);
	XVP::GraphicsMgr.createIndexBuffer(X_DUL_NUM_BOX_INDICES,true,m_boxIBHandle);

	XVertexBuffer* pVB = XVP::GraphicsMgr.lockVertexBuffer(m_sphereVBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_SPHERE_VERTS;++i)
		pVB->setPosition(i,m_sphereVertices[i].pos);
	XVP::GraphicsMgr.unlockVertexBuffer(m_sphereVBHandle);

	pVB = XVP::GraphicsMgr.lockVertexBuffer(m_spherePointsVBHandle,X_LT_DISCARD);
	XVector3D points[X_DUL_NUM_SPHERE_POINTS_VERTS];
	XIGenUniformSpherePoints(X_DUL_NUM_SPHERE_POINTS_VERTS,points);
    for (XU32 i = 0;i < X_DUL_NUM_SPHERE_POINTS_VERTS; ++i)
        pVB->setPosition(i,points[i]);
	XVP::GraphicsMgr.unlockVertexBuffer(m_spherePointsVBHandle);

	pVB = XVP::GraphicsMgr.lockVertexBuffer(m_boxVBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_BOX_VERTS;++i)
		pVB->setPosition(i,m_boxVertices[i].pos);
	XVP::GraphicsMgr.unlockVertexBuffer(m_boxVBHandle);
	
	pVB = XVP::GraphicsMgr.lockVertexBuffer(m_transformVBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_TRANSFORM_VERTS;++i)
	{
		pVB->setPosition(i,m_transformVertices[i].pos);
		pVB->setColor(i,0,m_transformVertices[i].diffuse);
	}
	XVP::GraphicsMgr.unlockVertexBuffer(m_transformVBHandle);

	pVB = XVP::GraphicsMgr.lockVertexBuffer(m_cylinderVBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_CYLINDER_VERTS*2;i+=2)
	{
		pVB->setPosition(i/2,gCylinderData[i*3],gCylinderData[i*3+1],gCylinderData[i*3+2]);
		//pVB->setNormal(i/2,0,gCylinderData[i*3+3],gCylinderData[i*3+4],gCylinderData[i*3+5]);

	}
	XVP::GraphicsMgr.unlockVertexBuffer(m_cylinderVBHandle);
	pVB = XVP::GraphicsMgr.lockVertexBuffer(m_cylinderTopVBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_CYLINDER_CAP_VERTS*2;i+=2)
	{
		pVB->setPosition(i/2,gCylinderDataCapsTop[i*3],gCylinderDataCapsTop[i*3+1],gCylinderDataCapsTop[i*3+2]);
		//pVB->setNormal(i/2,0,gCylinderDataCapsTop[i*3+3],gCylinderDataCapsTop[i*3+4],gCylinderDataCapsTop[i*3+5]);
	}
	XVP::GraphicsMgr.unlockVertexBuffer(m_cylinderTopVBHandle);
	pVB = XVP::GraphicsMgr.lockVertexBuffer(m_cylinderBottomVBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_CYLINDER_CAP_VERTS*2;i+=2)
	{
		pVB->setPosition(i/2,gCylinderDataCapsBottom[i*3],gCylinderDataCapsBottom[i*3+1],gCylinderDataCapsBottom[i*3+2]);
		//pVB->setNormal(i/2,0,gCylinderDataCapsBottom[i*3+3],gCylinderDataCapsBottom[i*3+4],gCylinderDataCapsBottom[i*3+5]);

	}
	XVP::GraphicsMgr.unlockVertexBuffer(m_cylinderBottomVBHandle);


	XU16* pIB = XVP::GraphicsMgr.lockIndexBuffer(m_sphereIBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_SPHERE_INDICES;++i)
		pIB[i] = m_sphereIndices[i];
	XVP::GraphicsMgr.unlockIndexBuffer(m_sphereIBHandle);
	pIB = XVP::GraphicsMgr.lockIndexBuffer(m_boxIBHandle,X_LT_DISCARD);
	for (XU32 i = 0;i < X_DUL_NUM_BOX_INDICES;++i)
		pIB[i] = m_boxIndices[i];
	XVP::GraphicsMgr.unlockIndexBuffer(m_boxIBHandle);
	
	//Create a wireframe material
	XVP::GraphicsMgr.createMaterial(NULL,m_wireframeMat);
	XMaterial* pMaterial = XVP::GraphicsMgr.lockMaterial(m_wireframeMat);
	pMaterial->setFillMode(X_FMT_WIRE);
	XVP::GraphicsMgr.unlockMaterial(m_wireframeMat);

	//Create a standard material with no z test
	XVP::GraphicsMgr.createMaterial(NULL,m_noZMat);
	pMaterial = XVP::GraphicsMgr.lockMaterial(m_noZMat);
	pMaterial->enableDepthTest(false);
	XVP::GraphicsMgr.unlockMaterial(m_noZMat);
	
	
	
	//Create an overdraw material
	m_visSC.init();
	XVP::GraphicsMgr.createMaterial(NULL,m_overdrawMat);
	pMaterial = XVP::GraphicsMgr.lockMaterial(m_overdrawMat);
	pMaterial->enableDepthTest(false);
	pMaterial->enableAlphaBlend(0,true);
	pMaterial->setBlendOp(X_BOT_ADD);
	pMaterial->setSrcBlend(X_BT_SRC_ALPHA);
	pMaterial->setDestBlend(X_BT_ONE);
	XVP::GraphicsMgr.unlockMaterial(m_overdrawMat);
	pMaterial->attachShaderController(&m_visSC);

	//Create a solid color material
	XVP::GraphicsMgr.createMaterial(NULL,m_solidColorMat);
	pMaterial = XVP::GraphicsMgr.lockMaterial(m_solidColorMat);
	XVP::GraphicsMgr.unlockMaterial(m_solidColorMat);
	pMaterial->attachShaderController(&m_visSC);
	//Create vis mode shader controllers
	
	
	//m_visSC.setColor(XColor(1,1,1,1));
	//m_visSC.setDepthBias(-.0005f);

	//Create constant buffers
	XVP::GraphicsMgr.createConstantBuffer(sizeof(VSConstants),true,m_VSConstants);
	XVP::GraphicsMgr.createConstantBuffer(sizeof(PSConstants),true,m_PSConstants);

	//Create vertex and pixel shaders.
	XStatus status = X_OK;
	
	status = XVP::GraphicsMgr.createVertexShader("Material Color.fx","mainVS",m_vsHandle);
	if (status == X_OK)
		status = XVP::GraphicsMgr.createPixelShader("Material Color.fx","mainPS",m_psHandle);

	status = XVP::GraphicsMgr.createVertexShader("Vertex Color.fx","mainVS",m_simpleVertColorVS);
	if (status == X_OK)
		status = XVP::GraphicsMgr.createPixelShader("Vertex Color.fx","mainPS",m_simpleVertColorPS);

	return status;
}


//*****************************************************************************
//*****************************************************************************
XStatus XDUL::destroy()
{
	//Destroy vertex data
	XVP::GraphicsMgr.destroyVertexBuffer(m_cylinderVBHandle);
	XVP::GraphicsMgr.destroyVertexBuffer(m_cylinderTopVBHandle);
	XVP::GraphicsMgr.destroyVertexBuffer(m_cylinderBottomVBHandle);
	XVP::GraphicsMgr.destroyVertexBuffer(m_sphereVBHandle);
	XVP::GraphicsMgr.destroyVertexBuffer(m_spherePointsVBHandle);
	XVP::GraphicsMgr.destroyVertexBuffer(m_boxVBHandle);
	XVP::GraphicsMgr.destroyIndexBuffer(m_boxIBHandle);
	XVP::GraphicsMgr.destroyIndexBuffer(m_sphereIBHandle);
	XVP::GraphicsMgr.destroyVertexBuffer(m_transformVBHandle);

	//Destroy material
	XVP::GraphicsMgr.destroyMaterial(m_wireframeMat);

	//Destroy constant buffers
	XVP::GraphicsMgr.destroyConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.destroyConstantBuffer(m_PSConstants);

	//Destroy our shaders
	XVP::GraphicsMgr.destroyVertexShader(m_vsHandle);
	m_vsHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyPixelShader(m_psHandle);
	m_psHandle = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyVertexShader(m_simpleVertColorVS);
	m_simpleVertColorVS = X_INVALID_HANDLE;
	XVP::GraphicsMgr.destroyPixelShader(m_simpleVertColorPS);
	m_simpleVertColorPS = X_INVALID_HANDLE;

	m_visSC.destroy();

	return X_OK;
}


XStatus XDUL::drawSphere(XScene* pScene,XVector3D& center,XF32 radius,XColor& color)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	XMatrix4x4 scale;
	scale._11 = scale._22 = scale._33 = radius;
	scale._41 = center.x;
	scale._42 = center.y;
	scale._43 = center.z;
	
	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	//Lock, fill and bind constant buffers
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = scale * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);
 
	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);
 	XVP::GraphicsMgr.renderVertexBuffer(m_sphereVBHandle,m_sphereIBHandle);
												
	return X_OK;
}

XStatus XDUL::drawSphere(XScene* pScene,XMatrix4x4 transform,XF32 radius,XColor& color)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	XMatrix4x4 scale;
	scale._11 = scale._22 = scale._33 = radius;
	scale *= transform;
	
	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	//Lock, fill and bind constant buffers
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = scale * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);
 
	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);
 
	
	//XVP::GraphicsMgr.setVertexShaderConstant(0,(XF32*)&matrix,4);
	/*XVP::GraphicsMgr.renderUserIndexedVertexBuffer(X_PT_TRIANGLE_LIST,X_VT_PLAIN,X_DUL_NUM_SPHERE_VERTS,
												&m_sphereVertices,X_DUL_NUM_SPHERE_INDICES,
												&m_sphereIndices,X_DUL_NUM_SPHERE_INDICES/3,
												sizeof(XPlainVertex));*/
	XVP::GraphicsMgr.renderVertexBuffer(m_sphereVBHandle,m_sphereIBHandle);									
												
	return X_OK;
}


XStatus XDUL::drawSpherePoints(XScene* pScene,XVector3D& center,XF32 radius,XColor& color)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	XMatrix4x4 scale;
	scale._11 = scale._22 = scale._33 = radius;
	scale._41 = center.x;
	scale._42 = center.y;
	scale._43 = center.z;
	
	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	//Lock, fill and bind constant buffers
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = scale * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);
 
	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);
 	XVP::GraphicsMgr.renderVertexBuffer(m_spherePointsVBHandle);
												
	return X_OK;
}

XStatus XDUL::drawSpherePoints(XScene* pScene,XMatrix4x4 transform,XF32 radius,XColor& color)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	XMatrix4x4 scale;
	scale._11 = scale._22 = scale._33 = radius;
	scale *= transform;
	
	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	//Lock, fill and bind constant buffers
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = scale * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);
 
	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);
 	XVP::GraphicsMgr.renderVertexBuffer(m_spherePointsVBHandle);									
												
	return X_OK;
}

XHandle XDUL::getSphereMaterial()
{
	return m_wireframeMat;
}

XStatus XDUL::drawCylinder(XScene* pScene,XMatrix4x4 transform,XColor& color)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);

	//Lock, fill and bind constant buffers
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = transform * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);
 
	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);
 
	

	XVP::GraphicsMgr.renderVertexBuffer(m_cylinderVBHandle);									
	XVP::GraphicsMgr.renderVertexBuffer(m_cylinderTopVBHandle);									
	XVP::GraphicsMgr.renderVertexBuffer(m_cylinderBottomVBHandle);									
												
	return X_OK;
}

XStatus XDUL::drawAABB(XScene* pScene,XVector3D& min,XVector3D& max,XColor& color)
{
    
      X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");
	
	XMatrix4x4 scale;
	XVector3D extents = (max - min) * .5f;
	XVector3D center = (min + max) * .5f;
	scale._11 = extents.x;
	scale._22 = extents.y;
	scale._33 = extents.z;
	scale._41 = center.x;
	scale._42 = center.y;
	scale._43 = center.z;
	
	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = scale * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);

	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);

	//XVP::GraphicsMgr.setVertexShaderConstant(0,(XF32*)&matrix,4);
	/*XVP::GraphicsMgr.renderUserIndexedVertexBuffer(X_PT_LINE_LIST,X_VT_PLAIN,8,
												verts,24,
												indices,12,
												sizeof(XPlainVertex));*/
	return XVP::GraphicsMgr.renderVertexBuffer(m_boxVBHandle,m_boxIBHandle);									

}

XStatus XDUL::drawBox(XScene* pScene,XMatrix4x4 transform,XColor& color)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	XVP::GraphicsMgr.setVertexShader(m_vsHandle);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_psHandle);
	
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = transform * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	pVSCB->depthBias.x = 0.0f;
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);

	PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	pPSCB->color = color;
	XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	XVP::GraphicsMgr.setPSConstantBuffer(X_USER_SLOT1,m_PSConstants);

	return XVP::GraphicsMgr.renderVertexBuffer(m_boxVBHandle,m_boxIBHandle);
}

XStatus XDUL::drawTransform(XScene* pScene,XMatrix4x4 transform,XBOOL depthTest)
{
	X_ASSERTM(pScene->getActiveCamera(),"No active camera to render with");

	if (depthTest)
		XVP::GraphicsMgr.applyMaterial(m_wireframeMat);
	else
		XVP::GraphicsMgr.applyMaterial(m_noZMat);

	XVP::GraphicsMgr.setVertexShader(m_simpleVertColorVS);
	XVP::GraphicsMgr.removeGeometryShader();
	XVP::GraphicsMgr.setPixelShader(m_simpleVertColorPS);
	
	VSConstants* pVSCB = (VSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_VSConstants,X_LT_DISCARD);
	pVSCB->worldViewProjMatrix = transform * 
						pScene->getActiveCamera()->getViewMatrix() * 
						pScene->getActiveCamera()->getProjMatrix();
	XVP::GraphicsMgr.unlockConstantBuffer(m_VSConstants);
	XVP::GraphicsMgr.setVSConstantBuffer(X_USER_SLOT0,m_VSConstants);

	
	//PSConstants* pPSCB = (PSConstants*)XVP::GraphicsMgr.lockConstantBuffer(m_PSConstants,X_LT_DISCARD);
	//pPSCB->color = XColor(1,1,0,1);
	//XVP::GraphicsMgr.unlockConstantBuffer(m_PSConstants);
	//XVP::GraphicsMgr.setPSConstantBuffer(X_PS_USER_SLOT0,m_PSConstants);

	return XVP::GraphicsMgr.renderVertexBuffer(m_transformVBHandle);
}


XStatus XDUL::setVisualizationMode(X_VIS_MODE_TYPE visMode)
{
	m_visMode = visMode;
	return X_OK;
}

XU32 XDULhash( XU32 a)
{
	XU32 key = a;
   key = (key+0x7ed55d16) + (key<<12);
   key = (key^0xc761c23c) ^ (key>>19);
   key = (key+0x165667b1) + (key<<5);
   key = (key+0xd3a2646c) ^ (key<<9);
   key = (key+0xfd7046c5) + (key<<3);
   key = (key^0xb55a4f09) ^ (key>>16);
   return key;
}





XStatus XDUL::fixupRenderEventForVisualization(XRenderEvent& event)
{
	int a = 0;
	int b = 0;
	switch (m_visMode)
	{
	case X_VMT_NONE:
		break;
	case X_VMT_OVERDRAW:
		m_visSC.setColor(XColor(1/m_overdrawThresh,
								1/m_overdrawThresh,
								1/m_overdrawThresh,
								1/m_overdrawThresh));
		event.materialHandle = m_overdrawMat;
		break;
	case X_VMT_DRAW_CALLS:

		a = XDULhash((XU32)(event.pSceneNode));
		a = a % 8;
		m_visSC.setColor(m_colorArray[a]);
		event.materialHandle = m_solidColorMat;
		break;
	case X_VMT_TEXTURE_SIZE:
		break;
	case X_VMT_TRI_COMPLEXITY:
		break;
	case X_VMT_SHADER_COMPLEXITY:
		break;
	case X_VMT_BUDGETS:
		break;
	}

	return X_OK;
}



XF32 XDUL::getOverdrawThreshold()
{
	return m_overdrawThresh;
}

XStatus XDUL::setOverdrawThreshold(XF32 threshold)
{
	m_overdrawThresh = threshold;
	return X_OK;
}

#endif


