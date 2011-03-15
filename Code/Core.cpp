#include "Core.h"

//#define TEST_ENTITY
//#define SCENE_CODE_WRITTEN

namespace Monocle
{
	Core::Core()
	{
		scene			= NULL;
		switchTo			= NULL;
		switchScenes		= false;
	}

	void Core::Init()
	{
		platform.Init();
		debug.Init();
		input.Init();
		graphics.Init();
	}

	void Core::Main()
	{
		bool isDone = false;
		while (!isDone)
		{
			//Switch scenes if necessary
			if (switchScenes)
			{
				switchScenes = false;
				if (scene != NULL)
					scene->End();
				scene = switchTo;
				switchTo = NULL;
				if (scene != NULL)
					scene->Begin();
			}

			//Debug::Log("Core::Main...");
			Update();

			// **** BEGIN UPDATE
			if (scene != NULL)
				scene->Update();
			// **** END UPDATE

			// **** BEGIN RENDER
			graphics.BeginFrame();
			
			if (scene != NULL)
				scene->Render();
			
			graphics.EndFrame();

			graphics.ShowBuffer();
			// **** END RENDER
		}
	}

	void Core::Update()
	{
	}

	void Core::SetScene(Scene* scene)
	{
		switchTo = scene;
		switchScenes = true;
	}

	Scene* Core::GetScene()
	{
		return scene;
	}
}