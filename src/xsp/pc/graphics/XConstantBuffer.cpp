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
//	Module Name:	XConstantBuffer.cpp
//	Author:			Lynn Duke
//	Creation Date:	2-20-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XConstantBuffer::XConstantBuffer()
{	
	m_pConstantBuffer = NULL;
	m_dynamic = false;
	m_locked = false;
}



//*****************************************************************************
//*****************************************************************************
XConstantBuffer::~XConstantBuffer()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XConstantBuffer::init(XU32 sizeInBytes,XBOOL dynamic)
{
	m_dynamic = dynamic;

	D3D10_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeInBytes;
	if (dynamic)
	{
		cbDesc.Usage = D3D10_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}
	else
	{
		cbDesc.Usage = D3D10_USAGE_DEFAULT;
		cbDesc.CPUAccessFlags = 0;
	}
    cbDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
    
    cbDesc.MiscFlags = 0;
	HRESULT hr = XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&cbDesc,NULL,&m_pConstantBuffer);
	X_ASSERTM(hr == S_OK,"Failed to create constant buffer.");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XConstantBuffer::destroy()
{
	SAFE_RELEASE(m_pConstantBuffer);
	return X_OK;
}




//*****************************************************************************
//*****************************************************************************
void* XConstantBuffer::lock(D3D10_MAP mapType)
{
	void* pConstantData = NULL;
	m_locked = true;
	m_pConstantBuffer->Map(mapType,NULL,&pConstantData);
    return pConstantData;
}



//*****************************************************************************
//*****************************************************************************
XStatus XConstantBuffer::unlock()
{
	m_locked = false;
	m_pConstantBuffer->Unmap();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XBOOL XConstantBuffer::isLocked()
{
	return m_locked;
}



//*****************************************************************************
//*****************************************************************************
XStatus XConstantBuffer::setData(void* pData,XU32 dataSizeInBytes)
{
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pConstantBuffer,
													 D3D10CalcSubresource(0,0,1),
													 NULL,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
ID3D10Buffer* XConstantBuffer::getD3DConstantBuffer()
{
	return m_pConstantBuffer;
}


XBOOL XConstantBuffer::isDynamic()
{
	return m_dynamic;
}