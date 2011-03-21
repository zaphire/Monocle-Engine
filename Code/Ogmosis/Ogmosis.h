#pragma once
#include <string>
#include "../Scene.h"
#include "../Graphics.h"
#include "../Level.h"

// NOTE: all this stuff is one file just for now, will refactor later

namespace Monocle
{

	class Ogmosis : public Scene
	{
	public:
		void Begin();
		void Update();
		void End();
	};
}