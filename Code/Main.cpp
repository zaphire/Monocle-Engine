#include "Monocle.h"

// test
#include "Vector2.h"
#include "Math.h"


//#define TEST_CORE
//#define TEST_PONG
#define TEST_MARIAN
//#define TEST_JUMPER

#ifdef TEST_PONG
#include "Tests/Pong/Pong.h"
#endif

#ifdef TEST_MARIAN
#include "Tests/Marian/Marian.h"
#endif

#ifdef TEST_JUMPER
#include "Tests/Jumper/Jumper.h"
#endif

using namespace Monocle;

int main(void)
{
	Core *core = new Core();

	core->Init();

#ifdef TEST_PONG
	core->SetScene(new Pong::GameScene());
#endif

#ifdef TEST_MARIAN
	core->SetScene(new Marian::TitleScene());
#endif

#ifdef TEST_JUMPER
	core->SetScene(new Jumper::GameScene());
#endif

	core->Main();
	
	delete core;
	
	return 0;
}