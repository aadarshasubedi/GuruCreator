/***** All content © 2008 DigiPen (USA) Corporation, all rights reserved. *****/
/*! \file    lunar.h
**  \author  Nathan Williams
**  \par     email: nwilliam\@digipen.edu
**  \par     Course: GAM300/350
**  \par     Updated: 02/16/2009 @ 14:21:44 -- nwilliam
**  \brief   Slightly modified version of Lunar. Removed ability to construct
             bound objects from Lua, reformated, simplified some code.
             Original: http://lua-users.org/wiki/CppBindingWithLunar
*******************************************************************************/

#ifndef LUNAR_H
#define LUNAR_H

#include "lua.hpp"
#include <string>

//Thomas O'Connor's light version
//doesn't require that the class being bound has a methods table
//just binds the data type only
template< typename T >
class LunarLite
{
	struct userdataType
	{
		T *pT;
	};

public:
	static std::string className;

	static void Register(lua_State* L, const char* cname)
    {
		className = cname;
		luaL_newmetatable(L, className.c_str() );
		int metatable = lua_gettop(L);

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, tostring);
		lua_settable(L, metatable);

		lua_pop(L, 1);  // drop metatable
    }

	// push onto the Lua stack a userdata containing a pointer to T object
    static int push( lua_State* L, T *obj )
    {
		if( !obj )
		{
			lua_pushnil(L);
			return 0;
		}

		luaL_getmetatable(L, className.c_str());  // lookup metatable in Lua registry

		if( lua_isnil(L, -1) )
		{
			lua_pushfstring( L, "No metatable on %s (is it registered)?", className.c_str() );
			const char* err = lua_tostring(L,-1);
			throw( err );
		}

		int mt = lua_gettop(L);

		subtable(L, mt, "userdata", "v");

		userdataType *ud = static_cast<userdataType*>( pushuserdata( L , obj , sizeof(userdataType) ) );

		if( ud )
		{
			ud->pT = obj;  // store pointer to object in userdata
			lua_pushvalue(L, mt);
			lua_setmetatable(L, -2);
		}

		lua_replace(L, mt);
		lua_settop(L, mt);
		return mt;  // index of userdata containing pointer to T object
    }

	// get userdata from Lua stack and return pointer to T object
	static T *check(lua_State* L, int narg)
	{
		userdataType *ud = static_cast<userdataType*>(luaL_checkudata(L, narg, className.c_str()));
		if(!ud) luaL_typerror(L, narg, className.c_str());
		return ud->pT;
	}

private:
	// No constructing, copying, or assigning Lunar objects
	LunarLite();
	LunarLite( const LunarLite<T>& );
	LunarLite& operator=( const LunarLite<T>& );

	static int tostring(lua_State* L)
    {
		userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
		lua_pushfstring(L, "%s (%p)", className.c_str(), ud->pT);
		return 1;
    }

	static void weaktable(lua_State* L, const char *mode)
    {
      lua_newtable(L);
      lua_pushvalue(L, -1);  // table is its own metatable
      lua_setmetatable(L, -2);
      lua_pushliteral(L, "__mode");
      lua_pushstring(L, mode);
      lua_settable(L, -3);   // metatable.__mode = mode
    }

    static void subtable(lua_State* L, int tindex, const char *name, const char *mode)
    {
      lua_pushstring(L, name);
      lua_gettable(L, tindex);

      if (lua_isnil(L, -1))
      {
        lua_pop(L, 1);
        lua_checkstack(L, 3);
        weaktable(L, mode);
        lua_pushstring(L, name);
        lua_pushvalue(L, -2);
        lua_settable(L, tindex);
      }
    }

    static void *pushuserdata(lua_State* L, void *key, size_t sz)
    {
      void *ud = 0;
      lua_pushlightuserdata(L, key);
      lua_gettable(L, -2);     // lookup[key]

      if (lua_isnil(L, -1))
      {
        lua_pop(L, 1);         // drop nil
        lua_checkstack(L, 3);
        ud = lua_newuserdata(L, sz);  // create new userdata
        lua_pushlightuserdata(L, key);
        lua_pushvalue(L, -2);  // dup userdata
        lua_settable(L, -4);   // lookup[key] = userdata
      }

      return ud;
    }
};

template<class T> 
std::string LunarLite<T>::className; //definition

template< typename T >
class Lunar
{
  struct userdataType
  {
    T *pT;
  };

  public:
    typedef int (T::*mfp)(lua_State* L);

    struct RegType
    {
      const char *name;
      mfp mfunc;
    };

    static void Register(lua_State* L)
    {
      lua_newtable(L);
      int methods = lua_gettop(L);

      luaL_newmetatable(L, T::className);
      int metatable = lua_gettop(L);

      // store method table in globals so that
      // scripts can add functions written in Lua.
      lua_pushstring(L, T::className);
      lua_pushvalue(L, methods);
      lua_settable(L, LUA_GLOBALSINDEX);

      // hide metatable from Lua getmetatable()
      lua_pushstring(L, "__metatable");
      lua_pushvalue(L, methods);
      lua_settable(L, metatable);

      lua_pushstring(L, "__index");
      lua_pushvalue(L, methods);
      lua_settable(L, metatable);

      lua_pushstring(L, "__tostring");
      lua_pushcfunction(L, tostring);
      lua_settable(L, metatable);

      lua_newtable(L);                // mt for method table

      lua_setmetatable(L, methods);

      // fill method table with methods from class T
      for( RegType* reg = T::methods; reg->name; ++reg )
      {
        lua_pushstring(L, reg->name);
        lua_pushlightuserdata(L, reg);
        lua_pushcclosure(L, thunk, 1);
        lua_settable(L, methods);
      }

      lua_pop(L, 2);  // drop metatable and method table
    }

    // push onto the Lua stack a userdata containing a pointer to T object
    static int push( lua_State* L, T *obj )
    {
      if( !obj )
      {
        lua_pushnil(L);
        return 0;
      }


      luaL_getmetatable(L, T::className);  // lookup metatable in Lua registry

      if( lua_isnil(L, -1) )
      {
        lua_pushfstring( L, "No metatable on %s (is it registered)?", T::className );
        const char* err = lua_tostring(L,-1);
        throw( err );
      }

      int mt = lua_gettop(L);

      subtable(L, mt, "userdata", "v");

      userdataType *ud = static_cast<userdataType*>( pushuserdata( L , obj , sizeof(userdataType) ) );

      if( ud )
      {
        ud->pT = obj;  // store pointer to object in userdata
        lua_pushvalue(L, mt);
        lua_setmetatable(L, -2);
      }

      lua_replace(L, mt);
      lua_settop(L, mt);
      return mt;  // index of userdata containing pointer to T object
    }

    // get userdata from Lua stack and return pointer to T object
    static T *check(lua_State* L, int narg)
    {
      userdataType *ud = static_cast<userdataType*>(luaL_checkudata(L, narg, T::className));
      if(!ud) luaL_typerror(L, narg, T::className);
      return ud->pT;
    }

  private:
    // No constructing, copying, or assigning Lunar objects
    Lunar();
    Lunar( const Lunar<T>& );
    Lunar& operator=( const Lunar<T>& );

    // member function dispatcher
    static int thunk(lua_State* L)
    {
      // stack has userdata, followed by method args
      T *obj = check(L, 1); // get 'self'
      lua_remove(L, 1);     // remove member function args start at index 1

      // get member function from upvalue
      RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
      return (obj->*(l->mfunc))(L);
    }

    static int tostring(lua_State* L)
    {
      userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
      lua_pushfstring(L, "%s (%p)", T::className, ud->pT);
      return 1;
    }

    static void weaktable(lua_State* L, const char *mode)
    {
      lua_newtable(L);
      lua_pushvalue(L, -1);  // table is its own metatable
      lua_setmetatable(L, -2);
      lua_pushliteral(L, "__mode");
      lua_pushstring(L, mode);
      lua_settable(L, -3);   // metatable.__mode = mode
    }

    static void subtable(lua_State* L, int tindex, const char *name, const char *mode)
    {
      lua_pushstring(L, name);
      lua_gettable(L, tindex);

      if (lua_isnil(L, -1))
      {
        lua_pop(L, 1);
        lua_checkstack(L, 3);
        weaktable(L, mode);
        lua_pushstring(L, name);
        lua_pushvalue(L, -2);
        lua_settable(L, tindex);
      }
    }

    static void *pushuserdata(lua_State* L, void *key, size_t sz)
    {
      void *ud = 0;
      lua_pushlightuserdata(L, key);
      lua_gettable(L, -2);     // lookup[key]

      if (lua_isnil(L, -1))
      {
        lua_pop(L, 1);         // drop nil
        lua_checkstack(L, 3);
        ud = lua_newuserdata(L, sz);  // create new userdata
        lua_pushlightuserdata(L, key);
        lua_pushvalue(L, -2);  // dup userdata
        lua_settable(L, -4);   // lookup[key] = userdata
      }

      return ud;
    }
};

#endif
