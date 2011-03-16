#include "../../Monocle.h"
#include "../../Sprite.h"

// by @NoelFB
using namespace Monocle;
using std::list;
using std::string;

namespace Jumper
{
	class Player : public Sprite
	{
	public:
		Player(Vector2 pos);
		void Update();
		float Sign(float i, float to);

		KeyCode keyUp;
		KeyCode keyRight;
		KeyCode keyLeft;

		Vector2 velocity;
		float gravity;
		float speed;
		bool onGround;
		float jump;
		float maxSpeed;
		bool isJumping;
	};

	
	class Wall : public Entity
	{
	public:
		Wall(Vector2 p, float w, float h);
		//void Update();
		void Render();

		//temporary, replace base class with Sprite eventually
		float width, height;
	};

	class GameScene : public Scene
	{
	public:
		Player *player;
		Wall *wall;

		void Begin();
		void End();
		void Update();
	};
}