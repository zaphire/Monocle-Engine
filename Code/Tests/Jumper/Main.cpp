#include "Monocle.h"
#include "Jumper.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->Init();
        game->SetScene(new Jumper::GameScene());
	game->Main();
	delete game;
	return 0;
}
