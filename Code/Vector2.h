#pragma once

namespace Monocle
{
	class Vector2
	{
	public:
		float x,y;

		static Vector2 zero;
		static Vector2 one;
		static Vector2 down;
		static Vector2 up;
		static Vector2 left;
		static Vector2 right;
		
		Vector2();
		Vector2(float x, float y);

		float GetSquaredMagnitude();
		float GetMagnitude();
		Vector2 GetNormalized();
		void Normalize();
		Vector2 GetPerpendicularLeft();
		Vector2 GetPerpendicularRight();
		void Clamp(float max);
		bool IsInRange(float range);
		float Dot(Vector2 other);
		
		/*
		static Vector2 Add(Vector2 a, Vector2 b);
		static Vector2 Subtract(Vector2 a, Vector2 b);
		static Vector2 Multiply(Vector2 a, float s);
		static Vector2 Scale(Vector2 a, float s);
		*/

		inline Vector2 operator+=(const Vector2& rhs);	
		inline Vector2 operator-=(const Vector2& rhs);
		inline Vector2 operator*=(float rhs);					// scalar multiplication
		inline Vector2 operator/=(float rhs);					// scalar inverse multiplication
	};

	inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs);	
	inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs);

	inline Vector2 operator*(float lhs, const Vector2& rhs);			// left scalar multiplication
	inline Vector2 operator*(const Vector2& lhs, float rhs);			// right scalar multiplication
	inline Vector2 operator/(const Vector2& lhs, float rhs);			// right scalar inverse multiplication
	
	inline float operator*(const Vector2& lhs, const Vector2& rhs);	// dot product
	inline float operator^(const Vector2& lhs, const Vector2& rhs);	// cross product
}