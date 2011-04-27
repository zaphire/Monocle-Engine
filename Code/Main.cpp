#include "Monocle.h"

// test
#include "Vector2.h"

#if defined(MONOCLE_LINUX)
	#define TEST_JUMPER
	//#define TEST_FLASH
	//#define TEST_OGMO
//#elif defined(MONOCLE_MAC)
//    #define TEST_PONG
#else
	//#define TEST_game
	//#define TEST_PONG
	//#define TEST_JUMPER
	//#define TEST_OGMO
	//#define TEST_LEVELEDITOR
	//#define TEST_FLASH
	//#define TEST_PUPPETTEST
    #define TEST_AUDIOTEST
#endif

#ifdef TEST_PONG 
	#include "Tests/Pong/Pong.h"
#endif

#ifdef TEST_JUMPER
	#include "Tests/Jumper/Jumper.h"
#endif

#ifdef TEST_FLASH
	#include "Tests/Flash/Flash.h"
#endif

#ifdef TEST_OGMO
	#include "Tests/Ogmo/Ogmo.h"
#endif

#ifdef TEST_LEVELEDITOR
	#include "Tests/LevelEditor/LevelEditorTest.h"
#endif

#ifdef TEST_PUPPETTEST
	#include "Tests/PuppetTest/PuppetTest.h"
#endif

#ifdef TEST_AUDIOTEST
    #include "Tests/AudioTest/AudioTest.h"
#endif

using namespace Monocle;

int main(void)
{
	Game *game = new Game();

	game->Init();

#ifdef TEST_PONG
	game->SetScene(new Pong::GameScene());
#endif

#ifdef TEST_JUMPER
	game->SetScene(new Jumper::GameScene());
#endif

#ifdef TEST_FLASH
	game->SetScene(new Flash::TestScene());
#endif

#ifdef TEST_OGMO
	game->SetScene(new Ogmo::World());
#endif

#ifdef TEST_LEVELEDITOR
	game->SetScene(new LevelEditorTest::LevelScene());
#endif

#ifdef TEST_PUPPETTEST
	game->SetScene(new PuppetTest::TestScene());
#endif
    
#ifdef TEST_AUDIOTEST
    game->SetScene(new AudioTest::GameScene());
#endif

	game->Main();
	
	delete game;

	return 0;
}