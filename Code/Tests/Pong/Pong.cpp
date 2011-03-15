#include "Pong.h"
#include "../../Input.h"

namespace Pong
{
	Ball::Ball() : Entity()
	{
		texture = (TextureAsset*)AssetDatabase::RequestAsset(AT_TEXTURE, "blah.png");
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
		: Entity("paddle")
	{

	}

	void Paddle::Update()
	{
		if (Input::IsKeyHeld(keyUp))
		{
			position += Vector2::up * Monocle::deltaTime;
		}
		if (Input::IsKeyHeld(keyDown))
		{
			position += Vector2::down * Monocle::deltaTime;
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