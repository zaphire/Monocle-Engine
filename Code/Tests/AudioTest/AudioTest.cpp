#include "AudioTest.h"
#include "../../Input.h"
#include "../../Collision.h"

#include <stdlib.h>
#include <sstream>

namespace AudioTest
{
	Text::Text(const std::string& text, FontAsset* font)
		: Entity(), font(font), text(text)
	{
	}

	void Text::Render()
	{
		Graphics::PushMatrix();
		Graphics::Translate(position);
		Graphics::SetBlend(BLEND_ALPHA);
		Graphics::SetColor(Color::white);
		Graphics::BindFont(font);

		Graphics::RenderText(*font, text, 0, 0);
		Graphics::PopMatrix();
	}

 

	/*
	**************************************************
	* G a m e S c e n e
	**************************************************
	*
	* Sets up the game's entities in Begin()
	* by creating a ball and two paddles
	* 
	* Handles messages sent to it indicating when the
	* ball has gone offside.
	* 
	*/

	void GameScene::Begin()
	{
		Debug::Log("AudioTest::GameScene::Begin()!");

		Scene::Begin();
		
		FontAsset* font = Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 25.0f);
        scText = new Text("Audio Test does nothing yet!", font);
        scText->position = Vector2(50, 50);
        Add(scText);
	}

	void GameScene::ReceiveNote(const std::string &note)
	{
	}

	
	void GameScene::Update()
	{
		Scene::Update();

		// do audiotest specific update
	}

	void GameScene::End()
	{
		Scene::End();
	}
}
