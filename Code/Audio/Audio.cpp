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

#define ADD_DECODER(v) decoders->push_back( new v );

namespace Monocle {
   
    Audio *Audio::instance = NULL;
    static std::map<std::string, AudioDecoder *> *decoderMap = NULL;
    static std::vector<AudioDecoder *> *decoders = NULL;
    
    void Audio::Init()
    {
        Debug::Log("Audio::Init...");
        
        // Init the separate decoders here
        ADD_DECODER(WaveDecoder);
#ifdef MONOCLE_AUDIO_OGG
        ADD_DECODER(OggDecoder);
#endif
            
        ChannelStream::Init();
            
        Debug::Log("...Done");

    }
    
    AudioDeck *Audio::NewDeck( AudioDecodeData *decodeData, bool freeDecodeDataWithDeck )
    {
        if (!instance->firstDeck)
        {
            instance->firstDeck = new AudioDeck( &instance->firstDeck, decodeData, freeDecodeDataWithDeck );
            return instance->firstDeck;
        }
        else
        {
            // New Deck must be built on the last deck
            AudioDeck *lastDeck = LastDeck();
            lastDeck->nextDeck = new AudioDeck(lastDeck, decodeData,freeDecodeDataWithDeck);
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
        
        AudioDecoder *decoder = decoderMap[0][ext];
        AudioDecodeData *decData = decoder->RequestData(audioAsset);
        
        if (!decData){
            Debug::Log("AUDIO: Cannot decode file: " + audioAsset->GetName());
            return 0;
        }
        
        return Audio::NewDeck( decData, true );
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
        
        if (!decoders)
            decoders = new std::vector<AudioDecoder*>;
        if (!decoderMap)
            decoderMap = new std::map<std::string, AudioDecoder *>;
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
        
        // Removes decoders (not our job!)
        /**
        it = decoderMap.begin();
        for (; it != decoderMap.end(); ++it){
            delete it->second;
        }
         **/
        
        if (decoders)
        {
            
            std::vector<AudioDecoder *>::iterator it;
            it = decoders->begin();
            
            for (; it != decoders->end(); ++it){
                delete it[0];
            }
            delete decoders;
        }
        
        if (decoderMap) delete decoderMap;
        decoders = NULL;
        decoderMap = NULL;
    }
    
    void Audio::RegisterDecoder(Monocle::AudioDecoder *decoder, std::string extension)
    {
        if (decoderMap == NULL) decoderMap = new std::map<std::string, AudioDecoder *>;
        decoderMap[0][extension] = decoder;
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
