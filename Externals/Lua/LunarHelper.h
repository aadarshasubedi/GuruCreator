/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    LunarHelper.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300
**  \par     Updated: 09/12/2008 @ 19:31:28 - nwilliam
**  \brief   Example usage and helper macros for using the Lunar template class.
*******************************************************************************/

#ifndef SF_LUNAR_HELPER_H
#define SF_LUNAR_HELPER_H

#ifdef USE_LUA
#include "lunar.h"

/* Example Usage:
  Foo.h:
    class Foo
    {
      public:
        void DoFoo();
        void DoBar();
        
      private:
        int m_foovar;
      
      
      LUA_DECLARE_CLASS( Foo )
      LUA_DECLARE_METHOD( LDoFoo )
      LUA_DECLARE_METHOD( LDoBar )
      LUA_DECLARE_METHOD( LuaOnlyMethod )
    };
  
  
  Foo.cpp
    Foo::DoFoo() { ...yada yada... }
    Foo::DoBar() { ... more yada .. }
    
    
    LAU_START_DEFINE_CLASS( Foo )
      LUA_NAME_METHOD( Foo, "DoFoo", LDoFoo )
      LUA_NAME_METHOD( Foo, "DoBar", LDoBar )
      LUA_NAME_METHOD( Foo, "LuaOnlyMethod", LuaOnlyMethod )
    LUA_END_DEFINE_CLASS
    
    LUA_DEFINE_METHOD( Foo, LDoFoo ) { ... lua yada yada ... }
    LUA_DEFINE_METHOD( Foo, LDoBar ) { ... lua more yada ... }
    LUA_DEFINE_METHOD( Foo, LuaOnlyMethod ) { ... lua only lols ... }
*/


// Displays a message box containing the last error code, the erroneous file, 
//  and erroneous line number.
#define PRINT_ERR_LUASTACK(L, errorStr)\
	{ \
		std::string error_msg(errorStr); \
		luaL_dostring( L, "return debug.traceback()" ); \
		error_msg += "\n"; \
		error_msg += lua_tostring( L, -1 ); \
		lua_pop( L, -1 ); \
		error_msg += "\n"; \
		DLogGuru::GetSingletonPtr()->LogSingleMessage(error_msg.c_str(), true, true); \
	}

//#define CHECK_PARAM_LUASTACK(L, count, errorStr)\

//
// These are used in the header / class declaration
//
#define LUA_DECLARE_CLASS( ClassName )\
  public:\
    static const char className[];\
    static Lunar<ClassName>::RegType methods[];

#define LUA_DECLARE_METHOD( MethodName )\
  int MethodName( lua_State* ls );


//
// These are used in the cpp / class implementation
//
#define LUA_START_DEFINE_CLASS( ClassName )\
  const char ClassName::className[] = #ClassName;\
  Lunar< ClassName >::RegType ClassName::methods[] = {\

#define LUA_NAME_METHOD( ClassName, NameInLua, Pointer )\
  { NameInLua, &ClassName::Pointer },

#define LUA_END_DEFINE_CLASS\
  { 0 , 0 } };\

#define LUA_DEFINE_METHOD( ClassName, MethodName )\
  int ClassName::MethodName( lua_State* ls )

#define LUA_DEFINE_METHOD_NOLS( ClassName, MethodName )\
  int ClassName::MethodName( lua_State* )


//
// This is used by whatever (ex. ScriptEngine) is running the master lua state
//
#define LUA_REGISTER_CLASS( ClassName, LuaState )\
  Lunar<ClassName>::Register( LuaState );


#endif // USE_LUA check

#endif // LUNAR_HELPER_H
