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
        RegisterDecoder(makeOggFunc, "ogg g2m");
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
        AudioDeck *d = firstDeck;
        while (d) {
            AudioDeck *nextDeck = d->nextDeck;
            d->Update();
            
            // Sometimes the deck will kill itself, so we have to get the next deck BEFORE calling Update().
            d = nextDeck;
        }
    }
    
    void Audio::PauseAll()
    {
        
    }
    
    void Audio::ResumeAll()
    {
        
    }
    
    Audio::Audio() {
        instance = this;
        this->firstDeck = 0;
        
        if (!decoderMap)
            decoderMap = new std::map<std::string, makeDecoderFunc>;
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
        deck->FreeDeckOnFinish();
        deck->Play();
        return;
    }
 
}
