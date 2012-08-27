/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef DSCRIPTGURU_H_INCLUDED
#define DSCRIPTGURU_H_INCLUDED


//If you want to use a pre-existing Lua system, uncomment this.
//#define USE_LUA 1

#ifdef USE_LUA
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	#include <luaconf.h>
}
#include "LunarHelper.h"
#endif

#include "DScriptDisciple.h"
#include "DSingleton.h"


class DScriptDisciple;
const unsigned MAX_SCRIPT_DISCIPLES = 12;


class DScriptGuru : public DSingleton<DScriptGuru>
{
//The lua manager needs to hold the main lua_state pointer, as well
// as create a ScriptDisiciple* that will actually hold all of the subsequent
// scripts after.  The main lua_State pointer is the entry point for my Lua implementation.
public:
	DScriptGuru();
	~DScriptGuru();


	//Initialize the master Lua state.  Basically creates Lua and allows an entry point for the
	// scripts to be running.
	void UpdateDisciples(float time);

private:
	//Here are all of the loaded scripts.  Pointers and offsets, woot woot!!
	unsigned			m_numDisciples;
	DScriptDisciple*	m_Disciples;
		
#ifdef USE_LUA
public:
	inline lua_State* GetMainLuaState() { return m_mainScriptState; };
	void OpenLibrary( const char* libName, const char* libRegFuncName,
						int (*funcPtr)(lua_State*), luaL_reg libTable[] );

	void InitMasterState();
		
	//Creates a loads a script from file and stores it to be updated.
	bool CreateImmediateDisciple( const char* filename )
	{
		//Since the guru owns the master state, I just need the filename.  This looks a lot
		// like the constructor for the Disciple, but is called later because of the pre-allocation
		// of the script memory.
		if( !m_Disciples[m_numDisciples].Init( m_mainScriptState )	)
		{
			const char* error = lua_tostring( m_mainScriptState, -1 );
			DLogGuru::GetSingletonPtr()->LogMultipleMessage( 1, "%s%s%s/n%s", "Error creating script:  <", filename, ">", error );
			return false;
		}

		m_Disciples[m_numDisciples].Load( filename );
		m_Disciples[m_numDisciples].Update( 0 );
		++m_numDisciples;
		return true;
	}
		
	template <typename T>
	void RegisterGlobal(T *global, const char *name);

	LUA_DECLARE_CLASS( DScriptGuru )
	LUA_DECLARE_METHOD( LCreateImmediateDisciple )
private:
	lua_State*			m_mainScriptState;
#else //if not using Lua, default state.
	void*				m_mainScriptState;
#endif //if using Lua
};


#ifdef USE_LUA
template <typename T>
void DScriptGuru::RegisterGlobal(T *global, const char *name)
{
  Lunar<T>::push(m_mainScriptState, global);
  lua_setglobal(m_mainScriptState, name);
}
#endif //if using Lua


#endif // DSCRIPTGURU_H_INCLUDED
