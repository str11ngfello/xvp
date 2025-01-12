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
//	Module Name:	XSceneGraph.h
//	Author:			Lynn Duke
//	Creation Date:	6-8-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SCENE_GRAPH_H__
#define __X_SCENE_GRAPH_H__





//*****************************************************************************
//! A container object for scene nodes in a scene graph.
/**
The XSceneGraphNode object is used to hold hierarchical information about a
scene node while it belongs to a scene graph.  For example, it contains a pointer
to the parent scene node and a list of the node's children.  Normally, you don't
create an XSceneGraphNode object manually, but instead use the supplied methods
of the XSceneGraph object to build scene graphs.

See XSceneGraph for more details.
*/
//*****************************************************************************
class XSceneGraphNode
{
public:
	XS8* m_pName;
 	XSceneNode* m_pSceneNode;
	XSceneGraphNode* m_pParentNode;
	XMemBlockList<XSceneGraphNode*> m_children;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSceneGraphNode();
	~XSceneGraphNode();
	XStatus init(XSceneNode* pNode,XS8* pNodeName,XSceneGraphNode* pParentNode);
	XStatus destroy();

};



//*****************************************************************************
//! A collection of scene nodes in the form of a tree.
/**
The XSceneGraph object is a hierarchical collection of XSceneGraphNode
objects stored in a tree data structure.  The tree structure adheres to the
following principles:

1. Only one node may be the root of the tree at any given time. <br>
2. Any node can have children. <br>
3. There is no limit on how many children a node may have.<br>

The XSceneGraphNode object is nothing more than a container object used
to store a pointer to an XSceneNode object.  The XSceneGraph object supports
operations to create, destroy and manipulate nodes in the tree.  

Scene graphs are useful in any situation where you need to store objects in a
hierarchical nature where parent scene nodes affect their children.  Specifically,
the XSceneGraph propogates 3D transform information down the tree in a 
depth-first manner in such a way that children will inherit their parents
movements in 3D space.  Some common uses for the XSceneGraph object include
storing bones for skin meshes, model objects for rigid bodies, etc...

See below for supported operations.
*/
//*****************************************************************************
class XSceneGraph
{
private:
	XSceneGraphNode*			m_pRoot;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	void destroyNodeAndChildren(XSceneGraphNode* pCurrentNode);
	void updateNodeAndChildren(XSceneGraphNode* pCurrentNode,XF32 dt,
							   XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	void traverseNodeAndChildren(XSceneGraphNode* pCurrentNode,XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData);
	//void renderNodeAndChildren(XSceneGraphNode* pCurrentNode,XF32 dt,XScene* pScene);
	//void renderNodeAndChildren(XSceneGraphNode* pCurrentNode);
    XSceneGraphNode* searchAndReturnNode(XSceneGraphNode* pCurrentNode,XS8* pNodeName);
	XSceneGraphNode* searchAndReturnNode(XSceneGraphNode* pCurrentNode,XSceneNode* pNode);
	XSceneGraphNode* searchAndCreateChildNode(XSceneNode* pNode,XS8* pParentNodeName,
										XS8* pNewNodeName);
	void searchAndDumpScene(XSceneGraphNode* pCurrentNode,ofstream& outputFile,XU32 index);
	void searchAndCountNodes(XU32& numNodes,XSceneGraphNode* pCurrentNode);
	void searchAndGetNodes(XMemBlockList<XSceneGraphNode*>& nodes,XSceneGraphNode* pCurrentNode);
	void walkNodeAndChildren(XSceneGraphNode* pCurrentNode,
							XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData);
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSceneGraph();
	~XSceneGraph();
	
	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus destroy();
	XSceneGraphNode* createNode(XSceneNode* pNode,XS8* pParentNodeName,
								   XS8* pNewNodeName);
	XSceneGraphNode* createChildNode(XSceneNode* pNode,
									XSceneGraphNode* pParentNode,
									XS8* pNewNodeName);
	XSceneGraphNode* getNode(XS8* pNodeName);
	XSceneGraphNode* getRootNode();
	XStatus destroyNode(XSceneNode* pNode);
	XStatus destroyNode(XS8* pNodeName);
	void destroyAllNodes();
	void dumpScene(ofstream& outputFile,XU32 index = 0);
	void getNodes(XS8* pNodeName,XMemBlockList<XSceneGraphNode*>& nodes);
	void walkNodes(XS8* pNodeName,XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
					XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),
					void* pUserData);
	XU32 getNumNodes();
	void update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	void traverseDepthFirst(XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData);
	//void render();
};

#endif