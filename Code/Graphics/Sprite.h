#include "../Color.h"
#include "../TextureAsset.h"
#include "../Graphics.h"
#include <string>

#pragma once

namespace Monocle
{
	class Entity;

	class Sprite : public Graphic
	{
	public:
		Sprite(const std::string &filename, float width=-1, float height=-1);
		Sprite(const std::string &filename, FilterType filter, float width=-1, float height=-1);
		~Sprite();
		//void Update();
		void Render();
		void GetWidthHeight(int *width, int *height);

		TextureAsset *texture;
		float width, height;//,angle;
		Vector2 textureOffset;
		Vector2 textureScale;

		static Sprite *selectedSprite;

		static bool showBounds;
	};
}