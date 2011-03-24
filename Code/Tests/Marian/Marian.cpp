#include "Marian.h"
#include "../../Macros.h"


namespace Marian
{
	/*
	LevelEditor::LevelEditor()
		: isOn(false), selectedSprite(NULL)
	{
	}

	void LevelEditor::Update()
	{
		if (Input::IsKeyPressed(KEY_TAB))
		{
			isOn = !isOn;
			Debug::Log("Toggle LevelEditor:");
			Debug::Log(isOn);

			Sprite::showBounds = isOn;

			if (!isOn)
			{
				if (selectedSprite)
				{
					selectedSprite = NULL;
				}
			}
		}

		if (isOn)
		{
			UpdateOldSpriteMethod();
		}
	}
	void LevelEditor::UpdateOldSpriteMethod()
	{
#ifdef MARIAN_EDITOR_OLDSPRITEMETHOD
		// select prev
		if (Input::IsKeyPressed(KEY_1))
		{
			selectedSpriteIndex--;
			if (selectedSpriteIndex < 0)
				selectedSpriteIndex = 0;

			if (selectedSprite)
				selectedSprite->isSelected = false;

			selectedSprite = (Sprite*)Game::GetScene()->GetEntity(selectedSpriteIndex);

			if (selectedSprite)
			{
				selectedSprite->isSelected = true;
			}
			else
			{
				selectedSpriteIndex = 0;
			}
		}

		// select next
		if (Input::IsKeyPressed(KEY_2))
		{
			selectedSpriteIndex ++;

			if (selectedSprite)
				selectedSprite->isSelected = false;

			selectedSprite = (Sprite*)Game::GetScene()->GetEntity(selectedSpriteIndex);

			if (selectedSprite)
			{
				selectedSprite->isSelected = true;
			}
			else
			{
				selectedSpriteIndex = 0;
			}
		}

		// add new sprite
		if (Input::IsKeyPressed(KEY_SPACE))
		{
			if (selectedSprite)
				selectedSprite->isSelected = false;

			selectedSprite = new Sprite(Vector2(400, 300), 0, "../../../../Content/Marian/Title.png", 512, 512);
			Tween::FromTo(&selectedSprite->color.a, 0.0f, 1.0f, 0.5f, EASE_LINEAR);
			Game::GetScene()->Add(selectedSprite);

			if (selectedSprite)
				selectedSprite->isSelected = true;
		}

		if (Input::IsKeyPressed(KEY_BACKSPACE))
		{
			if (selectedSprite)
			{
				Game::GetScene()->Remove(selectedSprite);
				// hmm... i think Scene::Remove is gonna have to delete this guy for us
				// we're probably leaking memory right now :)
				//delete selectedSprite;
				selectedSprite = NULL;
				selectedSpriteIndex --;
			}
		}

		// manipulate existing sprite
		if (selectedSprite)
		{
			float moveSpeed = 100.0f;
			float scaleSpeed = 0.5f;

			if (Input::IsKeyHeld(KEY_RIGHT))
			{
				selectedSprite->position += Vector2::right * moveSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(KEY_LEFT))
			{
				selectedSprite->position += Vector2::left * moveSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(KEY_UP))
			{
				selectedSprite->position += Vector2::up * moveSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(KEY_DOWN))
			{
				selectedSprite->position += Vector2::down * moveSpeed * Monocle::deltaTime;
			}

			if (Input::IsKeyHeld(KEY_P))
			{
				selectedSprite->scale += scaleSpeed * Monocle::deltaTime;
			}

			if (Input::IsKeyHeld(KEY_O))
			{
				selectedSprite->scale += -scaleSpeed * Monocle::deltaTime;
			}

			if (Input::IsKeyPressed(KEY_0))
			{
				selectedSprite->position = Vector2::zero;
			}
		}
#endif
	}
	*/

	Obstruction::Obstruction()
		: Entity()
	{
		SetCollider(new RectangleCollider(256, 32));
		SetGraphic(new Sprite("", 256, 32));
		AddTag("Obstruction");
	}

	Player::Player()
		: Entity()
	{
		SetLayer(0);

		SetGraphic(new Sprite("graphics/marian.png", 128, 128));
		SetCollider(new CircleCollider(32, Vector2::down*32));
		AddTag("Player");

	}

	void Player::Update()
	{
		Entity::Update();

		Vector2 lastPosition = position;

		const float walkSpeed = 200;
		const float maxFallSpeed = 50.0f;
		const float gravity = 980.0f;

		velocity.y += MIN(gravity * Monocle::deltaTime, maxFallSpeed);

		position += velocity * Monocle::deltaTime;

		if (Collide("Obstruction"))
		{
			position.y = lastPosition.y;
			velocity.y = 0;
		}

		if (Input::IsKeyHeld(KEY_RIGHT))
		{
			scale.x = 1;
			position.x += walkSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_LEFT))
		{
			scale.x = -1;
			position.x -= walkSpeed * Monocle::deltaTime;
		}
		if (Collide("Obstruction"))
		{
			position.x = lastPosition.x;
			velocity.x = 0;
		}
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

		Assets::SetContentPath("../../Content/Marian/");

		//Game::SetEditor(new LevelEditor());

		Sprite *sprite=NULL;
		Entity *bg = AddSpriteEntity(Vector2(400, 300), 20, "Graphics/ReverieSky.png", 1024, 512, 1.25f);
		Entity *title = AddSpriteEntity(Vector2(400, 300), -20, "Graphics/Title.png", 512, 512, 1.5f, &sprite);

		Tween::FromTo(&title->color.a, 0.0f, 1.0f, 8.0f, EASE_OUTSIN);
		Tween::FromTo(&title->position, Vector2(400, 1024), Vector2(400, 300), 4.0f, EASE_OUTSIN);
	}

	Entity* TitleScene::AddSpriteEntity(Vector2 pos, int layer, const char *filename, int width, int height, float scale, Sprite **spriteRef)
	{
		Entity *entity = new Entity();
		entity->position = pos;

		Sprite *sprite = new Sprite(filename, width, height);
		entity->SetGraphic(sprite);

		Add(entity);

		if (spriteRef != NULL)
		{
			*spriteRef = sprite;
		}

		return entity;
	}

	void TitleScene::Update()
	{
		Scene::Update();

		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			Game::Quit();
		}
	}

	void TitleScene::End()
	{
		Scene::End();
	}


	class FringeTileEditor
	{
	public:
	};


	void LevelScene::Begin()
	{
		Scene::Begin();

		Assets::SetContentPath("../../Content/Marian/");

		fringeTileEditor.Init(this);
		fringeTileEditor.Enable();

		Level::LoadProject("project.xml");
		Level::Load("tower.xml", this);

		player = new Player();
		player->position = Graphics::screenCenter;
		Add(player);

		Obstruction* obstruction = new Obstruction();
		obstruction->position = Graphics::screenCenter + Vector2::down * 128;
		Add(obstruction);

		isPaused = true;
	}

	void LevelScene::Update()
	{
		Scene::Update();
		fringeTileEditor.Update();

		if (Input::IsKeyPressed(KEY_S) && Input::IsKeyHeld(KEY_LCTRL))
		{
			Level::Save();
		}
		if (Input::IsKeyPressed(KEY_TAB))
		{
			isPaused = !isPaused;

			if (isPaused)
				fringeTileEditor.Enable();
			else
				fringeTileEditor.Disable();
		}
	}

	void LevelScene::End()
	{
		Scene::End();
	}

}