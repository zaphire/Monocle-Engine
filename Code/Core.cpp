#include "Core.h"

//If you want to use fixed timestep, uncomment the following three lines:
#define FIXED_TIMESTEP
#define MILLISECONDS_PER_FRAME 1000/60
#define MAX_UPDATES_PER_RENDER 4

namespace Monocle
{
	Core *Core::instance = NULL;

	Core::Core()
		:  scene(NULL), switchTo(NULL), isDone(false), editor(NULL)
	{
		instance = this;
		Monocle::deltaTime	= 0.0f;
	}

	void Core::Init()
	{
		debug.Init();
		platform.Init();
		assets.Init();
		input.Init();
		graphics.Init();
		collision.Init();
		audio.Init();
	}

	void Core::Main()
	{
		long lastTick = Platform::GetMilliseconds();
		long tick;

		while (!isDone)
		{
			platform.Update();
			tween.Update();

			if (editor != NULL)
			{
				editor->Update();
			}

			//Switch scenes if necessary
			if (switchTo != NULL)
			{
				if (scene != NULL)
					scene->End();
				scene = switchTo;
				switchTo = NULL;
				if (scene != NULL)
					scene->Begin();
			}

			tick = Platform::GetMilliseconds();
			Monocle::deltaTime = ((double)(tick - lastTick))/1000.0;

#if defined(FIXED_TIMESTEP)
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

	void Core::Quit()
	{
		instance->isDone = true;
	}

	void Core::SetScene(Scene* scene)
	{
		instance->switchTo = scene;
	}

	Scene* Core::GetScene()
	{
		return instance->scene;
	}

	void Core::SetEditor(Editor *editor)
	{
		instance->editor = editor;
	}
}