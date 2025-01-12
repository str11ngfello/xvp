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
//	Module Name:	XSkyBox.cpp
//	Author:			Lynn Duke
//	Creation Date:	8-6-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


const int XSkyBox::NUMFACES = 6;


//*****************************************************************************
/**
Constructor for the sky box.  
*/
//*****************************************************************************
XSkyBox::XSkyBox()
{
	//Initialize face indices to 0
	for (int q = 0;q < NUMFACES;q++) 
		m_faceArray[q] = 0;
	
	m_size = 100000.0f;

	m_verts[0].diffuse = 0xFFFFFFFF;
	m_verts[1].diffuse = 0xFFFFFFFF;
	m_verts[2].diffuse = 0xFFFFFFFF;
	m_verts[3].diffuse = 0xFFFFFFFF;
}



//*****************************************************************************
/**
Destructor for the sky box.  
*/
//*****************************************************************************
XSkyBox::~XSkyBox() 
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkyBox::init(XS8* pTopFilename,XS8* pBottomFilename,XS8* pFrontFilename,
					XS8* pBackFilename,XS8* pLeftFilename,XS8* pRightFilename)
{
	XVP::GraphicsMgr.createTextureFromFile(pTopFilename,m_faceArray[X_TOP]);
	XVP::GraphicsMgr.createTextureFromFile(pBottomFilename,m_faceArray[X_BOTTOM]);
	XVP::GraphicsMgr.createTextureFromFile(pFrontFilename,m_faceArray[X_FRONT]);
	XVP::GraphicsMgr.createTextureFromFile(pBackFilename,m_faceArray[X_BACK]);
	XVP::GraphicsMgr.createTextureFromFile(pLeftFilename,m_faceArray[X_LEFT]);
	XVP::GraphicsMgr.createTextureFromFile(pRightFilename,m_faceArray[X_RIGHT]); 
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XSkyBox::destroy()
{
	for (XU32 i = 0;i < NUMFACES;i++) 
	{
		XVP::GraphicsMgr.destroyTexture(m_faceArray[i]);
		m_faceArray[i] = X_INVALID_HANDLE;
	}

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
void XSkyBox::render(XMatrix4x4 &matView)
{
	XF32 f;

	f = 0.5 / (XF32)(512);
	m_verts[0].tu = 0.0f + f; m_verts[0].tv = 0.0f + f;
	m_verts[1].tu = 0.0f + f; m_verts[1].tv = 1.0f - f;
	m_verts[2].tu = 1.0f - f; m_verts[2].tv = 0.0f + f;
	m_verts[3].tu = 1.0f - f; m_verts[3].tv = 1.0f - f;

	f = m_size * 0.5f;

	// left (negative x)
	m_verts[0].position = XVector3D(-f,  f,  f);
	m_verts[1].position = XVector3D(-f, -f,  f);
	m_verts[2].position = XVector3D(-f,  f, -f);
	m_verts[3].position = XVector3D(-f, -f, -f);

	XVP::GraphicsMgr.setTexture(m_faceArray[X_LEFT],0);
	//XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_TRIANGLE_STRIP,X_VT_1D_1T,2,(void*)m_verts,
	//									sizeof(SKY_BOX_VERTEX));


	// right (positive x)
	m_verts[0].position = XVector3D( f,  f, -f);
	m_verts[1].position = XVector3D( f, -f, -f);
	m_verts[2].position = XVector3D( f,  f,  f);
	m_verts[3].position = XVector3D( f, -f,  f);

	XVP::GraphicsMgr.setTexture(m_faceArray[X_RIGHT],0);
/*	XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_TRIANGLE_STRIP,X_VT_1D_1T,2,(void*)m_verts,
										sizeof(SKY_BOX_VERTEX));*/


	// down (negative y)
	m_verts[1].position = XVector3D(-f, -f, -f);
	m_verts[3].position = XVector3D(-f, -f,  f);
	m_verts[0].position = XVector3D( f, -f, -f);
	m_verts[2].position = XVector3D( f, -f,  f);

	XVP::GraphicsMgr.setTexture(m_faceArray[X_BOTTOM],0);
	/*XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_TRIANGLE_STRIP,X_VT_1D_1T,2,(void*)m_verts,
										sizeof(SKY_BOX_VERTEX));*/

  
	// up (positive y)
	m_verts[0].position = XVector3D(-f,  f,  f);
	m_verts[1].position = XVector3D(-f,  f, -f);
	m_verts[2].position = XVector3D( f,  f,  f);
	m_verts[3].position = XVector3D( f,  f, -f);

	XVP::GraphicsMgr.setTexture(m_faceArray[X_TOP],0);
	/*XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_TRIANGLE_STRIP,X_VT_1D_1T,2,(void*)m_verts,
										sizeof(SKY_BOX_VERTEX));*/

	// back (negative z)
	m_verts[0].position = XVector3D(-f,  f, -f);
	m_verts[1].position = XVector3D(-f, -f, -f);
	m_verts[2].position = XVector3D( f,  f, -f);
	m_verts[3].position = XVector3D( f, -f, -f);

	XVP::GraphicsMgr.setTexture(m_faceArray[X_BACK],0);
	/*XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_TRIANGLE_STRIP,X_VT_1D_1T,2,(void*)m_verts,
										sizeof(SKY_BOX_VERTEX));*/

	// front (positive z)
	m_verts[0].position = XVector3D( f,  f,  f);
	m_verts[1].position = XVector3D( f, -f,  f);
	m_verts[2].position = XVector3D(-f,  f,  f);
	m_verts[3].position = XVector3D(-f, -f,  f);

	XVP::GraphicsMgr.setTexture(m_faceArray[X_FRONT],0);
	/*XVP::GraphicsMgr.renderUserVertexBuffer(X_PT_TRIANGLE_STRIP,X_VT_1D_1T,2,(void*)m_verts,
								   sizeof(SKY_BOX_VERTEX));*/
}
  
