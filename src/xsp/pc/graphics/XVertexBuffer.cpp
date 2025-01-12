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
//	Module Name:	XVertexBuffer.cpp
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
XVertexBuffer::XVertexBuffer()
{	
	m_pPositionsVB = NULL;
	m_pPositions = NULL;

	m_pColorsVB = NULL;
	m_pColors = NULL;

	m_pNormalsVB = NULL;
	m_pNormals = NULL;

	m_pUVsVB = NULL;
	m_pUVs = NULL;

	m_pTangentsVB = NULL;
	m_pTangents = NULL;

	m_pBinormalsVB = NULL;
	m_pBinormals = NULL;

	m_pBlendIndicesVB = NULL;
	m_pBlendIndices = NULL;

	m_pBlendWeightsVB = NULL;
	m_pBlendWeights = NULL;

	m_pVertexDeclaration = NULL;
	m_numVertices = 0;
	m_sizeInBytes = 0;
	m_dynamic = false;
}



//*****************************************************************************
//*****************************************************************************
XVertexBuffer::~XVertexBuffer()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::init(XHandle vdHandle,X_PRIMITIVE_TYPE primType,XU32 numVertices,
							XBOOL dynamic)
{
	if (vdHandle == X_INVALID_HANDLE)
		return X_ERR_INVALID_PARAMS;

	//Save a pointer to the vertex declaration
	m_pVertexDeclaration = XVP::GraphicsMgr.getVertexDeclarationInterface(vdHandle);
	m_pVertexDeclaration->getDesc(m_desc);

	D3D10_BUFFER_DESC bd;

	if (dynamic)
	{
		bd.Usage = D3D10_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	}
	else
	{
		bd.Usage = D3D10_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;;
	}
    m_dynamic = dynamic;

    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;   
    bd.MiscFlags = 0;
    	
	//Positions, theres always exactly 1 position stream.
	bd.ByteWidth = numVertices * sizeof(XVector3D);
	XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pPositionsVB);

	//XVP::GraphicsMgr.m_pD3DDevice->CreateVertexBuffer(numVertices * sizeof(XVector3D),
	//				           					usage,0,D3DPOOL_DEFAULT ,&m_pPositionsVB,NULL);

	//Colors
	if (m_desc.numColors != 0)
	{
		XNewArray(m_pColors,XU32*,m_desc.numColors,XOSMgr::getDefaultHeap())
		XNewArray(m_pColorsVB,ID3D10Buffer*,m_desc.numColors,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XU32);
		for (XU32 i = 0;i < m_desc.numColors;++i)
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pColorsVB[i]);
	}

	//Normals
	if (m_desc.numNormals != 0)
	{
		XNewArray(m_pNormals,XVector3D*,m_desc.numNormals,XOSMgr::getDefaultHeap())
		XNewArray(m_pNormalsVB,ID3D10Buffer*,m_desc.numNormals,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XVector3D);
		for (XU32 i = 0;i < m_desc.numNormals;++i)	
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pNormalsVB[i]);
	}

	//UVs
	if (m_desc.numUVs != 0)
	{
		XNewArray(m_pUVs,XVector2D*,m_desc.numUVs,XOSMgr::getDefaultHeap())
		XNewArray(m_pUVsVB,ID3D10Buffer*,m_desc.numUVs,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XVector2D);
		for (XU32 i = 0;i < m_desc.numUVs;++i)	
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pUVsVB[i]);
	}

	//Tangents
	if (m_desc.numTangents != 0)
	{
		XNewArray(m_pTangents,XVector3D*,m_desc.numTangents,XOSMgr::getDefaultHeap())
		XNewArray(m_pTangentsVB,ID3D10Buffer*,m_desc.numTangents,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XVector3D);
		for (XU32 i = 0;i < m_desc.numTangents;++i)	
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pTangentsVB[i]);
	}

	//Binormals
	if (m_desc.numBinormals != 0)
	{
		XNewArray(m_pBinormals,XVector3D*,m_desc.numBinormals,XOSMgr::getDefaultHeap())
		XNewArray(m_pBinormalsVB,ID3D10Buffer*,m_desc.numBinormals,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XVector3D);
		for (XU32 i = 0;i < m_desc.numBinormals;++i)	
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pBinormalsVB[i]);
	}

	//Blend Indices
	/*if (m_desc.numBoneIndices4Tuplets != 0)
	{
		XNewArray(m_pBlendIndices,XVector4D*,m_desc.numBoneIndices4Tuplets,XOSMgr::getDefaultHeap())
		XNewArray(m_pBlendIndicesVB,ID3D10Buffer*,m_desc.numBoneIndices4Tuplets,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XVector4D);
		for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)	
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pBlendIndicesVB[i]);
	}

	//Bone Weights
	if (m_desc.numBoneWeights4Tuplets != 0)
	{
		XNewArray(m_pBlendWeights,XVector4D*,m_desc.numBoneWeights4Tuplets,XOSMgr::getDefaultHeap())
		XNewArray(m_pBlendWeightsVB,ID3D10Buffer*,m_desc.numBoneWeights4Tuplets,XOSMgr::getDefaultHeap())
		bd.ByteWidth = numVertices * sizeof(XVector4D);
		for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)	
			XVP::GraphicsMgr.m_pD3DDevice->CreateBuffer(&bd,NULL,&m_pBlendWeightsVB[i]);
	}*/
/*
			//Create the vertex buffer depending on the type specified.
			switch (vbType)
			{
			case X_VT_RHW:
				hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexBuffer(numVertices * sizeof(XRHWVertex),
					           						   usage,0,
												       D3DPOOL_DEFAULT ,&m_vb,NULL);
				m_sizeInBytes = numVertices * sizeof(XRHWVertex);
				break;
			case X_VT_PLAIN: 
				hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexBuffer(numVertices * sizeof(XPlainVertex),
					           						   usage ,0,
												       D3DPOOL_DEFAULT ,&m_vb,NULL);
				m_sizeInBytes = numVertices * sizeof(XPlainVertex);
				break;
			case X_VT_MODEL: 
				hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexBuffer(numVertices * sizeof(XModelVertex),
					           						   usage ,0,
												       D3DPOOL_DEFAULT ,&m_vb,NULL);
				m_sizeInBytes = numVertices * sizeof(XModelVertex);
				break;
			case X_VT_SKIN: 
				hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexBuffer(numVertices * sizeof(XSkinVertex),
					           						   usage ,0,
													   D3DPOOL_DEFAULT ,&m_vb,NULL);
				m_sizeInBytes = numVertices * sizeof(XSkinVertex);
				break;
			case X_VT_PARTICLE: 
				hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexBuffer(numVertices * sizeof(XParticleVertex),
					           						   usage ,0,
													   D3DPOOL_DEFAULT ,&m_vb,NULL);
				m_sizeInBytes = numVertices * sizeof(XParticleVertex);
				break;
			default:
				//Bad vertex buffer type.
				return X_ERR_INVALID_PARAMS;
				break;
			}
*/
			m_numVertices = numVertices;
			m_primitiveType = primType;
			//m_vbType = vbType;

			return X_OK;

}

	
	
//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::destroy()
{
	//Release the vertex buffers.
	SAFE_RELEASE(m_pPositionsVB);

	for (XU32 i = 0;i < m_desc.numColors;++i)
		SAFE_RELEASE(m_pColorsVB[i]);
	XDeleteArray(m_pColorsVB);
	XDeleteArray(m_pColors);

	for (XU32 i = 0;i < m_desc.numNormals;++i)
		SAFE_RELEASE(m_pNormalsVB[i]);
	XDeleteArray(m_pNormalsVB);
	XDeleteArray(m_pNormals);

	for (XU32 i = 0;i < m_desc.numUVs;++i)
		SAFE_RELEASE(m_pUVsVB[i]);
	XDeleteArray(m_pUVsVB);
	XDeleteArray(m_pUVs);

	for (XU32 i = 0;i < m_desc.numTangents;++i)
		SAFE_RELEASE(m_pTangentsVB[i]);
	XDeleteArray(m_pTangentsVB);
	XDeleteArray(m_pTangents);

	for (XU32 i = 0;i < m_desc.numBinormals;++i)
		SAFE_RELEASE(m_pBinormalsVB[i]);
	XDeleteArray(m_pBinormalsVB);
	XDeleteArray(m_pBinormals);

	/*for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)
		SAFE_RELEASE(m_pBlendIndicesVB[i]);
	XDeleteArray(m_pBlendIndicesVB);
	XDeleteArray(m_pBlendIndices);

	for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)
		SAFE_RELEASE(m_pBlendWeightsVB[i]);
	XDeleteArray(m_pBlendWeightsVB);
	XDeleteArray(m_pBlendWeights);*/

	XResource::destroy();

	return X_OK;
}
	


//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::lockAllStreams(X_LOCK_TYPE lockType)
{
	//We can't lock if the vertex buffer isn't dynamic
	X_ASSERTM(m_dynamic,"Vertex buffer was not specified to by dynamic when it was created so it cannot be locked/unlocked");

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

	//Lock buffers
	HRESULT hr;
	hr = m_pPositionsVB->Map(mapType,0,(void**)&m_pPositions);
	X_ASSERTM(hr == S_OK,"Could not map vertex buffer.");

	for (XU32 i = 0;i < m_desc.numColors;++i)
		hr = m_pColorsVB[i]->Map(mapType,0,(void**)&m_pColors[i]);
	for (XU32 i = 0;i < m_desc.numNormals;++i)
		hr = m_pNormalsVB[i]->Map(mapType,0,(void**)&m_pNormals[i]);
	for (XU32 i = 0;i < m_desc.numUVs;++i)
		hr = m_pUVsVB[i]->Map(mapType,0,(void**)&m_pUVs[i]);
	for (XU32 i = 0;i < m_desc.numTangents;++i)
		hr = m_pTangentsVB[i]->Map(mapType,0,(void**)&m_pTangents[i]);
	for (XU32 i = 0;i < m_desc.numBinormals;++i)
		hr = m_pBinormalsVB[i]->Map(mapType,0,(void**)&m_pBinormals[i]);
	/*for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)
		hr = m_pBlendIndicesVB[i]->Map(mapType,0,(void**)&m_pBlendIndices[i]);
	for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)
		hr = m_pBlendWeightsVB[i]->Map(mapType,0,(void**)&m_pBlendWeights[i]);*/

	return X_OK;
/*
	void* pVertices;
	
	if (m_vb->Lock(0,0,(void**)&pVertices,lockFlags) == D3D_OK)
		return (void*)pVertices;
	else 
		return NULL;*/
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::unlockAllStreams()
{
	//We can't lock if the vertex buffer isn't dynamic
	X_ASSERTM(m_dynamic,"Vertex buffer was not specified to by dynamic when it was created so it cannot be locked/unlocked");

	//Unlock buffers
	m_pPositionsVB->Unmap();
	m_pPositions = NULL;

	for (XU32 i = 0;i < m_desc.numColors;++i)
	{
		m_pColorsVB[i]->Unmap();
		m_pColors[i] = NULL;
	}
	for (XU32 i = 0;i < m_desc.numNormals;++i)
	{
		m_pNormalsVB[i]->Unmap();
		m_pNormals[i] = NULL;
	}
	for (XU32 i = 0;i < m_desc.numUVs;++i)
	{
		m_pUVsVB[i]->Unmap();
		m_pUVs[i] = NULL;
	}
	for (XU32 i = 0;i < m_desc.numTangents;++i)
	{
		m_pTangentsVB[i]->Unmap();
		m_pTangents[i] = NULL;
	}
	for (XU32 i = 0;i < m_desc.numBinormals;++i)
	{
		m_pBinormalsVB[i]->Unmap();
		m_pBinormals[i] = NULL;
	}
	/*for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)
	{
		m_pBlendIndicesVB[i]->Unmap();
		m_pBlendIndices[i] = NULL;
	}
	for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)
	{
		m_pBlendWeightsVB[i]->Unmap();
		m_pBlendWeights[i] = NULL;
	}*/

	return X_OK;
	/*
	if ((m_vb->Unlock()) != D3D_OK)
		return X_ERR_OPERATION_FAILED;
	else
		return X_OK;*/
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::lockStream(X_SEMANTIC_TYPE semantic,XU32 streamIndex,X_LOCK_TYPE lockType)
{
	//We can't lock if the vertex buffer isn't dynamic
	X_ASSERTM(m_dynamic,"Vertex buffer was not specified to by dynamic when it was created so it cannot be locked/unlocked");

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

	//Lock buffer
	HRESULT hr;
	switch (semantic)
	{
	case X_ST_POSITION:
		X_ASSERT(streamIndex == 0);
		hr = m_pPositionsVB->Map(mapType,0,(void**)&m_pPositions);
		break;
	case X_ST_COLOR:
		X_ASSERT(streamIndex < m_desc.numColors);
		hr = m_pColorsVB[streamIndex]->Map(mapType,0,(void**)&m_pColors[streamIndex]);
		break;
	case X_ST_NORMAL:
		X_ASSERT(streamIndex < m_desc.numNormals);
		hr = m_pNormalsVB[streamIndex]->Map(mapType,0,(void**)&m_pNormals[streamIndex]);
		break;
	case X_ST_UV:
		X_ASSERT(streamIndex < m_desc.numUVs);
		hr = m_pUVsVB[streamIndex]->Map(mapType,0,(void**)&m_pUVs[streamIndex]);
		break;
	case X_ST_TANGENT:
		X_ASSERT(streamIndex < m_desc.numTangents);
		hr = m_pTangentsVB[streamIndex]->Map(mapType,0,(void**)&m_pTangents[streamIndex]);
		break;
	case X_ST_BINORMAL:
		X_ASSERT(streamIndex < m_desc.numBinormals);
		hr = m_pBinormalsVB[streamIndex]->Map(mapType,0,(void**)&m_pBinormals[streamIndex]);
		break;
	}
	X_ASSERTM(hr == S_OK,"Could not map vertex buffer.");

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::unlockStream(X_SEMANTIC_TYPE semantic,XU32 streamIndex)
{
	//We can't unlock if the vertex buffer isn't dynamic
	X_ASSERTM(m_dynamic,"Vertex buffer was not specified to by dynamic when it was created so it cannot be locked/unlocked");

	//Unlock buffer
	switch (semantic)
	{
	case X_ST_POSITION:
		X_ASSERT(streamIndex == 0);
		m_pPositionsVB->Unmap(); 
		m_pPositions = NULL;
		break;
	case X_ST_COLOR:
		X_ASSERT(streamIndex < m_desc.numColors);
		m_pColorsVB[streamIndex]->Unmap();
		m_pColors[streamIndex] = NULL;
		break;
	case X_ST_NORMAL:
		X_ASSERT(streamIndex < m_desc.numNormals);
		m_pNormalsVB[streamIndex]->Unmap();
		m_pNormals[streamIndex] = NULL;
		break;
	case X_ST_UV:
		X_ASSERT(streamIndex < m_desc.numUVs);
		m_pUVsVB[streamIndex]->Unmap();
		m_pUVs[streamIndex] = NULL;
		break;
	case X_ST_TANGENT:
		X_ASSERT(streamIndex < m_desc.numTangents);
		m_pTangentsVB[streamIndex]->Unmap();
		m_pTangents[streamIndex] = NULL;
		break;
	case X_ST_BINORMAL:
		X_ASSERT(streamIndex < m_desc.numBinormals);
		 m_pBinormalsVB[streamIndex]->Unmap();
		m_pBinormals[streamIndex] = NULL;
		break;
	}

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XU32 XVertexBuffer::getNumVerts()
{
	return m_numVertices;
}


//*****************************************************************************
//*****************************************************************************
X_PRIMITIVE_TYPE XVertexBuffer::getPrimType()
{
	return m_primitiveType;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::render(XU32 primCount)
{
	X_ASSERTM(primCount != 0,"Cannot render 0 primitives.");

#ifdef XVP_EDITOR
	XBOOL forcePink = !verifyInputSignature();
	XHandle oldVS = X_INVALID_HANDLE;
	XHandle oldPS = X_INVALID_HANDLE;
	if (forcePink)
	{
		oldVS = XVP::GraphicsMgr.getBoundVertexShader();
		oldPS = XVP::GraphicsMgr.getBoundPixelShader();
		XVP::GraphicsMgr.setVertexShader(XVP::GraphicsMgr.getVertexShaderByName("Pink.fx"));
		XVP::GraphicsMgr.setPixelShader(XVP::GraphicsMgr.getPixelShaderByName("Pink.fx"));
	}
#endif

	XU32 stream = 0;
		
	//Set vertex declaration
	XU32 stride,offset = 0;
	XVP::GraphicsMgr.m_pD3DDevice->IASetInputLayout(m_pVertexDeclaration->getDeclaration());

	//Set streams
	if (m_pPositionsVB)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pPositionsVB,&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numColors;++i)
	{
		stride = sizeof(XU32);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pColorsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numNormals;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pNormalsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numUVs;++i)
	{
		stride = sizeof(XVector2D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pUVsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numTangents;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pTangentsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numBinormals;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBinormalsVB[i],&stride,&offset);
	}
	/*for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)
	{
		stride = sizeof(XVector4D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBlendIndicesVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)
	{
		stride = sizeof(XVector4D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBlendWeightsVB[i],&stride,&offset);
	}*/

	//Okay, switch based on the type of primitive to render
	switch (m_primitiveType)
	{	
		case X_PT_POINT_LIST:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(primCount,0);
			break;
		case X_PT_LINE_LIST:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(primCount*2,0);
			break;
		case X_PT_LINE_STRIP:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(primCount+1,0);
			break;
		case X_PT_TRIANGLE_STRIP:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(primCount+2,0);
			break;
		case X_PT_TRIANGLE_LIST:	
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(primCount*3,0);
			break;
	}

#ifdef XVP_EDITOR
	if (forcePink)
	{
		XVP::GraphicsMgr.setVertexShader(oldVS);
		XVP::GraphicsMgr.setPixelShader(oldPS);
	}
#endif

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::render()
{
#ifdef XVP_EDITOR
	XBOOL forcePink = !verifyInputSignature();
	XHandle oldVS = X_INVALID_HANDLE;
	XHandle oldPS = X_INVALID_HANDLE;
	if (forcePink)
	{
		oldVS = XVP::GraphicsMgr.getBoundVertexShader();
		oldPS = XVP::GraphicsMgr.getBoundPixelShader();
		XVP::GraphicsMgr.setVertexShader(XVP::GraphicsMgr.getVertexShaderByName("Pink.fx"));
		XVP::GraphicsMgr.setPixelShader(XVP::GraphicsMgr.getPixelShaderByName("Pink.fx"));
	}
#endif


	XU32 stream = 0;
	XU32 stride,offset = 0;

	//Set vertex declaration
	XVP::GraphicsMgr.m_pD3DDevice->IASetInputLayout(m_pVertexDeclaration->getDeclaration());

	//Set streams
	if (m_pPositionsVB)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pPositionsVB,&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numColors;++i)
	{
		stride = sizeof(XU32);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pColorsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numNormals;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pNormalsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numUVs;++i)
	{
		stride = sizeof(XVector2D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pUVsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numTangents;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pTangentsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numBinormals;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBinormalsVB[i],&stride,&offset);
	}
	/*for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)
	{
		stride = sizeof(XVector4D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBlendIndicesVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)
	{
		stride = sizeof(XVector4D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBlendWeightsVB[i],&stride,&offset);
	}*/

	//Okay, switch based on the type of primitive to render
	switch (m_primitiveType)
	{	
		case X_PT_POINT_LIST:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(m_numVertices,0);
			break;
		case X_PT_LINE_LIST:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(m_numVertices,0);
			break;
		case X_PT_LINE_STRIP:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(m_numVertices,0);
			break;
		case X_PT_TRIANGLE_STRIP:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(m_numVertices,0);
			break;
		case X_PT_TRIANGLE_LIST:	
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			XVP::GraphicsMgr.m_pD3DDevice->Draw(m_numVertices,0);
			break;
	}

#ifdef XVP_EDITOR
	if (forcePink)
	{
		XVP::GraphicsMgr.setVertexShader(oldVS);
		XVP::GraphicsMgr.setPixelShader(oldPS);
	}
#endif

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::renderWithIndexBuffer(XBaseIndexBuffer* pIndexBuffer)
{
#ifdef XVP_EDITOR
	XBOOL forcePink = !verifyInputSignature();
	XHandle oldVS = X_INVALID_HANDLE;
	XHandle oldPS = X_INVALID_HANDLE;
	if (forcePink)
	{
		oldVS = XVP::GraphicsMgr.getBoundVertexShader();
		oldPS = XVP::GraphicsMgr.getBoundPixelShader();
		XVP::GraphicsMgr.setVertexShader(XVP::GraphicsMgr.getVertexShaderByName("Pink.fx"));
		XVP::GraphicsMgr.setPixelShader(XVP::GraphicsMgr.getPixelShaderByName("Pink.fx"));
	}
#endif


	XU32 stream = 0;
	XU32 stride,offset = 0;

	//Set vertex declaration
	XVP::GraphicsMgr.m_pD3DDevice->IASetInputLayout(m_pVertexDeclaration->getDeclaration());

	//Set streams
	if (m_pPositionsVB)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pPositionsVB,&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numColors;++i)
	{
		stride = sizeof(XU32);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pColorsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numNormals;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pNormalsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numUVs;++i)
	{
		stride = sizeof(XVector2D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pUVsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numTangents;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pTangentsVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numBinormals;++i)
	{
		stride = sizeof(XVector3D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBinormalsVB[i],&stride,&offset);
	}
	/*for (XU32 i = 0;i < m_desc.numBoneIndices4Tuplets;++i)
	{
		stride = sizeof(XVector4D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBlendIndicesVB[i],&stride,&offset);
	}
	for (XU32 i = 0;i < m_desc.numBoneWeights4Tuplets;++i)
	{
		stride = sizeof(XVector4D);
		XVP::GraphicsMgr.m_pD3DDevice->IASetVertexBuffers(stream++,1,&m_pBlendWeightsVB[i],&stride,&offset);
	}*/
	/*
	//Switch based on the type of vertex buffer.
	HRESULT hr;
	switch (m_vbType)
	{
	case X_VT_RHW:
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pRHWVertexDcl);
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetStreamSource(0,m_vb,0,
										 sizeof(XRHWVertex));
		break;
	case X_VT_PLAIN:
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pPlainVertexDcl);
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetStreamSource(0,m_vb,0,
										 sizeof(XPlainVertex));
		break;
	case X_VT_MODEL:
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pModelVertexDcl);
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetStreamSource(0,m_vb,0,
										 sizeof(XModelVertex));
		break;
	case X_VT_SKIN:
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pSkinVertexDcl);
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetStreamSource(0,m_vb,0,
										 sizeof(XSkinVertex));
		break;
	case X_VT_PARTICLE:
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pParticleVertexDcl);
		hr = XVP::GraphicsMgr.m_pD3DDevice->SetStreamSource(0,m_vb,0,
										 sizeof(XParticleVertex));
		break;
	default:
		break;
	}
	*/
	if (pIndexBuffer->setToRenderingDevice() != X_OK)
		return X_ERR_OPERATION_FAILED;
			
	
	//Okay, switch based on the type of primitive to render
	switch (m_primitiveType)
	{	
		case X_PT_POINT_LIST:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
			XVP::GraphicsMgr.m_pD3DDevice->DrawIndexed(pIndexBuffer->getLength(),0,0);
			break;
		case X_PT_LINE_LIST:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
			XVP::GraphicsMgr.m_pD3DDevice->DrawIndexed(pIndexBuffer->getLength(),0,0);
			break;
		case X_PT_LINE_STRIP:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
			XVP::GraphicsMgr.m_pD3DDevice->DrawIndexed(pIndexBuffer->getLength(),0,0);
			break;
		case X_PT_TRIANGLE_STRIP:
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			XVP::GraphicsMgr.m_pD3DDevice->DrawIndexed(pIndexBuffer->getLength(),0,0);
			break;
		case X_PT_TRIANGLE_LIST:	
			XVP::GraphicsMgr.m_pD3DDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			XVP::GraphicsMgr.m_pD3DDevice->DrawIndexed(pIndexBuffer->getLength(),0,0);
			break;
	}

#ifdef XVP_EDITOR
	if (forcePink)
	{
		XVP::GraphicsMgr.setVertexShader(oldVS);
		XVP::GraphicsMgr.setPixelShader(oldPS);
	}
#endif

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::renderUserVertexBuffer(X_PRIMITIVE_TYPE pType,
											  XHandle vertexDeclaration,
											XU32 numPrimitives,void* pPrimitiveArray,
											XU32 primitiveStride)
{
#if OLD
	X_ASSERTM(pPrimitiveArray,"Primitive array is null");
	
	HRESULT hr;

	XVertexDeclaration* pVD = XVP::GraphicsMgr.getVertexDeclarationInterface(vertexDeclaration);
	XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pVD->getDeclaration());

	//Okay, switch based on the type of primitive to render
	switch (pType)
	{
		case X_PT_POINT_LIST:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,numPrimitives,
											pPrimitiveArray,primitiveStride);
			break;
		case X_PT_LINE_LIST:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,numPrimitives,
											pPrimitiveArray,primitiveStride);
			break;
		case X_PT_LINE_STRIP:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP,numPrimitives,
											pPrimitiveArray,primitiveStride);
			break;
		case X_PT_TRIANGLE_LIST:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,numPrimitives,
											pPrimitiveArray,primitiveStride);
			break;
		case X_PT_TRIANGLE_STRIP:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,numPrimitives,
											pPrimitiveArray,primitiveStride);
			break;
		case X_PT_TRIANGLE_FAN:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,numPrimitives,
											pPrimitiveArray,primitiveStride);
			break;
		default:
			//Bad vertex buffer type.
			return X_ERR_INVALID_PARAMS;
			break;
	}

	if (hr != D3D_OK)
		return X_ERR_OPERATION_FAILED;
	else
		return X_OK;
#else
	return X_OK;
#endif
}

/*

//*****************************************************************************
//*****************************************************************************
XStatus XVertexBuffer::renderUserIndexedVertexBuffer(X_PRIMITIVE_TYPE pType,X_VERTEX_TYPE vType,
									XU32 numVertices,void* pVertices,XU32 numIndices,void* pIndices,
									XU32 numPrimitives,	XU32 vertexStride)
{
	if ((!pVertices) || (!pIndices))
		return X_ERR_INVALID_PARAMS;

	HRESULT hr;

	//Switch based on the type of vertex buffer.
	switch (vType)
	{
	case X_VT_RHW:
		XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pRHWVertexDcl);		
	break;
	case X_VT_PLAIN:
		XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pPlainVertexDcl);		
	break;
	case X_VT_MODEL:
		XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pModelVertexDcl);		
	break;
	case X_VT_SKIN:
		XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pSkinVertexDcl);	
	break;
	case X_VT_PARTICLE:
		XVP::GraphicsMgr.m_pD3DDevice->SetVertexDeclaration(pParticleVertexDcl);	
	break;
	default:
		break;
	}

	//Okay, switch based on the type of primitive to render
	switch (pType)
	{
		case X_PT_POINT_LIST:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_POINTLIST,0,numVertices,numPrimitives,
											pIndices,D3DFMT_INDEX16,pVertices,vertexStride);
			break;
		case X_PT_LINE_LIST:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST,0,numVertices,numPrimitives,
											pIndices,D3DFMT_INDEX16,pVertices,vertexStride);
			break;
		case X_PT_LINE_STRIP:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP,0,numVertices,numPrimitives,
											pIndices,D3DFMT_INDEX16,pVertices,vertexStride);
			break;
		case X_PT_TRIANGLE_LIST:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,numVertices,numPrimitives,
											pIndices,D3DFMT_INDEX16,pVertices,vertexStride);
			break;
		case X_PT_TRIANGLE_STRIP:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLESTRIP,0,numVertices,numPrimitives,
											pIndices,D3DFMT_INDEX16,pVertices,vertexStride);
			break;
		case X_PT_TRIANGLE_FAN:
			hr = XVP::GraphicsMgr.m_pD3DDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN,0,numVertices,numPrimitives,
											pIndices,D3DFMT_INDEX16,pVertices,vertexStride);
			break;
		default:
			//Bad vertex buffer type.
			return X_ERR_INVALID_PARAMS;
			break;
	}

	if (hr != D3D_OK)
		return X_ERR_OPERATION_FAILED;
	else
		return X_OK;
}

*/


	
	
XVertexDesc* XVertexBuffer::getVertexDesc()
{
	return &m_desc;
}
	
	
	
	

//*****************************************************************************
//*****************************************************************************
XVector3D* XVertexBuffer::getPositionStream()
{
	X_ASSERTM(m_pPositions,"Position stream must be locked");
	return m_pPositions;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setPosition(XU32 vertIndex,XVector3D& pos)
{
	X_ASSERTM(m_pPositions,"Position stream must be locked");
	m_pPositions[vertIndex] = pos;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setPosition(XU32 vertIndex,XF32 x,XF32 y,XF32 z)
{	
	X_ASSERTM(m_pPositions,"Position stream must be locked"); 
	m_pPositions[vertIndex].x = x;
	m_pPositions[vertIndex].y = y;
	m_pPositions[vertIndex].z = z;
} 
	


void XVertexBuffer::setPositionData(void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pPositionsVB,
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XVector3D* XVertexBuffer::getNormalStream(XU32 streamIndex)
{
	X_ASSERTM(m_pNormals,"Normal stream must be locked");
	if (m_pNormals)
	{
		if (streamIndex < m_desc.numNormals)
			return m_pNormals[streamIndex];
	}
	
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setNormal(XU32 vertIndex,XU32 streamIndex,XVector3D& normal)
{	
	X_ASSERTM(m_pNormals,"Normal stream must be locked");
	m_pNormals[streamIndex][vertIndex] = normal;
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setNormal(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z)
{	
	X_ASSERTM(m_pNormals,"Normal stream must be locked");
	m_pNormals[streamIndex][vertIndex].x = x;
	m_pNormals[streamIndex][vertIndex].y = y;
	m_pNormals[streamIndex][vertIndex].z = z;
} 



void XVertexBuffer::setNormalData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pNormalsVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XU32* XVertexBuffer::getColorStream(XU32 streamIndex)
{
	X_ASSERTM(m_pColors,"Color stream must be locked");
	if (m_pColors)
	{
		if (streamIndex < m_desc.numColors)
			return m_pColors[streamIndex];
	}
	
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setColor(XU32 vertIndex,XU32 streamIndex,XU8 r,XU8 g,XU8 b,XU8 a)
{	
	X_ASSERTM(m_pColors,"Color stream must be locked");
	m_pColors[streamIndex][vertIndex] = ((XU32)((((XU32(a))&0xff)<<24)|
												(((XU32(r))&0xff)<<16)|
												(((XU32(g))&0xff)<<8)|
												((XU32(b))&0xff)));
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setColor(XU32 vertIndex,XU32 streamIndex,XColor& color)
{	
	X_ASSERTM(m_pColors,"Color stream must be locked");
	m_pColors[streamIndex][vertIndex] = color.getABGR32Bit();
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setColor(XU32 vertIndex,XU32 streamIndex,XU32 ABGR)
{	
	X_ASSERTM(m_pColors,"Color stream must be locked");
	m_pColors[streamIndex][vertIndex] = ABGR;
} 



void XVertexBuffer::setColorData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pColorsVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XVector2D* XVertexBuffer::getUVStream(XU32 streamIndex)
{
	X_ASSERTM(m_pUVs,"UV stream must be locked");
	if (m_pUVs)
	{
		if (streamIndex < m_desc.numUVs)
			return m_pUVs[streamIndex];
	}
	
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setUV(XU32 vertIndex,XU32 streamIndex,XVector2D& uv)
{	
	X_ASSERTM(m_pUVs,"UV stream must be locked");
	m_pUVs[streamIndex][vertIndex] = uv;
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setUV(XU32 vertIndex,XU32 streamIndex,XF32 u,XF32 v)
{	
	X_ASSERTM(m_pUVs,"UV stream must be locked");
	m_pUVs[streamIndex][vertIndex].x = u;
	m_pUVs[streamIndex][vertIndex].y = v;
} 



void XVertexBuffer::setUVData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pUVsVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XVector3D* XVertexBuffer::getTangentStream(XU32 streamIndex)
{
	X_ASSERTM(m_pTangents,"Tangent stream must be locked");
	if (m_pTangents)
	{
		if (streamIndex < m_desc.numTangents)
			return m_pTangents[streamIndex];
	}

	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setTangent(XU32 vertIndex,XU32 streamIndex,XVector3D& tangent)
{	
	X_ASSERTM(m_pTangents,"Tangent stream must be locked");
	m_pTangents[streamIndex][vertIndex] = tangent;
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setTangent(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z)
{	
	X_ASSERTM(m_pTangents,"Tangent stream must be locked");
	m_pTangents[streamIndex][vertIndex].x = x;
	m_pTangents[streamIndex][vertIndex].y = y;
	m_pTangents[streamIndex][vertIndex].z = z;
} 
	


void XVertexBuffer::setTangentData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pTangentsVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XVector3D* XVertexBuffer::getBinormalStream(XU32 streamIndex)
{
	X_ASSERTM(m_pBinormals,"Binormal stream must be locked");
	if (m_pBinormals)
	{
		if (streamIndex < m_desc.numBinormals)
			return m_pBinormals[streamIndex];
	}
	
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setBinormal(XU32 vertIndex,XU32 streamIndex,XVector3D& binormal)
{	
	X_ASSERTM(m_pBinormals,"Binormal stream must be locked");
	m_pBinormals[streamIndex][vertIndex] = binormal;
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setBinormal(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z)
{	
	X_ASSERTM(m_pBinormals,"Binormal stream must be locked");
	m_pBinormals[streamIndex][vertIndex].x = x;
	m_pBinormals[streamIndex][vertIndex].y = y;
	m_pBinormals[streamIndex][vertIndex].z = z;
} 
	


void XVertexBuffer::setBinormalData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pBinormalsVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XVector4D* XVertexBuffer::getBlendIndicesStream(XU32 streamIndex)
{
	X_ASSERTM(m_pBlendIndices,"Blend index stream must be locked");
	if (m_pBlendIndices)
	{
		if (streamIndex < m_desc.numBoneIndices4Tuplets)
			return m_pBlendIndices[streamIndex];
	}
	
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setBlendIndices(XU32 vertIndex,XU32 streamIndex,XVector4D& indices)
{	
	X_ASSERTM(m_pBlendIndices,"Blend index stream must be locked");
	m_pBlendIndices[streamIndex][vertIndex] = indices;
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setBlendIndices(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z,XF32 w)
{	
	X_ASSERTM(m_pBlendIndices,"Blend index stream must be locked");
	m_pBlendIndices[streamIndex][vertIndex].x = x;
	m_pBlendIndices[streamIndex][vertIndex].y = y;
	m_pBlendIndices[streamIndex][vertIndex].z = z;
	m_pBlendIndices[streamIndex][vertIndex].w = w;
} 
	


void XVertexBuffer::setBlendIndicesData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pBlendIndicesVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}



//*****************************************************************************
//*****************************************************************************
XVector4D* XVertexBuffer::getBlendWeightsStream(XU32 streamIndex)
{
	X_ASSERTM(m_pBlendWeights,"Blend weight stream must be locked");
	if (m_pBlendWeights)
	{
		if (streamIndex < m_desc.numBoneWeights4Tuplets)
			return m_pBlendWeights[streamIndex];
	}
	
	return NULL;
}



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setBlendWeights(XU32 vertIndex,XU32 streamIndex,XVector4D& weights)
{	
	X_ASSERTM(m_pBlendWeights,"Blend weight stream must be locked");
	m_pBlendWeights[streamIndex][vertIndex] = weights;
} 



//*****************************************************************************
//*****************************************************************************
void XVertexBuffer::setBlendWeights(XU32 vertIndex,XU32 streamIndex,XF32 x,XF32 y,XF32 z,XF32 w)
{	
	X_ASSERTM(m_pBlendWeights,"Blend weight stream must be locked");
	m_pBlendWeights[streamIndex][vertIndex].x = x;
	m_pBlendWeights[streamIndex][vertIndex].y = y;
	m_pBlendWeights[streamIndex][vertIndex].z = z;
	m_pBlendWeights[streamIndex][vertIndex].w = w;
} 


void XVertexBuffer::setBlendWeightsData(XU32 streamIndex,void* pData,XU32 offset,XU32 dataSizeInBytes)
{
	X_ASSERTM(!m_dynamic,"Vertex buffer was specified to by dynamic when it was created so it must be locked before modification");
	X_ASSERTM(pData,"Data cannot be null");
	X_ASSERTM(dataSizeInBytes,"Data size cannot be 0");

	D3D10_BOX dstBox;
	dstBox.left = offset;
	dstBox.top = 0;
	dstBox.right = offset + dataSizeInBytes;
	dstBox.bottom = 1;
	dstBox.front = 0;
	dstBox.back = 1;
	XVP::GraphicsMgr.m_pD3DDevice->UpdateSubresource(m_pBlendWeightsVB[streamIndex],
													 D3D10CalcSubresource(0,0,1),
													 &dstBox,
													 pData,
													 dataSizeInBytes,
													 dataSizeInBytes);
}

#ifdef XVP_EDITOR
XBOOL XVertexBuffer::verifyInputSignature()
{
	XHandle vsHandle = XVP::GraphicsMgr.getBoundVertexShader();
	if (vsHandle != X_INVALID_HANDLE)
	{
		XBaseVertexShader* pVS = XVP::GraphicsMgr.getVertexShaderByHandle(vsHandle);
		ID3D10ShaderReflection* pSR = (ID3D10ShaderReflection*)pVS->getShaderReflection();

		D3D10_SHADER_DESC desc;
		D3D10_SIGNATURE_PARAMETER_DESC paramDesc;
		D3D10_INPUT_ELEMENT_DESC* pInputDesc = m_pVertexDeclaration->getInputDesc();
		pSR->GetDesc( &desc );

		for (XU32 i = 0;i < desc.InputParameters;++i)
		{
			pSR->GetInputParameterDesc(i,&paramDesc);
			
			XBOOL foundSemantic = false;
			for (XU32 j = 0;j < m_pVertexDeclaration->getNumStreams();++j)
				if (strcmp(paramDesc.SemanticName,pInputDesc[j].SemanticName) == 0)
				{
					foundSemantic = true;
					break;
				}
			
			if (!foundSemantic)
			{
				X_ASSERTF(foundSemantic,"Shader signature mismatch.  Vertex is missing %s semantic.  Switching to pink shader.",paramDesc.SemanticName);
				return false;
			}
			/*if (paramDesc.SemanticIndex != pInputDesc[i].SemanticIndex)
			{
				//X_ASSERTF(0,"Shader signature mismatch.  Semantic order does not match for semantic index %d.  Switching to pink shader.",pInputDesc[i].SemanticIndex);
				//return false;
			}
			if (strcmp(paramDesc.SemanticName,pInputDesc[i].SemanticName) != 0)
			{
			//	X_ASSERTF(0,"Shader signature mismatch.  Vertex is missing %s semantic.  Switching to pink shader.",pInputDesc[i].SemanticName);
			//	return false;
			}*/
		}
	}
	
	return true;
}
#endif

