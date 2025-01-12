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
//	This source is protected by copyright laws. Unauthorized reproduction or 
//	distribution of this source will result in severe civil and criminal
//	penalties, and will be prosecuted to the maximum extent possible under law.
//	
//*****************************************************************************
//*****************************************************************************

#ifndef __X_STRUCTS_H__
#define __X_STRUCTS_H__

#include "xvpsdk.h"
#include <vector>
#include <list>

using std::vector;
using std::string;


#define MAX_TEX_COORD_SETS		8
#define MAX_COLOR_SETS		8


enum INPUT_DATA_TYPE
{
	IDT_VERTEX,
	IDT_NORMAL,
	IDT_COLOR0,
	IDT_COLOR1,
	IDT_COLOR2,
	IDT_COLOR3,
	IDT_COLOR4,
	IDT_COLOR5,
	IDT_COLOR6,
	IDT_COLOR7,
	IDT_TEXCOORD0,
	IDT_TEXCOORD1,
	IDT_TEXCOORD2,
	IDT_TEXCOORD3,
	IDT_TEXCOORD4,
	IDT_TEXCOORD5,
	IDT_TEXCOORD6,
	IDT_TEXCOORD7,
};





struct QuaternionKey
{
	XF32 time;				//Timestamp in seconds
	XQuaternion quat;		//Quaternion rotation representation
};

struct Vector3DKey
{
	XF32 time;				//Timestamp in seconds
	XVector3D vec;			//Vector distance representation
};


struct DAEMaterial
{
	string name;
	string diffuseMapName;
	XColor diffuseColor;
	string ambientMapName;
	XColor ambientColor;
	string specularMapName;
	XColor specularColor;
	string reflectiveMapName;
	XColor reflectiveColor;
	string transparentMapName;
	XColor transparentColor;
	XF32 shininess;
	XF32 transparency;

	DAEMaterial()
	{
		shininess = 0;
		transparency = 0;
	}

	~DAEMaterial()
	{
		name.clear();
		diffuseMapName.clear();
		ambientMapName.clear();
		specularMapName.clear();
		reflectiveMapName.clear();
		transparentMapName.clear();
	}
};

struct DAEAnimation
{
	XU32 ID;
	string name;
	vector<XF32> timeList;
	vector<XQuaternion> rotKeyList;
	vector<XVector3D> posKeyList;
	vector<XVector3D> scaleKeyList;

	~DAEAnimation()
	{
		name.clear();
		timeList.clear();
		rotKeyList.clear();
		posKeyList.clear();
		scaleKeyList.clear();
	}
};



struct DAEBone
{
	string name;	
	XMatrix4x4 invBoneTransform;	//Bone's inverse transform
	
	~DAEBone()
	{
		name.clear();
	}
};


struct DAENode
{
	XU32 ID;
	string name;
	string boneName;
	string parentName;
	string geometryName;
	XMatrix4x4 modelTransform;
	XMatrix4x4 pivotTransform;


	~DAENode()
	{
		name.clear();
		boneName.clear();
		parentName.clear();
		geometryName.clear();
	}
};


struct DAEBoneWeight
{
	XU32 boneID;
	XF32 weight;
};


struct DAESkinVertex
{
	vector<DAEBoneWeight> boneWeights;

	~DAESkinVertex()
	{
		boneWeights.clear();
	}
};


struct DAESkin
{
	string name;
	string parentName;
	string geometryName;
	vector<DAEBone> bones;
	vector<DAESkinVertex> verts;

	~DAESkin()
	{
		name.clear();
		parentName.clear();
		geometryName.clear();
		bones.clear();
		verts.clear();
	}

	DAEBone* findBone(string boneName)
	{
		for (XU32 i = 0;i < bones.size();++i)
		{
			if (bones[i].name == boneName)
				return &bones[i];
		}

		return NULL;
	}
};


struct DAETriangles
{
	string materialName;							//Material applied to triangles
	vector<XU32> vertexIndices;						//List of vertex indices
	vector<XU32> normalIndices;						//List of normal indices
	vector<vector<XU32>> colorIndices;				//List of color index lists 
	vector<vector<XU32>> texCoordIndices;			//List of tex coord index lists 

	vector<vector<XVector4D>*> inputLists;			//List of inputs in order
	vector<vector<XU32>*> outputLists;				//List of outputs in order

	vector<INPUT_DATA_TYPE> listTypes;				//Types of lists in order
	
	DAETriangles()
	{
		for (XU32 i = 0;i < MAX_COLOR_SETS;++i)
		{
			vector<XU32> newList;
			colorIndices.push_back(newList);
		}
		for (XU32 i = 0;i < MAX_TEX_COORD_SETS;++i)
		{
			vector<XU32> newList;
			texCoordIndices.push_back(newList);
		}
	}
	
	~DAETriangles()
	{
		materialName.clear();
		vertexIndices.clear();
		normalIndices.clear();
		texCoordIndices.clear();
		inputLists.clear();
		outputLists.clear();
	}
};


struct DAEGeometry
{
	string geometryName;						//Name of Geometry
	vector<XVector4D> vertexList;				//Vertex Source
	vector<XVector4D> normalList;				//Normal Source
	vector<vector<XVector4D>> colorLists;		//List of color sources
	vector<vector<XVector4D>> texCoordLists;	//List of texture coordinate sources
	vector<DAETriangles> triangleLists;			//List of triangle sets

	DAEGeometry()
	{
		for (XU32 i = 0;i < MAX_COLOR_SETS;++i)
		{
			vector<XVector4D> newList;
			colorLists.push_back(newList);
		}
		for (XU32 i = 0;i < MAX_TEX_COORD_SETS;++i)
		{
			vector<XVector4D> newList;
			texCoordLists.push_back(newList);
		}
	}

	~DAEGeometry()
	{
		vertexList.clear();			
		normalList.clear();			
		triangleLists.clear();		
		geometryName.clear();	
		for (XU32 i = 0;i < texCoordLists.size();++i)
			texCoordLists[i].clear();
		texCoordLists.clear();		
	}
};


struct DAEGeometryLib
{
	vector<DAEGeometry> geometryList;

	~DAEGeometryLib()
	{
		geometryList.clear();
	}

	DAEGeometry* findGeometry(string geometryName)
	{
		for (XU32 i = 0;i < geometryList.size();++i)
		{
			if (geometryList[i].geometryName == geometryName)
				return &geometryList[i];
		}

		return NULL;
	}
};



struct DAEAnimationLib
{
	vector<DAEAnimation> animationList;

	~DAEAnimationLib()
	{
		animationList.clear();
	}

	DAEAnimation* findAnimation(string animationTargetName)
	{
		for (XU32 i = 0;i < animationList.size();++i)
		{
			if (animationList[i].name == animationTargetName)
				return &animationList[i];
		}

		return NULL;
	}
};


struct DAESkinLib
{
	vector<DAESkin> skinList;

	~DAESkinLib()
	{
		skinList.clear();
	}
};

struct DAEMaterialLib
{
	vector<DAEMaterial> materialList;

	~DAEMaterialLib()
	{
		materialList.clear();
	}

	DAEMaterial* findMaterial(string materialName)
	{
		for (XU32 i = 0;i < materialList.size();++i)
		{
			if (materialList[i].name == materialName)
				return &materialList[i];
		}

		return NULL;
	}
};

struct DAENodeLib
{
	vector<DAENode> nodeList;

	~DAENodeLib()
	{
		nodeList.clear();
	}

	DAENode* findNodeByName(string nodeName)
	{
		for (XU32 i = 0;i < nodeList.size();++i)
		{
			if (nodeList[i].name == nodeName)
				return &nodeList[i];
		}

		return NULL;
	}

	DAENode* findNodeByID(XU32 ID)
	{
		for (XU32 i = 0;i < nodeList.size();++i)
		{
			if (nodeList[i].ID == ID)
				return &nodeList[i];
		}

		return NULL;
	}

	DAENode* findNodeByBoneName(string boneName)
	{
		for (XU32 i = 0;i < nodeList.size();++i)
		{
			if (nodeList[i].boneName == boneName)
				return &nodeList[i];
		}

		return NULL;
	}
};


#endif