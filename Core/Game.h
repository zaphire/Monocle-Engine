#pragma once

#include "Monocle.h"
#include "Input.h"
#include "Graphics.h"
#include "Debug.h"
#include "Scene.h"
#include "Assets.h"
#include "Tween.h"
#include "Collision.h"
#include "Random.h"
#include "Audio/Audio.h"
#include "Level.h"

namespace Monocle
{

	//! Base class for creating a new game. Manages the main loop, timer and high-level updating, rendering.
	//!	Some games will be able to get away with just instantiating this class and adding Scenes.
	//! Some developers may want to extend Game and customize it, instead.
	class Game
	{
	public:
		//! Initializes all the default sub-systems. Platform, Input, Graphics, Debug, Assets, Tween, Collision, Random, Audio, Level
		Game(const std::string &name="MonoclePowered.org", int w=1024, int h=768, int bits=24, bool fullscreen=false);

		//! Runs the main game loop. Handles timing and high-level updating, rendering.
		void Main();

		//! Updates the game state.  Derived classes may override this to provide extra functionality that must be
		//! executed in each game loop iteration.
		virtual void Update();

		//! Sets the current Scene. Games can run one Scene at a time.
		static void SetScene(Scene* scene);
		//! Returns a pointer to the currently running Scene.
		static Scene* GetScene();

		//! Call Game::Quit to quit the main loop. (exit your game)
		static void Quit();

		//! Receives a string-based message, referred to as a "note." Can be overloaded.
		//virtual void ReceiveNote(const std::string &note);
		
		//virtual void Init();
        static float frames_per_sec;

	private:
		static Game *instance;

		//! Notifies the Game if it needs to exit sanely at the next opportunity
		bool isDone;

		//! The scene that is currently running
		Scene* scene;

		//! Scene to switch to at the end of the frame; if NULL, no switch
		Scene* switchTo;

		Platform platform;
		Input input;
		Graphics graphics;
		Debug debug;
		Assets assets;
		Tween tween;
		Collision collision;
		Random random;
		Audio audio;
		Level level;
	};
}
