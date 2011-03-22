#include "Sprite.h"
#include "../Assets.h"
#include "../Entity.h"

namespace Monocle
{
	bool Sprite::showBounds = false;
	Sprite *Sprite::selectedSprite = NULL;

	Sprite::Sprite(const std::string &filename, FilterType filter, float width, float height)
		: Graphic(),
		texture(NULL),
		width(width),
		height(height),
		textureOffset(Vector2::zero),
		textureScale(Vector2::one)
	{
		texture = Assets::RequestTexture(filename, filter);
		if (texture != NULL)
		{
			if (width == -1 || height == -1)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
		}
	}

	Sprite::Sprite(const std::string &filename, float width, float height)
		: Graphic(),
		texture(NULL),
		width(width),
		height(height),
		textureOffset(Vector2::zero),
		textureScale(Vector2::one)
	{
		texture = Assets::RequestTexture(filename);
		if (texture != NULL)
		{
			if (width == -1 || height == -1)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
		}
	}

	Sprite::Sprite()
		: Graphic(),
		texture(NULL),
		width(0),
		height(0),
		textureOffset(Vector2::zero),
		textureScale(Vector2::one)
	{
	}

	Sprite::~Sprite()
	{
		if (texture != NULL)
		{
			texture->RemoveReference();
			texture = NULL;
		}
	}

	/*
	Sprite::Sprite()
		: Entity(), texture(NULL), scale(1.0f), isSelected(false), angle(0.0f), width(64), height(64)
	{}
	*/

	/*
	void Sprite::Update()
	{
	}
	*/

	// store color info in entity?
	// that would bloat entity... hmm.
	// or make materials system...
	void Sprite::Render()
	{
		//Graphics::Rotate(angle, 0, 0, 1);
		Graphics::Translate(position.x, position.y, 0.0f);
		Graphics::Blend();
		//Graphics::SetColor(color);
		Graphics::BindTexture(texture);
		if (texture != NULL)
			Graphics::RenderQuad(width, height, textureOffset, textureScale);
		//Graphics::PopMatrix();


		// show bounds, for editor/selection purposes
		if (showBounds || selectedSprite == this)
		{
			if (texture != NULL)
			{
				if (selectedSprite == this)
					Graphics::SetColor(Color::red);
				else
					Graphics::SetColor(Color(1.0f,1.0f,1.0f,0.5f));

				Graphics::BindTexture(NULL);
				Graphics::RenderLineRect(0, 0, width, height);
			}
		}
	}
	
	void Sprite::GetWidthHeight(int *width, int *height)
	{
		if (texture)
		{
			*width = texture->width;
			*height = texture->height;
		}
	}
}