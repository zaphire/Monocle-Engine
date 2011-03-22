#include "../../Monocle.h"
#include "../../Tween.h"
#include "../../Graphics/Sprite.h"

using namespace Monocle;

namespace Marian
{
	/*
	class LevelEditor : public Editor
	{
	public:
		LevelEditor();
		void Update();
		bool isOn;
		Sprite *selectedSprite;
		int selectedSpriteIndex;

		void UpdateOldSpriteMethod();
	};
	*/

	class TitleScene : public Scene
	{
	public:
		void Begin();
		void End();
		void Update();

		Entity* AddSpriteEntity(Vector2 pos, int layer, const char *filename, int width, int height, float scale, Sprite** sprite=NULL);
	};

	class LevelScene : public Scene
	{
	public:
		void Begin();
		void Update();
		void End();
	};
}