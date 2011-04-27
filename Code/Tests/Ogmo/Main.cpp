#include "Monocle.h"
#include "Ogmo.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->Init();
	Assets::SetContentPath(Assets::GetContentPath()+"/Ogmo/");
	game->SetScene(new Ogmo::World());
	game->Main();
	delete game;
	return 0;
}
