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
	Color Color::orange = Color(1.0f,0.5f,0.0f,1.0f);
	Color Color::purple = Color(0.5f, 0.0f, 1.0f, 1.0f);

	Color::Color(const Color &color)
		: r(color.r), g(color.g), b(color.b), a(color.a)
	{
	}

	Color::Color()
		: r(1), g(1), b(1), a(1)
	{

	}

	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{

	}

	bool Color::operator!=(const Color& rhs)
	{
		return ((r != rhs.r) || (g != rhs.g) || (b != rhs.b) || (a != rhs.a));
	}

	Color operator+(const Color& lhs, const Color& rhs)
	{
		return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a + rhs.a);
	}
	
	Color operator-(const Color& lhs, const Color& rhs)
	{
		return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a - rhs.a);
	}

	Color operator*(float lhs, const Color& rhs)		// left scalar multiplication
	{
		return Color(lhs * rhs.r, lhs * rhs.g, lhs * rhs.b, lhs * rhs.a);
	}
	
	Color operator*(const Color& lhs, float rhs)		// right scalar multiplication
	{
		return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs, lhs.a * rhs);
	}
}