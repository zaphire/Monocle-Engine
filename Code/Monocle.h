#pragma once

#include "Game.h"
#include <string>

namespace Monocle
{
	//Time in seconds that has passed since the last frame
	extern double deltaTime;	

	extern double timeSinceStart;

	//Everyone's favorite food
	extern const float pi;

	//Convert radians to degrees
	extern const float rad2Deg;

	//Convert degrees to radians
	extern const float deg2Rad;

	// open a local file or remote URL
	void OpenURL(const std::string &url);

	std::string GetWorkingDirectory();
}