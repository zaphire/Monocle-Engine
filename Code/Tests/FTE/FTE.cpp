#include "FTE.h"

namespace FTE
{

	Player::Player()
		: Entity(), force(200.0f)
	{
		SetGraphic(new Sprite("graphics/player.png"));
		scale = Vector2::one * 0.5f;
	}

	void Player::Update()
	{
		Entity::Update();

		if (Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT))
		{
			Vector2 dir = Input::GetWorldMousePosition() - position;
			dir.Normalize();
			velocity += force * dir * Monocle::deltaTime;
		}

		position += velocity * Monocle::deltaTime;

		Graphics::SetCameraPosition(position);
	}


	LevelScene *levelScene = NULL;

	void LevelScene::Begin()
	{
		levelScene = this;

		Scene::Begin();

		// set the base content path (used by everything)
		Assets::SetContentPath(Assets::GetContentPath()+"FTE/");

		Graphics::SetBackgroundColor(Color::blue*0.1f + Color::black*0.9f);

		// tell the FTE what scene it is working in
		fringeTileEditor.Init(this);
		// enable it
		fringeTileEditor.Enable();
		// pause this scene's updating (freeze the game)
		isPaused = true;

		// load project file that defines our tilesets
		Level::LoadProject("project.xml");
		// load the actual level
		Level::Load("level.xml", this);

		// create the player
		Player *player = new Player();
		player->position = Graphics::screenCenter;
		Add(player);
	}

	void LevelScene::Update()
	{
		Scene::Update();

		// update the FTE
		fringeTileEditor.Update();

		if (Input::IsKeyPressed(KEY_S) && Input::IsKeyHeld(KEY_LCTRL))
		{
			Level::Save();
		}

		// if we hit tab...
		if (Input::IsKeyPressed(KEY_TAB))
		{
			// if we're not doing anything in the FTE...
			if (fringeTileEditor.GetState() == FTES_NONE)
			{
				// toggle pause state
				isPaused = !isPaused;

				if (isPaused)
					fringeTileEditor.Enable();
				else
					fringeTileEditor.Disable();
			}
		}
	}

	void LevelScene::End()
	{
		Scene::End();
	}
}