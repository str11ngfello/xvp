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
//	Module Name:	XTexture.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>




//*****************************************************************************
//*****************************************************************************
XTexture::XTexture()
{
	m_pTexture = NULL;
}



//*****************************************************************************
//*****************************************************************************
XTexture::~XTexture()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XTexture::initFromFile(XS8* pFilename)
{
	X_ASSERTM(pFilename,"Invalid texture filename.");

	//Create the texture
	HRESULT hr;
    hr = D3DX10CreateShaderResourceViewFromFile(XVP::GraphicsMgr.m_pD3DDevice,
		pFilename,
												NULL,
												NULL,
												&m_pTexture,
												&hr);
	if (hr == S_OK)
	{
		D3DX10GetImageInfoFromFile(pFilename,NULL,&m_info,&hr);
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTexture::initFromMemory(XS8* pMemoryChunk,XU32 memChunkSize)
{
	X_ASSERTM(pMemoryChunk,"Invalid texture memory pointer.");

	//Create the texture
	HRESULT hr;
    hr = D3DX10CreateShaderResourceViewFromMemory(XVP::GraphicsMgr.m_pD3DDevice,
												pMemoryChunk,
												memChunkSize,
												NULL,
												NULL,
												&m_pTexture,
												&hr);
		
	if (hr == S_OK)
	{
		D3DX10GetImageInfoFromMemory(pMemoryChunk,memChunkSize,NULL,&m_info,&hr);
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
//*****************************************************************************
ID3D10ShaderResourceView* XTexture::getD3DTexture()
{
	return m_pTexture;
}



//*****************************************************************************
//*****************************************************************************
XStatus XTexture::destroy()
{
	SAFE_RELEASE(m_pTexture);
	XResource::destroy();
	return X_OK;
}

