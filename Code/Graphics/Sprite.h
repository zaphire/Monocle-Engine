#include "../Color.h"
#include "../TextureAsset.h"
#include "../Graphics.h"

namespace Monocle
{
	class Entity;

	class Sprite : public Graphic
	{
	public:
		Sprite(const char *filename, float width=-1, float height=-1);
		~Sprite();
		void Render(Entity *entity);

		TextureAsset *texture;
		Color color;
		float width, height, scale, angle;
		Vector2 textureOffset;
		Vector2 textureScale;

		bool isSelected;

		static bool showBounds;
	};
}