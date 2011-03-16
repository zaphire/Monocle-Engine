#include "Sprite.h"
#include "TextureAsset.h"
#include "AssetDatabase.h"
#include "Graphics.h"

namespace Monocle
{
	bool Sprite::showBounds = false;

	Sprite::Sprite(Vector2 position, int layer, const char *filename, float width, float height)
		: Entity(), texture(NULL), isSelected(false), scale(1.0f), width(width), height(height), angle(0.0f)
	{
		this->position = position;
		SetLayer(layer);
		texture = AssetDatabase::RequestTexture(filename);
		if (texture != NULL)
		{
			if (width == -1 || height == -1)
			{
				width = texture->width;
				height = texture->height;
			}
		}
	}


	Sprite::Sprite()
		: Entity(), texture(NULL), scale(1.0f), isSelected(false), angle(0.0f), width(64), height(64)
	{}

	void Sprite::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position.x, position.y, depth);
		Graphics::Rotate(angle, 0, 0, 1);
		Graphics::Blend();
		Graphics::SetColor(color);
		Graphics::BindTexture(texture);
		if (texture != NULL)
			Graphics::RenderQuad(width * scale, height * scale);
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
				Graphics::RenderLineRect(position.x, position.y, width * scale, height * scale);
			}
		}
	}
}