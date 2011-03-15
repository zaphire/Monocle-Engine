// The main entry point of the Monocle engine.
#include <lua.hpp>
#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "AccessViolationException.h"
#include "DivideByZeroException.h"
#include "EntryPointNotFoundException.h"
#include "OutOfMemoryException.h"
#include "InvalidSyntaxException.h"
#include "InterpreterStateNotValidException.h"

#include "Program.h"
#include "Utility.h"
#include "Debugger.h"
#include "ClassRegistration.h"
#include "LowLevel.h"
#include <iostream>

// TODO: This function needs to be moved into an appropriate class.
void lua_gettablevalue (lua_State * luaVM, int tableindex, int valueindex)
{
	lua_pushnumber (luaVM, valueindex);
	lua_gettable (luaVM, tableindex);
	// use lua_to<type>(-1); to get the value.
}

namespace Monocle
{
	lua_State * Program::GlobalState = NULL;

	int Program::Main(int argc, char *argv[])
	{
		for (int i = 0; i < argc; i += 1)
		{
			std::string arg = argv[i];
			if (arg == "-v" || arg == "--version" ||
				arg == "-h" || arg == "--help" ||
				arg == "-l" || arg == "--license")
			{
				// TODO: Print information here.
				exit(0);
			}
		}

		// Search for GameInfo.xml in the current working directory.
		if (!Utility::FileExists("./GameInfo.xml"))
		{
			Debugger::RaiseException(new Engine::EntryPointNotFoundException());
			return -1;
		}

		// Get the game information from the GameInfo.xml file.
		GameInfo info;
		Program::ParseGameInfo(info);

		// Change the working directory.
		Utility::ChangeWorkingDirectory(info.CurrentWorkingDirectory);

		// Check to make sure the entry point file exists.
		if (!Utility::FileExists(info.EntryPointFile))
		{
			Debugger::RaiseException(new Engine::EntryPointNotFoundException());
			return -1;
		}

		// Now create a Lua instance, and set up the Lua environment for
		// use within a game.
		lua_State * L = lua_open();

		// Maintain a reference to the main lua_State in the Program class.
		// This allows Exceptions to grab line number and filename information
		// without requiring lua_State to be passed as an argument.
		Program::GlobalState = L;

		if (L == NULL)
		{
			Debugger::RaiseException(new Engine::InterpreterStateNotValidException());
			return -1;
		}
		luaL_openlibs(L);

		// Ensure there is enough stack space available for Lua.
		if (lua_checkstack(L, 50) == false)
		{
			Debugger::RaiseException(new Engine::InterpreterStateNotValidException());
			return -1;
		}

		// Register the panic function.
		lua_atpanic(L, &Debugger::LuaExceptionHandle);

		// Register the debugger hooks.
		Debugger::LuaHookInitalize(L);

		// Initalize the function bindings table.
		function::Setup(L);

		// Initalize the return cache table.
		Bindings<void *>::CacheInit(L);

		// Initalize all of the classes.
		Monocle::RegisterAllClasses(L);

		// Load the entry point file up and run it.
		lua_pushcfunction(L, &Debugger::LuaPCALLHandle);
		int ret = luaL_loadfile(L, info.EntryPointFile.c_str());
		if (ret == LUA_ERRSYNTAX)
		{
			std::string str = lua_tostring(L, -1);
			lua_pop(L, 1);
			Debugger::RaiseException(new Engine::InvalidSyntaxException(str));
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Engine::OutOfMemoryException());
			return -1;
		}

		ret = lua_pcall(L, 0, 0, -2);
		if (ret == LUA_ERRRUN)
		{
			// Already handled by the PCALL error handler callback
			// at this point.
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Engine::OutOfMemoryException());
			return -1;
		}
		lua_pop(L, 1);

		// Load the entry point call string into Lua, and then run it.
		lua_pushcfunction(L, &Debugger::LuaPCALLHandle);
		ret = luaL_loadstring(L, info.EntryPointCall.c_str());
		if (ret == LUA_ERRSYNTAX)
		{
			std::string str = lua_tostring(L, -1);
			lua_pop(L, 1);
			Debugger::RaiseException(new Engine::InvalidSyntaxException(str));
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Engine::OutOfMemoryException());
			return -1;
		}

		ret = lua_pcall(L, 0, 1, -2);
		if (ret == LUA_ERRRUN)
		{
			// Already handled by the PCALL error handler callback
			// at this point.
			return -1;
		}
		else if (ret == LUA_ERRMEM)
		{
			Debugger::RaiseException(new Engine::OutOfMemoryException());
			return -1;
		}

		// Return the value from the lua_pcall if it's a numeric
		// value, otherwise ignore it and return 0.
		if (lua_isnumber(L, -1))
			ret = lua_tonumber(L, -1);
		else
			ret = 0;

		// Close the Lua interpreter.
		lua_close(L);

		return ret;
	}

	void Program::ParseGameInfo(GameInfo & out)
	{
		// TODO: Here you would parse a GameInfo.xml file which describes
		// the entry point for the game.

		out.CurrentWorkingDirectory = ".";
		out.Debuggable = false;
		out.EntryPointFile = "main.lua";
		out.EntryPointCall = "Main();";
	}
}