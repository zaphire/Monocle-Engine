// Utility functions used by the engine, such as checking
// whether files exist.

#ifndef STATIC_Utility
#define STATIC_Utility

#include <string>
#include "autobind/types.h"

namespace Monocle
{
	class Utility
	{
		public:
			static bool FileExists(std::string path);
			static void ChangeWorkingDirectory(std::string path);
			static std::string Replace(std::string input, std::string from, std::string to, int count);
			static std::string ReplaceAll(std::string input, std::string from, std::string to);
			static std::string ReplaceOnce(std::string input, std::string from, std::string to);
	};
}

#endif