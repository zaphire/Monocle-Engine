#include <Monocle.h>
#include <Graphics/Sprite.h>
#include <Graphics/SpriteAnimation.h>
#include <Level.h>

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
		SpriteAnimation *sprite;
		//std::string tag;

		//moving stuff
		int const FRICTION_GROUND;
		int const FRICTION_AIR;
		float const MAXSPEED_GROUND;
		float const MAXSPEED_AIR;
		int const GRAVITY;
		int const JUMP;
		int const ACCELERATION;
		int const WALLJUMP;

		//double jump / clinging to wall
		bool doubleJump;
		int cling;
		int clingDir;
		bool onGround;
		bool direction;

		//other/util
		bool Motion(float &speed, float &to);
		bool CollideAt(const std::string &tag, float x, float y);

	};

	class Wall : public Entity
	{
	public:
		Wall(int x, int y);
		void Render();
	};

	class Coin : public Entity
	{
	public:
		Coin(int x, int y, Sprite *sprite);
		void Update();
		
		bool collected;
		Vector2 start;
		bool reset;
	};

	class Spike : public Entity
	{
	public:
		Spike(int x, int y, Sprite *sprite);
	};

	class World : public Scene
	{
	public:
		void Begin();
		void End();
		void Update();
		static void ResetCoins();
		static World* instance;

		static Player *player;
		Wall *wall;
		Spike *spike;
		Sprite *atCoin;
		Sprite *atSpike;
		std::list<Coin*> coins;

	};
}