// This file provides a debugging interface for the engine
// to report errors and warnings back to.

#ifndef STATIC_Debugger
#define STATIC_Debugger

#include <lua.hpp>
#include <string>
#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Exception.h"

namespace Monocle
{
	class Debugger
	{
		public:
			// We pass by reference to ensure that the subclassing information
			// isn't discarded (because passing it as an argument no longer
			// requires an implicit conversion / duplication).
			static void RaiseException(Engine::Exception * err);
			static int  LuaExceptionHandle(lua_State * L);
			static int  LuaPCALLHandle(lua_State * L);

			// These functions are the debugging hooks that are associated
			// with Lua.  They are used so that the debugger can pause execution
			// at required breakpoints, etc..
			static void LuaHookInitalize(lua_State * L);
			static void LuaHookOnEvent(lua_State * L, lua_Debug * D);

		private:
			static bool m_IsConnected;
			static const char* m_PCALLFileName;
			static int m_PCALLLineNumber;
			static bool m_UsePCALLEnvironment;
	};
}

#endif