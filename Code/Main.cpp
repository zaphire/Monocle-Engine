#include "Monocle.h"

// test
#include "Vector2.h"
#include "Math.h"

using namespace Monocle;

int main(void)
{
	Core *core = new Core();
	core->Init();
	core->Main();
	delete core;
	return 0;
}