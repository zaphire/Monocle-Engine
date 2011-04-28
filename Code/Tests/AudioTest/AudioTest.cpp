#include "AudioTest.h"

#include <stdlib.h>
#include <sstream>
#include <math.h>

#include "../../Audio/oggvorbis/OggDecoder.h"

namespace AudioTest
{
    AudioDeck *deck1;
    AudioDeck *deck2;
    
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

    Waveform::Waveform( AudioDeck *deck, int type )
    {
        this->deck = deck;
        this->type = type;
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
        
        if (type == 0)
        {
        
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
            
        } else {
            // Do the spectrum blocks
            
            float midy = Graphics::GetVirtualHeight()/2.0;
            float xstep = Graphics::GetVirtualWidth()/400.0;
            float val;
            float timeR = sinf(deck->cs->GetTotalPlayTime()/5000.0);
            float timeG = sinf(deck->cs->GetTotalPlayTime()/900.0)/2.0 + 0.5;
            float timeB = sinf(deck->cs->GetTotalPlayTime()/10000.0);
            
            lastpos.x = 0.0;
            lastpos.y = midy;
            thispos.x = thispos.y = 0.0;
            
            for (i=0;i<400;i++)
            {
                val = deck->vis->fSpectrum[0][i];
                
                Graphics::SetColor(Color(val*timeR, val*timeG, val*timeB, 1.0));
                Graphics::RenderLineRect( thispos.x, midy, xstep, val*100.0);
                
                thispos.x += xstep;
            }
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
		
		FontAsset* font = Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 25.0f);
        scText = new Text("Dual-worlds Audio Demo", font);
        scText->position = Vector2(50, 50);
        Add(scText);
        
        // Make the deck, and it starts playing... (we need a play())
        deck1 = Audio::NewDeck(Assets::RequestAudio("AudioTest/City01.g2m",true,"hellogirl"));
        deck2 = Audio::NewDeck(Assets::RequestAudio("AudioTest/City01Hell.g2m",false,"hellogirl"));
        
        deck2->volume = 0.0;
        
        deck1->Play();
        deck2->Play();
        
        Waveform* wave = new Waveform(deck1,1);
        Add(wave);

        Waveform* wave2 = new Waveform(deck2,1);
        Add(wave2);
	}

	void GameScene::ReceiveNote(const std::string &note)
	{
	}

	
	void GameScene::Update()
	{
		Scene::Update();
        
        scText->SetText("Time is " + StringOf(deck1->cs->GetTotalPlayTime()) + " / " + StringOf(deck1->GetTotalLength()));
        
        if (Input::IsKeyPressed(KEY_P)){
            deck1->Play();
        }
        
        if (Input::IsKeyPressed(KEY_S)){
            deck1->Pause();
        }
        
        if (Input::IsKeyPressed(KEY_F)){
            deck1->PauseWithFade(1000);
        }
        
        if (Input::IsKeyPressed(KEY_V)){
            deck1->ResumeWithFade(1000);
        }
	}

	void GameScene::End()
	{
		Scene::End();
        
        delete deck1;
        delete deck2;
	}

}
