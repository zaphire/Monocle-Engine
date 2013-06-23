// This file provides a profiling interface for the engine
// to detect the time which lines and functions take on average.

#ifndef STATIC_Profiler
#define STATIC_Profiler

#include <lua.hpp>
#include <string>
#include <map>
#include "autobind/types.h"
#include "autobind/binding/lua.h"
#include "Exception.h"
#include <time.h>

namespace Monocle
{
	class Profiler
	{
		public:
			// This function is used to handle the hook events which are raised.
			// The Debugger class will call this static function when it receives
			// the hook events.
			static void LuaHookOnEvent(lua_State * L, lua_Debug * D);

		private:
			typedef std::map<std::string, std::pair<double, unsigned long> >::iterator t_LineProfileIterator;
			static std::map<std::string, std::pair<double, unsigned long> > m_LineProfileData;
			static clock_t m_LineLastRecordedTime;
	};
}

#endif