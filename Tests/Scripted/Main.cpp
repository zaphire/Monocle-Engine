#include <Game.h>

int main(void)
{
	return 0;
}


/*
extern "C"
{
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
};

// Test LUA scripting.

using namespace Monocle;


void initLuaState(lua_State **L)
{
	lua_pushcfunction(*L, luaopen_io);
	lua_call(*L, 0, 0);

	lua_pushcfunction(*L, luaopen_table);
	lua_call(*L, 0, 0);

	lua_pushcfunction(*L, luaopen_string);
	lua_call(*L, 0, 0);

	lua_pushcfunction(*L, luaopen_math);
	lua_call(*L, 0, 0);
}

int main(void)
{
	// run require with module loader code
	// run main.lua

	int iErr = 0;
	lua_State *L = luaL_newstate();

	initLuaState(&L);

	if ((iErr = luaL_loadfile (L, std::string(Platform::GetDefaultContentPath()+"/Scripted/main.lua").c_str())) == 0)
	{
		// Call main...
		if ((iErr = lua_pcall (L, 0, LUA_MULTRET, 0)) == 0)
		{ 
			// Push the function name onto the stack
			lua_pushstring (L, "HelloWorld");
			// Function is located in the Global Table
			lua_gettable (L, LUA_GLOBALSINDEX);  
			lua_pcall (L, 0, 0, 0);
		}
		else
		{
			printf("pcall fail\n");
		}
	}
	else
	{
		printf("Couldn't load file!\n");
	}

	lua_close(L);

	return 0;
}
*/