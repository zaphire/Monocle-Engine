#include "Marian.h"


namespace Marian
{
	LevelEditor::LevelEditor()
		: isOn(false), selectedEntity(NULL)
	{
	}

	void LevelEditor::Update()
	{
		if (Input::IsKeyPressed(KEY_TAB))
		{
			isOn = !isOn;
			Debug::Log("Toggle LevelEditor:");
			Debug::Log(isOn);
		}

		if (isOn)
		{
			// you can do stuff now, yay

			if (Input::IsKeyPressed(KEY_1))
			{
				selectedEntity = Core::GetScene()->GetEntity(0);
			}

			if (Input::IsKeyPressed(KEY_2))
			{
				selectedEntity = Core::GetScene()->GetEntity(1);
			}


			if (selectedEntity)
			{
				if (Input::IsKeyHeld(KEY_RIGHT))
				{
					selectedEntity->position += Vector2::right * Monocle::deltaTime;
				}
				if (Input::IsKeyHeld(KEY_LEFT))
				{
					selectedEntity->position += Vector2::left * Monocle::deltaTime;
				}
				if (Input::IsKeyHeld(KEY_UP))
				{
					selectedEntity->position += Vector2::up * Monocle::deltaTime;
				}
				if (Input::IsKeyHeld(KEY_DOWN))
				{
					selectedEntity->position += Vector2::down * Monocle::deltaTime;
				}
			}
		}
	}

	Sprite::Sprite()
		: Entity(), texture(NULL), scale(1.0f)
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
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

		Core::SetEditor(new LevelEditor());

		Graphics::SetCameraPosition(Vector3(0,0,-6));

		Sprite *sprite = NULL;
		
		sprite = new Sprite();
		sprite->depth = -5.0f;
		sprite->texture = AssetDatabase::RequestTexture("../../../../../Content/Marian/ReverieSky.png");
		sprite->scale = 0.01f;
		Add(sprite);

		sprite = new Sprite();
		sprite->texture = AssetDatabase::RequestTexture("../../../../../Content/Marian/Title.png");
		sprite->scale = 0.003f;
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