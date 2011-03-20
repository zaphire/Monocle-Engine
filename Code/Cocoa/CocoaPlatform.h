#pragma once

#include "../Platform.h"

#include <sys/time.h>

namespace Monocle
{
	class CocoaPlatform
	{
	public:
		static CocoaPlatform* instance;

		CocoaPlatform();
		~CocoaPlatform();

		bool Init(int w, int h, int bits, bool fullscreen);

		NSWindow* window;
	};
}

/* vi: set ts=4 sw=4 noexpandtab: */

