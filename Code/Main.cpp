#include "Monocle.h"

// test
#include "Vector2.h"
#include "Math.h"


//#define TEST_CORE
#define TEST_PONG



#ifdef TEST_PONG
#include "Tests/Pong/Pong.h"
#endif

using namespace Monocle;

int main(void)
{
#ifdef TEST_CORE
	Core *core = new Core();
	core->Init();
	core->Main();
	delete core;
	return 0;
#endif

#ifdef TEST_PONG
	Pong::PongGame *core = new Pong::PongGame();
	core->Init();
	core->Main();
	delete core;
	return 0;
#endif
}