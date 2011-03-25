#include "Marian.h"
#include "../../Macros.h"
#include <cmath>


namespace Marian
{
	const float maxThreadRange = 600;
	const float stopRange = 64;
	const float walkRange = 128;

	LevelScene *levelScene = NULL;

	Cursor::Cursor()
		: Entity()
	{
		SetGraphic(new Sprite("graphics/butterfly.png", 96, 96));
		SetLayer(-2);
		AddTag("Cursor");
	}

	void Cursor::Update()
	{
		Entity::Update();
		
		if (levelScene->player)
		{
			if (Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT) || Input::IsMouseButtonHeld(MOUSE_BUTTON_RIGHT)  || Input::IsKeyHeld(KEY_SPACE))
			{
				
				Vector2 dir = Input::GetWorldMousePosition() - levelScene->player->position;
				dir.Clamp(maxThreadRange);
				Vector2 wantPosition = levelScene->player->position + dir;
				position = wantPosition;

				if (!dir.IsInRange(stopRange) && !Input::IsMouseButtonHeld(MOUSE_BUTTON_RIGHT))
				{
					Vector2 ndir = dir;
					ndir.Normalize();
					ndir.y = 0;
					levelScene->player->Move(ndir, dir.IsInRange(walkRange));
				}
			}
			else //if (Input::IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || Input::IsKeyReleased(KEY_SPACE))
			{
				//Tween::To(&position, levelScene->player->position, 0.1f, EASE_LINEAR);
				position = levelScene->player->position;
			}
		}
	}

	void Cursor::Render()
	{
		Entity::Render();

		// draw string (temp)
		if (levelScene)
		{
			Graphics::BindTexture(NULL);
			Graphics::SetColor(Color(1, 1, 1, 0.5f));
			Graphics::RenderLine(levelScene->player->position, position);
		}
	}

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

		const float walkSpeed = 150;
		const float runSpeed = 220;
		const float maxFallSpeed = 50.0f;
		const float gravity = 980.0f;

		velocity.y += MIN(gravity * Monocle::deltaTime, maxFallSpeed);

		position += velocity * Monocle::deltaTime;

		float useSpeed = isWalking?walkSpeed:runSpeed;


		if (Collide("Obstruction"))
		{
			position.y = lastPosition.y;
			velocity.y = 0;
		}

		if (Input::IsKeyHeld(KEY_RIGHT))
		{
			scale.x = 1;
			position.x += useSpeed * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(KEY_LEFT))
		{
			scale.x = -1;
			position.x -= useSpeed * Monocle::deltaTime;
		}

		const float turnThresh = 0.25f;
		if (moveDir.x > turnThresh)
			scale.x = 1;
		else if (moveDir.x < -turnThresh)
			scale.x = -1;

		if (fabs(moveDir.x) < 0.5f)
		{
			moveDir.x = 0;
		}

		position += moveDir * useSpeed * Monocle::deltaTime;

		if (Collide("Obstruction"))
		{
			position.x = lastPosition.x;
			velocity.x = 0;
		}

		moveDir = Vector2::zero;

		Graphics::SetCameraPosition(position + Vector2::up * 32);
	}

	void Player::Move(Vector2 dir, bool isWalking)
	{
		this->moveDir = dir;
		this->isWalking = isWalking;
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

		Assets::SetContentPath("../../Content/Marian/");

		//Game::SetEditor(new LevelEditor());

		Sprite *sprite=NULL;
		Entity *bg = AddSpriteEntity(Vector2(400, 300), 20, "graphics/reverie-sky.png", 1024, 512, 1.25f);
		Entity *title = AddSpriteEntity(Vector2(400, 300), -20, "graphics/title.png", 512, 512, 1.5f, &sprite);

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
		levelScene = this;

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

		cursor = new Cursor();
		cursor->position = Graphics::screenCenter;
		Add(cursor);

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