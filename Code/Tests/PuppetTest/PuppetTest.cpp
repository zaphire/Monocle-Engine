#include "PuppetTest.h"
#include <stdlib.h>

namespace PuppetTest
{
    Timeline::Timeline()
        : Entity(), currentAnimation(NULL)
    {
        
    }
    
    void Timeline::Render()
    {
        //Entity::Render();
        
        if (currentAnimation)
        {
            Graphics::PushMatrix();
            float lineHalfWidth = Graphics::GetVirtualWidth() * 0.9f * 0.5f;
            float y = Graphics::GetVirtualHeight() * 0.5f - 50;
            float p = currentAnimation->GetCurrentTime() / currentAnimation->GetDuration();
            Vector2 start = Graphics::GetScreenCenter() + Vector2(-lineHalfWidth, y);
            Vector2 end = Graphics::GetScreenCenter() + Vector2(lineHalfWidth, y);
            Graphics::BindTexture(NULL);
            Graphics::SetColor(Color::blue);
            Graphics::RenderLine(start, end);
            Vector2 playHead = (end - start) * p + start;
            float playHeadHalfHeight = 16;
            Graphics::RenderLine(playHead + Vector2::up * playHeadHalfHeight, playHead + Vector2::down * playHeadHalfHeight);
            Graphics::PopMatrix();
        }
    }
    
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
        
        keyBackwards = KEY_LEFT;
        keyForwards = KEY_RIGHT;

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
        
        timeline = new Timeline();
        Add(timeline);
	}
    
    void TestScene::Update()
    {
        Scene::Update();
        
        timeline->currentAnimation = puppetEntity->puppet.GetCurrentAnimation();
		
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
            else
            {
                if (puppetEntity->puppet.GetCurrentAnimation())
                {
                    if (Input::IsKeyPressed(keyBackwards))
                        puppetEntity->puppet.GetCurrentAnimation()->AdjustCurrentTime(-0.1f);
                    if (Input::IsKeyPressed(keyForwards))
                        puppetEntity->puppet.GetCurrentAnimation()->AdjustCurrentTime(0.1f);
                }
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
