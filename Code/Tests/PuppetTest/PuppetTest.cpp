#include "PuppetTest.h"
#include <stdlib.h>

namespace PuppetTest
{
	/// TEST SCENE
	void TestScene::Begin()
	{
		Scene::Begin();

		puppetEditor.Init(this);
	}

	void TestScene::Update()
	{
		Scene::Update();

		puppetEditor.Update();
	}

	void TestScene::End()
	{
		Scene::End();
	}
}
