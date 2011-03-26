#include "Monocle.h"
#include "Marian.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->Init();
	game->SetScene(new Marian::LevelScene());
	game->Main();
	delete game;
	return 0;
}
