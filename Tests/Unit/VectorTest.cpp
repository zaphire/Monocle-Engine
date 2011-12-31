#include <cpptest/cpptest-assert.h>

#include "VectorTest.h"
#include <Vector2.h>
#include <Vector3.h>
#include <cmath>

namespace Monocle
{
	namespace Test
	{
		namespace Unit
		{
			Vector::Vector()
			{
				TEST_ADD(Vector::Vec2Magnitude);
				TEST_ADD(Vector::Vec2Normalization);
				TEST_ADD(Vector::Vec2Perpendicular);
				TEST_ADD(Vector::Vec2Clamp);
				TEST_ADD(Vector::Vec2Range);
				TEST_ADD(Vector::Vec2Dot);
				TEST_ADD(Vector::Vec2Cross);
				TEST_ADD(Vector::Vec2Reflect);
				TEST_ADD(Vector::Vec2Angle);
			}
			
			void Vector::setup()
			{
				vec2 = new Monocle::Vector2();
				vec3 = new Monocle::Vector3();
			}
			
			void Vector::tear_down()
			{
				delete vec2;
				delete vec3;
			}
			
			void Vector::Vec2Magnitude()
			{
				vec2->Set(1,1);
				TEST_ASSERT( vec2->GetSquaredMagnitude() == 2 );
				
				vec2->Set(3,4);
				TEST_ASSERT( vec2->GetMagnitude() == 5);
				TEST_ASSERT( vec2->GetSquaredMagnitude() == 25);
			}
			
			void Vector::Vec2Normalization()
			{
				vec2->Set(3,4);
				Monocle::Vector2 nrm = vec2->GetNormalized();
				TEST_ASSERT(nrm.x == 0.6f);
				TEST_ASSERT(nrm.y == 0.8f);
				
				vec2->Normalize();
				TEST_ASSERT(vec2->x == 0.6f);
				TEST_ASSERT(vec2->y == 0.8f);
			}
			
			void Vector::Vec2Perpendicular()
			{
				vec2->Set(3,4);
				Vector2 left = vec2->GetPerpendicularLeft();
				Vector2 right = vec2->GetPerpendicularRight();
				
				TEST_ASSERT( left.x == -4 );
				TEST_ASSERT( left.y == 3 );
				
				TEST_ASSERT( right.x == 4 );
				TEST_ASSERT( right.y == -3 );
			}
			
			void Vector::Vec2Clamp()
			{
				vec2->Set(3,4);
				vec2->Clamp(3);
				
				TEST_ASSERT( vec2->GetMagnitude() == 3 );
			}
			
			void Vector::Vec2Range()
			{
				vec2->Set(3,4);
				
				TEST_ASSERT( vec2->IsInRange(10) );
			}
			
			void Vector::Vec2Dot()
			{
				vec2->Set(3,4);
				Vector2 other;
				
				other.Set(1,1);
				TEST_ASSERT( vec2->Dot(other) == 7.0f );
				TEST_ASSERT( other.Dot(*vec2) == 7.0f );
				
				other.Set(3,4);
				TEST_ASSERT( vec2->Dot(other) == 25.0f );
				
				other.Set(1000,1000);
				TEST_ASSERT( vec2->Dot(other) == 7000.0f );
				
				other.Set(-1,-1);
				TEST_ASSERT( vec2->Dot(other) == -7.0f );
			}
			
			void Vector::Vec2Cross()
			{
				vec2->Set(3,4);
				Vector2 other;
				
				other.Set(1,1);
				TEST_ASSERT( vec2->Cross(other) == -1.0f );
				TEST_ASSERT( other.Cross(*vec2) == 1.0f );
				
				other.Set(3,4);
				TEST_ASSERT( vec2->Cross(other) == 0.0f );
				TEST_ASSERT( other.Cross(*vec2) == 0.0f );
				
				other.Set(1000,1000);
				TEST_ASSERT( vec2->Cross(other) == -1000.0f );
				TEST_ASSERT( other.Cross(*vec2) == 1000.0f );
				
				other.Set(-1,-1);
				TEST_ASSERT( vec2->Cross(other) == 1.0f );
				TEST_ASSERT( other.Cross(*vec2) == -1.0f );
			}
			
			void Vector::Vec2Reflect()
			{
				vec2->Set(3,4);
				Vector2 other;
				
				other.Set(0,1);
				vec2->Reflect(other);
				TEST_ASSERT( *vec2 == Vector2(3,-4) );
				
				vec2->Set(3,4);
				other.Set(1,0);
				vec2->Reflect(other);
				TEST_ASSERT( *vec2 == Vector2(-3,4) );
			}
			
			void Vector::Vec2Angle()
			{
				vec2->Set(1,1);
				TEST_ASSERT( vec2->GetAngleDegrees() == 45.0f );
				TEST_ASSERT( vec2->GetAngleRadians() == 0.785398163397448309615660845f );
				
				vec2->Set(1,std::tan(1));
				std::cout << vec2->GetAngleDegrees() << std::endl;
				TEST_ASSERT( vec2->GetAngleRadians() == 1.0f );
			}
		}
	}
}
