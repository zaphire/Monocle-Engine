#include "Monocle.h"

// test
#include "Vector2.h"
#include "Math.h"


//#define TEST_CORE
//#define TEST_PONG



#ifdef TEST_PONG
#include "Tests/Pong/Pong.h"
#endif

using namespace Monocle;

int main(void)
{
	Core *core = new Core();
	core->Init();
#ifdef TEST_PONG
	core->SetScene(new Pong::GameScene());
#endif
	core->Main();
	delete core;
	return 0;
}