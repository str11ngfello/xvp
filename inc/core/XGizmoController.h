#ifndef __X_GIZMO_CONTROLLER_H__
#define __X_GIZMO_CONTROLLER_H__




class XGizmoController : public XController
{
private:
	XU32		m_viewportIndex;
	XVector3D	m_position;
	XMatrix4x4	m_transform;
	XBOOL		m_lockTransform;
	
public:

	XGizmoController();
	~XGizmoController();
	
	XS8* getTypeDesc();

	XStatus update(float dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
							 XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus setViewportIndex(XU32 viewportIndex);
	XStatus setPosition(XVector3D& position);
	XStatus setTransform(XMatrix4x4& transform);
	XBOOL isTransformLockEnabled();
	XStatus enableTransformLock(XBOOL enable);

};

#endif