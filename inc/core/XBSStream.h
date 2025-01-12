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
//*****************************************************************************
//*****************************************************************************

#ifndef __XBS_STREAM_H__
#define __XBS_STREAM_H__


#include "XColor.h"
#include "XMatrix4x4.h"

#define XBS_VERSION					1
#define XBS_MAX_NAME_LEN			64


//*****************************************************************************
//These are the chunk IDs that represent various chunks of in a binary stream
//*****************************************************************************
enum XBS_CHUNK_ID
{
	XBS_INVALID,
	XBS_MATERIAL,
	XBS_MODEL,
	XBS_MODEL_OBJECT,
	XBS_SKIN_MESH,
	XBS_BONE,
	XBS_LIGHT,
	XBS_PARTICLE_SYSTEM,
	XBS_ANIM_TRACK,
	XBS_NUM_CHUNK_IDS
};


//*****************************************************************************
//XBS structs
//*****************************************************************************
struct XBSChunkHeader
{
	XU32 chunkID;
	XU32 sizeInBytes;
	XU32 version;
};


struct XBSMaterialChunk
{
	XS8 name[XBS_MAX_NAME_LEN];
	
	XBSMaterialChunk()
	{
		name[0] = '\0';
	}
};


struct XBSModelObjectChunk
{
	XS8 name[XBS_MAX_NAME_LEN];
	XS8 parentName[XBS_MAX_NAME_LEN];
	XMatrix4x4 modelTransform;
	XMatrix4x4 invBoneTransform;
	XU32 numVertexBuffers;
	XBOOL isBone;

	XBSModelObjectChunk()
	{
		isBone = false;
		name[0] = '\0';
		parentName[0] = '\0';
		numVertexBuffers = 0;
	}
};

struct XBSSkinChunk
{
	XS8 name[XBS_MAX_NAME_LEN];
	XS8 parentName[XBS_MAX_NAME_LEN];
	XMatrix4x4 modelTransform;
	XU32 numVertexBuffers;

	XBSSkinChunk()
	{
		name[0] = '\0';
		parentName[0] = '\0';
		numVertexBuffers = 0;
	}
};

struct XBSBoneChunk
{
	XS8 name[XBS_MAX_NAME_LEN];
	XS8 parentName[XBS_MAX_NAME_LEN];
	XMatrix4x4 modelTransform;
	XMatrix4x4 invBoneTransform;

	XBSBoneChunk()
	{
		name[0] = '\0';
		parentName[0] = '\0';
	}
};



//*****************************************************************************
//Functions for writing chunks to a stream
//*****************************************************************************
void XBSWriteMaterialChunk(FILE* pFile);
void XBSWriteModelChunk();



#endif
