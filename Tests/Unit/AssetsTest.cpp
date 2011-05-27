#include "AssetsTest.h"

namespace Monocle
{
    class Game;
    class Asset;
    
    namespace Test
    {
        namespace Unit
        {
            void DerivedAssets::AssertInstance()
            {
                
            }
            
            Assets::Assets()
            {
                TEST_ADD(Assets::GetContentPath)
                TEST_ADD(Assets::Init)
                TEST_ADD(Assets::RequestTexture)
                TEST_ADD(Assets::RequestFont)
                TEST_ADD(Assets::RequestAudio)
            }
            
            void Assets::setup()
            {
                assets = new DerivedAssets();
            }
            
            void Assets::tear_down()
            {
                delete assets;
                assets = NULL;
            }
            
            void Assets::Init()
            {
                TEST_ASSERT(assets->GetContentPath() == Monocle::Platform::GetDefaultContentPath());
            }
            
            void Assets::RequestAudio()
            {
                AudioAsset *audio = assets->RequestAudio("doesntexit.wav", false);
                TEST_ASSERT( audio == NULL );
                audio = assets->RequestAudio("AudioTest/Coin.wav", false);
                TEST_ASSERT( audio != NULL );
            }
            
            void Assets::RequestFont()
            {
                
            }
            
            void Assets::RequestTexture()
            {
                
            }
        }
    }
}
