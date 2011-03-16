#include "Pong.h"
#include "../../Input.h"
#include "../../Collision.h"

namespace Pong
{
	/*
	**************************************************
	* B a l l
	**************************************************
	*
	* Moves via velocity Vector2
	* Collides with entities tagged with "Paddle"
	* Sends a message to the scene when it goes offside
	* 
	*/

	Ball::Ball() : Entity(), texture(NULL)
	{
		AddTag("Ball");
		AddRectangleCollider(25.0f, 25.0f);
		velocity = Vector2::right * 200.0f;
	}

	void Ball::Update()
	{
		// store our last position
		Vector2 lastPosition = position;

		// update our current position using velocity
		position += velocity * Monocle::deltaTime;

		// check collisions against the paddles
		Collider* collider = Collide("Paddle");
		if (collider)
		{
			Debug::Log("Ball hit an entity tagged with 'Paddle'");
			position = lastPosition;
			
			Vector2 diff = position - collider->GetEntity()->position;
			diff.Normalize();
			diff *= velocity.GetMagnitude();
			velocity = diff;
		}

		// if we hit the top or bottom of the screen
		if (position.y < 0 || position.y > 600)
		{
			position = lastPosition;
			velocity.y *= -1;
		}
		
		// if we go off the left side of the screen
		if (position.x < 0)
		{
			SendMessageToScene("BallOffLeft");
		}

		// if we go off the right side of the screen
		if (position.x > 800)
		{
			SendMessageToScene("BallOffRight");
		}
	}

	void Ball::Render()
	{
		Graphics::BindTexture(texture);
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::RenderQuad(25.0f, 25.0f);
		Graphics::PopMatrix();
	}

	
	/*
	**************************************************
	* P a d d le
	**************************************************
	*
	* Moves up and down, controlled by user input
	* Is tagged with "Paddle"
	* 
	*/

	Paddle::Paddle()
		: Entity(), speed(0.0f)
	{
		AddTag("Paddle");
		AddRectangleCollider(25.0f, 100.0f);
	}

	void Paddle::Update()
	{
		const float accel = 1000.0f;
		const float maxSpeed = 300.0f;
		const float friction = 500.0f;
		const float maxY = 600.0f;
		if (Input::IsKeyHeld(keyUp))
		{
			speed += accel * Monocle::deltaTime;
			if (speed > maxSpeed) speed = maxSpeed;
		}
		else if (Input::IsKeyHeld(keyDown))
		{
			speed -= accel * Monocle::deltaTime;
			if (speed < -maxSpeed) speed = -maxSpeed;
		}
		else
		{
			if (speed > 0.0f)
			{
				speed -= friction * Monocle::deltaTime;
			}
			else if (speed < 0.0f)
			{
				speed += friction * Monocle::deltaTime;
			}

			if (abs(speed) < 0.05f)
			{
				speed = 0.0f;
			}
		}
		position += Vector2::up * Monocle::deltaTime * speed;
		
		if (position.y > maxY)
		{
			position.y = maxY;
			speed = -1.0f;
		}
		else if (position.y < 0)
		{
			position.y = 0;
			speed = 1.0f;
		}
	}

	void Paddle::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::RenderQuad(25, 100.0f);
		Graphics::PopMatrix();
	}


	/*
	**************************************************
	* G a m e S c e n e
	**************************************************
	*
	* Sets up the game's entities in Begin()
	* by creating a ball and two paddles
	* 
	* Handles messages sent to it indicating when the
	* ball has gone offside.
	* 
	*/

	void GameScene::Begin()
	{
		Debug::Log("Pong::GameScene::Begin()!");

		Scene::Begin();
		
		ball = new Ball();
		ball->position = Vector2(400, 300);
		Add(ball);

		paddle1 = new Paddle();
		paddle1->position = Vector2(100, 300);
		paddle1->keyUp = KEY_W;
		paddle1->keyDown = KEY_S;
		Add(paddle1);

		paddle2 = new Paddle();
		paddle2->position = Vector2(700, 300);
		paddle2->keyUp = KEY_UP;
		paddle2->keyDown = KEY_DOWN;
		Add(paddle2);
	}

	void GameScene::ReceiveMessage(const std::string &message)
	{
		if (message == "BallOffLeft")
		{
			ResetBall();
		}
		else if (message == "BallOffRight")
		{
			ResetBall();
		}
	}

	void GameScene::ResetBall()
	{
		ball->position = Vector2(400,300);
		ball->velocity = Vector2::Random() * 200.0f;
	}

	/*
	void GameScene::Update()
	{
		Scene::Update();

		// do pong specific update
	}
	*/

	void GameScene::End()
	{
		Scene::End();

		delete ball;
		delete paddle1;
		delete paddle2;
	}
}