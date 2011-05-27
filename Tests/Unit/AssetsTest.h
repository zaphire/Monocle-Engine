#pragma once

#include <cpptest-suite.h>

#include <Assets.h>

namespace Monocle
{
    class Game;
    class Assets;
    
    namespace Test
    {
        namespace Unit
        {
            class DerivedAssets : public Monocle::Assets
            {
            public:
                DerivedAssets() : Monocle::Assets() {}
                
                void AssertInstance();
            };
            
            class Assets: public ::Test::Suite
            {
            public:
                Assets();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Init();
                void RequestTexture();
                void RequestFont();
                void RequestAudio();
                
                Monocle::Assets *assets;
            };

        }
    }
}
