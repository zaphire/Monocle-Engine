#include "Monocle.h"
#include "LevelEditorTest.h"

using namespace Monocle;

int main(void)
{
	// create a new game instance
	Game *game = new Game();
	// set the base content path (used by everything)
	Assets::SetContentPath(Assets::GetContentPath() + "/LevelEditorTest/");
	// set the first scene
	game->SetScene(new LevelEditorTest::LevelScene());
	// run the main function
	game->Main();
	// delete the game
	delete game;
	return 0;
}
