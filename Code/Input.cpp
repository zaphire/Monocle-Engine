#include "Input.h"
#include <stdio.h> // for null :P
#include "Debug.h"

namespace Monocle
{
	Input *Input::instance = NULL;

	Input::Input()
	{
		instance = this;
	}

	void Input::Init()
	{

	}

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		return Platform::IsKeyPressed(keyCode);
	}
}