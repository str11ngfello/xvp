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

#ifndef __X_DECAL_MGR_H__
#define __X_DECAL_MGR_H__




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
class XDecalMgr : public XSceneNode
{
private:

	class XDecal
	{
	private:
		XHandle m_vbHandle;
		XVector3D m_position;
		XF32 m_age;
		XF32 m_lifetime;
		
	public:
		XDecal()
		{
			m_age = -1.0f;		// < 0 means dead
			m_lifetime = 0.0f;
		}

		XStatus init(XVector3D& position,XF32 lifetime)
		{	
			m_position = position;
			m_age = 0.0f;
			m_lifetime = lifetime;

			return X_OK;
		}

		XStatus destroy()
		{
			return X_OK;
		}

		XBOOL update(XF32 dt)
		{
			//First update the age of the decal.  If it has died,
			//then return false to indicate the decal has died.
			if (m_age + dt > m_lifetime)
				return false;

			//If we make if here then this particular decal is alive so we
			//should update it.  
			m_age += dt;

			return true;
		}
	};

	XFreeList<XDecal>	m_decalFreeList;
	XDecal**			m_pDecalsInUse;
	XU32				m_numActiveDecals;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XDecalMgr();
	~XDecalMgr();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	X_SCENE_NODE_TYPE getType();
	XS8* getTypeDesc();
	XStatus init(XU32 maxDecals);
	XStatus destroy();
	XStatus update(XF32 dt,XScene* pScene,XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus add(XVector3D& position,XF32 lifetime);
	XStatus removeAll();
	XStatus setMaxDecals(XU32 maxDecals);

};


#endif
