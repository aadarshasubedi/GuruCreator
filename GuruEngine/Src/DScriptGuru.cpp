/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#include "DScriptGuru.h"
#include "DScriptDisciple.h"
#include "DScriptLibraries.h"
#include "Logger.h"
#ifdef USE_LUA
#include "Lunar.h"
#endif //if using Lua

DScriptGuru::DScriptGuru() : m_mainScriptState(NULL)
{
	//Initializes/Opens Lua and loads all necessary libraries.
	//@Note: Only when using Lua.
	#ifdef USE_LUA
		InitMasterState();
	#endif

	//Creates the list of total possible scripts that can be created.
	m_Disciples = new DScriptDisciple[MAX_SCRIPT_DISCIPLES];
	m_numDisciples = 0;
}

DScriptGuru::~DScriptGuru()
{
	delete [] m_Disciples;
}

#ifdef USE_LUA
void DScriptGuru::InitMasterState()
{
	m_mainScriptState = lua_open();
	luaL_openlibs( m_mainScriptState );

	//...From this point on, any libraries that get loaded in are bound to the main master state.
	RegisterLibary( m_mainScriptState );
	RegisterInputLibrary( m_mainScriptState );
	//RegisterObjectLibrary(m_mainScriptState);

	//lua_pushcfunction( m_mainScriptState, RegisterObjectLibrary );
	//lua_setglobal(m_mainScriptState, "RegisterGameObjectLibrary");
	//luaL_dostring(m_mainScriptState, "RegisterGameObjectLibrary()"); // works correctly
	//Load all appropriate libraries here...  or later with the OpenLibrary() member method.
}
#endif //if using Lua


void DScriptGuru::UpdateDisciples(float time)
{
	//go through disciple list and update.
	for( unsigned d_iter = 0; d_iter < m_numDisciples; ++d_iter )
	{
		//Call update on each disciple.
		m_Disciples[d_iter].Update( time );
	}
}


#ifdef USE_LUA



void DScriptGuru::OpenLibrary( const char* , const char* libRegFuncName,
					int (*funcPtr)(lua_State*), luaL_reg [] )
{

	lua_pushcfunction( m_mainScriptState, funcPtr );
	lua_setglobal(m_mainScriptState, libRegFuncName);
	std::string textFunc(libRegFuncName);
	textFunc += "()";
	luaL_dostring(m_mainScriptState, textFunc.c_str()); // works correctly
}


LUA_START_DEFINE_CLASS( DScriptGuru )
	LUA_NAME_METHOD( DScriptGuru, "CreateImmediateDisciple", LCreateImmediateDisciple )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( DScriptGuru, LCreateImmediateDisciple )
{
	const char* filename = lua_tostring(ls,1);
	CreateImmediateDisciple( filename );
	return 0;
}
#endif //if using Lua
