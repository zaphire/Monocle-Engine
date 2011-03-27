#include "Monocle.h"
#include "FTE.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->Init();
	game->SetScene(new FTE::LevelScene());
	game->Main();
	delete game;
	return 0;
}
