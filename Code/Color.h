#pragma once

namespace Monocle
{
	class Color
	{
	public:
		Color(const Color &color);
		Color();
		Color(float r, float g, float b, float a);

		bool operator!=(const Color& rhs);

		float r, g, b, a;

		static Color white;
		static Color grey;
		static Color black;
		static Color red;
		static Color green;
		static Color blue;
		static Color yellow;
		static Color orange;
		static Color purple;
	};

	Color operator+(const Color& lhs, const Color& rhs);	
	Color operator-(const Color& lhs, const Color& rhs);
	Color operator*(float lhs, const Color& rhs);		// left scalar multiplication
	Color operator*(const Color& lhs, float rhs);		// right scalar multiplication
}