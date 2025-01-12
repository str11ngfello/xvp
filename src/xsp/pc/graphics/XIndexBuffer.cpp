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
//	Module Name:	XIndexBuffer.cpp
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
XIndexBuffer::XIndexBuffer()
{
}



//*****************************************************************************
//*****************************************************************************
XIndexBuffer::~XIndexBuffer()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XIndexBuffer::init(XU32 numIndices,XBOOL dynamic)
{
    D3D10_BUFFER_DESC bd;
	if (dynamic)
	{
		bd.Usage = D3D10_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
	}
    bd.ByteWidth = sizeof(XU16) * numIndices;
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    
    bd.MiscFlags = 0;
    HRESULT hr = XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_ib);

    if (hr == S_OK)
	{
		m_numIndices = numIndices;
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;

#if OLD
	HRESULT hr;

	DWORD usage = 0;
	if (dynamic)
		usage = D3DUSAGE_DYNAMIC;
	else
		usage = D3DUSAGE_WRITEONLY;

	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateIndexBuffer(numIndices * sizeof(WORD),
											usage,D3DFMT_INDEX16,D3DPOOL_DEFAULT,
											&m_ib,NULL);
			
	
	if (hr == D3D_OK)
	{
		m_numIndices = numIndices;
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;

#endif
}



//*****************************************************************************
//*****************************************************************************
XStatus XIndexBuffer::destroy()
{
	//Release the index buffer.
	SAFE_RELEASE(m_ib);
	XResource::destroy();
	return X_OK;
}
	


//*****************************************************************************
//*****************************************************************************
XStatus XIndexBuffer::setToRenderingDevice()
{
	XVP::GraphicsMgr.m_pD3DDevice->IASetIndexBuffer(m_ib,DXGI_FORMAT_R16_UINT,0);
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU16* XIndexBuffer::lock(X_LOCK_TYPE lockType)
{
	XU16* pIndices;
	HRESULT hr;
	D3D10_MAP mapType;
	switch (lockType)
	{
	case X_LT_READ_ONLY:
		mapType = D3D10_MAP_READ;
		break;
	case X_LT_WRITE_ONLY:
		mapType = D3D10_MAP_WRITE;
		break;
	case X_LT_READ_WRITE:
		mapType = D3D10_MAP_READ_WRITE;
		break;
	case X_LT_DISCARD:
		mapType = D3D10_MAP_WRITE_DISCARD;
		break;
	default:
		X_ASSERT_FORCED("Unrecognized lock type while locking vertex buffer")
		break;
	}

	hr = m_ib->Map(mapType,0,(void**)&pIndices);

	if (hr == S_OK)
		return pIndices;
	else 
		return NULL;
}


//*****************************************************************************
//*****************************************************************************
XStatus XIndexBuffer::unlock()
{
	m_ib->Unmap();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XIndexBuffer::getLength()
{
	return m_numIndices;
}


void XIndexBuffer::setData(void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_ib,
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}




