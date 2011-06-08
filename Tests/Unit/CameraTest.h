#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    class Camera;
    
    namespace Test
    {
        namespace Unit
        {            
            //Dependencies: Assets, Asset
            class Camera: public ::Test::Suite
            {
            public:
                Camera();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
				void Visibility();
				void VisibleLayers();
                
                Monocle::Camera *camera;
            };

        }
    }
}
