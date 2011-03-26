#include "Game.h"

//If you want to use fixed timestep, uncomment the following three lines:
//#define FIXED_TIMESTEP
#define MILLISECONDS_PER_FRAME 1000/60
#define MAX_UPDATES_PER_RENDER 4
// prevent updates from getting too big on non-fixed time
#define MAX_DELTA_TIME 1.0/30.0

namespace Monocle
{
	Game *Game::instance = NULL;

	Game::Game()
		:  scene(NULL), switchTo(NULL), isDone(false)
		//, editor(NULL)
	{
		instance = this;
		Monocle::deltaTime	= 0.0f;
	}

	void Game::Init()
	{
		debug.Init();
		platform.Init();
		assets.Init();
		input.Init();
		graphics.Init();
		collision.Init();
		audio.Init();
		level.Init();
	}

	void Game::Main()
	{
		long lastTick = Platform::GetMilliseconds();
		long firstTick = Platform::GetMilliseconds();
		long tick;

		while (!isDone)
		{
			platform.Update();
			tween.Update();

			/*
			if (editor != NULL)
			{
				editor->Update();
			}
			*/

			//Switch scenes if necessary
			if (switchTo != NULL)
			{
				if (scene != NULL)
				{
					scene->End();
					scene->game = NULL;
					// delete scene?
				}

				scene = switchTo;
				
				switchTo = NULL;

				if (scene != NULL)
				{
					scene->game = this;
					scene->Begin();
				}
			}

			tick = Platform::GetMilliseconds();

#if defined(FIXED_TIMESTEP)
			int updates = 0;
			while (updates < MAX_UPDATES_PER_RENDER && (tick - lastTick) >= MILLISECONDS_PER_FRAME)
			{
				Monocle::deltaTime = MILLISECONDS_PER_FRAME/1000.0;
				Monocle::timeSinceStart += Monocle::deltaTime;
				//printf("ms: %f\n", Monocle::deltaTime);

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
			Monocle::deltaTime = MIN(((double)(tick - lastTick))/1000.0, MAX_DELTA_TIME);
			Monocle::timeSinceStart += Monocle::deltaTime;
			//printf("ms: %f\n", Monocle::deltaTime);

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

	void Game::Quit()
	{
		instance->isDone = true;
	}

	void Game::SetScene(Scene* scene)
	{
		instance->switchTo = scene;
	}

	Scene* Game::GetScene()
	{
		return instance->scene;
	}

	void Game::ReceiveNote(const std::string &note)
	{
	}

	/*
	void Game::SetEditor(Editor *editor)
	{
		instance->editor = editor;
	}
	*/
}
