#include "Monocle.h"
#include "Ogmo.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	Assets::SetContentPath(Assets::GetContentPath()+"/Ogmo/"); // jw: Why are we doing this, inconsistent.
	game->SetScene(new Ogmo::World());
	game->Main();
	delete game;
	return 0;
}
