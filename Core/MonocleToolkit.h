#pragma once

#include "Macros.h"
#include <string>
#include <sstream>

namespace Monocle
{
	//! Time in seconds that has passed since the last frame
	extern double deltaTime;	

	//! Time in seconds since game started
	extern double timeSinceStart;

	//! Time in seconds since current scene started
	extern double timeSinceSceneStart;
    
    //! Entities drawn last frame
    extern long entitiesDrawn;

	//! Math symbol PI
	extern const float pi;

	//! Convert radians to degrees
	extern const float rad2Deg;

	//! Convert degrees to radians
	extern const float deg2Rad;

	//! Open a local file or remote URL
	void OpenURL(const std::string &url);
	//! return the current working directory
	std::string GetWorkingDirectory();
	//! call a callback function for each file in the directory with file extension equal to type
	void ForEachFile(std::string path, std::string type, void callback(const std::string &filename, intptr_t param), intptr_t param);
	
	template <class T>
	std::string StringOf(T object)
	{
		std::ostringstream os;
		os << object;
		return(os.str());
	};
    
    void PremultiplyAlpha( unsigned char *data, int w, int h );
}
