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
		void Init();

		void Main();

		void SetScene(Scene* scene);
		Scene* GetScene();

		Platform platform;
		Input input;
		Graphics graphics;
		Debug debug;

	private:
		//The currently running scene
		Scene* scene;

		//The scene to switch to at the end of the frame
		Scene* switchTo;

		//Whether the game should switch scenes at the end of the frame
		bool switchScenes;
	};
}