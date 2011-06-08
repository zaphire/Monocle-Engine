#pragma once

#include <cpptest/cpptest-suite.h>

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
                
                void init();
                
                void remove(Asset *asset);
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
                void GetContentPath();
                
                Monocle::Assets *assets;
            };

        }
    }
}
