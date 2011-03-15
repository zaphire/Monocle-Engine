#pragma once

#include "Monocle.h"
#include "Input.h"
#include "Graphics.h"
#include "Debug.h"
#include "Scene.h"
#include "AssetDatabase.h"

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
		AssetDatabase assetDatabase;

		static void Quit();

	private:
		static Core *instance;

		bool isDone;
		//The currently running scene
		Scene* scene;

		//The scene to switch to at the end of the frame
		Scene* switchTo;

		//Whether the game should switch scenes at the end of the frame
		bool switchScenes;


	};
}