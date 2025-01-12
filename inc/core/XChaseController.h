#ifndef __X_CHASE_CONTROLLER_H__
#define __X_CHASE_CONTROLLER_H__




class XChaseController : public XController
{
private:
	XSceneNode*		m_pTarget;
	float			m_xRotOffset;
	float			m_yRotOffset;
	float			m_zRotOffset;
	float			m_xTransOffset;
	float			m_yTransOffset;
	float			m_zTransOffset;
	float			m_blendRate;
	float			m_scaledBlendRate;

public:

	XChaseController();
	~XChaseController();
	
	XU8 getTypeFlags();
	XS8* getTypeDesc();

	XStatus update(float dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
							 XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus setTarget(XSceneNode* pTarget);
	XStatus setXRotOffset(float xRotOffset);
	float getXRotOffset();
	XStatus setYRotOffset(float yRotOffset);
	float getYRotOffset();
	XStatus setZRotOffset(float zRotOffset);
	float getZRotOffset();
	XStatus setXTransOffset(float xTransOffset);
	float getXTransOffset();
	XStatus setYTransOffset(float yTransOffset);
	float getYTransOffset();
	XStatus setZTransOffset(float zTransOffset);
	float getZTransOffset();
	XStatus setBlendRate(float blendRate);
	float getBlendRate();
};

#endif