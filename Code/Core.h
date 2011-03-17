#pragma once

#include "Monocle.h"
#include "Input.h"
#include "Graphics.h"
#include "Debug.h"
#include "Scene.h"
#include "Assets.h"
#include "Tween.h"
#include "Editor.h"
#include "Collision.h"
#include "Random.h"
#include "Audio.h"

namespace Monocle
{
	class Core
	{
	public:
		Core();
		void Init();

		void Main();

		static void SetScene(Scene* scene);
		static Scene* GetScene();

		static void SetEditor(Editor *editor);

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
		Assets assets;
		Tween tween;
		Collision collision;
		Random random;
		Audio audio;

		Editor *editor;
	};
}