#include "Core.h"

//#define TEST_ENTITY
//#define SCENE_CODE_WRITTEN

//If you want to use fixed timestep, uncomment the following two lines:
#define FIXED_TIMESTEP
#define MILLISECONDS_PER_FRAME 1000/60

namespace Monocle
{
	Core::Core()
	{
		scene				= NULL;
		switchTo			= NULL;
		switchScenes		= false;
		Monocle::deltaTime	= 0.0;
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
		long lastTick = Platform::GetMilliseconds();
		long tick;

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

			tick = Platform::GetMilliseconds();
			Monocle::deltaTime = ((double)(tick - lastTick))/1000.0;

			// **** BEGIN UPDATE
#ifdef FIXED_TIMESTEP
			if ((tick - lastTick) >= MILLISECONDS_PER_FRAME)
			{
				if (scene != NULL)
					scene->Update();
				lastTick = tick;
			}
#else
			if (scene != NULL)
				scene->Update();
			lastTick = tick;
#endif
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