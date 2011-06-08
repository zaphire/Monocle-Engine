#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    class Vector2;
    class Vector3;
    
    namespace Test
    {
        namespace Unit
        {
            //Dependencies: Assets, Asset
            class Vector : public ::Test::Suite
            {
            public:
                Vector();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
				void Vec2Magnitude();
				void Vec2Normalization();
				void Vec2Perpendicular();
				void Vec2Clamp();
				void Vec2Range();
				void Vec2Dot();
				void Vec2Cross();
				void Vec2Reflect();
				void Vec2Angle();
                
                Monocle::Vector2 *vec2;
                Monocle::Vector3 *vec3;
            };

        }
    }
}
