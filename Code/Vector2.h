#pragma once

#include "Math.h"

namespace Monocle
{
	class Vector2
	{
	public:
		float x,y;

		static Vector2 zero;
		static Vector2 one;
		static Vector2 up;
		static Vector2 down;
		static Vector2 left;
		static Vector2 right;
		
		Vector2(float x, float y);

		float getSquaredMagnitude();
		float getMagnitude();
		Vector2 getNormalized();
		void normalize();
		Vector2 getPerpendicularLeft();
		Vector2 getPerpendicularRight();
		void Clamp(float max);
		bool IsInRange(float range);
		float Dot(Vector2 other);
		
		/*
		static Vector2 Add(Vector2 a, Vector2 b);
		static Vector2 Subtract(Vector2 a, Vector2 b);
		static Vector2 Multiply(Vector2 a, float s);
		static Vector2 Scale(Vector2 a, float s);
		*/

	};
}