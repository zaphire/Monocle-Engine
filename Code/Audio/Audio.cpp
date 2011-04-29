//
//  Audio.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include <string>
#include <algorithm>

#include "Audio.h"

#include "oggvorbis/OggDecoder.h"
#include "../Debug.h"
#include "../MonocleToolkit.h"

namespace Monocle {
    
    Audio *Audio::instance = NULL;
    
    void Audio::Init()
    {
        AudioDecoder *decoder;
        
        Debug::Log("Audio::Init...");
        
#ifdef MONOCLE_AUDIO_OGG
        decoder = new OggDecoder();
        decoderMap["ogg"] = decoder;
        decoderMap["g2m"] = decoder;
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
        std::string ext = audioAsset->GetExtension();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        
        if(instance->decoderMap.find(ext) == instance->decoderMap.end())
        {
            Debug::Log("AUDIO: Could not find a decoder for the file: " + audioAsset->GetName());
            return 0;
        }
        
        AudioDecoder *decoder = instance->decoderMap[ext];
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
            d->Update();
            d = d->nextDeck;
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
    }
    
    Audio::~Audio() {
        std::map<std::string,AudioDecoder*>::iterator it;
        
        // Deletes the decks
        AudioDeck *nextDeck = this->firstDeck;
        while (nextDeck) {
            AudioDeck *d = nextDeck->nextDeck;
            delete nextDeck;
            nextDeck = d;
        }
        
        // Removes decoders
        it = decoderMap.begin();
        for (; it != decoderMap.end(); ++it){
            delete it->second;
        }
        decoderMap.clear();
    }
 
}