#include "../../Monocle.h"
#include "../../LevelEditor/FringeTileEditor.h"

using namespace Monocle;

// Test the FringeTileEditor
namespace FTE
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

		FringeTileEditor fringeTileEditor;
	};

	extern LevelScene *levelScene;
};