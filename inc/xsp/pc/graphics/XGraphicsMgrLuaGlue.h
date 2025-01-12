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
//	Module Name:	XGraphicsMgrLuaGlue.h
//	Author:			Lynn Duke
//	Creation Date:	5-29-2005
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_GRAPHICS_MGR_LUA_GLUE_H__
#define __X_GRAPHICS_MGR_LUA_GLUE_H__

#include <XOSMgr.h>
extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#ifdef X_CONSOLE
int LAPI_graphics(lua_State* pLuaState);
#endif
/*
int LAPI_getScreenWidth(lua_State* pLuaState);
int LAPI_getScreenHeight(lua_State* pLuaState);
int LAPI_setClearColor(lua_State* pLuaState);
int LAPI_getClearColor(lua_State* pLuaState);
int LAPI_setDepthClearValue(lua_State* pLuaState);
int LAPI_getDepthClearValue(lua_State* pLuaState);
int LAPI_setStencilClearValue(lua_State* pLuaState);
int LAPI_getStencilClearValue(lua_State* pLuaState);*/
int LAPI_dumpScreenshot(lua_State* pLuaState);
int LAPI_setRedGammaPercent(lua_State* pLuaState);
int LAPI_setBlueGammaPercent(lua_State* pLuaState);
int LAPI_setGreenGammaPercent(lua_State* pLuaState);
int LAPI_setRGBGammaPercent(lua_State* pLuaState);
int LAPI_invertRGBGamma(lua_State* pLuaState);
int LAPI_getRedGammaPercent(lua_State* pLuaState);
int LAPI_getBlueGammaPercent(lua_State* pLuaState);
int LAPI_getGreenGammaPercent(lua_State* pLuaState);
int LAPI_enableAlphaSorting(lua_State* pLuaState);
/*int LAPI_createLight(lua_State* pLuaState);
int LAPI_destroyLight(lua_State* pLuaState);
int LAPI_setLightDiffuse(lua_State* pLuaState);
int LAPI_setLightSpecular(lua_State* pLuaState);
int LAPI_setLightAmbient(lua_State* pLuaState);
int LAPI_setLightPosition(lua_State* pLuaState);
int LAPI_setLightDirection(lua_State* pLuaState);;
int LAPI_setLightRange(lua_State* pLuaState);
int LAPI_setLightFalloff(lua_State* pLuaState);
int LAPI_setLightConstantAtt(lua_State* pLuaState);
int LAPI_setLightLinearAtt(lua_State* pLuaState);
int LAPI_setLightQuadraticAtt(lua_State* pLuaState);
int LAPI_setLightInnerConeAngle(lua_State* pLuaState);
int LAPI_setLightOuterConeAngle(lua_State* pLuaState);
int LAPI_getLightDiffuse(lua_State* pLuaState);
int LAPI_getLightSpecular(lua_State* pLuaState);
int LAPI_getLightAmbient(lua_State* pLuaState);
int LAPI_getLightPosition(lua_State* pLuaState);
int LAPI_getLightDirection(lua_State* pLuaState);
int LAPI_getLightRange(XHandle lightHandle);
int LAPI_getLightFalloff(XHandle lightHandle);
int LAPI_getLightConstantAtt(XHandle lightHandle);
int LAPI_getLightLinearAtt(XHandle lightHandle);
int LAPI_getLightQuadraticAtt(XHandle lightHandle);
int LAPI_getLightInnerConeAngle(XHandle lightHandle);
int LAPI_getLightOuterConeAngle(XHandle lightHandle);
int LAPI_enableLight(XHandle lightHandle,XU32 lightIndex);
int LAPI_disableLight(XHandle lightHandle);
int LAPI_destroyAllLights(lua_State* pLuaState);
int LAPI_getNumLights(lua_State* pLuaState);
int LAPI_createMaterial(lua_State* pLuaState);
int LAPI_destroyMaterial(lua_State* pLuaState);
int LAPI_setMaterialDiffuse(lua_State* pLuaState);
int LAPI_setMaterialAmbient(lua_State* pLuaState);
int LAPI_setMaterialEmissive(lua_State* pLuaState);
int LAPI_setMaterialSpecular(lua_State* pLuaState);
int LAPI_setMaterialSpecPower(lua_State* pLuaState);
int LAPI_getMaterialDiffuse(lua_State* pLuaState);
int LAPI_getMaterialAmbient(lua_State* pLuaState);
int LAPI_getMaterialEmissive(lua_State* pLuaState);
int LAPI_getMaterialSpecular(lua_State* pLuaState);
int LAPI_getMaterialSpecPower(lua_State* pLuaState);
int LAPI_setMaterial(lua_State* pLuaState);
int LAPI_destroyAllMaterials(lua_State* pLuaState);
int LAPI_getNumMaterials(lua_State* pLuaState);
int LAPI_createIndexBuffer(lua_State* pLuaState);
int LAPI_lockIndexBuffer(lua_State* pLuaState);
int LAPI_unlockIndexBuffer(lua_State* pLuaState);
int LAPI_destroyIndexBuffer(lua_State* pLuaState);
int LAPI_getIndexBufferLength(lua_State* pLuaState);
int LAPI_getNumIndexBuffers(lua_State* pLuaState);
int LAPI_destroyAllIndexBuffers(lua_State* pLuaState);
int LAPI_createVertexBuffer(lua_State* pLuaState);
int LAPI_destroyVertexBuffer(lua_State* pLuaState);
int LAPI_renderVertexBuffer(lua_State* pLuaState);
int LAPI_renderVertexBuffer(lua_State* pLuaState);
int LAPI_renderUserVertexBuffer(lua_State* pLuaState);
int LAPI_lockVertexBuffer(lua_State* pLuaState);
int LAPI_unlockVertexBuffer(lua_State* pLuaState);
int LAPI_getVertexBufferLength(lua_State* pLuaState);
int LAPI_getVertexBufferType(lua_State* pLuaState);
int LAPI_getVertexBufferPrimType(lua_State* pLuaState);
int LAPI_getNumVertexBuffers(lua_State* pLuaState);
int LAPI_destroyAllVertexBuffers(lua_State* pLuaState);
int LAPI_createTexture(lua_State* pLuaState);
int LAPI_destroyTexture(lua_State* pLuaState);
int LAPI_setTexture(lua_State* pLuaState);
int LAPI_removeTexture(lua_State* pLuaState);
int LAPI_renderTexture(lua_State* pLuaState);
int LAPI_getTextureWidth(lua_State* pLuaState);
int LAPI_getTextureHeight(lua_State* pLuaState);
int LAPI_getTextureDepth(lua_State* pLuaState);
int LAPI_getNumTextures(lua_State* pLuaState);
int LAPI_getTextureFilename(lua_State* pLuaState);
int LAPI_destroyAllTextures(lua_State* pLuaState);
int LAPI_createVertexShader(lua_State* pLuaState);
int LAPI_destroyVertexShader(lua_State* pLuaState);
int LAPI_setVertexShader(lua_State* pLuaState);
int LAPI_setVertexShaderConstant(lua_State* pLuaState);
int LAPI_removeVertexShader(lua_State* pLuaState);
int LAPI_destroyAllVertexShaders(lua_State* pLuaState);
int LAPI_createPixelShader(lua_State* pLuaState);
int LAPI_destroyPixelShader(lua_State* pLuaState);
int LAPI_setPixelShader(lua_State* pLuaState);
int LAPI_setPixelShaderConstant(lua_State* pLuaState);
int LAPI_removePixelShader(lua_State* pLuaState);
int LAPI_destroyAllPixelShaders(lua_State* pLuaState);
int LAPI_createMovie(lua_State* pLuaState);
int LAPI_destroyMovie(lua_State* pLuaState);
int LAPI_playMovie(lua_State* pLuaState);
int LAPI_pauseMovie(lua_State* pLuaState);
int LAPI_resumeMovie(lua_State* pLuaState);
int LAPI_stopMovie(lua_State* pLuaState);
int LAPI_setMovieTime(lua_State* pLuaState);
int LAPI_setMoviePlayCount(lua_State* pLuaState);
int LAPI_getMovieWidth(lua_State* pLuaState);
int LAPI_getMovieHeight(lua_State* pLuaState);
int LAPI_isMoviePlaying(lua_State* pLuaState);
int LAPI_destroyAllMovies(lua_State* pLuaState);
int LAPI_enableLighting(lua_State* pLuaState);
int LAPI_disableLighting(lua_State* pLuaState);
int LAPI_isLightingEnabled(lua_State* pLuaState);
int LAPI_enableClipping(lua_State* pLuaState);
int LAPI_disableClipping(lua_State* pLuaState);
int LAPI_isClippingEnabled(lua_State* pLuaState);
int LAPI_enableZWrite(lua_State* pLuaState);
int LAPI_disableZWrite(lua_State* pLuaState);
int LAPI_isZWriteEnabled(lua_State* pLuaState);
int LAPI_enableAlphaTest(lua_State* pLuaState);
int LAPI_disableAlphaTest(lua_State* pLuaState);
int LAPI_isAlphaTestEnabled(lua_State* pLuaState);
int LAPI_enableLastPixel(lua_State* pLuaState);
int LAPI_disableLastPixel(lua_State* pLuaState);
int LAPI_isLastPixelEnabled(lua_State* pLuaState);
int LAPI_enableDither(lua_State* pLuaState);
int LAPI_disableDither(lua_State* pLuaState);
int LAPI_isDitherEnabled(lua_State* pLuaState);
int LAPI_enableAlphaBlend(lua_State* pLuaState);
int LAPI_disableAlphaBlend(lua_State* pLuaState);
int LAPI_isAlphaBlendEnabled(lua_State* pLuaState);
int LAPI_enableFog(lua_State* pLuaState);
int LAPI_disableFog(lua_State* pLuaState);	
int LAPI_isFogEnabled(lua_State* pLuaState);
int LAPI_enableSpecular(lua_State* pLuaState);
int LAPI_disableSpecular(lua_State* pLuaState);
int LAPI_isSpecularEnabled(lua_State* pLuaState);
int LAPI_enableRangeFog(lua_State* pLuaState);
int LAPI_disableRangeFog(lua_State* pLuaState);
int LAPI_isRangeFogEnabled(lua_State* pLuaState);
int LAPI_enableStencilBuffer(lua_State* pLuaState);
int LAPI_disableStencilBuffer(lua_State* pLuaState);
int LAPI_isStencilBufferEnabled(lua_State* pLuaState);
int LAPI_enableColorVertex(lua_State* pLuaState);
int LAPI_disableColorVertex(lua_State* pLuaState);
int LAPI_isColorVertexEnabled(lua_State* pLuaState);
int LAPI_enableLocalViewer(lua_State* pLuaState);
int LAPI_disableLocalViewer(lua_State* pLuaState);
int LAPI_isLocalViewerEnabled(lua_State* pLuaState);
int LAPI_enableNormalizeNormals(lua_State* pLuaState);
int LAPI_disableNormalizeNormals(lua_State* pLuaState);
int LAPI_isNormalizeNormalsEnabled(lua_State* pLuaState);
int LAPI_enableSoftwareVertexProcessing(lua_State* pLuaState);
int LAPI_disableSoftwareVertexProcessing(lua_State* pLuaState);
int LAPI_isSoftwareVertexProcessingEnabled(lua_State* pLuaState);
int LAPI_enablePointSprites(lua_State* pLuaState);
int LAPI_disablePointSprites(lua_State* pLuaState);
int LAPI_isPointSpritesEnabled(lua_State* pLuaState);
int LAPI_enablePointScale(lua_State* pLuaState);
int LAPI_disablePointScale(lua_State* pLuaState);
int LAPI_isPointScaleEnabled(lua_State* pLuaState);
int LAPI_enableMultiSampleAntialias(lua_State* pLuaState);
int LAPI_disableMultiSampleAntialias(lua_State* pLuaState);
int LAPI_isMultiSampleAntialiasEnabled(lua_State* pLuaState);
int LAPI_enableIndexedVertexBlend(lua_State* pLuaState);
int LAPI_disableIndexedVertexBlend(lua_State* pLuaState);
int LAPI_isIndexedVertexBlendEnabled(lua_State* pLuaState);
int LAPI_setDepthBuffer(lua_State* pLuaState);
int LAPI_getDepthBuffer(lua_State* pLuaState);
int LAPI_setShadeMode(lua_State* pLuaState);
int LAPI_getShadeMode(lua_State* pLuaState);
int LAPI_setFillMode(lua_State* pLuaState);
int LAPI_getFillMode(lua_State* pLuaState);
int LAPI_setSrcBlend(lua_State* pLuaState);
int LAPI_getSrcBlend(lua_State* pLuaState);
int LAPI_setDestBlend(lua_State* pLuaState);
int LAPI_getDestBlend(lua_State* pLuaState);
int LAPI_setCullMode(lua_State* pLuaState);
int LAPI_getCullMode(lua_State* pLuaState);
int LAPI_setDepthFunc(lua_State* pLuaState);
int LAPI_getDepthFunc(lua_State* pLuaState);
int LAPI_setAlphaFunc(lua_State* pLuaState);
int LAPI_getAlphaFunc(lua_State* pLuaState);	
int LAPI_setAlphaRef(lua_State* pLuaState);
int LAPI_getAlphaRef(lua_State* pLuaState);
int LAPI_setFogColor(lua_State* pLuaState);
int LAPI_getFogColor(lua_State* pLuaState);	
int LAPI_setFogStart(lua_State* pLuaState);
int LAPI_getFogStart(lua_State* pLuaState);	
int LAPI_setFogEnd(lua_State* pLuaState);
int LAPI_getFogEnd(lua_State* pLuaState);	
int LAPI_setFogDensity(lua_State* pLuaState);
int LAPI_getFogDensity(lua_State* pLuaState);	
int LAPI_setFogTableMode(lua_State* pLuaState);
int LAPI_getFogTableMode(lua_State* pLuaState);	
int LAPI_setDepthBias(lua_State* pLuaState);
int LAPI_getDepthBias(lua_State* pLuaState);	
int LAPI_setStencilFail(lua_State* pLuaState);
int LAPI_getStencilFail(lua_State* pLuaState);	
int LAPI_setStencilZFail(lua_State* pLuaState);
int LAPI_getStencilZFail(lua_State* pLuaState);	
int LAPI_setStencilPass(lua_State* pLuaState);
int LAPI_getStencilPass(lua_State* pLuaState);	
int LAPI_setStencilFunc(lua_State* pLuaState);
int LAPI_getStencilFunc(lua_State* pLuaState);	
int LAPI_setStencilRef(lua_State* pLuaState);
int LAPI_getStencilRef(lua_State* pLuaState);	
int LAPI_setStencilMask(lua_State* pLuaState);
int LAPI_getStencilMask(lua_State* pLuaState);	
int LAPI_setStencilWriteMask(lua_State* pLuaState);
int LAPI_getStencilWriteMask(lua_State* pLuaState);	
int LAPI_setTextureFactor(lua_State* pLuaState);
int LAPI_getTextureFactor(lua_State* pLuaState);	
int LAPI_setWrap0(lua_State* pLuaState);
int LAPI_getWrap0(lua_State* pLuaState);	
int LAPI_setWrap1(lua_State* pLuaState);
int LAPI_getWrap1(lua_State* pLuaState);	
int LAPI_setWrap2(lua_State* pLuaState);
int LAPI_getWrap2(lua_State* pLuaState);	
int LAPI_setWrap3(lua_State* pLuaState);
int LAPI_getWrap3(lua_State* pLuaState);	
int LAPI_setWrap4(lua_State* pLuaState);
int LAPI_getWrap4(lua_State* pLuaState);
int LAPI_setWrap5(lua_State* pLuaState);
int LAPI_getWrap5(lua_State* pLuaState);
int LAPI_setWrap6(lua_State* pLuaState);
int LAPI_getWrap6(lua_State* pLuaState);
int LAPI_setWrap7(lua_State* pLuaState);
int LAPI_getWrap7(lua_State* pLuaState);
int LAPI_setAmbientLight(lua_State* pLuaState);
int LAPI_getAmbientLight(lua_State* pLuaState);
int LAPI_setFogVertexMode(lua_State* pLuaState);
int LAPI_getFogVertexMode(lua_State* pLuaState);
int LAPI_setDiffuseMatSource(lua_State* pLuaState);
int LAPI_getDiffuseMatSource(lua_State* pLuaState);
int LAPI_setSpecularMatSource(lua_State* pLuaState);
int LAPI_getSpecularMatSource(lua_State* pLuaState);	
int LAPI_setAmbientMatSource(lua_State* pLuaState);
int LAPI_getAmbientMatSource(lua_State* pLuaState);	
int LAPI_setEmissiveMatSource(lua_State* pLuaState);
int LAPI_getEmissiveMatSource(lua_State* pLuaState);	
int LAPI_setVertexBlend(lua_State* pLuaState);
int LAPI_getVertexBlend(lua_State* pLuaState);	
int LAPI_setPointSize(lua_State* pLuaState);
int LAPI_getPointSize(lua_State* pLuaState);	
int LAPI_setPointSizeMin(lua_State* pLuaState);
int LAPI_getPointSizeMin(lua_State* pLuaState);	
int LAPI_setPointScaleA(lua_State* pLuaState);
int LAPI_getPointScaleA(lua_State* pLuaState);	
int LAPI_setPointScaleB(lua_State* pLuaState);
int LAPI_getPointScaleB(lua_State* pLuaState);	
int LAPI_setPointScaleC(lua_State* pLuaState);
int LAPI_getPointScaleC(lua_State* pLuaState);	
int LAPI_setMultiSampleMask(lua_State* pLuaState);
int LAPI_getMultiSampleMask(lua_State* pLuaState);	
int LAPI_setPatchEdgeStyle(lua_State* pLuaState);
int LAPI_getPatchEdgeStyle(lua_State* pLuaState);	
int LAPI_setPatchSegments(lua_State* pLuaState);
int LAPI_getPatchSegments(lua_State* pLuaState);
int LAPI_setPointSizeMax(lua_State* pLuaState);
int LAPI_getPointSizeMax(lua_State* pLuaState);	
int LAPI_setTweenFactor(lua_State* pLuaState);
int LAPI_getTweenFactor(lua_State* pLuaState);	
int LAPI_setBlendOp(lua_State* pLuaState);
int LAPI_getBlendOp(lua_State* pLuaState);
int LAPI_setColorOp(lua_State* pLuaState);
int LAPI_getColorOp(lua_State* pLuaState);
int LAPI_setColorArg1(lua_State* pLuaState);
int LAPI_getColorArg1(lua_State* pLuaState);
int LAPI_setColorArg2(lua_State* pLuaState);
int LAPI_getColorArg2(lua_State* pLuaState);
int LAPI_setAlphaOp(lua_State* pLuaState);
int LAPI_getAlphaOp(lua_State* pLuaState);
int LAPI_setAlphaArg1(lua_State* pLuaState);
int LAPI_getAlphaArg1(lua_State* pLuaState);
int LAPI_setAlphaArg2(lua_State* pLuaState);
int LAPI_getAlphaArg2(lua_State* pLuaState);
int LAPI_setBumpEnvMat00(lua_State* pLuaState);
int LAPI_getBumpEnvMat00(lua_State* pLuaState);
int LAPI_setBumpEnvMat01(lua_State* pLuaState);
int LAPI_getBumpEnvMat01(lua_State* pLuaState);
int LAPI_setBumpEnvMat10(lua_State* pLuaState);
int LAPI_getBumpEnvMat10(lua_State* pLuaState);
int LAPI_setBumpEnvMat11(lua_State* pLuaState);
int LAPI_getBumpEnvMat11(lua_State* pLuaState);
int LAPI_setTexCoordIndex(lua_State* pLuaState);
int LAPI_getTexCoordIndex(lua_State* pLuaState);
int LAPI_setAddressU(lua_State* pLuaState);
int LAPI_getAddressU(lua_State* pLuaState);
int LAPI_setAddressV(lua_State* pLuaState);
int LAPI_(lua_State* pLuaState);
int LAPI_setBorderColor(lua_State* pLuaState);
int LAPI_getBorderColor(lua_State* pLuaState);
int LAPI_setMagFilter(lua_State* pLuaState);
int LAPI_getMagFilter(lua_State* pLuaState);
int LAPI_setMinFilter(lua_State* pLuaState);
int LAPI_getMinFilter(lua_State* pLuaState);
int LAPI_setMipFilter(lua_State* pLuaState);
int LAPI_getMipFilter(lua_State* pLuaState);
int LAPI_setMipmapLodBias(lua_State* pLuaState);
int LAPI_getMipmapLodBias(lua_State* pLuaState);
int LAPI_setMaxMipLevel(lua_State* pLuaState);
int LAPI_getMaxMipLevel(lua_State* pLuaState);
int LAPI_setMaxAnisotropy(lua_State* pLuaState);
int LAPI_getMaxAnisotropy(lua_State* pLuaState);
int LAPI_setBumpEnvLScale(lua_State* pLuaState);
int LAPI_getBumpEnvLScale(lua_State* pLuaState);
int LAPI_setBumpEnvLOffset(lua_State* pLuaState);
int LAPI_getBumpEnvLOffset(lua_State* pLuaState);
int LAPI_setTextureTransformFlags(lua_State* pLuaState);
int LAPI_getTextureTransformFlags(lua_State* pLuaState);
int LAPI_setAddressW(lua_State* pLuaState);
int LAPI_getAddressW(lua_State* pLuaState);
int LAPI_setColorArg0(lua_State* pLuaState);
int LAPI_getColorArg0(lua_State* pLuaState);
int LAPI_setAlphaArg0(lua_State* pLuaState);
int LAPI_getAlphaArg0(lua_State* pLuaState);
int LAPI_setResultArg0(lua_State* pLuaState);
int LAPI_getResultArg0(lua_State* pLuaState);
int LAPI_getMaxTextureWidth(lua_State* pLuaState);
int LAPI_getMaxTextureHeight(lua_State* pLuaState);
int LAPI_getMaxSimultaneousTextures(lua_State* pLuaState);
int LAPI_getMaxTextureStages(lua_State* pLuaState);
int LAPI_getMaxTextureRepeat(lua_State* pLuaState);
int LAPI_getMaxActiveLights(lua_State* pLuaState);
int LAPI_getMaxPixelShaderVersion(lua_State* pLuaState);
int LAPI_getMaxVertexShaderVersion(lua_State* pLuaState);
int LAPI_getDeviceCaps(lua_State* pLuaState);
*/


#endif