#include "Jumper.h"
#include <Input.h>
#include <Level.h>

namespace Jumper
{
	/// PLAYER
	Player::Player(Vector2 pos) 
		: Entity()
	{
		position = pos;
		SetLayer(-10);

		AddTag("Player");

		SetCollider(new RectangleCollider(40, 64));

		sprite = new Sprite("Graphics/Player.png", FILTER_NONE, 64, 64);
		SetGraphic(sprite);

		speed = 4000.0f;
		gravity = 2000.0f;
		velocity = Vector2(0.0f, 0.0f);
		jump = gravity * 0.4f;
		maxSpeed = 400.0f;
		leanAmount = 0.05f;
	}

	void Player::Update()
	{
		// increase acceleration
		if (Input::IsKeyMaskHeld("left"))
		{
			if(velocity.x > -maxSpeed) { velocity.x -= speed * Monocle::deltaTime; }
		}
		else if (Input::IsKeyMaskHeld("right"))
		{
			if(velocity.x < maxSpeed) { velocity.x += speed * Monocle::deltaTime; }
		}
		else
		{
			velocity.x = 0;
		}

		// lean
		rotation = velocity.x * leanAmount;

		// jump
		if(Input::IsKeyMaskHeld("jump") && onGround)
		{
			velocity.y = -jump;
			isJumping = true;
			onGround = false;

			// stretch a bit when jumping
			sprite->width = 64 * 1.0f;
			sprite->height = 64 * 1.1f;
			rotation = 0;
		}

		// friction

		// gravity
		velocity.y += gravity * Monocle::deltaTime;

		//move
		Vector2 lastPosition = position;
		float temp = 0.001f;

		position.x += velocity.x * Monocle::deltaTime;
		if(Collide("Wall") || Collide("Player"))
		{
			position.x = lastPosition.x;
			velocity.x = 0.0f;
		}

		position.y += velocity.y * Monocle::deltaTime;

		onGround = false;

		if (Collide("Wall") || Collide("Player"))
		{
			// small ground collision problem here if falling fast (warps back up too far)
			// could do a line intersection with the collider we hit
			// collider->IntersectsLine()
			// ^ if this gave us the intersection point, we could snap right to the ground instead

			position.y = lastPosition.y;
			velocity.y = 0;
			onGround = true;
			
			// get fat when we're landing
			if (isJumping)
			{
				sprite->width = 64 * 1.1f;
				sprite->height = 64 * 1.0f;
				rotation = 0;
			}

			isJumping = false;
		}
	}

	float Player::Sign(float i, float to)
	{
		return i < 0 ? - to : (i > 0 ? to : 0);
	}

	/// WALL
	Wall::Wall(Vector2 pos, float w, float h)
		: Entity()
	{
		position = pos;
		AddTag("Wall");
		SetCollider(new RectangleCollider(w, h));
		this->width = w;
		this->height = h;
	}

	void Wall::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::BindTexture(NULL);
		Graphics::RenderQuad(width, height);
		Graphics::PopMatrix();
	}

	/// GAME SCENE
	void GameScene::Begin()
	{
		Debug::Log("Jumper::GameScene::Begin()!");
		Scene::Begin();

		Input::DefineMaskKey("jump", KEY_UP);
		Input::DefineMaskKey("jump", KEY_Z);
		Input::DefineMaskKey("jump", KEY_A);

		Input::DefineMaskKey("left", KEY_LEFT);
		Input::DefineMaskKey("right", KEY_RIGHT);

		Graphics::SetBackgroundColor(Color::blue * 0.1f);

		Add(new Wall(Vector2(400.0f, 500.0), 200.0f, 10.0f));
		Add(new Wall(Vector2(600.0f, 400.0), 100.0f, 10.0f));
		Add(new Wall(Vector2(200.0f, 400.0), 20.0f, 150.0f));

		SpawnPlayer(Vector2(400, 300));
	}

	void GameScene::SpawnPlayer(Vector2 pos)
	{
		player = new Player(pos);
		Add(player);
	}

	void GameScene::Update()
	{
		Scene::Update();

		if (Input::IsKeyPressed(KEY_SPACE))
		{
			SpawnPlayer(Vector2(400.0f, 300.0f));
		}
        if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
			SpawnPlayer(Input::GetWorldMousePosition());
        }
	}

	void GameScene::End()
	{
		Level::End();
		delete player;
		//walls.clear();
	}
}
