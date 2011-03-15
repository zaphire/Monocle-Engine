#include "Marian.h"


namespace Marian
{
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
		Graphics::RenderQuad(texture->width * scale, texture->height * scale);
		Graphics::PopMatrix();
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

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

	void TitleScene::End()
	{
		Scene::End();
		

	}

}