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

    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 down;
    static const Vector2 up;
    static const Vector2 left;
    static const Vector2 right;

    Vector2();
    Vector2(const Vector2 &other);    
    Vector2(const float x, const float y);

    static Vector2 Random();    
    static Vector2 RandomDeviant(const Vector2& observed, float angle);

    void Set(const float x, const float y);
    void Normalize();
    void Clamp(float max);   
    
    bool IsUnitLength()const;
    bool IsZeroLength()const;
    bool IsInRange(const float range)const;
    float Dot(const Vector2& other)const;
    float Cross(const Vector2& other)const;
    float SquaredDistance(const Vector2& other)const;
    float Distance(const Vector2& other)const;
    Vector2 MidPoint(const Vector2& other)const;
    Vector2 LERP(const Vector2& to, const float percentage) const;

    float GetSquaredMagnitude()const;
    float GetMagnitude()const;
    Vector2 GetNormalized()const;
    Vector2 GetPerpendicularLeft()const;
    Vector2 GetPerpendicularRight()const;    
    Vector2 xx()const;
    Vector2 yy()const;
    Vector2 yx()const;

    // Operators

    const float operator[](unsigned char i)const; // gets vector element at [0'x'] or [1'y']
    float& operator[](unsigned char i); // sets vector element at [0'x'] or [1'y']

    Vector2 operator-()const; // force x and y negative.
    Vector2 operator+()const; // force x and y posative.

    Vector2& operator=(const Vector2& rhs); 

    bool operator==(const Vector2& rhs)const;
    bool operator!=(const Vector2& rhs)const;
    bool operator<(const Vector2& rhs)const;
    bool operator>(const Vector2& rhs)const;
    bool operator<=(const Vector2& rhs)const;
    bool operator>=(const Vector2& rhs)const;

    Vector2 operator+(const Vector2& rhs)const;
    Vector2 operator-(const Vector2& rhs)const;
    Vector2 operator*(const Vector2& rhs)const;
    Vector2 operator/(const Vector2& rhs)const;

    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);
    Vector2& operator*=(const Vector2& rhs);
    Vector2& operator/=(const Vector2& rhs);

    Vector2& operator=(const float rhs);

    bool operator==(const float rhs)const;
    bool operator!=(const float rhs)const;
    bool operator<(const float rhs)const;
    bool operator>(const float rhs)const;
    bool operator<=(const float rhs)const;
    bool operator>=(const float rhs)const;

    Vector2 operator+(const float rhs)const;
    Vector2 operator-(const float rhs)const;
    Vector2 operator*(const float rhs)const;
    Vector2 operator/(const float rhs)const;

    Vector2& operator+=(const float rhs);
    Vector2& operator-=(const float rhs);
    Vector2& operator*=(const float rhs);
    Vector2& operator/=(const float rhs);

    // friends.

    friend bool operator==(const float lhs, const Vector2& rhs)
    {
      return lhs == rhs.GetMagnitude();
    }
    friend bool operator!=(const float lhs, const Vector2& rhs)
    {
      return lhs != rhs.GetMagnitude();
    }
    friend bool operator<(const float lhs, const Vector2& rhs)
    {
      return lhs < rhs.GetMagnitude();
    }
    friend bool operator>(const float lhs, const Vector2& rhs)
    {
      return lhs > rhs.GetMagnitude();
    }
    friend bool operator<=(const float lhs, const Vector2& rhs)
    {
      return lhs <= rhs.GetMagnitude();
    }
    friend bool operator>=(const float lhs, const Vector2& rhs)
    {
      return lhs >= rhs.GetMagnitude();
    }

    friend Vector2 operator+(const float lhs, const Vector2& rhs)
    {
      return Vector2(lhs + rhs.x,lhs + rhs.y);
    }
    friend Vector2 operator-(const float lhs, const Vector2& rhs)
    {
      return Vector2(lhs - rhs.x,lhs - rhs.y);
    }
    friend Vector2 operator*(const float lhs, const Vector2& rhs)
    {
      return Vector2(lhs * rhs.x,lhs * rhs.y);
    }
    friend Vector2 operator/(const float lhs, const Vector2& rhs)
    {
      return Vector2(lhs / rhs.x,lhs / rhs.y);
    }

    friend float& operator+=(float& lhs, const Vector2& rhs)
    {
      lhs += rhs.GetMagnitude();
      return lhs;
    }
    friend float& operator-=(float& lhs, const Vector2& rhs)
    {
      lhs -= rhs.GetMagnitude();
      return lhs;
    }
    friend float& operator*=(float& lhs, const Vector2& rhs)
    {
      lhs *= rhs.GetMagnitude();
      return lhs;
    }
    friend float& operator/=(float& lhs, const Vector2& rhs)
    {
      lhs /= rhs.GetMagnitude();
      return lhs;
    }
  };

}
