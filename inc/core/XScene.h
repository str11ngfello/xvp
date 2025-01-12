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
//	Module Name:	XScene.h
//	Author:			Lynn Duke
//	Creation Date:	3-15-2007
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __X_SCENE_H__
#define __X_SCENE_H__

class XUserInterface;

#define X_MAX_UPDATE_LEVELS					64
#define X_MAX_RENDER_LEVELS					32

#define X_MAX_SCENE_NAME_LEN				32
#define X_MAX_SCENE_NAME_SIZE				X_MAX_SCENE_NAME_LEN+1

class XCamera;


enum X_SELECTION_FILTER
{
	X_SF_NONE,
	X_SF_ACTOR
};


//*****************************************************************************
//Update Level Description
//*****************************************************************************
struct XUpdateLevelDesc
{
	XS8 name[X_MAX_SCENE_NAME_SIZE];
	//XU32 maxUpdateEvents;

	XUpdateLevelDesc()
	{
		name[0] = '\0';
	//	maxUpdateEvents = 0;
	}

};

//*****************************************************************************
//Render Level Description
//*****************************************************************************
struct XRenderLevelDesc
{
	XS8 name[X_MAX_SCENE_NAME_SIZE];
	X_SORT_TYPE sortType;
	XU32 maxRenderEvents;
	XBOOL autoAlphaManagement;
	XS32 depthBufferSamplerSlot;
	
	XRenderLevelDesc()
	{
		name[0] = '\0';
		sortType = X_ST_NONE;
		maxRenderEvents = 0;
		autoAlphaManagement = false;
		depthBufferSamplerSlot = -1;
	}
};

//*****************************************************************************
//Scene Description
//*****************************************************************************
struct XSceneDesc
{
	XS8 name[X_MAX_SCENE_NAME_SIZE];
	XU32 matrixStackSize;

	XViewportArrayDesc viewportArrayDesc;
	XRenderTargetArrayDesc renderTargetArrayDesc;
	
	XBOOL enableResizeEvents;

	//XBOOL enableUpdates;
	XU32 numUpdateLevels;
	XUpdateLevelDesc updateLevelDescs[X_MAX_UPDATE_LEVELS];

	//XBOOL enableRenders;
	XS32 numRenderLevels;
	XRenderLevelDesc renderLevelDescs[X_MAX_RENDER_LEVELS];
	XS32 autoAlphaRenderLevel;
	//XU32 maxPreRenderEvents;
	XBOOL enableColorClear;
	XBOOL enableDepthClear;
	XBOOL enableStencilClear;
	
	XSceneDesc()
	{
		name[0] = '\0';
		matrixStackSize = 256;
		enableResizeEvents = false;
		//enableUpdates = true;
		//enableRenders = true;
		numUpdateLevels = 0;
		numRenderLevels = 0;
		autoAlphaRenderLevel = -1;
		//maxPreRenderEvents = 0;
		enableColorClear = true;
		enableDepthClear = true;
		enableStencilClear = true;
	}
};


//*****************************************************************************
//! Abstracts input from logical actions.
/**
Action maps provide a layer of separation between logical actions and the ways
in which those logical actions are triggered.
*/
//*****************************************************************************
class XScene
{
private:

	//*****************************************************************************
	//*****************************************************************************
	enum X_UPDATE_STATE
	{
		X_US_NORMAL,
		X_US_SPRITES,
		X_US_USER_INTERFACES,
		X_US_SCENE_GRAPH,
		X_US_CONTROLLERS,
		X_US_RENDER
	};
	
	struct SceneConstants
	{
		XMatrix4x4		viewMatrix;
		XMatrix4x4		invViewMatrix;
		XMatrix4x4		projMatrix;
		XMatrix4x4		invProjMatrix;
		XMatrix4x4		viewProjMatrix;
		XMatrix4x4		invViewProjMatrix;
		XVector4D		viewportParams; //width, height, xoffset, yoffset..all normalized
		XVector4D		v4FogParams; //x = fog start,y = fog end,z = fog density, w = frame dt
		XVector4D		v4FogColor; //xyz = rgb color, w = total elapsed time
		XVector4D		v4GlobalAmbient;
		
		SceneConstants()
		{
			v4FogParams.x = 0.0f;
			v4FogParams.y = 1000.0f;
			v4FogColor.x = 1.0f;
			v4FogColor.y = 1.0f;
			v4FogColor.z = 1.0f;
			v4FogColor.w = 0.0f;
		}
	};

	

	//*****************************************************************************
	//*****************************************************************************
	class XSpriteInfo
	{
	public:
		XU32 zOrder;
		XBaseSprite* pSprite;

		XBOOL XSpriteInfo::operator == (XSpriteInfo S)
		{
			if (S.pSprite == pSprite)
				return true;
			else
				return false;
		}
	};

	//*****************************************************************************
	//*****************************************************************************
	class XUIInfo
	{
	public:
		XU32 zOrder;
		XUserInterface* pUI;

		XBOOL XUIInfo::operator == (XUIInfo UI)
		{
			if (UI.pUI == pUI)
				return true;
			else
				return false;
		}
	};

	//*****************************************************************************
	//*****************************************************************************
	enum X_CMD_TYPE
	{
		X_CMDT_ADD,
		X_CMDT_REMOVE,
	};

	//*****************************************************************************
	//*****************************************************************************
	struct XSpriteCmd
	{
		X_CMD_TYPE		cmdType;
		XSpriteInfo		info;
	};	
	
	//*****************************************************************************
	//*****************************************************************************
	struct XUICmd
	{
		X_CMD_TYPE		cmdType;
		XUIInfo			info;
	};	

	//*****************************************************************************
	//*****************************************************************************
	struct XNodeCmd
	{
		X_CMD_TYPE		cmdType;
		XSceneNode*		pNode;
		XU32			level;
		XBOOL			update;
	};	

	struct XNormalizedViewport
	{
		XF32 left;
		XF32 top;
		XF32 right;
		XF32 bottom;
	};

	X_UPDATE_STATE				m_updateState;
	XMemBlockList<XNodeCmd>		m_nodeCmdQueue;
	XBOOL						m_renderSprites;
	XFreeArray<XMatrix4x4>		m_matrixStack;
	XMemBlockList<XSpriteInfo>	m_frontSpriteArray;
	XMemBlockList<XSpriteInfo>	m_backSpriteArray;
	XMemBlockList<XSpriteCmd>	m_spriteCmdQueue;
	XMemBlockList<XUIInfo>		m_UIArray;
	XMemBlockList<XUICmd>		m_UICmdQueue;

	XBOOL						m_enableResizeEvents;

	//XBOOL						m_enableUpdates;
	XU32						m_numUpdateLevels;
	XBOOL*						m_pUpdateLevelEnabled;
	XUpdateLevelDesc*			m_pUpdateLevelDescs;
	XMemBlockList<XSceneNode*>*	m_pUpdateNodeLists;
	XMemBlockList<XSceneNode*>*	m_pRenderNodeLists;
	XMemBlockList<XSceneNode*>	m_gizmoList;

	//XU32*						m_pSceneNodeCounts;

	//XBOOL						m_enableRenders;
	XU32						m_numRenderLevels;
	XBOOL*						m_pRenderLevelEnabled;
	XRenderLevelDesc*			m_pRenderLevelDescs;
	XRenderEvent**				m_pRenderEventLists;
	XU32*						m_pRenderEventCounts;
	//XU32						m_maxPreRenderEvents;
	//XU32						m_numPreRenderEvents;
	//XSceneNode**				m_pPreRenderEvents;
	XU32						m_activeRenderLevel;
	XMemBlockList<XSceneNode*>	m_selectedNodes;


	//XMemBlockList<XRenderEvent>	m_renderEventList;
	//XHandle*					m_pMaterialList;					

	XHandle					m_spriteMaterial;
	XHandle					m_spriteVertexShader;
	XHandle					m_spritePixelShader;

	X_SELECTION_FILTER			m_selectionFilter;

	XLightMgr					m_lightMgr;		

	XGizmoMgr				m_gizmoMgr;

	XHandle					m_selMatHandle;
	XMaterialColorSC	m_selectionSC;

	static XVector3D					m_activeCameraPosition;
		XBOOL						m_alphaSort;

	XColor			m_fogColor;
	XF32				m_fogStart;
	XF32				m_fogEnd;
	XF32				m_fogDensity;
	XColor				m_globalAmbient;
	//Scene viewports
	XCamera*					m_pViewportCameras[X_MAX_VIEWPORTS];
	XCamera*					m_pActiveCamera;
	XS32						m_activeViewportIndex;
	//XMemBlockList<XPostEffect*>	m_viewportPostEffects[X_MAX_VIEWPORTS];
	//XViewportArrayDesc			m_orgViewportArrayDesc;
	XViewportArrayDesc			m_viewportArrayDesc;
	XHandle						m_viewportRTAs[2][X_MAX_VIEWPORTS];
	XS8							m_lastRTA[X_MAX_VIEWPORTS];
	XNormalizedViewport			m_normalizedViewports[X_MAX_VIEWPORTS];
	XBOOL						m_wireFillMode[X_MAX_VIEWPORTS];
	XHandle						m_wireMatHandle;
	XBOOL						m_enableClear;

	XHandle						m_renderContext;
	XHandle						m_renderTargetArray;
	
	XHandle						m_spriteVBHandle;


	XHandle						m_VSSceneConstants;
	XHandle						m_GSSceneConstants;
	XHandle						m_PSSceneConstants;

	void createViewportRenderTargets(XViewportArrayDesc& desc);

	XBOOL m_enableColorClear;
	XBOOL m_enableDepthClear;
	XBOOL m_enableStencilClear;

	XBOOL m_enable;


	XHandle m_pointClampState;

	struct TEX_OVERRIDE
	{
		XHandle rta;
		XU32 rtIndex;
		XHandle samplerState;

		TEX_OVERRIDE()
		{
			rta = X_INVALID_HANDLE;
			samplerState = X_INVALID_HANDLE;
		}
	};
	TEX_OVERRIDE m_texOverrides[X_MAX_SAMPLERS];
	XHandle m_materialOverride;

	void (*m_pPreRenderCallback)(XScene* pScene);
	void (*m_pPostRenderCallback)(XScene* pScene);
	XBOOL m_inited;
public:
	//*****************************************************************************
	//Constructors/Destructors
	//*****************************************************************************
	XScene();
	virtual ~XScene();

	//*****************************************************************************
	//Public Interface
	//*****************************************************************************
	XStatus init(XSceneDesc& desc);
	XStatus destroy();
	XStatus update(XF32 dt);
	virtual XStatus postUpdate(XF32 dt);
	XStatus render(XF32 dt);
	XStatus renderPostEffects(XF32 dt);
	virtual XStatus postRender(XF32 dt);
	//XStatus addPreRenderEvent(XSceneNode* pSceneNode);
	XStatus addRenderEvent(XSceneNode* pSceneNode,void* pEventData,XHandle materialHandle);
	XStatus setPreRenderCallback(void (*callback)(XScene* pScene));
	XStatus setPostRenderCallback(void (*callback)(XScene* pScene));
	XHandle getSelectionMaterial();
	XMaterialColorSC* getSelectionMaterialSC();
	XStatus enableWireShading(XU32 viewportIndex,XBOOL enableWire);
	

	//XMatrix4x4& getProjMatrix();
	//XMatrix4x4& getViewMatrix();
	//XStatus setProjMatrix(XMatrix4x4& projMatrix);
	//XStatus setViewMatrix(XMatrix4x4& viewMatrix);
	XBOOL isEnabled();
	XStatus enable(XBOOL enable);
	XBOOL isInitialized();
	//XStatus addAlphaRenderEvent(XRenderEvent* pRenderEvent);
	XStatus enableAlphaSorting(XBOOL enable);
	XBOOL isAlphaSortingEnabled();
	static int sortBackToFront(const void* pNode1,const void* pNode2); 
	static int sortFrontToBack(const void* pNode1,const void* pNode2); 
	
	XStatus setViewportArray(XViewportArrayDesc& desc);
	XStatus getViewportArray(XViewportArrayDesc& desc);
	XStatus setViewportCamera(XU32 viewportIndex,XCamera* pCamera);
	XCamera* getViewportCamera(XU32 viewportIndex);
	XStatus removeAllViewportCameras();
	XCamera* getActiveCamera();
	XS32 getActiveViewportIndex();
	XLightMgr* getLightMgr();
	XHandle getRenderTargetArray();

	XHandle getRenderContext();
	XStatus setRenderContext(XHandle renderContext);
	virtual XStatus handleResizeEvent(XU32 width,XU32 height);
	
	XStatus enableClears(XBOOL color,XBOOL depth,XBOOL stencil);

	XU32 getNumUpdateLevels();
	XMemBlockList<XSceneNode*>* getUpdateLevel(XU32 levelIndex);
	XU32 getNumRenderLevels();
	XMemBlockList<XSceneNode*>* getRenderLevel(XU32 levelIndex);
	XBOOL isUpdateLevelEnabled(XU32 updateLevelIndex);
	XStatus enableUpdateLevel(XU32 updateLevelIndex,XBOOL enable);
	XBOOL isRenderLevelEnabled(XU32 renderLevelIndex);
	XStatus enableRenderLevel(XU32 renderLevelIndex,XBOOL enable);
	XStatus setTextureOverrideFromRTA(XU32 samplerIndex,XHandle samplerState,XHandle renderTargetArray,XU32 renderTargetIndex);
	XStatus removeTextureOverride(XU32 samplerIndex);
	XStatus setMaterialOverride(XHandle matHandle);
	XStatus removeMaterialOverride();

	XColor& getFogColor();
	void setFogColor(XColor& color);
	XF32 getFogStart();
	void setFogStart(XF32 start);
	XF32 getFogEnd();
	void setFogEnd(XF32 end);
	XF32 getFogDensity();
	void setFogDensity(XF32 density);
	XColor& getGlobalAmbient();
	void setGlobalAmbient(XColor& ambient);


	//*************************************************************************
	//Scene Graph API functions.
	//*************************************************************************
	XStatus addUpdateNode(XSceneNode* pNode,XU32 updateLevel = 0);
	XStatus removeUpdateNode(XSceneNode* pNode,XU32 updateLevel);
	XStatus removeUpdateNode(XSceneNode* pNode);
	XStatus removeAllUpdateNodes(XU32 updateLevel);
	XStatus removeAllUpdateNodes();
	XBOOL containsUpdateNode(XSceneNode* pNode,XU32 updateLevel);
	XBOOL containsUpdateNode(XSceneNode* pNode);
	XStatus addRenderNode(XSceneNode* pNode,XU32 renderLevel = 0);
	XStatus removeRenderNode(XSceneNode* pNode,XU32 renderLevel);
	XStatus removeRenderNode(XSceneNode* pNode);
	XStatus removeAllRenderNodes(XU32 renderLevel);
	XStatus removeAllRenderNodes();
	XBOOL containsRenderNode(XSceneNode* pNode,XU32 renderLevel);
	XBOOL containsRenderNode(XSceneNode* pNode);

	//*************************************************************************
	//Sprite API functions.
	//*************************************************************************
	XStatus addSpriteToScene(XBaseSprite* pSprite,int zOrder = -1);
	XStatus removeSpriteFromScene(XBaseSprite* pSprite);
	XBOOL isSpriteInScene(XBaseSprite* pSprite);
	XStatus setSpriteZOrder(XBaseSprite* pSprite,XU32 zOrder);
	XU32 getNumSprites();
	XBOOL getSpriteZOrder(XBaseSprite* pSprite,int& zOrder);
	XStatus enableSprites();
	XStatus disableSprites();

	//*************************************************************************
	//UI API functions.
	//*************************************************************************
	XStatus addUIToScene(XUserInterface* pUI,int zOrder = -1);
	XStatus removeUIFromScene(XUserInterface* pUI);
	XBOOL isUIInScene(XUserInterface* pUI);
	XStatus setUIZOrder(XUserInterface* pUI,XU32 zOrder);
	XU32 getNumUIs();
	XBOOL getUIZOrder(XUserInterface* pUI,int& zOrder);
	

	void processSpriteCmds(XF32 dt);
	void processUICmds();
	void processNodeCmds();
	void beginSpriteRendering();
	void endSpriteRendering();

	XFreeArray<XMatrix4x4>* getMatrixStack();
	XU32 getMatrixStackCapacity();

	XStatus pickSceneNodes(XMemBlockList<HitInfo>& hits,XS32 screenX,
							   XS32 screenY);
	XMemBlockList<XSceneNode*>* getSelectedNodes();
	XStatus selectSceneNodes(XS32 screenX,XS32 screenY,XBOOL multiple = false);
	XStatus clearSelection();
	XStatus invertSelection();
	XStatus selectAll();
	XStatus setSelectionFilter(X_SELECTION_FILTER filter);
	XStatus addGizmoNode(XSceneNode* pNode);
	XStatus removeGizmoNode(XSceneNode* pNode);
	XStatus setGizmoMode(X_GIZMO_MODE mode);
	XStatus updateGizmoHover(XS32 x,XS32 y);
	XStatus updateGizmoDrag(XF32 startX,XF32 startY,XF32 endX,XF32 endY);
	XStatus gizmoOnMouseUp();
	XSceneNode* selectGizmoNode(XS32 screenX,XS32 screenY);
	XStatus pickGizmoNodes(XMemBlockList<HitInfo>& hits,XS32 screenX,XS32 screenY);
	XGizmoMgr* getGizmoMgr();
	static XSceneGraphNode* onLevelDown(XSceneGraphNode* pNode,void* pUserData);


};


#endif