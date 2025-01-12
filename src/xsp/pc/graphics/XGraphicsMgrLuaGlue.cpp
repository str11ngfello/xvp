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
//	Module Name:	XGraphicsMgrLuaGlue.cpp
//	Author:			Lynn Duke
//	Creation Date:	5-29-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>



#ifdef X_CONSOLE
static const XS8* XGraphicsLuaLibDirectory[] = 
{
	NULL
};



int LAPI_graphics(lua_State* pLuaState)
{
	XS8** p = (XS8**)XGraphicsLuaLibDirectory;
	while (*p)
	{
		X_CONSOLE(XConsole::setLastCommandResponseString(NULL,*p));
		++p;
	}
	return 0;
}
#endif

/*
int LAPI_getScreenWidth(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XU32 screenWidth = XVP::GraphicsMgr.getScreenWidth();
	lua_pushnumber(pLuaState,screenWidth);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,screenWidth);)
	return 1;
}

int LAPI_getScreenHeight(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XU32 screenHeight = XVP::GraphicsMgr.getScreenHeight();
	lua_pushnumber(pLuaState,screenHeight);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,screenHeight);)
	return 1;
}



int LAPI_setClearColor(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 4)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::GraphicsMgr.setClearColor(XColor((XF32)lua_tonumber(pLuaState,1),
												(XF32)lua_tonumber(pLuaState,2),
														  (XF32)lua_tonumber(pLuaState,3),
														  (XF32)lua_tonumber(pLuaState,4)));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_getClearColor(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XColor color;
	XVP::GraphicsMgr.getClearColor(color);

	lua_pushnumber(pLuaState,color.m_R);
	lua_pushnumber(pLuaState,color.m_G);
	lua_pushnumber(pLuaState,color.m_B);
	lua_pushnumber(pLuaState,color.m_A);
	X_CONSOLE(XConsole::setLastCommandResponseColor(NULL,color);)

	return 4;
}

int LAPI_setDepthClearValue(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::GraphicsMgr.setDepthClearValue((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_getDepthClearValue(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XF32 z = XVP::GraphicsMgr.getDepthClearValue();

	lua_pushnumber(pLuaState,z);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,z);)

	return 1;
}

int LAPI_setStencilClearValue(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::GraphicsMgr.setStencilClearValue((XU32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}

int LAPI_getStencilClearValue(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XU32 stencil = XVP::GraphicsMgr.getStencilClearValue();

	lua_pushnumber(pLuaState,stencil);
	X_CONSOLE(XConsole::setLastCommandResponseUInt32(NULL,stencil);)

	return 1;
}

*/
int LAPI_dumpScreenshot(lua_State* pLuaState)
{
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::GraphicsMgr.dumpScreenshot((XS8*)lua_tostring(pLuaState,1));
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
}


int LAPI_setRedGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus temp = XVP::GraphicsMgr.setRedGammaPercent((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(temp);)
	return 1;
#else
	return 0;
#endif
}


int LAPI_setBlueGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus temp = XVP::GraphicsMgr.setBlueGammaPercent((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(temp);)
	return 1;
#else
	return 0;
#endif
}

int LAPI_setGreenGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus temp = XVP::GraphicsMgr.setGreenGammaPercent((XF32)lua_tonumber(pLuaState,1));
	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(temp);)
	return 1;
#else
	return 0;
#endif
}

int LAPI_setRGBGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 3)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus temp = XVP::GraphicsMgr.setRGBGammaPercent((XF32)lua_tonumber(pLuaState,1),
														(XF32)lua_tonumber(pLuaState,2),
														(XF32)lua_tonumber(pLuaState,3));
	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(temp);)
	return 1;
#else
	return 0;
#endif
}


int LAPI_invertRGBGamma(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}
	XStatus status = XVP::GraphicsMgr.invertRGBGamma();
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
#else
	return 0;
#endif
}


int LAPI_getRedGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XF32 temp = XVP::GraphicsMgr.getRedGammaPercent();

	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,temp);)

	return 1;
#else
	return 0;
#endif
}

int LAPI_getBlueGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XF32 temp = XVP::GraphicsMgr.getBlueGammaPercent();

	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,temp);)

	return 1;
#else
	return 0;
#endif
}


int LAPI_getGreenGammaPercent(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 0)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XF32 temp = XVP::GraphicsMgr.getGreenGammaPercent();

	lua_pushnumber(pLuaState,temp);
	X_CONSOLE(XConsole::setLastCommandResponseFloat(NULL,temp);)

	return 1;
#else
	return 0;
#endif
}

int LAPI_enableAlphaSorting(lua_State* pLuaState)
{
#ifdef OLD
	if (lua_gettop(pLuaState) != 1)
	{
		X_CONSOLE(XConsole::setLastCommandResponseXStatus(X_ERR_WRONG_NUMBER_OF_PARAMS);)
		return 0;
	}

	XStatus status = XVP::GraphicsMgr.enableAlphaSorting((XBOOL)lua_toboolean(pLuaState,1));										
	lua_pushnumber(pLuaState,status);
	X_CONSOLE(XConsole::setLastCommandResponseXStatus(status);)
	return 1;
#else
	return 0;
#endif
}
/*
XStatus createLight(XLightDesc& desc,XHandle& lightHandle);
XStatus destroyLight(XHandle lightHandle);
XStatus setLightDiffuse(XHandle lightHandle,XColor& diffuse);
XStatus setLightSpecular(XHandle lightHandle,XColor& specular);
XStatus setLightAmbient(XHandle lightHandle,XColor& ambient);
XStatus setLightPosition(XHandle lightHandle,XVector3D& position);
XStatus setLightDirection(XHandle lightHandle,XVector3D& direction);
XStatus setLightRange(XHandle lightHandle,XF32 range);
XStatus setLightFalloff(XHandle lightHandle,XF32 falloff);
XStatus setLightConstantAtt(XHandle lightHandle,XF32 attenuation);
XStatus setLightLinearAtt(XHandle lightHandle,XF32 attenuation);
XStatus setLightQuadraticAtt(XHandle lightHandle,XF32 attenuation);
XStatus setLightInnerConeAngle(XHandle lightHandle,XF32 angle);
XStatus setLightOuterConeAngle(XHandle lightHandle,XF32 angle);
XColor getLightDiffuse(XHandle lightHandle);
XColor getLightSpecular(XHandle lightHandle);
XColor getLightAmbient(XHandle lightHandle);
XVector3D getLightPosition(XHandle lightHandle);
XVector3D getLightDirection(XHandle lightHandle);
XF32 getLightRange(XHandle lightHandle);
XF32 getLightFalloff(XHandle lightHandle);
XF32 getLightConstantAtt(XHandle lightHandle);
XF32 getLightLinearAtt(XHandle lightHandle);
XF32 getLightQuadraticAtt(XHandle lightHandle);
XF32 getLightInnerConeAngle(XHandle lightHandle);
XF32 getLightOuterConeAngle(XHandle lightHandle);
XStatus enableLight(XHandle lightHandle,XU32 lightIndex);
XStatus disableLight(XHandle lightHandle);
XStatus destroyAllLights();
XU32 getNumLights();
XStatus createMaterial(XColor& diffuse,XColor& ambient,XColor& emissive,
							  XColor& specular,XF32 specPower,XHandle& matHandle);
XStatus destroyMaterial(XHandle matHandle);
XStatus setMaterialDiffuse(XHandle matHandle,XColor& diffuse);
XStatus setMaterialAmbient(XHandle matHandle,XColor& ambient);
XStatus setMaterialEmissive(XHandle matHandle,XColor& emissive);
XStatus setMaterialSpecular(XHandle matHandle,XColor& specular);
XStatus setMaterialSpecPower(XHandle matHandle,XF32 specPower);
XColor getMaterialDiffuse(XHandle matHandle);
XColor getMaterialAmbient(XHandle matHandle);
XColor getMaterialEmissive(XHandle matHandle);
XColor getMaterialSpecular(XHandle matHandle);
XF32 getMaterialSpecPower(XHandle matHandle);
XStatus setMaterial(XHandle matHandle);
XStatus destroyAllMaterials();
XU32 getNumMaterials();
XStatus createIndexBuffer(XU32 numIndices,XBOOL dynamic,XHandle& ibHandle);
WORD* lockIndexBuffer(XHandle ibHandle);
XStatus unlockIndexBuffer(XHandle ibHandle);
XStatus destroyIndexBuffer(XHandle ibHandle);
XU32 getIndexBufferLength(XHandle ibHandle);
XU32 getNumIndexBuffers();
XStatus destroyAllIndexBuffers();
XStatus createVertexBuffer(X_VERTEX_TYPE vbType,X_PRIMITIVE_TYPE primType,
							XU32 numVertices,XBOOL dynamic,XHandle& vbHandle);
XStatus destroyVertexBuffer(XHandle vbHandle);
XStatus renderVertexBuffer(XHandle vbHandle);
XStatus renderVertexBuffer(XHandle vbHandle,XHandle ibHandle);
XStatus renderUserVertexBuffer(X_PRIMITIVE_TYPE pType,X_VERTEX_TYPE vType,
								XU32 numPrimitives,void* p_PrimitiveArray,
								XU32 primitiveStride);
void* lockVertexBuffer(XHandle vbHandle,X_LOCK_FLAGS flags = X_LF_NONE);
XStatus unlockVertexBuffer(XHandle vbHandle);
XU32 getVertexBufferLength(XHandle vbHandle);
X_VERTEX_TYPE getVertexBufferType(XHandle vbHandle);
X_PRIMITIVE_TYPE getVertexBufferPrimType(XHandle vbHandle);
XU32 getNumVertexBuffers();
XStatus destroyAllVertexBuffers();
XStatus createTexture(XS8* pFilename,X_TEXTURE_TYPE type,XHandle& texHandle);
XStatus destroyTexture(XHandle texHandle);
XStatus setTexture(XHandle texHandle,XU32 texStage);
XStatus removeTexture(XU32 texStage);
XStatus renderTexture(XU32 orgWidth,XU32 orgHeight,XRect& srcRect,XRect& destRect);
XU32 getTextureWidth(XHandle texHandle);
XU32 getTextureHeight(XHandle texHandle);
XU32 getTextureDepth(XHandle texHandle);
XU32 getNumTextures();
XS8* getTextureFilename(XHandle texHandle);
XStatus destroyAllTextures();
XStatus createVertexShader(XS8* pFilename,XS8* entryFunctionName,
						   X_VERTEX_SHADER_TYPE type,XHandle& vsHandle);
XStatus destroyVertexShader(XHandle vsHandle);
XStatus setVertexShader(XHandle vsHandle);
XStatus setVertexShaderConstant(XU32 constReg,XF32* pValues,XU32 num4Tuples);
XStatus removeVertexShader();
XStatus destroyAllVertexShaders();
XStatus createPixelShader(XS8* pFilename,XS8* entryFunctionName,X_PIXEL_SHADER_TYPE type,
						  XHandle& psHandle);
XStatus destroyPixelShader(XHandle psHandle);
XStatus setPixelShader(XHandle psHandle);
XStatus setPixelShaderConstant(XU32 constReg,XF32* pValues,XU32 num4Tuples);
XStatus removePixelShader();
XStatus destroyAllPixelShaders();
XStatus createMovie(XS8* pFilename,XHandle& movieHandle);
XStatus destroyMovie(XU32 movieIndex);
XStatus playMovie(XU32 movieIndex,int playCount);
XStatus pauseMovie(XU32 movieIndex);
XStatus resumeMovie(XU32 movieIndex);
XStatus stopMovie(XU32 movieIndex);
XStatus setMovieTime(XU32 movieIndex,XU32 milliseconds);
XStatus setMoviePlayCount(XU32 movieIndex,int playCount);
XU32 getMovieWidth(XU32 movieIndex);
XU32 getMovieHeight(XU32 movieIndex);
XBOOL isMoviePlaying(XU32 movieIndex);
XStatus destroyAllMovies();
XStatus enableLighting();
XStatus disableLighting();
XBOOL isLightingEnabled();
XStatus enableClipping();
XStatus disableClipping();
XBOOL isClippingEnabled();
XStatus enableZWrite();
XStatus disableZWrite();
XBOOL isZWriteEnabled();
XStatus enableAlphaTest();
XStatus disableAlphaTest();
XBOOL isAlphaTestEnabled();
XStatus enableLastPixel();
XStatus disableLastPixel();
XBOOL isLastPixelEnabled();
XStatus enableDither();
XStatus disableDither();
XBOOL isDitherEnabled();
XStatus enableAlphaBlend();
XStatus disableAlphaBlend();
XBOOL isAlphaBlendEnabled();
XStatus enableFog();
XStatus disableFog();	
XBOOL isFogEnabled();
XStatus enableSpecular();
XStatus disableSpecular();
XBOOL isSpecularEnabled();
XStatus enableRangeFog();
XStatus disableRangeFog();
XBOOL isRangeFogEnabled();
XStatus enableStencilBuffer();
XStatus disableStencilBuffer();
XBOOL isStencilBufferEnabled();
XStatus enableColorVertex();
XStatus disableColorVertex();
XBOOL isColorVertexEnabled();
XStatus enableLocalViewer();
XStatus disableLocalViewer();
XBOOL isLocalViewerEnabled();
XStatus enableNormalizeNormals();
XStatus disableNormalizeNormals();
XBOOL isNormalizeNormalsEnabled();
XStatus enableSoftwareVertexProcessing();
XStatus disableSoftwareVertexProcessing();
XBOOL isSoftwareVertexProcessingEnabled();
XStatus enablePointSprites();
XStatus disablePointSprites();
XBOOL isPointSpritesEnabled();
XStatus enablePointScale();
XStatus disablePointScale();
XBOOL isPointScaleEnabled();
XStatus enableMultiSampleAntialias();
XStatus disableMultiSampleAntialias();
XBOOL isMultiSampleAntialiasEnabled();
XStatus enableIndexedVertexBlend();
XStatus disableIndexedVertexBlend();
XBOOL isIndexedVertexBlendEnabled();
XStatus setDepthBuffer(X_DEPTH_BUFFER_TYPE value);
X_DEPTH_BUFFER_TYPE getDepthBuffer();
XStatus setShadeMode(X_SHADE_MODE_TYPE value);
X_SHADE_MODE_TYPE getShadeMode();
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
XStatus setAlphaRef(XU32 value);
XU32 getAlphaRef();	
XStatus setFogColor(XColor value);
XColor getFogColor();	
XStatus setFogStart(XF32 value);
XF32 getFogStart();	
XStatus setFogEnd(XF32 value);
XF32 getFogEnd();	
XStatus setFogDensity(XF32 value);
XF32 getFogDensity();	
XStatus setFogTableMode(X_FOG_MODE_TYPE value);
X_FOG_MODE_TYPE getFogTableMode();	
XStatus setDepthBias(XF32 value);
XF32 getDepthBias();	
XStatus setStencilFail(X_STENCIL_OP_TYPE value);
X_STENCIL_OP_TYPE getStencilFail();	
XStatus setStencilZFail(X_STENCIL_OP_TYPE value);
X_STENCIL_OP_TYPE getStencilZFail();	
XStatus setStencilPass(X_STENCIL_OP_TYPE value);
X_STENCIL_OP_TYPE getStencilPass();	
XStatus setStencilFunc(X_CMP_FUNC_TYPE value);
X_CMP_FUNC_TYPE getStencilFunc();	
XStatus setStencilRef(int value);
int getStencilRef();	
XStatus setStencilMask(XU32 value);
XU32 getStencilMask();	
XStatus setStencilWriteMask(XU32 value);
XU32 getStencilWriteMask();	
XStatus setTextureFactor(XU32 value);
XU32 getTextureFactor();	
XStatus setWrap0(XU32 value);
XU32 getWrap0();	
XStatus setWrap1(XU32 value);
XU32 getWrap1();	
XStatus setWrap2(XU32 value);
XU32 getWrap2();	
XStatus setWrap3(XU32 value);
XU32 getWrap3();	
XStatus setWrap4(XU32 value);
XU32 getWrap4();	
XStatus setWrap5(XU32 value);
XU32 getWrap5();
XStatus setWrap6(XU32 value);
XU32 getWrap6();	
XStatus setWrap7(XU32 value);
XU32 getWrap7();	
XStatus setAmbientLight(XColor value);
XColor getAmbientLight();	
XStatus setFogVertexMode(X_FOG_MODE_TYPE value);
X_FOG_MODE_TYPE getFogVertexMode();	
XStatus setDiffuseMatSource(X_MATERIAL_COLOR_SOURCE_TYPE value);
X_MATERIAL_COLOR_SOURCE_TYPE getDiffuseMatSource();	
XStatus setSpecularMatSource(X_MATERIAL_COLOR_SOURCE_TYPE value);
X_MATERIAL_COLOR_SOURCE_TYPE getSpecularMatSource();	
XStatus setAmbientMatSource(X_MATERIAL_COLOR_SOURCE_TYPE value);
X_MATERIAL_COLOR_SOURCE_TYPE getAmbientMatSource();	
XStatus setEmissiveMatSource(X_MATERIAL_COLOR_SOURCE_TYPE value);
X_MATERIAL_COLOR_SOURCE_TYPE getEmissiveMatSource();	
XStatus setVertexBlend(X_VERTEX_BLEND_FLAGS_TYPE value);
X_VERTEX_BLEND_FLAGS_TYPE getVertexBlend();	
XStatus setPointSize(XF32 value);
XF32 getPointSize();	
XStatus setPointSizeMin(XF32 value);
XF32 getPointSizeMin();	
XStatus setPointScaleA(XF32 value);
XF32 getPointScaleA();	
XStatus setPointScaleB(XF32 value);
XF32 getPointScaleB();	
XStatus setPointScaleC(XF32 value);
XF32 getPointScaleC();	
XStatus setMultiSampleMask(XU32 value);
XU32 getMultiSampleMask();	
XStatus setPatchEdgeStyle(X_PATCH_EDGE_STYLE_TYPE value);
X_PATCH_EDGE_STYLE_TYPE getPatchEdgeStyle();	
XStatus setPatchSegments(XF32 value);
XF32 getPatchSegments();	
XStatus setPointSizeMax(XF32 value);
XF32 getPointSizeMax();	
XStatus setTweenFactor(XF32 value);
XF32 getTweenFactor();	
XStatus setBlendOp(X_BLEND_OP_TYPE value);
X_BLEND_OP_TYPE getBlendOp();
XStatus setColorOp(XU32 texStage,X_TEXTURE_OP_TYPE value);
X_TEXTURE_OP_TYPE getColorOp(XU32 texStage);
XStatus setColorArg1(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getColorArg1(XU32 texStage);
XStatus setColorArg2(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getColorArg2(XU32 texStage);
XStatus setAlphaOp(XU32 texStage,X_TEXTURE_OP_TYPE value);
X_TEXTURE_OP_TYPE getAlphaOp(XU32 texStage);
XStatus setAlphaArg1(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getAlphaArg1(XU32 texStage);
XStatus setAlphaArg2(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getAlphaArg2(XU32 texStage);
XStatus setBumpEnvMat00(XU32 texStage,XF32 value);
XF32 getBumpEnvMat00(XU32 texStage);
XStatus setBumpEnvMat01(XU32 texStage,XF32 value);
XF32 getBumpEnvMat01(XU32 texStage);
XStatus setBumpEnvMat10(XU32 texStage,XF32 value);
XF32 getBumpEnvMat10(XU32 texStage);
XStatus setBumpEnvMat11(XU32 texStage,XF32 value);
XF32 getBumpEnvMat11(XU32 texStage);
XStatus setTexCoordIndex(XU32 texStage,XU32 value);
XU32 getTexCoordIndex(XU32 texStage);
XStatus setAddressU(XU32 texStage,X_TEXTURE_ADDRESS_TYPE value);
X_TEXTURE_ADDRESS_TYPE getAddressU(XU32 texStage);
XStatus setAddressV(XU32 texStage,X_TEXTURE_ADDRESS_TYPE value);
X_TEXTURE_ADDRESS_TYPE getAddressV(XU32 texStage);
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
XStatus setMaxMipLevel(XU32 texStage,XU32 value);
XU32 getMaxMipLevel(XU32 texStage);
XStatus setMaxAnisotropy(XU32 texStage,XU32 value);
XU32 getMaxAnisotropy(XU32 texStage);
XStatus setBumpEnvLScale(XU32 texStage,XF32 value);
XF32 getBumpEnvLScale(XU32 texStage);
XStatus setBumpEnvLOffset(XU32 texStage,XF32 value);
XF32 getBumpEnvLOffset(XU32 texStage);
XStatus setTextureTransformFlags(XU32 texStage,X_TEXTURE_TRANSFORM_FLAG_TYPE value);
X_TEXTURE_TRANSFORM_FLAG_TYPE getTextureTransformFlags(XU32 texStage);
XStatus setAddressW(XU32 texStage,X_TEXTURE_ADDRESS_TYPE value);
X_TEXTURE_ADDRESS_TYPE getAddressW(XU32 texStage);
XStatus setColorArg0(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getColorArg0(XU32 texStage);
XStatus setAlphaArg0(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getAlphaArg0(XU32 texStage);
XStatus setResultArg0(XU32 texStage,X_TEXTURE_ARGUMENT_TYPE value);
X_TEXTURE_ARGUMENT_TYPE getResultArg0(XU32 texStage);
XU32 getMaxTextureWidth();
XU32 getMaxTextureHeight();
XU32 getMaxSimultaneousTextures();
XU32 getMaxTextureStages();
XU32 getMaxTextureRepeat();
XU32 getMaxActiveLights();
X_PIXEL_SHADER_VERSION getMaxPixelShaderVersion();
X_VERTEX_SHADER_VERSION getMaxVertexShaderVersion();
void* getDeviceCaps();


*/
