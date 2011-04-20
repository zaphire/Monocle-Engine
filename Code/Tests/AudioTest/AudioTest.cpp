#include "AudioTest.h"

#include <stdlib.h>
#include <sstream>

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
        
        // This is all a bit complicated for now, it'll all be pushed to the audio class later.
        
        AudioAsset *audio = Assets::RequestAudio("AudioTest/virt-robo.ogg", od);
        add = od.RequestData( *audio );
        
        cs = new ChannelStream();
        cs->open(add->ch,add->bit,add->samplerate);
        
        // Start by filling the buffer
        unsigned int size;
        
        // Fill First Buffers
        unsigned char *data = cs->getStaticBuffer(&size);
        size = od.Render( size, (void*)data, *add);
        cs->lockNumberedBuffer(size, 0);
        
        data = cs->getStaticBuffer(&size);
        size = od.Render( size, (void*)data, *add);
        cs->lockNumberedBuffer(size, 1);
        
        data = cs->getStaticBuffer(&size);
        size = od.Render( size, (void*)data, *add);
        cs->lockNumberedBuffer(size, 2);
        
        cs->play();
	}

	void GameScene::ReceiveNote(const std::string &note)
	{
	}

	
	void GameScene::Update()
	{
		Scene::Update();

		// do audiotest specific update
        ThinkAudio();
	}

	void GameScene::End()
	{
		Scene::End();
        
        od.FreeDecoderData(*add);

        cs->close();
        delete cs;
	}
    
    void GameScene::ThinkAudio()
    {
        // ALL BELOW IS AUDIO GENERATION STUFF...
		int buffers_to_fill = cs->needsUpdate();
        unsigned int size;
		
		while (buffers_to_fill--)
		{
			unsigned char *data;
			
			data = cs->getBuffer(&size);
			
            size = od.Render(size,(void*)data,*add);
			
			cs->lockBuffer(size);
			
            /*			if (playbackPos >= targetLength)
             break;*/
		}
    }
}
