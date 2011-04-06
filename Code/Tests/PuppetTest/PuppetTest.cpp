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

		Graphics::Set2D(1024, 768);
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
        
        if (Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT))
            puppetEntity->position = WEIGHTED_AVERAGE(puppetEntity->position, Input::GetWorldMousePosition(), 15);
    }
    
	void TestScene::End()
	{
		Scene::End();
	}
}
