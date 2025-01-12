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
//	Module Name:	XCamera.h
//	Author:			Lynn Duke
//	Creation Date:	7-16-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_CAMERA_H__
#define __X_CAMERA_H__




//*****************************************************************************
//! Provides a viewpoint into the 3D scene.
/**
In order to view a 3D scene, you must view it through a camera.  The X-VP
object that allows you to control the viewpoint from which your scene is
displayed is the XCamera.  Several methods allow you to change key properties
of the camera such as aspect ratio, near and far clipping planes, etc...

Any number of XCamera objects can be allocated, however, only one camera can
be actively used by the engine at any given time.  Before the engine will
display anything on the screen, it must have an active camera.  You set
a camera active by calling the setActive() method of the XCamera object.
*/
//*****************************************************************************
class XCamera : public XSceneNode
{
private:

	XF32				m_horizontalFOV;
	XF32				m_aspectRatio;
	XF32				m_nearClipPlane;
	XF32				m_farClipPlane;
	XU32				m_viewportWidth;
	XU32				m_viewportHeight;
	XViewingFrustum		m_frustum;
	
	XMatrix4x4			m_prevProjMatrix;
	XMatrix4x4			m_projMatrix;
	XMatrix4x4			m_prevViewMatrix;
	XMatrix4x4			m_viewMatrix;
	XMatrix4x4			m_orthoProjMatrix;
	XBOOL				m_dirtyProj;
	X_PROJECTION_TYPE		m_projType;
	XF32				m_orthoZoom;

	XS32				m_ID;

	XMemBlockList<XPostEffect*>	m_postEffects;



public:

#ifndef XVP_FINAL
	//XModel					m_refModel;
	XScene*					m_pDebugScene;
#endif
XU8	m_editorOrthoType;
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XCamera();
	~XCamera();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XStatus init();
	XStatus initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize);
	XStatus destroy();
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XF32 getHorizontalFOV();
	void setHorizontalFOV(XF32 angle);
	XF32 getAspectRatio();
	XF32 getNearClipPlane();
	void setNearClipPlane(XF32 nearClipPlane);
	XF32 getFarClipPlane();
	void setFarClipPlane(XF32 farClipPlane);
	X_PROJECTION_TYPE getProjectionType();
	void setProjectionType(X_PROJECTION_TYPE type);
	XF32 getOrthoZoom();
	void setOrthoZoom(XF32 orthoZoom);
	XU32 getViewportWidth();
	void setViewportWidth(XU32 viewportWidth);
	XU32 getViewportHeight();
	void setViewportHeight(XU32 viewportHeight);
	XMatrix4x4& getPrevViewMatrix();
	XMatrix4x4& getViewMatrix();
	XMatrix4x4& getPrevProjMatrix();
	XMatrix4x4& getProjMatrix();
	XS32 getID();
	void setID(XS32);
	void extractFrustumPlanes(XViewingFrustum& frustum);
	bool pointInFrustum(XVector3D point);
	bool sphereInFrustumQuick(XSphere& sphere);
	bool cubeInFrustumQuick(XVector3D center,XF32 size);
	X_INTERSECTION_TYPE sphereInFrustum(XSphere& sphere);
	X_INTERSECTION_TYPE cubeInFrustum(XVector3D center,XF32 size);
	X_INTERSECTION_TYPE AABBInFrustum(XVector3D bMin,XVector3D bMax);
	XStatus render();
	virtual XStatus handleViewportResize(XU32 width,XU32 height);

#ifndef XVP_FINAL
	void setDebugScene(XScene* pScene);
	//void setModelTransform(XMatrix4x4& modelTransform)
	//{
		//XModel::setModelTransform(modelTransform);
		//m_refModel.setModelTransform(modelTransform);
	//}
#endif

	void setEditorOrthoType(XU8 orthoType);
	//*************************************************************************
	//Post Effect API functions.
	//*************************************************************************
	XStatus addPostEffect(XPostEffect* pPostEffect);
	XStatus removePostEffect(XPostEffect* pPostEffect);
	XStatus removeAllPostEffects();
	XBOOL containsPostEffect(XPostEffect* pPostEffect);
	XMemBlockList<XPostEffect*>& getPostEffects();
/*
#ifndef XVP_FINAL
	virtual XStatus onAddedToScene(XScene* pScene,XBOOL updateNode);
	virtual XStatus onRemovedFromScene(XScene* pScene,XBOOL updateNode);
#endif*/

	XStatus setFloatProperty(XS8* pPropertyName,XF32 value);
	XStatus setEnumProperty(XS8* pPropertyName,XS32 value);

};


#endif
