#include "FTE.h"

namespace FTE
{
	LevelScene *levelScene = NULL;

	void LevelScene::Begin()
	{
		levelScene = this;

		Scene::Begin();

		// set the base content path (used by everything)
		Assets::SetContentPath("../../Content/FTE/");

		// tell the FTE what scene it is working in
		fringeTileEditor.Init(this);
		// enable it
		fringeTileEditor.Enable();
		// pause this scene's updating (freeze the game)
		isPaused = true;

		// load project file that defines our tilesets
		Level::LoadProject("project.xml");
		// load the actual level
		Level::Load("level.xml", this);
	}

	void LevelScene::Update()
	{
		Scene::Update();

		// update the FTE
		fringeTileEditor.Update();

		if (Input::IsKeyPressed(KEY_S) && Input::IsKeyHeld(KEY_LCTRL))
		{
			Level::Save();
		}

		// if we hit tab...
		if (Input::IsKeyPressed(KEY_TAB))
		{
			// if we're not doing anything in the FTE...
			if (fringeTileEditor.GetState() == FTES_NONE)
			{
				// toggle pause state
				isPaused = !isPaused;

				if (isPaused)
					fringeTileEditor.Enable();
				else
					fringeTileEditor.Disable();
			}
		}
	}

	void LevelScene::End()
	{
		Scene::End();
	}
}