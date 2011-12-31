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
	bool Debug::useFile = false;
	bool Debug::useConsole = true;
	
	void Debug::Init()
	{
		render = false;
		logout.open("monocle.log");
		std::clog.rdbuf(logout.rdbuf());
	}

	void Debug::Log(const char *outputString)
	{
		if (useConsole)
			std::cout << outputString << std::endl;
		if (useFile)
			std::clog << outputString << std::endl;
	}

	void Debug::Log(bool boolean)
	{
		if (useConsole)
			std::cout << (boolean?"true":"false") << std::endl;
		if (useFile)
			std::clog << (boolean?"true":"false") << std::endl;
	}

	void Debug::Log(int num)
	{
		if (useConsole)
			std::cout << num << std::endl;
		if (useFile)
			std::clog << num << std::endl;
	}

	void Debug::Log(long num)
	{
		if (useConsole)
			std::cout << num << std::endl;
		if (useFile)
			std::clog << num << std::endl;
	}

	void Debug::Log(float num)
	{
		std::clog << num << std::endl;
	}

	void Debug::Log(double num)
	{
		if (useConsole)
			std::cout << num << std::endl;
		if (useFile)
			std::clog << num << std::endl;
	}

	void Debug::Log(const Vector2& vec)
	{
		if (useConsole)
			std::cout << "Vector2: (" << vec.x << ", " << vec.y << ")" << std::endl;
		if (useFile)
			std::clog << "Vector2: (" << vec.x << ", " << vec.y << ")" << std::endl;
	}

	void Debug::Log(const Vector3& vec)
	{
		if (useConsole)
			std::cout << "Vector3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
		if (useFile)
			std::clog << "Vector3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
	}

	void Debug::Log(const std::string& string)
	{
		if (useConsole)
			std::cout << string << std::endl;
		if (useFile)
			std::clog << string << std::endl;
	}
	
	void Debug::SetFileLogging(bool useFile)
	{
		Debug::useFile = useFile;
	}

	void Debug::SetConsoleLogging(bool useConsole)
	{
		Debug::useConsole = useConsole;
	}

	bool Debug::GetFileLogging()
	{
		return useFile;
	}

	bool Debug::GetConsoleLogging()
	{
		return useConsole;
	}
}
