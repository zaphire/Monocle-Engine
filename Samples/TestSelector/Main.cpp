//
//  Main.cpp
//  MonocleTests
//
//  Created by Josh Whelchel on 5/2/11.
//

#include "Monocle.h"
#include "TestSelector.h"
#include "../AudioTest/AudioTest.h"
#include "../Flash/Flash.h"
#include "../Jumper/Jumper.h"
#include "../LevelEditor/LevelEditorTest.h"
#include "../Ogmo/Ogmo.h"
#include "../Pong/Pong.h"
#include "../PuppetTest/PuppetTest.h"

using namespace Monocle;

static Scene *nextScene = NULL;
static std::string newAssetPath = "";
static Game *pGame = NULL;

namespace TestSelector
{
    GameScene::GameScene()
    {
        
    }
    
    void GameScene::Begin()
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
        
    void GameScene::Update()
    {
        static int yOff = 30;
        Scene::Update();
        
        nextScene = NULL;
        newAssetPath = "";
        
        Touch *t =Input::IsTouchEnding();
        if (t){
            Debug::Log("Touch ended at ("+StringOf(t->position.x)+","+StringOf(t->position.y)+")");
        }
        
        if (Input::IsKeyPressed(KEY_1) || Input::IsTouchInRect(Vector2(0,100-yOff),Vector2(500,129-yOff),TOUCH_PHASE_ENDED))
        {
            nextScene = new AudioTest::GameScene();
        }
        else
        if (Input::IsKeyPressed(KEY_2) || Input::IsTouchInRect(Vector2(0,130-yOff),Vector2(500,159-yOff),TOUCH_PHASE_ENDED))
        {
            newAssetPath = "Flash/";
            nextScene = new Flash::TestScene();
        }
        else
        if (Input::IsKeyPressed(KEY_3) || Input::IsTouchInRect(Vector2(0,160-yOff),Vector2(500,189-yOff),TOUCH_PHASE_ENDED))
        {
            newAssetPath = "Jumper/";
            nextScene = new Jumper::GameScene();
        }
        else
        if (Input::IsKeyPressed(KEY_4) || Input::IsTouchInRect(Vector2(0,190-yOff),Vector2(500,219-yOff),TOUCH_PHASE_ENDED))
        {
            newAssetPath = "Ogmo/";
            nextScene = new Ogmo::World();
        }
        else
        if (Input::IsKeyPressed(KEY_5) || Input::IsTouchInRect(Vector2(0,220-yOff),Vector2(500,249-yOff),TOUCH_PHASE_ENDED))
        {
            newAssetPath = "LevelEditorTest/";
            nextScene = new LevelEditorTest::LevelScene();
        }
        else
        if (Input::IsKeyPressed(KEY_6) || Input::IsTouchInRect(Vector2(0,250-yOff),Vector2(500,279-yOff),TOUCH_PHASE_ENDED))
        {
            nextScene = new Pong::GameScene();
        }
        else
        if (Input::IsKeyPressed(KEY_7) || Input::IsTouchInRect(Vector2(0,280-yOff),Vector2(500,310-yOff),TOUCH_PHASE_ENDED))
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
}

#ifndef MONOCLE_IOS

int main(void)
{
	pGame = new Game();
	pGame->SetScene(new TestSelector::GameScene());
	pGame->Main();
    delete pGame;
    
	return 0;
}

#endif
