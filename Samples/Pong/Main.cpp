#include "Monocle.h"
#include "Pong.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->SetScene(new Pong::GameScene());
	game->Main();
	delete game;
	return 0;
}
