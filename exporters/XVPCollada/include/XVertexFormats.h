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
//	Module Name:	XVertexFormats.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_VERTEX_FORMATS_H__
#define __X_VERTEX_FORMATS_H__

#include <xvpsdk.h>



//*****************************************************************************
//! Vertex type specified in RHW coordinates with a color component and texture coordinates.
/*!  
Vertex type specified in RHW coordinates with a color component and texture coordinates.
*/
//*****************************************************************************
struct XRHWVertex
{
	XVector4D	pos;		/*!< The vertex position in RHW coordinates. */
	XU32		diffuse;	/*!< The vertex diffuse color. */
    XVector2D	texCoords;	/*!< The vertex texture coordinates - set 1. */
};



//*****************************************************************************
//*****************************************************************************
struct XPlainVertex
{
	XVector3D	pos;		/*!< Vertex position */
	XU32		diffuse;	/*!< The vertex diffuse color. */
};



//*****************************************************************************
//Generic vertex type containing all possible attributes
//*****************************************************************************
struct XUberVertex
{
	XVector3D	pos;			
	XVector3D	normal;			
    XU32		diffuse[8];		
    XVector2D	texCoords[8];	
	XVector3D	tangent;
	XVector3D	binormal;
	XF32		boneWeights[8];
	XF32		boneIndices[8];
	XU32		numBones;		

	XUberVertex()
	{
		for (XU32 i = 0;i < 8;++i)
		{
			boneWeights[i] = 0.0f;
			boneIndices[i] = 0.0f;
			diffuse[i] = 0xFFFFFFFF;
		}
		normal = XVector3D::YAxis;
		tangent = XVector3D::ZAxis;
		binormal = XVector3D::XAxis;
		numBones = 0;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XSkinVertex
{
	XVector3D	pos;			/*!< Vertex position */
	XF32		boneWeights[8];	/*!< Up to 8 blend weights */
	XF32		boneIndices[8];	/*!< Up to 8 blend indices */
	XVector3D	normal;			/*!< Vertex Normal */
    XU32		diffuse;		/*!< Diffuse color */
    XVector2D	texCoords;		/*!< 2 texture coordinates */
	XU32		numBones;		/*!< Up to 4 bones can affect this vertex */
};



//*****************************************************************************
//*****************************************************************************
struct XParticleVertex
{
	XVector3D	pos;		/*!< Vertex position */
	XU32		diffuse;	/*!< The vertex diffuse color. */
    XVector4D	texCoords;	/*!< 4 texture coordinates */
};


#endif
