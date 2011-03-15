// This file provides a debugging interface for the engine
// to report errors and warnings back to.

#include "Debugger.h"
#include "LowLevel.h"
#include "Exception.h"
#include "DebuggerNotAttachedException.h"
#include "InterpreterException.h"
#include "InvalidObjectThrownException.h"
#include <iostream>
#include "Profiler.h"

namespace Monocle
{
	std::map<std::string, std::pair<double, unsigned long> > Profiler::m_LineProfileData =
		std::map<std::string, std::pair<double, unsigned long> >();
	clock_t Profiler::m_LineLastRecordedTime = clock();

	void Profiler::LuaHookOnEvent(lua_State * L, lua_Debug * D)
	{
		clock_t t = clock();

		// Store the information in memory rather than writing it out
		// to file so that we don't degrade the performance of the application
		// (significantly)
		if (D->event == LUA_MASKLINE)
		{
			lua_getinfo(L, "S", D);

			std::string ss;
			ss += D->source;
			ss += ":";
			ss += D->currentline;

			Profiler::t_LineProfileIterator i = Profiler::m_LineProfileData.find(ss);
			if (i == Profiler::m_LineProfileData.end())
			{
				// No profile data yet for this line.
				Profiler::m_LineProfileData.insert(
					std::pair<std::string, std::pair<double, unsigned long> >(
						ss,
						std::pair<double, unsigned long>(t - Profiler::m_LineLastRecordedTime, 1)
						)
					);
			}
			else
			{
				i->second.first += t - Profiler::m_LineLastRecordedTime;
				i->second.second += 1;
			}

			Profiler::m_LineLastRecordedTime = clock();
		}
	}
}