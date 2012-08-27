/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifdef USE_LUA

#include "DScriptLibraries.h"
#include "Logger.h"


DScriptDisciple* GetScriptObject(lua_State *scriptThread)
{
	lua_pushlightuserdata(scriptThread, scriptThread);
	lua_gettable(scriptThread, LUA_GLOBALSINDEX);
	DScriptDisciple * s = static_cast<DScriptDisciple *>( lua_touserdata(scriptThread, -1) );

	return s;
}

static int luaTestFunction( lua_State* L )
{
	//Log something to the logger to make sure the library works.
	const char* text = lua_tostring(L, 1);

	DLogGuru::GetSingletonPtr()->LogSingleMessage( "Lib works" );
	DLogGuru::GetSingletonPtr()->LogSingleMessage( text );

	return 0;
}

static int luaWaitFrame( lua_State* L )
{
	DScriptDisciple* scriptClass = GetScriptObject(L);
	scriptClass->State() = LS_PAUSED;

	return ( lua_yield(L, 1) );
}

static const luaL_reg mainLib[] =
{
	//Test function
	{"TestFunction", 	luaTestFunction} ,
	{"WaitFrame",	 	luaWaitFrame} ,

	{NULL, NULL}
};

void RegisterLibary( lua_State* state )
{
	luaL_openlib(state, "Scene", mainLib, 0);
}
#endif //if using Lua
