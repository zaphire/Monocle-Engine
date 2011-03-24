#include "LevelEditor.h"
#include "../Input.h"

namespace Monocle
{
	// O G M O S I S

	void LevelEditor::Begin()
	{
		Scene::Begin();

		//Level::Load("HappyLandLevel");
	}

	void LevelEditor::Update()
	{
		Scene::Update();

		if (Input::IsKeyPressed(KEY_S))
		{
			Debug::Log("save...");
			Level::Save();
		}
	}

	void LevelEditor::End()
	{
		Scene::End();
	}
}
