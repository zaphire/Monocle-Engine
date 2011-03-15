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

	void PongGame::Init()
	{
		Core::Init();

		// do pong specific init
		graphics.SetCameraPosition(Vector3(0,0,-6));

		ball = new Ball();
		ball->position = Vector2(0, 0);
		scene.Add(ball);

		paddle1 = new Paddle();
		paddle1->position = Vector2(-2, 0);
		scene.Add(paddle1);

		paddle2 = new Paddle();
		paddle2->position = Vector2(2, 0);
		scene.Add(paddle2);
	}

	void PongGame::Update()
	{
		Core::Update();

		// do pong specific update

	}

}