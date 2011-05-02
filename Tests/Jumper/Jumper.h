#include <Monocle.h>
#include <Graphics/Sprite.h>
#include <Level.h>

// by @NoelFB
using namespace Monocle;

namespace Jumper
{
	/*
	class Mover : public Sprite
	{
	public:
		Mover(int layer, std::string graphic, Vector2 pos, float scale);
		void Update();

		Vector2 velocity;
	};
	*/

	class Player : public Entity
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

		float leanAmount; // how much does he lean into his left/right movement?

		Sprite *sprite;
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

		void SpawnPlayer(Vector2 pos);
	};
}
