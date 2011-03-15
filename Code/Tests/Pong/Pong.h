#include "../../Monocle.h"

using namespace Monocle;

namespace Pong
{
	class Paddle : public Entity
	{
	public:
		void Update();
		void Render();
	};

	class Ball : public Entity
	{
	public:
		void Update();
		void Render();
	};

	class PongGame : public Core
	{
	public:
		Ball *ball;
		Paddle *paddle1, *paddle2;
		void Init();
		void Update();
	};
}