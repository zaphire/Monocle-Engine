#include "PuppetTest.h"
#include <stdlib.h>

namespace PuppetTest
{
	PuppetEntity::PuppetEntity()
		: Entity()
	{
		puppet.Load("puppet.xml", this);
		puppet.DebugSetup();
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
		Add(puppetEntity);
	}

	void TestScene::End()
	{
		Scene::End();
	}
}
