#include "Jumper.h"
#include "../../Input.h"

// by @NoelFB
namespace Jumper
{
	Player::Player(Vector2 pos) 
		: Sprite(pos, -10, "../../../../Content/Jumper/Player.png", 64, 64)
	{
		speed = 100.0f;
		gravity = 0.25f;
		velocity = Vector2(0.0f, 0.0f);
		jump = 8.0f;
		maxSpeed = 4.0f;
		leanAmount = 1.5f;

		AddTag("Player");
		AddRectangleCollider(40, 64);
	}

	void Player::Update()
	{
		// increase acceleration
		if (Input::IsKeyHeld(keyLeft))
		{
			if(velocity.x > -maxSpeed) { velocity.x -= speed * Monocle::deltaTime; }
		}
		else if (Input::IsKeyHeld(keyRight))
		{
			if(velocity.x < maxSpeed) { velocity.x += speed * Monocle::deltaTime; }
		}
		else
		{
			velocity.x = 0;
		}

		// lean
		angle = velocity.x * leanAmount;

		// jump
		if(Input::IsKeyHeld(keyUp) && onGround)
		{
			velocity.y = -jump;
			isJumping = true;
			onGround = false;

			// stretch a bit when jumping
			width = 64 * 1.0f;
			height = 64 * 1.1f;
			angle = 0;
		}

		// friction

		// gravity
		velocity.y += gravity;

		
		//move
		Vector2 lastPosition = position;
		float temp = 0.001f;

		position.x += velocity.x;
		if(Collide("Wall") || Collide("Player"))
		{
			position.x = lastPosition.x;
			velocity.x = 0.0f;
		}

		position.y += velocity.y;

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
				width = 64 * 1.1f;
				height = 64 * 1.0f;
				angle = 0;
			}

			isJumping = false;
		}
	}

	float Player::Sign(float i, float to)
	{
		return i < 0 ? - to : (i > 0 ? to : 0);
	}


	Wall::Wall(Vector2 pos, float w, float h)
		: Entity()
	{
		position = pos;
		AddTag("Wall");
		AddRectangleCollider(w, h);
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

	void GameScene::Begin()
	{
		Debug::Log("Jumper::GameScene::Begin()!");
		Scene::Begin();

		Graphics::SetBackgroundColor(Color::blue * 0.1f);

		Add(new Wall(Vector2(400.0f, 500.0), 200.0f, 10.0f));
		Add(new Wall(Vector2(600.0f, 400.0), 100.0f, 10.0f));
		Add(new Wall(Vector2(200.0f, 400.0), 20.0f, 150.0f));

		SpawnPlayer();
	}

	void GameScene::SpawnPlayer()
	{
		player = new Player(Vector2(400.0f, 300.0f));
		player->keyUp = KEY_UP;
		player->keyLeft = KEY_LEFT;
		player->keyRight = KEY_RIGHT;
		Add(player);
	}

	void GameScene::Update()
	{
		Scene::Update();

		if (Input::IsKeyPressed(KEY_SPACE))
		{
			SpawnPlayer();
		}
	}

	void GameScene::End()
	{
		delete player;
		//walls.clear();
	}
}