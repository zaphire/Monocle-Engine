#include "Monocle.h"
#include "PuppetTest.h"

using namespace Monocle;

int main(void)
{
        Game *game = new Game();
        game->Init();
        game->SetScene(new PuppetTest::TestScene());
        game->Main();
        delete game;
        return 0;
}
