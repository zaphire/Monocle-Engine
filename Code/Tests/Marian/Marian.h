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

	enum Flag
	{
		FLAG_NONE=0,
		FLAG_INTRO,
		FLAG_MAX
	};

	class Continuity
	{
	public:
		void SetMaxFlags(int maxFlags);

		void SetFlag(int flag, int value);
		int GetFlag(int flag);
		bool IsFlag(int flag, int value);
		
	private:
		std::map<int, int> flags;
	};

	class Cursor : public Entity
	{
	public:
		Cursor();
		void Update();
		void Render();

		Entity *attachedToEntity;
	};

	class Threadable : public Entity
	{
	public:
		Threadable();
		void Update();
		void Thread();
	};

	class TriggerLevel : public Entity
	{
	public:
		TriggerLevel(const std::string &level);
		void Update();
		void Render();
	};

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
		void Move(Vector2 dir, bool isWalking=false);

		Vector2 velocity;
		Vector2 moveDir;
		bool isWalking;
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
		Cursor *cursor;
	};

	extern LevelScene *levelScene;
}