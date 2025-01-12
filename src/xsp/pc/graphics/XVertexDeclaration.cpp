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
//	Module Name:	XVertexDeclaration.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-8-2006
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



//*****************************************************************************
//*****************************************************************************
XVertexDeclaration::XVertexDeclaration()
{
	m_pInputLayout = NULL;
	m_numStreams = 0;
}



//*****************************************************************************
//*****************************************************************************
XVertexDeclaration::~XVertexDeclaration()
{
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexDeclaration::init(XVertexDesc& desc,XS8* pShaderName)
{
	D3D10_INPUT_ELEMENT_DESC declaration[X_MAX_NUMBER_OF_STREAMS]; 
	XU32 currentStream = 0;

	//Position
	declaration[currentStream].SemanticName = "POSITION"; 
	declaration[currentStream].SemanticIndex = 0; 
	declaration[currentStream].Format = DXGI_FORMAT_R32G32B32_FLOAT; 
	declaration[currentStream].InputSlot = currentStream; 
	declaration[currentStream].AlignedByteOffset = 0; 
	declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
	declaration[currentStream].InstanceDataStepRate = 0;
#ifdef XVP_EDITOR
	memcpy(&m_inputDesc[currentStream],&declaration[currentStream],sizeof(D3D10_INPUT_ELEMENT_DESC));
	//m_inputDesc[currentStream].SemanticName = XOSMgr::createString("POSITION");
#endif
	currentStream++; 

	//Colors 
	for (XU32 i = 0;i < desc.numColors;++i)
	{
		declaration[currentStream].SemanticName = "COLOR"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
#ifdef XVP_EDITOR
	memcpy(&m_inputDesc[currentStream],&declaration[currentStream],sizeof(D3D10_INPUT_ELEMENT_DESC));
	//m_inputDesc[currentStream].SemanticName = XOSMgr::createString("COLOR");
#endif
		currentStream++; 
	}

	//Normals 
	for (XU32 i = 0;i < desc.numNormals;++i)
	{
		declaration[currentStream].SemanticName = "NORMAL"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R32G32B32_FLOAT; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
#ifdef XVP_EDITOR
	memcpy(&m_inputDesc[currentStream],&declaration[currentStream],sizeof(D3D10_INPUT_ELEMENT_DESC));
	//m_inputDesc[currentStream].SemanticName = XOSMgr::createString("NORMAL");
#endif
		currentStream++; 
	}

	//UVs 
	for (XU32 i = 0;i < desc.numUVs;++i)
	{
		declaration[currentStream].SemanticName = "TEXCOORD"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R32G32_FLOAT; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
#ifdef XVP_EDITOR
	memcpy(&m_inputDesc[currentStream],&declaration[currentStream],sizeof(D3D10_INPUT_ELEMENT_DESC));
	//m_inputDesc[currentStream].SemanticName = XOSMgr::createString("TEXCOORD");
#endif
		currentStream++;  
	}

	
	//Tangents 
	for (XU32 i = 0;i < desc.numTangents;++i)
	{
		declaration[currentStream].SemanticName = "TANGENT"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R32G32B32_FLOAT; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
#ifdef XVP_EDITOR
	memcpy(&m_inputDesc[currentStream],&declaration[currentStream],sizeof(D3D10_INPUT_ELEMENT_DESC));
	//m_inputDesc[currentStream].SemanticName = XOSMgr::createString("TANGENT");
#endif
		currentStream++; 
	}

	//Binormals 
	for (XU32 i = 0;i < desc.numBinormals;++i)
	{
		declaration[currentStream].SemanticName = "BINORMAL"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R32G32B32_FLOAT; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
#ifdef XVP_EDITOR
	memcpy(&m_inputDesc[currentStream],&declaration[currentStream],sizeof(D3D10_INPUT_ELEMENT_DESC));
	//m_inputDesc[currentStream].SemanticName = XOSMgr::createString("BINORMAL");
#endif
		currentStream++; 
	} 

	//Blend Indices
	/*for (XU32 i = 0;i < desc.numBoneIndices4Tuplets;++i)
	{
		declaration[currentStream].SemanticName = "BLENDINDICES"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R8G8B8A8_UINT; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
		currentStream++; 
	} 

	//Blend Weights 
	for (XU32 i = 0;i < desc.numBoneWeights4Tuplets;++i)
	{
		declaration[currentStream].SemanticName = "BLENDWEIGHT"; 
		declaration[currentStream].SemanticIndex = i; 
		declaration[currentStream].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; 
		declaration[currentStream].InputSlot = currentStream; 
		declaration[currentStream].AlignedByteOffset = 0; 
		declaration[currentStream].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		declaration[currentStream].InstanceDataStepRate = 0;
		currentStream++; 
	} */


	
	XHandle tempHandle = XVP::GraphicsMgr.getVertexShaderByName(pShaderName);
	X_ASSERT(tempHandle != X_INVALID_HANDLE)
	XBaseVertexShader* pShader = XVP::GraphicsMgr.getVertexShaderByHandle(tempHandle);

	X_DEBUG
	(
		XS8 buf[2048];
		sprintf(buf,"Didn't find shader %s. A vertex declaration is being created before the matching shader has been created.  Shader must be created first.",pShaderName);
		X_ASSERTM(pShader,buf);
		sprintf(buf,"Creating input layout for %s.",pShaderName);
		X_DEBUG_LOG_LN(buf,X_DLL_INFO);
	)
	XVP::GraphicsMgr.m_pD3DDevice->CreateInputLayout(declaration,
													 currentStream,
													 pShader->getShaderSignature(),
													 pShader->getShaderSignatureLength(),
													 &m_pInputLayout);
	//HRESULT hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexDeclaration(declaration,&m_pDcl);

	m_numStreams = currentStream;
	//Save a copy of the description
	m_desc = desc;

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexDeclaration::destroy()
{
	SAFE_RELEASE(m_pInputLayout);
	XResource::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
ID3D10InputLayout* XVertexDeclaration::getDeclaration()
{
	return m_pInputLayout;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVertexDeclaration::getDesc(XVertexDesc& desc)
{
	desc = m_desc;
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XU32 XVertexDeclaration::getNumStreams()
{
	return m_numStreams;
}








