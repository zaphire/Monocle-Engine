#include "AssetsTest.h"
#include "../Tests.h"

#include <cpptest/cpptest-assert.h>

#include <Platform.h>

#include <Game.h>
#include <FontAsset.h>
#include <Audio/AudioAsset.h>
#include <TextureAsset.h>

namespace Monocle
{
    class Game;
    class Asset;
    
    namespace Test
    {
        namespace Unit
        {
            void DerivedAssets::init()
            {
                Monocle::Assets::Init();
            }
            
            void DerivedAssets::remove(Asset *asset)
            {
                RemoveAsset(asset);
            }
            
            Assets::Assets()
            {
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
                static_cast<DerivedAssets*>(assets)->init();
                TEST_ASSERT(assets->GetContentPath() == Monocle::Platform::GetDefaultContentPath());
            }
            
            void Assets::RequestAudio()
            {
                assets->SetContentPath( CONTENT_PATH );
                AudioAsset *audio = assets->RequestAudio("doesntexist.wav", false);
                //File does not exist, return value should be null
                TEST_ASSERT( audio == NULL );
                audio = assets->RequestAudio("AudioTest/Coin.wav", false);
                //audio exists, return value should be non-null
                TEST_ASSERT( audio != NULL );
                
                AudioAsset *newaudio = assets->RequestAudio("AudioTest/Coin.wav", false);
                //requested the same file, should return identical pointer
                TEST_ASSERT( audio == newaudio );
                
                newaudio = assets->RequestAudio("AudioTest/Powerup.wav", false);
                TEST_ASSERT( newaudio != NULL );
                //new file, should not be the same pointer
                TEST_ASSERT( audio != newaudio );
                
                delete audio;
            }
            
            void Assets::RequestFont()
            {
                assets->SetContentPath( "../../../Content/" );
                FontAsset *font = assets->RequestFont("doesntexist.ttf", 12);
                TEST_ASSERT( font == NULL );
                font = assets->RequestFont("AudioTest/LiberationSans-Regular.ttf", 12);
                TEST_ASSERT( font != NULL );
                
                FontAsset *newfont = assets->RequestFont("AudioTest/LiberationSans-Regular.ttf", 14);
                TEST_ASSERT( newfont != NULL );
                TEST_ASSERT( font != newfont );
                
                newfont = assets->RequestFont("AudioTest/LiberationSans-Regular.ttf", 12);
                TEST_ASSERT( font != NULL );
                TEST_ASSERT( font == newfont );
                
                delete font;
                delete newfont;
                font = NULL;
                newfont = NULL;
            }
            
            void Assets::RequestTexture()
            {
                Game *g = new Game();
                
                Monocle::Assets::SetContentPath( "../../../Content/" );
                
                TextureAsset *tex = Monocle::Assets::RequestTexture("doesntexist.png");
                TEST_ASSERT( tex == NULL );
                
                tex = Monocle::Assets::RequestTexture("Ogmo/player.png");
                TEST_ASSERT( tex != NULL );
                
                TextureAsset *newtex = Monocle::Assets::RequestTexture("Ogmo/player.png");
                TEST_ASSERT( tex == newtex )
                
                delete tex;
                tex = NULL;
            }
        }
    }
}
