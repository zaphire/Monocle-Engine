#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    class FontAsset;
    
    namespace Test
    {
        namespace Unit
        {            
            //Dependencies: Assets, Asset
            class FontAsset: public ::Test::Suite
            {
            public:
                FontAsset();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Type();
                void GetGlyphData();
                void GetTextWidth();
                void GetTextHeight();
                
                Monocle::FontAsset *font;
            };

        }
    }
}
