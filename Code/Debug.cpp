#include "Debug.h"

#include <iostream>

namespace Monocle
{
	void Debug::Init()
	{
	}

	void Debug::Log(const char *outputString)
	{
		// poop stuff out to a file!
		//outputString

		std::cout << outputString << std::endl;
	}
}