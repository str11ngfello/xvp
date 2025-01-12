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
//	Module Name:	XBsp.h
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BSP_H__
#define __X_BSP_H__



#if 0


//*****************************************************************************
//Defines
//*****************************************************************************
#define D3DFVF_LVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

static const int		CP_FRONT	= 1001;		//In Front of Plane
static const int		CP_BACK		= 1002;		//Behind Plane
static const int		CP_ONPLANE	= 1003;		//On Plane
static const int		CP_SPANNING	= 1004;		//Spanning Plane
static const FLOAT		g_EPSILON	= 0.001f;	//Tolerance for FLOATs
                


//*****************************************************************************
//Structures 
//*****************************************************************************
struct D3DLVERTEX
{
    XF32		x; 
    XF32		y;    
    XF32		z;  
	D3DCOLOR	color;
	XF32		tu;
	XF32		tv;
};

struct PLANE
{
	XVector3D PointOnPlane;
	XVector3D normal;
};

//Holds a Convex polygon as a Indexed Tri List
struct POLYGON		
{
	D3DLVERTEX	*VertexList;
	XVector3D Normal;
	WORD		NumberOfVertices;
	WORD		NumberOfIndices;
	WORD		*Indices;
	POLYGON		*Next;
	WORD		TextureIndex;
};

//Holds the AABB of the BSP Nodes and Leafs.These are used to build
//initial portals at each node and to perform frustum rejection on leafs
struct BOUNDINGBOX						
{										
	XVector3D BoxMin;					
	XVector3D BoxMax;
};

//Because the PVS is compressed this if the offset into the PVSData
//array that this leafs PVS Set begins
struct LEAF								
{							
	long		StartPolygon;			
	long		EndPolygon;				
	long		PVSIndex;				
	BOUNDINGBOX BoundingBox;			
};

struct NODE 
{
	XU8		IsLeaf;
	XU32		Plane;
	XU32		Front;
	XS32		Back;
};





//*****************************************************************************
//Structures 
//*****************************************************************************
class XBsp
{
private:
	XU32			m_textureArrayOffset;
	XPlane			m_frustumPlanes[6];		//View Frustum used for Clipping
	BOOL			m_dontFrustumReject;
	long			m_bytesPerSet;			//Bytes reserved for uncompressed LEAF pvs	
	long			m_numPolygons;
	long			m_numNodes;
	long			m_numLeafs;
	long			m_numPlanes;
	WORD			m_numTextures;			//Number Of Textures
	long			MAXNUMBEROFNODES;
	long			MAXNUMBEROFPLANES;
	long			MAXNUMBEROFPOLYGONS;
	long			MAXNUMBEROFLEAFS;
	POLYGON**		m_texturePolygonList;	//Polygons sorting by texture
	POLYGON*		m_polygonList;			//Top of Polygon Linked List
	long			m_PVSCompressedSize;	//Size of compressed PVS array
	char			(*m_textureLUT)[21];		//Texture Name LookUpTable

	//*****************************************************************************
	//Resource arrays that will contain the final tree, portals and PVS
	//*****************************************************************************
	POLYGON*		m_polygonArray;
	NODE*			m_nodeArray;
	LEAF*			m_leafArray;
	PLANE*			m_planeArray;
	XS8*			m_PVSData;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBsp();
	~XBsp();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	BOOL Get_Intersect(XVector3D *linestart,XVector3D *lineend,XVector3D *vertex,
					   XVector3D *normal,XVector3D * intersection, XF32 *percentage);
	BOOL LeafInFrustum(long Leaf);
	BOOL CollideSphere(XVector3D *SphereCenter,XF32 SphereRadius,long Node);
	BOOL LineOfSight(XVector3D *Start,XVector3D *End, long Node);
	void Render(LPDIRECT3DDEVICE9 lpDevice);
	void DrawTree(long leaf,LPDIRECT3DDEVICE9 lpDevice);
	void DeletePolygonArray(void);
	void DeletePolygon(POLYGON *Poly);
	void ExtractFrustumPlanes(XViewingFrustum* vf);
	void LoadTextures(LPDIRECT3DDEVICE9 device);
	void LoadBSPTree(char *filename,LPDIRECT3DDEVICE9 device);
	int ClassifyPoly( PLANE *Plane, POLYGON * Poly );
	int ClassifyPoint(XVector3D *pos,PLANE * Plane);
};

#endif 
#endif