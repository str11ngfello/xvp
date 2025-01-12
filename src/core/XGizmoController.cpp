#include <xvpsdk.h>

XGizmoController::XGizmoController()
{
	m_viewportIndex = 0;
	m_lockTransform = false;
}


XGizmoController::~XGizmoController()
{
}




//*****************************************************************************
/**
Returns a string description of the chase controller.  This is the
description that appears in log files and other debug output.

@return A string description of the chase controller.
*/
//*****************************************************************************
XS8* XGizmoController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XGizmoController";
	return description; 
}



XStatus XGizmoController::update(float dt)
{
	return X_OK;
}


XStatus XGizmoController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
										  XFreeArray<XMatrix4x4>* pMatrixStack)
{
	XVector3D camPos,nodePos;
	XCamera* pCam =pScene->getViewportCamera(m_viewportIndex);
	if (!pCam)
		return X_OK;
	pCam->getWorldPosition(camPos);
	pNode->getWorldPosition(nodePos);
	XF32 distance = camPos.distance(nodePos);
	XF32 scale = distance/75.0f;
	XMatrix4x4 newTransform = m_transform;//pNode->getModelTransform();
	
	//Create a new matrix that keep the rotation of the node if not locking transform
	//but substitute new scale
	if (m_lockTransform)
	{
		newTransform.row1 = XVector4D(1.0f,0.0f,0.0f,0.0f);
		newTransform.row2 = XVector4D(0.0f,1.0f,0.0f,0.0f);
		newTransform.row3 = XVector4D(0.0f,0.0f,1.0f,0.0f);
	}
	newTransform.row1.setLength(scale);
	newTransform.row2.setLength(scale);
	newTransform.row3.setLength(scale);

	pNode->setModelTransform(newTransform);

	return X_OK;
}



XStatus XGizmoController::setViewportIndex(XU32 viewportIndex)
{
	m_viewportIndex = viewportIndex;
	return X_OK;
}
XStatus XGizmoController::setPosition(XVector3D& position)
{
	m_position = position;
	return X_OK;
}

XStatus XGizmoController::setTransform(XMatrix4x4& transform)
{
	m_transform = transform;
	return X_OK;
}


XBOOL XGizmoController::isTransformLockEnabled()
{
	return m_lockTransform;
}

XStatus XGizmoController::enableTransformLock(XBOOL enable)
{
	m_lockTransform = enable;
	return X_OK;
}