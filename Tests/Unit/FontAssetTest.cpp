#include <cpptest/cpptest-assert.h>

#include "FontAssetTest.h"
#include "../Tests.h"

#include <FontAsset.h>
#include <Rect.h>
#include <Assets.h>

#include <Graphics.h>
#include <Platform.h>

#include <Game.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
            FontAsset::FontAsset()
            {
                TEST_ADD(FontAsset::Type)
                TEST_ADD(FontAsset::GetGlyphData)
            }
            
            void FontAsset::setup()
            {
                Game g;
                
                Monocle::Assets::SetContentPath(CONTENT_PATH);
                font = Monocle::Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 24);
            }
            
            void FontAsset::tear_down()
            {
                delete font;
                font = NULL;
            }
            
            void FontAsset::Type()
            {
                TEST_ASSERT(font->type == ASSET_FONT);
            }
            
            void FontAsset::GetGlyphData()
            {
                for(int c = 32; c < 126; c++)
                {
                    Monocle::Rect glyphbounds, texcoords;
                    float x,y;
                    font->GetGlyphData(c, &x, &y, glyphbounds, texcoords);
                    
                    //std::cout << "Rect: ( loc: (" << glyphbounds.topLeft.x << "," << glyphbounds.topLeft.y << ") dim: (" << (glyphbounds.bottomRight.x - glyphbounds.topLeft.x) << "," << (glyphbounds.bottomRight.y - glyphbounds.topLeft.y) << ") )" << std::endl;
                }
            }
        }
    }
}
