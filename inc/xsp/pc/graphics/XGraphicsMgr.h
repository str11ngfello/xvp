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
//	Module Name:	XGraphicsMgr.h
//	Author:			Lynn Duke
//	Creation Date:	1-20-2002
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GRAPHICS_MGR_H__
#define __X_GRAPHICS_MGR_H__

#define _WINSOCKAPI_

#include <d3d10.h>
#include <d3dx10.h>
#include <XOSMgr.h>
//#include <D3DFont.h>
//#include <DXUtil.h>
//#include <d3denumeration.h>
//#include <dshow.h>
#include <XColor.h>
#include <XRect.h>
#include <XResourceList.h>
#include <XStringMap.h>
#include <XBaseTexture.h>
#include <XMatrix4x4.h>
#include <XVPDesc.h>
#include <XLightDesc.h>
#include <XMaterialList.h>
#include <XVertexDeclaration.h>
#include <XVertexBuffer.h>
#include <XIndexBuffer.h>
#include <XMemBlockList.h>
#include <XBaseVertexShader.h>
#include <XBaseGeometryShader.h>
#include <XBasePixelShader.h>
#include <XRenderContext.h>



//*****************************************************************************
//Structs
//*****************************************************************************
struct gdriverColorInfo
{
	XU32 bpp;
	XU32 rBits;
	XU32 gBits;
	XU32 bBits;
	XU32 aBits;
};





//*****************************************************************************
//! Graphics device and resource manager.
/**
The XGraphicsMgr object represents the main graphics driver for X-VP.  The
engine manages the graphics driver which deals with aspects of graphic devices
as well as resources and data needed by the devices.  You can use the graphics
driver to render geometry to the screen, control render states, etc.

See below for more information.
*/
//*****************************************************************************
class XGraphicsMgr 
{
private:

	XFullScreenModeDesc*		m_pFullScreenModes;
	XU32						m_numFullScreenModes;
	
	//*****************************************************************************
	//Resource Lists
	//*****************************************************************************
	XMaterialList				m_materials;
	XResourceList				m_textures;
	XResourceList				m_vertexDeclarations;
	XResourceList				m_vertexBuffers;
	XResourceList				m_renderContexts;
	XResourceList				m_renderTargetArrays;
	XResourceList				m_indexBuffers;
	XResourceList				m_constantBuffers;
	XResourceList				m_vertexShaders;
	XResourceList				m_geometryShaders;
	XResourceList				m_pixelShaders;
	XResourceList				m_samplerStates;
	XResourceList				m_depthStencilStates;
	XResourceList				m_rasterizerStates;
	XResourceList				m_blendStates;
	XResourceList				m_movies;

	//*****************************************************************************
	//Driver State Information
	//*****************************************************************************
	XHeap*						m_pHeap;


	XHandle						m_vsHandleCache;
	XHandle						m_gsHandleCache;
	XHandle						m_psHandleCache;
	XHandle						m_matHandleCache;
	XHandle						m_texHandleCache[X_MAX_SAMPLERS];

	HRESULT						m_hr;
	XStatus						m_status;
	XBOOL						m_inited;
	
	
	XHandle						m_spriteVBHandle;

	
	//Viewport quad
	XHandle						m_viewportQuadVB;
	XHandle						m_viewportQuadVS;
	XHandle						m_viewportQuadMat;

	XHandle						m_activeRenderContext;
	
	XHandle						m_defaultTexHandle;
	XHandle						m_missingTexHandle;
	XHandle						m_missingMatHandle;
	XHandle						m_streamingTexHandle;
	XBOOL						m_texOverrides[X_MAX_SAMPLERS];

	XMemBlockList<XHandle>		m_vsoHandles;
	XMemBlockList<XHandle>		m_gsoHandles;
	XMemBlockList<XHandle>		m_psoHandles;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	XStatus createDevice(XVPDesc& initDesc);
	//XBOOL getColorDepth(D3DFORMAT fmt,gdriverColorInfo* pRetValue);
	D3D10_MAP translateLockType(X_LOCK_TYPE lockType);
	XStatus createVertexDeclarationName(XVertexDesc& desc,XS8 name[X_MAX_VERT_DCL_NAME_LEN]);
	XStatus initLuaGlue();
	XStatus applyMaterial(XMaterial* pMaterial);


//	void freeMovieResources(GraphicsMovie* pMovie);

public:
	static DXGI_FORMAT translateDataFormatType(X_DATA_FORMAT_TYPE dataFormat);
	static X_DATA_FORMAT_TYPE translateDataFormatType(DXGI_FORMAT dataFormat);
	static DXGI_MODE_SCALING translateScalingMode(X_SCALING_MODE scaleMode);
	static X_SCALING_MODE translateScalingMode(DXGI_MODE_SCALING scaleMode);
	static DXGI_MODE_SCANLINE_ORDER translateScanlineOrder(X_SCANLINE_ORDER scanlineOrder);
	static X_SCANLINE_ORDER translateScanlineOrder(DXGI_MODE_SCANLINE_ORDER scanlineOrder);
	
	ID3D10Device*			m_pD3DDevice;
	IDXGIFactory*			m_pDXGIFactory;

	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XGraphicsMgr();
	~XGraphicsMgr();

	//*****************************************************************************
	//Standard XGraphicsMgr Functions
	//*****************************************************************************
	XStatus update(XF32 dt);
	XStatus renderEvent(XRenderEvent* pRenderEvent);
	XStatus addScene(XHandle renderContext,XU32 rcLayer,XScene* pScene,XScene* pDependentScene);
	XStatus removeScene(XHandle renderContext,XScene* pScene);
	XStatus removeAllScenes(XHandle renderContext);
	XStatus setViewport(XViewport& viewport);
	XStatus setScissorRect(XRect& scissorRect);
	XStatus setScissorRect(XViewport& viewport);



	//XStatus addRenderEvent(XScene* pScene,XSceneNode* pSceneNode,XHandle matHandle,void* pUserData);

	XStatus render(XF32 dt);

	//XStatus getTexMatrix(XU32 texStage,XMatrix4x4& texMatrix);

	//XStatus dumpDeviceCapabilities(XS8* pFilename);
	XStatus dumpScreenshot(XS8* pFilename);
	void* getRenderingDevice();
	XStatus generalInit(XVPDesc& desc);
	XStatus init(XVPDesc& initDesc);
	XStatus destroy();
	XStatus loadShaderWad(XS8* pFilename);

	//*****************************************************************************
	//Display Mode Functions
	//*****************************************************************************
	XU32 getNumFullScreenModes();
	XStatus getFullScreenMode(XU32 index,XFullScreenModeDesc& fullScreenModeDesc);
	void* getWindowHandle(XHandle renderContext);
	XBOOL isFullScreenMode(XHandle renderContext);
	XStatus setFullScreenMode(XHandle renderContext,XU32 fullScreenModeIndex);
	XStatus setWindowedMode(XHandle renderContext);
	XStatus resizeWindow(XHandle renderContext,XU32 width,XU32 height);

	//*****************************************************************************
	//Gamma Functions
	//*****************************************************************************
#ifdef OLD
	XStatus setRedGammaPercent(XF32 percent);
	XStatus setBlueGammaPercent(XF32 percent);
	XStatus setGreenGammaPercent(XF32 percent);
	XStatus setRGBGammaPercent(XF32 rPercent,XF32 gPercent,XF32 bPercent);
	XStatus invertRGBGamma();
	XF32 getRedGammaPercent();
	XF32 getBlueGammaPercent();
	XF32 getGreenGammaPercent();
#endif

	//*****************************************************************************
	//Render Context Functions
	//*****************************************************************************
	XStatus createRenderContext(XRenderContextDesc& desc,XHandle& rcHandle);
	XStatus destroyRenderContext(XHandle rcHandle);
	XStatus clearRenderContext(XHandle renderContext,XBOOL color,XBOOL depth,XBOOL stencil);

	XU32 getRenderContextWidth(XHandle renderContext);
	XU32 getRenderContextHeight(XHandle renderContext);
	XStatus setRenderContextClearColor(XHandle renderContext,XColor& color);
	XStatus getRenderContextClearColor(XHandle renderContext,XColor& color);
	XStatus setRenderContextDepthClearValue(XHandle renderContext,XF32 value);
	XF32 getRenderContextDepthClearValue(XHandle renderContext);
	XStatus setRenderContextStencilClearValue(XHandle renderContext,XU32 value);
	XU32 getRenderContextStencilClearValue(XHandle renderContext);

	XU32 getNumRenderContexts();
	XStatus destroyAllRenderContexts();
	XStatus addRenderContextRef(XHandle rcHandle);
	XStatus setRenderContext(XHandle renderContext);
	XHandle getRenderContext();
	XRenderContextDesc* getRenderContextDesc(XHandle renderContext);

	//*****************************************************************************
	//Render Target Array Functions
	//*****************************************************************************
	XStatus createRenderTargetArray(XRenderTargetArrayDesc& desc,XHandle& renderTargetArray);
	XStatus clearRenderTargetArray(XHandle renderTargetArray,XBOOL color,XBOOL depth,XBOOL stencil);
	XStatus clearRenderTargetArray(XHandle renderTargetArray,XU32 renderTargetIndex);
	XStatus setRenderTargetClearColor(XHandle renderTargetArray,XU32 renderTargetIndex,XColor& color);
	XStatus getRenderTargetClearColor(XHandle renderTargetArray,XU32 renderTargetIndex,XColor& color);
	XStatus setRenderTargetArrayClearColor(XHandle renderTargetArray,XColor& color);
	XStatus setDepthClearValue(XHandle renderTargetArray,XF32 value);
	XStatus overrideDepthBuffer(XHandle renderTargetArray,XHandle overrideRTA);
	XF32 getDepthClearValue(XHandle renderTargetArray);
	XStatus setStencilClearValue(XHandle renderTargetArray,XU32 value);
	XU32 getStencilClearValue(XHandle renderTargetArray);
	XStatus destroyRenderTargetArray(XHandle renderTargetArray);
	XU32 getNumRenderTargetArrays();
	XStatus destroyAllRenderTargetArrays();
	XStatus addRenderTargetArrayRef(XHandle rcHandle);
	XStatus resolveRenderTargetArray(XHandle renderTargetArray);
	XStatus setRenderTargetArray(XHandle renderTargetArray,XBOOL setDepthBuffer = true);
	XStatus removeRenderTargetArray();
	XRenderTargetArrayDesc* getRenderTargetArrayDesc(XHandle renderTargetArray);
	XStatus setRenderTargetAsTexture(XHandle renderTargetArray,XU32 renderTargetIndex,XU32 texStage);
	XStatus setDepthStencilAsTexture(XHandle renderTargetArray,XU32 texStage);
	XStatus copyRenderTargetToRenderContext(XHandle renderTargetArray,
														XU32 renderTargetIndex,
														XHandle renderContext);
	XStatus copyViewportToRenderContext(XHandle renderTargetArray, 
										XU32 renderTargetIndex,
										XViewport& viewport,
										XHandle renderContext);
	XStatus copyViewportToRenderTarget(XHandle srcRenderTargetArray,
										 XU32 srcRenderTargetIndex,
										 XViewport& srcViewport,
										 XHandle destRenderTargetArray,
										 XU32 destRenderTargetIndex);
	XStatus copyRectToRenderTarget(XHandle srcRenderTargetArray,
										 XU32 srcRenderTargetIndex,
										 XRect& srcRect,
										 XHandle destRenderTargetArray,
										 XU32 destRenderTargetIndex,
										 XU32 destTopLeftX,
										 XU32 destTopLeftY);
	XStatus copyRectToRenderContext(XHandle srcRenderTargetArray,
										 XU32 srcRenderTargetIndex,
										 XRect& srcRect,
										 XHandle destRenderContext,
										 XU32 destTopLeftX,
										 XU32 destTopLeftY);

	//*****************************************************************************
	//Index Buffer Functions
	//*****************************************************************************
	XStatus createIndexBuffer(XU32 numIndices,XBOOL dynamic,XHandle& ibHandle);
	WORD* lockIndexBuffer(XHandle ibHandle,X_LOCK_TYPE lockType);
	XStatus unlockIndexBuffer(XHandle ibHandle);
	XStatus destroyIndexBuffer(XHandle ibHandle);
	XIndexBuffer* getIndexBuffer(XHandle ibHandle);
	XU32 getIndexBufferLength(XHandle ibHandle);
	XU32 getNumIndexBuffers();
	XStatus destroyAllIndexBuffers();
	XStatus addIndexBufferRef(XHandle ibHandle);

	//*****************************************************************************
	//Vertex Declaration Functions
	//*****************************************************************************
	XStatus createVertexDeclaration(XVertexDesc& desc,XHandle& vdHandle);
	XStatus destroyVertexDeclaration(XHandle vdHandle);
	XVertexDeclaration* getVertexDeclarationInterface(XHandle vdHandle);
	XStatus getVertexDeclarationDesc(XHandle vdHandle,XVertexDesc& desc);
	XU32 getNumVertexDeclarations(XHandle vdHandle);
	XStatus destroyAllVertexDeclarations();
	XStatus addVertexDeclarationRef(XHandle vdHandle);
	XStatus createShaderSignature(XVertexDesc& desc,XS8* pShaderName);



	//*****************************************************************************
	//Vertex Buffer Functions
	//*****************************************************************************
	XStatus createVertexBuffer(XVertexDesc& desc,X_PRIMITIVE_TYPE primType,
								XU32 numVertices,XBOOL dynamic,XHandle& vbHandle);
	XStatus destroyVertexBuffer(XHandle vbHandle);
	XStatus renderVertexBuffer(XHandle vbHandle);
	XStatus renderVertexBufferCount(XHandle vbHandle,XU32 primCount);
	XStatus renderVertexBuffer(XHandle vbHandle,XHandle ibHandle);
	XStatus renderUserVertexBuffer(X_PRIMITIVE_TYPE pType,
											  XHandle vertexDeclaration,
											XU32 numPrimitives,void* pPrimitiveArray,
											XU32 primitiveStride);/*
	XStatus renderUserIndexedVertexBuffer(X_PRIMITIVE_TYPE pType,X_VERTEX_TYPE vType,
									XU32 numVertices,void* pVertices,XU32 numIndices,void* pIndices,
									XU32 numPrimitives,	XU32 vertexStride);*/
	XVertexBuffer* getVertexBuffer(XHandle vbHandle);
	XVertexBuffer* lockVertexBuffer(XHandle vbHandle,X_LOCK_TYPE lockType);
	XStatus unlockVertexBuffer(XHandle vbHandle);
	XU32 getVertexBufferLength(XHandle vbHandle);
	X_VERTEX_TYPE getVertexBufferType(XHandle vbHandle);
	X_PRIMITIVE_TYPE getVertexBufferPrimType(XHandle vbHandle);
	XU32 getNumVertexBuffers();
	XStatus destroyAllVertexBuffers();
	XStatus addVertexBufferRef(XHandle vbHandle);


	//*****************************************************************************
	//Constant Buffer Functions
	//*****************************************************************************
	XStatus createConstantBuffer(XU32 sizeInBytes,XBOOL dynamic,XHandle& cbHandle);
	XStatus destroyConstantBuffer(XHandle cbHandle);
	void* lockConstantBuffer(XHandle cbHandle,X_LOCK_TYPE lockType);
	XStatus unlockConstantBuffer(XHandle cbHandle);
	XStatus setConstantBufferData(XHandle cbHandle,void* pData,XU32 dataSizeInBytes);
	XU32 getNumConstantBuffers();
	XStatus destroyAllConstantBuffers();
	
	//*****************************************************************************
	//Texture Functions
	//*****************************************************************************
	XStatus createTextureFromFile(XS8* pFilename,XHandle& texHandle,bool absolutePath = false);
	XStatus createTextureFromMemory(XS8* pName,XS8* pMemoryChunk,XU32 memChunkSize,XHandle& texHandle);
	XStatus destroyTexture(XHandle texHandle);
	XStatus setTexture(XHandle texHandle,XU32 texStage);
	XStatus removeTexture(XU32 texStage);
	XStatus removeAllTextures();
	XStatus renderTexture(XU32 orgWidth,XU32 orgHeight,XRect& srcRect,XRect& destRect,
						XF32 opacity = 1.0f);
	XU32 getTextureWidth(XHandle texHandle);
	XU32 getTextureHeight(XHandle texHandle);
	XU32 getTextureDepth(XHandle texHandle);
	XU32 getNumTextures();
	XS8* getTextureFilename(XHandle texHandle);
	XStatus destroyAllTextures();
	XStatus addTextureRef(XHandle texHandle);
	XStatus setTextureOverrideFromTexture(XHandle texStage,XHandle texHandle);
	XStatus setTextureOverrideFromRTA(XHandle texStage,XHandle samplerState,XHandle renderTargetArray,XU32 renderTargetIndex);

	XStatus removeTextureOverride(XHandle texStage);

	//*************************************************************************
	//Material API functions.
	//*************************************************************************
	XStatus createMaterial(XS8* materialName,XHandle& matHandle);
	XStatus createMaterial(XS8* pMemoryChunk,XS8* materialName,XHandle& matHandle);
	XStatus cloneMaterial(XHandle matToClone,XHandle& matHandle);
	XStatus destroyMaterial(XHandle matHandle);
	XMaterial* lockMaterial(XHandle matHandle);
	XStatus unlockMaterial(XHandle matHandle);
	XHandle getMaterial(XS8* materialName);
	XStatus applyMaterial(XHandle matHandle);
	XMaterial* getMaterialInterface(XHandle matHandle);
	XStatus destroyAllMaterials();
	XS8* getMaterialName(XHandle matHandle);
	XStatus addMaterialRef(XHandle matHandle);

	//*****************************************************************************
	//Vertex Shader Functions
	//*****************************************************************************
	XStatus createVertexShader(XS8* pFilename,XS8* pEntryFunctionName,XHandle& vsHandle);
	XStatus createVertexShaderFromString(XS8* pShaderName,XS8* pBuffer,XU32 bufferLen,XS8* pEntryFunctionName,XHandle& vsHandle);
	XStatus destroyVertexShader(XHandle vsHandle);
	XBaseVertexShader* getVertexShaderByHandle(XHandle vsHandle);
	XHandle getVertexShaderByName(XS8* pShaderName);
	XHandle getBoundVertexShader();
	XStatus setVertexShader(XHandle vsHandle);
	XStatus removeVertexShader();
	XStatus destroyAllVertexShaders();
	XStatus addVertexShaderRef(XHandle vsHandle);
	XStatus setVSConstantBuffer(XU32 slotIndex,XHandle cbHandle);

	//*****************************************************************************
	//Geometry Shader Functions
	//*****************************************************************************
	XStatus createGeometryShader(XS8* pFilename,XS8* pEntryFunctionName,XHandle& gsHandle);
	XStatus destroyGeometryShader(XHandle gsHandle);
	XBaseGeometryShader* getGeometryShaderByName(XS8* pShaderName);
	XStatus setGeometryShader(XHandle gsHandle);
	XStatus destroyAllGeometryShaders();
	XStatus addGeometryShaderRef(XHandle gsHandle);
	XStatus setGSConstantBuffer(XU32 slotIndex,XHandle cbHandle);
	XStatus removeGeometryShader();

	//*****************************************************************************
	//Pixel Shader Functions
	//*****************************************************************************
	XStatus createPixelShader(XS8* pFilename,XS8* entryFunctionName,XHandle& psHandle);
	XStatus destroyPixelShader(XHandle psHandle);
	XHandle getBoundPixelShader();
	XStatus setPixelShader(XHandle psHandle);
	XStatus removePixelShader();
	XStatus destroyAllPixelShaders();
	XStatus addPixelShaderRef(XHandle psHandle);
	XHandle getPixelShaderByName(XS8* pShaderName);
	XStatus setPSConstantBuffer(XU32 slotIndex,XHandle cbHandle);

	//*****************************************************************************
	//DepthStencil State Functions
	//*****************************************************************************
	XStatus createDepthStencilState(XDepthStencilDesc& desc,XHandle& dssHandle);
	XStatus destroyDepthStencilState(XHandle dssHandle);
	XStatus setDepthStencilState(XHandle dssHandle);
	XStatus destroyAllDepthStencilStates();
	XStatus addDepthStencilStateRef(XHandle dssHandle);
	XU32 getNumDepthStencilStates();

	//*****************************************************************************
	//Rasterizer State Functions
	//*****************************************************************************
	XStatus createRasterizerState(XRasterizerDesc& desc,XHandle& rsHandle);
	XStatus destroyRasterizerState(XHandle rsHandle);
	XStatus setRasterizerState(XHandle rsHandle);
	XStatus destroyAllRasterizerStates();
	XStatus addRasterizerStateRef(XHandle rsHandle);
	XU32 getNumRasterizerStates();

	//*****************************************************************************
	//Blend State Functions
	//*****************************************************************************
	XStatus createBlendState(XBlendDesc& desc,XHandle& bsHandle);
	XStatus destroyBlendState(XHandle bsHandle);
	XStatus setBlendState(XHandle bsHandle);
	XStatus destroyAllBlendStates();
	XStatus addBlendStateRef(XHandle bsHandle);
	XU32 getNumBlendStates();

	//*****************************************************************************
	//Sampler State Functions
	//*****************************************************************************
	XStatus createSamplerState(XSamplerDesc& desc,XHandle& ssHandle);
	XStatus destroySamplerState(XHandle ssHandle);
	XStatus setSamplerState(XHandle ssHandle,XU32 texStage);
	XStatus destroyAllSamplerStates();
	XStatus addSamplerStateRef(XHandle ssHandle);
	XU32 getNumSamplerStates();
	
	//*****************************************************************************
	//Movie Functions
	//*****************************************************************************
	XStatus createMovie(XS8* pFilename,XHandle& movieHandle);
	XStatus destroyMovie(XHandle movieHandle);
	XStatus playMovie(XHandle movieHandle,XS32 playCount);
	XStatus pauseMovie(XHandle movieHandle);
	XStatus resumeMovie(XHandle movieHandle);
	XStatus stopMovie(XHandle movieHandle);
	XStatus setMovieTime(XHandle movieHandle,XU32 milliseconds);
	XU32 getMovieTime(XHandle);
	XStatus setMoviePlayCount(XHandle movieHandle,XS32 playCount);
	XS32 getMoviePlayCount(XHandle movieHandle);
	XU32 getMovieDuration(XHandle movieHandle);
	XU32 getMovieWidth(XHandle movieHandle);
	XU32 getMovieHeight(XHandle movieHandle);
	XBOOL isMoviePlaying(XHandle movieHandle);
	XStatus destroyAllMovies();
	XStatus addMovieRef(XHandle movieHandle);

	//*****************************************************************************
	//Render State Functions
	//*****************************************************************************
	/*XStatus enableDepthTest(XBOOL enable);
	XBOOL isDepthTestEnabled();
	XStatus enableDepthWrite(XBOOL enable);
	XBOOL isDepthWriteEnabled();
	XStatus enableAlphaTest(XBOOL enable);
	XBOOL isAlphaTestEnabled();
	XStatus enableAlphaBlend(XBOOL enable);
	XBOOL isAlphaBlendEnabled();
	XStatus enableStencilBuffer(XBOOL enable);
	XBOOL isStencilBufferEnabled();
	XStatus enablePointSprites(XBOOL enable);
	XBOOL isPointSpritesEnabled();
	XStatus enableMultiSampleAntialias(XBOOL enable);
	XBOOL isMultiSampleAntialiasEnabled();

	XStatus setDepthBuffer(X_DEPTH_BUFFER_TYPE value);
	X_DEPTH_BUFFER_TYPE getDepthBuffer();
	XStatus setFillMode(X_FILL_MODE_TYPE value);
	X_FILL_MODE_TYPE getFillMode();
	XStatus setSrcBlend(X_BLEND_TYPE value);
	X_BLEND_TYPE getSrcBlend();
	XStatus setDestBlend(X_BLEND_TYPE value);
	X_BLEND_TYPE getDestBlend();
	XStatus setCullMode(X_CULL_TYPE value);
	X_CULL_TYPE getCullMode();
	XStatus setDepthFunc(X_CMP_FUNC_TYPE value);
	X_CMP_FUNC_TYPE getDepthFunc();	
	XStatus setAlphaFunc(X_CMP_FUNC_TYPE value);
	X_CMP_FUNC_TYPE getAlphaFunc();	
	XStatus setAlphaRef(ULONG32 value);
	XU32 getAlphaRef();	
	XStatus setDepthBias(XF32 value);
	XF32 getDepthBias();	
	XStatus setStencilFail(X_STENCIL_OP_TYPE value);
	X_STENCIL_OP_TYPE getStencilFail();	
	XStatus setStencilDepthFail(X_STENCIL_OP_TYPE value);
	X_STENCIL_OP_TYPE getStencilDepthFail();	
	XStatus setStencilPass(X_STENCIL_OP_TYPE value);
	X_STENCIL_OP_TYPE getStencilPass();	
	XStatus setStencilFunc(X_CMP_FUNC_TYPE value);
	X_CMP_FUNC_TYPE getStencilFunc();	
	XStatus setStencilRef(XS32 value);
	XS32 getStencilRef();	
	XStatus setStencilMask(ULONG32 value);
	XU32 getStencilMask();	
	XStatus setStencilWriteMask(ULONG32 value);
	XU32 getStencilWriteMask();	
	XStatus setWrap0(ULONG32 value);
	XU32 getWrap0();	
	XStatus setWrap1(ULONG32 value);
	XU32 getWrap1();	
	XStatus setWrap2(ULONG32 value);
	XU32 getWrap2();	
	XStatus setWrap3(ULONG32 value);
	XU32 getWrap3();	
	XStatus setWrap4(ULONG32 value);
	XU32 getWrap4();	
	XStatus setWrap5(ULONG32 value);
	XU32 getWrap5();
	XStatus setWrap6(ULONG32 value);
	XU32 getWrap6();	
	XStatus setWrap7(ULONG32 value);
	XU32 getWrap7();	
	XStatus setWrap8(ULONG32 value);
	XU32 getWrap8();	
	XStatus setWrap9(ULONG32 value);
	XU32 getWrap9();	
	XStatus setWrap10(ULONG32 value);
	XU32 getWrap10();	
	XStatus setWrap11(ULONG32 value);
	XU32 getWrap11();	
	XStatus setWrap12(ULONG32 value);
	XU32 getWrap12();	
	XStatus setWrap13(ULONG32 value);
	XU32 getWrap13();	
	XStatus setWrap14(ULONG32 value);
	XU32 getWrap14();	
	XStatus setWrap15(ULONG32 value);
	XU32 getWrap15();	
	XStatus setPointSize(XF32 value);
	XF32 getPointSize();	
	XStatus setPointSizeMin(XF32 value);
	XF32 getPointSizeMin();	
	XStatus setPointSizeMax(XF32 value);
	XF32 getPointSizeMax();	
	XStatus setMultiSampleMask(ULONG32 value);
	XU32 getMultiSampleMask();	
	XStatus setBlendOp(X_BLEND_OP_TYPE value);
	X_BLEND_OP_TYPE getBlendOp();

	//*************************************************************************
	//Texture Stage functions.
	//*************************************************************************
	XStatus setAddressU(XU32 texStage,X_TEXTURE_ADDRESS_TYPE value);
	X_TEXTURE_ADDRESS_TYPE getAddressU(XU32 texStage);
    XStatus setAddressV(XU32 texStage,X_TEXTURE_ADDRESS_TYPE value);
	X_TEXTURE_ADDRESS_TYPE getAddressV(XU32 texStage);
   	XStatus setAddressW(XU32 texStage,X_TEXTURE_ADDRESS_TYPE value);
	X_TEXTURE_ADDRESS_TYPE getAddressW(XU32 texStage);
	XStatus setBorderColor(XU32 texStage,XColor value);
	XColor getBorderColor(XU32 texStage);
	XStatus setMagFilter(XU32 texStage,X_TEXTURE_FILTER_TYPE value);
	X_TEXTURE_FILTER_TYPE getMagFilter(XU32 texStage);
	XStatus setMinFilter(XU32 texStage,X_TEXTURE_FILTER_TYPE value);
	X_TEXTURE_FILTER_TYPE getMinFilter(XU32 texStage);
   	XStatus setMipFilter(XU32 texStage,X_TEXTURE_FILTER_TYPE value);
	X_TEXTURE_FILTER_TYPE getMipFilter(XU32 texStage);
	XStatus setMipmapLodBias(XU32 texStage,XF32 value);
	XF32 getMipmapLodBias(XU32 texStage);
	XStatus setMaxMipLevel(XU32 texStage,ULONG32 value);
	XU32 getMaxMipLevel(XU32 texStage);
	XStatus setMaxAnisotropy(XU32 texStage,ULONG32 value);
	XU32 getMaxAnisotropy(XU32 texStage);*/

	//*****************************************************************************
	//Device Capabilities
	//*****************************************************************************
	XU32 getMaxTextureWidth();
	XU32 getMaxTextureHeight();
	XU32 getMaxSimultaneousTextures();
	XU32 getMaxTextureStages();
	XU32 getMaxTextureRepeat();
	X_PIXEL_SHADER_VERSION getMaxPixelShaderVersion();
	X_VERTEX_SHADER_VERSION getMaxVertexShaderVersion();
	void* getDeviceCaps();


	XStatus renderViewportQuad(XScene* pScene,XHandle srcRTA,XViewport& srcViewport);

};




//*****************************************************************************
//*****************************************************************************
inline DXGI_FORMAT XGraphicsMgr::translateDataFormatType(X_DATA_FORMAT_TYPE dataFormat)
{
	switch (dataFormat)
	{
	case X_DFT_UNKNOWN:
		return DXGI_FORMAT_UNKNOWN;
	case X_DFT_R32G32B32A32_TYPELESS:
		return DXGI_FORMAT_R32G32B32A32_TYPELESS;
	case X_DFT_R32G32B32A32_FLOAT:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case X_DFT_R32G32B32A32_UINT:
		return DXGI_FORMAT_R32G32B32A32_UINT;
	case X_DFT_R32G32B32A32_SINT:
		return DXGI_FORMAT_R32G32B32A32_SINT;
	case X_DFT_R32G32B32_TYPELESS:
		return DXGI_FORMAT_R32G32B32_TYPELESS;
	case X_DFT_R32G32B32_FLOAT:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case X_DFT_R32G32B32_UINT:
		return DXGI_FORMAT_R32G32B32_UINT;
	case X_DFT_R32G32B32_SINT:
		return DXGI_FORMAT_R32G32B32_SINT;
	case X_DFT_R16G16B16A16_TYPELESS:
		return DXGI_FORMAT_R16G16B16A16_TYPELESS;
	case X_DFT_R16G16B16A16_FLOAT:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case X_DFT_R16G16B16A16_UNORM:
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	case X_DFT_R16G16B16A16_UINT:
		return DXGI_FORMAT_R16G16B16A16_UINT;
	case X_DFT_R16G16B16A16_SNORM:
		return DXGI_FORMAT_R16G16B16A16_SNORM;
	case X_DFT_R16G16B16A16_SINT:
		return DXGI_FORMAT_R16G16B16A16_SINT;
	case X_DFT_R32G32_TYPELESS:
		return DXGI_FORMAT_R32G32_TYPELESS;
	case X_DFT_R32G32_FLOAT:
		return DXGI_FORMAT_R32G32_FLOAT;
	case X_DFT_R32G32_UINT:
		return DXGI_FORMAT_R32G32_UINT;
	case X_DFT_R32G32_SINT:
		return DXGI_FORMAT_R32G32_SINT;
	case X_DFT_R32G8X24_TYPELESS:
		return DXGI_FORMAT_R32G8X24_TYPELESS;
	case X_DFT_D32_FLOAT_S8X24_UINT:
		return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	case X_DFT_R32_FLOAT_X8X24_TYPELESS:
		return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
	case X_DFT_X32_TYPELESS_G8X24_UINT:
		return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
	case X_DFT_R10G10B10A2_TYPELESS:
		return DXGI_FORMAT_R10G10B10A2_TYPELESS;
	case X_DFT_R10G10B10A2_UNORM:
		return DXGI_FORMAT_R10G10B10A2_UNORM;
	case X_DFT_R10G10B10A2_UINT:
		return DXGI_FORMAT_R10G10B10A2_UINT;
	case X_DFT_R11G11B10_FLOAT:
		return DXGI_FORMAT_R11G11B10_FLOAT;
	case X_DFT_R8G8B8A8_TYPELESS:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;
	case X_DFT_R8G8B8A8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case X_DFT_R8G8B8A8_UNORM_SRGB:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case X_DFT_R8G8B8A8_UINT:
		return DXGI_FORMAT_R8G8B8A8_UINT;
	case X_DFT_R8G8B8A8_SNORM:
		return DXGI_FORMAT_R8G8B8A8_SNORM;
	case X_DFT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_SINT;
	case X_DFT_R16G16_TYPELESS:
		return DXGI_FORMAT_R16G16_TYPELESS;
	case X_DFT_R16G16_FLOAT:
		return DXGI_FORMAT_R16G16_FLOAT;
	case X_DFT_R16G16_UNORM:
		return DXGI_FORMAT_R16G16_UNORM;
	case X_DFT_R16G16_UINT:
		return DXGI_FORMAT_R16G16_UINT;
	case X_DFT_R16G16_SNORM:
		return DXGI_FORMAT_R16G16_SNORM;
	case X_DFT_R16G16_SINT:
		return DXGI_FORMAT_R16G16_SINT;
	case X_DFT_R32_TYPELESS:
		return DXGI_FORMAT_R32_TYPELESS;
	case X_DFT_D32_FLOAT:
		return DXGI_FORMAT_D32_FLOAT;
	case X_DFT_R32_FLOAT:
		return DXGI_FORMAT_R32_FLOAT;
	case X_DFT_R32_UINT:
		return DXGI_FORMAT_R32_UINT;
	case X_DFT_R32_SINT:
		return DXGI_FORMAT_R32_SINT;
	case X_DFT_R24G8_TYPELESS:
		return DXGI_FORMAT_R24G8_TYPELESS;
	case X_DFT_D24_UNORM_S8_UINT:
		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case X_DFT_R24_UNORM_X8_TYPELESS:
		return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	case X_DFT_X24_TYPELESS_G8_UINT:
		return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	case X_DFT_R8G8_TYPELESS:
		return DXGI_FORMAT_R8G8_TYPELESS;
	case X_DFT_R8G8_UNORM:
		return DXGI_FORMAT_R8G8_UNORM;
	case X_DFT_R8G8_UINT:
		return DXGI_FORMAT_R8G8_UINT;
	case X_DFT_R8G8_SNORM:
		return DXGI_FORMAT_R8G8_SNORM;
	case X_DFT_R8G8_SINT:
		return DXGI_FORMAT_R8G8_SINT;
	case X_DFT_R16_TYPELESS:
		return DXGI_FORMAT_R16_TYPELESS;
	case X_DFT_R16_FLOAT:
		return DXGI_FORMAT_R16_FLOAT;
	case X_DFT_D16_UNORM:
		return DXGI_FORMAT_D16_UNORM;
	case X_DFT_R16_UNORM:
		return DXGI_FORMAT_R16_UNORM;
	case X_DFT_R16_UINT:
		return DXGI_FORMAT_R16_UINT;
	case X_DFT_R16_SNORM:
		return DXGI_FORMAT_R16_SNORM;
	case X_DFT_R16_SINT:
		return DXGI_FORMAT_R16_SINT;
	case X_DFT_R8_TYPELESS:
		return DXGI_FORMAT_R8_TYPELESS;
	case X_DFT_R8_UNORM:
		return DXGI_FORMAT_R8_UNORM;
	case X_DFT_R8_UINT:
		return DXGI_FORMAT_R8_UINT;
	case X_DFT_R8_SNORM:
		return DXGI_FORMAT_R8_SNORM;
	case X_DFT_R8_SINT:
		return DXGI_FORMAT_R8_SINT;
	case X_DFT_A8_UNORM:
		return DXGI_FORMAT_A8_UNORM;
	case X_DFT_R1_UNORM:
		return DXGI_FORMAT_R1_UNORM;
	case X_DFT_R9G9B9E5_SHAREDEXP:
		return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	case X_DFT_R8G8_B8G8_UNORM:
		return DXGI_FORMAT_R8G8_B8G8_UNORM;
	case X_DFT_G8R8_G8B8_UNORM:
		return DXGI_FORMAT_G8R8_G8B8_UNORM;
	case X_DFT_BC1_TYPELESS:
		return DXGI_FORMAT_BC1_TYPELESS;
	case X_DFT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM;
	case X_DFT_BC1_UNORM_SRGB:
		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case X_DFT_BC2_TYPELESS:
		return DXGI_FORMAT_BC2_TYPELESS;
	case X_DFT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM;
	case X_DFT_BC2_UNORM_SRGB:
		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case X_DFT_BC3_TYPELESS:
		return DXGI_FORMAT_BC3_TYPELESS;
	case X_DFT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM;
	case X_DFT_BC3_UNORM_SRGB:
		return DXGI_FORMAT_BC3_UNORM_SRGB;
	case X_DFT_BC4_TYPELESS:
		return DXGI_FORMAT_BC4_TYPELESS;
	case X_DFT_BC4_UNORM:
		return DXGI_FORMAT_BC4_UNORM;
	case X_DFT_BC4_SNORM:
		return DXGI_FORMAT_BC4_SNORM;
	case X_DFT_BC5_TYPELESS:
		return DXGI_FORMAT_BC5_TYPELESS;
	case X_DFT_BC5_UNORM:
		return DXGI_FORMAT_BC5_UNORM;
	case X_DFT_BC5_SNORM:
		return DXGI_FORMAT_BC5_SNORM;
	case X_DFT_B5G6R5_UNORM:
		return DXGI_FORMAT_B5G6R5_UNORM;
	case X_DFT_B5G5R5A1_UNORM:
		return DXGI_FORMAT_B5G5R5A1_UNORM;
	case X_DFT_B8G8R8A8_UNORM:
		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case X_DFT_B8G8R8X8_UNORM:
		return DXGI_FORMAT_B8G8R8X8_UNORM;
	default:
		X_ASSERT_FORCED("Unrecognized data format type.  Defaulting to unknown.");
		return DXGI_FORMAT_UNKNOWN;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_DATA_FORMAT_TYPE XGraphicsMgr::translateDataFormatType(DXGI_FORMAT dataFormat)
{
	switch (dataFormat)
	{
	case DXGI_FORMAT_UNKNOWN:
		return X_DFT_UNKNOWN;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		return X_DFT_R32G32B32A32_TYPELESS;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return X_DFT_R32G32B32A32_FLOAT;
	case DXGI_FORMAT_R32G32B32A32_UINT:
		return X_DFT_R32G32B32A32_UINT;
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return X_DFT_R32G32B32A32_SINT;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
		return X_DFT_R32G32B32_TYPELESS;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return X_DFT_R32G32B32_FLOAT;
	case DXGI_FORMAT_R32G32B32_UINT:
		return X_DFT_R32G32B32_UINT;
	case DXGI_FORMAT_R32G32B32_SINT:
		return X_DFT_R32G32B32_SINT;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		return X_DFT_R16G16B16A16_TYPELESS;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return X_DFT_R16G16B16A16_FLOAT;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		return X_DFT_R16G16B16A16_UNORM;
	case DXGI_FORMAT_R16G16B16A16_UINT:
		return X_DFT_R16G16B16A16_UINT;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		return X_DFT_R16G16B16A16_SNORM;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		return X_DFT_R16G16B16A16_SINT;
	case DXGI_FORMAT_R32G32_TYPELESS:
		return X_DFT_R32G32_TYPELESS;
	case DXGI_FORMAT_R32G32_FLOAT:
		return X_DFT_R32G32_FLOAT;
	case DXGI_FORMAT_R32G32_UINT:
		return X_DFT_R32G32_UINT;
	case DXGI_FORMAT_R32G32_SINT:
		return X_DFT_R32G32_SINT;
	case DXGI_FORMAT_R32G8X24_TYPELESS:
		return X_DFT_R32G8X24_TYPELESS;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		return X_DFT_D32_FLOAT_S8X24_UINT;
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		return X_DFT_R32_FLOAT_X8X24_TYPELESS;
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return X_DFT_X32_TYPELESS_G8X24_UINT;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		return X_DFT_R10G10B10A2_TYPELESS;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		return X_DFT_R10G10B10A2_UNORM;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		return X_DFT_R10G10B10A2_UINT;
	case DXGI_FORMAT_R11G11B10_FLOAT:
		return X_DFT_R11G11B10_FLOAT;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		return X_DFT_R8G8B8A8_TYPELESS;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return X_DFT_R8G8B8A8_UNORM;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return X_DFT_R8G8B8A8_UNORM_SRGB;
	case DXGI_FORMAT_R8G8B8A8_UINT:
		return X_DFT_R8G8B8A8_UINT;
	case DXGI_FORMAT_R8G8B8A8_SNORM:
		return X_DFT_R8G8B8A8_SNORM;
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return X_DFT_R8G8B8A8_SINT;
	case DXGI_FORMAT_R16G16_TYPELESS:
		return X_DFT_R16G16_TYPELESS;
	case DXGI_FORMAT_R16G16_FLOAT:
		return X_DFT_R16G16_FLOAT;
	case DXGI_FORMAT_R16G16_UNORM:
		return X_DFT_R16G16_UNORM;
	case DXGI_FORMAT_R16G16_UINT:
		return X_DFT_R16G16_UINT;
	case DXGI_FORMAT_R16G16_SNORM:
		return X_DFT_R16G16_SNORM;
	case DXGI_FORMAT_R16G16_SINT:
		return X_DFT_R16G16_SINT;
	case DXGI_FORMAT_R32_TYPELESS:
		return X_DFT_R32_TYPELESS;
	case DXGI_FORMAT_D32_FLOAT:
		return X_DFT_D32_FLOAT;
	case DXGI_FORMAT_R32_FLOAT:
		return X_DFT_R32_FLOAT;
	case DXGI_FORMAT_R32_UINT:
		return X_DFT_R32_UINT;
	case DXGI_FORMAT_R32_SINT:
		return X_DFT_R32_SINT;
	case DXGI_FORMAT_R24G8_TYPELESS:
		return X_DFT_R24G8_TYPELESS;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		return X_DFT_D24_UNORM_S8_UINT;
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		return X_DFT_R24_UNORM_X8_TYPELESS;
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return X_DFT_X24_TYPELESS_G8_UINT;
	case DXGI_FORMAT_R8G8_TYPELESS:
		return X_DFT_R8G8_TYPELESS;
	case DXGI_FORMAT_R8G8_UNORM:
		return X_DFT_R8G8_UNORM;
	case DXGI_FORMAT_R8G8_UINT:
		return X_DFT_R8G8_UINT;
	case DXGI_FORMAT_R8G8_SNORM:
		return X_DFT_R8G8_SNORM;
	case DXGI_FORMAT_R8G8_SINT:
		return X_DFT_R8G8_SINT;
	case DXGI_FORMAT_R16_TYPELESS:
		return X_DFT_R16_TYPELESS;
	case DXGI_FORMAT_R16_FLOAT:
		return X_DFT_R16_FLOAT;
	case DXGI_FORMAT_D16_UNORM:
		return X_DFT_D16_UNORM;
	case DXGI_FORMAT_R16_UNORM:
		return X_DFT_R16_UNORM;
	case DXGI_FORMAT_R16_UINT:
		return X_DFT_R16_UINT;
	case DXGI_FORMAT_R16_SNORM:
		return X_DFT_R16_SNORM;
	case DXGI_FORMAT_R16_SINT:
		return X_DFT_R16_SINT;
	case DXGI_FORMAT_R8_TYPELESS:
		return X_DFT_R8_TYPELESS;
	case DXGI_FORMAT_R8_UNORM:
		return X_DFT_R8_UNORM;
	case DXGI_FORMAT_R8_UINT:
		return X_DFT_R8_UINT;
	case DXGI_FORMAT_R8_SNORM:
		return X_DFT_R8_SNORM;
	case DXGI_FORMAT_R8_SINT:
		return X_DFT_R8_SINT;
	case DXGI_FORMAT_A8_UNORM:
		return X_DFT_A8_UNORM;
	case DXGI_FORMAT_R1_UNORM:
		return X_DFT_R1_UNORM;
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		return X_DFT_R9G9B9E5_SHAREDEXP;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
		return X_DFT_R8G8_B8G8_UNORM;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		return X_DFT_G8R8_G8B8_UNORM;
	case DXGI_FORMAT_BC1_TYPELESS:
		return X_DFT_BC1_TYPELESS;
	case DXGI_FORMAT_BC1_UNORM:
		return X_DFT_BC1_UNORM;
	case DXGI_FORMAT_BC1_UNORM_SRGB:
		return X_DFT_BC1_UNORM_SRGB;
	case DXGI_FORMAT_BC2_TYPELESS:
		return X_DFT_BC2_TYPELESS;
	case DXGI_FORMAT_BC2_UNORM:
		return X_DFT_BC2_UNORM;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
		return X_DFT_BC2_UNORM_SRGB;
	case DXGI_FORMAT_BC3_TYPELESS:
		return X_DFT_BC3_TYPELESS;
	case DXGI_FORMAT_BC3_UNORM:
		return X_DFT_BC3_UNORM;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
		return X_DFT_BC3_UNORM_SRGB;
	case DXGI_FORMAT_BC4_TYPELESS:
		return X_DFT_BC4_TYPELESS;
	case DXGI_FORMAT_BC4_UNORM:
		return X_DFT_BC4_UNORM;
	case DXGI_FORMAT_BC4_SNORM:
		return X_DFT_BC4_SNORM;
	case DXGI_FORMAT_BC5_TYPELESS:
		return X_DFT_BC5_TYPELESS;
	case DXGI_FORMAT_BC5_UNORM:
		return X_DFT_BC5_UNORM;
	case DXGI_FORMAT_BC5_SNORM:
		return X_DFT_BC5_SNORM;
	case DXGI_FORMAT_B5G6R5_UNORM:
		return X_DFT_B5G6R5_UNORM;
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		return X_DFT_B5G5R5A1_UNORM;
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return X_DFT_B8G8R8A8_UNORM;
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		return X_DFT_B8G8R8X8_UNORM;
	default:
		X_ASSERT_FORCED("Unrecognized data format type.  Defaulting to unknown.");
		return X_DFT_UNKNOWN;
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_SCALING_MODE XGraphicsMgr::translateScalingMode(DXGI_MODE_SCALING scaleMode)
{
	switch (scaleMode)
	{
	case DXGI_MODE_SCALING_UNSPECIFIED:
		return X_SCALING_UNKNOWN;
	case DXGI_MODE_SCALING_CENTERED:
		return X_SCALING_CENTERED;
	case DXGI_MODE_SCALING_STRETCHED:
		return X_SCALING_STRETCHED;
	default:
		X_ASSERT_FORCED("Unknown scaling mode. Defaulting to unspecified.");
		return X_SCALING_UNKNOWN;
	}
}



//*****************************************************************************
//*****************************************************************************
inline DXGI_MODE_SCALING XGraphicsMgr::translateScalingMode(X_SCALING_MODE scaleMode)
{
	switch (scaleMode)
	{
	case X_SCALING_UNKNOWN:
		return DXGI_MODE_SCALING_UNSPECIFIED;
	case X_SCALING_CENTERED:
		return DXGI_MODE_SCALING_CENTERED;
	case X_SCALING_STRETCHED:
		return DXGI_MODE_SCALING_STRETCHED;
	default:
		X_ASSERT_FORCED("Unknown scaling mode. Defaulting to unspecified.");
		return DXGI_MODE_SCALING_UNSPECIFIED;
	}
}



//*****************************************************************************
//*****************************************************************************
inline DXGI_MODE_SCANLINE_ORDER XGraphicsMgr::translateScanlineOrder(X_SCANLINE_ORDER scanlineOrder)
{
	switch (scanlineOrder)
	{
	case X_SCANLINE_ORDER_UNKNOWN:
		return DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	case X_SCANLINE_ORDER_PROGRESSIVE:
		return DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	case X_SCANLINE_ORDER_UPPER_FIELD_FIRST:
		return DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;
	case X_SCANLINE_ORDER_LOWER_FIELD_FIRST:
		return DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST;
	default:
		X_ASSERT_FORCED("Unknown scanline mode.  Defaulting to unknown.");
		return DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; 
	}
}



//*****************************************************************************
//*****************************************************************************
inline X_SCANLINE_ORDER XGraphicsMgr::translateScanlineOrder(DXGI_MODE_SCANLINE_ORDER scanlineOrder)
{
	switch (scanlineOrder)
	{
	case DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED:
		return X_SCANLINE_ORDER_UNKNOWN;
	case DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE:
		return X_SCANLINE_ORDER_PROGRESSIVE;
	case DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST:
		return X_SCANLINE_ORDER_UPPER_FIELD_FIRST;
	case DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST:
		return X_SCANLINE_ORDER_LOWER_FIELD_FIRST;
	default:
		X_ASSERT_FORCED("Unknown scanline mode.  Defaulting to unknown.");
		return X_SCANLINE_ORDER_UNKNOWN; 
	}
}



#endif