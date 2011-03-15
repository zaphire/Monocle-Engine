/*
** $Id: lualib.h,v 1.36.1.1 2007/12/27 13:02:25 roberto Exp $
** Lua standard libraries
** See Copyright Notice in lua.h
*/


#ifndef lualib_h
#define lualib_h

#include "lua.h"


/* Key to file-handle type */
#define LUA_FILEHANDLE		"FILE*"


#define LUA_COLIBNAME	"Coroutine"
LUALIB_API int (luaopen_base) (lua_State *L);

#define LUA_TABLIBNAME	"Table"
LUALIB_API int (luaopen_table) (lua_State *L);

#define LUA_IOLIBNAME	"IO"
LUALIB_API int (luaopen_io) (lua_State *L);

#define LUA_OSLIBNAME	"OS"
LUALIB_API int (luaopen_os) (lua_State *L);

#define LUA_STRLIBNAME	"String"
LUALIB_API int (luaopen_string) (lua_State *L);

#define LUA_MATHLIBNAME	"Math"
LUALIB_API int (luaopen_math) (lua_State *L);

#define LUA_DBLIBNAME	"Debug"
LUALIB_API int (luaopen_debug) (lua_State *L);

#define LUA_LOADLIBNAME	"Package"
LUALIB_API int (luaopen_package) (lua_State *L);


/* open all previous libraries */
LUALIB_API void (luaL_openlibs) (lua_State *L); 



#ifndef lua_assert
#define lua_assert(x)	((void)0)
#endif


#endif
