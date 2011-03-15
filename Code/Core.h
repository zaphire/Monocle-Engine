#pragma once

#include "Input.h"
#include "Graphics.h"
#include "Debug.h"
#include "Scene.h"

namespace Monocle
{
	class Core
	{
	public:
		Core();
		virtual void Init();
		virtual void Update();

		void Main();

		Platform platform;
		Input input;
		Graphics graphics;
		Debug debug;
		Scene scene;
	};
}