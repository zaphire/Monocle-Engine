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
            Vector2 playHead;
            float playHeadHalfHeight = 16;
            
            if (Debug::selectedEntity)
            {
                Graphics::SetColor(Color::orange);
                Part *part = dynamic_cast<Part*>(Debug::selectedEntity);
                if (part)
                {
                    PartKeyFrames *partKeyFrames = currentAnimation->GetPartKeyFrames(part);
                    if (partKeyFrames)
                    {
                        for (std::list<KeyFrame>::iterator i = partKeyFrames->keyFrames.begin(); i != partKeyFrames->keyFrames.end(); ++i)
                        {
                            playHead = (end-start) * ((*i).GetTime()/currentAnimation->GetDuration()) + start;
                            Graphics::RenderLine(playHead + Vector2::up * playHeadHalfHeight, playHead + Vector2::down * playHeadHalfHeight);
                        }
                    }
                }
            }

			playHead = (end - start) * p + start;

			Graphics::SetColor(Color::blue);
            Graphics::RenderLine(playHead + Vector2::up * playHeadHalfHeight * 0.5f, playHead + Vector2::down * playHeadHalfHeight * 0.5f);

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

		keyMoveLeft = KEY_A;
		keyMoveRight = KEY_D;
		keyMoveUp = KEY_W;
		keyMoveDown = KEY_S;

		keyRotateLeft = KEY_J;
		keyRotateRight = KEY_L;
        
        keyBackwards = KEY_LEFT;
        keyForwards = KEY_RIGHT;
        
        keySetKeyFrame = KEY_SPACE;

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

	const float TIME_STEP = 0.05f;
    
    void TestScene::Update()
    {
        Scene::Update();
        
        timeline->currentAnimation = puppetEntity->puppet.GetCurrentAnimation();
		
		Animation *anim = puppetEntity->puppet.GetCurrentAnimation();

		if (Input::IsKeyPressed(keyTogglePause))
		{
			puppetEntity->puppet.TogglePause();
			Debug::render = Debug::showBounds = puppetEntity->puppet.IsPaused();
			if (!puppetEntity->puppet.IsPaused())
			{
				Debug::selectedEntity = NULL;
			}
			else
			{
				if (anim)
				{
					anim->SetCurrentTime(int(anim->GetCurrentTime()/TIME_STEP)*TIME_STEP);
				}
			}
		}

		if (puppetEntity->puppet.IsPaused())
		{
			if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				Debug::selectedEntity = this->GetNearestEntityByControlPoint(Input::GetWorldMousePosition(), "", Debug::selectedEntity);

			if (Debug::selectedEntity)
			{
				float moveSpeed = 10.0f;
				float rotateSpeed = 15.0f;
				
				if (Input::IsKeyHeld(KEY_LSHIFT))
				{
					float multiplier = 5.0f;
					moveSpeed *= multiplier;
					rotateSpeed *= multiplier;
				}

				float moveAmount = Monocle::deltaTime * moveSpeed;
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

            }
            
            if (anim)
            {
                Part *part = dynamic_cast<Part*>(Debug::selectedEntity);
                
                if (Input::IsKeyPressed(keyBackwards))
                    anim->AdjustCurrentTime(-TIME_STEP);
                if (Input::IsKeyPressed(keyForwards))
                    anim->AdjustCurrentTime(TIME_STEP);
                
                if (Input::IsKeyPressed(keySetKeyFrame))
                {
                    anim->SetPartKeyFrame(part, KeyFrame(anim->GetCurrentTime(), *part));
                    //puppetEntity.puppet.GetCurrentAnimation()->AddNewPartKeyFrame();
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
