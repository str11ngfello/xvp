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
//	Module Name:	XSceneGraph.cpp
//	Author:			Lynn Duke
//	Creation Date:	6-8-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
/**
Constructs a scene graph node by initializing the scene graph node
properties to the following default values:<br><br>

<ul>
<li> Name = NULL </li>
<li> Scene Node Pointer = NULL </li>
<li> Parent Node Pointer = NULL </li>
</ul>
*/
//*****************************************************************************
XSceneGraphNode::XSceneGraphNode()
{
	m_pName = NULL;
	m_pSceneNode = NULL;
	m_pParentNode = NULL;
}



//*****************************************************************************
/**
Destructor for the scene graph node.  
*/
//*****************************************************************************
XSceneGraphNode::~XSceneGraphNode()
{
}



//*****************************************************************************
/**
Constructs a scene graph node by using the following parameters.

@param pNode	The scene node to store in the graph node.
@param pNodeName	A name for the node.
@param pParentNode	The parent graph node.
*/
//*****************************************************************************
XStatus XSceneGraphNode::init(XSceneNode* pNode,XS8* pNodeName,XSceneGraphNode* pParentNode)
{
	if (pNodeName)
	{
		m_pName = XOSMgr::createString(pNodeName);
		//m_pName = new XS8[strlen(pNodeName)+1];
		strcpy(m_pName,pNodeName);
	}
	else
		m_pName = NULL;
	m_pParentNode = pParentNode;
	m_pSceneNode = pNode;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneGraphNode::destroy()
{
	XOSMgr::destroyString(&m_pName);
	m_children.clear();
	return X_OK;
}



//*****************************************************************************
/**
Constructor for the scene graph.  
*/
//*****************************************************************************
XSceneGraph::XSceneGraph()
{
	m_pRoot = NULL;
}



//*****************************************************************************
/**
Destructor for the scene graph.  This function will destroy all remaining
nodes in the scene graph.
*/
//*****************************************************************************
XSceneGraph::~XSceneGraph()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XSceneGraph::destroy()
{
	//Destroy all.
	destroyAllNodes();

	return X_OK;
}



//*****************************************************************************
/**
Creates a new scene graph node in the scene graph to hold a scene node.

@param pNode A pointer to the scene node to store in the new graph node.
@param pParentNodeName	The name of the graph node to attach the new graph node
						under. If this parameter is NULL, the new node will be
						created as the root of the scene graph tree.  NULL is
						only valid if there are no nodes in the graph, therefore,
						NULL is typically used to create the first node in the tree.
						If the graph already has a root node, passing NULL for
						this parameter will cause the function to fail.
@param pNewNodeName The name of the newly created node.  This parameter cannot
					be NULL.
@return A pointer to the newly created graph node.  If this function fails,
		the return value is NULL.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::createNode(XSceneNode* pNode,
										 XS8* pParentNodeName,
										 XS8* pNewNodeName)
{
	//If the pParentNodeName is NULL then the user wants this node to
	//be the root of the tree.  There can only be one root, therefore,
	//we cannot recieve another NULL parent for the duration of this 
	//tree.  If we do, we will return NULL specifying that the root
	//create was not sucessful.  However, if pParentNodeName is not NULL,
	//then we need to walk through the tree and find the node with this 
	//name and add the new node as a child.
	if ((pParentNodeName == NULL) || (strcmp(pParentNodeName,"") == 0))
	{
		//Check to make sure that a root does not already exist.
		X_ASSERTM(m_pRoot == NULL,"The tree already has a root node.  Only one root node is allowed.")

		XNew(m_pRoot,XSceneGraphNode,XOSMgr::getDefaultHeap());
		m_pRoot->init(pNode,pNewNodeName,NULL);
		return m_pRoot;
		
	}
	else if (m_pRoot != NULL)
		return searchAndCreateChildNode(pNode,pParentNodeName,pNewNodeName);		
	else 
		return NULL;	
}



//*****************************************************************************
/**
Creates a new scene graph node in the scene graph to hold a scene node.

@param pNode A pointer to the scene node to store in the new graph node.
@param pParentNodeName	The name of the graph node to attach the new graph node
						under. This parameter cannot be NULL.
@param pNewNodeName The name of the newly created graph node.  This parameter
					cannot be NULL.
@return A pointer to the newly created graph node.  If this function fails,
		the return value is NULL.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::searchAndCreateChildNode(XSceneNode* pNode,
											      XS8* pParentNodeName,
											      XS8* pNewNodeName)
{
	//Make sure names are valid
	if (!pParentNodeName)
		return NULL;

	//Find the parent node first.
	XSceneGraphNode* parentNode = searchAndReturnNode(m_pRoot,pParentNodeName);

	//If we found the parent node, then add a new child
	if (parentNode)
	{
		XSceneGraphNode* newNode = NULL;
		XNew(newNode,XSceneGraphNode,XOSMgr::getDefaultHeap());
		newNode->init(pNode,pNewNodeName,parentNode);		
		parentNode->m_children.insertBack(newNode);
		return newNode;
	}
	else
	{
		return NULL;
	}
}



//*****************************************************************************
/**
Creates a new scene graph node in the scene graph to hold a scene node.

@param pNode A pointer to the scene node to store in the new graph node.
@param pParentNodeName	The name of the graph node to attach the new graph node
						under. This parameter cannot be NULL.
@param pNewNodeName The name of the newly created graph node.  This parameter
					cannot be NULL.
@return A pointer to the newly created graph node.  If this function fails,
		the return value is NULL.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::createChildNode(XSceneNode* pNode,
											  XSceneGraphNode* pParentNode,
											  XS8* pNewNodeName)
{
	//Make sure names are valid
	if (!pNewNodeName)
		return NULL;

	//If we found the parent node, then add a new child
	if (pParentNode)
	{
		XSceneGraphNode* newNode = NULL;
		XNew(newNode,XSceneGraphNode,XOSMgr::getDefaultHeap());
		newNode->init(pNode,pNewNodeName,pParentNode);		
		pParentNode->m_children.insertBack(newNode);
		return newNode;
	}
	else
	{
		return NULL;
	}
}



//*****************************************************************************
/**
Destroys a scene graph node and its associated resources by searching for 
a particular scene node in the graph.  This function will only remove one
graph node at a time.  Therefore, if the graph contains multiple graph nodes
that contain the same scene node, only one graph node will be destroyed
per function call.  The search is performed in a depth-first traversal of
the graph.

@remark	When the graph node is destroyed, all children of the node are
		destroyed as well.
@param pNode	The scene node to destroy.
@return A return code of type XStatus.
*/
//*****************************************************************************	
XStatus XSceneGraph::destroyNode(XSceneNode* pNode)
{
	//Make sure nodeName is valid
	if (!pNode)
		return X_ERR_INVALID_PARAMS;

	//Find the node by the passed in name first.
	XSceneGraphNode* nodeToDestroy = searchAndReturnNode(m_pRoot,pNode);

	//If we found it destroy it, otherwise just return
	if (!nodeToDestroy)
		return X_ERR_OPERATION_FAILED;
	
	//Get the parent pointer for later cleanup.
	XSceneGraphNode* parentPtr = nodeToDestroy->m_pParentNode;

	//Delete the node and it's children.
	destroyNodeAndChildren(nodeToDestroy);

	//Now remove the NULL reference to the old node from its parent,
	//if the parent is NULL then we assume the root and therefore the
	//entire tree has been destroyed so we mark m_pRoot NULL.
	if (parentPtr)
		parentPtr->m_children.remove(nodeToDestroy);
	else
		m_pRoot = NULL;

	return X_OK;
}

//*****************************************************************************
/**
Destroys a scene graph node and its associated resources by searching for 
a particular name in the tree.  This function will only remove one
graph node at a time.  Therefore, if the graph contains multiple graph nodes
that contain the same name, only one graph node will be destroyed
per function call.  The search is performed in a depth-first traversal of
the graph.

@remark	When the graph node is destroyed, all children of the node are
		destroyed as well.
@param pNodeName	The name of the graph node to destroy.
@return A return code of type XStatus.
*/
//*****************************************************************************	
XStatus XSceneGraph::destroyNode(XS8* pNodeName)
{
	//Make sure nodeName is valid
	if (!pNodeName)
		return X_ERR_INVALID_PARAMS;

	//Find the node by the passed in name first.
	XSceneGraphNode* nodeToDestroy = searchAndReturnNode(m_pRoot,pNodeName);

	//If we found it destroy it, otherwise just return
	if (!nodeToDestroy)
		return X_ERR_NO_SUCH_OBJECT;
	
	//Get the parent pointer for later cleanup.
	XSceneGraphNode* parentPtr = nodeToDestroy->m_pParentNode;

	//Delete the node and it's children.
	destroyNodeAndChildren(nodeToDestroy);

	//Now remove the NULL reference to the old node from its parent,
	//if the parent is NULL then we assume the root and therefore the
	//entire tree has been destroyed so we mark m_pRoot NULL.
	if (parentPtr)
		parentPtr->m_children.remove(nodeToDestroy);
	else
		m_pRoot = NULL;

	return X_OK;
}



//*****************************************************************************
/**
Destroys a graph node and its children recursively. 

@param pCurrentNode	The graph node to destroy.  All children of this graph node
					will also be destroyed.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::destroyNodeAndChildren(XSceneGraphNode* pCurrentNode)
{
	//Make sure that pCurrentNode is not null
	if (!pCurrentNode)
		return;

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		destroyNodeAndChildren(i.getValue());//pCurrentNode->m_children[pCurrentNode->m_children.size()-1]);	
		//pCurrentNode->m_children.pop_back();
		i.getNext();
	}

	pCurrentNode->destroy();
	XDelete(pCurrentNode)
}	



//*****************************************************************************
/**
Destroys all graph nodes in the scene graph and frees all associated resources.

@return None.
*/
//*****************************************************************************	
void XSceneGraph::destroyAllNodes()
{
	//Make sure that the root is not null
	if (!m_pRoot)
		return;

	destroyNode(m_pRoot->m_pSceneNode);
	m_pRoot = NULL;
}	



//*****************************************************************************
/**
Searches for and returns a specific graph node in the tree based on the name
of the graph node.

@param pCurrentNode	The graph node at which to begin a depth first search.
@param pNodeName	The name of the graph node to find.
@return A pointer to the queried graph node.  If the graph node is not found,
		NULL is returned.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::searchAndReturnNode(XSceneGraphNode* pCurrentNode,
													   XS8* pNodeName)
{
	//Make sure that we have a valid node.
	if (!pCurrentNode)
		return NULL;
	
	//Check to see if this is the node we want
	if (pCurrentNode->m_pName)
	{
		if (strcmp(pNodeName,pCurrentNode->m_pName) == 0)
			return pCurrentNode;
	}
	
	XSceneGraphNode* result = NULL;

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
					   
		result = searchAndReturnNode(i.getValue(),pNodeName);
		if (result != NULL)
			return result;
		i.getNext();
	}

	return NULL;
}



//*****************************************************************************
/**
Searches the graph nodes for a specific scene node.  Returns the container
graph node for the passed in scene node.

@param pCurrentNode	The graph node at which to begin a depth first search.
@param pNode	A pointer to the scene node contained in the graph node.  This
				parameter is used as the search key.
@return A pointer to the queried graph node.  If the graph node is not found,
		NULL is returned.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::searchAndReturnNode(XSceneGraphNode* pCurrentNode,
													   XSceneNode* pNode)
{
	//Make sure that we have a valid params.
	if (!pCurrentNode)
		return NULL;
	
	//Check to see if this is the node we want
	if (pCurrentNode->m_pSceneNode == pNode)
		return pCurrentNode;
	else
	{	
		XSceneGraphNode* result = NULL;

		XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
		while (i.isValid())
		{
			//Skip if NULL, shouldn't be, but just in case
			if (i.getValue() == NULL)
			{
				i.getNext();
				continue;
			}
					   
			result = searchAndReturnNode(i.getValue(),pNode);
			if (result != NULL)
				return result;
			i.getNext();
		}

		return NULL;
	}
}



//*****************************************************************************
/**
Updates a scene graph.  Each scene node in the graph is visited and updated
in a depth first traversal.

@param dt	The amount of time, in seconds, that has passed since the
					last update.
@param pMatriStack	A pointer to the scene graph's matrix stack.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//If the root of the tree exists
	if (m_pRoot)
		updateNodeAndChildren(m_pRoot,dt,pScene,pMatrixStack);

}



//*****************************************************************************
/**
Updates a scene graph by recursively visiting each node in the graph in a
depth-first traversal and initiating each scene node's update sequence.

@param pCurrentNode	The graph node at which to begin the update process.
@param dt	The amount of time, in seconds, that has passed since the
					last update.
@param pMatriStack	A pointer to a matrix stack.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::updateNodeAndChildren(XSceneGraphNode* pCurrentNode,
										  XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//Check for valid pointers...
	if (!pCurrentNode)
		return;

	X_DEBUG
	(
		std::string	s;
		if (pCurrentNode->m_pName)
			s += pCurrentNode->m_pName;
		s += " (";
		s += pCurrentNode->m_pSceneNode->getTypeDesc();
		s += ")";
		X_DEBUG_LOG_INDENT_INC(3)
		X_DEBUG_LOG_LN(s.c_str(),X_DLL_VERBOSE);
	)

	//if (pCurrentNode->m_pSceneNode)
	//{
		if (pCurrentNode->m_pParentNode)
			pCurrentNode->m_pSceneNode->prepForUpdate(dt,pScene,pCurrentNode->m_pParentNode->m_pSceneNode,pMatrixStack);
		else
			pCurrentNode->m_pSceneNode->prepForUpdate(dt,pScene,NULL,pMatrixStack);
	//}

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		updateNodeAndChildren(i.getValue(),dt,pScene,pMatrixStack);
		i.getNext();
	}

	X_DEBUG_LOG_INDENT_DEC(3)
	pMatrixStack->removeBack();
}



//*****************************************************************************
//*****************************************************************************
void XSceneGraph::traverseDepthFirst(XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),void* pUserData)
{
	//If the root of the tree exists
	if (m_pRoot)
		traverseNodeAndChildren(m_pRoot,onLevelDown,onLevelUp,pUserData);
}

							

//*****************************************************************************
//*****************************************************************************
void XSceneGraph::traverseNodeAndChildren(XSceneGraphNode* pCurrentNode,
						XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
						XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),
						void* pUserData)
{
	//Check for valid pointers...
	if (!pCurrentNode)
		return;

	if (onLevelDown)
	{
		if (!onLevelDown(pCurrentNode,pUserData))
			return;
	}

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		traverseNodeAndChildren(i.getValue(),onLevelDown,onLevelUp,pUserData);
		i.getNext();
	}

	if (onLevelUp)
	{
		if (!onLevelUp(pCurrentNode,pUserData))
			return;
	}
}



//*****************************************************************************
/**
Updates a scene graph.  Each scene node in the graph is visited and updated
in a depth first traversal.

@param dt	The amount of time, in seconds, that has passed since the
					last update.
@param pMatriStack	A pointer to the scene graph's matrix stack.
@return None.
*/
//*****************************************************************************	
/*void XSceneGraph::render(XF32 dt,XScene* pScene)
{
	//If the root of the tree exists
	if (m_pRoot)
		renderNodeAndChildren(m_pRoot,dt,pScene);
}*/



//*****************************************************************************
/**
Updates a scene graph by recursively visiting each node in the graph in a
depth-first traversal and initiating each scene node's update sequence.

@param pCurrentNode	The graph node at which to begin the update process.
@param dt	The amount of time, in seconds, that has passed since the
					last update.
@param pMatriStack	A pointer to a matrix stack.
@return None.
*/
//*****************************************************************************	
/*(void XSceneGraph::renderNodeAndChildren(XSceneGraphNode* pCurrentNode,XF32 dt,XScene* pScene)
{
	//Check for valid pointers...
	if (!pCurrentNode)
		return;

	pCurrentNode->m_pSceneNode->prepForRender(dt,pScene);

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		renderNodeAndChildren(i.getValue(),dt,pScene);
		i.getNext();
	}
}
*/


//*****************************************************************************
/**
Gets a graph node by name by performing a depth-first traveral of the scene
graph.

@param pNodeName	The name of the graph node to search for.  If this value is
					NULL, the root node is returned.
@return A pointer to the queried graph node.  If the node is not found in the
		graph, NULL is returned.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::getNode(XS8* pNodeName)
{
	if (pNodeName)
		return searchAndReturnNode(m_pRoot,pNodeName);
	else 
		return m_pRoot;		
}



//*****************************************************************************
/**
Dumps a textual representation of the scene graph to a text file.

@param	outputFile	An open text file ready to receive output.
@param	index	The indentation level.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::dumpScene(ofstream& outputFile,XU32 index)
{
	if (!outputFile.is_open())
		return;

	searchAndDumpScene(m_pRoot,outputFile,index);
	return;		
}



//*****************************************************************************
/**
Recursively visits each node in the graph and dumps a textual representation
of the scene graph to a text file.

@param	pCurrentNode	The graph node at which to begin the dump.
@param	outputFile	An open text file ready to receive output.
@param	index	The indentation level.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::searchAndDumpScene(XSceneGraphNode* pCurrentNode,
									   ofstream& outputFile,XU32 index)
{
	//Dump information about this node
	for (XU32 ctr = 0;ctr < index;++ctr)
		outputFile << "   ";
	if (index != 0)
		outputFile << "->";

	if (!pCurrentNode->m_pName)
	{
		outputFile << setiosflags(ios::left) << "NO_NAME" 
			       << " TYPE: " << pCurrentNode->m_pSceneNode->getTypeDesc() <<endl;
	}
	else
	{
		outputFile << setiosflags(ios::left) << pCurrentNode->m_pName 
			       << " TYPE: " << pCurrentNode->m_pSceneNode->getTypeDesc() <<endl;
	}
	++index;
	
	pCurrentNode->m_pSceneNode->dumpScene(outputFile,index);

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		searchAndDumpScene(i.getValue(),outputFile,index);
		i.getNext();
	}
	--index;
}



//*****************************************************************************
/**
Returns the root graph node in the scene graph.

@return The root of the graph.  If there are no nodes in the tree, the return
		value is NULL.
*/
//*****************************************************************************	
XSceneGraphNode* XSceneGraph::getRootNode()
{
	return m_pRoot;
}



//*****************************************************************************
/**
Gets the number of graph nodes in the scene graph.

@return The number of graph nodes in the scene graph.
*/
//*****************************************************************************	
XU32 XSceneGraph::getNumNodes()
{
	//Make sure we have a valid root.
	if (!m_pRoot)
		return 0;

	XU32 numNodes = 0;
	searchAndCountNodes(numNodes,m_pRoot);

	return numNodes;
}



//*****************************************************************************
//*****************************************************************************
void XSceneGraph::walkNodes(XS8* pNodeName,
							XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
							XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),
							void* pUserData)
{
	//If pNodeName is NULL, then begin collecting nodes from the root of 
	//the tree.  Otherwise, try to get the specified node and begin collecting
	//from there.
	if (!pNodeName)
	{
		//Make sure that we have a root
		if (m_pRoot)
			walkNodeAndChildren(m_pRoot,onLevelDown,onLevelUp,pUserData);
	}
	else
	{
		XSceneGraphNode* pNode = getNode(pNodeName);
		if (pNode)
			walkNodeAndChildren(pNode,onLevelDown,onLevelUp,pUserData);
	}
	//if (m_pRoot)
	//	walkNodeAndChildren(m_pRoot,onLevelDown,onLevelUp,pUserData);
}

	

//*****************************************************************************
//*****************************************************************************
void XSceneGraph::walkNodeAndChildren(XSceneGraphNode* pCurrentNode,
						XSceneGraphNode* (*onLevelDown)(XSceneGraphNode* pNode,void* pUserData),
						XSceneGraphNode* (*onLevelUp)(XSceneGraphNode* pNode,void* pUserData),
						void* pUserData)
{
	//Check for valid pointers...
	if (!pCurrentNode)
		return;

	if (onLevelDown)
	{
		if (!onLevelDown(pCurrentNode,pUserData))
			return;
	}

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		walkNodeAndChildren(i.getValue(),onLevelDown,onLevelUp,pUserData);
		i.getNext();
	}

	if (onLevelUp)
	{
		if (!onLevelUp(pCurrentNode,pUserData))
			return;
	}
}



//*****************************************************************************
/**
Returns a collection of nodes in the scene graph.  This function allows
you to get a subset of the scene graph in terms of nodes.

@param pNodeName The name of the graph node at which to begin collecting nodes.
@param nodes A vector where each collected node will be stored.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::getNodes(XS8* pNodeName,XMemBlockList<XSceneGraphNode*>& nodes)
{
	//If pNodeName is NULL, then begin collecting nodes from the root of 
	//the tree.  Otherwise, try to get the specified node and begin collecting
	//from there.
	if (!pNodeName)
	{
		//Make sure that we have a root
		if (m_pRoot)
			searchAndGetNodes(nodes,m_pRoot);
	}
	else
	{
		XSceneGraphNode* pNode = getNode(pNodeName);
		if (pNode)
			searchAndGetNodes(nodes,pNode);
	}
}



//*****************************************************************************
/**
Recursively collects scene nodes in the scene graph.  This function allows
you to get a subset of the scene graph in terms of scene nodes.

@param nodes A vector where each collected scene node will be stored.
@param pCurrentNode The graph node at which to begin collecting scene nodes.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::searchAndGetNodes(XMemBlockList<XSceneGraphNode*>& nodes,
										   XSceneGraphNode* pCurrentNode)
{
	//Check for valid pointers...
	if (!pCurrentNode)
		return;

	//Add this node to the list.
	nodes.insertBack(pCurrentNode);

	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		searchAndGetNodes(nodes,i.getValue());
		i.getNext();
	}
}



//*****************************************************************************
/**
Recursively counts graph nodes in the scene graph.  

@param numNodes A variable to increment each time a graph node is found.
@param pCurrentNode The graph node at which to begin counting.
@return None.
*/
//*****************************************************************************	
void XSceneGraph::searchAndCountNodes(XU32& numNodes,XSceneGraphNode* pCurrentNode)
{
	//Check for valid pointers...
	if (!pCurrentNode)
		return;

	//Increment the counter.
	++numNodes;


	XMemBlockList<XSceneGraphNode*>::iterator i(pCurrentNode->m_children.begin());
	while (i.isValid())
	{
		//Skip if NULL, shouldn't be, but just in case
		if (i.getValue() == NULL)
		{
			i.getNext();
			continue;
		}
				   
		searchAndCountNodes(numNodes,i.getValue());
		i.getNext();
	}
}
