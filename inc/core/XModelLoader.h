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
//	Module Name:	XModelLoader.h
//	Author:			Lynn Duke
//	Creation Date:	10-7-02
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MODEL_LOADER_H_
#define __X_MODEL_LOADER_H_




//*****************************************************************************
//Defines
//*****************************************************************************
#define MAX_CHUNK_NAME_LENGTH 255
#define MAX_CHUNK_VALUE_NAME_LENGTH 255
#define MAX_TEMP_BUF_LENGTH 255



//*****************************************************************************
//Forward Declarations
//*****************************************************************************
struct XmfColor;
struct XmfFace;
struct XmfSkinVertex;
struct XmfModel;
struct XafRotationKey;
struct XafPositionKey;
struct XafKeyFrameSet;
struct UvaFloatKey;
struct UvaMatAnimationTrack;
struct XafAnimationTrack;
struct MlVertexTexInfo;
struct MlPolyFaceListChange;
struct XlfLight;
struct XmfMaterial;
struct XsmBone;
struct XmfGeometry;


//*****************************************************************************
//Structures
//*****************************************************************************
struct XmfColor
{
	XF32 a;
	XF32 r;
	XF32 g;
	XF32 b;
};

struct XmfFace
{
	XU16 x;
	XU16 y;
	XU16 z;
};

struct XmfSkinVertex
{
	vector<XU32> boneIDList;
	vector<XF32> boneWeightList;
};

struct XmfParticleSystem
{
	XMatrix4x4 modelTransform;	
	int materialID;
	XU32 emitterType;
	XF32 emitterLength;
	XF32 emitterWidth;
	XF32 emitterHeight;
	XF32 birthRate;
	XF32 startAlpha;
	XF32 endAlpha;
	XVector3D startColor;
	XVector3D endColor;
	XF32 particleGrowTime;
	XF32 particleShrinkTime;
	XF32 particleSpeed;
	XF32 particleSpeedVariation;
	XF32 particleLife;
	XF32 particleLifeVariation;
	XF32 particleSize;
	XF32 particleSizeVariation;
	XVector3D directionVec;
	XF32 directionVecVariation;
	XS8* name;						
	XS8* parentName;						
	
	XmfParticleSystem()
	{
		name = NULL;
		parentName = NULL;
	}
};

struct XlfLight
{
	X_LIGHT_TYPE type;
	XColor diffuse;
	XColor specular;
	XColor ambient;
	XVector3D position;
	XVector3D direction;
	XF32 range;
	XF32 falloff;
	XF32 attenuation;
	XF32 innerConeAngle;
	XF32 outerConeAngle;
};

struct XafRotationKey
{
	XU32 time;				//Timestamp in milliseconds
	XQuaternion quat;	//Quaternion rotation representation
};

struct XafPositionKey
{
	XU32 time;				//Timestamp in milliseconds
	XVector3D vec;			//Vector distance representation
};

struct XafScaleKey
{
	XU32 time;				//Timestamp in milliseconds
	XVector3D vec;			//Scale factor vector
};

struct UvaFloatKey
{
	XU32 time;				//Timestamp in milliseconds
	XF32 value;				//float value
};

struct XafKeyFrameSet
{
	XU32 ID;
	vector<XafRotationKey> rotKeyList;
	vector<XafPositionKey> posKeyList;
	vector<XafScaleKey>	 scaleKeyList;
};

struct UvaMatAnimationTrack
{
	vector<UvaFloatKey> uOffsetKeyList;
	vector<UvaFloatKey> vOffsetKeyList;
	vector<UvaFloatKey>	wAngleKeyList;
};

struct XafAnimationTrack
{
	vector<XafKeyFrameSet> keysList;
};

struct MlVertexTexInfo
{
	XU32 vertexIndex;
	vector<XU32> texIndexList;
};


struct MlPolyFaceListChange
{
	XU32 fromPolyIndex;
	XU32 toPolyIndex;
	XU32 texIndex;
};



//*****************************************************************************
//*****************************************************************************
struct XmfMaterial
{
	XVector4D	diffuseColor;		//Object's diffuse color
	XVector4D	ambientColor;		//Object's ambient color
	XVector4D	specularColor;		//Object's specular color
	XF32		specPower;			//Specular power of object
	XU32		fillMode;			//Fillmode for the object
	XS8*		texFilename;		//Filename of texture

	XmfMaterial()
	{
		texFilename = NULL;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XsmBone
{
	XU32		ID;					//Bone's numerical ID
	XMatrix4x4  modelTransform;		//Bone's model transform
	XMatrix4x4	invBoneTransform;	//Bone's inverse transform
	XS8*		name;				//Name of bone
	XS8*		parentName;			//Name of bone's parent object
	
	XsmBone()
	{
		name = NULL;
		parentName = NULL;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XmfGeometry
{
	vector<XVector3D>	vertexList;				//List of vertices
	XU32		numVerts;						//Number of vertices
	vector<XVector2D> texCoordList;			//List of texture coordinates
	XU32		numTexCoordPairs;				//Number of texture coordinate pairs
	vector<XVector3D> vNormalList;				//List of vertex normals
	XU32		numVNormals;					//Number of vertex normals
	vector<XmfFace> polyFaceList;				//List of polygons
	XU32		numPolyFaces;					//Number of polygons
	vector<XmfFace> texFaceList;				//List of texture coordinate faces
	XU32		numTexFaces;					//Number of texture faces
	vector<XmfColor> cpvVertList;				//List of Color-Per-Vertex Verts.
	XU32		numCpvVerts;					//Number of Color-Per-Vertex Verts
	vector<XmfSkinVertex> skinVertList;		//List of skin vertices
	XU32		numSkinVerts;					//Number of skinned vertices.
	XS8*		objectName;						//Name of object
	XS8*		parentName;						//Name of parent's object
	XMatrix4x4 modelTransform;					//Object's transform
	int		materialID;						//Object's material index
	XSphere boundingSphere;			//Bounding Sphere

	XmfGeometry()
	{
		objectName = NULL;
		parentName = NULL;
		numVerts = 0;
		numTexCoordPairs = 0;
		numPolyFaces = 0;
		numTexFaces = 0;
		materialID = 0;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XCF_CAMERA
{
	XMatrix4x4 modelTransform;					
	XF32 horizontalFOV;
	XF32 nearClipPlane;
	XF32 farClipPlane;
	XS8* pName;

	XCF_CAMERA::XCF_CAMERA()
	{
		pName = NULL;
	}
};



//*****************************************************************************
//*****************************************************************************
struct XmfModel
{
	vector<XmfMaterial> materialList;
	vector<XmfGeometry> objectList;
	vector<XsmBone> boneList;
	vector<XMatrix4x4> poseTransforms;
	XafAnimationTrack	m_animationTrack;
	UvaMatAnimationTrack m_matAnimationTrack;
	XmfParticleSystem particleSystem;
	XlfLight light;
	XCF_CAMERA camera;
};



//*****************************************************************************
//*****************************************************************************
class XModelLoader
{
private:
	XBOOL xmfProcessChunk(XS8* chunkName,FILE* f);
	XBOOL xmfProcessGeometryObject(FILE* f);
	XBOOL xmfProcessMaterialObject(FILE* f);
	XBOOL xpoProcessChunk(XS8* chunkName,FILE* f);
	XBOOL xpoProcessTransform(FILE* f);
	XBOOL xmfProcessBone(FILE* f);
	void xmfWeldVertices(XmfGeometry* object);
	void xmfAppendSkinVertex(vector<XmfSkinVertex>* destList,XmfSkinVertex* data);
	XBOOL xafProcessChunk(XS8* chunkName,FILE* f);
	XBOOL xafProcessKeyFrameSet(FILE* f);
	XBOOL uvaProcessChunk(XS8* chunkName,FILE* f);
	XBOOL uvaProcessMatAnimationTrack(FILE* f);
	XBOOL xlfProcessChunk(XS8* chunkName,FILE* f);
	XBOOL xlfProcessLight(FILE* f);
	XBOOL xcfProcessChunk(XS8* chunkName,FILE* f);
	XBOOL xcfProcessCamera(FILE* f);
	XBOOL xpsProcessChunk(XS8* chunkName,FILE* f);
	XBOOL xpsProcessParticleSystem(FILE* f);
	MlVertexTexInfo* getVertexTexInfoBlock(vector<MlVertexTexInfo>* list,XU32 index);
	XBOOL texCoordIndexExists(MlVertexTexInfo* vtBlockPtr,XU32 index);

public:
	XmfModel				m_model;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XModelLoader();
	~XModelLoader();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus load(XS8* pFilename);
	void destroyModelData();
};

#endif