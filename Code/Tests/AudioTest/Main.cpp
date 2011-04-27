#include "Monocle.h"
#include "AudioTest.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->SetScene(new AudioTest::GameScene());
	game->Main();
	delete game;
	return 0;
}
