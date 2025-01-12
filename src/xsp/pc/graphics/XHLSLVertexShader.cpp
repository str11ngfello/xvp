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
//	Module Name:	XHLSLVertexShader.cpp
//	Author:			Lynn Duke
//	Creation Date:	1-20-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 



//*****************************************************************************
//*****************************************************************************
XHLSLVertexShader::XHLSLVertexShader()
{
	m_pSignatureBlob = NULL;

#ifdef XVP_EDITOR
	m_pReflection = NULL;
#endif
}



//*****************************************************************************
//*****************************************************************************
XHLSLVertexShader::~XHLSLVertexShader()
{
}


//*****************************************************************************
//*****************************************************************************
XStatus XHLSLVertexShader::initFromSourceCode(XS8* pFilename,XS8* entryFunctionName)
{
	HRESULT hr;

    // Create vertex shader
    ID3D10Blob* pErrors = NULL;     
    ID3D10Blob* pByteCode = NULL;     

   
    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION;
		dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION;
		dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif

    // Compile the vertex shader from the file
	hr = D3DX10CompileFromFile(pFilename,
								NULL,
								NULL,
								entryFunctionName,
                                D3D10GetVertexShaderProfile(XVP::GraphicsMgr.m_pD3DDevice),
								dwShaderFlags,
								0,
								NULL,
								&pByteCode,
                                &pErrors,
								&hr);
	X_ASSERTM(pErrors == NULL,(XS8*)pErrors->GetBufferPointer())
	X_ASSERTF(hr == S_OK,"Failed to compile vertex shader %s",pFilename)

    //Create the vertex shader and get the signature
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexShader((void*)pByteCode->GetBufferPointer(),
															pByteCode->GetBufferSize(),
															&m_pVertexShader );
	X_ASSERTM(hr == S_OK,"Could not create vertex shader from byte code.");

	hr = D3D10GetInputSignatureBlob(pByteCode->GetBufferPointer(),pByteCode->GetBufferSize(),&m_pSignatureBlob);
	X_ASSERTM(hr == S_OK,"Could not get shader signature from byte code.");
    
#ifdef XVP_EDITOR
	D3D10ReflectShader((void*)pByteCode->GetBufferPointer(),pByteCode->GetBufferSize(),&m_pReflection);
#endif

    SAFE_RELEASE(pByteCode);
    SAFE_RELEASE(pErrors);

	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XHLSLVertexShader::initFromSourceCodeString(XS8* pBuffer,XU32 bufferLen,XS8* entryFunctionName)
{
	HRESULT hr;

    // Create vertex shader
    ID3D10Blob* pErrors = NULL;     
    ID3D10Blob* pByteCode = NULL;     

   
    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION;
		dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_SKIPOPTIMIZATION;
		dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif

    // Compile the vertex shader from the buffer
	hr = D3DX10CompileFromMemory(pBuffer,
								bufferLen,
								NULL,
								NULL,
								NULL,
								entryFunctionName,
                                D3D10GetVertexShaderProfile(XVP::GraphicsMgr.m_pD3DDevice),
								dwShaderFlags,
								0,
								NULL,
								&pByteCode,
                                &pErrors,
								&hr);
	X_ASSERTM(pErrors == NULL,(XS8*)pErrors->GetBufferPointer())
	X_ASSERTF(hr == S_OK,"Failed to compile vertex shader from string")

    //Create the vertex shader and get the signature
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexShader((void*)pByteCode->GetBufferPointer(),
															pByteCode->GetBufferSize(),
															&m_pVertexShader );
	X_ASSERTM(hr == S_OK,"Could not create vertex shader from byte code.");

	hr = D3D10GetInputSignatureBlob(pByteCode->GetBufferPointer(),pByteCode->GetBufferSize(),&m_pSignatureBlob);
	X_ASSERTM(hr == S_OK,"Could not get shader signature from byte code.");
    
#ifdef XVP_EDITOR
	D3D10ReflectShader((void*)pByteCode->GetBufferPointer(),pByteCode->GetBufferSize(),&m_pReflection);
#endif

    SAFE_RELEASE(pByteCode);
    SAFE_RELEASE(pErrors);

	return X_OK;
}

//*****************************************************************************
//*****************************************************************************
XStatus XHLSLVertexShader::initFromObjectCode(void* pObjectCode,XU32 objectCodeSize)
{
	HRESULT hr;

    //Create the vertex shader and get the signature
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreateVertexShader(pObjectCode,
															objectCodeSize,
															&m_pVertexShader);
	X_ASSERTM(hr == S_OK,"Could not create vertex shader from object code.");

	hr = D3D10GetInputSignatureBlob(pObjectCode,objectCodeSize,&m_pSignatureBlob);
	X_ASSERTM(hr == S_OK,"Could not get shader signature from object code.");
    
#ifdef XVP_EDITOR
	D3D10ReflectShader(pObjectCode,objectCodeSize,&m_pReflection);
#endif

	return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XHLSLVertexShader::destroy()
{
	SAFE_RELEASE(m_pSignatureBlob);
	SAFE_RELEASE(m_pVertexShader);

#ifdef XVP_EDITOR
	SAFE_RELEASE(m_pReflection);
#endif
	XResource::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
ID3D10VertexShader* XHLSLVertexShader::getD3DVertexShader()
{
	return m_pVertexShader;
}


//*****************************************************************************
//*****************************************************************************
void* XHLSLVertexShader::getShaderSignature()
{
	return m_pSignatureBlob->GetBufferPointer();
}



//*****************************************************************************
//*****************************************************************************
XU32 XHLSLVertexShader::getShaderSignatureLength()
{
	return (XU32)m_pSignatureBlob->GetBufferSize();
}

