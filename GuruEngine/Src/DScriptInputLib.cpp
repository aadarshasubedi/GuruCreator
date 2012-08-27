/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/



#ifdef USE_LUA

#include "DScriptLibraries.h"
#include "InputGuru.h"


static int luaInputKeyPressed( lua_State* L )
{
	int keyval = (int)lua_tonumber( L, 1 );

	lua_pushboolean(L, DInputGuru::GetSingletonPtr()->IsKeyPressed((SDLKey)keyval) );

	return 1;
}

static int luaInputKeyReleased( lua_State* L )
{
	SDLKey keyval = (SDLKey)lua_tointeger( L, 1 );

	bool bajin = DInputGuru::GetSingletonPtr()->IsKeyReleased( keyval );
	if( bajin )
	{
		lua_pushboolean(L, true);
	}
	else
	{
		lua_pushboolean(L, false);
	}

	return 1;
}

static int luaInputKeyHeld( lua_State* L )
{
	SDLKey keyval = (SDLKey)lua_tointeger( L, 1 );

	bool bajin = DInputGuru::GetSingletonPtr()->IsKeyHeld( keyval );
	if( bajin )
	{
		lua_pushboolean(L, true);
	}
	else
	{
		lua_pushboolean(L, false);
	}
	return 1;
}

static int luaMouseMoving( lua_State* L )
{
	lua_pushboolean(L, DInputGuru::GetSingletonPtr()->MouseMoving());
	return 1;
}

static int luaMouseLeftClick( lua_State*  )
{
	return 0;
}

static int luaMouseRightClick( lua_State*  )
{
	return 0;
}

static int luaMouseLeftRelease( lua_State*  )
{
	return 0;
}

static int luaMouseRightRelease( lua_State*  )
{
	return 0;
}

static int luaMouseLeftHeld( lua_State*  )
{
	return 0;
}

static int luaMouseRightHeld( lua_State*  )
{
	return 0;
}

// xbox controller
static int luaXBoxBackPressed( lua_State* ls )
{
	//hacked to always check for back button
	bool result = DInputGuru::GetSingletonPtr()->mXController.CheckButtonPressed(XINPUT_GAMEPAD_BACK);
	lua_pushboolean(ls, result );
	return 1;
}

static int luaXBoxButtonPressed( lua_State* ls )
{
	bool result = DInputGuru::GetSingletonPtr()->mXController.ButtonPressed();
	lua_pushboolean(ls, result );
	return 1;
}

static int luaXBoxButtonHeld( lua_State* ls )
{
	bool result = DInputGuru::GetSingletonPtr()->mXController.ButtonHeld();
	lua_pushboolean(ls, result );
	return 1;

}

static int luaXBoxButtonReleased( lua_State* ls )
{
	bool result = DInputGuru::GetSingletonPtr()->mXController.ButtonReleased();
	lua_pushboolean(ls, result );
	return 1;
}

static int luaXBoxButtonUp( lua_State* ls )
{
	bool result = DInputGuru::GetSingletonPtr()->mXController.ButtonUp();
	lua_pushboolean(ls, result );
	return 1;
}

static const luaL_reg inputLib[] =
{
	{"KeyPressed", 			luaInputKeyPressed },
	{"KeyReleased",			luaInputKeyReleased },
	{"KeyHeld", 			luaInputKeyHeld },

	{"MouseMoving",			luaMouseMoving },
	{"MouseLeftClick",		luaMouseLeftClick },
	{"MouseRightClick",		luaMouseRightClick },
	{"MouseLeftRelease",	luaMouseLeftRelease },
	{"MouseRightRelease",	luaMouseRightRelease },

	{"MouseLeftHeld",		luaMouseLeftHeld },
	{"MouseRightHeld",		luaMouseRightHeld },

	{"XBoxBackPressed",		luaXBoxBackPressed },

	{"XBoxButtonPressed",	luaXBoxButtonPressed },
	{"XBoxButtonHeld",		luaXBoxButtonHeld },
	{"XBoxButtonReleased",	luaXBoxButtonReleased },
	{"XBoxButtonUp",		luaXBoxButtonUp},

	{NULL, NULL}
};

void RegisterInputLibrary( lua_State* state )
{
	luaL_openlib( state, "Input", inputLib, 0 );
}

#endif // if using Lua
