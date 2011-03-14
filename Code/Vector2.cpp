#include "Vector2.h"
#include <math.h>

namespace Monocle
{
	Vector2 Vector2::zero = Vector2(0,0);
	Vector2 Vector2::one = Vector2(1,1);
	Vector2 Vector2::up = Vector2(0, 1);
	Vector2 Vector2::down = Vector2(0, -1);
	Vector2 Vector2::left = Vector2(-1, 0);
	Vector2 Vector2::right = Vector2(1, 0);

	Vector2::Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float Vector2::getSquaredMagnitude()
	{
		return pow(x, 2) + pow(y, 2);
	}

	float Vector2::getMagnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vector2 Vector2::getNormalized()
	{
		float mag = getMagnitude();
		return Vector2(x / mag, y / mag);
	}

	void Vector2::normalize()
	{
		float mag = getMagnitude();
		x /= mag;
		y /= mag;
	}

	Vector2 Vector2::getPerpendicularLeft()
	{
		return Vector2(-y, x);
	}

	Vector2 Vector2::getPerpendicularRight()
	{
		return Vector2(y, -x);
	}

	void Vector2::Clamp(float max)
	{
		if ((pow(x, 2) + pow(y, 2)) > pow(max, 2))
		{
			normalize();
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
}