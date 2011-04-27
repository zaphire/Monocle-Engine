#include "../../Monocle.h"
#include "../../LevelEditor/LevelEditor.h"

using namespace Monocle;

// Test the FringeTileEditor
namespace LevelEditorTest
{
	class Player : public Entity
	{
	public:
		Player();
		void Update();
		Vector2 velocity;
		float force;
	};

	class LevelScene : public Scene
	{
	public:
		void Begin();
		void Update();
		void End();

		LevelEditor levelEditor;
	};

	extern LevelScene *levelScene;
};
