#include "Sprite.h"
#include "../Assets.h"
#include "../Entity.h"

namespace Monocle
{

	Sprite::Sprite(const std::string &filename, FilterType filter, float width, float height)
		: Graphic(),
		texture(NULL),
		width(width),
		height(height),
		textureOffset(Vector2::zero),
		textureScale(Vector2::one),
		blend(BLEND_ALPHA)
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
		textureScale(Vector2::one),
		blend(BLEND_ALPHA)
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
		width(1.0f),
		height(1.0f),
		textureOffset(Vector2::zero),
		textureScale(Vector2::one),
		blend(BLEND_ALPHA)
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

	void Sprite::Update()
	{
	}

	// store color info in entity?
	// that would bloat entity... hmm.
	// or make materials system...
	void Sprite::Render(Entity *entity)
	{
		Graphics::PushMatrix();

			Graphics::Translate(position.x, position.y, 0.0f);
			Graphics::Rotate(rotation, 0, 0, 1);
			Graphics::BindTexture(texture);
			Graphics::SetBlend(blend);

			if (Debug::showBounds && entity->IsDebugLayer())
			{
				if (Debug::selectedEntity == entity)
				{
					Color copyColor = entity->color;
					copyColor.a *= 0.8f;
					Graphics::SetColor(copyColor);
				}
				else if (Debug::selectedEntity != NULL && Debug::selectedEntity->GetLayer() > entity->GetLayer())
				{
					Color copyColor = entity->color;
					copyColor.a *= 0.6f;
					Graphics::SetColor(copyColor);
				}
			}

			Graphics::RenderQuad(width, height, textureOffset, textureScale);

		Graphics::PopMatrix();


		// show bounds, for editor/selection purposes
		if ((Debug::showBounds || Debug::selectedEntity == entity) && entity->IsDebugLayer())
		{
			Graphics::PushMatrix();
			Graphics::Translate(position.x, position.y, 0.0f);

			if (Debug::selectedEntity == entity)
				Graphics::SetColor(Color::orange);
			else
				Graphics::SetColor(Color(0.9f,0.9f,1.0f,0.25f));

			Graphics::BindTexture(NULL);
			Graphics::RenderLineRect(0, 0, width, height);

			if (Debug::selectedEntity != entity)
			{
				Graphics::RenderLine(Vector2(-width*0.5f, -height*0.5f), Vector2(width*0.5f, height*0.5f));
				Graphics::RenderLine(Vector2(width*0.5f, -height*0.5f), Vector2(-width*0.5f, height*0.5f));
			}
			else
			{
				Graphics::SetColor(Color::orange - Color(0,0,0,0.5f));
				Graphics::RenderLine(Vector2(-width*0.5f, -height*0.5f), Vector2(width*0.5f, height*0.5f));
				Graphics::RenderLine(Vector2(width*0.5f, -height*0.5f), Vector2(-width*0.5f, height*0.5f));
			}

			Graphics::PopMatrix();
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