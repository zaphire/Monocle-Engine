#pragma once

namespace Monocle
{
	//!
	//! \brief 2 dimensional vector. (x and y coordinates) Can be used to represent directions and/or positions.
	//!
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
		
		Vector2(const Vector2 &vector2);
		Vector2();
		Vector2(float x, float y);

		

        void Set(float x, float y);

		// return random vector
		static Vector2 Random();

		float GetSquaredMagnitude();
		float GetMagnitude();
		Vector2 GetNormalized();
		void Normalize();
		Vector2 GetPerpendicularLeft();
		Vector2 GetPerpendicularRight();
		void Clamp(float max);
		bool IsInRange(float range);
		float Dot(Vector2 other);
		float Cross(Vector2 other);
		//! return angle in radians
		float GetAngleRadians();
		float GetAngleDegrees();
		
		/*
		static Vector2 Add(Vector2 a, Vector2 b);
		static Vector2 Subtract(Vector2 a, Vector2 b);
		static Vector2 Multiply(Vector2 a, float s);
		static Vector2 Scale(Vector2 a, float s);
		*/

		Vector2 xx() const;
		Vector2 yy() const;
		Vector2 yx() const;

		float& operator[] (unsigned int i);
		
		Vector2& operator=(const Vector2& rhs);
		bool operator==(const Vector2& rhs);
		bool operator!=(const Vector2& rhs);
	
		Vector2& operator+=(const Vector2& rhs);
		Vector2& operator-=(const Vector2& rhs);
		Vector2& operator*=(float rhs);				// scalar multiplication
		Vector2& operator/=(float rhs);				// scalar inverse multiplication

	};

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs);	
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
	Vector2 operator-(const Vector2 &rhs);
	
	Vector2 operator*(float lhs, const Vector2& rhs);		// left scalar multiplication
	Vector2 operator*(const Vector2& lhs, float rhs);		// right scalar multiplication
	Vector2 operator/(const Vector2& lhs, float rhs);		// right scalar inverse multiplication
	Vector2 operator/(float lhs, const Vector2 &rhs);

	

	
	Vector2 operator*(const Vector2& lhs, const Vector2& rhs);	// multiply components (scale)
	//float operator^(const Vector2& lhs, const Vector2& rhs);	// cross product
}
