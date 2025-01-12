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
//	Module Name:	XVP.cpp
//	Author:			Lynn Duke
//	Creation Date:	4-29-2001
//
//	Description:	
//
//*****************************************************************************
//*****************************************************************************

#include <xvpsdk.h>


#define X_SYSTEM_FONT				"sysFont.png"
#define X_SYSTEM_FONT_METRICS		"sysFont.ini"

XGraphicsMgr XVP::GraphicsMgr;	
XAudioMgr XVP::AudioMgr;	
XNetworkMgr XVP::NetworkMgr;	
XInputMgr XVP::InputMgr;	

XResourceList			XVP::m_charSets;
XVP::X_UPDATE_STATE				XVP::m_updateState = X_US_NORMAL;
XMemBlockList<XVP::XControllerCmd>		XVP::m_controllerCmdQueue;
XMemBlockList<XVP::XShaderControllerCmd>		XVP::m_shaderControllerCmdQueue;
XBOOL						XVP::m_paused = false;
char						XVP::m_mediaPath[X_MAX_PATH_SIZE] = "";
XTimer						XVP::m_mainTimer;
XF32						XVP::m_frameDt = 0;
XF32						XVP::m_totalRunningTime = 0;
XMemBlockList<XController*>			XVP::m_controllerArray;
XMemBlockList<XShaderController*>			XVP::m_shaderControllerArray;
XStatus					XVP::m_status;

XMemBlockList<XState*>		XVP::m_preUpdateStateList;
XMemBlockList<XVP::XStateCmd>	XVP::m_preUpdateStateCmdQueue;
XMemBlockList<XState*>		XVP::m_postUpdateStateList;
XMemBlockList<XVP::XStateCmd>	XVP::m_postUpdateStateCmdQueue;
XMemBlockList<XState*>		XVP::m_postRenderStateList;
XMemBlockList<XVP::XStateCmd>	XVP::m_postRenderStateCmdQueue;

XBOOL					XVP::m_inited = false;
XHandle		XVP::m_systemCharSet = X_INVALID_HANDLE;
XHandle		XVP::m_jaxConnection = X_INVALID_HANDLE;
XHandle		XVP::m_jaxListener = X_INVALID_HANDLE;
XObjectFactory<XShaderController>		XVP::ShaderFactory;
void (*XVP::m_pPhysicsUpdateCallback)(XF32 dt);

//XBOOL		XVP::m_profilerAvailable = false;

X_STATS
(
 XVP::XStats		XVP::stats;
)


X_NET_MSG_ACTION_TYPE jaxListenCallback(X_NET_MSG_TYPE msgType,XNetAddress& clientAddress,
																   XHandle tcpConnHandle,
																   void* pUserData);

#define X_XVP_LUA_LIB_NAME	"XVP"

static const luaL_reg XVPLuaLib[] = 
{
	{"showMethods",LAPI_xvp},
	{"setMediaPath",LAPI_setMediaPath},
	{"getMediaPath",LAPI_getMediaPath},
	{"getElapsedTime",LAPI_getElapsedTime},
	//{"getMatrixStackCapacity",LAPI_getMatrixStackCapacity},
	{"getFPS",LAPI_getFPS},
	{"pause",LAPI_pause},
	{"resume",LAPI_resume},
	/*{"pushPreUpdateState",LAPI_pushPreUpdateState},
	{"pushPostUpdateState",LAPI_pushPostUpdateState},
	{"pushPostRenderState",LAPI_pushPostRenderState},
	{"popPreUpdateState",LAPI_popPreUpdateState},
	{"popPostUpdateState",LAPI_popPostUpdateState},
	{"popPostRenderState",LAPI_popPostRenderState},
	{"getNumPreUpdateStates",LAPI_getNumPreUpdateStates},
	{"getNumPostUpdateStates",LAPI_getNumPostUpdateStates},
	{"getNumPostRenderStates",LAPI_getNumPostRenderStates},
	{"isPreUpdateStateRegistered",LAPI_isPreUpdateStateRegistered},
	{"isPostUpdateStateRegistered",LAPI_isPostUpdateStateRegistered},
	{"isPostRenderStateRegistered",LAPI_isPostRenderStateRegistered},*/
	//{"getNumSprites",LAPI_getNumSprites},
	//{"enableSprites",LAPI_enableSprites},
	//{"disableSprites",LAPI_disableSprites},
	//{"flushScene",LAPI_flushScene},
	//{"dumpScene",LAPI_dumpScene},
	{NULL,NULL}
};


XBOOL RegReadDword(HKEY root,XS8* keyName,XS8* valueName,DWORD* value)
{
	HRESULT hr;

	//First open up the key.
	HKEY currentKey;
	RegOpenKey(root,keyName,&currentKey);

	//Then read dword data from key.
	DWORD size = 4;
	hr = RegQueryValueEx(currentKey,valueName,NULL,NULL,(LPBYTE)value,&size);
	if (hr != ERROR_SUCCESS)
		return false;
	
	//Now close the key.
	hr = RegCloseKey(currentKey);
	if (hr != ERROR_SUCCESS)
		return false;


	//Everything went ok so return true.
	return true;
}

XBOOL RegCreateKey(HKEY root,XS8* keyName)
{
	HRESULT hr;
	HKEY hKey = NULL;

	hr = RegCreateKeyEx(root,keyName,0,"",REG_OPTION_NON_VOLATILE,
		                KEY_ALL_ACCESS | KEY_READ | KEY_WRITE,NULL,&hKey,NULL);
	if (hr != ERROR_SUCCESS)
		return false;

	//Be sure to close the newly created key.
	hr = RegCloseKey(hKey);
	if (hr != ERROR_SUCCESS)
		return false;

	//Everything went ok so return true.
	return true;
}

XBOOL RegWriteDword(HKEY root,XS8* keyName,XS8* valueName,DWORD value)
{
	HRESULT hr;

	//First open up the key.
	HKEY currentKey;
	RegOpenKey(root,keyName,&currentKey);

	//Then write dword data to key.
	hr = RegSetValueEx(currentKey,valueName,0,REG_DWORD,(LPBYTE)&value,sizeof(DWORD));
	if (hr != ERROR_SUCCESS)
		return false;

	//Now close the key.
	hr = RegCloseKey(currentKey);
	if (hr != ERROR_SUCCESS)
		return false;

	//Everything went ok so return true.
	return true;
}



//*****************************************************************************
/**
Private Constructor.
*/
//*****************************************************************************
XVP::XVP()
{
}



//*****************************************************************************
/**
Private Destructor.
*/
//*****************************************************************************
XVP::~XVP()
{	
}


	
//*****************************************************************************
/**
Pauses X-VP.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::pause()
{
	m_paused = true;
	return X_OK;
}



//*****************************************************************************
/**'
Resumes X-VP after a pause.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::resume()
{
	m_paused = false;
	return X_OK;
}



//*****************************************************************************
/**
Pushes a state onto X-VP's pre update state list.  Once a state has been pushed,
X-VP will continually update it at the appropriate time during each update
until it is popped off of the list.

@param pState	The state to push as a pre update state.
@param pUserData	User data to add for the state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::pushPreUpdateState(XState* pState,void* pUserData)
{
	//Make sure the state is valid.
	if (!pState)
		return X_ERR_INVALID_PARAMS;

	//Make sure the state is not already in the list or queue.
	XMemBlockList<XState*>::iterator i(m_preUpdateStateList.begin());
	while (i.isValid())
	{
		if (i.getValue() == pState)
			return X_ERR_REDUNDANT_OPERATION;
		i.getNext();
	}
	XMemBlockList<XStateCmd>::iterator j(m_preUpdateStateCmdQueue.begin());
	while (j.isValid())
	{
		if (j.getValue().pState == pState)
			return X_ERR_REDUNDANT_OPERATION;
		j.getNext();
	}
	//for (XU32 j = 0;j < m_preUpdateStateCmdQueue.getNumItems();++j)
	//	if (m_preUpdateStateCmdQueue[j]->pState == pState)
	//		return X_ERR_REDUNDANT_OPERATION;
	
	XStateCmd cmd;
	cmd.cmdType = X_CMDT_ADD;
	cmd.pState = pState;
	cmd.pUserData = pUserData;
	m_preUpdateStateCmdQueue.insertBack(cmd);
	

	return X_OK;
}



//*****************************************************************************
/**
Pushes a state onto X-VP's post update state list.  Once a state has been pushed,
X-VP will continually update it at the appropriate time during each update
until it is popped off the list.

@param pState	The state to push as a post update state.
@param pUserData	User data to add for the state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::pushPostUpdateState(XState* pState,void* pUserData)
{
	//Make sure the state is valid.
	if (!pState)
		return X_ERR_INVALID_PARAMS;

	//Make sure the state is not already in the list.
	XMemBlockList<XState*>::iterator i(m_postUpdateStateList.begin());
	while (i.isValid())
	{
		if (i.getValue() == pState)
			return X_ERR_REDUNDANT_OPERATION;
		i.getNext();
	}
	XMemBlockList<XStateCmd>::iterator j(m_postUpdateStateCmdQueue.begin());
	while (j.isValid())
	{
		if (j.getValue().pState == pState)
			return X_ERR_REDUNDANT_OPERATION;
		j.getNext();
	}

	XStateCmd cmd;
	cmd.cmdType = X_CMDT_ADD;
	cmd.pState = pState;
	cmd.pUserData = pUserData;
	
	m_postUpdateStateCmdQueue.insertBack(cmd);

	return X_OK;
}



//*****************************************************************************
/**
Pushes a state onto X-VP's post render state list.  Once a state has been pushed,
X-VP will continually update it at the appropriate time during each update
until it is popped off the list.

@param pState	The state to push as a post render state.
@param pUserData	User data to add for the state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::pushPostRenderState(XState* pState,void* pUserData)
{
	//Make sure the state is valid.
	if (!pState)
		return X_ERR_INVALID_PARAMS;

	//Make sure the state is not already in the list.
	XMemBlockList<XState*>::iterator i(m_postRenderStateList.begin());
	while (i.isValid())
	{
		if (i.getValue() == pState)
			return X_ERR_REDUNDANT_OPERATION;
		i.getNext();
	}
	XMemBlockList<XStateCmd>::iterator j(m_postRenderStateCmdQueue.begin());
	while (j.isValid())
	{
		if (j.getValue().pState == pState)
			return X_ERR_REDUNDANT_OPERATION;
		j.getNext();
	}
	
	XStateCmd cmd;
	cmd.cmdType = X_CMDT_ADD;
	cmd.pState = pState;
	cmd.pUserData = pUserData;
	
	m_postRenderStateCmdQueue.insertBack(cmd);

	return X_OK;
}



//*****************************************************************************
/**
Pops a state from X-VP's pre update state list.  This function will always
pop the last state added to the list and send it a deactivation message.
If a previous state was on the list, it get's resumed.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::popPreUpdateState()
{

	XStateCmd cmd;
	cmd.cmdType = X_CMDT_REMOVE;
	m_preUpdateStateCmdQueue.insertBack(cmd);

	return X_OK;
}



//*****************************************************************************
/**
Pops a state from X-VP's post update state list.  This function will always
pop the last state added to the list and send it a deactivation message.
If a previous state was on the list, it get's resumed.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::popPostUpdateState()
{
	XStateCmd cmd;
	cmd.cmdType = X_CMDT_REMOVE;
	
	m_postUpdateStateCmdQueue.insertBack(cmd);

	return X_OK;
}



//*****************************************************************************
/**
Pops a state from X-VP's post render state list.  This function will always
pop the last state added to the list and send it a deactivation message.
If a previous state was on the list, it get's resumed.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::popPostRenderState()
{
	XStateCmd cmd;
	cmd.cmdType = X_CMDT_REMOVE;
	
	m_postRenderStateCmdQueue.insertBack(cmd);

	return X_OK;
}



//*****************************************************************************
/**
Gets the number of pre update states that are in X-VP's pre update state list.

@return The number of pre update states.
*/
//*****************************************************************************
XU32 XVP::getNumPreUpdateStates()
{
	return m_preUpdateStateList.getNumItems();
}



//*****************************************************************************
/**
Gets the number of pre update states that are in X-VP's post update state list.

@return The number of post update states.
*/
//*****************************************************************************
XU32 XVP::getNumPostUpdateStates()
{
	return m_postUpdateStateList.getNumItems();
}



//*****************************************************************************
/**
Gets the number of pre update states that are in X-VP's post render state list.

@return The number of post render states.
*/
//*****************************************************************************
XU32 XVP::getNumPostRenderStates()
{
	return m_postRenderStateList.getNumItems();
}



//*****************************************************************************
/**
Determines if a specific pre update state is registered with X-VP.

@param pState	The state to check for.
@return True if the state is registered (has been pushed), false otherwise.
*/
//*****************************************************************************
XBOOL XVP::isPreUpdateStateRegistered(XState* pState)
{
	XMemBlockList<XState*>::iterator stateItr;
	while (stateItr.isValid())
	{
		if (stateItr.getValue() == pState)
			return true;
		stateItr.getNext();
	}
	
	return false;		
}



//*****************************************************************************
/**
Determines if a specific post update state is registered with X-VP.

@param pState	The state to check for.
@return True if the state is registered (has been pushed), false otherwise.
*/
//*****************************************************************************
XBOOL XVP::isPostUpdateStateRegistered(XState* pState)
{
	XMemBlockList<XState*>::iterator i(m_postUpdateStateList.begin());
	while (i.isValid())
	{
		if (i.getValue() == pState)
			return true;
		i.getNext();
	}
	
	return false;		
}


//*****************************************************************************
/**
Determines if a specific post render state is registered with X-VP.

@param pState	The state to check for.
@return True if the state is registered (has been pushed), false otherwise.
*/
//*****************************************************************************
XBOOL XVP::isPostRenderStateRegistered(XState* pState)
{
	XMemBlockList<XState*>::iterator i(m_postRenderStateList.begin());
	while (i.isValid())
	{
		if (i.getValue() == pState)
			return true;
		i.getNext();
	}
	
	return false;		
}


//*****************************************************************************
/**
Returns the amount of elapsed time, in seconds, since the last X-VP update.

@return The elapsed time in seconds since X-VP was last updated.
*/
//*****************************************************************************
XF32 XVP::getFrameDt()
{
	return m_frameDt;
}

XF32 XVP::getDuration()
{
	return m_totalRunningTime;
}

//*****************************************************************************
/**
Resets X-VP's main timer.

@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::resetTime()
{
	m_mainTimer.reset();
	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
/*XStatus XVP::dumpStatsToFile(XS8* pFilename)
{
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;
	
	ofstream outFile(pFilename);
	
	//Lets get the number of vertex buffers.
	XU32 vbCtr = 0;
	XU32 vbStringBytes = 0;
	for (XU32 i = 0;i < m_numVertexBuffers;i++)
		if (m_vertexBufferArray[i] != NULL)
		{
			vbCtr++;
			vbStringBytes += strlen(m_vertexBufferArray[i]->vbName);
		}

	//Now lets get the number of index buffers
	XU32 ibCtr = 0;
	XU32 ibStringBytes = 0;
	for (i = 0;i < m_numIndexBuffers;i++)
		if (m_indexBufferArray[i] != NULL)
		{
			ibCtr++;
			ibStringBytes += strlen(m_indexBufferArray[i]->ibName);
		}

	//Now lets get the number of texture objects
	XU32 tCtr = 0;
	XU32 tStringBytes = 0;
	for (i = 0;i < m_numTextures;i++)
		if (m_textureArray[i] != NULL)
		{
			tCtr++;
		}

	//Now lets get the number of sound objects
	XU32 soundCtr = 0;
	XU32 soundStringBytes = 0;
	for (i = 0;i < m_numSounds;i++)
		if (m_soundArray[i] != NULL)
		{
			soundCtr++;
			soundStringBytes += strlen(m_soundArray[i]->filePathAndName);
			soundStringBytes += strlen(m_soundArray[i]->filenameOnly);
		}

	//First off, give some general memory stats, etc...
	outFile << "***************************************************************" << endl
		    << "XEngine Layer" << endl
			<< "***************************************************************"
			<< endl << endl;
	outFile << "Memory allocated to XVertexBuffer objects: "
		    << (sizeof(XVertexBuffer) * vbCtr) + vbStringBytes << " Bytes" << endl;
	outFile << "Memory allocated to XIndexBuffer objects: "
		    << (sizeof(XIndexBuffer) * ibCtr) + ibStringBytes << " Bytes" << endl;
	outFile << "Memory allocated to XTexture objects: "
		    << (sizeof(XTexture) * tCtr) + tStringBytes << " Bytes" << endl;
	outFile << "Memory allocated to AudioSound objects: "
		    << (sizeof(AudioSound) * soundCtr) + soundStringBytes << " Bytes" << endl;
	outFile << "Approximate Memory Consumed by XEngine Layer: "
		    << (sizeof(AudioSound) * getNumSounds()) +
			   (sizeof(XCursor) * getNumSprites()) +
			   (sizeof(XSprite) * getNumSprites()) +
			   (sizeof(XVertexBuffer) * vbCtr) + vbStringBytes +
			   (sizeof(XIndexBuffer) * ibCtr) + ibStringBytes +
			   (sizeof(XTexture) * tCtr) + tStringBytes +
			   sizeof(XEngine) << " Bytes" << endl << endl;
	
	outFile << endl << endl;
	

	outFile << endl << endl;
	
	outFile << endl << endl;
	
	outFile << "Total Vertex Buffers in Use: " << vbCtr << " out of " 
	 	    << m_numVertexBuffers << endl << endl;
	for (i = 0;i < m_numVertexBuffers;i++)
		if (m_vertexBufferArray[i] != NULL)
		{
			outFile << "\tVertex Buffer Name: "
				 << m_vertexBufferArray[i]->vbName << endl;
			outFile << "\tNumber of Vertices: " 
				 << m_vertexBufferArray[i]->numVertices << endl;
			outFile << "\tReference Count: " 
				 << m_vertexBufferArray[i]->refCount << endl;
			outFile << "\tVertex Type: " 
				 << m_vertexBufferArray[i]->vbType << endl;
			outFile << "\tGDriver Vertex Buffer Index: "
				 << m_vertexBufferArray[i]->GraphicsIndex << endl << endl;
		}

	outFile << endl << endl;

	
	outFile << "Total Index Buffers in Use: " << ibCtr << " out of " 
		 << m_numIndexBuffers << endl << endl;
	for (i = 0;i < m_numIndexBuffers;i++)
		if (m_indexBufferArray[i] != NULL)
		{
			outFile << "\tIndex Buffer Name: "
				 << m_indexBufferArray[i]->ibName << endl;
			outFile << "\tNumber of Indices: " 
				 << m_indexBufferArray[i]->numIndices << endl;
			outFile << "\tReference Count: " 
				 << m_indexBufferArray[i]->refCount << endl;
			outFile << "\tGDriver Index Buffer Index: "
				 << m_indexBufferArray[i]->GraphicsIndex << endl << endl;
		}

	outFile << endl << endl;

	
	outFile << "Total Textures in Use: " << tCtr << " out of " 
		 << m_numTextures << endl << endl;
	for (i = 0;i < m_numTextures;i++)
		if (m_textureArray[i] != NULL)
		{
			outFile << "\tTexture Name: "
				 << m_textureArray[i]->filenameOnly << endl;
			outFile << "\tTexture Path and Name: " 
				 << m_textureArray[i]->filePathAndName << endl;
			outFile << "\tReference Count: " 
				 << m_textureArray[i]->refCount << endl;
		}

	outFile << endl << endl;

	
	outFile << "Total Sounds in Use: " << soundCtr << " out of " 
		 << m_numSounds << endl << endl;
	for (i = 0;i < m_numSounds;i++)
		if (m_soundArray[i] != NULL)
		{
			outFile << "\tSound Name: "
				 << m_soundArray[i]->filenameOnly << endl;
			outFile << "\tSound Path and Name: " 
				 << m_soundArray[i]->filePathAndName << endl;
			outFile << "\tReference Count: " 
				 << m_soundArray[i]->refCount << endl;
			outFile << "\tSDriver Sound Index: "
				 << m_soundArray[i]->AudioIndex << endl << endl;
		}

		outFile << endl << endl;

	
	outFile << "Total Audio Paths in Use: " << getNumAudioPaths() << " out of " 
		 << m_numAudioPaths << endl << endl;
	for (i = 0;i < m_numAudioPaths;i++)
		if (m_audioPathArray[i] != NULL)
		{
			outFile << "\tAudio Path Index : "
				 << i << endl;
			outFile << "\tSDriver Audio Path Index: " 
				 << m_audioPathArray[i]->AudioIndex << endl;
		}
		
	outFile << endl << endl;

	outFile << "***************************************************************" << endl
		    << "XGDriver Object" << endl
			<< "***************************************************************"
			<< endl << endl;

	outFile << "***************************************************************" << endl
		    << "XSDriver Object" << endl
			<< "***************************************************************"
			<< endl << endl;

	outFile << "***************************************************************" << endl
		    << "XIDriver Object" << endl
			<< "***************************************************************"
			<< endl << endl;

	outFile << "***************************************************************" << endl
		    << "XNDriver Object" << endl
			<< "***************************************************************"
			<< endl << endl;

	//Everything went ok.
	return X_OK;
}
*/


//*****************************************************************************
//*****************************************************************************
XStatus XVP::setMediaPath(XS8* pPath)
{
	//Make sure we have a valid path.
	if (!pPath)
		return X_ERR_INVALID_PARAMS;
	
	//Make sure that we aren't over the maximum buffer size
	if (strlen(pPath) > X_MAX_PATH_SIZE-2)
		return X_ERR_INVALID_PARAMS;

	strcpy(m_mediaPath,pPath);
	
	//Append a slash on the end if needed
	if (m_mediaPath[strlen(m_mediaPath)-1] != '/')
	{
		m_mediaPath[strlen(m_mediaPath)-1] = '/';
		m_mediaPath[strlen(m_mediaPath)] = '\0';
	}

	return X_OK;
}

	

//*****************************************************************************
//*****************************************************************************
XS8* XVP::getMediaPath()
{
	return m_mediaPath;
}



//*****************************************************************************
/**
Adds a controller to X-VP.  Once the controller is added, the engine
will update the controller at the appropriate times.

@param pController	The controller to register with X-VP.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::addController(XController* pController)
{
	if (!pController)
		return X_ERR_INVALID_PARAMS;

	//We can only add controllers if we are not in the controller update
	//state.
	if (m_updateState == X_US_CONTROLLERS)
	{
		XControllerCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.pController = pController;
		m_controllerCmdQueue.insertBack(cmd);
	}
	else
        m_controllerArray.insertBack(pController);

	return X_OK;
}



//*****************************************************************************
/**
Removes a controller from X-VP.  Once the controller is removed, it will
no longer be updated by the engine.

@param pController	The controller to remove from X-VP.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::removeController(XController* pController)
{
	if (!pController)
		return X_ERR_INVALID_PARAMS;

	//We can only remove controllers if we are not in the controller update
	//state.
	if (m_updateState == X_US_CONTROLLERS)
	{
		XControllerCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pController = pController;
		m_controllerCmdQueue.insertBack(cmd);
	}
	else
		m_controllerArray.remove(pController);

	return X_OK;
}



//*****************************************************************************
/**
Adds a shader controller to X-VP.  Once the shader controller is added, the engine
will update the controller at the appropriate times.

@param pShaderController	The shader controller to register with X-VP.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::addShaderController(XShaderController* pShaderController)
{
	if (!pShaderController)
		return X_ERR_INVALID_PARAMS;

	//We can only add controllers if we are not in the controller update
	//state.
	if (m_updateState == X_US_CONTROLLERS)
	{
		XShaderControllerCmd cmd;
		cmd.cmdType = X_CMDT_ADD;
		cmd.pShaderController = pShaderController;
		m_shaderControllerCmdQueue.insertBack(cmd);
	}
	else
        m_shaderControllerArray.insertBack(pShaderController);

	return X_OK;
}



//*****************************************************************************
/**
Removes a shader controller from X-VP.  Once the shader controller is removed, it will
no longer be updated by the engine.

@param pShaderController	The shader controller to remove from X-VP.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::removeShaderController(XShaderController* pShaderController)
{
	if (!pShaderController)
		return X_ERR_INVALID_PARAMS;

	//We can only remove controllers if we are not in the controller update
	//state.
	if (m_updateState == X_US_CONTROLLERS)
	{
		XShaderControllerCmd cmd;
		cmd.cmdType = X_CMDT_REMOVE;
		cmd.pShaderController = pShaderController;
		m_shaderControllerCmdQueue.insertBack(cmd);
	}
	else
		m_shaderControllerArray.remove(pShaderController);

	return X_OK;
}



//*****************************************************************************
/**
Sets a default rendering state to the rendering device.  This is used in
cases where a suitable material or texture cannot be found for the geometry being
rendered.  This is mainly for internal engine use, but can be used by anyone
to set a fail-safe renderstate before rendering geometry.

This renderstate is created at engine initialization time and destroyed when
the engine shuts down.

@return A return code of type XStatus.
*/
//*****************************************************************************
/*XStatus XVP::setDefaultMaterial()
{
	return setMaterial(m_defaultRSHandle);
}*/



//*****************************************************************************
/**
Generates vertex normals for the following vertex buffer types.

<ul>
<li> X_VT_1N_1T </li>
<li> X_VT_1N_1D_1T </li>
<li> X_VT_1N_1D_2T </li>
<li> X_VT_1N_1D_3T </li>
<li> X_VT_1N_1D_4T </li>
<li> X_VT_1N_1D_5T </li>
<li> X_VT_1N_1D_6T </li>
<li> X_VT_1N_1D_7T </li>
<li> X_VT_1N_1D_8T </li>
</ul>

If the vertex buffer passed in is not one of the above types, this function
will fail.

@param vbHandle	The handle of the vertex buffer for which to generate
						vertex normals.
@param ibHandle	The handle of the index buffer associated with the passed
						in vertex buffer.
@return A return code of type XStatus.
*/
//*****************************************************************************
/*XStatus XVP::generateNormals(XHandle vbHandle,XHandle ibHandle)
{
	//For right now, only X_VT_1N_1D_1T vertex type is supported.
	X_VERTEX_TYPE vt = GraphicsMgr.getVertexBufferType(vbHandle);
	if ((vt != X_VT_MODEL) && (vt != X_VT_SKIN))
		return X_ERR_NOT_SUPPORTED;

	//Only X_PT_TRIANGLE_LIST primitive type is supported.
	if (GraphicsMgr.getVertexBufferPrimType(vbHandle) != X_PT_TRIANGLE_LIST)
		return X_ERR_NOT_SUPPORTED;

	//Get pointers to the vertex buffer and index buffer.
	XVector3D tempVec1,tempVec2,tempVec3,tempVec4;
	XModelVertex* pVB = (XModelVertex*)GraphicsMgr.lockVertexBuffer(vbHandle);
	WORD* ib = GraphicsMgr.lockIndexBuffer(ibHandle);

	//For each vertex in the vertex array
	XU32 numVerts = GraphicsMgr.getVertexBufferLength(vbHandle);
	XU32 numIndices = GraphicsMgr.getIndexBufferLength(ibHandle);

	for (XU32 i = 0;i < numVerts;i++)
	{
		XVector3D sum(0,0,0);
		for (XU32 j = 0;j < numIndices;j+=3)
		{
			//For each face vertex, check if its the same as the vertex as in
			//our current outer vertex loop. If it is, add it to the sum.
			if ((i == ib[j]) || (i == ib[j+1]) || (i == ib[j+2]))
			{
				tempVec1 = pVB[ib[j]].pos;
				tempVec2 = pVB[ib[j+1]].pos;
				tempVec3 = tempVec2 - tempVec1;
				tempVec2 = pVB[ib[j+2]].pos;
				tempVec4 = tempVec2 - tempVec1;

				sum += tempVec3.cross(tempVec4);
			}
		}

		//Now, normalize the Normal and store in the vertex 
		if ((sum.x != 0) && (sum.y != 0) && (sum.z != 0))
			sum.normalize();

		pVB[i].normal = sum;
	}

	//Don't forget to release the vertex and index buffer Ptr's.
	GraphicsMgr.unlockVertexBuffer(vbHandle);
	GraphicsMgr.unlockIndexBuffer(ibHandle);

	//If we make it here then everything went ok.
	return X_OK;
}
*/


//*****************************************************************************
/**
Generates vertex normals for the following vertex buffer types.

<ul>
<li> X_VT_1N_1T </li>
<li> X_VT_1N_1D_1T </li>
<li> X_VT_1N_1D_2T </li>
<li> X_VT_1N_1D_3T </li>
<li> X_VT_1N_1D_4T </li>
<li> X_VT_1N_1D_5T </li>
<li> X_VT_1N_1D_6T </li>
<li> X_VT_1N_1D_7T </li>
<li> X_VT_1N_1D_8T </li>
</ul>

If the vertex buffer passed in is not one of the above types, this function
will fail.

@param vbHandle	The handle of the vertex buffer for which to generate
						vertex normals.
@return A return code of type XStatus.
*/
//*****************************************************************************
/*XStatus XVP::generateNormals(XHandle vbHandle)
{
	//For right now, only X_VT_1N_1D_1T vertex type is supported.
	X_VERTEX_TYPE vt = GraphicsMgr.getVertexBufferType(vbHandle);
	if ((vt != X_VT_MODEL) && (vt != X_VT_SKIN))
		return X_ERR_NOT_SUPPORTED;

	//Only X_PT_TRIANGLE_LIST primitive type is supported.
	if (GraphicsMgr.getVertexBufferPrimType(vbHandle) != X_PT_TRIANGLE_LIST)
		return X_ERR_NOT_SUPPORTED;

	//Get a pointer to the vertex buffer.
	XVector3D tempVec1,tempVec2,tempVec3,tempVec4;
	XModelVertex* pVB = (XModelVertex*)GraphicsMgr.lockVertexBuffer(vbHandle);
	
	//For each vertex in the vertex array
	XU32 numVerts = GraphicsMgr.getVertexBufferLength(vbHandle);
	for (XU32 i = 0;i < numVerts;i++)
	{
		XVector3D sum(0,0,0);
		for (XU32 j = 0;j < numVerts;j+=3)
		{
			//For each face vertex, check if its the same as the vertex as in
			//our current outer vertex loop. If it is, add it to the sum.
			if ((i == j) || (i == j+1) || (i == j+2))
			{
				tempVec1 = pVB[j].pos;
				tempVec2 = pVB[j+1].pos;
				tempVec3 = tempVec2 - tempVec1;
				tempVec2 = pVB[j+2].pos;
				tempVec4 = tempVec2 - tempVec1;

				sum += tempVec3.cross(tempVec4);
			}
		}

		//Now, normalize the Normal and store in the vertex 
		if ((sum.x != 0) && (sum.y != 0) && (sum.z != 0))
			sum.normalize();

		pVB[i].normal = sum;
	}

	//Don't forget to release the vertex and index buffer pointers.
	GraphicsMgr.unlockVertexBuffer(vbHandle);

	//If we make it here then everything went ok.
	return X_OK;
}
*/



#if 0

//*****************************************************************************
/**
Creates a render state.  Render states are used to hold the rendering properties
for the rendering device.  The number of render states that may exist at any
given time is virtually infinite.  

@param matHandle	A variable to receive the handle of the newly created render
				state.  
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::createMaterial(XS8* materialName,XHandle& matHandle)
{
	//First, see if the material already exists.
	/*if (materialName)
	{
		XResource* pResource = (XMaterial*)m_materials.findResourceByName(materialName,matHandle);
		if (pResource)
		{
			pResource->incRefCount();
			return X_OK;
		}
	}*/

	//Need to create a new material
	XMaterial* pMaterial = new XMaterial();
	pMaterial->init();
	matHandle = m_materials.addResource(pMaterial,materialName);

	if (matHandle != X_INVALID_HANDLE)
	{
		X_DEBUG
		(
			char buf[10];
			std::string s = "Created Render State (INDEX = ";
			s += itoa(matHandle,buf,10);
			s += ")";
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
		)
		return X_OK;
	}
	else
		return X_ERR_OPERATION_FAILED;
}



//*****************************************************************************
/**
Clones a render state.  This function will copy all render state parameters from
one render state to another including texture info.  

@param rsToClone	The handle of the render state to clone.
@param matHandle	A variable to hold the handle ofnewly created render state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::cloneMaterial(XHandle matToClone,XHandle& matHandle)
{
	/*
	//First create a new render state.
	createMaterial(matHandle);

	//Now clone it.
	XMaterial* pRS = getMaterialInterface(matHandle); 
	XMaterial* pRSToClone = getMaterialInterface(rsToClone); 

	pRS->createMaterial(GraphicsMgr.getMaterialDiffuse(pRSToClone->m_matHandle),
					 GraphicsMgr.getMaterialAmbient(pRSToClone->m_matHandle),
					 GraphicsMgr.getMaterialEmissive(pRSToClone->m_matHandle),
					 GraphicsMgr.getMaterialSpecular(pRSToClone->m_matHandle),
					 GraphicsMgr.getMaterialSpecPower(pRSToClone->m_matHandle));
						
	pRS->m_properties =	pRSToClone->m_properties;
	pRS->m_srcBlend = pRSToClone->m_srcBlend;
	pRS->m_destBlend =	pRSToClone->m_destBlend;
	pRS->m_alphaRef = pRSToClone->m_alphaRef;
	pRS->m_alphaFunc = pRSToClone->m_alphaFunc;
	pRS->m_depthBuffer = pRSToClone->m_depthBuffer;
	pRS->m_shadeMode = pRSToClone->m_shadeMode;;
	pRS->m_cullMode = pRSToClone->m_cullMode;
	pRS->m_depthFunc = pRSToClone->m_depthFunc;
	pRS->m_depthBias = pRSToClone->m_depthBias;
	pRS->m_blendOp = pRSToClone->m_blendOp;
	pRS->m_fillMode = pRSToClone->m_fillMode;
	pRS->m_stencilFail = pRSToClone->m_stencilFail;
	pRS->m_stencilZFail = pRSToClone->m_stencilZFail;
	pRS->m_stencilPass = pRSToClone->m_stencilPass;
	pRS->m_stencilFunc = pRSToClone->m_stencilFunc;
	pRS->m_stencilRef = pRSToClone->m_stencilRef;
	pRS->m_stencilMask = pRSToClone->m_stencilMask;
	pRS->m_stencilWriteMask = pRSToClone->m_stencilWriteMask;
	
	for (XU32 i = 0;i < GraphicsMgr.getMaxTextureStages();++i)
	{
		XS8* pFilename = pRSToClone->getTextureFilename(i);
		if (pFilename)
			pRS->createTextureFromFile(i,pFilename);

		pRS->m_pTextureStates[i].m_colorOp = pRSToClone->m_pTextureStates[i].m_colorOp;
		pRS->m_pTextureStates[i].m_colorArg1 = pRSToClone->m_pTextureStates[i].m_colorArg1;
		pRS->m_pTextureStates[i].m_colorArg2 = pRSToClone->m_pTextureStates[i].m_colorArg2;
		pRS->m_pTextureStates[i].m_alphaOp = pRSToClone->m_pTextureStates[i].m_alphaOp;
		pRS->m_pTextureStates[i].m_alphaArg1 = pRSToClone->m_pTextureStates[i].m_alphaArg1;
		pRS->m_pTextureStates[i].m_alphaArg2 = pRSToClone->m_pTextureStates[i].m_alphaArg2;
		pRS->m_pTextureStates[i].m_texCoordIndex = pRSToClone->m_pTextureStates[i].m_texCoordIndex;
		pRS->m_pTextureStates[i].m_addressU = pRSToClone->m_pTextureStates[i].m_addressU;
		pRS->m_pTextureStates[i].m_addressV = pRSToClone->m_pTextureStates[i].m_addressV;
		pRS->m_pTextureStates[i].m_addressW = pRSToClone->m_pTextureStates[i].m_addressW;
		pRS->m_pTextureStates[i].m_magFilter = pRSToClone->m_pTextureStates[i].m_magFilter;
		pRS->m_pTextureStates[i].m_minFilter = pRSToClone->m_pTextureStates[i].m_minFilter;
		pRS->m_pTextureStates[i].m_mipFilter = pRSToClone->m_pTextureStates[i].m_mipFilter;
		pRS->m_pTextureStates[i].m_textureTransformFlags = pRSToClone->m_pTextureStates[i].m_textureTransformFlags;
		pRS->m_pTextureStates[i].m_colorArg0 = pRSToClone->m_pTextureStates[i].m_colorArg0;
		pRS->m_pTextureStates[i].m_alphaArg0 = pRSToClone->m_pTextureStates[i].m_alphaArg0;
		pRS->m_pTextureStates[i].m_resultArg0 = pRSToClone->m_pTextureStates[i].m_resultArg0;	
	}
	
	pRS->setScreen(pRSToClone->getScreen());
*/
	return X_OK;
}



//*****************************************************************************
/**
Destroys a render state and frees its associated resources.  

@param matHandle	The handle of the render state to destroy.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::destroyMaterial(XHandle matHandle)
{
	if (matHandle != X_INVALID_HANDLE)
	{
		return m_materials.removeResource(matHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
//*****************************************************************************
XHandle XVP::getMaterial(XS8* materialName)
{
	if (!materialName)
		return X_ERR_INVALID_PARAMS;

	XHandle matHandle = X_INVALID_HANDLE;
	m_materials.findResourceByName(materialName,matHandle);
	return matHandle;
}



//*****************************************************************************
/**
Destroys all existing render states and all associated resources.

@return None.
*/
//*****************************************************************************
XStatus XVP::destroyAllMaterials()
{
	return m_materials.destroyAllResources();
}



//*****************************************************************************
//*****************************************************************************
XS8* XVP::getMaterialName(XHandle matHandle)
{
	if (matHandle != X_INVALID_HANDLE)
	{
		XResource* pResource = m_materials.getResource(matHandle);
		if (pResource)
			return pResource->getName();
	}

	return NULL;
}



//*****************************************************************************
/**
Registers a scene node with a particular render state.  When the engine is ready
to render the scene node, it will preset the rendering device with the registered
render state settings for the node and then call the render() method for the node.
This function allows the engine to "batch" together scene nodes which share common
render states to provide more efficient render state switches.  

@param matHandle	The render state to associate a scene node with. A value of
				X_INVALID_HANDLE is valid.  It means that the scene node doesn't
				want to	register with a specific render state, but still wants to 
				recieve	a render notification from the engine.  Scene nodes who pass
				in X_INVALID_HANDLE for this parameter are responsible for setting
				their own render state settings to the rendering device before
				rendering themselves.
@param pNode	The scene node to register with the passed in render state.
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::addRenderEvent(XSceneNode* pSceneNode,XHandle matHandle,
							void* pUserData)
{
	//Even though a render state value of 0 is not valid, if a 0 is passed
	//to this function it means that the scene node doesn't want to register
	//with a specific render state, but still wants to recieve the render
	//notification from the engine.  Scene nodes who pass in 0 for the
	//render state are responsible for setting their own render state before
	//rendering and are added to a special list.
	if (!pSceneNode)
		return X_ERR_INVALID_PARAMS;

	if (matHandle == X_INVALID_HANDLE)
	{
		XRenderEvent re;
		re.pMaterial = NULL;
		re.pSceneNode = pSceneNode;
		re.pUserData = pUserData;
		m_dontCareRenderClientList.push_back(re);
		return X_OK;
	}
	else 
	{
		XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
		if (pMaterial)
		{
			pMaterial->addRenderEvent(pSceneNode,pUserData);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//***************************************************************************** 
//***************************************************************************** 
XStatus XVP::addMaterialRef(XHandle matHandle)
{
	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
	if (pMaterial)
	{
		pMaterial->incRefCount();
		return X_OK;
	}
	else 
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Sets a render state to the rendering device.  The properties of the render
state are applied to the rendering device and all subsequent geometry is rendered
with the render state settings.

@param matHandle	The handle of the render state to set to the rendering device.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::setMaterial(XHandle matHandle)
{
	XMaterial* pMaterial = (XMaterial*)m_materials.getResource(matHandle);
	if (pMaterial)
	{

		X_DEBUG
		(
			std::string s;
			char buf[30];
			s += "---------- BEGIN RENDER STATE SET (INDEX = ";
			s += itoa(matHandle,buf,10);
			s += ") ----------";
			X_DEBUG_LOG_LN(s.c_str(),X_DLL_VERBOSE);
		)

			//Okay, basically we run through all the information in this
			//render state and set it to the rendering device.  First,
			//however, we need to clear everything out so we can start
			//from a clean slate.
			
			//Run through the texture array and set the textures and associated
			//texture stage states.
			XU32 numStages = GraphicsMgr.getMaxTextureStages();
			for (XU32 i = 0;i < numStages;i++)
			{
				//if (X_IS_BIT_SET(pMaterial->m_texBitArray,i))
				if (pMaterial->m_pTextureHandles[i] != X_INVALID_HANDLE)
				{
					X_DEBUG
					(
						std::string s;
						s += "Setting texture - ";
						XS8* pFilename = GraphicsMgr.getTextureFilename(pMaterial->m_pTextureHandles[i]);
						if (pFilename)
							s += pFilename;					
						s += " to stage ";
						s += itoa(i,buf,10);
						X_DEBUG_LOG_LN(s.c_str(),X_DLL_VERBOSE);
					)

					GraphicsMgr.setTexture(pMaterial->m_pTextureHandles[i],i);
					
					//Now set this stages states.
					GraphicsMgr.setAddressU(i,(X_TEXTURE_ADDRESS_TYPE)pMaterial->m_pTextureStates[i].m_addressU);
					GraphicsMgr.setAddressV(i,(X_TEXTURE_ADDRESS_TYPE)pMaterial->m_pTextureStates[i].m_addressW);
					GraphicsMgr.setAddressW(i,(X_TEXTURE_ADDRESS_TYPE)pMaterial->m_pTextureStates[i].m_addressW);
					GraphicsMgr.setMagFilter(i,(X_TEXTURE_FILTER_TYPE)pMaterial->m_pTextureStates[i].m_magFilter);
					GraphicsMgr.setMinFilter(i,(X_TEXTURE_FILTER_TYPE)pMaterial->m_pTextureStates[i].m_minFilter);
					GraphicsMgr.setMipFilter(i,(X_TEXTURE_FILTER_TYPE)pMaterial->m_pTextureStates[i].m_mipFilter);
				}
				else
					GraphicsMgr.removeTexture(i);
			}

			//Now take care of the 2-state parameters (enable/disable), such
			//as alpha blending, depth write, etc.  When certain properties are
			//enabled, it is necessary to set other parameters which are 
			//affected.  For example, enabling alpha blending will cause
			//other properties to be used such as destBlend and srcBlend.
			
			if (pMaterial->m_properties & X_RSP_ALPHA_TEST)
			{
				GraphicsMgr.enableAlphaTest(true);
				GraphicsMgr.setAlphaFunc((X_CMP_FUNC_TYPE)pMaterial->m_alphaFunc);
				GraphicsMgr.setAlphaRef(pMaterial->m_alphaRef);
			}
			else
				GraphicsMgr.enableAlphaTest(false);
			
			if (pMaterial->m_properties & X_RSP_ALPHA_BLEND)
			{		
				GraphicsMgr.enableAlphaBlend(true);
				GraphicsMgr.setSrcBlend((X_BLEND_TYPE)pMaterial->m_srcBlend);
				GraphicsMgr.setDestBlend((X_BLEND_TYPE)pMaterial->m_destBlend);
				GraphicsMgr.setBlendOp((X_BLEND_OP_TYPE)pMaterial->m_blendOp);
			}
			else
				GraphicsMgr.enableAlphaBlend(false);
			
			GraphicsMgr.setDepthFunc((X_CMP_FUNC_TYPE)pMaterial->m_depthFunc);
			GraphicsMgr.setDepthBias(pMaterial->m_depthBias);
			GraphicsMgr.setFillMode((X_FILL_MODE_TYPE)pMaterial->m_fillMode);

			X_DEBUG
			(
				std::string s2;
				s2 += "---------- END RENDER STATE SET (INDEX = ";
				s2 += itoa(matHandle,buf,10);
				s2 += ") ----------";
				X_DEBUG_LOG_LN(s2.c_str(),X_DLL_VERBOSE);
			)
			
			//Everything went okay.
			return X_OK;
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Gets the interface to a render state.  Using the interface, you can set 
properties of the render state.

@param matHandle	The handle of the render state for which to get the interface.
@return A pointer to the interface for the render state.
*/
//*****************************************************************************
XMaterial* XVP::getMaterialInterface(XHandle matHandle)
{
	return (XMaterial*)m_materials.getResource(matHandle);
}

#endif


#if 0

//*****************************************************************************
/**
Creates a light from a .xlf file.

@param pFilename	The path to the .xlf file to load.  This filename must have
					a .xlf extension or this function will fail.
@param lightHandle	A variable to receive the light handle. 
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::loadLight(XS8* pFilename,XHandle& lightHandle)
{
	XModelLoader loader;
	XLightDesc desc;

	//Make sure we have a valid filename
	if (!pFilename)
		return X_ERR_INVALID_PARAMS;

	//Try to load the light file.
	loader.load(pFilename);
	desc.ambient = loader.m_model.light.ambient;
	desc.diffuse = loader.m_model.light.diffuse;
	desc.specular = loader.m_model.light.specular;
	//desc.direction = loader.m_model.light.direction;
	//desc.position = loader.m_model.light.position;
	desc.linearAtt = loader.m_model.light.attenuation;
	desc.range = loader.m_model.light.range;
	//desc.falloff = loader.m_model.light.falloff;
	desc.innerConeAngle = loader.m_model.light.innerConeAngle;
	desc.outerConeAngle = loader.m_model.light.outerConeAngle;
	desc.type = loader.m_model.light.type;

	if (GraphicsMgr.createLight(desc,lightHandle) != X_OK)
		return X_ERR_OPERATION_FAILED;
	else	
		return X_OK; 
}
#endif


//*****************************************************************************
/**
Creates a character set.  Character sets are used to hold font rendering
information including the image data and character dimensions.  

@param pTextureFilename	The path to the image containing the font data.
@param pCharDataFile The path to the file containing character dimension data.
@param csHandle	A variable to receive the character set handle. 
@return A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::createCharSet(XS8* pTextureFilename,XS8* pCharDataFile,
							XHandle& csHandle)
{
	//Validate params.
	if (!pTextureFilename)
		return X_ERR_INVALID_PARAMS;

	XResource* pResource = (XCharSet*)m_charSets.findResourceByName(pTextureFilename,
																	csHandle);

	if (pResource)
	{
		pResource->incRefCount();
		return X_OK;
	}
	else
	{
		XCharSet* pCharSet = NULL;
		XNew(pCharSet,XCharSet,XOSMgr::getDefaultHeap());
		if (pCharSet->init(pTextureFilename,pCharDataFile) == X_OK)
		{
			csHandle = m_charSets.addResource(pCharSet,pTextureFilename);
			return X_OK;
		}
		else
			return X_ERR_OPERATION_FAILED;
	}
}



//*****************************************************************************
/**
Destroys a character set and frees its associated resources.  

@param csHandle	The handle of the chararcter set to destroy.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::destroyCharSet(XHandle csHandle)
{
	if (csHandle != X_INVALID_HANDLE)
	{
		return m_charSets.removeResource(csHandle);
	}
	else
		return X_ERR_NO_SUCH_OBJECT;
}



//*****************************************************************************
/**
Destroys all character sets and all associated resources.

@return None.
*/
//*****************************************************************************
XStatus XVP::destroyAllCharSets()
{
	return m_charSets.destroyAllResources();
}



//*****************************************************************************
/**
Gets the interface to a character set.  Using the interface, you can set 
properties of the character set.

@param csHandle	The handle of the character set for which to get the interface.
@return A pointer to the interface for the character set.
*/
//*****************************************************************************
XCharSet* XVP::getCharSetInterface(XHandle csHandle)
{
	return (XCharSet*)m_charSets.getResource(csHandle);
}






//*****************************************************************************
/**
Update's X-VP.  This is the main heartbeat function of the engine.   It
must be called in order to update sprites, controllers, the 3D scene graph, 
etc... 

@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::update()
{
	X_START_PROFILE("XVP::Update()")
	XLua::executeFunction("OnXVPPreUpdate",NULL,0,0);
	
	X_DEBUG_LOG_LN("\n*** BEGIN XVP UPDATE ***",X_DLL_VERBOSE);

	//Get the timescale.  If the engine is paused, leave timescale set to 0.
	XF32 dt = 0;
	if (!m_paused)
		dt = getFrameDt();

	XMemBlockList<XState*>::iterator stateItr;

	//Clear out the stats structure for the next frame
	X_STATS(memset(&stats,0,sizeof(stats));)

	//Update the cursor for user interfaces.
	XUserInterface::updateCursor(dt);

	//Update input, audio and network managers
	X_START_PROFILE("InputMgr.update()")
	InputMgr.update(dt);
	X_STOP_PROFILE()
	X_START_PROFILE("AudioMgr.update()")
	AudioMgr.update(dt);
	X_STOP_PROFILE()
	X_START_PROFILE("NetworkMgr.update()")
	NetworkMgr.update(dt);
	X_STOP_PROFILE()

	//Check for the F1 key to show or hide the console.
	X_CONSOLE
	(
		if (XConsole::isInitialized())
		{
			if (InputMgr.keyDownOnce(X_KEY_F1))
			{
				if (XConsole::isVisible())
					XConsole::hide();
				else
					XConsole::show();
			}
			XConsole::update(dt);
		}
	)
		
	//Check for the F2 key to show or hide the console.
	X_PROFILER
	(
		if (XProfiler::isInitialized())
		{
			if (InputMgr.keyDownOnce(X_KEY_F2))
			{
				if (XProfiler::isVisible())
					XProfiler::hide();
				else
					XProfiler::show();
			}
			XProfiler::update(dt);
		}
	)

	//Update Lua
	XLua::update(dt);

	//Call registered pre-update states
	X_DEBUG_LOG_LN("---------- BEGIN PRE-UPDATE STATE ----------",X_DLL_VERBOSE);
	X_START_PROFILE("Process Pre Update States")
	processPreUpdateStateCmds();
	stateItr.set(m_preUpdateStateList.begin());
	while (stateItr.isValid())
	{
		stateItr.getValue()->update(dt);
		stateItr.getNext();
	}
	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END PRE-UPDATE STATE ----------",X_DLL_VERBOSE);

	//Update physics simulation if one exists
	if (m_pPhysicsUpdateCallback)
		m_pPhysicsUpdateCallback(dt);

	//Update controllers 
	X_DEBUG_LOG_LN("---------- BEGIN CONTROLLER ARRAY UPDATE ----------",X_DLL_VERBOSE);
	X_START_PROFILE("Update Controllers")
	m_updateState = X_US_CONTROLLERS;
	XMemBlockList<XController*>::iterator itr(m_controllerArray.begin());
	while (itr.isValid())
	{
		X_DEBUG_LOG_LN(itr.getValue()->getTypeDesc(),X_DLL_VERBOSE);
		itr.getValue()->update(dt);
		itr.getNext();
	}
	XMemBlockList<XShaderController*>::iterator itr2(m_shaderControllerArray.begin());
	while (itr2.isValid())
	{
		XShaderController* ptest = itr2.getValue();
		X_DEBUG_LOG_LN(itr2.getValue()->getTypeDesc(),X_DLL_VERBOSE);
		itr2.getValue()->update(dt);
		itr2.getNext();
	}
	m_updateState = X_US_NORMAL;
	processControllerCmds(dt);
	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END CONTROLLER ARRAY UPDATE ----------",X_DLL_VERBOSE);

	//Update graphics manager
	X_START_PROFILE("GraphicsMgr.update")
	GraphicsMgr.update(dt);
	X_STOP_PROFILE()

	//Call registered post-update states
	X_DEBUG_LOG_LN("---------- BEGIN POST-UPDATE STATE ----------",X_DLL_VERBOSE);
	X_START_PROFILE("Process Post Update States")
	processPostUpdateStateCmds();
	stateItr.set(m_postUpdateStateList.begin());
	while (stateItr.isValid())
	{
		stateItr.getValue()->update(dt);
		stateItr.getNext();
	}
	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END POST-UPDATE STATE ----------",X_DLL_VERBOSE);

	X_DEBUG_LOG_LN("---------- BEGIN SCENE GRAPH RENDER ----------",X_DLL_VERBOSE);
	//Render
	GraphicsMgr.render(dt);
	X_DEBUG_LOG_LN("---------- END SCENE GRAPH RENDER ----------",X_DLL_VERBOSE);
 	    
	//Call registered post-render states
	X_DEBUG_LOG_LN("---------- BEGIN POST-RENDER CALLBACK ----------",X_DLL_VERBOSE);
	X_START_PROFILE("Process Post Render States")
	processPostRenderStateCmds();
	stateItr.set(m_postRenderStateList.begin());
	while (stateItr.isValid())
	{
		stateItr.getValue()->update(dt);
		stateItr.getNext();
	}
	X_STOP_PROFILE()
	X_DEBUG_LOG_LN("---------- END POST-RENDER CALLBACK ----------",X_DLL_VERBOSE);
	
	//Update the time.
	m_frameDt = m_mainTimer.getElapsedTime();
	m_totalRunningTime = m_mainTimer.getDuration();

	X_DEBUG_LOG_LN("*** END XVP UPDATE ***",X_DLL_VERBOSE);

	X_STOP_PROFILE()

	//Poll jax for commands
	if (m_jaxConnection != X_INVALID_HANDLE)
	{
		XS8 rcvBuf[1024];
		XU32 rcvLen = 0;
		XVP::NetworkMgr.receiveTCPData(m_jaxConnection,rcvBuf,1000,rcvLen);

		if (rcvLen != 0)
		{
			rcvBuf[rcvLen] = '\0';
			XLua::executeString(rcvBuf);
		}
	}

	XLua::executeFunction("OnXVPPostUpdate",NULL,0,0);

	//Everything went ok.
	return X_OK;
}




//*****************************************************************************
/**
Processes all pre update state commands on the command queue.

@return None.
*/
//*****************************************************************************
void XVP::processPreUpdateStateCmds()
{
	X_DEBUG(std::string s;)
	XStateCmd* pCmd;
	
	//Let's process and empty the command queues.  
	XMemBlockList<XStateCmd>::iterator i(m_preUpdateStateCmdQueue.begin());
	//for (XU32 i = 0;i < m_preUpdateStateCmdQueue.getNumItems();++i)
	while (i.isValid())
	{
		pCmd = &i.getValue();//m_preUpdateStateCmdQueue[i];
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			//If there is a current state, tell it to pause itself			
			if (m_preUpdateStateList.getNumItems() != 0)
				(m_preUpdateStateList.getBack())->pause();
			
			//Now, tell the new state to activate itself.  If the state has not been
			//initialized yet, it will be automatically initialized by preActivate().
			pCmd->pState->preActivate(pCmd->pUserData);			
			m_preUpdateStateList.insertBack(pCmd->pState);

			X_DEBUG
			(
				s = "Pushed pre update state: ";
				if (pCmd->pState)
					s += pCmd->pState->getStateDesc();
				X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
			)
			break;
		case X_CMDT_REMOVE:
			
			if (m_preUpdateStateList.getNumItems() != 0)
			{
				X_DEBUG
				(
					s = "Popped pre update state: ";
					s += m_preUpdateStateList.getBack()->getStateDesc();
					X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
				)
				
				m_preUpdateStateList.getBack()->deactivate();
				m_preUpdateStateList.removeBack();
				
				//If there is a paused state, tell it to resume itself.
				if (m_preUpdateStateList.getNumItems() != 0)
					m_preUpdateStateList.getBack()->resume();
			}

			break;
		}	
		i.getNext();
	}

	m_preUpdateStateCmdQueue.clear();
}



//*****************************************************************************
/**
Processes all post update state commands on the command queue.

@return None.
*/
//*****************************************************************************
void XVP::processPostUpdateStateCmds()
{
	X_DEBUG(std::string s;)
	XStateCmd* pCmd;
	
	//Let's process and empty the command queues. 
	XMemBlockList<XStateCmd>::iterator i(m_postUpdateStateCmdQueue.begin());
	while (i.isValid())
	{
		pCmd = &i.getValue();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			//If there is a current state, tell it to pause itself.
			if (m_postUpdateStateList.getNumItems() != 0)
				m_postUpdateStateList.getBack()->pause();
			
			//Now, tell the new state to activate itself.  If the state has not been
			//initialized yet, it will be automatically initialized by preActivate().
			pCmd->pState->preActivate(pCmd->pUserData);

			m_postUpdateStateList.insertBack(pCmd->pState);

			X_DEBUG
			(
				s = "Added post update state: ";
				if (pCmd->pState)
					s += pCmd->pState->getStateDesc();
				X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
			)

			break;
		case X_CMDT_REMOVE:

			if (m_postUpdateStateList.getNumItems() != 0)
			{
				X_DEBUG
				(
					s = "Popped pre update state: ";
					s += m_postUpdateStateList.getBack()->getStateDesc();
					X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
				)

				//Tell the state to deactivate itself and then remove it from
				//the list
				m_postUpdateStateList.getBack()->deactivate();
				m_postUpdateStateList.removeBack();

				//If there is a paused state, tell it to resume itself.
				if (m_postUpdateStateList.getNumItems() != 0)
					m_postUpdateStateList.getBack()->resume();

			}
				
			break;
		}
		i.getNext();
	}
	
	m_postUpdateStateCmdQueue.clear();		
}



//*****************************************************************************
/**
Processes all post render state commands on the command queue.

@return None.
*/
//*****************************************************************************
void XVP::processPostRenderStateCmds()
{
	X_DEBUG(std::string s;)
	XStateCmd* pCmd;

	//Let's process and empty the command queues.  
	XMemBlockList<XStateCmd>::iterator i(m_postRenderStateCmdQueue.begin());
	while (i.isValid())
	{
		pCmd = &i.getValue();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			//If there is a current state, tell it to pause itself.
			if (m_postRenderStateList.getNumItems() != 0)
				m_postRenderStateList.getBack()->pause();
			
			//Now, tell the new state to activate itself.  If the state has not been
			//initialized yet, it will be automatically initialized by preActivate().
			pCmd->pState->preActivate(pCmd->pUserData);

			m_postRenderStateList.insertBack(pCmd->pState);

			X_DEBUG
			(
				s = "Added post render state: ";
				if (pCmd->pState)
					s += pCmd->pState->getStateDesc();
				X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
			)

			break;
		case X_CMDT_REMOVE:
			if (m_postRenderStateList.getNumItems() != 0)
			{
				X_DEBUG
				(
					s = "Popped pre update state: ";
					s += m_postRenderStateList.getBack()->getStateDesc();
					X_DEBUG_LOG_LN(s.c_str(),X_DLL_INFO);
				)
				
				//Tell the state to deactivate itself and then remove it from
				//the list
				m_postRenderStateList.getBack()->deactivate();
				m_postRenderStateList.removeBack();


				//If there is a paused state, tell it to resume itself.
				if (m_postRenderStateList.getNumItems() != 0)
					m_postRenderStateList.getBack()->resume();
			}

			break;
		}
		i.getNext();
	}
		
	m_postRenderStateCmdQueue.clear();		
}

	

//*****************************************************************************
/**
Processes all controller commands on the command queue.

@return None.
*/
//*****************************************************************************
void XVP::processControllerCmds(XF32 dt)
{
	//Let's process and empty the command queues.  Remember to call the update()
	//for each controller so that they don't miss a beat.  
	while (m_controllerCmdQueue.getNumItems() != 0)
	{
		XControllerCmd* pCmd = &m_controllerCmdQueue.getFront();
		switch (pCmd->cmdType)
		{
		case X_CMDT_ADD:
			addController(pCmd->pController);
			pCmd->pController->update(dt);
			break;
		case X_CMDT_REMOVE:
			removeController(pCmd->pController);
			break;
		}
		m_controllerCmdQueue.removeFront();		
	}
}



//*****************************************************************************
/**
Renders the scene.

@return A return code of type XStatus.
*/
//*****************************************************************************
/*XStatus XVP::render()
{
	//Make sure there is an active camera, if not, just return.
	//if (!m_pActiveCamera)
	//	return X_ERR_OPERATION_FAILED;
	//else
		return GraphicsMgr.flip();
}*/



//*****************************************************************************
/**
Get X-VP's current frames per second reading.  This is the number
of frames displayed to the screen within one second.

@param dt	The amount of time that has passed since the last frame
					update in seconds.
@return The number of frames rendered per second.
*/
//*****************************************************************************
XU32 XVP::getFPS(XF32 dt)
{
	static XU32	finalFrameCount = 0;
	static XU32	elapsedFrameCount = 0;
	static XF32	totalElapsedTime = 0.0f;

	totalElapsedTime += dt;
	elapsedFrameCount++;
	if (totalElapsedTime >= 1.0f) 
	{
		finalFrameCount = elapsedFrameCount;
		elapsedFrameCount = 0;
		totalElapsedTime = 0.0f;
	} 

	return finalFrameCount;	
}


XBOOL XVP::isInitialized()
{
	return m_inited;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVP::startProfiler()
{
	X_PROFILER
	( 
		//Initialize profiler if we have a valid system font
		if (m_systemCharSet != X_INVALID_HANDLE)
		{
			XProfiler::init(m_systemCharSet);
			//m_profilerAvailable = true;
		}
		else
		{
			X_ASSERT_FORCED("No system font loaded");
		}
	)

	return X_OK;
}



//*****************************************************************************
//*****************************************************************************
XStatus XVP::startConsole()
{
	X_CONSOLE
	( 
		//Initialize console if we have a valid system font
		if (m_systemCharSet != X_INVALID_HANDLE)
			XConsole::init(m_systemCharSet);
	)

	return X_OK;
}



//*****************************************************************************
/**
Initializes X-VP.  This initialization function will automatically
choose the best possible device based upon the parameters supplied.  

@param desc Initialization structure that defines startup configuration.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::init(void* pProcessHandle)
{
	//Uncomment to expire...
	
	/*DWORD d;
	if (!RegReadDword(HKEY_LOCAL_MACHINE,"System\\xsys32x","xsystemIDx2",&d))
	{
		RegCreateKey(HKEY_LOCAL_MACHINE,"System\\xsys32x");
		RegWriteDword(HKEY_LOCAL_MACHINE,"System\\xsys32x","xsystemIDx2",1);
	}
	else
	{
		if (d < 100)
		{
			d++;
			RegWriteDword(HKEY_LOCAL_MACHINE,"System\\xsys32x","xsystemIDx2",d);
		}
		else
			return X_ERR_OPERATION_FAILED;
	}
	*/

	//Initialize the Lua interface first so all components may have access
	//to Lua.  Also, register XVP's lua glue lib.
	XLua::init();
	XLua::registerLibrary(X_XVP_LUA_LIB_NAME,XVPLuaLib);
	XLua::executeFunction("OnXVPPreInit",NULL,0,0);

	//Read in the configuration 
	XVPDesc xvpDesc;
	xvpDesc.pProcessHandle = pProcessHandle;
	xvpDesc.initAudioMgr = XLua::getXBOOLGlobalVar("XVP_CONF_initAudioMgr");
	xvpDesc.initInputMgr = XLua::getXBOOLGlobalVar("XVP_CONF_initInputMgr");
	xvpDesc.initNetworkMgr = XLua::getXBOOLGlobalVar("XVP_CONF_initNetworkMgr");		
	xvpDesc.generateLogFile = XLua::getXBOOLGlobalVar("XVP_CONF_generateLogFile");
	xvpDesc.enableAsserts = XLua::getXBOOLGlobalVar("XVP_CONF_enableAsserts");
	xvpDesc.enableDeviceMessages = XLua::getXBOOLGlobalVar("XVP_CONF_enableDeviceMessages");
	xvpDesc.enableInternalFreeLists = XLua::getXBOOLGlobalVar("XVP_CONF_enableInternalFreeLists");
	xvpDesc.audioStreamThreshold = XLua::getXU32GlobalVar("XVP_CONF_audioStreamThreshold");
	xvpDesc.enableKeyboard = XLua::getXBOOLGlobalVar("XVP_CONF_enableKeyboard");
	xvpDesc.enableMouse = XLua::getXBOOLGlobalVar("XVP_CONF_enableMouse");
	xvpDesc.enableJoysticks = XLua::getXBOOLGlobalVar("XVP_CONF_enableJoysticks");
	xvpDesc.debugRenderContext = XLua::getXU32GlobalVar("XVP_CONF_debugRenderContext");
		
	xvpDesc.maxMaterials = XLua::getXU32GlobalVar("XVP_CONF_maxMaterials");
	xvpDesc.maxCharSets = XLua::getXU32GlobalVar("XVP_CONF_maxCharSets");
	xvpDesc.maxLights = XLua::getXU32GlobalVar("XVP_CONF_maxLights");			
	xvpDesc.maxTextures = XLua::getXU32GlobalVar("XVP_CONF_maxTextures");
	xvpDesc.maxVertexDeclarations = XLua::getXU32GlobalVar("XVP_CONF_maxVertexDeclarations");
	xvpDesc.maxVertexBuffers = XLua::getXU32GlobalVar("XVP_CONF_maxVertexBuffers");
	xvpDesc.maxIndexBuffers = XLua::getXU32GlobalVar("XVP_CONF_maxIndexBuffers");
	xvpDesc.maxRenderContexts = XLua::getXU32GlobalVar("XVP_CONF_maxRenderContexts");
	xvpDesc.maxRenderTargetArrays = XLua::getXU32GlobalVar("XVP_CONF_maxRenderTargetArrays");
	xvpDesc.maxVertexShaders = XLua::getXU32GlobalVar("XVP_CONF_maxVertexShaders");
	xvpDesc.maxGeometryShaders = XLua::getXU32GlobalVar("XVP_CONF_maxGeometryShaders");
	xvpDesc.maxPixelShaders = XLua::getXU32GlobalVar("XVP_CONF_maxPixelShaders");
	xvpDesc.maxDepthStencilStates = XLua::getXU32GlobalVar("XVP_CONF_maxDepthStencilStates");		
	xvpDesc.maxRasterizerStates = XLua::getXU32GlobalVar("XVP_CONF_maxRasterizerStates");		
	xvpDesc.maxBlendStates = XLua::getXU32GlobalVar("XVP_CONF_maxBlendStates");					
	xvpDesc.maxSamplerStates = XLua::getXU32GlobalVar("XVP_CONF_maxSamplerStates");				
	xvpDesc.maxConstantBuffers = XLua::getXU32GlobalVar("XVP_CONF_maxConstantBuffers");
	xvpDesc.maxMovies = XLua::getXU32GlobalVar("XVP_CONF_maxMovies");
	xvpDesc.maxSounds = XLua::getXU32GlobalVar("XVP_CONF_maxSounds");
	xvpDesc.maxTCPConnections = XLua::getXU32GlobalVar("XVP_CONF_maxTCPConnections");		
	xvpDesc.maxUDPConnections = XLua::getXU32GlobalVar("XVP_CONF_maxUDPConnections");			
	xvpDesc.maxTCPListeners = XLua::getXU32GlobalVar("XVP_CONF_maxTCPListeners");	

	X_DEBUG_LOG_LN("\n*** BEGIN XVP INIT ***",X_DLL_INFO);

	//Init the OS manager so we can gain access to memory routines, etc...
	if ((m_status = XOSMgr::init(xvpDesc)) != X_OK)
	{
		X_ASSERT_FORCED("OS Manager initialization failed");
		return m_status;
	}

	m_mediaPath[0] = '\0';

	m_frameDt = 0.0;
	m_paused = false;
	m_updateState = X_US_NORMAL;

	//Initialize the managers.
	if (initManagers(xvpDesc) != X_OK)
	{
		X_ASSERT_FORCED("One or more core managers failed to initialize");
		return X_ERR_OPERATION_FAILED;
	}

	//Initialize general XVP settings.
	if (generalInit(xvpDesc) != X_OK)
	{
		X_ASSERT_FORCED("General initialization failed");
		return X_ERR_OPERATION_FAILED;
	}
		
	startConsole();

	X_DEBUG_LOG_LN("*** END XVP INIT ***",X_DLL_INFO);

	m_inited = true;
	
	XLua::executeFunction("OnXVPPostInit",NULL,0,0);

	//Everything went well.
	return X_OK;
}



//***************************************************************************** 
/**
Shuts down X-VP and frees all associated resources.  This must be called
before the user application exits.

@return	A return code of type XStatus.
*/
//***************************************************************************** 
XStatus XVP::destroy()
{
	XLua::executeFunction("OnXVPPreDestroy",NULL,0,0);

	if (m_jaxListener != X_INVALID_HANDLE)
		NetworkMgr.destroyTCPListener(m_jaxListener);

#ifdef XVP_EDITOR
	//Destroy gizmo static data
	XGizmoMgr::destroyStaticData();
#endif

	//Clean up post effects
	XPostEffect::destroyStaticData();

	//Shutdown the managers.
	GraphicsMgr.destroy();
	InputMgr.destroy();
	AudioMgr.destroy();
	NetworkMgr.destroy();

	m_charSets.destroy();

	m_controllerArray.clear();	
	//m_matrixStack.destroy();
	m_preUpdateStateList.clear();
	m_preUpdateStateCmdQueue.clear();
	m_postUpdateStateList.clear();
	m_postUpdateStateCmdQueue.clear();
	m_postRenderStateList.clear();
	m_postRenderStateCmdQueue.clear();

	XLua::executeFunction("OnXVPPostDestroy",NULL,0,0);

	//Shut down Lua.
	XLua::destroy();

	//Destroy system font.
	if (m_systemCharSet != X_INVALID_HANDLE)
		destroyCharSet(m_systemCharSet);

	//Shutdown console
	X_CONSOLE
	(
		if (XConsole::isInitialized())
			XConsole::destroy();
	)

	//Shutdown profiler
	X_PROFILER
	(
		if (XProfiler::isInitialized())
			XProfiler::destroy();
	)

	//Shutdown debug utility library
	X_DUL(XDUL::destroy();)

	ShaderFactory.destroy();
	m_shaderControllerArray.clear();

	//Finally, shutdown the OS manager
	XOSMgr::destroy();

	m_inited = false;

	return X_OK;
}


XStatus XVP::setPhysicsUpdateCallback(void (*callback)(XF32 dt))
{
	m_pPhysicsUpdateCallback = callback;
	return X_OK;
}




//*****************************************************************************
/**
Responsible for all generic X-VP setup.

@param desc Initialization structure that defines startup configuration.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::generalInit(XVPDesc& desc)
{
	XU32 i = 0;

	m_charSets.init("CharSets",desc.maxCharSets);

	//Init random number generator
	XInitRandomNumberGenerator();

	//Init lists
	//m_preUpdateStateCmdQueue.init(16,NULL);
	//m_frontSpriteArray.init(32,NULL);
	//m_backSpriteArray.init(32,NULL);

	//Init matrix stack. Push an empty matrix on the stack.  We don't need
	//to maintain the matrix, it's just a root transform.
	//m_matrixStack.init(256,"MatrixStack");
	//m_matrixStack.insertBack();


	//Set the default clear color to black.
	//GraphicsMgr.setClearColor(XColor(0,0,0,0));
	
	//Create system font if available.
	char oldTexturePath[X_MAX_PATH_SIZE];
	strcpy(oldTexturePath,XOSMgr::getPath(X_PATH_TEXTURE));
	XOSMgr::setPath(X_PATH_TEXTURE,"../system/console/");
	if (XOSMgr::fileExists("../system/console/"X_SYSTEM_FONT))
		createCharSet(X_SYSTEM_FONT,X_SYSTEM_FONT_METRICS,m_systemCharSet);
	XOSMgr::setPath(X_PATH_TEXTURE,oldTexturePath);

	ShaderFactory.init(100);

	X_DUL
	(
		//Initialize the debug utility library
		XDUL::init();
	)

	if (desc.initNetworkMgr)
	{
		XNetAddress listenAddress;
		listenAddress.setPort(5000);
		NetworkMgr.createTCPListener(listenAddress,jaxListenCallback,NULL,m_jaxListener);
	}

	//Initialize Lua Glue
	XUserInterface::initLuaGlue();
	XActor::initLuaGlue();

	//Initialize post effects
	XPostEffect::initStaticData();

#ifdef XVP_EDITOR
	//Initialize gizmo static data
	XGizmoMgr::initStaticData();
#endif

	
	//Start the main timer
	m_mainTimer.start();

	//Everything went ok.
	return X_OK;
}



//*****************************************************************************
/**
Responsible for initializing all X-VP managers.

@param desc Initialization structure that defines startup configuration.
@return	A return code of type XStatus.
*/
//*****************************************************************************
XStatus XVP::initManagers(XVPDesc& desc)
{
	//Let's get the graphics manager up and running.
	if ((m_status = GraphicsMgr.init(desc)) != X_OK)
	{
		X_ASSERT_FORCED("Graphics Manager initialization failed");
		return m_status;
	}
	
	//Let's get the sound manager up and running.
	if (desc.initAudioMgr)
	{
		if ((m_status = AudioMgr.init(desc)) != X_OK)
		{
			X_ASSERT_FORCED("Sound Manager initialization failed");
			return m_status;
		}
	}

	//Let's get the input manager up and running.
	if (desc.initInputMgr)
	{
		if ((m_status = InputMgr.init(desc)) != X_OK)
		{
			X_ASSERT_FORCED("Input Manager initialization failed");
			return m_status;
		}
	}

	//Let's get the network manager up and running.
	if (desc.initNetworkMgr)
	{
		if ((m_status = NetworkMgr.init(desc)) != X_OK)
		{
			X_ASSERT_FORCED("Network Manager initialization failed");
			return m_status;
		}
	}


	//Everything went well
	return X_OK;	
}



X_NET_MSG_ACTION_TYPE XVP::jaxListenCallback(X_NET_MSG_TYPE msgType,XNetAddress& clientAddress,
																   XHandle tcpConnHandle,
																   void* pUserData)
{
	if (msgType == X_NMT_CONNECTION_REQUEST)
        return X_NMAT_ACCEPT;
	else if (msgType == X_NMT_CONNECTION_ACCEPTED)
	{
		//Destroy old connection is applicable
		if (m_jaxConnection != X_INVALID_HANDLE)
			NetworkMgr.destroyTCPConnection(m_jaxConnection);
		m_jaxConnection = tcpConnHandle;

	}

	return X_NMAT_ACCEPT;
}
