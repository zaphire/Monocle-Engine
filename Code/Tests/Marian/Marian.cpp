#include "Marian.h"

namespace Marian
{
	Sprite::Sprite()
		: Entity(), texture(NULL)
	{
	}

	void Sprite::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::Alpha();
		Graphics::BindTexture(texture);
		Graphics::RenderQuad(5.0f);
		Graphics::PopMatrix();
	}

	void TitleScene::Begin()
	{
		Scene::Begin();

		Graphics::SetCameraPosition(Vector3(0,0,-6));

		Sprite *sprite = new Sprite();
		sprite->texture = AssetDatabase::RequestTexture("../../../../../Content/Marian/Title.png");
		Add(sprite);
	}

	void TitleScene::End()
	{
		Scene::End();
		

	}

}