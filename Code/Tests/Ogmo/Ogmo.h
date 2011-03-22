#include "../../Monocle.h"
#include "../../Graphics/Sprite.h"
#include "../../Ogmosis/Ogmosis.h"
#include "../../Level.h"

// by @NoelFB
using namespace Monocle;

namespace Ogmo
{
	class Player : public Entity
	{
	public:
		Player(int x, int y);
		void Update();

		Vector2 velocity;

		//details
		Sprite *sprite;
		std::string tag;

		//moving stuff
		float const FRICTION_GROUND;
		float const FRICTION_AIR;
		float const GRAVITY;
		float const JUMP;
		float const ACCELERATION;

		//double jump / clinging to wall
		bool doubleJump;
		int cling;
		int clingDir;
		bool onGround;

		//other/util
		float Sign(float i, float to);
		bool Motion(float &speed, float &to);
		bool Ground();
		bool CollideAt(const std::string &tag, float x, float y);

	};

	class Wall : public Entity
	{
	public:
		Wall(int x, int y);
		void Render();
	};

	class World : public Scene
	{
	public:
		void Begin();
		void End();
		void Update();

		Player *player;
		Wall *wall;

	};
}