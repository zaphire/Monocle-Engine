#pragma once

#include "../Color.h"
#include "../TextureAsset.h"
#include "../Graphics.h"
#include <string>

namespace Monocle
{
	class Entity;

	class Sprite : public Graphic
	{
	public:
		Sprite(const std::string &filename, float width=-1, float height=-1);
		Sprite(const std::string &filename, FilterType filter, float width=-1, float height=-1);
		Sprite();
		~Sprite();
		void Update();
		void Render(Entity *entity);
		void GetWidthHeight(int *width, int *height);

		TextureAsset *texture;
		float width, height;//,angle;
		Vector2 textureOffset;
		Vector2 textureScale;

		BlendType blend;
	};
}