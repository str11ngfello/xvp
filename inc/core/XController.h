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
//	Module Name:	XController.h
//	Author:			Lynn Duke
//	Creation Date:	6-6-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CONTROLLER_H__
#define __X_CONTROLLER_H__





//*****************************************************************************
//*****************************************************************************
enum X_CONTROLLER_TYPE_FLAGS
{
	X_CTRT_PRE_UPDATE = 0x01,
	X_CTRT_POST_UPDATE = 0x02,
	X_CTRT_RENDER = 0x04
};

//*****************************************************************************
//! Base controller object that specifies interface requirements for controllers.
/**
Along with the XSceneNode object, the XController object is one of the most
fundamental objects in the entire engine.  Controllers can be attached to scene
nodes derived from XSceneNode and can influence the behavior of the node.
Controllers are classified into two main types, update controllers and render
controllers. 

Depending on the type of controller, it will modify the node during the update pass
or the render pass of X-VP's main scene graph.  Typcially, you will
override member functions based upon the capabilites you want to build into
your specific controller.  For example, if you want your controller to modify
a scene node's local transform during the scene graph's update pass, you would
override handleUpdateNotification() and supply your custom functionality.  On
the other hand, if you wanted your custom controller to modify the scene node's
texture states during the scene graph's render pass, you would override the
handleRenderNotification() method and supply your custom functionality there.

See the member documentation for details.
*/
//*****************************************************************************
class XController
{
protected:
	XU8								m_typeFlags;
	XU32							m_refCount;
	XMemBlockList<XSceneNode*>		m_updateClients;
	XMemBlockList<XSceneNode*>		m_renderClients;
	XBOOL							m_enabled;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XController();
	virtual ~XController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	virtual XStatus destroy();
	XU32 getNumClients();
	virtual XU8 getTypeFlags();
	virtual XS8* getTypeDesc();
	virtual bool isCompatible(XSceneNode* pNode);
	XStatus deallocationNotify(XSceneNode* pNode);
	XBOOL isEnabled();
	virtual XStatus enable(XBOOL enable);

	XStatus prepForAttachment(XSceneNode* pNode);
	XStatus prepForDetachment(XSceneNode* pNode);

	virtual XStatus handleUpdateAttachment(XSceneNode* pNode);
	virtual XStatus handleUpdateDetachment(XSceneNode* pNode);
	virtual XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
											   XFreeArray<XMatrix4x4>* pMatrixStack);
	virtual XStatus handleNodePostUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
											   XFreeArray<XMatrix4x4>* pMatrixStack);


	virtual XStatus handleRenderAttachment(XSceneNode* pNode);
	virtual XStatus handleRenderDetachment(XSceneNode* pNode);
	virtual XStatus handleNodeRender(XScene* pScene,XSceneNode* pNode);
	
	virtual XStatus update(XF32 dt);
};

#endif