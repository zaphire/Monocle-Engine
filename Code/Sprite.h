#include "Entity.h"
#include "Color.h"
#include "TextureAsset.h"

namespace Monocle
{
	class Sprite : public Entity
	{
	public:
		Sprite(Vector2 position, int layer, const char *filename, float width=-1, float height=-1);
		Sprite();
		void Render();
		TextureAsset *texture;
		Color color;
		float width, height, scale;

		bool isSelected;

		static bool showBounds;
	};
}