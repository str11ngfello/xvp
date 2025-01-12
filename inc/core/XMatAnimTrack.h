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
//	Module Name:	XMatAnimTrack.h
//	Author:			Lynn Duke
//	Creation Date:	7-24-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_MAT_ANIM_TRACK_H__
#define __X_MAT_ANIM_TRACK_H__




//*****************************************************************************
//*****************************************************************************
class XFloatKey
{
public:
	XU32			time;	//Timestamp in milliseconds
	XF32			value;	//float representation
};




//*****************************************************************************
//! Represents a single, complete material animation and its corresponding data.
/**
The XMatAnimTrack object is used to store a single animation sequence.  Once 
this object has been intialized with animation data, it can be added to one or
more animation controllers (XAnimController) for playback on scene nodes.

The animation controller is the primary consumer of XMatAnimTrack objects.  As the
animation controller is instructed to play animations, it refers back to the
XMatAnimTrack objects to obtain information such as rotation, position and 
scale key frames which it then uses to control the local transforms of scene
nodes. 

The steps below illustrate typical usage of XMatAnimTrack objects in conjunction
with animation controllers. 

1.  Initialize one or more animation tracks using XMatAnimTrack objects. <br>
2.  Add the animation tracks to the animation controller. <br>
3.  Attach the animation controller to the desired scene node(s). <br>
4.  Use the XAnimController interface to control the scene node(s). <br><br>

See the XAnimController for more details.
*/
//*****************************************************************************
class XMatAnimTrack
{
public:
	XFreeArray<XFloatKey>			m_uOffsetKeyList;
	XFreeArray<XFloatKey>			m_vOffsetKeyList;
	XFreeArray<XFloatKey>			m_wAngleKeyList;
	XU32							m_lastUOffsetIndex;		//last u offset key searched for
	XU32							m_lastVOffsetIndex;		//last v offset key searched for
	XU32							m_lastWAngleIndex;		//last w angle key searched for
	XU32							m_timeLength;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XMatAnimTrack();
	virtual ~XMatAnimTrack();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pFilename);
	XStatus destroy();
};

#endif
