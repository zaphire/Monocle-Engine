#include "Monocle.h"
#include "PuppetTest.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	Assets::SetContentPath(Assets::GetContentPath() + "/PuppetTest/");
	game->SetScene(new PuppetTest::TestScene());
	game->Main();
	delete game;
	return 0;
}
