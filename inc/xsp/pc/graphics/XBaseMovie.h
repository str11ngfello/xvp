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
//	Module Name:	XBaseMovie.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_BASE_MOVIE_H__
#define __X_BASE_MOVIE_H__

#include <XOSMgr.h>
#include <XResource.h>
#include <XMemBlockList.h>
#include <d3d10.h>
#include <d3dx10.h>




//*****************************************************************************
//! Base Movie.
//*****************************************************************************
class XBaseMovie : public XResource
{
public:
	static XMemBlockList<XBaseMovie*>	m_movieList;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XBaseMovie();
	virtual ~XBaseMovie();

	//*****************************************************************************
	//Standard Texture Functions
	//*****************************************************************************
	virtual XStatus init(XS8* pFilename) = 0;
	virtual XStatus destroy() = 0;
	virtual ID3D10ShaderResourceView* getD3DTexture() = 0;
	virtual XStatus play(XS32 playCount) = 0;
	virtual XStatus pause() = 0;
	virtual XStatus resume() = 0;
	virtual XStatus stop() = 0;
	virtual XStatus setTime(XU32 milliseconds) = 0;
	virtual	XU32 getTime() = 0;
	virtual XStatus setPlayCount(XS32 playCount) = 0;
	virtual XS32 getPlayCount() = 0;
	virtual XU32 getDuration() = 0;
	virtual XU32 getWidth() = 0;
	virtual XU32 getHeight() = 0;
	virtual bool isPlaying() = 0;
	virtual	XStatus update() = 0;
};


#endif