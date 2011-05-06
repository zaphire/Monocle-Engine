#include "Vector2.h"
#include "Random.h"
#include "MonocleToolkit.h"
#include <cmath>

namespace Monocle
{
  //TODO: Replace these constants with a math namespace containing the same functionality.
  const float PI = 2.0f * acos(0.0f);
  const float TWO_PI = 2.0f * PI;

	const Vector2 Vector2::zero = Vector2(0.0f,0.0f);
	const Vector2 Vector2::one = Vector2(1.0f,1.0f);
	const Vector2 Vector2::up = Vector2(0.0f, -1.0f); //2d style
	const Vector2 Vector2::down = Vector2(0.0f, 1.0f);
	const Vector2 Vector2::left = Vector2(-1.0f, 0.0f);
	const Vector2 Vector2::right = Vector2(1.0f, 0.0f);

	Vector2::Vector2()
		: x(0.0f), y(0.0f)
	{
	}

  Vector2::Vector2(const Vector2 &other)
		: x(other.x), y(other.y)
	{
	}

  Vector2::Vector2(float x, float y)
		: x(x), y(y)
	{
	}

	Vector2 Vector2::Random()
	{
		float rad = (Random::Range(0,360)) * Monocle::deg2Rad;
		return Vector2(sin(rad), cos(rad));
	}

	Vector2 Vector2::RandomDeviant(const Vector2& observed, float angle)
	{
    // logic from: http://www.ogre3d.org/docs/api/html/OgreVector2_8h_source.html#l00510
    angle *= Random::Unit() * TWO_PI;
    float cosa = cos(angle);
    float sina = sin(angle);		
		return 
      Vector2(cosa * observed.x - sina * observed.y,
              sina * observed.x + cosa * observed.y);
	}

  void Vector2::Set(const float x, const float y)
  {
      this->x = x, this->y = y;
  }

	void Vector2::Normalize()
	{
		float mag = GetMagnitude();
		x /= mag;
		y /= mag;
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

  bool Vector2::IsUnitLength()const
  {
    return GetMagnitude() == 1.0f;
  }
  
  bool Vector2::IsZeroLength()const
  {
    return *(this) == 0.0f; // only 0,0 vectors have Mag == 0. Skip expensive GetMagnitude();
  }

	bool Vector2::IsInRange(const float range)const
	{
		return ((pow(x, 2) + pow(y, 2)) <= pow(range, 2));
	}

	float Vector2::Dot(const Vector2& other)const
	{
		return (x * other.x + y * other.y);
	}
	
	float Vector2::Cross(const Vector2& other)const
	{
		return (x * other.y - y * other.x);
	}

  float Vector2::SquaredDistance(const Vector2& other)const
  {
    return (*(this) - other).GetSquaredMagnitude();
  }

  float Vector2::Distance(const Vector2& other)const
  {
    return (*(this) - other).GetMagnitude();
  }

  Vector2 Vector2::MidPoint(const Vector2& other)const
  {
    return Vector2((this->x + other.x) * 0.5f, (this->y + other.y) * 0.5f);
  }

  Vector2 Vector2::LERP(const Vector2& to, const float percentage) const
  {
    return *(this) + (to - *(this)) * percentage;
  }

	float Vector2::GetSquaredMagnitude()const
	{
		return pow(x, 2) + pow(y, 2);
	}

	float Vector2::GetMagnitude()const
	{
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	Vector2 Vector2::GetNormalized()const
	{
		float mag = GetMagnitude();
		return Vector2(x / mag, y / mag);
	}

	Vector2 Vector2::GetPerpendicularLeft()const
	{
		return Vector2(-y, x);
	}

	Vector2 Vector2::GetPerpendicularRight()const
	{
		return Vector2(y, -x);
	}

  Vector2 Vector2::xx()const
  {
    return Vector2(this->x, this->x);
  }

  Vector2 Vector2::yy()const
  {
    return Vector2(this->y, this->y);
  }

  Vector2 Vector2::yx()const
  {
    return Vector2(this->y, this->x);
  }

  const float Vector2::operator[](unsigned char i)const
  {
    switch(i)
    {
    case 0: return x;
    case 1: return y;

    default: return x; // TODO: Handle this scenario with an assertion?
    }
  }

  float& Vector2::operator[](unsigned char i)
  {
    switch(i)
    {
    case 0: return x;
    case 1: return y;

    default: return x; // TODO: Handle this scenario with an assertion?
    }
  }

  Vector2 Vector2::operator-()const
  {
    return Vector2(x < 0.0f ? x : - x, y < 0 ? y : - y);
  }
  Vector2 Vector2::operator+()const
  {
    return Vector2(x > 0.0f ? x : - x, y > 0 ? y : - y);
  }

  Vector2& Vector2::operator=(const Vector2& rhs)
  {
    this->x = rhs.x;
    this->y = rhs.y;
    return *(this);
  }

  bool Vector2::operator==(const Vector2& rhs)const
  {
    return this->x == rhs.x && this->y == rhs.y;
  }
  bool Vector2::operator!=(const Vector2& rhs)const
  {
    return !(*(this) == rhs);
  }
  bool Vector2::operator<(const Vector2& rhs)const
  {
    return this->GetSquaredMagnitude() < rhs.GetSquaredMagnitude();
  }
  bool Vector2::operator>(const Vector2& rhs)const
  {
    return this->GetSquaredMagnitude() > rhs.GetSquaredMagnitude();
  }
  bool Vector2::operator<=(const Vector2& rhs)const
  {
    return this->GetSquaredMagnitude() <= rhs.GetSquaredMagnitude();
  }
  bool Vector2::operator>=(const Vector2& rhs)const
  {
    return this->GetSquaredMagnitude() >= rhs.GetSquaredMagnitude();
  }

  Vector2 Vector2::operator+(const Vector2& rhs)const
  {
    return Vector2(this->x + rhs.x, this->y + rhs.y);
  }
  Vector2 Vector2::operator-(const Vector2& rhs)const
  {
    return Vector2(this->x - rhs.x, this->y - rhs.y);
  }
  Vector2 Vector2::operator*(const Vector2& rhs)const
  {
    return Vector2(this->x * rhs.x, this->y * rhs.y);
  }
  Vector2 Vector2::operator/(const Vector2& rhs)const
  {
    return Vector2(this->x / rhs.x, this->y / rhs.y);
  }

  Vector2& Vector2::operator+=(const Vector2& rhs)
  {
    this->x += rhs.x;
    this->y += rhs.y;
    return *(this);
  }
  Vector2& Vector2::operator-=(const Vector2& rhs)
  {
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *(this);
  }
  Vector2& Vector2::operator*=(const Vector2& rhs)    
  {
    this->x *= rhs.x;
    this->y *= rhs.y;
    return *(this);
  }
  Vector2& Vector2::operator/=(const Vector2& rhs)
  {
    this->x /= rhs.x;
    this->y /= rhs.y;
    return *(this);
  }

  Vector2& Vector2::operator=(const float rhs)
  {
    this->x = rhs;
    this->y = rhs;
    return *(this); 
  }

  bool Vector2::operator==(const float rhs)const
  {
    return this->GetMagnitude() == rhs;
  }
  bool Vector2::operator!=(const float rhs)const
  {
    return this->GetMagnitude() != rhs;
  }
  bool Vector2::operator<(const float rhs)const
  {
    return this->GetMagnitude() < rhs;
  }
  bool Vector2::operator>(const float rhs)const
  {
    return this->GetMagnitude() > rhs;
  }
  bool Vector2::operator<=(const float rhs)const
  {
    return this->GetMagnitude() <= rhs;
  }
  bool Vector2::operator>=(const float rhs)const
  {
    return this->GetMagnitude() >= rhs;
  }

  Vector2 Vector2::operator+(const float rhs)const
  {
    return Vector2(this->x + rhs,this->y + rhs);
  }
  Vector2 Vector2::operator-(const float rhs)const
  {
    return Vector2(this->x - rhs,this->y - rhs);
  }
  Vector2 Vector2::operator*(const float rhs)const
  {
    return Vector2(this->x * rhs,this->y * rhs);
  }
  Vector2 Vector2::operator/(const float rhs)const
  {
    return Vector2(this->x / rhs,this->y / rhs);
  }

  Vector2& Vector2::operator+=(const float rhs)
  {
    this->x += rhs;
    this->y += rhs;
    return *(this);
  }
  Vector2& Vector2::operator-=(const float rhs)
  {
    this->x -= rhs;
    this->y -= rhs;
    return *(this);
  }
  Vector2& Vector2::operator*=(const float rhs)
  {
    this->x *= rhs;
    this->y *= rhs;
    return *(this);
  }
  Vector2& Vector2::operator/=(const float rhs)
  {
    this->x /= rhs;
    this->y /= rhs;
    return *(this);
  }

}
