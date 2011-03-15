#include "../../Monocle.h"

using namespace Monocle;

namespace Pong
{
	class Paddle : public Entity
	{
	public:
		Paddle();
		void Update();
		void Render();

		KeyCode keyUp;
		KeyCode keyDown;
	};

	class Ball : public Entity
	{
	public:
		void Update();
		void Render();
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