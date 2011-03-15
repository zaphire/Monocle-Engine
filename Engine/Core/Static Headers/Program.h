// The main entry point of the Monocle engine.

#ifndef STATIC_Program
#define STATIC_Program

#include <string>
#include "lua.h"

namespace Monocle
{
	struct GameInfo
	{
		std::string EntryPointCall;
		std::string EntryPointFile;
		std::string CurrentWorkingDirectory;
		bool Debuggable;
	};

	class Program
	{
		public:
			static lua_State * GlobalState;

			static int Main(int argc, char *argv[]);
			static void ParseGameInfo(GameInfo & out);
	};
}

#endif