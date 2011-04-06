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

		Assets::SetContentPath(Assets::GetContentPath() + "/PuppetTest/");

		PuppetEntity *puppetEntity = new PuppetEntity();
		puppetEntity->position = Graphics::GetScreenCenter();
		puppetEntity->scale = Vector2::one * 0.75f;
		Add(puppetEntity);
	}

	void TestScene::End()
	{
		Scene::End();
	}
}
