#include "Monocle.h"
#include "LevelEditorTest.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->Init();
	game->SetScene(new LevelEditorTest::LevelScene());
	game->Main();
	delete game;
	return 0;
}
