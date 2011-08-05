#include "AudioTest.h"

#include <stdlib.h>
#include <sstream>
#include <math.h>

//#define MONOCLE_AUDIOTEST_CROSSFADING
#define MONOCLE_AUDIOTEST_LOOPER
//#define MONOCLE_AUDIOTEST_SFX

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

    Waveform::Waveform( AudioDeck *deck, int type )
    {
        this->deck = deck;
        this->type = type;
    }
    
    void Waveform::Render()
    {
        if (deck->IsMuted())
            return;
        
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
//                thispos.y = deck->vis->fWaveform[0][i] + midy; //-128 to +128
                thispos.y = deck->GetVisWaveform(i)*128.0 + midy;
                thispos.x = xstep*i;
                
                if (deck->vis && deck->vis->bClear) thispos.y = midy;
                
                Graphics::RenderLine(lastpos, thispos);
                
                lastpos = thispos;
            }
            
        } else {
            // Do the spectrum blocks
            
            if (deck->vis && deck->vis->bClear){
                Graphics::SetColor(Color::white);
                Graphics::RenderTriangle(400.0);
                Graphics::PopMatrix();
                return;
            }
            
            float midy = Graphics::GetVirtualHeight()/2.0;
			float xstep = Graphics::GetVirtualWidth()/400.0;
			float val;

			float timeR = sinf(deck->GetCurrentTime()/5000.0);
            float timeG = sinf(deck->GetCurrentTime()/900.0)/2.0 + 0.5;
            float timeB = sinf(deck->GetCurrentTime()/10000.0);
            
            lastpos.x = 0.0;
            lastpos.y = midy;
            thispos.x = thispos.y = 0.0;
            
            for (i=0;i<400;i++)
            {
//                val = deck->vis->fSpectrum[0][i];
				val = deck->GetVisSpectrum(i)*10.0;

				Graphics::SetColor(Color(val*timeR, val*timeG, val*timeB, 1.0));
				Graphics::RenderLineRect( thispos.x, midy, xstep, val*100.0);
                
                thispos.x += xstep;
            }
        }
        
        Graphics::PopMatrix();
    }
    
#ifdef MONOCLE_AUDIOTEST_SFX
    
    int cnt = 0;
    
    /*
     **************************************************
     * G a m e S c e n e
     **************************************************
     *
     * Sets up the game's entities in Begin()
     * 
     */
    
    AudioAsset *laser, *coin;
    
	void GameScene::Begin()
	{
		Debug::Log("AudioTest::GameScene::Begin()!");
        
		Scene::Begin();
		
		FontAsset* font = Assets::RequestFont("AudioTest/LiberationSans-Regular.ttf", 25.0f);
        scText = new Text("L: Laser, C: Coin", font);
        scText->position = Vector2(50, 50);
        Add(scText);
        
        laser = Assets::RequestAudio("AudioTest/Laser.wav");
        coin = Assets::RequestAudio("AudioTest/Coin.wav");
        
        if (!laser || !coin)
            return Platform::ErrorShutdown("Couldn't load Laser.wav or Coin.wav");
	}
    
	void GameScene::ReceiveNote(const std::string &note)
	{
	}
    
	
	void GameScene::Update()
	{
		Scene::Update();
        
        if (Input::IsKeyPressed(KEY_L) || Input::IsTouchBeginning()){
            cnt++;
            laser->Play();
        }
        
        if (Input::IsKeyPressed(KEY_C) || Input::IsTouchEnding()){
            cnt++;
            coin->Play();
        }
        
        scText->SetText("L: Laser, C: Coin cnt: " + StringOf(cnt));
	}
    
	void GameScene::End()
	{
		Scene::End();
        
        delete laser;
        delete coin;
	}
    
#endif
    
#ifdef MONOCLE_AUDIOTEST_LOOPER
    
    AudioDeck *deck;
    
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
//#ifdef MONOCLE_IOS
//        deck = Audio::NewDeck( Assets::RequestAudio("AudioTest/pgl.caf") );
//#else
        deck = Audio::NewDeck( Assets::RequestAudio("AudioTest/ShortLoop.ogg", true) );
//#endif
        
        if (!deck)
            return Platform::ErrorShutdown("Couldn't load ShortLoop.ogg");
    
        deck->EnableVis();
        deck->Play();
        deck->SetLoops(0);
        deck->SetFadeOut(500);
        
        Waveform* wave = new Waveform(deck,1);
        Add(wave);
        
        wave = new Waveform(deck,0);
        Add(wave);
	}
    
	void GameScene::ReceiveNote(const std::string &note)
	{
	}
    
	
	void GameScene::Update()
	{
		Scene::Update();
        
        std::string looping = "off";
        if (deck->LoopsRemaining()==-1) looping = "on";
        
        scText->SetText("Looping " + looping + " (L), pos is " + StringOf(deck->GetCurrentTime()) + " / " + StringOf(deck->GetTotalLength()) + "  | "+ StringOf(deck->GetChannelStream()->IsPlaying()));
        
        if (Input::IsKeyPressed(KEY_S))
            deck->Seek(500);
        
        if (Input::IsKeyPressed(KEY_L) || Input::IsTouchBeginning()){
            if (deck->LoopsRemaining()==-1)
                deck->SetLoops(1);
            else
                deck->SetLoops(0);
            
            deck->Play();
        }
	}
    
	void GameScene::End()
	{
		Scene::End();
        
        delete deck;
	}
    
#endif
    
#ifdef MONOCLE_AUDIOTEST_CROSSFADING
    
    AudioDeck *deck1;
    AudioDeck *deck2;

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
		//deck1 = Audio::NewDeck(Assets::RequestAudio("AudioTest/City01.g2m",true,"hellogirl"));
        deck1 = Audio::NewDeck(Assets::RequestAudio("AudioTest/City01.g2m", true, "hellogirl"));
		deck2 = Audio::NewDeck(Assets::RequestAudio("AudioTest/City01Hell.g2m",true,"hellogirl"));
        
        if (!deck1 || !deck2)
            return Platform::ErrorShutdown("Couldn't Load City01.g2m or City01Hell.g2m");
        
        deck2->Mute();

		deck1->EnableVis();
        deck2->EnableVis();
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
        
        scText->SetText("Press F to crossfade. " + StringOf(deck1->GetCurrentTime()) + " / " + StringOf(deck1->GetTotalLength()));
        
        if (Input::IsKeyPressed(KEY_F) || Input::IsTouchBeginning())
		{
			if (deck2->IsMuted()){
				deck2->UnmuteWithFade(1000);
				deck1->MuteWithFade(1000);
			} else {
                deck1->UnmuteWithFade(1000);
                deck2->MuteWithFade(1000);
            }
        }
	}

	void GameScene::End()
	{
		Scene::End();
        
        delete deck1;
        delete deck2;
	}
    
#endif

}
