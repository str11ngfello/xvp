
#ifndef XVP_LUA_GLUE
#define XVP_LUA_GLUE

int LAPI_xvp(lua_State* pLuaState);
int LAPI_setMediaPath(lua_State* pLuaState);
int LAPI_getMediaPath(lua_State* pLuaState);
int LAPI_getElapsedTime(lua_State* pLuaState);
int LAPI_getMatrixStackCapacity(lua_State* pLuaState);
int LAPI_getFPS(lua_State* pLuaState);
int LAPI_pause(lua_State* pLuaState);
int LAPI_resume(lua_State* pLuaState);
int LAPI_pushPreUpdateState(XState* pState,void* pUserData = NULL);
int LAPI_pushPostUpdateState(XState* pState,void* pUserData = NULL);
int LAPI_pushPostMaterial(XState* pState,void* pUserData = NULL);
int LAPI_popPreUpdateState();
int LAPI_popPostUpdateState();
int LAPI_popPostMaterial();
int LAPI_getNumPreUpdateStates();
int LAPI_getNumPostUpdateStates();
int LAPI_getNumPostMaterials();
int LAPI_isPreUpdateStateRegistered(XState* pState);
int LAPI_isPostUpdateStateRegistered(XState* pState);
int LAPI_isPostMaterialRegistered(XState* pState);
/*int LAPI_getNumSprites(lua_State* pLuaState);

int LAPI_enableSprites(lua_State* pLuaState);
int LAPI_disableSprites(lua_State* pLuaState);
int LAPI_flushScene(lua_State* pLuaState);
int LAPI_dumpScene(lua_State* pLuaState);
*/


int LAPI_AppRegisterCreationHandler(lua_State* pLuaState);
int LAPI_AppRegisterDestructionHandler(lua_State* pLuaState);
int LAPI_AppRegisterUpdateHandler(lua_State* pLuaState);

#endif