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

	void Debug::Log(int num)
	{
		std::cout << num << std::endl;
	}

	void Debug::Log(long num)
	{
		std::cout << num << std::endl;
	}

	void Debug::Log(float num)
	{
		std::cout << num << std::endl;
	}

	void Debug::Log(double num)
	{
		std::cout << num << std::endl;
	}

	void Debug::Log(const Vector2& vec)
	{
		std::cout << "Vector2: (" << vec.x << ", " << vec.y << ")" << std::endl;
	}

	void Debug::Log(const Vector3& vec)
	{
		std::cout << "Vector3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
	}
}