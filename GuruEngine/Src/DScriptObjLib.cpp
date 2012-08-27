/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/




#ifdef USE_LUA

#include "DScriptLibraries.h"
#include "DObject.h"
#include "Logger.h"
#include "Game_Engine.h"
#include "GameObjectDescriptor.h"


void	lua_pushobject( lua_State* state, GameObject> obj )
{
	if( !obj ) { return; }

	lua_newtable(state);
	lua_pushstring(state,"ObjPtr");
	lua_pushlightuserdata(state,(void*)obj.get());
	lua_settable(state,-3);
	luaL_getmetatable(state, "MetaDude.Object" );
	lua_setmetatable(state,-2);
}

GameObject> lua_toobject( lua_State* state, int arg )
{
	//Check if valide table.
	if( lua_istable(state, arg ) )
	{
		//It is valid table, so see if it has the _pointer field.
		void* ret = NULL;
		lua_getfield( state, arg, "ObjPtr" );

		if( lua_isnil( state, -1 ) )
		{
			//This is bad, do exceptions and logs and shit.
			return NULL;
		}

		//So, we're in business, so get the pointer and cast.
		ret = lua_touserdata(state, -1);

		if( ret )
		{
			GameObject* obj = (GameObject*)ret;
			return std::make_shared<GameObject>(*obj);
		}

		return NULL;
	}
	else
	{
		//This isn't a table being passed in, so it's definitely
		// not a game object.  Report badness here.
		return NULL;
	}
}

static int luaCreateObject( lua_State* L )
{
	//SDL_GL_RenderModule* module = (SDL_GL_RenderModule*)Engine::GetSingletonPtr()->Module();
	auto engine = (Game_Engine*)Game_Engine::GetSingletonPtr();
	GameObjectDescriptor descriptor;
	engine->mWorldScene->CreateGameObject(descriptor);

	auto obj = engine->mWorldScene->mObjFactory->GetLastObjectCreated();
	
	//@todo:
	//Not provided yet, but I'll want at least some basic info coming through from LUA
	// so I know as to how to create this object.
	//
	//For now I'm going to just use some basic values.

	//DObject* obj = module->CreateObject( CVector3(0,0,0), 10.f, 10.f, 10.f);
	lua_pushobject(L, obj);
	return 1;
}


static int luaTestLog( lua_State* L )
{
	GameObject> obj = lua_toobject(L,1);

	if( !obj ) { return 0; }
	
	return 0;
}


static const luaL_reg objectLib[] =
{
	{"Create",			luaCreateObject },
	{"TestLog",			luaTestLog },

	{NULL,NULL}
};



int RegisterObjectLibrary( lua_State* state )
{
	luaL_newmetatable( state, "MetaDude.Object" );

	lua_pushstring(state, "__index");
	lua_pushvalue(state, -2);  /* pushes the metatable */
	lua_settable(state, -3);  /* metatable.__index = metatable */

	luaL_openlib(state, NULL, objectLib, 0);
	luaL_openlib(state, "Object", objectLib, 0);
	luaL_getmetatable(state, "MetaDude.Object");
	lua_setmetatable(state, -2);	//give the library the gameobject metatable

	return 0;
}
#endif //if using Lua
