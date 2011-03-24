#include "Marian.h"
#include "../../Macros.h"


namespace Marian
{
	TriggerLevel::TriggerLevel(const std::string &level)
		: Entity()
	{
		SetCollider(new RectangleCollider(32, 32));
	}

	void TriggerLevel::Update()
	{
		Entity::Update();

		if (Collide("Player"))
		{
			Debug::Log("hit trigger");
		}
	}

	void TriggerLevel::Render()
	{
		Graphics::Translate(position);
		Graphics::RenderLineRect(0, 0, 32, 32);
	}

	Obstruction::Obstruction(int w, int h)
		: Entity()
	{
		SetCollider(new RectangleCollider(w, h));
		SetGraphic(new Sprite("", w, h));
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

		Obstruction* obstruction = new Obstruction(2048, 32);
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
			if (fringeTileEditor.GetState() == FTES_NONE)
			{
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