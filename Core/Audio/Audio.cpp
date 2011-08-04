//
//  Audio.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include <string>
#include <algorithm>
#include <vector>

#include "Audio.h"

#include "../Debug.h"
#include "../MonocleToolkit.h"

#ifdef MONOCLE_AUDIO_OGG
#include "Decoders/OggDecoder.h"
#endif
#include "Decoders/WaveDecoder.h"

namespace Monocle {
   
    Audio *Audio::instance = NULL;
    static std::map<std::string, makeDecoderFunc> *decoderMap = NULL;
    
    AudioDecoder *makeOggFunc(AudioAsset*);
    AudioDecoder *makeWaveFunc(AudioAsset*);
    
    void Audio::Init()
    {
        Debug::Log("Audio::Init...");
            
        ChannelStream::Init();
        
        // Register decoders
#ifdef MONOCLE_AUDIO_OGG
        RegisterDecoder(makeOggFunc, "ogg g2m");
#endif
        RegisterDecoder(makeWaveFunc, "wav wave");
            
        Debug::Log("...Done");

    }
    
    AudioDeck *Audio::NewDeck( AudioDecoder *decoder, bool freeDecoderWithDeck )
    {
        if (!instance->firstDeck)
        {
            instance->firstDeck = new AudioDeck( &instance->firstDeck, decoder, freeDecoderWithDeck );
            return instance->firstDeck;
        }
        else
        {
            // New Deck must be built on the last deck
            AudioDeck *lastDeck = LastDeck();
            lastDeck->nextDeck = new AudioDeck(lastDeck, decoder,freeDecoderWithDeck);
            return lastDeck->nextDeck;
        }
    }
    
    AudioDeck *Audio::NewDeck(Monocle::AudioAsset *audioAsset)
    {
        if (!audioAsset)
            return NULL;
        
        std::string ext = audioAsset->GetExtension();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        if(decoderMap[0].find(ext) == decoderMap[0].end())
        {
            Debug::Log("AUDIO: Could not find a decoder for the file: " + audioAsset->GetName());
            return 0;
        }
        
        makeDecoderFunc mf = decoderMap[0][ext];
        AudioDecoder *decoder = mf( audioAsset );
        
        if (!decoder){
            Debug::Log("AUDIO: Cannot decode file: " + audioAsset->GetName());
            return 0;
        }
        
        return Audio::NewDeck( decoder, true );
    }
    
    // Grab the last deck in the list
    AudioDeck *Audio::LastDeck()
    {
        AudioDeck *d = instance->firstDeck;
        while (d) {
            if (d->nextDeck) d = d->nextDeck;
            else return d;
        }
        return 0;
    }
    
    void Audio::Update()
    {
        if (IsPaused())
            return;     // Wow, we don't care! ;D
        
        AudioDeck *d = firstDeck;
        while (d) {
            AudioDeck *nextDeck = d->nextDeck;
            d->Update();
            
            // Sometimes the deck will kill itself, so we have to get the next deck BEFORE calling Update().
            d = nextDeck;
        }
    }

    Audio::Audio() {
        instance = this;
        this->firstDeck = NULL;
        
        if (!decoderMap)
            decoderMap = new std::map<std::string, makeDecoderFunc>;
        
        this->musicDeck = NULL;
        
        this->allPaused = false;
    }
    
    Audio::~Audio() {
//        std::map<std::string,AudioDecoder*>::iterator it;
        
        // Deletes the decks
        AudioDeck *nextDeck = this->firstDeck;
        while (nextDeck) {
            AudioDeck *d = nextDeck->nextDeck;
            delete nextDeck;
            nextDeck = d;
        }
        
        if (decoderMap) delete decoderMap;
        decoderMap = NULL;
        musicDeck = NULL;
    }
    
    void Audio::RegisterDecoder(makeDecoderFunc makeFunc, std::string extension)
    {
        if (decoderMap == NULL) decoderMap = new std::map<std::string, makeDecoderFunc>;
        
        std::stringstream ss(extension);
        std::string tmp;
        
        while (ss >> tmp) {
            decoderMap[0][tmp] = makeFunc;
        }
    }
    
    void Audio::PlaySound( AudioAsset *asset, int loops, float volume, float pan, float pitch )
    {
        AudioDeck *deck = NewDeck(asset);
        if (!deck) return;
        deck->SetVolume(volume);
        deck->SetPan(pan);
        deck->SetPitch(pitch);
        deck->SetLoops(loops);
        deck->FreeDeckOnFinish();
        deck->Play();
        return;
    }
    
    /** Music Stuff **/
    void Audio::PlayMusic( AudioAsset *asset, float volume, long fadeTime )
    {
        AudioDeck *mus;
        StopMusic(fadeTime);
        
        mus = NewDeck(asset);
        mus->SetLoops(0);
        mus->SetVolume(volume);
        mus->SetFadeIn(fadeTime);
        mus->Play();
        
        instance->musicDeck = mus;
    }
    
    void Audio::StopMusic( long fadeTime )
    {
        if (instance->musicDeck){
            if (fadeTime > 0){
                instance->musicDeck->FreeDeckOnFinish();
                instance->musicDeck->StopWithFade(fadeTime);
            }
            else
            {
                instance->musicDeck->Stop();
                delete instance->musicDeck;
            }
        }
        
        instance->musicDeck = NULL;
    }
    
    AudioDeck *Audio::GetMusicDeck()
    {
        return instance->musicDeck;
    }
 
    void Audio::PauseMusic()
    {
        AudioDeck *mus = GetMusicDeck();
        if (mus) mus->Pause();
    }
    
    void Audio::ResumeMusic()
    {
        AudioDeck *mus = GetMusicDeck();
        if (mus) mus->Resume();        
    }
    
    bool Audio::IsPaused()
    {
        return instance->allPaused;
    }
    
    void Audio::PauseAll()
    {
        instance->allPaused = true;
        
        AudioDeck *d = instance->firstDeck;
        while (d) {
            AudioDeck *nextDeck = d->nextDeck;
            ChannelStream *cs;
            cs = d->GetChannelStream();
            
            if (cs) cs->Pause();
            
            // Sometimes the deck will kill itself, so we have to get the next deck BEFORE calling Update().
            d = nextDeck;
        }
    }
    
    void Audio::ResumeAll()
    {
        AudioDeck *d = instance->firstDeck;
        while (d) {
            AudioDeck *nextDeck = d->nextDeck;
            ChannelStream *cs;
            cs = d->GetChannelStream();
            
            if (cs) {
                if (!d->IsPaused(false))
                    cs->Resume();
            }
            
            // Sometimes the deck will kill itself, so we have to get the next deck BEFORE calling Update().
            d = nextDeck;
        }
        
        instance->allPaused = false;
    }
    
    void Audio::SystemHalt()
    {
        ChannelStream::InterruptBegin();
    }
    
    void Audio::SystemResume()
    {
        ChannelStream::InterruptEnd();
    }
}
