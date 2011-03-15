#include "Pong.h"
#include "../../Input.h"

namespace Pong
{
	Ball::Ball() : Entity(), texture(NULL)
	{
	}

	void Ball::Update()
	{
		position += Vector2::right * Monocle::deltaTime;
	}

	void Ball::Render()
	{
		Graphics::BindTexture(texture);
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::RenderQuad(0.25f);
		Graphics::PopMatrix();
	}

	Paddle::Paddle()
		: Entity(), speed(0.0f)
	{
		AddTag("paddle");
	}

	void Paddle::Update()
	{
		const float accel = 8.0f;
		const float maxSpeed = 3.0f;
		const float friction = 4.0f;
		const float maxY = 2.0f;
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
		else if (position.y < -maxY)
		{
			position.y = -maxY;
			speed = 1.0f;
		}
	}

	void Paddle::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::RenderQuad(0.25f, 1.0f);
		Graphics::PopMatrix();
	}

	void GameScene::Begin()
	{
		Debug::Log("Pong::GameScene::Begin()!");

		Scene::Begin();

		// do pong specific init
		Graphics::SetCameraPosition(Vector3(0,0,-6));
		
		ball = new Ball();
		ball->position = Vector2(0, 0);
		Add(ball);

		paddle1 = new Paddle();
		paddle1->position = Vector2(-2, 0);
		paddle1->keyUp = KEY_W;
		paddle1->keyDown = KEY_S;
		Add(paddle1);

		paddle2 = new Paddle();
		paddle2->position = Vector2(2, 0);
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