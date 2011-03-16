#include "Entity.h"
#include "Color.h"
#include "TextureAsset.h"

namespace Monocle
{
	class Sprite : public Entity
	{
	public:
		Sprite(int layer, const char *filename, Vector2 position, float scale);
		Sprite();
		void Render();
		TextureAsset *texture;
		Color color;
		float scale;

		bool isSelected;

		static bool showBounds;
	};
}