#pragma once

#include "Monocle.h"
#include "Input.h"
#include "Graphics.h"
#include "Debug.h"
#include "Scene.h"
#include "AssetDatabase.h"
#include "Tween.h"

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

		static void Quit();

	private:
		static Core *instance;

		bool isDone;

		//The currently running scene
		Scene* scene;

		//The scene to switch to at the end of the frame; if NULL, no switch
		Scene* switchTo;

		//access these using the public static functions
		Platform platform;
		Input input;
		Graphics graphics;
		Debug debug;
		AssetDatabase assetDatabase;
		Tween tween;
	};
}