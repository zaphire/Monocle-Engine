#include "Monocle.h"
#include "Jumper.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	Assets::SetContentPath(Assets::GetContentPath()+"/Jumper/");
	game->SetScene(new Jumper::GameScene());
	game->Main();
	delete game;
	return 0;
}
