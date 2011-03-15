#include "Core.h"

//If you want to use fixed timestep, uncomment the following three lines:
//#define FIXED_TIMESTEP
#define MILLISECONDS_PER_FRAME 1000/60
#define MAX_UPDATES_PER_RENDER 4

namespace Monocle
{
	Core *Core::instance = NULL;

	Core::Core()
	{
		instance			= this;
		scene				= NULL;
		switchTo				= NULL;
		switchScenes			= false;
		Monocle::deltaTime	= 0.0;
		isDone				= false;
		
	}

	void Core::Init()
	{
		debug.Init();
		platform.Init();
		assetDatabase.Init();
		input.Init();
		graphics.Init();
	}

	void Core::Quit()
	{
		instance->isDone = true;
	}

	void Core::Main()
	{

		long lastTick = Platform::GetMilliseconds();
		long tick;

		while (!isDone)
		{
			platform.Update();
			tween.Update();

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

#ifdef FIXED_TIMESTEP
			int updates = 0;
			while (updates < MAX_UPDATES_PER_RENDER && (tick - lastTick) >= MILLISECONDS_PER_FRAME)
			{
				input.Update();	
				//Update
				if (scene != NULL)
					scene->Update();
				++updates;
				lastTick += MILLISECONDS_PER_FRAME;
			}

			//Auto catch-up if it is really slowing down
			if (updates == MAX_UPDATES_PER_RENDER)
				lastTick = tick;

			//Render
			graphics.BeginFrame();
			if (scene != NULL)
				scene->Render();
			graphics.EndFrame();
			graphics.ShowBuffer();
#else
			//Update
			input.Update();
			if (scene != NULL)
				scene->Update();

			//Render
			graphics.BeginFrame();
			if (scene != NULL)
				scene->Render();
			graphics.EndFrame();
			graphics.ShowBuffer();

			lastTick = tick;
#endif
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