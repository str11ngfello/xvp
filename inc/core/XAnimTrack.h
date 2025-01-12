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
//	Module Name:	XAnimTrack.h
//	Author:			Lynn Duke
//	Creation Date:	12-10-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_ANIM_TRACK_H__
#define __X_ANIM_TRACK_H__



//*****************************************************************************
//*****************************************************************************
class XRotationKey
{
public:
	XU32			time;	//Timestamp in milliseconds
	XQuaternion	quat;	//Quaternion rotation representation
};



//*****************************************************************************
//*****************************************************************************
class XPositionKey
{
public:
	XU32		time;	//Timestamp in milliseconds
	XVector3D	vec;	//Vector distance representation
};



//*****************************************************************************
//*****************************************************************************
class XScaleKey
{
public:
	XU32		time;	//Timestamp in milliseconds
	XVector3D	vec;	//Scaling vector representation
};



//*****************************************************************************
//*****************************************************************************
struct XKeyFrameSet
{
	//XU32					ID;
	XFreeArray<XRotationKey>	rotKeyList;
	XFreeArray<XPositionKey>	posKeyList;
	XFreeArray<XScaleKey>		scaleKeyList;
	XU32						lastRotIndex;		//last rotation key searched for
	XU32						lastPosIndex;		//last position key searched for
	XU32						lastScaleIndex;		//last scale key searched for

	XKeyFrameSet::XKeyFrameSet()
	{
		lastRotIndex = 0;
		lastPosIndex = 0;
		lastScaleIndex = 0;
	}

	XKeyFrameSet::~XKeyFrameSet()
	{
		rotKeyList.clear();
		posKeyList.clear();
		scaleKeyList.clear();
	}
};



//*****************************************************************************
//! Represents a single, complete animation and it corresponding data.
/**
The XAnimTrack object is used to store a single animation sequence.  Once 
this object has been intialized with animation data, it can be added to one or
more animation controllers (XAnimController) for playback on scene nodes.

The animation controller is the primary consumer of XAnimTrack objects.  As the
animation controller is instructed to play animations, it refers back to the
XAnimTrack objects to obtain information such as rotation, position and 
scale key frames which it then uses to control the local transforms of scene
nodes. 

The steps below illustrate typical usage of XAnimTrack objects in conjunction
with animation controllers. 

1.  Initialize one or more animation tracks using XAnimTrack objects. <br>
2.  Add the animation tracks to the animation controller. <br>
3.  Attach the animation controller to the desired scene node(s). <br>
4.  Use the XAnimController interface to control the scene node(s). <br><br>

See the XAnimController for more details.
*/
//*****************************************************************************
class XAnimTrack
{
public:
	vector<XKeyFrameSet*>			m_keysList;
	XU32							m_timeLength;
	XS8*							m_pName;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XAnimTrack();
	virtual ~XAnimTrack();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pName, XS8* pFilename);
	XStatus initFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize);
	XStatus destroy();
	XS8* getName();
	XStatus setName(XS8* pName);
};

#endif
