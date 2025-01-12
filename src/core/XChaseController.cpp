#include <xvpsdk.h>

XChaseController::XChaseController()
{
	m_pTarget = NULL;	
	m_xRotOffset = 0;
	m_yRotOffset = 0;
	m_zRotOffset = 0;
	m_xTransOffset = 0;
	m_yTransOffset = 0;
	m_zTransOffset = 0;
	m_blendRate = 0;
	m_scaledBlendRate = 0;
}


XChaseController::~XChaseController()
{
}



//*****************************************************************************
//*****************************************************************************
XU8 XChaseController::getTypeFlags()
{
	static XU8 typeFlags = X_CTRT_PRE_UPDATE;
	return typeFlags;
}



//*****************************************************************************
/**
Returns a string description of the chase controller.  This is the
description that appears in log files and other debug output.

@return A string description of the chase controller.
*/
//*****************************************************************************
XS8* XChaseController::getTypeDesc()
{
	//Declare a static description string for this controller.  The base
	//controller class will use this string for identification.
	static char description[X_TYPE_DESC_LEN] = "XChaseController";
	return description; 
}



XStatus XChaseController::update(float dt)
{
	m_scaledBlendRate = m_blendRate * dt;
	return X_OK;
}


XStatus XChaseController::handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
										  XFreeArray<XMatrix4x4>* pMatrixStack)
{
	//If we don't have a target we can't chase anything, but that's okay.
	if (!m_pTarget)
		return X_OK;

    XMatrix4x4 normalMatrix = m_pTarget->getWorldTransform();;
	XMatrix4x4 offsetMatrix;

	offsetMatrix.createYRotation(m_yRotOffset);
	normalMatrix = offsetMatrix * normalMatrix;
	
	offsetMatrix.setIdentity();
	offsetMatrix.createXRotation(m_xRotOffset);
	normalMatrix = offsetMatrix * normalMatrix;
	

	offsetMatrix.setIdentity();
	offsetMatrix.createZRotation(m_zRotOffset);
	normalMatrix = offsetMatrix * normalMatrix;

	offsetMatrix.setIdentity();
	offsetMatrix._41 = m_xTransOffset;
	offsetMatrix._42 = m_yTransOffset;
	offsetMatrix._43 = m_zTransOffset;
	normalMatrix = offsetMatrix * normalMatrix;

	XMatrix4x4 nodeMatrix = pNode->getWorldTransform();

	XVector3D deltaVec = XVector3D(normalMatrix.row4) - XVector3D(nodeMatrix.row4);
	deltaVec *= m_scaledBlendRate;
	nodeMatrix._41 += deltaVec.x;
	nodeMatrix._42 += deltaVec.y;
	nodeMatrix._43 += deltaVec.z;

	deltaVec = XVector3D(normalMatrix.row3) - XVector3D(nodeMatrix.row3);
	deltaVec *= m_scaledBlendRate;
	nodeMatrix._31 += deltaVec.x;
	nodeMatrix._32 += deltaVec.y;
	nodeMatrix._33 += deltaVec.z;

	deltaVec = XVector3D(normalMatrix.row2) - XVector3D(nodeMatrix.row2);
	deltaVec *= m_scaledBlendRate;
	nodeMatrix._21 += deltaVec.x;
	nodeMatrix._22 += deltaVec.y;
	nodeMatrix._23 += deltaVec.z;

	deltaVec = XVector3D(normalMatrix.row1) - XVector3D(nodeMatrix.row1);
	deltaVec *= m_scaledBlendRate;
	nodeMatrix._11 += deltaVec.x;
	nodeMatrix._12 += deltaVec.y;
	nodeMatrix._13 += deltaVec.z;

	nodeMatrix.orthonormalize();
    pNode->setModelTransform(nodeMatrix);

	return X_OK;
}



XStatus XChaseController::setTarget(XSceneNode *pTarget)
{
	m_pTarget = pTarget;
	return X_OK;
}


XStatus XChaseController::setXRotOffset(float xRotOffset)
{
	m_xRotOffset = xRotOffset;
	return X_OK;
}



float XChaseController::getXRotOffset()
{
	return m_xRotOffset;
	return X_OK;
}



XStatus XChaseController::setYRotOffset(float yRotOffset)
{
	m_yRotOffset = yRotOffset;
	return X_OK;
}



float XChaseController::getYRotOffset()
{
	return m_yRotOffset;
}



XStatus XChaseController::setZRotOffset(float zRotOffset)
{
	m_zRotOffset = zRotOffset;
	return X_OK;
}



float XChaseController::getZRotOffset()
{
	return m_xRotOffset;
}



XStatus XChaseController::setXTransOffset(float xTransOffset)
{
	m_xTransOffset = xTransOffset;
	return X_OK;
}



float XChaseController::getXTransOffset()
{
	return m_xTransOffset;
}



XStatus XChaseController::setYTransOffset(float yTransOffset)
{
	m_yTransOffset = yTransOffset;
	return X_OK;
}



float XChaseController::getYTransOffset()
{
	return m_yTransOffset;
}



XStatus XChaseController::setZTransOffset(float zTransOffset)
{
	m_zTransOffset = zTransOffset;
	return X_OK;
}



float XChaseController::getZTransOffset()
{
	return m_zTransOffset;
}



XStatus XChaseController::setBlendRate(float blendRate)
{
	m_blendRate = blendRate;
	return X_OK;
}



float XChaseController::getBlendRate()
{
	return m_blendRate;
}
