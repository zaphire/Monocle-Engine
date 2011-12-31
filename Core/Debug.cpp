#include "Debug.h"

#include <iostream>
#include <fstream>
#include "Entity.h"

namespace Monocle
{
	bool Debug::render = false;
	bool Debug::showBounds = false;
	Entity *Debug::selectedEntity;
	int Debug::layerMin = -50;
	int Debug::layerMax = 50;
	
	void Debug::Init()
	{
		render = false;
		logout.open("monocle.log");
		std::clog.rdbuf(logout.rdbuf());
	}

	void Debug::Log(const char *outputString)
	{
		// poop stuff out to a file!
		//outputString

		std::clog << outputString << std::endl;
	}

	void Debug::Log(bool boolean)
	{
		std::clog << (boolean?"true":"false") << std::endl;
	}

	void Debug::Log(int num)
	{
		std::clog << num << std::endl;
	}

	void Debug::Log(long num)
	{
		std::clog << num << std::endl;
	}

	void Debug::Log(float num)
	{
		std::clog << num << std::endl;
	}

	void Debug::Log(double num)
	{
		std::clog << num << std::endl;
	}

	void Debug::Log(const Vector2& vec)
	{
		std::clog << "Vector2: (" << vec.x << ", " << vec.y << ")" << std::endl;
	}

	void Debug::Log(const Vector3& vec)
	{
		std::clog << "Vector3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
	}

	void Debug::Log(const std::string& string)
	{
		std::clog << string << std::endl;
	}
}
