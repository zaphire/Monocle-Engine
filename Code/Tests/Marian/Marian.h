#include "../../Monocle.h"
#include "../../TextureAsset.h"
#include "../../Tween.h"
#include "../../Color.h"

using namespace Monocle;

namespace Marian
{
	class Sprite;

	class LevelEditor : public Editor
	{
	public:
		LevelEditor();
		void Update();
		bool isOn;
		Sprite *selectedSprite;
		int selectedSpriteIndex;
	};

	class Sprite : public Entity
	{
	public:
		Sprite(int layer, const char *filename, float scale);
		Sprite();
		void Render();
		TextureAsset *texture;
		Color color;
		float scale;

		bool isSelected;

		static bool showBounds;
	};

	class TitleScene : public Scene
	{
	public:
		void Begin();
		void End();
		void Update();
	};

}