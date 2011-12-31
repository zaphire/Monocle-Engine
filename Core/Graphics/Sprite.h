#pragma once

#include "../Color.h"
#include "../TextureAsset.h"
#include "../Graphics.h"
#include "../Shader.h"
#include "ZwopSpriteSheet.h"
#include <string>

namespace Monocle
{
	class Entity;

	class Sprite : public Graphic
	{
	public:
		Sprite(const std::string &filename, float width=-1.0, float height=-1.0);
		Sprite(const std::string &filename, FilterType filter, float width=-1.0, float height=-1.0);
        
        Sprite(ZwopSprite *zwopSprite, float width=-1.0, float height=-1.0);
        Sprite(ZwopSprite *zwopSprite, FilterType filter, float width=-1.0, float height=-1.0);
        
		Sprite();
		~Sprite();
		void Update();
		void Render(Entity *entity);
		void GetWidthHeight(float *width, float *height);
		void SetShader(Shader* shader);
		Shader* GetShader();
        
        void AdjustForZwopSprite( ZwopSprite *zs );

		TextureAsset *texture;
		float width, height;//,angle;
		Vector2 textureOffset;
		Vector2 textureScale;
        
        // Original textureOffset and textureScale (for animations within spritesheets)
		Vector2 textureOffsetModifier;
		Vector2 textureScaleModifier;
        
        // Render offset (use for hotspots in animations, etc)
        // This essentially is how far from position we'll render (but we want to retain our position for worldposition purposes)
        Vector2 renderOffset;
        
        // Offsets (for trimmed sprites)
        Vector2 trimOffset;
        Vector2 trimScale;

		BlendType blend;
	private:
		Shader* shader;
	};
}