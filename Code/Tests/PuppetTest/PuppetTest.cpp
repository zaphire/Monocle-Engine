#include "PuppetTest.h"
#include <stdlib.h>

namespace PuppetTest
{
	PuppetEntity::PuppetEntity()
		: Entity()
	{
		puppet.Load("asdlkfjasdf", this);
	}

	void PuppetEntity::Update()
	{
		Entity::Update();
		puppet.Update();
	}

	void TestScene::Begin()
	{
		Scene::Begin();

		PuppetEntity *puppetEntity = new PuppetEntity();
		puppetEntity->position = Graphics::GetScreenCenter();
		Add(puppetEntity);
	}

	void TestScene::End()
	{
		Scene::End();
	}
}
