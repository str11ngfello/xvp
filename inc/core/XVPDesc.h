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
//	Module Name:	XVPDesc.h
//	Author:			Lynn Duke
//	Creation Date:	5-7-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_XVP_DESC_H__
#define __X_XVP_DESC_H__




//*****************************************************************************
//!X-VP description structure.
/**
This structure is used to initialize X-VP.  
*/
//*****************************************************************************
struct XVPDesc
{
	void* pProcessHandle;				/*!< OS handle to process. */
	XBOOL initAudioMgr;					/*!< Initialize Audio Manager. */
	XBOOL initInputMgr;					/*!< Initialize Input Manager. */
	XBOOL initNetworkMgr;				/*!< Initialize Network Manager. */
	XBOOL generateLogFile;				/*!< True to generate log file, false otherwise. */
	XBOOL enableAsserts;				/*!< Enables/Disables asserts.  Only valid if asserts were compiled in. */
	XBOOL enableDeviceMessages;			/*!< Enables/Disables device debug messages.  E.g., DX10 messages */
	XBOOL enableInternalFreeLists;		/*!< Enables/Disables internal free lists for allocations. */
	XBOOL enableKeyboard;				/*!< Enables/Disables keyboard. */
	XBOOL enableMouse;					/*!< Enables/Disables mouse. */
	XBOOL enableJoysticks;				/*!< Enables/Disables joystick. */
	XU32 debugRenderContext;			/*!< Render context that will receive debug rendering. */
	XU32 audioStreamThreshold;			/*!< Maximum pcm data size to load into memory. */
	XU32 maxCharSets;					/*!< Max number of character sets. */
	XU32 maxLights;						/*!< Max number of lights. */
	XU32 maxMaterials;					/*!< Max number of materials. */
	XU32 maxTextures;					/*!< Max number of textures. */
	XU32 maxVertexDeclarations;			/*!< Max number of vertex declarations. */
	XU32 maxVertexBuffers;				/*!< Max number of vertex buffers. */
	XU32 maxIndexBuffers;				/*!< Max number of index buffers. */
	XU32 maxRenderContexts;				/*!< Max number of render contexts. */
	XU32 maxRenderTargetArrays;			/*!< Max number of render target arrays. */
	XU32 maxConstantBuffers;			/*!< Max number of constant buffers. */
	XU32 maxVertexShaders;				/*!< Max number of vertex shaders. */
	XU32 maxGeometryShaders;			/*!< Max number of geometry shaders. */
	XU32 maxPixelShaders;				/*!< Max number of pixel shaders. */
	XU32 maxDepthStencilStates;			/*!< Max number of depth stencil states. */
	XU32 maxRasterizerStates;			/*!< Max number of rasterizer states. */
	XU32 maxBlendStates;				/*!< Max number of blend states. */
	XU32 maxSamplerStates;				/*!< Max number of sampler states. */
	XU32 maxMovies;						/*!< Max number of movies. */
	XU32 maxSounds;						/*!< Max number of sounds. */
	XU32 maxTCPConnections;				/*!< Max number of TCP connections. */
	XU32 maxUDPConnections;				/*!< Max number of UDP connections. */
	XU32 maxTCPListeners;				/*!< Max number of TCP listeners. */
	
	//XU32 (*pFullScreenModeCallback)	(XFullScreenModeDesc* pFullScreenModes,
	//								 XU32 numFullScreenModes);


	XVPDesc::XVPDesc()
	{
		pProcessHandle = NULL;
		initAudioMgr = true;					
		initInputMgr = true;
		initNetworkMgr = true;				
		generateLogFile = false;
		enableAsserts = true;
		enableDeviceMessages = true;
		enableInternalFreeLists = true;
		enableKeyboard = true;
		enableMouse = true;
		enableJoysticks = true;
		audioStreamThreshold = 262144;
		debugRenderContext = 0;
			
		maxMaterials = 8192;
		maxCharSets = 512;		
		maxLights = 16;			
		maxTextures = 8192;
		maxVertexDeclarations = 256;
		maxVertexBuffers = 8192;
		maxIndexBuffers = 8192;
		maxRenderContexts = 64;
		maxRenderTargetArrays = 4096;
		maxVertexShaders = 4096;
		maxGeometryShaders = 4096;
		maxPixelShaders = 4096;
		maxDepthStencilStates = 8192;		
		maxRasterizerStates = 8192;				
		maxBlendStates = 8192;					
		maxSamplerStates = 8192;					
		maxConstantBuffers = 8192;
		maxMovies = 128;
		maxSounds = 4096;
		maxTCPConnections = 32;		
		maxUDPConnections = 32;			
		maxTCPListeners = 32;	

		//pFullScreenModeCallback = NULL;
	}
};

#endif