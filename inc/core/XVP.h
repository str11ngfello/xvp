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
//	Module Name:	XVP.h
//	Author:			Lynn Duke
//	Creation Date:	4-29-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#ifndef __XVP_H__
#define __XVP_H__




//*****************************************************************************
//! X-VP interface and its drivers are exposed through this object.
/**
The XVP object exposes the core interface of the engine as well as
the Graphics, Sound, Input and Network driver interfaces.  
*/
//*****************************************************************************
class XVP
{

public:
		
	//*****************************************************************************
	//*****************************************************************************
	X_STATS
	(
		struct XStats
		{
			XU32 numFrustumCullsPerFrame;
		};	
		static XStats stats;
	)

	//*************************************************************************
	//Managers/Factories
	//*************************************************************************
	static XGraphicsMgr		GraphicsMgr;	//Graphics driver
	static XAudioMgr		AudioMgr;		//Audio driver
	static XInputMgr		InputMgr;		//Input driver
	static XNetworkMgr		NetworkMgr;		//Network driver

	static XObjectFactory<XShaderController>	ShaderFactory;	//Shader factory

	static void (*m_pPhysicsUpdateCallback)(XF32 dt);

private:

	//*****************************************************************************
	//*****************************************************************************
	enum X_UPDATE_STATE
	{
		X_US_NORMAL,
		X_US_CONTROLLERS,
	};
	
	//*****************************************************************************
	//*****************************************************************************
	enum X_ALPHA_QUEUE_MODE
	{
		X_AQM_RENDER_STATE,
		X_AQM_PRIORITY
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
	struct XControllerCmd
	{
		X_CMD_TYPE		cmdType;
		XController*	pController;
	};	
	
	//*****************************************************************************
	//*****************************************************************************
	struct XShaderControllerCmd
	{
		X_CMD_TYPE			cmdType;
		XShaderController*	pShaderController;
	};	

	//*****************************************************************************
	//*****************************************************************************
	struct XStateCmd
	{
		X_CMD_TYPE	cmdType;
		XState*	pState;
		void*		pUserData;
	};

	//*****************************************************************************
	//*****************************************************************************
	struct XShaderControllerTableEntry
	{
		XS8 name[X_MAX_SHADER_CONTROLLER_NAME_LENGTH];
		XShaderController* pShaderController;

		XShaderControllerTableEntry()
		{
			name[0] = '\0';
			pShaderController = NULL;
		}
	};
	

	//*************************************************************************
	//Constructors/Destructors
	//*************************************************************************
	XVP();
	~XVP();

	//*****************************************************************************
	//Resource Arrays
	//*****************************************************************************
	static XResourceList			m_charSets;

	//*****************************************************************************
	//X-VP State Variables
	//*****************************************************************************
	static X_UPDATE_STATE				m_updateState;
	static XMemBlockList<XControllerCmd>		m_controllerCmdQueue;
	static XMemBlockList<XShaderControllerCmd>		m_shaderControllerCmdQueue;
	static XBOOL						m_paused;
	static char						m_mediaPath[X_MAX_PATH_SIZE];
	static XTimer						m_mainTimer;
	static XF32						m_frameDt;
	static XF32						m_totalRunningTime;
	
	
	static XMemBlockList<XController*>			m_controllerArray;
	static XMemBlockList<XShaderController*>			m_shaderControllerArray;
	static XStatus					m_status;
	static XBOOL						m_inited;
	static XHandle					m_systemCharSet;
	static XHandle					m_jaxConnection;
	static XHandle					m_jaxListener; 

	//*****************************************************************************
	//State Data
	//*****************************************************************************
	static XMemBlockList<XState*>		m_preUpdateStateList;
	static XMemBlockList<XStateCmd>		m_preUpdateStateCmdQueue;
	static XMemBlockList<XState*>		m_postUpdateStateList;
	static XMemBlockList<XStateCmd>	m_postUpdateStateCmdQueue;
	static XMemBlockList<XState*>		m_postRenderStateList;
	static XMemBlockList<XStateCmd>	m_postRenderStateCmdQueue;

	//*****************************************************************************
	//Private Interface
	//*****************************************************************************
	static XStatus initManagers(XVPDesc& desc);
	static XStatus generalInit(XVPDesc& desc);
	static void incrementFrustumCull();
	static void processControllerCmds(XF32 dt);
	static void processShaderControllerCmds(XF32 dt);
	static void processPreUpdateStateCmds();
	static void processPostUpdateStateCmds();
	static void processPostRenderStateCmds();
	//static XStatus render();


public:
	
	//*************************************************************************
	//General Public Interface Functions.
	//*************************************************************************
	static XStatus addController(XController* pController);
	static XStatus removeController(XController* pController);
	static XStatus addShaderController(XShaderController* pShaderController);
	static XStatus removeShaderController(XShaderController* pShaderController);
	//static XStatus addScene(XScene* pScene);
	//static XStatus removeScene(XScene* pScene);
	static XStatus update();
	static XStatus generateNormals(XHandle vbHandle,XHandle ibHandle);
	static XStatus generateNormals(XHandle vbHandle);
	static XStatus init(void* pProcessHandle);
	//XStatus dumpStatsToFile(XS8* pFilename);
	static XStatus setMediaPath(XS8* pPath);
	static XS8* getMediaPath();
	static XStatus resetTime();
	static XF32 getFrameDt();
	static XF32 getDuration();
	static XStatus destroy();
	static XStatus pause();
	static XStatus resume();
	static XU32 getFPS(XF32 dt);
	static XBOOL isInitialized();
	static XStatus startProfiler();
	static XStatus startConsole();
	static XStatus setPhysicsUpdateCallback(void (*callback)(XF32 dt));
	
	//*************************************************************************
	//State Functions.
	//*************************************************************************
	static XStatus pushPreUpdateState(XState* pState,void* pUserData = NULL);
	static XStatus pushPostUpdateState(XState* pState,void* pUserData = NULL);
	static XStatus pushPostRenderState(XState* pState,void* pUserData = NULL);
	static XStatus popPreUpdateState();
	static XStatus popPostUpdateState();
	static XStatus popPostRenderState();
	static XU32 getNumPreUpdateStates();
	static XU32 getNumPostUpdateStates();
	static XU32 getNumPostRenderStates();
	static XBOOL isPreUpdateStateRegistered(XState* pState);
	static XBOOL isPostUpdateStateRegistered(XState* pState);
	static XBOOL isPostRenderStateRegistered(XState* pState);

	//*************************************************************************
	//Material API functions.
	//*************************************************************************
	/*static XStatus createMaterial(XS8* materialName,XHandle& matHandle);
	static XStatus cloneMaterial(XHandle matToClone,XHandle& matHandle);
	static XStatus destroyMaterial(XHandle matHandle);
	static XHandle getMaterial(XS8* materialName);
	static XStatus setMaterial(XHandle matHandle);
	static XMaterial* getMaterialInterface(XHandle matHandle);
	static XStatus destroyAllMaterials();
	static XS8* getMaterialName(XHandle matHandle);
	static XStatus addRenderEvent(XSceneNode* pSceneNode,XHandle matHandle,void* pUserData);
	static XStatus addMaterialRef(XHandle matHandle);*/

	//*************************************************************************
	//Charset API functions.
	//*************************************************************************
	static XStatus createCharSet(XS8* pTextureFilename,XS8* pCharDataFile,
							  XHandle& csHandle);
	static XStatus destroyCharSet(XHandle csHandle);
	static XCharSet* getCharSetInterface(XHandle csHandle);
	static XStatus destroyAllCharSets();

	
	static X_NET_MSG_ACTION_TYPE jaxListenCallback(X_NET_MSG_TYPE msgType,XNetAddress& clientAddress,
																   XHandle tcpConnHandle,
																   void* pUserData);

};


#endif
