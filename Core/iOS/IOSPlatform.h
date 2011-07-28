//
//  IOSPlatform.h
//  MonocleIOS
//
//  Created by Josh Whelchel on 7/28/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_IOS

#pragma once

#include "../Platform.h"
#include "EAGLView.h"

#include <sys/time.h>

namespace Monocle
{
	class IOSPlatform
	{
	public:
		static IOSPlatform* instance;
		static Platform* platform;
        
		~IOSPlatform();
        
		void Init(const std::string &name, int w, int h, int bits, bool fullscreen);
        
		static EAGLView *view;
		static UIWindow *window;
	};
}

#endif
