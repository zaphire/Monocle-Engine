#pragma once

namespace Monocle
{
	class Color
	{
	public:
		Color();
		Color(float r, float g, float b, float a);

		float r, g, b, a;

		static Color white;
		static Color grey;
		static Color black;
		static Color red;
		static Color green;
		static Color blue;
		static Color yellow;
	};
}