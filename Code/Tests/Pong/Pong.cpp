#include "Pong.h"
#include "../../Input.h"

namespace Pong
{
	Ball::Ball() : Entity(), texture(NULL)
	{
	}

	void Ball::Update()
	{
		const float speed = 100.0f;
		position += Vector2::right * speed * Monocle::deltaTime;
	}

	void Ball::Render()
	{
		Graphics::BindTexture(texture);
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::RenderQuad(25.0f, 25.0f);
		Graphics::PopMatrix();
	}

	Paddle::Paddle()
		: Entity(), speed(0.0f)
	{
		AddTag("paddle");
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

	void GameScene::Update()
	{
		Scene::Update();

		// do pong specific update
	}

	void GameScene::End()
	{
		delete ball;
		delete paddle1;
		delete paddle2;
	}
}