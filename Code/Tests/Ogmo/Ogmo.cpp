#include "Ogmo.h"
#include "../../Input.h"
#include "../../Level.h"
#include "../../Monocle.h"

namespace Ogmo
{
	// T H E   P L A Y E R (entity)
	Player::Player(int x, int y) : Entity(),
		FRICTION_GROUND(0.90f), FRICTION_AIR(0.95f), GRAVITY(4.0f),
		JUMP(1.40f), ACCELERATION(10.0f), doubleJump(false), cling(0),
		tag("PLAYER"), onGround(false)
	{
		position = Vector2(x, y);

		AddTag(tag);
		SetCollider(new RectangleCollider(8, 8));

		sprite = new Sprite("player.png", FILTER_NONE, 8, 8);
		SetGraphic(sprite);
	}

	void Player::Update()
	{
		//input
		if (Input::IsKeyMaskHeld("left"))
		{
			if(cling < 0) { velocity.x -= ACCELERATION * Monocle::deltaTime; }
		}
		else if (Input::IsKeyMaskHeld("right"))
		{
			if(cling < 0) { velocity.x += ACCELERATION * Monocle::deltaTime; }
		}

		if (Input::IsKeyMaskPressed("jump") && onGround)
		{
			//jump
			velocity.y = - JUMP;
		}
		else if (Input::IsKeyMaskPressed("jump") && cling > 0)
		{
			//jump
			velocity.y = - JUMP;
			velocity.x = clingDir * 2;
			cling = -1;
		}
		else if (Input::IsKeyMaskPressed("jump") && !doubleJump)
		{
			velocity.y = - JUMP;
			doubleJump = true;
		}


		// maxspeed
		// REMOVED - Friction acts as a maxspeed, in this case
		//if(abs(velocity.x) > 0.20) { velocity.x = Sign(velocity.x, 0.20); }
		
		// friction
		if(onGround) { velocity.x *= FRICTION_GROUND; }
		else { velocity.x *= FRICTION_AIR; }

		// gravity
		velocity.y += GRAVITY * Monocle::deltaTime;

		//move right/left
		position.x += velocity.x;

		//did we just hit a wall? Move back until we aren't
		bool col = false;
		while(Collide("WALL")) { position.x -= Sign(velocity.x, 0.1); col = true; }

		//so.. we hit a wall ...
		if(col) 
		{ 
			if(!onGround) 
			{
				// if we're not on the ground, we cling to the wall (SMB-style)
				// this way, if the user wants to preform a wall-jump, they don't
				// get moved away from the wall
				cling = 10;
				if(velocity.x > 0) {clingDir = -1; }
				else { clingDir = 1; }
			}

			//stop moving
			velocity.x = 0;
			
		}

		//move up/down
		position.y += velocity.y;

		//did we just hit a wall? Move back until we aren't any more
		col = false;
		while(Collide("WALL")) { position.y -= Sign(velocity.y, 0.1); col = true; }
		if(col) { velocity.y = 0; }

		//check for ground
		if(velocity.y != 0)
		{
			position.y += 1;
			if(Collide("WALL")) { onGround = true; doubleJump = false; }
			else { onGround = false; }
			position.y -= 1;
		}

		cling --;
	}

	float Player::Sign(float i, float to)
	{
		return i < 0 ? - to : (i > 0 ? to : 0);
	}

	// T H E   W A L L (entity)
	Wall::Wall(int x, int y) : Entity()
	{
		position = Vector2(x, y);
		AddTag("WALL");
		SetCollider(new RectangleCollider(8, 8));
	}

	void Wall::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::BindTexture(NULL);
		Graphics::RenderQuad(8, 8);
		Graphics::PopMatrix();
	}

	// T H E   W O R L D (scene)
	void World::Begin()
	{
		//set screen size
		Graphics::Set2D(160, 120);
		Graphics::SetCameraPosition(Vector2(80, 60));

		//assets
		Assets::SetContentPath("../../Content/Ogmo/");

		//controls
		Input::DefineMaskKey("jump", KEY_UP);
		Input::DefineMaskKey("jump", KEY_Z);
		Input::DefineMaskKey("jump", KEY_A);

		Input::DefineMaskKey("left", KEY_LEFT);
		Input::DefineMaskKey("right", KEY_RIGHT);

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
	}

	void World::End()
	{
		delete player;
	}
}