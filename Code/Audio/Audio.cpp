//
//  Audio.cpp
//
//  Created by Josh Whelchel on 4/26/11.
//

#include <string.h>

#include "Audio.h"

#include "oggvorbis/OggDecoder.h"
#include "Debug.h"

namespace Monocle {
    
    Audio *Audio::instance = NULL;
    
    void Audio::Init()
    {
        AudioDecoder *decoder;
        
        Debug::Log("Audio::Init...");
        
#ifdef MONOCLE_AUDIO_OGG
        decoder = new OggDecoder();
        decoderMap["ogg"] = decoder;
#endif
            
        ChannelStream::init();
            
        Debug::Log("...Done");

    }
    
    AudioDeck *Audio::NewDeck( AudioDecodeData *decodeData )
    {
        if (!instance->firstDeck)
        {
            instance->firstDeck = new AudioDeck( &instance->firstDeck, decodeData );
            return instance->firstDeck;
        }
        else
        {
            // New Deck must be built on the last deck
            AudioDeck *lastDeck = LastDeck();
            lastDeck->nextDeck = new AudioDeck(lastDeck, decodeData);
            return lastDeck->nextDeck;
        }
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