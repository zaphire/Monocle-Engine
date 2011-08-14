#include "PuppetTest.h"
#include <stdlib.h>

namespace PuppetTest
{
	/// TEST SCENE
	void TestScene::Begin()
	{
		Scene::Begin();
		
		Graphics::SetBackgroundColor(Color::white);

		Add(puppetEditor = new PuppetEditor());
		puppetEditor->Load("puppet.xml");
	}

	void TestScene::Update()
	{
		Scene::Update();
	}

	void TestScene::End()
	{
		puppetEditor = NULL;
		Scene::End();
	}
}
