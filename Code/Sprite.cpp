#include "Sprite.h"
#include "TextureAsset.h"
#include "AssetDatabase.h"
#include "Graphics.h"

namespace Monocle
{
	bool Sprite::showBounds = false;

	Sprite::Sprite(int layer, const char *filename, Vector2 position, float scale)
		: Entity(), texture(NULL), scale(scale), isSelected(false)
	{
		this->position = position;
		SetLayer(layer);
		texture = AssetDatabase::RequestTexture(filename);
	}


	Sprite::Sprite()
		: Entity(), texture(NULL), scale(1.0f), isSelected(false)
	{}

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

		if (showBounds || isSelected)
		{
			if (texture != NULL)
			{
				if (isSelected)
					Graphics::SetColor(Color::red);
				else
					Graphics::SetColor(Color(0.5f,0.5f,0.5f,0.25f));
				Graphics::BindTexture(NULL);
				Graphics::RenderLineRect(position.x, position.y, texture->width * scale, texture->height * scale);
			}
		}
	}
}