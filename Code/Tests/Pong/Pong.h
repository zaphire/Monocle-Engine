#include "../../Monocle.h"
#include "../../TextureAsset.h"

using namespace Monocle;

namespace Pong
{
	class Paddle : public Entity
	{
	public:
		void Update();
		void Render();

		KeyCode keyUp;
		KeyCode keyDown;
	};

	class Ball : public Entity
	{
	public:
		Ball();
		void Update();
		void Render();
		TextureAsset *texture;
	};

	class GameScene : public Scene
	{
	public:
		Ball *ball;
		Paddle *paddle1, *paddle2;

		void Begin();
		void End();
		void Update();
	};
}