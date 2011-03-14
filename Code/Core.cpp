#include "Core.h"

namespace Monocle
{
	Core::Core()
	{
	}

	void Core::Init()
	{
		debug.Init();

		input.Init();

		graphics.Init();
	}

	void Core::Main()
	{
		bool isDone = false;
		while (!isDone)
		{
			Debug::Log("Hello, World!");
		}
	}
}