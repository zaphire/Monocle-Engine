#pragma once

namespace Monocle
{
	//!
	//! 3 dimensional vector class. Not fully implemented yet.
	//!
	class Vector3
	{
	public:
		float x,y,z;

		static Vector3 zero;
		static Vector3 one;
		static Vector3 down;
		static Vector3 up;
		static Vector3 left;
		static Vector3 right;

		Vector3();
		Vector3(float x, float y, float z);

		/*
		float GetSquaredMagnitude();
		float GetMagnitude();
		Vector2 GetNormalized();
		void Normalize();
		Vector2 GetPerpendicularLeft();
		Vector2 GetPerpendicularRight();
		void Clamp(float max);
		bool IsInRange(float range);
		float Dot(Vector2 other);
		*/
		
		/*
		static Vector2 Add(Vector2 a, Vector2 b);
		static Vector2 Subtract(Vector2 a, Vector2 b);
		static Vector2 Multiply(Vector2 a, float s);
		static Vector2 Scale(Vector2 a, float s);
		*/

	};
}