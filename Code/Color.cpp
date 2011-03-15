#include "Color.h"

namespace Monocle
{
	Color Color::white = Color(1,1,1,1);

	Color::Color()
		: r(1), g(1), b(1), a(1)
	{

	}

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{

	}
}