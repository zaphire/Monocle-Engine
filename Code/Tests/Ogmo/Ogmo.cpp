#include "Ogmo.h"
#include "../../Input.h"
#include "../../Level.h"
#include "../../Monocle.h"
#include "../../Macros.h"

#include <stdlib.h>

namespace Ogmo
{
	// T H E   P L A Y E R (entity)
	Player::Player(int x, int y) : Entity(), 
		FRICTION_GROUND(800),
		FRICTION_AIR(400),
		GRAVITY(300),
                JUMP(120.0f),
                MAXSPEED_GROUND(60.0f),
		MAXSPEED_AIR(100.0f),
		ACCELERATION(800),
                WALLJUMP(160.0f),
		doubleJump(false),
		cling(0),
		onGround(false)
	{
		position = Vector2(x, y);

		AddTag("PLAYER");
		SetCollider(new RectangleCollider(8, 8));

		sprite = new SpriteAnimation("player.png", FILTER_NONE, 8, 8);
		sprite->Add("stand", 0, 0, 0);
		sprite->Add("run", 0, 3, 12.0f);
		sprite->Add("jumpUp", 8, 8, 0);
		sprite->Add("jumpDown", 9, 9, 0);
		sprite->Play("run");

		SetLayer(-1);
		SetGraphic(sprite);

		direction = true;
	}

	void Player::Update()
	{
		// GRAB INPUT AND ACCELERATE
		if (Input::IsKeyMaskHeld("left"))
		{
			scale.x = -1;
			if(cling < 0)
			{
				velocity.x -= ACCELERATION * Monocle::deltaTime;
			}
			sprite->Play("run");
			direction = false;
			scale.x = -1;
		}
		else if (Input::IsKeyMaskHeld("right"))
		{
			scale.x = 1;
			if(cling < 0)
			{
				velocity.x += ACCELERATION * Monocle::deltaTime;
			}
			sprite->Play("run");
			direction = true;
			scale.x = 1;
		}
		else
		{
			sprite->Play("stand");

			// friction
			if(onGround)
			{
                                velocity.x = APPROACH(velocity.x, 0, FRICTION_GROUND * Monocle::deltaTime);
			}
			else
			{
                                velocity.x = APPROACH(velocity.x, 0, FRICTION_AIR * Monocle::deltaTime);
			}
		}

		// JUMP INPUT
                if (Input::IsKeyMaskPressed("jump") && (onGround || cling > 0 || !doubleJump))
		{
			// jump
                        velocity.y = - JUMP;

			if(!onGround)
			{
				// wall jump
				if(cling > 0)
				{
                                        velocity.x = clingDir * WALLJUMP;
					cling = -1;
				}

				// double jump
				if(cling < 0)
				{
					doubleJump = true;
				}
			}
		}

		// gravity
		velocity.y += GRAVITY * Monocle::deltaTime;

		// maxspeed
		int maxspeed = onGround ? MAXSPEED_GROUND : MAXSPEED_AIR;
                if(abs(velocity.x) > maxspeed) { velocity.x = SIGN(velocity.x, 1) * maxspeed; }
		// Motion
		Motion(velocity.x, position.x);
		Motion(velocity.y, position.y);

		//check for ground

		if(CollideAt("WALL", position.x, position.y + 1))
		{
			onGround = true;
			doubleJump = false;
		}
		else
		{
			onGround = false;

			//change sprite
			if(velocity.y < 0)
			{
				sprite->Play("jumpUp");
			}
			else
			{
				sprite->Play("jumpDown");
			}

			//check for wall jump
			if(CollideAt("WALL", position.x + 1, position.y) && Input::IsKeyMaskHeld("right")) 
			{ 
				cling = 10; 
				clingDir = -1; 
			}
			if(CollideAt("WALL", position.x - 1, position.y) && Input::IsKeyMaskHeld("left")) 
			{ 
				cling = 10; 
				clingDir = 1;
			}
		}

		//decrease how long we can cling to a wall for
		cling --;

		//and at the end of the day, are we dead?
		if(Collide("SPIKE"))
		{
			World::ResetCoins();
		}
	
	}

	bool Player::Motion(float &speed, float &to)
	{
		// move
		to += speed * Monocle::deltaTime;

		// collide
		bool col = false;
		while(Collide("WALL"))
		{
			to -= SIGN(speed, 0.1);
			col = true;
		}
		
		// stop motion on collision
		if(col)
		{
			speed = 0;
		}

		// return
		return col;
	}


	bool Player::CollideAt(const std::string &tag, float x, float y)
	{
		Vector2 pre = Vector2(position.x, position.y);
		bool collide = false;

		position.x = x;
		position.y = y;

		if(Collide(tag))
		{
			collide = true;
		}

		position = Vector2(pre.x, pre.y);
		return collide;
	}

	// T H E   W A L L (entity)
	Wall::Wall(int x, int y) : Entity()
	{
		position = Vector2(x, y);
		AddTag("WALL");
		SetCollider(new RectangleCollider(8, 8));
		SetLayer(5);
	}

	void Wall::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::BindTexture(NULL);
                Graphics::RenderQuad(8, 8);
		Graphics::PopMatrix();
	}


	// T H E   C O I N (entity)
	Coin::Coin(int x, int y, Sprite *sprite) : Entity(), collected(false), start(x, y), reset(false)
	{
		position = Vector2(x, y);
		SetLayer(-1);
		SetGraphic(sprite);

		AddTag("COIN");
		SetCollider(new RectangleCollider(8, 8));

		while(Collide("WALL"))
		{
			position = Vector2(((int) rand() % 160 / 8) * 8 + 4, ((int) rand() % 120 / 8) * 8 + 4);
			start = Vector2(position.x, position.y);
		}
	}

	void Coin::Update()
	{
		//we hit the player!
		if(Collide("PLAYER") && !reset)
		{
			collected = true;
		}

		if(collected && isVisible)
		{
			//scale out
			scale.x -= 0.1;
			scale.y -= 0.1;

			//follow player
			position.x -= (position.x - World::player->position.x) / 10;
			position.y -= (position.y - World::player->position.y) / 10;

			//we can't be seen no more
			if(scale.x < 0)
			{
				isVisible = false;
			}
		}
		
		if(reset)
		{
			//we can see you!
			isVisible = true;

			//scale in
			if(scale.x != 1)
			{
				scale.x += 0.1;
				scale.y += 0.1;
			}

			//slide to start position
			position.x -= (position.x - start.x) / 10;
			position.y -= (position.y - start.y) / 10;

			//hop to start position
			if(abs(position.x - start.x) < 0.2 && abs(position.y - start.y) < 0.2)
			{
				reset = false;
				position.x = start.x;
				position.y = start.y;
			}
		}
	}

	// T H E   S P I K E (entity)
	Spike::Spike(int x, int y, Sprite *sprite) : Entity()
	{
		position = Vector2(x, y);

		SetLayer(-1);
		SetGraphic(sprite);

		AddTag("SPIKE");
		SetCollider(new RectangleCollider(8, 8));

		if(position.x == -1 && position.y == -1)
		{
			position = Vector2(((int) rand() % 160 / 8) * 8 + 4, ((int) rand() % 120 / 8) * 8 + 4);
			while(Collide("WALL"))
			{
				position = Vector2(((int) rand() % 160 / 8) * 8 + 4, ((int) rand() % 120 / 8) * 8 + 4);
			}

			while(!Collide("WALL")) { position.y += 1; }
		}
	}

	// T H E   W O R L D (scene)
	Player *World::player;
	World *World::instance;

	void World::Begin()
	{
		instance = this;

		//set screen size
		Graphics::Set2D(160, 120);
		//Graphics::SetCameraPosition(Vector2(80, 60));
		GetCamera()->position = Vector2(80, 60);

		//assets
		Assets::SetContentPath(Assets::GetContentPath()+"/Ogmo/");

		atCoin = new Sprite("coin.png", FILTER_NONE, 8, 8);
		atSpike = new Sprite("spike.png", FILTER_NONE, 8, 8);

		//controls
		Input::DefineMaskKey("jump", KEY_UP);
		Input::DefineMaskKey("jump", KEY_Z);
		Input::DefineMaskKey("jump", KEY_A);

		Input::DefineMaskKey("left", KEY_LEFT);
		Input::DefineMaskKey("right", KEY_RIGHT);
		Input::DefineMaskKey("kill", KEY_K);

		//eventually this will load the level
		Level::SetScene(this);
		Level::LoadProject("project.xml");
		Level::Load("level01.xml");

		//add player
		player = new Player(120, 8);
		Add(player);

		//add walls
		for(int i = 0; i < 160; i += 8)
		{
			wall = new Wall(i + 4, 116);
			Add(wall);
		}
		for(int i = 0; i < 120; i += 8)
		{
			wall = new Wall(4, i + 4);
			Add(wall);
		}

		for(int i = 0; i < 80; i += 8)
		{
			wall = new Wall(60, i + 4);
			Add(wall);
		}

		for(int i = 0; i < 18; i += 8)
		{
			wall = new Wall(12 + i, 28);
			Add(wall);
		}

		//add a few random coins
		for(int i = 0; i < 10; i ++)
		{
			Coin *c = new Coin(((int) rand() % 160 / 8) * 8 + 4, ((int) rand() % 120 / 8) * 8 + 4, atCoin);
			coins.push_back(c);
			Add(c);
		}

		//add spikes
		for(int i = 0; i < 8; i ++)
		{
			Add(new Spike(-1, -1, atSpike));
		}
	}

	void World::Update()
	{
		Scene::Update();

		///TESTEDITOR
		if (Input::IsKeyPressed(KEY_S) && Input::IsKeyHeld(KEY_LCTRL))
		{
			Debug::Log("save level...");
			Level::Save();
		}

		/*
		if (Input::IsKeyPressed(KEY_F1))
		{
			// doesn't clean up properly yet
			Debug::Log("load level...");
			Level::Load();
		}
		*/
	}

	void World::ResetCoins()
	{
		
		for (std::list<Coin*>::iterator i = instance->coins.begin(); i != instance->coins.end(); ++i)
		{
			if((*i)->collected)
			{
				(*i)->collected = false;
				(*i)->position = Vector2(player->position.x, player->position.y);
				(*i)->reset = true;
			}
		}
		
	}

	void World::End()
	{
		delete player;
		delete wall;
	}
}
