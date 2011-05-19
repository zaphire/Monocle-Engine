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
    float Game::frames_per_sec = 0;

	Game::Game()
		:  scene(NULL), switchTo(NULL), isDone(false)
		//, editor(NULL)
	{
		instance = this;
        frames_per_sec = 0.0;
		Monocle::deltaTime	= 0.0f;

		debug.Init();
		platform.Init();
		assets.Init();
		input.Init();
		graphics.Init();
		collision.Init();
		audio.Init();
		level.Init();
	}

	//!
	//! If FIXED_TIMESTEP is defined, the Main loop will use fixed timestep timing. i.e. Monocle::deltaTime will be the same every frame.
	//!
	//! If FIXED_TIMESTEP is not defined, dynamic timing will be used. Monocle::deltaTime will be different values every frame.
	//! However, Monocle::deltaTime will never be allowed to rise above MAX_DELTA_TIME.
	//! This means that, when using dynamic timing, on computers running slower than 1/MAX_DELTA_TIME frames per second, the game will slow down - rather than becoming choppy or unstable.
	//!
	//! If a scene is set (using Game::SetScene), that Scene's Scene::Update and Scene::Render will be called.
	//!
	//! If no Scene is set, nothing will happen.
	//! 
	void Game::Main()
	{
		long lastTick = Platform::GetMilliseconds();
		long firstTick = Platform::GetMilliseconds();
		long tick;

		while (!isDone)
		{
			platform.Update();

			while (!platform.IsActive())
			{
                if (!audio.IsPaused())
                    audio.PauseAll();
                
                // Otherwise, if we're not supposed to care if it pauses, we'll call audio.Update();
                
				platform.Update();
			}
            
            if (audio.IsPaused())
                audio.ResumeAll();

            audio.Update();
			tween.Update();

			// update timer
			tick = Platform::GetMilliseconds();
            
            // Frames per second
            frames_per_sec = (frames_per_sec * 0.9) + ((tick-lastTick) * 0.1);

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
		}
	}

	//!
	//! Does not quit immediately. Sets an internal boolean and exits at the top of the Game::Main loop.
	//!
	void Game::Quit()
	{
		instance->isDone = true;
	}

	//!
	//! The new Scene won't be set immediately. It will be set at the end of the frame.
	//!
	void Game::SetScene(Scene* scene)
	{
		instance->switchTo = scene;
	}

	//!
	//! Will return NULL if no Scene has been set.
	//!
	Scene* Game::GetScene()
	{
		return instance->scene;
	}

	//void Game::ReceiveNote(const std::string &note)
	//{
	//}

	/*
	void Game::SetEditor(Editor *editor)
	{
		instance->editor = editor;
	}
	*/
}
