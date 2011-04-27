#include "Vector2.h"
#include "Random.h"
#include "MonocleToolkit.h"
#include <cmath>

namespace Monocle
{
	Vector2 Vector2::zero = Vector2(0,0);
	Vector2 Vector2::one = Vector2(1,1);
	Vector2 Vector2::up = Vector2(0, -1); //2d style
	Vector2 Vector2::down = Vector2(0, 1);
	Vector2 Vector2::left = Vector2(-1, 0);
	Vector2 Vector2::right = Vector2(1, 0);

	Vector2::Vector2(const Vector2 &vector2)
		: x(vector2.x), y(vector2.y)
	{
	}

	Vector2::Vector2()
		: x(0.0f), y(0.0f)
	{
	}

	Vector2::Vector2(float x, float y)
		: x(x), y(y)
	{
	}
    
    void Vector2::Set(float x, float y)
    {
        this->x = x, this->y = y;
    }

	Vector2 Vector2::Random()
	{
		float rad = (Random::Range(0,360)) * Monocle::deg2Rad;
		return Vector2(sin(rad), cos(rad));
	}

	float Vector2::GetSquaredMagnitude()
	{
		return pow(x, 2) + pow(y, 2);
	}

	float Vector2::GetMagnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vector2 Vector2::GetNormalized()
	{
		float mag = GetMagnitude();
		return Vector2(x / mag, y / mag);
	}

	void Vector2::Normalize()
	{
		float mag = GetMagnitude();
		x /= mag;
		y /= mag;
	}

	Vector2 Vector2::GetPerpendicularLeft()
	{
		return Vector2(-y, x);
	}

	Vector2 Vector2::GetPerpendicularRight()
	{
		return Vector2(y, -x);
	}

	void Vector2::Clamp(float max)
	{
		if ((pow(x, 2) + pow(y, 2)) > pow(max, 2))
		{
			Normalize();
			x *= max;
			y *= max;
		}
	}

	bool Vector2::IsInRange(float range)
	{
		return ((pow(x, 2) + pow(y, 2)) <= pow(range, 2));
	}

	float Vector2::Dot(Vector2 b)
	{
		return (x * b.x + y * b.y);
	}
	
	float Vector2::Cross(Vector2 b)
	{
		return (x * b.y - y * b.x);
	}
	
	float Vector2::operator[](int i)
	{
		if (i <= 0)
		{
			return x;
		}
		else
		{
			return y;
		}
	}
	
	Vector2& Vector2::operator=(const Vector2& rhs)
	{
		if (&rhs != this)
		{
			x = rhs.x;
			y = rhs.y;
		}
		return *this;
	}
	
	bool Vector2::operator==(const Vector2& rhs)
	{
		return ((x == rhs.x) && (y == rhs.y));
	}
	
	bool Vector2::operator!=(const Vector2& rhs)
	{
		return ((x != rhs.x) || (y != rhs.y));
	}
	
	Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		x = x + rhs.x;
		y = y + rhs.y;
		return *this;
	}
	
	Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		x = x - rhs.x;
		y = y - rhs.y;
		return *this;
	}
	
	Vector2& Vector2::operator*=(float rhs)				// scalar multiplication
	{
		x = x * rhs;
		y = y * rhs;
		return *this;
	}
	
	Vector2& Vector2::operator/=(float rhs)				// scalar inverse multiplication
	{
		x = x / rhs;
		y = y / rhs;
		return *this;
	}

	Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	
	Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Vector2 operator-(const Vector2 &rhs)
	{
		return Vector2(-rhs.x, -rhs.y);
	}
	
	Vector2 operator*(float lhs, const Vector2& rhs)		// left scalar multiplication
	{
		return Vector2(lhs * rhs.x, lhs * rhs.y);
	}
	
	Vector2 operator*(const Vector2& lhs, float rhs)		// right scalar multiplication
	{
		return Vector2(lhs.x * rhs, lhs.y * rhs);
	}
	
	Vector2 operator/(const Vector2& lhs, float rhs)		// right scalar inverse multiplication
	{
		return Vector2(lhs.x / rhs, lhs.y / rhs);
	}
	
	Vector2 operator*(const Vector2& lhs, const Vector2& rhs)		// multiply component (scale)
	{
		return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	Vector2 operator/(float lhs, const Vector2 &rhs)
	{
		return Vector2(lhs / rhs.x, lhs / rhs.y);
	}
	
	/*
	float operator^(const Vector2& lhs, const Vector2& rhs)		// cross product
	{
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}
	*/
}
