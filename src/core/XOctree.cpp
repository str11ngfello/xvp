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
//	Module Name:	XOctree.cpp
//	Author:			Lynn Duke
//	Creation Date:	12-30-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//Defines
//*****************************************************************************
#define XO_NULL_NODE    -1
#define XO_MAX_VAL	((XF32)(unsigned int)(1 << 31))



//*****************************************************************************
/**
Constructor for the octree.  
*/
//*****************************************************************************
XOctreeNode::XOctreeNode()
{	
	Node_ID = -100;
	SphereRadius = 0;
	NumPolygons = 0;

	Min = XVector3D(20000000.0f,20000000.0f,20000000.0f);
	Max = XVector3D(-20000000.0f,-20000000.0f,-20000000.0f);
	Center = XVector3D(0,0,0);
	Size = 0;
	
	rendered = false;
	collided = false;

	for(int i = 0; i < 8; i++)
		Children[i] = XO_NULL_NODE;
	for(int j = 0; j < 6; j++)
		Neighbors[j] = XO_NULL_NODE;
}



//*****************************************************************************
/**
Destructor for the octree.
*/
//*****************************************************************************
XOctree::XOctree()
{
	m_numTotalPolysInOctants = 0;
	m_maxPolysPerNode = 0;
	m_numNodes = 0;
	m_numCollidablePolys = 0;
	m_nodeBits = NULL;
	m_polyBits = NULL;
	m_collidablePolyBits = NULL;
	m_sphereCull = false;
	m_hasMeshBeenLoaded = false;
	m_numFrustumTests = 0;
	m_numCollisionTests = 0;
	m_numIntersects = 0;
	m_pCamera = NULL;
}



//*****************************************************************************
/**
Returns a string description of the octree.  This is the
description that appears in log files and other debug output.

@return A string description of the octree.
*/
//*****************************************************************************
XS8* XOctree::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XOctree";
	return description; 
}



//*****************************************************************************
//*****************************************************************************
XStatus XOctree::destroy()
{
	X_DESTROY_BIT_ARRAY(m_nodeBits)
	X_DESTROY_BIT_ARRAY(m_polyBits)
	X_DESTROY_BIT_ARRAY(m_collidablePolyBits)
	XDeleteArray(m_polyMatIndexArray)

	//if (m_pX)
	//{
		XVP::GraphicsMgr.destroyVertexBuffer(m_vbHandle);
		XVP::GraphicsMgr.destroyIndexBuffer(m_ibHandle);
	//}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
/*void XOctree::loadHeightMap(XS8* hmFilename,int spreadFactor,int riseFactor,
							 XU32 maxPolygonsPerNode,XBOOL generateNormals,XVP* x)
{
	//Make sure that the filename is valid
	if (!hmFilename)
		return;
	
	//Record the XEngine pointer and the maxPolygonsPerNode
	m_pX = x;
	m_maxPolysPerNode = maxPolygonsPerNode;

	//First, we need to load the heightmap in from disk.
	XBitmap hm;
	hm.load(hmFilename);

	//Next, load in the color reference map
	XBitmap tex;
	tex.load(hmFilename);

	//Also, save the passed in rise and spread factor.
	m_vertSpreadFactor = spreadFactor;
	m_vertRiseFactor = riseFactor;

	//Create a new vertex buffer. The vertex buffer
	//will be big enough to hold all the heightmap's pixels.
	if (XVP::GraphicsMgr.createVertexBuffer(X_VT_1N_1D_1T,X_PT_TRIANGLE_LIST,10000,m_vbHandle) != X_OK)
		return;
	
	
	//Now we need to cycle through the pixels in the heightmap and
	//derive the associated vertices from the grayscale color. Since
	//the heightmap is in greyscale, we can read R,G or B and they will
	//all be the same. I choose green (G). First lock the vertex buffer,
	//then fill it up.
	XVertex1N1D1T* pVertices;

	unsigned int vertCounter = 0;
	int spreadCounterX = (int)((hm.getWidth()/2) * m_vertSpreadFactor) * -1;
	int spreadCounterY = (int)(hm.getHeight()/2) * m_vertSpreadFactor;
	unsigned char rgb1[3],rgb2[3];
	pVertices = (XVertex1N1D1T*)XVP::GraphicsMgr.lockVertexBuffer(m_vbHandle);
	for (int i = 0;i < hm.getHeight();i++)
	{
		for (int j = 0;j < hm.getWidth();j++)
		{
			hm.getColor(j,i,&rgb1[0],&rgb1[1],&rgb1[2]);
			tex.getColor(j,i,&rgb2[0],&rgb2[1],&rgb2[2]);
			pVertices[vertCounter].diffuse = D3DCOLOR_ARGB(255,rgb2[0],rgb2[1],rgb2[2]);
			pVertices[vertCounter].pos.x = (XF32)spreadCounterX;
			pVertices[vertCounter].pos.z = (XF32)spreadCounterY;
			pVertices[vertCounter].pos.y = (XF32)rgb1[1] * m_vertRiseFactor;

			vertCounter++;
			spreadCounterX += m_vertSpreadFactor;
		}
		spreadCounterY -= m_vertSpreadFactor;
		spreadCounterX = (int)((hm.getWidth()/2) * m_vertSpreadFactor) * -1;
	}

	//Don't forget to release the vertex buffer.
	XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);

	//Now that we have derived the vertices, we will
	//now create the index buffer and derive the polygons
	//based on a systematic way of numbering the pixels in
	//the height map.
	//
	//      pixels                     polygons
	//   . . . . . . .              ._._._._._._.  
	//   . . . . . . .              |/|/|/|/|/|/|
	//   . . . . . . .  becomes ->  |/|/|/|/|/|/|
	//   . . . . . . .              |/|/|/|/|/|/|
	//   . . . . . . .              |/|/|/|/|/|/|

	WORD* pIndices;
	XS32 numTriangles = 2 * (hm.getWidth()-1) * (hm.getHeight()-1);
	XVP::GraphicsMgr.createIndexBuffer(numTriangles*3,m_ibHandle);
	pIndices = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle);
	
	//Now comes the "systematic numbering of pixels into triangles" mentioned
	//above. 
	XS32 rowNumber = 0;
	XS32 indexCounter = 0;
	for (i = 0;i < hm.getHeight()-1;i++)
	{
		for (int j = 0;j < hm.getWidth()-1;j++)
		{ 
			//Triangle 1 
			pIndices[indexCounter] = j + (rowNumber*hm.getWidth());
			pIndices[indexCounter+1] = j + 1 + (rowNumber*hm.getWidth());
			pIndices[indexCounter+2] = j + hm.getWidth() + (rowNumber*hm.getWidth());
			
			//Triangle 2
			pIndices[indexCounter+3] = j + hm.getWidth() + (rowNumber*hm.getWidth());
			pIndices[indexCounter+4] = j + 1 + (rowNumber*hm.getWidth());
			pIndices[indexCounter+5] = j + 1 + hm.getWidth() + (rowNumber*hm.getWidth());

			indexCounter+=6;
		}
		rowNumber++;
	}

	//Don't forget to release the index buffer when we are done.
	XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);

	m_hasMeshBeenLoaded = true;

	//Generate the normals if specified
	if (generateNormals)
		XVP::generateNormals(m_vbHandle,m_ibHandle);

	//Finish the octree building process
	initializeOctree();
}
*/


//*****************************************************************************
//*****************************************************************************
XStatus XOctree::init(XS8* pFilename,XU32 maxPolygonsPerNode)
{
	//Make sure that the passed in filename is valid
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;
	
	m_maxPolysPerNode = maxPolygonsPerNode;

	XModelVertex* m_vbPtr = NULL;
	WORD* m_ibPtr = NULL;
	XModelLoader loader;
	XmfMaterial* matPtr = NULL;
	XmfGeometry* objPtr = NULL;
	XMaterial* pRs = NULL;

	//Load in the mesh data
	loader.load(pFilename);	

	//Cycle through the materials listed in the loader.m_model and create a render
	//block for each one.  This will later be used to group together
	//visible polys that share the same material properties.
	for (XU32 i = 0;i < loader.m_model.materialList.size();i++)
	{
		octreeRenderBlock rb;
		rb.inUse = false;
		rb.nextInsertionPoint = 0;
		rb.numPolys = 0;
		
		//Get pointers to the important stuff for the current loop iteration
		objPtr = &loader.m_model.objectList[i];
		matPtr = &loader.m_model.materialList[objPtr->materialID];

		//Create the render state.
		if (XVP::GraphicsMgr.createMaterial(NULL,rb.matHandle) != X_OK)
			return X_ERR_OPERATION_FAILED;

		//Grab an interface to the render state.
		pRs = XVP::GraphicsMgr.getMaterialInterface(rb.matHandle);
        
		//Add texture to render state.
		if (loader.m_model.materialList[i].texFilename)
		{
			if (XVP::GraphicsMgr.createTextureFromFile(loader.m_model.materialList[i].texFilename,rb.texHandle) != X_OK)
				return X_ERR_OPERATION_FAILED;
			pRs->setTextureHandle(0,rb.texHandle);
			XVP::GraphicsMgr.destroyTexture(rb.texHandle);
		}

		XColor diffuse(matPtr->diffuseColor.x,
						matPtr->diffuseColor.y,
						matPtr->diffuseColor.z,
						matPtr->diffuseColor.w);
		XColor ambient(matPtr->ambientColor.x,
						matPtr->ambientColor.y,
						matPtr->ambientColor.z,
						matPtr->ambientColor.w);
		XColor specular(matPtr->specularColor.x,
						matPtr->specularColor.y,
						matPtr->specularColor.z,
						matPtr->specularColor.w);

			//pRs->m_ambient = ambient;
			//pRs->m_diffuse = diffuse;
			//pRs->m_specular = specular;
			//pRs->m_specPower = matPtr->specPower;

		/*if (loader.m_model.materialList[i].fillMode == 0)
			pRs->setFillMode(X_FMT_SOLID);
		else
			pRs->setFillMode(X_FMT_WIRE);*/
			
		//pRs->enableLighting();

		//Finally push the newly created render block onto the list.
		m_renderBlockList.push_back(rb);
	}

	//Store the size of the render block list
	m_rblSize = (XU32)m_renderBlockList.size();

	//Calculate the total number of vertices as the sum of
	//each objects vertices. Also calculate the total sum of all
	//faces from each object. 
	XU32 numFacesFromAllModels = 0;
	XU32 numVertsFromAllModels = 0;
	for (XU32 i = 0;i < loader.m_model.objectList.size();i++)
	{
		objPtr = &loader.m_model.objectList[i];
		numVertsFromAllModels += (XU32)objPtr->vertexList.size();
		numFacesFromAllModels += (XU32)objPtr->polyFaceList.size();
	}

	//Create an array to hold the material index for each poly
	XNewArray(m_polyMatIndexArray,XS8,numFacesFromAllModels,XOSMgr::getDefaultHeap());

	XVertexDesc desc;
	desc.numNormals = 1;
	desc.numUVs = 1;
	desc.numColors = 1;

	XVP::GraphicsMgr.createVertexBuffer(desc,X_PT_TRIANGLE_LIST,numVertsFromAllModels,false,m_vbHandle);
	m_vbPtr = (XModelVertex*)XVP::GraphicsMgr.lockVertexBuffer(m_vbHandle,X_LT_WRITE_ONLY);

	XU32 ctr = 0;
	for (XU32 i = 0;i < loader.m_model.objectList.size();i++)
	{
		//Get pointers to the important stuff for the current loop iteration
		objPtr = &loader.m_model.objectList[i];
		matPtr = &loader.m_model.materialList[objPtr->materialID];

		XColor diffuse(matPtr->diffuseColor.x,
						matPtr->diffuseColor.y,
						matPtr->diffuseColor.z,
						matPtr->diffuseColor.w);

		if ((objPtr->texCoordList.size() > 0) && (objPtr->texFaceList.size() > 0))
		{
			for (XU32 k = 0;k < objPtr->polyFaceList.size();k++)
			{
				m_vbPtr[objPtr->polyFaceList[k].x + ctr].texCoords = objPtr->texCoordList[objPtr->texFaceList[k].x];
				m_vbPtr[objPtr->polyFaceList[k].y + ctr].texCoords = objPtr->texCoordList[objPtr->texFaceList[k].y];
				m_vbPtr[objPtr->polyFaceList[k].z + ctr].texCoords = objPtr->texCoordList[objPtr->texFaceList[k].z];
			}
		}

		for (XU32 j = 0;j < objPtr->vertexList.size();j++)
		{
			m_vbPtr[ctr].pos = objPtr->vertexList[j];
			m_vbPtr[ctr].normal = objPtr->vNormalList[j];

			//Now fill in vertex color info is present, if not just set the diffuse color
			//for the vertex to that of the diffuse material
			if (loader.m_model.objectList[i].cpvVertList.size() > 0)
			{
				m_vbPtr[ctr].diffuse = 0;
				m_vbPtr[ctr].diffuse |= ((XU8)(objPtr->cpvVertList[j].a * 255)) << 24;
				m_vbPtr[ctr].diffuse |= ((XU8)(objPtr->cpvVertList[j].r * 255)) << 16;
				m_vbPtr[ctr].diffuse |= ((XU8)(objPtr->cpvVertList[j].g * 255)) << 8;
				m_vbPtr[ctr].diffuse |= ((XU8)(objPtr->cpvVertList[j].b * 255));
				
			}
			else
				m_vbPtr[ctr].diffuse = diffuse.getARGB32Bit();

			ctr++;
		}
	}

	//Read in vertex normals.
	//for (i = 0;i < objPtr->vNormalList.size();i++)
	//	m_vbPtr[i].normal = objPtr->vNormalList[i];


	XU32 k = 0;
	XU32 previousVerticesOffset = 0;
	XVP::GraphicsMgr.createIndexBuffer(numFacesFromAllModels*3,false,m_ibHandle);
	m_ibPtr = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle,X_LT_DISCARD);

	for (XU32 i = 0;i < loader.m_model.objectList.size();i++)
	{
		//Get pointers to the interesting stuff during the current iteration
		objPtr = &loader.m_model.objectList[i];

		//We need to offset the indices of each object, by the previous
		//objects indices.
		previousVerticesOffset = 0;
		for (XU32 l = 0;l < i;l++)
			previousVerticesOffset += (XU32)loader.m_model.objectList[l].vertexList.size();

		for (XU32 j = 0;j < objPtr->polyFaceList.size();j++)
		{
			m_ibPtr[k] = objPtr->polyFaceList[j].x + previousVerticesOffset;
			m_ibPtr[k+1] = objPtr->polyFaceList[j].y + previousVerticesOffset;
			m_ibPtr[k+2] = objPtr->polyFaceList[j].z + previousVerticesOffset;
		
			//Set the material ID for this poly in the m_polyMatIndexArray
			m_polyMatIndexArray[k/3] = objPtr->materialID;

			k+=3;
		}
	}

	XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);
	XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);

	m_hasMeshBeenLoaded = true;

	//set the total num of polys
	m_numTotalPolys = XVP::GraphicsMgr.getIndexBufferLength(m_ibHandle) / 3;

	//Generate the vertex normals if they are not found in the file.
	if (objPtr->vNormalList.size() == 0)
	{    
		//Generate the normals.  This call will lock the vertex buffer
		//so we use is here after the vertex buffer has been unlocked from 
		//the above for loop.
		//XVP::generateNormals(m_vbHandle,m_ibHandle);
	} 

	//Clean up the data
	loader.destroyModelData();

	//Finish building the Octree
	initializeOctree();

	return X_OK;	
}



//*****************************************************************************
//*****************************************************************************
void XOctree::initializeOctree()
{
	XF32 max = -200000000.0f;
	
	//Get the vertex and index buffer pointers and number of vertices 
	//and indices.
	XModelVertex* m_vbPtr = (XModelVertex*)XVP::GraphicsMgr.lockVertexBuffer(m_vbHandle,X_LT_WRITE_ONLY);
	WORD* m_ibPtr = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle,X_LT_READ_ONLY);
	XU32 numVerts = XVP::GraphicsMgr.getVertexBufferLength(m_vbHandle);
	XU32 numIndices = XVP::GraphicsMgr.getIndexBufferLength(m_ibHandle);
	XU32 numPolys = numIndices/3;

	//Create the root node which intially holds all meshes and triangles
	XOctreeNode root;
	root.Node_ID = m_numNodes;	
	root.NumPolygons = numPolys;

	XF32 value;
	for(XU32 i = 0;i < numVerts;i++)
	{
		value = fabs(m_vbPtr[i].pos.x);
		if (value > max)
			max = value;
		value = fabs(m_vbPtr[i].pos.y);
		if (value > max)
			max = value;
		value = fabs(m_vbPtr[i].pos.z);
		if (value > max)
			max = value;
	}
	
	//Release the vertex and index buffer pointers.
	XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);
	XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);

	//Add the polygon indices to the root node
	for (XU32 i = 0;i < numPolys;i++)
		root.Polygons.push_back(i);

	root.Max = XVector3D( max, max, max);
	root.Min = XVector3D(-max,-max,-max);

	//Fill in remaining root node info
	root.Center  = root.Min + root.Max;
	root.Center /= 2.0f;

	//Calculate the sphere radius
	XF32 dx = root.Max.x - root.Center.x;
	XF32 dy = root.Max.y - root.Center.y;
	XF32 dz = root.Max.z - root.Center.z;
	root.SphereRadius = (XF32)sqrt(dx * dx + dy * dy + dz * dz);

	//Calc the length of the side
	root.Size = fabs(root.Max.x - root.Min.x);

	//Add the root node to the list
	m_masterNodeList.push_back(root);
	m_numNodes++;

	//Proceed with building the octree
	buildOctree(0);

	//Calculate the neighbors
	finalizeOctree();

	//Create a bit array for the nodes
	X_CREATE_BIT_ARRAY(m_nodeBits,m_numNodes);
	X_CLEAR_BIT_ARRAY(m_nodeBits,m_numNodes);

	//Create a bit array for the polygon list.
	X_CREATE_BIT_ARRAY(m_polyBits,m_numTotalPolys);
	X_CLEAR_BIT_ARRAY(m_polyBits,m_numTotalPolys);

	//Create a bit array for the polygon list.
	X_CREATE_BIT_ARRAY(m_collidablePolyBits,m_numTotalPolys);
	X_CLEAR_BIT_ARRAY(m_collidablePolyBits,m_numTotalPolys);
}



//*****************************************************************************
//*****************************************************************************
void XOctree::buildOctree(int index)
{
	//See if we need to subdivide this node
	if(m_masterNodeList[index].NumPolygons > m_maxPolysPerNode){

		//Initialize node's children
		for(int i = 0; i < 8; i++)
		{	
			XOctreeNode NewNode;
			NewNode.Node_ID = m_numNodes;

			//Set this node's child index
			m_masterNodeList[index].Children[i] = m_numNodes;

			//Partition a new box for the child
			subdivideBox(&m_masterNodeList[index], &NewNode, i);
			
			//See which of the parent's triangles lie within the new node
			fillNode(&m_masterNodeList[index], &NewNode);
			
			//Add the new node and recurse on its children
			m_masterNodeList.push_back(NewNode);
			m_numNodes++;
		}
		
		//Don't need the parents polys anymore
		int matches = 0;
		XBOOL found = false;
		int testid = 0;
		for(int jj = 0; jj < m_masterNodeList[index].NumPolygons; jj++)
		{
			testid = m_masterNodeList[index].Polygons[jj];
			found  = false;
			for(int kk = 0; kk < 8; kk++)
			{
				int childindex = m_masterNodeList[index].Children[kk];
				XOctreeNode *node = &m_masterNodeList[childindex];
				for(int ll = 0; ll < node->NumPolygons; ll++)
				{
					if(node->Polygons[ll] == testid){
						found = true;							
						matches++;
						break;		
					}
				}
				if(found)
					break;
			}
		}
		
		m_masterNodeList[index].Polygons.clear();
		m_masterNodeList[index].NumPolygons = 0;
		
		//Recurse on this node's children
		for(int j = 0; j < 8; j++)
			buildOctree(m_masterNodeList[index].Children[j]);
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::fillNode(XOctreeNode *parent, XOctreeNode *child)
{
   	child->NumPolygons = 0;
	for(int pindex = 0; pindex < parent->NumPolygons; pindex++)
	{
		int polygon_index = parent->Polygons[pindex];
		if(polygonInsideNode(child, polygon_index))
		{
			child->Polygons.push_back(polygon_index);
			child->NumPolygons++;					
			m_numTotalPolysInOctants++;
		}
	}
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOctree::polygonInsideNode(XOctreeNode *node, int pindex)
{
	XU32 vertIndex = pindex * 3;
	
	//Get the vertex and index buffer pointers.
	XModelVertex* m_vbPtr = (XModelVertex*)XVP::GraphicsMgr.lockVertexBuffer(m_vbHandle,X_LT_WRITE_ONLY);
	WORD* m_ibPtr = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle,X_LT_READ_ONLY);

	if ((XIAABBTriangle(node->Min,node->Max,
						  m_vbPtr[m_ibPtr[vertIndex]].pos,
						  m_vbPtr[m_ibPtr[vertIndex+1]].pos,
						  m_vbPtr[m_ibPtr[vertIndex+2]].pos)) == 0)
	{		
		//Release the vertex and index buffer pointers.
		XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);
		XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);
		return true;
	}
	else
	{		
		//Release the vertex and index buffer pointers.
		XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);
		XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);
		return false;
	}
} 



//*****************************************************************************
//*****************************************************************************
void XOctree::subdivideBox(XOctreeNode *parent, XOctreeNode *child, int i)
{	
	XVector3D BoxSize;
	BoxSize.x = (parent->Max.x - parent->Min.x);
	BoxSize.y = (parent->Max.y - parent->Min.y);
	BoxSize.z = (parent->Max.z - parent->Min.z);

	if(i % 2 == 0)
		child->Center.x = (parent->Center.x - (BoxSize.x / 4));
	else
		child->Center.x = (parent->Center.x + (BoxSize.x / 4));

	if(i < 4)
		child->Center.y = (parent->Center.y - (BoxSize.y / 4));
	else
		child->Center.y = (parent->Center.y + (BoxSize.y / 4));

	if(i == 2 || i == 3 || i == 6 || i == 7)
		child->Center.z = (parent->Center.z - (BoxSize.z / 4));
	else
		child->Center.z = (parent->Center.z + (BoxSize.z / 4));


	child->Min.x = (child->Center.x - BoxSize.x / 4);
	child->Min.y = (child->Center.y - BoxSize.y / 4);
	child->Min.z = (child->Center.z - BoxSize.z / 4);

	child->Max.x = (child->Center.x + BoxSize.x / 4);
	child->Max.y = (child->Center.y + BoxSize.y / 4);
	child->Max.z = (child->Center.z + BoxSize.z / 4);
	
	//Calculate the sphere radius for box as well
	XF32 dx = child->Max.x - child->Center.x;
	XF32 dy = child->Max.y - child->Center.y;
	XF32 dz = child->Max.z - child->Center.z;
	child->SphereRadius = (XF32)sqrt(dx * dx + dy * dy + dz * dz);

	//Calc the length of a cube side
	child->Size = fabs(child->Max.z - child->Min.z);
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOctree::finalizeOctree()
{
	//Find and assign neighbors for every node in the tree
	for(int i = 0; i < m_numNodes; i++)
	{
		XOctreeNode *Node = &m_masterNodeList[i];

		for(int j = 0; j < 6; j++)
		{
			int found = XO_NULL_NODE;
            XF32 foundSize = XO_MAX_VAL;

			XBoundingBox box;
			box.Max = Node->Max;
			box.Min = Node->Min;

			XBoundingBoxSide Side;
			Side.SetFromBBox(box, j);

			findNeighbor(0, &Side, j, &found, &foundSize);
	        Node->Neighbors[j] = found;
		}
	}

	return true;
}



//*****************************************************************************
//*****************************************************************************
void XOctree::findNeighbor(int index,XBoundingBoxSide *Side,int Idx,int *Found,XF32 *FoundSize)
{
	//Node to start from
	XOctreeNode *parent = &m_masterNodeList[index];

	if(parent && parent->Children[0] != XO_NULL_NODE){

        for(int i = 0; i < 8; i++)
		{
            XOctreeNode *Child = &m_masterNodeList[parent->Children[i]];

            //Find the side of the test box facing the opposite
            //direction of the current side
            int TestIdx = getOppositeIndex(Idx);

			XBoundingBox box;
			box.Max = Child->Max;
			box.Min = Child->Min;

            XBoundingBoxSide TestSide;
            TestSide.SetFromBBox(box, TestIdx);

            //If it's the same size or greater, test to see if 
            //planes are neighbors
            if(Side->GetSize() <= TestSide.GetSize())
            {
                if(Side->Neighbors(TestSide))
                    if(TestSide.GetSize() < *FoundSize)
                    {
                        *Found = parent->Children[i];
                        *FoundSize = TestSide.GetSize();
                    }

                //Keep searching if we haven't found a perfect fit
                if(*FoundSize != Side->GetSize())                                 
                    findNeighbor(parent->Children[i], Side, Idx, Found, FoundSize);
            }
        }
 	}
}



//*****************************************************************************
//*****************************************************************************
int XOctree::getOppositeIndex(int index)
{
    if(index % 2 == 0)
        return(index + 1);
    else
        return(index - 1);
}



//*****************************************************************************
//*****************************************************************************
XStatus XOctree::render()
{
	//Make sure there is an active camera, otherwise the octree cannot
	//be rendered.
	if (!m_pCamera)
		return X_ERR_OPERATION_FAILED;

	//Start rendering from the root node
	X_CLEAR_BIT_ARRAY(m_polyBits,m_numTotalPolys)
	renderTree(0);
	renderTree2();

	m_numFrustumTests = 0;
	m_numCollisionTests = 0;
	m_numIntersects = 0;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XOctree::renderTree(int index)
{	
	XOctreeNode *node = &m_masterNodeList[index];

	//If node is empty and has no children, bail (ie. just empty space)
	if(node->NumPolygons == 0 && node->Children[0] == XO_NULL_NODE)
		return;

	//Test the node against the view m_frustum
	X_INTERSECTION_TYPE rv;
	XSphere sphere;
	
	if(m_sphereCull)
	{
		sphere.center = node->Center;
		sphere.radius = node->SphereRadius;
		rv = m_pCamera->sphereInFrustum(sphere);
	}
	else
		rv = m_pCamera->cubeInFrustum(node->Center, node->Size);
	
	++m_numFrustumTests;

	switch(rv)
	{
	case X_IT_OUTSIDE: 
		return;
	case X_IT_INSIDE: 
		renderAllChildren(index);
		return;
	case X_IT_INTERSECTS: 
		if(node->Children[0] != XO_NULL_NODE)
			for(int i = 0; i < 8; i++)
				renderTree(node->Children[i]);
		else
			for (int i = 0;i < node->NumPolygons;i++)
				X_SET_BIT(m_polyBits,node->Polygons[i])							
		break;
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::renderTree2()
{
	//First things first. Reset the appropriate info in our render
	//blocks in preparation for the next frame.
	for (XU32 i = 0;i < m_rblSize;i++)
	{
		m_renderBlockList[i].numPolys = 0;
		m_renderBlockList[i].nextInsertionPoint = 0;
		m_renderBlockList[i].inUse = false;
	}

	//We need to run through the visible polys and determine
	//the size (if any) of each render block's index buffer
	for (XU32 i = 0;i < m_numTotalPolys;i++)
		if (X_IS_BIT_SET(m_polyBits,i))//m_polyBits->isBitSet(i))
			m_renderBlockList[m_polyMatIndexArray[i]].numPolys++;

	//Now that we've got the number of polys that are to be put
	//in each render block, create the index buffers in each
	//render block and also lock the index buffer and store the
	//pointer to the data.
	for (XU32 i = 0;i < m_rblSize;i++)
	{
		if (m_renderBlockList[i].numPolys <= 0)
			continue;

		XVP::GraphicsMgr.createIndexBuffer(m_renderBlockList[i].numPolys*3,true,
									   m_renderBlockList[i].ibHandle);

		m_renderBlockList[i].ibPtr = XVP::GraphicsMgr.lockIndexBuffer(m_renderBlockList[i].ibHandle,X_LT_DISCARD);
	}
	

	//We also need to lock the main index buffer.
	WORD* pMainIndices = NULL;
	pMainIndices = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle,X_LT_READ_ONLY);
	
	//We now have all index buffers locked so we can carve up the visible
	//polys into their respective render blocks based on their material ID.
	for (XU32 i = 0;i < m_numTotalPolys;i++)
	{
		if (X_IS_BIT_SET(m_polyBits,i))
		{
			m_renderBlockList[m_polyMatIndexArray[i]].ibPtr[m_renderBlockList[m_polyMatIndexArray[i]].nextInsertionPoint] 
							= pMainIndices[i * 3];
			m_renderBlockList[m_polyMatIndexArray[i]].ibPtr[m_renderBlockList[m_polyMatIndexArray[i]].nextInsertionPoint+1]
							= pMainIndices[(i * 3) + 1];
			m_renderBlockList[m_polyMatIndexArray[i]].ibPtr[m_renderBlockList[m_polyMatIndexArray[i]].nextInsertionPoint+2] 
							= pMainIndices[(i * 3) + 2];

			m_renderBlockList[m_polyMatIndexArray[i]].nextInsertionPoint += 3;

			//Set the in use flag.
			m_renderBlockList[m_polyMatIndexArray[i]].inUse = true;
		}
	}

	//Now that all polys have been put into their respective render blocks and
	//thus been associated with there respective materials, we can release
	//all index buffer pointers.
	for (XU32 i = 0;i < m_rblSize;i++) 
		if (m_renderBlockList[i].inUse)
			XVP::GraphicsMgr.unlockIndexBuffer(m_renderBlockList[i].ibHandle);

	//Don't forget to release main index buffer pointer.
	XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);

	//Now set the world matrix to the identity matrix and render!
	//XVP::GraphicsMgr.setWorldMatrix(m_worldTransform);
	XVP::GraphicsMgr.removeVertexShader();
	XVP::GraphicsMgr.removePixelShader();
	for (XU32 i = 0;i < m_rblSize;i++)
	{
		if (m_renderBlockList[i].inUse)
		{
			//Set the render state in the render block.
			XVP::GraphicsMgr.applyMaterial(m_renderBlockList[i].matHandle);

			//Finally...render polys in this render block.
			XVP::GraphicsMgr.renderVertexBuffer(m_vbHandle,m_renderBlockList[i].ibHandle);

			//Delete the index buffer.
			XVP::GraphicsMgr.destroyIndexBuffer(m_renderBlockList[i].ibHandle);		
		}
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::renderAllChildren(int index)
{
	XOctreeNode *node = &m_masterNodeList[index];
	if(node->Children[0] != XO_NULL_NODE)
	{
		for(int i = 0; i < 8; i++)
			renderAllChildren(node->Children[i]);
	} 
	else
	{
		for (int i = 0;i < node->NumPolygons;i++)
			X_SET_BIT(m_polyBits,node->Polygons[i])
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::renderWireframeTree(int index,XColor color)
{
	XOctreeNode *node = &m_masterNodeList[index];
	if(node->Children[0] == XO_NULL_NODE)
	{
		if(node->status == X_IT_INSIDE)
		{
			color = XColor(0,1,0,0);
			node->status = X_IT_OUTSIDE;
		}

		XModelVertex v[24];
		memset(v,0,24 * sizeof(XModelVertex));

		for(int i = 0; i < 24; i++)
			v[i].diffuse = color.getARGB32Bit();

		v[0].pos.x = node->Min.x;	v[0].pos.y = node->Min.y;	v[0].pos.z = node->Min.z;
		v[1].pos.x = node->Max.x;	v[1].pos.y = node->Min.y;	v[1].pos.z = node->Min.z;
		v[2].pos.x = node->Min.x;	v[2].pos.y = node->Min.y;	v[2].pos.z = node->Min.z;
		v[3].pos.x = node->Min.x;	v[3].pos.y = node->Min.y;	v[3].pos.z = node->Max.z;
		v[4].pos.x = node->Min.x;	v[4].pos.y = node->Min.y;	v[4].pos.z = node->Min.z;
		v[5].pos.x = node->Min.x;	v[5].pos.y = node->Max.y;	v[5].pos.z = node->Min.z;
		v[6].pos.x = node->Max.x;	v[6].pos.y = node->Min.y;	v[6].pos.z = node->Min.z;
		v[7].pos.x = node->Max.x;	v[7].pos.y = node->Min.y;	v[7].pos.z = node->Max.z;
		v[8].pos.x = node->Max.x;	v[8].pos.y = node->Min.y;	v[8].pos.z = node->Min.z;
		v[9].pos.x = node->Max.x;	v[9].pos.y = node->Max.y;	v[9].pos.z = node->Min.z;
		v[10].pos.x = node->Min.x;	v[10].pos.y = node->Min.y;	v[10].pos.z = node->Max.z;
		v[11].pos.x = node->Max.x;	v[11].pos.y = node->Min.y;	v[11].pos.z = node->Max.z;
		v[12].pos.x = node->Min.x;	v[12].pos.y = node->Min.y;	v[12].pos.z = node->Max.z;
		v[13].pos.x = node->Min.x;	v[13].pos.y = node->Max.y;	v[13].pos.z = node->Max.z;
		v[14].pos.x = node->Max.x;	v[14].pos.y = node->Min.y;	v[14].pos.z = node->Max.z;
		v[15].pos.x = node->Max.x;	v[15].pos.y = node->Max.y;	v[15].pos.z = node->Max.z;
		v[16].pos.x = node->Min.x;	v[16].pos.y = node->Max.y;	v[16].pos.z = node->Min.z;
		v[17].pos.x = node->Min.x;	v[17].pos.y = node->Max.y;	v[17].pos.z = node->Max.z;
		v[18].pos.x = node->Min.x;	v[18].pos.y = node->Max.y;	v[18].pos.z = node->Min.z;
		v[19].pos.x = node->Max.x;	v[19].pos.y = node->Max.y;	v[19].pos.z = node->Min.z;
		v[20].pos.x = node->Max.x;	v[20].pos.y = node->Max.y;	v[20].pos.z = node->Min.z;
		v[21].pos.x = node->Max.x;	v[21].pos.y = node->Max.y;	v[21].pos.z = node->Max.z;
		v[22].pos.x = node->Max.x;	v[22].pos.y = node->Max.y;	v[22].pos.z = node->Max.z;
		v[23].pos.x = node->Min.x;	v[23].pos.y = node->Max.y;	v[23].pos.z = node->Max.z;
		
		
		//XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_LINE_LIST,X_VT_MODEL,12,&v,sizeof(XModelVertex));
	}
	else
	{
		renderWireframeTree(node->Children[0],color);
		renderWireframeTree(node->Children[1],color);
		renderWireframeTree(node->Children[2],color);
		renderWireframeTree(node->Children[3],color);
		renderWireframeTree(node->Children[4],color);
		renderWireframeTree(node->Children[5],color);
		renderWireframeTree(node->Children[6],color);
		renderWireframeTree(node->Children[7],color);
   }
}



//*****************************************************************************
//*****************************************************************************
void XOctree::markCurrentNode(XVector3D position, XBOOL mark_neighbors)
{
	int id = 0;
	getCurrentNode(position,0,&id);
	XOctreeNode *node = &m_masterNodeList[id];

	X_SET_BIT(m_nodeBits,id)
	for(int i = 0; i < 6; i++)
	{
		int nid = node->Neighbors[i];
		if(nid != XO_NULL_NODE)
			X_SET_BIT(m_nodeBits,nid)
	}
}

/*void XOctree::renderMarkedNodes(int index, eIDevice *device, DWORD color){
	for(int i = 0; i < m_numNodes; i++)
	{
		XOctreeNode *node = &m_masterNodeList[i];
		if(m_nodeBits->IsBitSet(node->Node_ID))
		{
			device->DrawAABBFast(node->Min,node->Max,color);
		}
	}
}
*/

/*void XOctree::renderCurrentNode(XVector3D position, eIDevice *device, DWORD color)
{
	database->VisiblePolygons->Clear();

	int id = 0;
	GetCurrentNode(position,0,&id);
	XOctreeNode *node = &m_masterNodeList[id];
	device->DrawAABBFast(node->Min,node->Max,color);
	TextMngr->PrintMessage("Node %d: %d Polygons",id,node->NumPolygons);

	for(int i = 0; i < node->NumPolygons; i++)
		database->VisiblePolygons->SetBit(node->Polygons[i]);
}
*/



//*****************************************************************************
//*****************************************************************************
void XOctree::getCurrentNode(XVector3D position, int index, int *node)
{
	if(XIPointInAABB(position, m_masterNodeList[index].Min, m_masterNodeList[index].Max))
	{
		*node = index;
		if(m_masterNodeList[index].Children[0] != XO_NULL_NODE){
			for(int i = 0; i < 8; i++)
				getCurrentNode(position,m_masterNodeList[index].Children[i],node);
		}
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::collideSphere(XVector3D center, XF32 radius)
{
	//Clear the appropriate information for the upcoming collision tests
	X_CLEAR_BIT_ARRAY(m_collidablePolyBits,m_numTotalPolys);
	m_numCollidablePolys = 0;
	m_collidablePolyList.clear();

	//Grab the pointers to the index and vertex buffers and store
	//them in the working copies of the member pointers.  The actual
	//sphereCollide routine will use these to get the polygon data.
	m_vbPtr = (XModelVertex*)XVP::GraphicsMgr.lockVertexBuffer(m_vbHandle,X_LT_WRITE_ONLY);
	m_ibPtr = XVP::GraphicsMgr.lockIndexBuffer(m_ibHandle,X_LT_READ_ONLY);

	//Now call the actual recursive collision routine to get a 
	//list of polygons that could potentially collide with the sphere.
	sphereCollide(0, center, radius);

	//Now that the collision routine is done, release the temporary 
	//vertex and index buffers.
	XVP::GraphicsMgr.unlockVertexBuffer(m_vbHandle);
	XVP::GraphicsMgr.unlockIndexBuffer(m_ibHandle);
}



//*****************************************************************************
//*****************************************************************************
void XOctree::sphereCollide(int index, XVector3D ocenter, XF32 oradius)
{
	XOctreeNode *node = &m_masterNodeList[index];

	//If node is empty and has no children, bail (ie. just empty space)
	if(node->NumPolygons == 0 && node->Children[0] == XO_NULL_NODE)
		return;

	//If the sphere doesn't intersect node, skip children
	if(XISphereAABB(ocenter,oradius,node->Min,node->Max) == false)
		return;

	//Recurse on node's children if they exist
	if(node->Children[0] != XO_NULL_NODE)
	{
		for(int i = 0; i < 8; i++)
			sphereCollide(node->Children[i], ocenter, oradius);
	} 
	else
	{  
		for(int i = 0; i < node->NumPolygons; i++)
		{
			if (X_IS_BIT_SET(m_collidablePolyBits,node->Polygons[i]))
				continue;
			

			//Now actually place the polygon data into the potential collider
			//list
			/*p.vert1 = m_vbPtr[m_ibPtr[(node->Polygons[i]*3)]].pos;
			p.vert2 = m_vbPtr[m_ibPtr[(node->Polygons[i]*3)+1]].pos;
			p.vert3 = m_vbPtr[m_ibPtr[(node->Polygons[i]*3)+2]].pos;
			m_collidablePolyList.push_back(p);	
			*/
			p_polys[(m_numCollidablePolys)].vert1 = m_vbPtr[m_ibPtr[(node->Polygons[i]*3)]].pos;
			p_polys[(m_numCollidablePolys)].vert2 = m_vbPtr[m_ibPtr[(node->Polygons[i]*3)+1]].pos;
			p_polys[(m_numCollidablePolys)].vert3 = m_vbPtr[m_ibPtr[(node->Polygons[i]*3)+2]].pos;

			X_SET_BIT(m_collidablePolyBits,node->Polygons[i])
			m_numCollidablePolys++;

		}
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::collideAABB(XVector3D min, XVector3D max)
{
	X_CLEAR_BIT_ARRAY(m_collidablePolyBits,m_numTotalPolys);
	m_numCollidablePolys = 0;
	AABBCollide(0, min, max);
}



//*****************************************************************************
//*****************************************************************************
void XOctree::AABBCollide(int index, XVector3D min, XVector3D max)
{
	XOctreeNode *node = &m_masterNodeList[index];

	//if node is empty and has no children, bail (ie. just empty space)
	if(node->NumPolygons == 0 && node->Children[0] == XO_NULL_NODE)
		return;

	//if the box doesn't intersect node, skip children
	if(XIAABBAABB(min,max,node->Min,node->Max) == false)
		return;

	//recurse on node's children if they exist
	if(node->Children[0] != XO_NULL_NODE)
	{
		for(int i = 0; i < 8; i++)
			AABBCollide(node->Children[i], min, max);
	} 
	else
	{  
		for(int i = 0; i < node->NumPolygons; i++)
		{
			//was polygon already tested?
			if (X_IS_BIT_SET(m_collidablePolyBits,node->Polygons[i]))
				continue;


	/*
			//test polygon AABB for immediate rejection
			
			/*LPPOLYGON polygon = &database->Polygons[node->Polygons[i]];
			
			if (polygon->max.x1 < min.x1) continue;
			if (polygon->max.x2 < min.x2) continue;
			if (polygon->max.x3 < min.x3) continue;
			if (polygon->min.x1 > max.x1) continue;
			if (polygon->min.x2 > max.x2) continue;
			if (polygon->min.x3 > max.x3) continue;
			
			
			XBoundingBox bb(			
	*/		

			//valid potentially collidable polygon found
			X_SET_BIT(m_collidablePolyBits,node->Polygons[i])
			m_numCollidablePolys++;
			/*database->CollidablePolygons->SetBit(node->Polygons[i]);*/
		}
	}
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOctree::saveOctreeToFile(XS8* filename)
{
	FILE *fp = fopen(filename,"wb");

	//Save threshold value
	fwrite(&m_maxPolysPerNode,sizeof(int),1,fp);

	//Save number nodes in tree
	fwrite(&m_numNodes,sizeof(int),1,fp);

	//Save master node list
	for(int i = 0; i < m_numNodes; i++)
	{
		XOctreeNode *node = &m_masterNodeList[i];
		fwrite(&node->Node_ID,sizeof(int),1,fp);
		fwrite(&node->Children,sizeof(signed int),8,fp);
		fwrite(&node->Neighbors,sizeof(signed int),6,fp);
		fwrite(&node->Max,sizeof(XVector3D),1,fp);
		fwrite(&node->Min,sizeof(XVector3D),1,fp);
		fwrite(&node->Center,sizeof(XVector3D),1,fp);
		fwrite(&node->SphereRadius,sizeof(XF32),1,fp);
		fwrite(&node->NumPolygons,sizeof(int),1,fp);
		for(int j = 0; j < node->NumPolygons; j++)
		{
			fwrite(&node->Polygons[j],sizeof(int),1,fp);
		}
	}

	fclose(fp);

	return true;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XOctree::loadOctreeFromFile(XS8* filename)
{
	FILE *fp = fopen(filename,"rb");
	int tmp = 0;

	//Read threshold value
	fread(&m_maxPolysPerNode,sizeof(int),1,fp);

	//Read number nodes in tree
	fread(&m_numNodes,sizeof(int),1,fp);

	//Read and create master node list
	for(int i = 0; i < m_numNodes; i++)
	{
		XOctreeNode node;
		fread(&node.Node_ID,sizeof(int),1,fp);
		fread(&node.Children,sizeof(signed int),8,fp);
		fread(&node.Neighbors,sizeof(signed int),6,fp);
		fread(&node.Max,sizeof(XVector3D),1,fp);
		fread(&node.Min,sizeof(XVector3D),1,fp);
		fread(&node.Center,sizeof(XVector3D),1,fp);
		fread(&node.SphereRadius,sizeof(XF32),1,fp);
		fread(&node.NumPolygons,sizeof(int),1,fp);

		for(int j = 0; j < node.NumPolygons; j++)
		{
			int tmp = 0;
			fread(&tmp,sizeof(int),1,fp);
			node.Polygons.push_back(tmp);
		}
		m_masterNodeList.push_back(node);
	}

	fclose(fp);

	return true;
}

/*XBOOL XOctree::lineOfSight(XVector3D start, XVector3D end,
						   XBOOL ignore_transparent, int pindex,
						   int *tri_hit, XVector3D *int_point)
{
	int tindex = 0;
	int numtests = 0;
	XBOOL rv = false;

	//reset number colliable triangles
	//database->CollidablePolygons->Clear();
	X_CLEAR_BIT_ARRAY(m_collidablePolyBits,m_numTotalPolys);

	//build the colliders list
	RayNodeIntersection(start, end);

	//if there are no potential colliders, just return true (ie. clear path)
	XBOOL potentialColliders = false;
	for (XU32 i = 0;i < m_numTotalPolys;i++)
	    if (X_IS_BIT_SET(m_collidablePolyBits,m_numTotalPolys))
		{
			potentialColliders = true;
			break;
		}
	if (!potentialColliders)
		return true;
	//if(database->CollidablePolygons->AllBitsFalse())
		//return true;

	//otherwise we have to check for blockers
	for(int i = 0; i < database->num_polygons; i++)
	{
		//if(!database->CollidablePolygons->IsBitSet(i) || pindex == i)
		if (!X_IS_BIT_SET(m_collidablePolyBits,i) || (pindex == i))
			continue;

		//get the polygon
		eIDBPolygon *polygon = &database->Polygons[i];

		//ignore transparent polygons as appropriate
		//if(polygon->is_transparent && ignore_transparent)
			//continue;

		//check adjacent planar tris if necessary
		XBOOL ignore = false;
		if(pindex != -100){
			LPADJACENCY a = &database->Adjacencies[pindex];
			for(int ii = 0; ii < 3; ii++)
			{
				if(a->polys[ii] == i){
					if(a->is_planar->IsBitSet(ii))
					{
						ignore = true;
						break;
					}
				}
			}
		}
		if(ignore) continue;

		//intersect the line with the plane of the polygon
		XVector3D intersectPoint;
		
		rv = XIntersectLinePlane(start,
								end,
								(XVector3D)database->Vertices[polygon->Vertices[0]].Properties,
								polygon->PolygonNormal,
								&intersectPoint);
		if(rv == false)
			continue;

		//check if intersect point is within polygon
		XVector3D verts[3];
		verts[0] = (XVector3D)database->Vertices[polygon->Vertices[0]].Properties;
		verts[1] = (XVector3D)database->Vertices[polygon->Vertices[1]].Properties;
		verts[2] = (XVector3D)database->Vertices[polygon->Vertices[2]].Properties;
		rv = XIsPointInTriangle(intersectPoint, verts);
		if(rv == false)
			continue;
		else{
			if(tri_hit)
				*tri_hit = i;

			if(int_point)
				*int_point = intersectPoint;

			return false;
		}
	}
	return true;
}
*/



//*****************************************************************************
//*****************************************************************************
void XOctree::traceRay(XRay3D ray)
{
	X_CLEAR_BIT_ARRAY(m_nodeBits,m_numNodes);
	intersectRayNode(0,ray.origin,ray.end,ray.normal);
}



//*****************************************************************************
//*****************************************************************************
void XOctree::rayNodeIntersection(XVector3D startpos, XVector3D endpos)
{
	//Find all aabb's that intersect this ray
	//then check their mesh spheres and add the polys
	//to the mesh collision lists for further testing
	XVector3D dir = endpos - startpos;
	dir.normalize();

	intersectRayNode(0,startpos,endpos,dir);
}



//*****************************************************************************
//*****************************************************************************
void XOctree::intersectRayNode(int index, XVector3D start, XVector3D end, XVector3D dir)
{	
	//Get the node
	XOctreeNode *node = &m_masterNodeList[index];

	//If node is empty and has no children, don't bother checking it
	if(node->NumPolygons == 0 && node->Children[0] == XO_NULL_NODE)
		return;

	//Check for intersection
	XVector3D intpoint;
	XF32 t;

	XBOOL rv = XIRayAABB(start, dir,node->Min, node->Max,intpoint,t);

	//If intersected, recurse on children or update flags
	if(rv == true)
	{
		X_SET_BIT(m_nodeBits,index);
		if(node->Children[0] != XO_NULL_NODE)
		{
			for(int i = 0; i < 8; i++)
				intersectRayNode(node->Children[i],start,end,dir);
		}
		else
		{
			for(int i = 0; i < node->NumPolygons; i++)
				X_SET_BIT(m_collidablePolyBits,node->Polygons[i]);
		}
	}
}



//*****************************************************************************
//*****************************************************************************
void XOctree::printTree(XS8* pFilename)
{
	FILE *fp;
	if((fp = fopen(pFilename,"w")) == NULL){
		return;
	}
	fprintf(fp,"\nOCTREE DATA");
	fprintf(fp,"------------------");
	fprintf(fp,"\nTotal Num Nodes :%d\n", m_numNodes);
	fprintf(fp,"\nTotal Num Polys in Octree :%d\n", m_numTotalPolys);
	fprintf(fp,"\nTotal Num Polys in Octants :%d\n",m_numTotalPolysInOctants);
	for(int i = 0; i < m_numNodes; i++)
	{
		XOctreeNode *node = &m_masterNodeList[i];
		fprintf(fp,"\n\nOctree node %d info\n", node->Node_ID);
		fprintf(fp,"----------------");
		fprintf(fp,"\nMin x:%f, y:%f, z:%f", node->Min.x, node->Min.y, node->Min.z);
		fprintf(fp,"\nMax x:%f, y:%f, z:%f", node->Max.x, node->Max.y, node->Max.z);
		fprintf(fp,"\nCenter x:%f, y:%f, z:%f\n", node->Center.x, node->Center.y, node->Center.z);
		fprintf(fp,"\nNum Node Polys %d\n",  node->NumPolygons);

		fprintf(fp,"\nChildren: ");
		for(int j = 0; j < 8; j++)
			fprintf(fp,"%d ", node->Children[j]);

		fprintf(fp,"\nNeighbors: ");
		for(int k = 0; k < 6; k++)
			fprintf(fp,"%d ", node->Neighbors[k]);

		fprintf(fp,"\nPolys: ");
		for (XU32 l = 0; l < node->Polygons.size(); l++)
			fprintf(fp,"%i ",node->Polygons[l]);
	}
	fclose(fp);
}



//*****************************************************************************
//*****************************************************************************
XMaterial* XOctree::getRenderBlockRSPtr(XU32 texStage,XS8* textureFilename)
{
	//Validate texture stage number
	if ((texStage < 0) || (texStage > XVP::GraphicsMgr.getMaxTextureStages()-1))
		return NULL;

	//Make sure we have a valid filename
	if (!textureFilename)
		return NULL;

	XS8* texName;

	//Run through the render block list and return the first render
	//block who has a matching texture filename
	XMaterial* rsPtr;
	for (XU32 i = 0;i < m_renderBlockList.size();i++)
	{
		rsPtr = XVP::GraphicsMgr.getMaterialInterface(m_renderBlockList[i].matHandle);
		if (rsPtr)
		{
			
			texName = XVP::GraphicsMgr.getTextureFilename(rsPtr->getTextureHandle(texStage));		
			if (texName)
				if (strcmp(texName,textureFilename) == 0)
					return rsPtr;
		}
	}

	//Could not find a match.
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
XMaterial* XOctree::getRenderBlockRSPtr(XU32 renderBlockID)
{
	//Make sure that we have a valid render block ID
	if (renderBlockID >= m_renderBlockList.size())
		return NULL;

	return XVP::GraphicsMgr.getMaterialInterface(m_renderBlockList[renderBlockID].matHandle);
}



//*****************************************************************************
//*****************************************************************************
XStatus XOctree::setCamera(XCamera* pCamera)
{
	m_pCamera = pCamera;
	return X_OK;
}

	

//*****************************************************************************
//*****************************************************************************
XCamera* XOctree::getCamera()
{
	return m_pCamera;
}