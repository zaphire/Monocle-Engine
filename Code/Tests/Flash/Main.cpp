#include "Monocle.h"
#include "Flash.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->Init();
	game->SetScene(new Flash::TestScene());
	game->Main();
	delete game;
	return 0;
}
