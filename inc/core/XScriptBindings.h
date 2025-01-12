#ifndef __X_SCRIPT_BINDINGS_H__
#define __X_SCRIPT_BINDINGS_H__

#ifdef USE_CUSTOM_LUA_BINDINGS
//callback function for getting the lua ref index for the table belonging to the point
extern "C" int nLuaGetRefIndex(void *pvUserData);
extern "C" void vLuaGetMemberData(lua_State *poLuaState, void *objptr, void *memptr, void *value);
extern "C" void vLuaSetMemberData(lua_State *poLuaState, void *objptr, void *memptr, void *value);
void vLuaResetPoolCounts();

//class to wrap the script binding data for a member
class EgScScriptMemberBinding
{
	public:
		virtual void vGetMemberData(lua_State *poLuaState, void *objptr, void *value) = 0;
		virtual void vSetMemberData(lua_State *poLuaState, void *objptr, void *value) = 0;
};
#endif

//class that is used for binding functions to the scripting engine
class XScriptBindings
{
	public:
		static void vBind(lua_State *poLuaState);
		static void vUnbind();
};

#endif