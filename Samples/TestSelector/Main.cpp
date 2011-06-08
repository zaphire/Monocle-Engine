//
//  Main.cpp
//  MonocleTests
//
//  Created by Josh Whelchel on 5/2/11.
//

#include "Monocle.h"
#include "AudioTest/AudioTest.h"
#include "Flash/Flash.h"
#include "Jumper/Jumper.h"
#include "LevelEditor/LevelEditorTest.h"
#include "Ogmo/Ogmo.h"
#include "Pong/Pong.h"
#include "PuppetTest/PuppetTest.h"

using namespace Monocle;

static Scene *nextScene = NULL;
static std::string newAssetPath = "";
static Game *pGame = NULL;

namespace TestSelector
{
    class GameScene : public Scene
    {
    public:
        void Begin()
        {
            AudioTest::Text *scText;
            
            const char *testNames[] = {
                "Audio",
                "Flash",
                "Jumper",
                "Ogmo",
                "Level Editor",
                "Pong",
                "Puppet Test"
            };
            
            Debug::Log("TestSelector::GameScene::Begin()!");
            
            Scene::Begin();

			FontAsset* font = Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 25.0f);
			scText = new AudioTest::Text("Please select a test to run:", font);
            scText->position = Vector2(50, 50);
            Add(scText);
            
            int y = 100;
            for (int i=0;i<sizeof(testNames)/sizeof(const char*);i++){
                scText = new AudioTest::Text(StringOf(i+1) + ". " + std::string(testNames[i]), font);
                scText->position = Vector2(80,y);
                Add(scText);
                y+= 30;
            }
        }
        
        void Update()
        {
            Scene::Update();
            
            nextScene = NULL;
            newAssetPath = "";
            
            if (Input::IsKeyPressed(KEY_1))
			{
                nextScene = new AudioTest::GameScene();
            }
            
            if (Input::IsKeyPressed(KEY_2))
			{
                newAssetPath = "Flash/";
                nextScene = new Flash::TestScene();
            }
            
            if (Input::IsKeyPressed(KEY_3))
			{
                newAssetPath = "Jumper/";
                nextScene = new Jumper::GameScene();
            }
            
            if (Input::IsKeyPressed(KEY_4))
			{
                newAssetPath = "Ogmo/";
                nextScene = new Ogmo::World();
            }
            
            if (Input::IsKeyPressed(KEY_5))
			{
                newAssetPath = "LevelEditorTest/";
                nextScene = new LevelEditorTest::LevelScene();
            }
            
            if (Input::IsKeyPressed(KEY_6))
			{
				nextScene = new Pong::GameScene();
			}

			if (Input::IsKeyPressed(KEY_7))
			{
				newAssetPath = "PuppetTest/";
				nextScene = new PuppetTest::TestScene();
			}

			if (nextScene)
			{
				Assets::SetContentPath(Platform::GetDefaultContentPath() + newAssetPath);
				pGame->SetScene(nextScene);
				
            }
        }
    };
};

int main(void)
{
	pGame = new Game();
	pGame->SetScene(new TestSelector::GameScene());
	pGame->Main();
    delete pGame;
    
	return 0;
}

