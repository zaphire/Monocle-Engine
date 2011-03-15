#include "Color.h"

namespace Monocle
{
	Color Color::white	= Color(1,1,1,1);
	Color Color::grey	= Color(0.5f, 0.5f, 0.5f, 1.0f);

	Color Color::black	= Color(0,0,0,1);
	Color Color::red	= Color(1,0,0,1);
	Color Color::green	= Color(0,1,0,1);
	Color Color::blue	= Color(0,0,1,1);
	Color Color::yellow = Color(1,1,0,1);

	Color::Color()
		: r(1), g(1), b(1), a(1)
	{

	}

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{

	}
}