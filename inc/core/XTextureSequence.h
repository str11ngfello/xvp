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
//	Module Name:	XTextureSequence.h
//	Author:			Lynn Duke
//	Creation Date:	5-6-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_TEXTURE_SEQUENCE_H__
#define __X_TEXTURE_SEQUENCE_H__




//*****************************************************************************
//*****************************************************************************
class XTextureSequence
{
private:
	vector<XHandle>		m_textureHandles;
	XU32				m_currentIndex;	
	XF32				m_elapsedTime;
	XF32				m_delayFrameTime;
	XF64				m_numFramesToElapse;
	XU32				m_width;
	XU32				m_height;
	XBOOL				m_playing;

public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XTextureSequence();
	~XTextureSequence();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pFilenameBase,XS8* pExtension,XU32 numFiles);
	XStatus destroy();
	XStatus update(XF32 dt);
	XStatus pause();
	XStatus resume();
	XStatus setFrameDelay(XF32 frameDelay);
	XF32 getFrameDelay();
	XHandle getCurrentHandle();
	XU32 getNumTextures();
	XU32 getWidth();
	XU32 getHeight();
};

#endif