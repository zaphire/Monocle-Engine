#include "InputHandlerTest.h"

#include <Monocle.h>

int main()
{
    Monocle::Game game("Input Handler Test");
    game.SetScene(new InputHandlerTestScene());
    game.Main();
    
    return 0;
}
