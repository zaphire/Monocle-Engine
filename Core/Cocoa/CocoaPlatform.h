#pragma once

#include "../Platform.h"
#include "CocoaWindowListener.h"

#include <sys/time.h>

namespace Monocle
{
	class CocoaPlatform
	{
	public:
		static CocoaPlatform* instance;
		static Platform* platform;

		~CocoaPlatform();

		void Init(const std::string &name, int w, int h, int bits, bool fullscreen);

		NSWindow* window;
		WindowData* windowData;
        
        std::string bundleResourcesPath;
	};
}

/* vi: set ts=4 sw=4 noexpandtab: */

