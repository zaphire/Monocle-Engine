#pragma once

namespace Monocle
{
	//! Describes a color using 4 floats in 0.0 - 1.0 range.
	class Color
	{
	public:
		Color(const Color &color);
		Color();
		Color(float r, float g, float b, float a);

        //! Clamps the rgba values to between 0 and 1.0, inclusive.
		void Clamp();

        //! Indirect accessor for rgba values. Color[0] = r, [1] = g, [2] = b, [3] = a, [4+] = undefined
		float& operator[] (unsigned int i);
		bool operator!=(const Color& rhs);

		//! Red, green, blue, alpha (translucency)
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
	Color operator/(const Color& lhs, float rhs);

}
