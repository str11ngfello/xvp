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
//	Module Name:	XHLSLPixelShader.cpp
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
XHLSLPixelShader::XHLSLPixelShader()
{
}



//*****************************************************************************
//*****************************************************************************
XHLSLPixelShader::~XHLSLPixelShader()
{
}


//*****************************************************************************
//*****************************************************************************
XStatus XHLSLPixelShader::initFromSourceCode(XS8* pFilename,XS8* entryFunctionName)
{
	HRESULT hr;

    // Create pixel shader
    ID3D10Blob* pCode = NULL;     
    ID3D10Blob* pErrors = NULL;     

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
    /*#ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif
*/
    // Assemble the vertex shader from the file
    hr = D3DX10CompileFromFile(pFilename,
								NULL,
								NULL,
								entryFunctionName,
                                D3D10GetPixelShaderProfile(XVP::GraphicsMgr.m_pD3DDevice),
								dwShaderFlags,
								0,
								NULL,
								&pCode,
                                &pErrors,
								&hr);

	X_ASSERTM(pErrors == NULL,(XS8*)pErrors->GetBufferPointer())
	X_ASSERTF(hr == S_OK,"Failed to compile pixel shader %s",pFilename)

	
    // Create the pixel shader
    hr = XVP::GraphicsMgr.m_pD3DDevice->CreatePixelShader((void*)pCode->GetBufferPointer(),
															pCode->GetBufferSize(),
															&m_pPixelShader );
	X_ASSERTM(hr == S_OK,"Could not create pixel shader from byte code.");

    SAFE_RELEASE(pCode);
    SAFE_RELEASE(pErrors);
	
    return X_OK;
}


//*****************************************************************************
//*****************************************************************************
XStatus XHLSLPixelShader::initFromObjectCode(void* pObjectCode,XU32 objectCodeSize)
{
	HRESULT hr;

    //Create the pixel shader and get the signature
	hr = XVP::GraphicsMgr.m_pD3DDevice->CreatePixelShader(pObjectCode,
															objectCodeSize,
															&m_pPixelShader);
	X_ASSERTM(hr == S_OK,"Could not create pixel shader from object code.");
    
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XHLSLPixelShader::destroy()
{
	SAFE_RELEASE(m_pPixelShader);
	XResource::destroy();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
ID3D10PixelShader* XHLSLPixelShader::getD3DPixelShader()
{
	return m_pPixelShader;
}

