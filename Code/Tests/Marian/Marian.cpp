#include "Marian.h"


namespace Marian
{
	LevelEditor::LevelEditor()
		: isOn(false), selectedSprite(NULL)
	{
	}

	void LevelEditor::Update()
	{
		if (Input::IsKeyPressed(KEY_TAB))
		{
			isOn = !isOn;
			Debug::Log("Toggle LevelEditor:");
			Debug::Log(isOn);

			Sprite::showBounds = isOn;

			if (!isOn)
			{
				if (selectedSprite)
				{
					selectedSprite->isSelected = false;
					selectedSprite = NULL;
				}
			}
		}

		if (isOn)
		{
			UpdateOldSpriteMethod();
		}
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

		Assets::SetContentPath("../../Content/Marian/");

		Core::SetEditor(new LevelEditor());

		Sprite *sprite=NULL;
		Entity *bg = AddSpriteEntity(Vector2(400, 300), 20, "Graphics/ReverieSky.png", 1024, 512, 1.25f);
		Entity *title = AddSpriteEntity(Vector2(400, 300), -20, "Graphics/Title.png", 512, 512, 1.5f, &sprite);

		Tween::FromTo(&sprite->color.a, 0.0f, 1.0f, 8.0f, EASE_OUTSIN);
		Tween::FromTo(&title->position, Vector2(400, 1024), Vector2(400, 300), 4.0f, EASE_OUTSIN);
	}

	Entity* TitleScene::AddSpriteEntity(Vector2 pos, int layer, const char *filename, int width, int height, float scale, Sprite **spriteRef)
	{
		Entity *entity = new Entity();
		entity->position = pos;
		Sprite *sprite = new Sprite(filename, width, height);
		sprite->scale = scale;
		entity->SetGraphic(sprite);
		Add(entity);
		if (spriteRef != NULL)
		{
			*spriteRef = sprite;
		}
		return entity;
	}

	void TitleScene::Update()
	{
		Scene::Update();

		if (Input::IsKeyPressed(KEY_ESCAPE))
		{
			Core::Quit();
		}
	}

	void TitleScene::End()
	{
		Scene::End();
	}


	void LevelEditor::UpdateOldSpriteMethod()
	{
#ifdef MARIAN_EDITOR_OLDSPRITEMETHOD
		// select prev
		if (Input::IsKeyPressed(KEY_1))
		{
			selectedSpriteIndex--;
			if (selectedSpriteIndex < 0)
				selectedSpriteIndex = 0;

			if (selectedSprite)
				selectedSprite->isSelected = false;

			selectedSprite = (Sprite*)Core::GetScene()->GetEntity(selectedSpriteIndex);

			if (selectedSprite)
			{
				selectedSprite->isSelected = true;
			}
			else
			{
				selectedSpriteIndex = 0;
			}
		}

		// select next
		if (Input::IsKeyPressed(KEY_2))
		{
			selectedSpriteIndex ++;

			if (selectedSprite)
				selectedSprite->isSelected = false;

			selectedSprite = (Sprite*)Core::GetScene()->GetEntity(selectedSpriteIndex);

			if (selectedSprite)
			{
				selectedSprite->isSelected = true;
			}
			else
			{
				selectedSpriteIndex = 0;
			}
		}

		/*
		if (Input::IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
		{
			// need to get mouse position into world coordinates somehow
			Entity *entity = Core::GetScene()->GetEntityNearestTo(Input::GetMousePosition());
			if (entity)
			{
				if (selectedSprite)
					selectedSprite->isSelected = false;

				selectedSprite = ((Sprite*)entity);

				if (selectedSprite)
					selectedSprite->isSelected = true;
			}
		}
		*/

		// add new sprite
		if (Input::IsKeyPressed(KEY_SPACE))
		{
			if (selectedSprite)
				selectedSprite->isSelected = false;

			selectedSprite = new Sprite(Vector2(400, 300), 0, "../../../../Content/Marian/Title.png", 512, 512);
			Tween::FromTo(&selectedSprite->color.a, 0.0f, 1.0f, 0.5f, EASE_LINEAR);
			Core::GetScene()->Add(selectedSprite);

			if (selectedSprite)
				selectedSprite->isSelected = true;
		}

		if (Input::IsKeyPressed(KEY_BACKSPACE))
		{
			if (selectedSprite)
			{
				Core::GetScene()->Remove(selectedSprite);
				// hmm... i think Scene::Remove is gonna have to delete this guy for us
				// we're probably leaking memory right now :)
				//delete selectedSprite;
				selectedSprite = NULL;
				selectedSpriteIndex --;
			}
		}

		// manipulate existing sprite
		if (selectedSprite)
		{
			float moveSpeed = 100.0f;
			float scaleSpeed = 0.5f;

			if (Input::IsKeyHeld(KEY_RIGHT))
			{
				selectedSprite->position += Vector2::right * moveSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(KEY_LEFT))
			{
				selectedSprite->position += Vector2::left * moveSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(KEY_UP))
			{
				selectedSprite->position += Vector2::up * moveSpeed * Monocle::deltaTime;
			}
			if (Input::IsKeyHeld(KEY_DOWN))
			{
				selectedSprite->position += Vector2::down * moveSpeed * Monocle::deltaTime;
			}

			if (Input::IsKeyHeld(KEY_P))
			{
				selectedSprite->scale += scaleSpeed * Monocle::deltaTime;
			}

			if (Input::IsKeyHeld(KEY_O))
			{
				selectedSprite->scale += -scaleSpeed * Monocle::deltaTime;
			}

			if (Input::IsKeyPressed(KEY_0))
			{
				selectedSprite->position = Vector2::zero;
			}
		}
#endif
	}
}