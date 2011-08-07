#include "Sprite.h"
#include "../Assets.h"
#include "../Entity.h"

namespace Monocle
{

	Sprite::Sprite(const std::string &filename, FilterType filter, float width, float height)
		: Graphic(),
		texture(NULL),
		shader(NULL),
		width(width),
		height(height),
		textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        originalTextureOffset(Vector2::zero),
        originalTextureScale(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
		blend(BLEND_ALPHA)
	{
		texture = Assets::RequestTexture(filename, filter);
		if (texture != NULL)
		{
			if (width == -1.0 || height == -1.0)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
		}
	}

	Sprite::Sprite(const std::string &filename, float width, float height)
		: Graphic(),
		texture(NULL),
		shader(NULL),
		width(width),
		height(height),
		textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        originalTextureOffset(Vector2::zero),
        originalTextureScale(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
		blend(BLEND_ALPHA)
	{
		texture = Assets::RequestTexture(filename);
		if (texture != NULL)
		{
			if (width == -1.0 || height == -1.0)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
		}
	}
    
    Sprite::Sprite(ZwopSprite *zwopSprite, FilterType filter, float width, float height)
        : Graphic(),
        texture(NULL),
        shader(NULL),
        width(width),
        height(height),
        textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        originalTextureOffset(Vector2::zero),
        originalTextureScale(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
        blend(BLEND_ALPHA)
    {
        texture = Assets::RequestTexture( zwopSprite->GetSheet()->GetTextureName(), filter );
        if (width == -1.0 || height == -1.0)
        {
            this->width = zwopSprite->GetSourceSize().x;
            this->height = zwopSprite->GetSourceSize().y;
        }
        
        originalTextureScale = textureScale = zwopSprite->GetTextureScale();
        originalTextureOffset = textureOffset = zwopSprite->GetTextureOffset();
        
        trimScale = zwopSprite->GetSize() / zwopSprite->GetSourceSize();
        trimOffset = zwopSprite->GetSpriteOffset();
    }
    
    Sprite::Sprite(ZwopSprite *zwopSprite, float width, float height)
    : Graphic(),
        texture(NULL),
        shader(NULL),
        width(width),
        height(height),
        textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        originalTextureOffset(Vector2::zero),
        originalTextureScale(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
        blend(BLEND_ALPHA)
    {
        texture = Assets::RequestTexture( zwopSprite->GetSheet()->GetTextureName() );
        if (width == -1.0 || height == -1.0)
        {
            this->width = zwopSprite->GetSourceSize().x;
            this->height = zwopSprite->GetSourceSize().y;
        } 
        
        originalTextureScale = textureScale = zwopSprite->GetTextureScale();
        originalTextureOffset = textureOffset = zwopSprite->GetTextureOffset(); 
        
        trimScale = zwopSprite->GetSize() / zwopSprite->GetSourceSize();
        trimOffset = zwopSprite->GetSpriteOffset();
    }

	Sprite::Sprite()
		: Graphic(),
		texture(NULL),
		shader(NULL),
		width(1.0f),
		height(1.0f),
		textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        originalTextureOffset(Vector2::zero),
        originalTextureScale(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
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
		if(shader != NULL)
		{
			delete shader;
			shader = NULL;
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
		if(shader != NULL)
		{
			shader->Use();
		}
		Graphics::PushMatrix();
        
            // Calculate proper offset
            Vector2 offset = trimOffset;

			Graphics::Translate(position.x+offset.x, position.y-offset.y, 0.0f);
			Graphics::Rotate(rotation, 0, 0, 1);
            Graphics::Scale( trimScale );
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
		Shader::None();

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

	void Sprite::SetShader(Shader *shader)
	{
		if(this->shader == NULL)
		{
			this->shader = shader;
		}
		else
		{
			Shader::None();
			delete this->shader;
			this->shader = shader;
		}
	}
	
	Shader* Sprite::GetShader()
	{
		return shader;
	}
}