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
//	Module Name:	XBsp.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-2-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


#if 0
//*****************************************************************************
/**
Constructor for the BSP Tree.
*/
//*****************************************************************************
XBsp::XBsp()
{
	m_PVSCompressedSize = 0;
}



//*****************************************************************************
/**
Destructor for the BSP tree.
*/
//*****************************************************************************
XBsp::~XBsp()
{
}



//*****************************************************************************
//Given a plane and a poly, returns the poly's position relative to the plane
//itself (i.e Front/Back/On/Spanning)
//*****************************************************************************
int XBsp::ClassifyPoly(PLANE* plane,POLYGON* poly)
{
	int infront = 0,behind = 0,onPlane = 0;
	XF32 result = 0;
	XVector3D*	vec1 = &plane->PointOnPlane;

	//Loop around each vertex
	for (int a = 0;a < poly->NumberOfVertices;a++)
	{
		XVector3D* vec2 = (XVector3D*)&poly->VertexList[a];
		XVector3D Direction = (*vec1) - (*vec2);
		result = Direction.dot(plane->normal);

		//Check the position
		if (result > g_EPSILON)
			behind++;
		else if (result < -g_EPSILON)
			infront++;
		else
		{
			onPlane++;
			infront++;
			behind++;
		}
	} 

	if (onPlane == poly->NumberOfVertices)
		return CP_ONPLANE;
	if (behind  == poly->NumberOfVertices)
		return CP_BACK;
	if (infront == poly->NumberOfVertices)
		return CP_FRONT;

	//If we make it here, then the poly must be spanning the plane
	return CP_SPANNING;
}



//*****************************************************************************
//Given a plane and a point, returns the points's position relative to the
//plane itself (i.e Front/Back/On)
//*****************************************************************************
int XBsp::ClassifyPoint(XVector3D* pos,PLANE* plane) 
{
	XF32 result;
	XVector3D* vec1 = &plane->PointOnPlane;
	XVector3D Direction = (*vec1) - (*pos);
	result = Direction.dot(plane->normal);

	if (result < -g_EPSILON)
		return CP_FRONT;
	if (result > g_EPSILON)
		return CP_BACK;
	return CP_ONPLANE;
}



//*****************************************************************************
//Simply traverses the tree to find the current leaf the camera in. 
//Once the leaf is found we call DrawTree to draw this leafs PVS Set
//*****************************************************************************
void XBsp::Render(LPDIRECT3DDEVICE9 lpDevice)
{
	//First extract the viewing frustum
	extern XVP xvp;
	XVector3D pos;
	XCamera* pCamera = XVP::getActiveCamera();
	if (pCamera)
		pos = XVector3D(pCamera->getWorldTransform()._41,pCamera->getWorldTransform()._42,pCamera->getWorldTransform()._43);
	xvp.extractFrustumPlanes((XViewingFrustum&)m_frustumPlanes);
	
	int node = 0, leaf=0;
	
	while(true) 
	{
		switch(ClassifyPoint(&pos,&m_planeArray[m_nodeArray[node].Plane]))
		{
			case CP_FRONT: 
				if (m_nodeArray[node].IsLeaf != 0)
				{
					leaf = m_nodeArray[node].Front;
					DrawTree(leaf,lpDevice);
					return;
				}
				else 
					node = m_nodeArray[node].Front;
				break;
			case CP_BACK: 
				if (m_nodeArray[node].Back == -1)
					return;
				else
					node = m_nodeArray[node].Back;
				break;
			case CP_ONPLANE:
				if (m_nodeArray[node].IsLeaf != 0)
				{
					leaf = m_nodeArray[node].Front;
					DrawTree(leaf,lpDevice);
					return;
				} 
				else
					node = m_nodeArray[node].Front;
				break;	
			default:
				break;
		} 
	} 
} 



//*****************************************************************************
//This the method that actually Renders the entire PVS Set.Notice
//that the PVS set is BIT compressed and uses Zero Length Encoding to 
//keep the size of the PVS Set down.
//*****************************************************************************
void XBsp::DrawTree(long leaf,LPDIRECT3DDEVICE9 lpDevice)
{
	extern XVP xvp;

	//Draw the PVS Set
	int i;
	long currentleaf = 0;
	long PVSOFFSET = m_leafArray[leaf].PVSIndex;
	XS8* PVSPointer = m_PVSData;
	POLYGON* CurrentPoly = NULL;
	
	PVSPointer += PVSOFFSET;
	
	//Set All the Texture Batch Pointers to NULL
	for (i = 0;i < m_numTextures;i++)
		m_texturePolygonList[i] = NULL;

	//Loop around leaves
	while (currentleaf < m_numLeafs)
	{
		// If this is a non 0 PVS byte
		if (*PVSPointer != 0)
		{	
			for (i = 0;i < 8;i++)
			{	
				XU8 mask = 1 << i;
				XU8 pvs = *PVSPointer;
				
				if (pvs & mask)
				{
					if (LeafInFrustum(currentleaf) || m_dontFrustumReject)
					{	
						XU32 start = m_leafArray[currentleaf].StartPolygon;
						XU32 finish = m_leafArray[currentleaf].EndPolygon; 
						XU32 Count = 0;

						//Collect the polys from this leaf and add them to
						//the correct texture list
						for (Count = start;Count < finish;Count++)
						{
							CurrentPoly = &m_polygonArray[Count];
							CurrentPoly->Next = m_texturePolygonList[CurrentPoly->TextureIndex];
							m_texturePolygonList[CurrentPoly->TextureIndex] = CurrentPoly;
						}  
					} 
				} 
				currentleaf++;
			} 
			PVSPointer++;
		}
		else
		{
			//we have hit a zero so read in the next byte and see how long
			//the run of zeros is
			PVSPointer++;
			XU8 RunLength = *PVSPointer;
			PVSPointer++;
			currentleaf += RunLength * 8;
		} 
	} 

	//We now have all the polygons in linked lists grouped by texture.  So,
	//just set each texture and render all the polygons in that textures list.
	for (i = 0;i < m_numTextures;i++)
	{
		if (m_texturePolygonList[i] != NULL)
		{
			xvp.GraphicsMgr.setTexture(i+m_textureArrayOffset,0);
			CurrentPoly = m_texturePolygonList[i];
			
			//Traverse linked list
			while (CurrentPoly != NULL)
			{
				lpDevice->SetFVF(D3DFVF_LVERTEX);
				lpDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,
												 CurrentPoly->NumberOfVertices,
												 CurrentPoly->NumberOfIndices/3,
												 &CurrentPoly->Indices[0],
												 D3DFMT_INDEX16,
												 &CurrentPoly->VertexList[0],
												 sizeof(D3DLVERTEX));
				CurrentPoly=CurrentPoly->Next;
			}
		}
	}

	lpDevice->SetTexture(0,NULL);
} 



//*****************************************************************************
//Checks if there is a clear Line of sight between two points be 
//recursing the tree and making sure that none of the line segments
//end up in solid space.
//*****************************************************************************
BOOL XBsp::LineOfSight(XVector3D* start,XVector3D* end,long node)
{
	XF32 temp;
	XVector3D intersection;

	NODE* currNode = &m_nodeArray[node];
	PLANE* plane = &m_planeArray[currNode->Plane];

	int pointA = ClassifyPoint(start,plane);
	int pointB = ClassifyPoint(end,plane);

	//If both points are on the plane
	if ((pointA == CP_ONPLANE) && (pointB == CP_ONPLANE))
	{
		if (currNode->IsLeaf == 0)
			return LineOfSight(start,end,currNode->Front);
		else
			//This is a front leaf.  Front Leafs are always empty so
			//this is empty space
			return true;		
	}

	if ((pointA == CP_FRONT) && (pointB == CP_BACK))
	{
		if (currNode->Back == -1)
			return false; 
		Get_Intersect(start,end,&plane->PointOnPlane,&plane->normal,&intersection,&temp);
		
		if (currNode->IsLeaf == 0)
			return LineOfSight(start,&intersection,currNode->Front) && 
							   LineOfSight(end,&intersection,currNode->Back);
		else
			return TRUE && LineOfSight(end,&intersection,currNode->Back); 

	} 

	if ((pointA == CP_BACK) && (pointB == CP_FRONT))
	{
		if (currNode->Back==-1)
			return FALSE;

		Get_Intersect(start,end,&plane->PointOnPlane,&plane->normal,&intersection,&temp);
 
		if (currNode->IsLeaf==0)
			return LineOfSight(end,&intersection,currNode->Front) && 
							   LineOfSight(start,&intersection,currNode->Back);
		else
			return TRUE && LineOfSight(start,&intersection,currNode->Back); 
	}


	//if we make it here, one of the points is on the plane
	if ((pointA == CP_FRONT) || (pointB == CP_FRONT))
	{
		if (currNode->IsLeaf==0)
			return LineOfSight(start,end,currNode->Front);
		else
			return TRUE;
	} 
	else
	{
		if (currNode->Back==-1)
			return FALSE;
		else
			return LineOfSight(start,end,currNode->Back);
	}

	return TRUE;
}
  


//*****************************************************************************
//Passes sphere through the tree and checks for any parts in solid space.
//*****************************************************************************
BOOL XBsp::CollideSphere (XVector3D* sphereCenter,XF32 sphereRadius,long node)
{  
	XF32 temp;
	int pointA,pointB;
	XVector3D intersection,sphereRadiusEnd,temp1,temp2;
	PLANE* plane = &m_planeArray[m_nodeArray[node].Plane];
	NODE* currNode = &m_nodeArray[node];

	pointA = ClassifyPoint(sphereCenter,plane);

	if (pointA == CP_BACK)
		sphereRadiusEnd =* sphereCenter + (plane->normal * sphereRadius);
	else
		sphereRadiusEnd =* sphereCenter + (-plane->normal * sphereRadius);
	
	pointB = ClassifyPoint(&sphereRadiusEnd,plane);

	//If both points are on the plane
	if ((pointA == CP_ONPLANE) && (pointB == CP_ONPLANE))
	{	
		//if this is not a leaf...recurse
 		if (currNode->IsLeaf == 0)
			return CollideSphere(sphereCenter,sphereRadius,currNode->Front);
		else
			// This is a front leaf.Front Leafs are always empty so this is empty space
			return FALSE;		
	} 

	//Spanning front to back
	if ((pointA == CP_FRONT) && (pointB == CP_BACK))
	{
		if (currNode->Back == -1)
			return TRUE; 

		Get_Intersect(sphereCenter,&sphereRadiusEnd,&plane->PointOnPlane,&plane->normal,
					  &intersection,&temp);
		
		if (currNode->IsLeaf == 0)
		{
			temp1 = intersection - *sphereCenter;
			temp2 = intersection - sphereRadiusEnd;
			return CollideSphere(sphereCenter,temp1.mag(),currNode->Front) || 
								 CollideSphere(&sphereRadiusEnd,temp2.mag(),currNode->Back);
		} 
		else
		{
			temp1 = intersection - sphereRadiusEnd;
			return  CollideSphere(&sphereRadiusEnd,temp1.mag(),currNode->Back); 
		}
	} 

	//spanning back to front
	if ((pointA == CP_BACK) && (pointB == CP_FRONT))
	{
		if (currNode->Back == -1)
			return TRUE;
			
		Get_Intersect(&sphereRadiusEnd,sphereCenter,&plane->PointOnPlane,&plane->normal,&intersection,&temp);
 
		if (currNode->IsLeaf == 0)
		{
			temp1 = intersection - sphereRadiusEnd;
			temp2 = intersection - *sphereCenter;
			return CollideSphere(&sphereRadiusEnd,temp1.mag(),currNode->Front) || 
				   CollideSphere(sphereCenter,temp2.mag(),currNode->Back) ;
		}
		else
		{
			temp1 = intersection - *sphereCenter;
			return CollideSphere(sphereCenter,temp1.mag(),currNode->Back); 
		} 
	} 

	//if we make it here, one of the points is on the plane
	if ((pointA == CP_FRONT) || (pointB == CP_FRONT))
	{
		if (currNode->IsLeaf == 0)
			return CollideSphere(sphereCenter,sphereRadius,currNode->Front);
		else
			return FALSE;		
	}
	else
	{
		if (currNode->Back == -1)
			return TRUE;
		else
			return CollideSphere(sphereCenter,sphereRadius,currNode->Back);
	} 
	return FALSE;
}



//*****************************************************************************
//Returns the intersection point between a ray and a plane
//*****************************************************************************
BOOL XBsp::Get_Intersect(XVector3D* lineStart,XVector3D* lineEnd,XVector3D* vertex,
						  XVector3D* normal,XVector3D* intersection,XF32* percentage)
{
	XVector3D direction, L1;
	XF32 lineLength,distFromPlane;

	direction.x = lineEnd->x - lineStart->x;
	direction.y = lineEnd->y - lineStart->y;
	direction.z = lineEnd->z - lineStart->z;
	lineLength = direction.dot(*normal);
	
	if (fabsf(lineLength) < g_EPSILON)
		return FALSE; 

	L1.x = vertex->x - lineStart->x;
	L1.y = vertex->y - lineStart->y;
	L1.z = vertex->z - lineStart->z;

	distFromPlane = L1.dot(*normal);

	//How far from lineStart, intersection is as a percentage of 0 to 1 
	*percentage = distFromPlane / lineLength; 

	//The plane is behind the start of the line or
	//The line does not reach the plane
	if ((*percentage < 0.0f) || (*percentage > 1.0f))
		return FALSE;

	//add the percentage of the line to line start
	intersection->x = lineStart->x + direction.x * (*percentage);
	intersection->y = lineStart->y + direction.y * (*percentage);
	intersection->z = lineStart->z + direction.z * (*percentage);

	return TRUE;
}


 
//*****************************************************************************
//This method Extracts the Frustum Planes from a Combined 
//View/Projection matrix.
//*****************************************************************************
void XBsp::ExtractFrustumPlanes(XViewingFrustum* vf)
{
	memcpy(m_frustumPlanes,vf->frustumPlanes,sizeof(XPlane) * 6);
}



//*****************************************************************************
//Tests if the Bounding Box surrounding the leaf is completely outside 
//the frustum.
// NOTE : This check is only done on visible leafs in a Leafs PVS Set to 
//		  further carve off more polygons than just the straight PVS Set.
//        Remember a PVS set does not take into account Direction of camera so
//        some of the PVS Set will not be visible.
//*****************************************************************************
BOOL XBsp::LeafInFrustum (long leaf)
{
	XVector3D bMax = m_leafArray[leaf].BoundingBox.BoxMax;
	XVector3D bMin = m_leafArray[leaf].BoundingBox.BoxMin;
	XVector3D nearPoint,farPoint;
	XPlane* plane = m_frustumPlanes;
	
	for (int i = 0; i < 6; i++) 
	{
		if (plane->normal.x > 0.0)
		{
			if (plane->normal.y > 0.0)
			{
				if (plane->normal.z > 0.0)
				{
					nearPoint.x = bMin.x; nearPoint.y = bMin.y; nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMin.x; nearPoint.y = bMin.y; nearPoint.z = bMax.z;
				} 
			} 
			else
			{
				if (plane->normal.z > 0.0)
				{
					nearPoint.x = bMin.x; nearPoint.y = bMax.y; nearPoint.z = bMin.z;
				} 
				else
				{
					nearPoint.x = bMin.x; nearPoint.y = bMax.y; nearPoint.z = bMax.z;
				} 
			} 
		}
		else
		{
			if (plane->normal.y > 0.0)
			{
				if (plane->normal.z > 0.0)
				{
					nearPoint.x = bMax.x; nearPoint.y = bMin.y; nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMax.x; nearPoint.y = bMin.y; nearPoint.z = bMax.z;
				} 
			}
			else
			{
				if (plane->normal.z > 0.0)
				{
					nearPoint.x = bMax.x; nearPoint.y = bMax.y; nearPoint.z = bMin.z;
				}
				else
				{
					nearPoint.x = bMax.x; nearPoint.y = bMax.y; nearPoint.z = bMax.z;
				} 
			} 
		} 

		// near extreme point is outside, and thus
		// the AABB is Totally outside the polyhedron
		if (plane->normal.dot(nearPoint) + plane->distance > 0)
			return TRUE;
		
		plane++;
	} 
	
	return FALSE;
}



//*****************************************************************************
//Delete Polygon Array
//*****************************************************************************
void XBsp::DeletePolygonArray(void)
{
	for (long i = 0; i < m_numPolygons; i++)
	{
		delete m_polygonArray[i].VertexList;
		delete m_polygonArray[i].Indices;
	}
	
	free(m_polygonArray);
}



//*****************************************************************************
// Loads the BSP arrays into memory for rendering
// NOTE : The portals are not saved to this file so we don't load them
//	      (although they could be if you needed them) because they are only 
//        currently used to calcuate the PVS and so are not needed at run time.
//*****************************************************************************
void XBsp::LoadBSPTree(char *filename,LPDIRECT3DDEVICE9 device)
{
	long a;
	FILE* stream;
	stream = fopen(filename, "rb");

	//Read Node array.  We can read the array as one block 
	fread(&m_numNodes,sizeof(long),1,stream);
	m_nodeArray = (NODE*)malloc(m_numNodes * sizeof(NODE));
	
	NODE* n = m_nodeArray;
	for (a = 0;a < m_numNodes;a++)
	{
		fread(&n->IsLeaf,sizeof (unsigned char),1,stream);
		fread(&n->Plane,sizeof (unsigned long),1,stream);
		fread(&n->Front,sizeof (unsigned long),1,stream);
		fread(&n->Back ,sizeof (signed long),1,stream);
		n++;
	}

	//Read the Plane Array
	fread(&m_numPlanes,sizeof(long),1,stream);
	m_planeArray = (PLANE*)malloc(m_numPlanes * sizeof(PLANE));	
	fread(m_planeArray,sizeof(PLANE),m_numPlanes,stream);

	//Read Leaf Array.
	fread(&m_numLeafs,sizeof(long),1,stream);
	m_leafArray = (LEAF*)malloc(m_numLeafs * sizeof(LEAF));
	
	LEAF* l = m_leafArray;
	for (a = 0;a < m_numLeafs;a++)
	{
		fread(&l->StartPolygon,sizeof(long),1,stream);
		fread(&l->EndPolygon,sizeof(long),1,stream);
		fread(&l->PVSIndex,sizeof(long),1,stream);
		fread(&l->BoundingBox,sizeof(BOUNDINGBOX),1,stream);
		l++;
	}

	//Read Polygon Array
	fread(&m_numPolygons,sizeof(long),1,stream);
	m_polygonArray = (POLYGON*)malloc(m_numPolygons * sizeof(POLYGON));
	POLYGON* p = m_polygonArray;

	for (a = 0; a < m_numPolygons;a++)
	{
		fread(&p->NumberOfVertices,sizeof(WORD),1,stream);
		p->VertexList=new D3DLVERTEX[p->NumberOfVertices];
		fread(p->VertexList,sizeof(D3DLVERTEX),p->NumberOfVertices,stream);
		fread(&p->NumberOfIndices,sizeof(WORD),1,stream);
		p->Indices=new WORD [p->NumberOfIndices];
		fread(p->Indices,sizeof(WORD),p->NumberOfIndices,stream);
		fread(&p->Normal,sizeof(XVector3D),1,stream);
		fread(&p->TextureIndex,sizeof(WORD),1,stream);
		p++;
	} 

	//Now all we have to do is read the PVS Data itself
	fread(&m_PVSCompressedSize,sizeof(long),1,stream);
	m_PVSData = (XS8*)malloc(m_PVSCompressedSize * sizeof(XU8));
	fread(m_PVSData,sizeof(XU8),m_PVSCompressedSize,stream);

	//And then read in the texture look up table
	fread(&m_numTextures,sizeof(WORD),1,stream);				// Read number of textures used
	m_textureLUT = new XU8[m_numTextures][21];					// Allocate a set of 21 character strings.
	fread(m_textureLUT,sizeof(char) * 21, m_numTextures,stream);// Read all sets of 21 char strings

	// All done
	fclose(stream);

	//Now we can load the textures
	LoadTextures(device);
}



//*****************************************************************************
// Name : LoadTextures ()
// Desc : Loads the textures for level rendering
//*****************************************************************************
void XBsp::LoadTextures(LPDIRECT3DDEVICE9 device)  
{
	extern XVP xvp;
	XU32 temp;
	char textureName[25];
	
	//Allocate Texture Sorting Array
	m_texturePolygonList = new POLYGON*[m_numTextures];
	strcpy(textureName,m_textureLUT[0]);
	strcat(textureName,".JPG");
	xvp.GraphicsMgr.createTexture(textureName,X_TT_2D,m_textureArrayOffset);
			
	for (XU32 i = 1;i < m_numTextures;i++)
	{
		strcpy(textureName,m_textureLUT[i]);
		strcat(textureName,".JPG");
		xvp.GraphicsMgr.createTexture(textureName,X_TT_2D,temp);
	}
}


#endif