// XVPManaged.h

#pragma once
#include "xvpsdk.h"
#include "ProxyObject.h"

#using <System.Drawing.DLL>
using namespace System;
using namespace System::Drawing;


#define MAX_CONTROLS			32

//Should match VIEWPORT_COLOR in ViewportConfig
#define VIEWPORT_COLOR			XColor(.38f,.38f,.38f,1.0f)  
#define INITIAL_ZOOM			40
#define INITIAL_ORTHO_ZOOM		.16f


enum
{
	UL_DEFAULT,
	NUM_UPDATE_LEVELS
};

enum
{
	RL_OPAQUE,
	RL_OCCLUSION,
	RL_ALPHA,
	RL_PARTICLES,
	RL_DEBUG,
	NUM_RENDER_LEVELS
};

namespace XVPManaged 
{
	//This enum must match the managed version exactly
	enum ViewType
	{
		VT_PERSPECTIVE,
		VT_LEFT,
		VT_TOP,
		VT_RIGHT,
		VT_BOTTOM,
		VT_FRONT,
		VT_BACK
	};

	struct SceneView
	{
		XCamera cameras[4];
		XCamera* userCameras[4];
		XCamera distortionCameras[4];
		XPRZController PRZControllers[4];

		XGrid grids[4];
		XVertexColorSC gridSC;
		XF32 gridUnitSize;
		XU32 gridSize;
		XU32 gridMajorLine;

		ViewType viewTypes[4];
		XHDRPostEffect hdrPostEffects[4];

		XLight scenarioLights[3];
		XLightingMode currentLightingMode;
		XF32 lightAngle;
		XF32 lightAnimAngle;
		XBOOL animateLights;
		XF32 lightAnimSpeed;


		SceneView()
		{
			for (XU32 i = 0;i < 4;++i)
				userCameras[i] = NULL;

			currentLightingMode = X_LM_DEFAULT;
			lightAngle = 0.0f;
			lightAnimAngle = 0.0f;
			lightAnimSpeed = 0.0f;
		}
	};

	XHandle m_shadowSampler;
	XS8 m_systemPath[1024];
	XS8 m_shaderPath[1024];

	XScene* m_pScenes[MAX_CONTROLS];
	XScene* m_pDistortionScenes[MAX_CONTROLS];
	XHandle m_renderContexts[MAX_CONTROLS];
	SceneView m_sceneViews[MAX_CONTROLS];
	XStringMap<XScene*>	m_sceneMap[MAX_CONTROLS];

	XBOOL m_inited = false;
	XBOOL inputEnabled = false;

	//XShaderController* pSC;
	//XParticleSystemBlinnPhongSC psmc;
	//XParticleSystemTemplate pst;
	//XParticleSystem ps[MAX_CONTROLS];
	//XShaderController* m_pShaderControllers[MAX_SHADER_CONTROLLERS];

	public ref class XVPM
	{
	private:
		static void setCameraViewOrientation(XPRZController& przController,ViewType viewType,bool preserveZoom);

	public:

		XVPM();
		~XVPM();

		static void init(IntPtr hInstance);
		static bool inited();
		static void destroy();
		static void update(XF32 dt);
		static XU32 addWindow(IntPtr windowHandle,XU32 width,XU32 height);
		static void removeWindow(XU32 windowHandle);
		static void resizeWindow(XU32 windowHandle,XU32 width,XU32 height);
		static void activate(XU32 windowHandle,XBOOL activate);
		static XBOOL isGridShown(XU32 windowHandle,XU32 viewportIndex);
		static void showGrid(XU32 windowHandle,XU32 viewportIndex,XBOOL show);
		static void configureGrid(XU32 windowHandle,XF32 unitSize,XU32 gridSize,XU32 majorLine);
		static XU32 getGridSize(XU32 windowHandle);
		static XF32 getGridUnitSize(XU32 windowHandle);
		static XU32 getGridMajorLine(XU32 windowHandle);
		static XS32 getViewType(XU32 windowHandle,XU32 viewportIndex);
		static void setViewType(XU32 windowHandle,XU32 viewportIndex,XS32 viewType,XBOOL preserveZoom);
		static void addCameraRotation(XU32 windowHandle,XU32 viewportIndex,XF32 xAngle,XF32 yAngle);
		static void addCameraPan(XU32 windowHandle,XU32 viewportIndex,XS32 startX,XS32 startY,XS32 endX,XS32 endY);
		static void addCameraZoom(XU32 windowHandle,XU32 viewportIndex,XF32 distance);
		static void getCameraRotation(XU32 windowHandle,XS32 viewportIndex,IntPtr rotation);
		static void getCameraPan(XU32 windowHandle,XS32 viewportIndex,IntPtr pan);
		static XF32 getCameraZoom(XU32 windowHandle,XS32 viewportIndex);
		static void setCameraRotation(XU32 windowHandle,XU32 viewportIndex,IntPtr rotation);
		static void setCameraPan(XU32 windowHandle,XU32 viewportIndex,IntPtr pan);
		static void setCameraZoom(XU32 windowHandle,XU32 viewportIndex,XF32 distance);
		static void setCameraOrthoType(XU32 windowHandle,XU32 viewportIndex,XS32 viewType);
		static void enableInput(XU32 windowHandle,XU32 viewportIndex);
		static void disableInput(XU32 windowHandle);
		static void selectSceneNodes(XU32 windowHandle,XS32 x,XS32 y,XBOOL multiple);
		static void selectAll(XU32 windowHandle);
		static void invertSelection(XU32 windowHandle);
		static void clearSelection(XU32 windowHandle);
		static void zoomSelection(XU32 windowHandle,XU32 viewportIndex);
		static void zoomAll(XU32 windowHandle,XU32 viewportIndex);
		static void setSelectionColor(XU32 windowHandle,XF32 r,XF32 g,XF32 b);
		static XU32 getNumSelectedNodes(XU32 windowHandle);
		static void enableWireShading(XU32 windowHandle,XU32 viewportIndex,XBOOL enable);
		static void setBackgroundColor(XU32 windowHandle,XF32 r,XF32 g,XF32 b);
		static void setBackgroundColor(XU32 windowHandle,XColor& color);
		static void setLayout(XU32 windowHandle,XU32 layout,System::Drawing::Rectangle v0,System::Drawing::Rectangle v1,System::Drawing::Rectangle v2,System::Drawing::Rectangle v3);
		static void resetViewports(XU32 windowHandle);
		static void resetViewportCamera(XU32 windowHandle,XU32 viewportIndex);
		static void setGizmoMode(XU32 windowHandle,XU32 mode);
		static void updateGizmoHover(XU32 windowHandle,XS32 x,XS32 y);
		static void updateGizmoDrag(XU32 windowHandle,XF32 startX,XF32 startY,XF32 endX,XF32 endY);
		static void gizmoOnMouseUp(XU32 windowHandle);
		static void getPlacementTransform(XU32 windowHandle,IntPtr transform,XU32 viewportIndex,int screenX,int screenY);
		static XBOOL isMoveSnapEnabled(XU32 windowHandle);
		static void enableMoveSnap(XU32 windowHandle,XBOOL enable);
		static XF32 getMoveSnapAmount(XU32 windowHandle);
		static void setMoveSnapAmount(XU32 windowHandle,XF32 amount);
		static XBOOL isRotateSnapEnabled(XU32 windowHandle);
		static void enableRotateSnap(XU32 windowHandle,XBOOL enable);
		static XF32 getRotateSnapAmount(XU32 windowHandle);
		static void setRotateSnapAmount(XU32 windowHandle,XF32 amount);
		static XBOOL isScaleSnapEnabled(XU32 windowHandle);
		static void enableScaleSnap(XU32 windowHandle,XBOOL enable);
		static XF32 getScaleSnapAmount(XU32 windowHandle);
		static void setScaleSnapAmount(XU32 windowHandle,XF32 amount);

		static void loadShaderWad(IntPtr pFilename);
		static XBOOL isShaderRegistered(IntPtr pShaderName);
		static XBOOL shaderExists(IntPtr pShaderName);
		static XU32 getLightingMode(XU32 windowHandle);
		static void setLightingMode(XU32 windowHandle,XU32 mode);
		static void setLightScenario(XU32 windowHandle,IntPtr pScenarioName);
		static void setScenarioLightColor(XU32 windowHandle,XU32 lightIndex,XF32 r,XF32 g,XF32 b);
		static XF32 getScenarioLightIntensity(XU32 windowHandle,XU32 lightIndex);
		static void setScenarioLightIntensity(XU32 windowHandle,XU32 lightIndex,XF32 intensity);
		static void setScenarioLightAnimSpeed(XU32 windowHandle,XF32 speed);
		static void setScenarioLightAngle(XU32 windowHandle,XS32 angle);
		static void enableShadows(XU32 windowHandle,XBOOL enable);
		static void setActorContainer(IntPtr sceneNode,IntPtr actorContainer);
		
		//Scene Properties Managed Wrapper
		static void setFogColor(XU32 windowHandle,XF32 r,XF32 g,XF32 b);
		static void setFogStart(XU32 windowHandle,XF32 fogStart);
		static void setFogEnd(XU32 windowHandle,XF32 fogEnd);
		static void setFogDensity(XU32 windowHandle,XF32 fogDensity);
		static void setGlobalAmbient(XU32 windowHandle,XF32 r,XF32 g,XF32 b);

		//Particle System Managed Wrapper 
		static IntPtr initParticleSystem(XU32 windowHandle,XU32 numEmitters);
		static void destroyParticleSystem(IntPtr particleSystem);
		static IntPtr initEmitter(IntPtr particleSystem,XU32 emitterIndex);
		static IntPtr getEmitterMaterial(IntPtr emitter);
		static void startParticleSystem(IntPtr particleSystem);
		static void stopParticleSystem(IntPtr particleSystem);
		static void restartParticleSystem(IntPtr particleSystem);
		static void addParticleSystemEmitter(IntPtr particleSystem);

		//Model Managed Wrapper
		static IntPtr initModel(XU32 windowHandle,IntPtr pFilename,XU32 numLODs);
		static void postInitModel(IntPtr model);
		static void destroyModel(IntPtr model,XU32 windowHandle);
		static void mergeModelLOD(IntPtr model,IntPtr pFilename,XF32 LODDistance);
		static void setModelLODDist(IntPtr model,XU32 LODIndex,XF32 LODDist);
		static IntPtr getModelMaterial(IntPtr model,IntPtr pMaterialName);
		static void destroyModelMaterial(IntPtr material);
		static void attachControllerToModel(IntPtr model,IntPtr controller,IntPtr pNodeName);
		static void detachControllerFromModel(IntPtr model,IntPtr controller,IntPtr pNodeName);
		static bool setScene(XU32 windowHandle,IntPtr proxy,IntPtr pSceneName);
		static void generateLightmap(array<IntPtr>^ models,array<IntPtr>^ lights,array<System::String^>^ filenames,XU32 maxSize,XU32 pixelsPerSurfAreaUnit,XU32 forcedSize,XF32 blurSize);
		static void generateAOMaps(array<IntPtr>^ models,array<System::String^>^ filenames,XU32 maxSize,XU32 quality,XU32 forcedSize,XF32 blurSize);

		//Camera Managed Wrapper
		static IntPtr initCamera(XU32 windowHandle);
		static void destroyCamera(XU32 windowHandle,IntPtr camera);
		static bool setUserCamera(XU32 windowHandle,IntPtr camera,XU32 viewportIndex);
		static void setStandardCamera(XU32 windowHandle,XU32 viewportIndex);
		static void removeAllPostEffects(IntPtr camera);
		static void addPostEffect(IntPtr camera,IntPtr postEffect);

		//Light Managed Wrapper
		static IntPtr initLight(XU32 windowHandle);
		static void destroyLight(IntPtr camera);

		//Post Effects Managed Wrapper 
		static IntPtr initHDRPostEffect(IntPtr camera);				
		static void destroyHDRPostEffect(IntPtr postEffect);
		static IntPtr initDOFPostEffect(IntPtr camera);				
		static void destroyDOFPostEffect(IntPtr postEffect);
		static IntPtr initEdgePostEffect(IntPtr camera);				
		static void destroyEdgePostEffect(IntPtr postEffect);
		static IntPtr initDistortionPostEffect(XU32 windowHandle,IntPtr camera);				
		static void destroyDistortionPostEffect(IntPtr postEffect);
		static IntPtr initMotionBlurPostEffect(XU32 windowHandle,IntPtr camera);				
		static void destroyMotionBlurPostEffect(IntPtr postEffect);

		//Animation Controller Managed Wrapper
		static IntPtr initAnimController(XU32 windowHandle);
		static void destroyAnimController(IntPtr animController);
		static void setNumAnimTracks(IntPtr animController,XU32 numAnimTracks);
		static void setAnimTrack(IntPtr animController,XU32 trackIndex,IntPtr pTrackFilename);
		static XU32 getAnimTrackTime(IntPtr animController,XU32 trackIndex);
		static XU32 getAnimTrackDuration(IntPtr animController,XU32 trackIndex);
		static void setAnimTrackTime(IntPtr animController,XU32 trackIndex,XU32 milliseconds);
		static void playAnimTrack(IntPtr animController,XU32 trackIndex,XF32 speedScale,XS32 playCount,XF32 weight);
		static void pauseAnimTrack(IntPtr animController,XU32 trackIndex);
		static void stopAnimTrack(IntPtr animController,XU32 trackIndex);
		static void stopAllTracks(IntPtr animController);
		static void setAnimTrackSpeed(IntPtr animController,XU32 trackIndex,XF32 speed);
		static void setAnimTrackPlayCount(IntPtr animController,XU32 trackIndex,XS32 playCount);
		static void setAnimTrackWeight(IntPtr animController,XU32 trackIndex,XF32 weight);

		//Actor Container Managed Wrapper
		static IntPtr initActorContainer(XU32 windowHandle);
		static void destroyActorContainer(IntPtr actorContainer);
		static XF32* getActorContainerTransform(IntPtr actorContainer);

		//PhysX Controller Managed Wrapper
		static IntPtr initPhysXController(XU32 windowHandle);
		static void destroyPhysXController(IntPtr physXController);
		static void addForce(IntPtr physXController,XF32 x,XF32 y,XF32 z);

		//Property Bag Managed Wrapper
		static void setIntProperty(IntPtr pProxyObject,IntPtr pPropertyName,XS32 value);
		static void setUIntProperty(IntPtr pProxyObject,IntPtr pPropertyName,XU32 value);
		static void setFloatProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 value);
		static void setDoubleProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF64 value);
		static void setBoolProperty(IntPtr pProxyObject,IntPtr pPropertyName,XBOOL value);
		static void setColorProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 r,XF32 g,XF32 b,XF32 a);
		static void setEnumProperty(IntPtr pProxyObject,IntPtr pPropertyName,XS32 value);
		static void setVector2DProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 x,XF32 y);
		static void setVector3DProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 x,XF32 y,XF32 z);
		static void setVector4DProperty(IntPtr pProxyObject,IntPtr pPropertyName,XF32 x,XF32 y,XF32 z,XF32 w);
		static void setMaterialShaderName(IntPtr pProxyObject,IntPtr pShaderName);
		static void	setMaterialTexture(IntPtr pProxyObject,XU32 samplerIndex,IntPtr pFilename);
		static void	setSamplerAddressU(IntPtr pProxyObject,XU32 samplerIndex,XS32 addressType);
		static void	setSamplerAddressV(IntPtr pProxyObject,XU32 samplerIndex,XS32 addressType);
		static void	setSamplerAddressW(IntPtr pProxyObject,XU32 samplerIndex,XS32 addressType);
		static void	setSamplerMipLODBias(IntPtr pProxyObject,XU32 samplerIndex,XF32 bias);
		static void	setSamplerMaxAnisotropy(IntPtr pProxyObject,XU32 samplerIndex,XU32 maxAnisotropy);
		static void	setSamplerMinLOD(IntPtr pProxyObject,XU32 samplerIndex,XF32 minLOD);
		static void	setSamplerMaxLOD(IntPtr pProxyObject,XU32 samplerIndex,XF32 maxLOD);
		static void	setSamplerBorderColor(IntPtr pProxyObject,XU32 samplerIndex,XF32 r,XF32 g,XF32 b,XF32 a);
		static void	setSamplerFilter(IntPtr pProxyObject,XU32 samplerIndex,XS32 filterType);
		static void	setSamplerFunc(IntPtr pProxyObject,XU32 samplerIndex,XS32 comparisonFunc);
	};
}
