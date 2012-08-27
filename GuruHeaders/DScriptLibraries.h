/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef DSCRIPTLIBRARIES_H_INCLUDED
#define DSCRIPTLIBRARIES_H_INCLUDED


#ifdef USE_LUA
//Defines the libraries for the application here....  will likely separate into multiple library files.
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	#include <luaconf.h>
}

#include "DScriptDisciple.h"
#include <vector>

class GameObject;

DScriptDisciple* GetScriptObject(lua_State *scriptThread);
void	lua_pushobject( lua_State* state );
Entity* lua_toobject( lua_State* state, int arg );


void RegisterLibary( lua_State* state );
void RegisterInputLibrary( lua_State* state );
int RegisterObjectLibrary( lua_State* state );
#endif // if using Lua

#endif // DSCRIPTLIBRARIES_H_INCLUDED
