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
//	Module Name:	XOctree.h
//	Author:			Lynn Duke
//	Creation Date:	12-30-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_OCTREE_H__
#define __X_OCTREE_H__



//*****************************************************************************
//Structures
//*****************************************************************************
struct octreeRenderBlock
{
public:
	XHandle ibHandle;
	XHandle matHandle;
	XHandle texHandle;
	XU32 nextInsertionPoint;
	XU32 numPolys;
	WORD* ibPtr;
	XBOOL inUse;
};



//*****************************************************************************
//*****************************************************************************
class XOctreeNode
{
public:
	int Node_ID;
	signed int Children[8];
	signed int Neighbors[6];
	XVector3D Max;
	XVector3D Min;
	XVector3D Center;
	XF32 SphereRadius;
	XF32 Size; 
	XBOOL rendered;
	XBOOL collided;
	int NumPolygons;
	std::vector<int> Polygons;
	char status;
	XHandle ibHandle; 
	
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XOctreeNode();
};



//*****************************************************************************
//*****************************************************************************
class XOctree : public XSceneNode
{
public:
	struct XModelVertex
	{
		XVector3D	pos;		/*!< Vertex position */
		XVector3D	normal;		/*!< Vertex Normal */
		XU32		diffuse;	/*!< Diffuse color */
		XVector2D	texCoords;	/*!< 2 texture coordinates */
	};

private:
	XU32						m_vertSpreadFactor;
	XU32						m_vertRiseFactor;
	XMatrix4x4					m_octreeWorldMatrix;
	XBOOL						m_hasMeshBeenLoaded;
	int							m_numNodes;
	vector<XOctreeNode>		m_masterNodeList;
	vector<octreeRenderBlock>	m_renderBlockList;
	XU32						m_rblSize;
	XU32						m_numTotalPolysInOctants; 
	XS8*						m_polyMatIndexArray;
	WORD*						m_ibPtr;
	XModelVertex*				m_vbPtr;
	int							m_maxPolysPerNode;
	int							m_numCollidablePolys;
	XU32*						m_nodeBits;
	XU32*						m_polyBits;
	XBOOL						m_sphereCull;
	int							m_numFrustumTests;
	int							m_numCollisionTests;
	int							m_numIntersects;
	XPolygon					p;
	XCamera*					m_pCamera;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus destroy();
	XS8* getTypeDesc();
	void buildOctree(int index);
	void subdivideBox(XOctreeNode *parent, XOctreeNode *child, int index);
	void fillNode(XOctreeNode *parent, XOctreeNode *child);
	XBOOL polygonInsideNode(XOctreeNode *node, int polygon);
	void initializeOctree();
	XBOOL finalizeOctree();
	XBOOL createIndexBuffers();
	void findNeighbor(int index, XBoundingBoxSide *Side, int Idx, int *Found, XF32 *FoundSize);
	int  getOppositeIndex(int index);
	void renderTree(int index);
	void renderTree2();
	void renderAllChildren(int index);
	void sphereCollide(int index, XVector3D ocenter, XF32 oradius);
	void rayNodeIntersection(XVector3D startpos, XVector3D endpos);
	void intersectRayNode(int index, XVector3D start, XVector3D end, XVector3D dir);
	void setCurrentNode(int index, XVector3D *p);

public:
	XHandle					m_vbHandle;
	XHandle					m_ibHandle;
	XU32					m_numTotalPolys;
	XU32*					m_collidablePolyBits;
	vector<XPolygon>		m_collidablePolyList;
	XU32*					numPolys;
	XPolygon*				p_polys;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XOctree();
	virtual ~XOctree();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	/*void loadHeightMap(XS8* hmFilename,int spreadFactor,int riseFactor,XU32 maxPolygonsPerNode,
					   XBOOL generateNormals,XEngine* xep);*/
	void printTree(XS8* pFilename);
	XStatus init(XS8* pFilename,XU32 maxPolygonsPerNode);
	XStatus render();
	void markCurrentNode(XVector3D position, XBOOL mark_neighbors = false);
	void clearMarkings(){if(m_nodeBits)X_CLEAR_BIT_ARRAY(m_nodeBits,m_numNodes)}
	void traceRay(XRay3D ray);
	void renderCurrentNode(XVector3D position);
	void renderWireframeTree(int index,XColor color);
	//void renderMarkedNodes(int index, XEngine *device, DWORD color = 0xFF00ff00);
	//void renderCurrentNode(XVector3D position, XEngine *device, DWORD color = 0xFF00ff00);	
	//XBOOL lineOfSight(XVector3D start, XVector3D end, XBOOL ignore_transparent = true, int pindex = -100, int *tri_hit = NULL, XVector3D *intpoint = NULL);
	void collideAABB(XVector3D min, XVector3D max);
	void AABBCollide(int index, XVector3D min, XVector3D max);
	void collideSphere(XVector3D center, XF32 radius);
	int  getNumCollidablePolys() {return m_numCollidablePolys;}
	void getCurrentNode(XVector3D position, int index, int *node);
	XBOOL saveOctreeToFile(XS8* filename);
	XBOOL loadOctreeFromFile(XS8* filename);
	void toggleCullType(){m_sphereCull = !m_sphereCull;}
	XMaterial* getRenderBlockRSPtr(XU32 texStage,XS8* textureFilename);
	XMaterial* getRenderBlockRSPtr(XU32 renderBlockID);
	XStatus setCamera(XCamera* pCamera);
	XCamera* getCamera();
};

#endif