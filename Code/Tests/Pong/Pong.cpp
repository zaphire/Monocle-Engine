#include "Pong.h"

namespace Pong
{

	void Ball::Update()
	{
		position.x += 0.01f;
		//position += velocity * Time::deltaTime;
	}

	void Ball::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::RenderQuad(0.25f);
		Graphics::PopMatrix();
	}

	void Paddle::Update()
	{
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
		Add(paddle1);

		paddle2 = new Paddle();
		paddle2->position = Vector2(2, 0);
		Add(paddle2);
	}

	void GameScene::Update()
	{
		Scene::Update();

		// do pong specific update
		/*
		//output current MS
		char buf[64];
		itoa(Platform::GetMilliseconds(), buf, 10);
		Debug::Log(buf);
		*/
	}

	void GameScene::End()
	{
		delete ball;
		delete paddle1;
		delete paddle2;
	}
}