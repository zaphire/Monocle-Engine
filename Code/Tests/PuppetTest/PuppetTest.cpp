#include "PuppetTest.h"
#include <stdlib.h>

namespace PuppetTest
{
	PuppetEntity::PuppetEntity()
		: Entity()
	{
		puppet.Load("puppet.xml", this);
		//puppet.DebugSetup();
		puppet.Play("idle");
	}

	void PuppetEntity::Update()
	{
		Entity::Update();
		puppet.Update();
	}

	void TestScene::Begin()
	{
		Scene::Begin();

		keyTogglePause = KEY_TAB;

		keyMoveLeft = KEY_LEFT;
		keyMoveRight = KEY_RIGHT;
		keyMoveUp = KEY_UP;
		keyMoveDown = KEY_DOWN;

		keyRotateLeft = KEY_J;
		keyRotateRight = KEY_L;

		Graphics::Set2D(800, 600);
		Graphics::SetBackgroundColor(Color::white);
		GetCamera()->position = Graphics::GetScreenCenter();

		Assets::SetContentPath(Assets::GetContentPath() + "/PuppetTest/");

        puppetEntity = new PuppetEntity();
		puppetEntity->position = Graphics::GetScreenCenter() + Vector2::down * 100;
		puppetEntity->scale = Vector2::one * 0.75f;
		Add(puppetEntity);

		Entity *entity = new Entity();
		entity->SetLayer(5);
		entity->SetGraphic(new Sprite("graphics/logo.png"));
		entity->position = Graphics::GetScreenCenter() + Vector2::up * 138;
		Add(entity);


	}
    
    void TestScene::Update()
    {
        Scene::Update();
        

		
		if (Input::IsKeyPressed(keyTogglePause))
		{
			puppetEntity->puppet.TogglePause();
			Debug::render = Debug::showBounds = puppetEntity->puppet.IsPaused();
			if (!puppetEntity->puppet.IsPaused())
			{
				Debug::selectedEntity = NULL;
			}
		}

		if (puppetEntity->puppet.IsPaused())
		{
			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				Debug::selectedEntity = this->GetNearestEntityByControlPoint(Input::GetWorldMousePosition(), "", Debug::selectedEntity);

			if (Debug::selectedEntity)
			{
				const float moveSpeed = 10.0f;
				float moveAmount = Monocle::deltaTime * moveSpeed;
				const float rotateSpeed = 15.0f;
				float rotateAmount = Monocle::deltaTime * rotateSpeed;

				if (Input::IsKeyHeld(keyMoveLeft))
					Debug::selectedEntity->position += Vector2::left * moveAmount;
				if (Input::IsKeyHeld(keyMoveRight))
					Debug::selectedEntity->position += Vector2::right * moveAmount;
				if (Input::IsKeyHeld(keyMoveUp))
					Debug::selectedEntity->position += Vector2::up * moveAmount;
				if (Input::IsKeyHeld(keyMoveDown))
					Debug::selectedEntity->position += Vector2::down * moveAmount;

				if (Input::IsKeyHeld(keyRotateLeft))
					Debug::selectedEntity->rotation -= rotateAmount;
				if (Input::IsKeyHeld(keyRotateRight))
					Debug::selectedEntity->rotation += rotateAmount;
			}
		}
		else
		{
			if (Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT))
				puppetEntity->position = WEIGHTED_AVERAGE(puppetEntity->position, Input::GetWorldMousePosition(), 15);
		}
    }
    
	void TestScene::End()
	{
		Scene::End();
	}
}
