#pragma once

#include "Platform.h"

namespace Monocle
{
	class Core
	{
	public:
		Core();
		void Init();
		void Main();

		Input input;
		Graphics graphics;
		Debug debug;
	};
}