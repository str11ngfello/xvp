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
//	Module Name:	XAnimController.h
//	Author:			Lynn Duke
//	Creation Date:	6-12-2003
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ANIM_CONTROLLER_H__
#define __X_ANIM_CONTROLLER_H__




//*****************************************************************************
//!Allows you to play, stop, pause, resume, and even mix animations on scene nodes.
/**
The XAnimController object allows you to animate scene nodes.  It can hold one
or more animation tracks represented by XAnimTrack objects.  The steps below
illustrate typical usage of the animation controller. 

1.  Initialize one or more animation tracks using XAnimTrack objects. <br>
2.  Add the animation tracks to the animation controller. <br>
3.  Attach the animation controller to the desired scene node(s). <br>
4.  Use the XAnimController interface to control the scene node(s). <br><br>

The XAnimController interface exposes several methods that allow you to control
the scene node(s).  You can play, stop, pause, resume, and even mix animation
tracks.  See the documentation below for more information.

@warning	Its important that XAnimTrack objects do not get deallocated while
			they are part of any animation controllers.  This will cause the application
			to become unstable.  
*/
//*****************************************************************************
class XAnimController : public XController
{
private:
	XFreeArray<XAnimTrack*>		m_trackIndexArray;
	XFreeArray<XAnimTrackInfo>	m_currentTrackList;
	XPose*						m_pPose;
	XU32						m_numTracks;
	XMatrix4x4					m_finalMatrix;

	
	//*****************************************************************************
	//Internal accumulator object.
	//*****************************************************************************
	class accumulator
	{
	public:
		XQuaternion rotation;
		XVector3D	translation;
		XVector3D	scale;
		XF32 weight;

		accumulator::accumulator()
		{
			scale.x = scale.y = scale.z = 1;
			weight = 0;
		}
	};

	void flushTimeEvents(XHandle trackHandle);
	void syncTimeEvents(XHandle trackHandle);
	void handleTimeEvents(XHandle trackHandle);

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XAnimController();
	virtual ~XAnimController();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XU32 maxTracks);
	XStatus destroy();
	XU8 getTypeFlags();
	XS8* getTypeDesc();
	bool isCompatible(XSceneNode* pNode);
	XStatus update(XF32 dt);
	XStatus handleNodePreUpdate(XScene* pScene,XSceneNode* pNode,XSceneNode* pParentNode,
							 XFreeArray<XMatrix4x4>* pMatrixStack);
	XStatus addTrack(XAnimTrack* pAnimTrack,XHandle& trackHandle);
	XStatus playTrack(XHandle trackHandle,XF32 speedScale,int playCount,XF32 weight);
	XStatus pauseTrack(XHandle trackHandle);
	XStatus resumeTrack(XHandle trackHandle);
	XStatus stopTrack(XHandle trackHandle);
	XStatus stopAllTracks();
	XStatus flushTracks();
	XStatus setTrackSpeed(XHandle trackHandle,XF32 speedScale);
	XStatus setTrackPlayCount(XHandle trackHandle,int playCount);
	XStatus setTrackWeight(XHandle trackHandle,XF32 weight);
	XF32 getTrackSpeed(XHandle trackHandle);
	XU8 getTrackPlayCount(XHandle trackHandle);
	bool isTrackPlaying(XHandle trackHandle);
	XU32 getTrackTime(XHandle trackHandle);
	XStatus setTrackTime(XHandle trackHandle,XU32 milliseconds);
	XU32 getTrackDuration(XHandle trackHandle);
	XStatus registerEventCallback(XHandle trackHandle,void (*callback) 
		(X_ANIM_EVENT_TYPE type,XU32 eventTime,void* pUserData));
	XStatus setEventCallbackData(XHandle trackHandle,void* pData);
	XStatus registerEvent(XHandle trackHandle,XU32 eventTime);
	XStatus unregisterEvent(XHandle trackHandle,XU32 eventTime);
};


#endif