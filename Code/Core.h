#pragma once

#include "Input.h"
#include "Graphics.h"
#include "Debug.h"

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