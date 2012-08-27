/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef DSCRIPTDISCIPLE_H_INCLUDED
#define DSCRIPTDISCIPLE_H_INCLUDED

#include "Logger.h"

//Lua includes.
class DScriptGuru;


#ifdef USE_LUA
//Very simple script class that holds the lua_state*.  It will also
// hold values for error checking and debugging.
enum LUA_SCRIPT_STATE
{
	LS_INIT,
	LS_UNLOADED,

	LS_LOADED,
	LS_RUNNING,
	LS_PAUSED,
	LS_CRASHED,

	LS_NUM_STATES
};
#endif


class DScriptDisciple
{
public:
	DScriptDisciple();
	~DScriptDisciple();

	int Update( float elapsedTime );
	bool Load( const char* filename );

private:
	int			m_error;

#ifdef USE_LUA
public:
	DScriptDisciple(lua_State* master, const char* theFile);
	lua_State* 	m_luastate;
	LUA_SCRIPT_STATE m_scriptstate;

	LUA_SCRIPT_STATE&	State() { return m_scriptstate; }	

	inline
	bool Init(lua_State* master)
#else
	inline
	bool Init(void* master = NULL)
#endif //if using Lua
	{
#ifdef USE_LUA
		//The master state needs to create a new thread and put the script on it.
		//At this point, the lua
		m_luastate = lua_newthread( master );

		//If the state was allocated properly, then I can go ahead and set the script state
		// and push some necessary data into the script stack, namely the pointer to this actual
		// script.  This way states can be matched to the files they come from.
		if( m_luastate )
		{
			m_scriptstate = LS_UNLOADED;

			//Puts this state onto the master state as light user data.
			lua_pushlightuserdata( master, m_luastate );
			//Puts the pointer to this script disciple as the value for this key-value entry into the table.
			lua_pushlightuserdata( master, this );

			//This actually finishes the job and sets everything I put into the stack and makes sure to set
			// it to the appropriate Global table index.
			lua_settable( master, LUA_GLOBALSINDEX );
			return true;
		}

		//Badness happened when making the script thread.  Log some kind of error.
		const char* errorMsg = lua_tostring( master, -1 );
		DLogGuru::GetSingletonPtr()->LogSingleMessage( errorMsg, true, true );
#else
		//@TODO: Do something different obviously.
		master = NULL;
#endif
		return false;
	}
};


#endif // DSCRIPTDISCIPLE_H_INCLUDED
