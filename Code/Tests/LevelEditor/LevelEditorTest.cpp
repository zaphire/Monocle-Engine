#include "LevelEditorTest.h"

namespace LevelEditorTest
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

		Scene::GetCamera()->position = position;
		//Graphics::SetCameraPosition(position);
	}


	LevelScene *levelScene = NULL;

	LevelScene::LevelScene()
		: Scene(), levelEditor(NULL), puppetEditor(NULL)
	{
	}
	
	void LevelScene::Begin()
	{
		// init instance pointer
		levelScene = this;
		
		// call the super
		Scene::Begin();

		// set the base content path (used by everything)
		Assets::SetContentPath(Assets::GetContentPath() + "/LevelEditorTest/");
		
		// set the background color to a dark blue (10% blue + 90% black)
		Graphics::SetBackgroundColor(Color::blue*0.1f + Color::black*0.9f);
		
		// create new LevelEditor
		levelEditor = new LevelEditor();
		// tell the levelEditor what scene it is working in
		levelEditor->Init(this);
		// enable it
		levelEditor->Enable();
		
		// create new PuppetEditor
		puppetEditor = new PuppetEditor();
		// tell the puppetEditor what scene it's working in 
		puppetEditor->Init(this);
		
		// pause this scene's updating (freeze the game)
		isPaused = true;

		// load project file that defines our tilesets
		Level::LoadProject("project.xml");
		
		// load the actual level
		Level::Load("level.xml", this);

		// create the player
		Player *player = new Player();
		player->position = Graphics::GetScreenCenter();
		Add(player);
	}

	void LevelScene::Update()
	{
		Scene::Update();

		// update the levelEditor
		levelEditor->Update();
		
		puppetEditor->Update();

		if (Input::IsKeyPressed(KEY_S) && Input::IsKeyHeld(KEY_LCTRL))
		{
			Level::Save();
		}

		// if we hit tab...
		if (Input::IsKeyPressed(KEY_TAB))
		{
			// if we're not doing anything in the levelEditor...
			if (levelEditor->GetState() == FTES_NONE)
			{
				// toggle pause state
				isPaused = !isPaused;

				if (isPaused)
					levelEditor->Enable();
				else
					levelEditor->Disable();
			}
		}
		
		// if we hit CTRL+A
		if (Input::IsKeyHeld(KEY_LCTRL) && Input::IsKeyPressed(KEY_A))
		{
			if (!isPaused) isPaused = true;
			
			levelEditor->Disable();
			puppetEditor->Enable();
		}
	}

	void LevelScene::End()
	{
		if (puppetEditor)
		{
			delete puppetEditor;
			puppetEditor = NULL;
		}
		if (levelEditor)
		{
			delete levelEditor;
			levelEditor = NULL;
		}
		Scene::End();
	}
}
