#include "Ogmosis.h"
#include "../Input.h"

namespace Monocle
{
	// O G M O S I S

	void Ogmosis::Begin()
	{
		Scene::Begin();

		//Level::Load("HappyLandLevel");
	}

	void Ogmosis::Update()
	{
		Scene::Update();

		if (Input::IsKeyPressed(KEY_S))
		{
			Debug::Log("save...");
			Level::Save();
		}
	}

	void Ogmosis::End()
	{
		Scene::End();
	}
}
