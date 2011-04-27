#include "AudioTest.h"

#include <stdlib.h>
#include <sstream>

#include "../../Audio/oggvorbis/OggDecoder.h"

namespace AudioTest
{
    OggDecoder od;
    AudioDecodeData *add;
    ChannelStream *cs;
    
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

    Waveform::Waveform( AudioDeck *deck )
    {
        this->deck = deck;
    }
    
    void Waveform::Render()
    {
        Graphics::PushMatrix();
        //Graphics::Translate(position);
        
        Graphics::BindTexture(NULL);
        Graphics::SetBlend(BLEND_ADDITIVE);
        Graphics::SetColor(Color::white);
        
        int i;
        
        Vector2 lastpos, thispos;
        
        float midy = Graphics::GetVirtualHeight()/2.0;
        float xstep = Graphics::GetVirtualWidth()/576.0;
        
        lastpos.x = 0.0;
        lastpos.y = midy;
        thispos.x = thispos.y = 0.0;

        for (i=0;i<576;i++)
        {
            thispos.y = deck->vis->fWaveform[0][i] + midy; //-128 to +128
            thispos.x = xstep*i;
            
            if (deck->vis->bClear) thispos.y = midy;
            
            Graphics::RenderLine(lastpos, thispos);
            
            lastpos = thispos;
        }
        
        Graphics::PopMatrix();
    }

	/*
	**************************************************
	* G a m e S c e n e
	**************************************************
	*
	* Sets up the game's entities in Begin()
	* 
	*/

	void GameScene::Begin()
	{
		Debug::Log("AudioTest::GameScene::Begin()!");

		Scene::Begin();
        
        AudioAsset *virtSong = Assets::RequestAudio("AudioTest/virt-robo.ogg");
        add = od.RequestData( *virtSong );
		
		FontAsset* font = Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 25.0f);
        scText = new Text("Audio is " + StringOf(add->total) + "ms", font);
        scText->position = Vector2(50, 50);
        Add(scText);
        
        // Make the deck, and it starts playing... (we need a play())
        deck = Audio::NewDeck(add);
        deck->SetFadeIn(1000);
        
        Waveform* wave = new Waveform(deck);
        Add(wave);

	}

	void GameScene::ReceiveNote(const std::string &note)
	{
	}

	
	void GameScene::Update()
	{
		Scene::Update();
        
        scText->SetText("Time is " + StringOf(deck->cs->GetTotalPlayTime()) + " / " + StringOf(add->total));
        
        if (Input::IsKeyPressed(KEY_P)){
            deck->Play();
        }
        
        if (Input::IsKeyPressed(KEY_S)){
            deck->Pause();
        }
        
        if (Input::IsKeyPressed(KEY_F)){
            deck->PauseWithFade(1000);
        }
        
        if (Input::IsKeyPressed(KEY_V)){
            deck->ResumeWithFade(1000);
        }
	}

	void GameScene::End()
	{
		Scene::End();
        
        delete deck;
        delete add;
	}

}
