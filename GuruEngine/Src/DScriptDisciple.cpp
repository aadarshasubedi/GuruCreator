/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#include "DScriptGuru.h"
#include "DScriptDisciple.h"
#include "Logger.h"


#ifdef USE_LUA
DScriptDisciple::DScriptDisciple(lua_State* master, const char* theFile)
{
	//Make sure the master can initialize this script in it's thread pool.  If so,
	// I can go ahead and then load in the actual script file with that code to be
	// executed.
	if( !Init( master ) )
	{
	}
	else
	{
		//Actually load in the script file.  Uses the luaL_loadfile to do this.  These
		// scripts are essentially going to be coroutines defined in a file format.
		Load( theFile );
	}
}
#endif //if using Lua

DScriptDisciple::DScriptDisciple()
{
	//Do nothing, guru will Initialize and load later.
}

DScriptDisciple::~DScriptDisciple()
{
}


bool DScriptDisciple::Load( const char* filename ) //parameter: "filename" - whatever script file to be loaded.
{
#ifdef USE_LUA
	//Now that the lua state has been created on a thread from the main state
	// I can load a script there from a file to update the logic in the file.
	int status = luaL_loadfile( m_luastate, filename );

	//Status is something other than zero if there was a problem.  Quick check for that.
	if( !status )
	{
		//The status variable wasn't negative, so good load on the file status.
		m_scriptstate = LS_LOADED;
		return true;
	}

	//@LOGGER.
	const char* error = lua_tostring( m_luastate, -1 );

	//log the error string somehow.
	DLogGuru::GetSingletonPtr()->LogSingleMessage( error, true, true );
#else
	FILE* fp = NULL;
	errno_t error = fopen_s(&fp, filename, "r");

	if( error ) { return false; }
	if( fp == NULL ) { return false; }

	//Do Stuff with the file.
	//....
	//

	fclose(fp);

#endif
	return false;
}


int DScriptDisciple::Update( float elapsedTime) //parameter: "elapsedTime" - to keep track of timers and such when needed.
{
	//Depending on the status of the script, I'll run either
	int status = 0;  //status variable to get errors calling lua functions.

#ifdef USE_LUA
	switch( m_scriptstate )
	{
		case LS_UNLOADED:
		{
			//The thread has been created but no file loaded in yet.
			break;
		}

		case LS_CRASHED:
			{
				break;
			}

		case LS_LOADED:
		{
			//A script file has been loaded in, so push the necessary data and
			// call resume with the elapsedTime.
			lua_pushnumber( m_luastate, elapsedTime );
			status = lua_resume( m_luastate, 1 );

			if( status == 0 )
			{
				m_scriptstate = LS_UNLOADED;
			}
			else if( status != LUA_YIELD )
			{
				//	#define LUA_YIELD	1
				//#define LUA_ERRRUN	2
				//#define LUA_ERRSYNTAX	3
				//#define LUA_ERRMEM	4
				//#define LUA_ERRERR	5
				//If resume returned zero while the script is in the loaded state, this means
				// it has ran and is done so we need to unload it.
				m_scriptstate = LS_CRASHED;
			}
			break;
		}

		case LS_RUNNING:
		{
			//If it's running, then make sure to pass the current elapsedTime in there and keep
			// resuming the script.
			break;
		}

		case LS_PAUSED:
		{
			//The script is paused, so resume with 0 as it's input.
			lua_pushnumber( m_luastate, elapsedTime );
			status = lua_resume( m_luastate, 1 );
			if( status == 0 )
			{ 
				m_scriptstate = LS_UNLOADED;
			}
			else if( status != LUA_YIELD )
			{
				m_scriptstate = LS_CRASHED;
			}
			
			break;
		}

		default:
		{
			break;
		}
	}

	//If the status was bad for any of these calls, output that to a logger or screen.
	//Again, status is non-zero with errors.
	if( status == 0 )
	{
		//There's nothing bad with it.  woo hoo.
	}
	else if( status == LUA_YIELD)
	{
		//The status will usually be here for a running script since it will be using a coroutine.
		// Yield is one of the coroutine statuses, and so this will be the main status for most
		// of the scripts most of the time.
	}
	else
	{
		//There was an error in resuming the script in it's current state.  Send out an error message
		// to the logger what the status is, the state pointer, the file it's associated with, and
		// of course the error message that Lua beautifully puts up on top of the stack for me.
		//@LOGGER.
		if( lua_gettop(m_luastate) > 0 )
		{
			const char* errorMessage = lua_tostring( m_luastate, -1 );
			PRINT_ERR_LUASTACK(m_luastate, errorMessage );
		}
		else
		{
			PRINT_ERR_LUASTACK(m_luastate, "ScriptGuru: Unknown error" );
		}
	}
#else
	//Do something with the time here, like grab framerate.
	static float accum = 0.0f;
	accum += elapsedTime;

#endif //if using Lua

	return status;
}
