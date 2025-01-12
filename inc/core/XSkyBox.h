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
//	Module Name:	XSkyBox.h
//	Author:			Lynn Duke
//	Creation Date:	8-6-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SKYBOX_H__
#define __X_SKYBOX_H__




//*****************************************************************************
//Structures
//*****************************************************************************
struct SKY_BOX_VERTEX
{
	XVector3D	position;	// The position
    XU32		diffuse;	// The color
    XF32		tu, tv;		// The texture coordinates
};



//*****************************************************************************
//*****************************************************************************
class XSkyBox
{
private:
	XF32			m_size;
	SKY_BOX_VERTEX	m_verts[4];
	XHandle			m_faceArray[6];


public:
	static const int NUMFACES;
	enum BoxFace 
	{
		X_TOP = 0,
		X_BOTTOM,
		X_LEFT,
		X_RIGHT,
		X_FRONT,
		X_BACK
	};

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XSkyBox();
	virtual ~XSkyBox();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XS8* pTopFilename,XS8* pBottomFilename,XS8* pFrontFilename,
			  XS8* pBackFilename,XS8* pLeftFilename,XS8* pRightFilename);
	XStatus destroy();
	void render(XMatrix4x4 &matView);
	XF32 getSize(void) const { return(m_size); }
	void setSize(const XF32 data) { m_size = data; } 
};

#endif