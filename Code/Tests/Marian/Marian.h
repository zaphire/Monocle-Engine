#include "../../Monocle.h"
#include "../../Tween.h"
#include "../../Graphics/Sprite.h"
#include "../../LevelEditor/FringeTileEditor.h"

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

	class Obstruction : public Entity
	{
	public:
		Obstruction(int w, int h);
	};

	class Player : public Entity
	{
	public:
		Player();
		void Update();

		Vector2 velocity;
	};

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

		FringeTileEditor fringeTileEditor;

		Player *player;
	};
}