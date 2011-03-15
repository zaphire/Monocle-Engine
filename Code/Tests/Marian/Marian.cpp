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

			// add new sprite
			if (Input::IsKeyPressed(KEY_SPACE))
			{
				if (selectedSprite)
					selectedSprite->isSelected = false;

				selectedSprite = new Sprite("../../../../../Content/Marian/Title.png", 0.003f);
				Tween::FromTo(&selectedSprite->color.a, 0.0f, 1.0f, 0.5f, EASE_LINEAR);
				Core::GetScene()->Add(selectedSprite);

				if (selectedSprite)
					selectedSprite->isSelected = true;
			}

			// manipulate existing sprite
			if (selectedSprite)
			{
				float moveSpeed = 1.0f;
				float scaleSpeed = 0.001f;

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
		}
	}

	Sprite::Sprite(const char *filename, float scale)
		: Entity(), texture(NULL), scale(scale), isSelected(false)
	{
		texture = AssetDatabase::RequestTexture(filename);
	}


	Sprite::Sprite()
		: Entity(), texture(NULL), scale(1.0f), isSelected(false)
	{
		
	}

	void Sprite::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position.x, position.y, depth);
		Graphics::Blend();
		Graphics::SetColor(color);
		Graphics::BindTexture(texture);
		if (texture != NULL)
			Graphics::RenderQuad(texture->width * scale, texture->height * scale);
		Graphics::PopMatrix();

		if (isSelected)
		{
			if (texture != NULL)
			{
				Graphics::SetColor(Color::white);
				Graphics::BindTexture(NULL);
				Graphics::RenderLineRect(position.x, position.y, texture->width * scale, texture->height * scale);
			}
		}
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

		Core::SetEditor(new LevelEditor());

		Graphics::SetCameraPosition(Vector3(0,0,-6));

		Sprite *sprite = NULL;
		
		sprite = new Sprite("../../../../../Content/Marian/ReverieSky.png", 0.01f);
		sprite->depth = -5.0f;
		Add(sprite);

		sprite = new Sprite("../../../../../Content/Marian/Title.png", 0.003f);
		Tween::FromTo(&sprite->color.a, 0.0f, 1.0f, 5.0f, EASE_OUTSIN);
		Add(sprite);
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

}