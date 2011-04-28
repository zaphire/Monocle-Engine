#pragma once

#include <string>
#include <map>

#include "ChannelStream.h"
#include "AudioDecoder.h"
#include "AudioDeck.h"
#include "AudioAsset.h"
#include "AudioVis.h"

namespace Monocle
{
    /**
        Platform-independent audio system, responsible for maintaining all playing channels (AudioDecks).
     */
	class Audio
	{
	public:
		Audio();
        ~Audio();
        
		void Init();
        
        /**
            Updates the AudioDecks and handles volumes.
         */
        void Update();
        
        
        static void PauseAll();
        static void ResumeAll();
        
        /**
            Creates a new deck from a given AudioAsset. Assets::RequestAudio is responsible for providing
            a valid asset (which can be loaded into memory).
         */
        static AudioDeck *NewDeck( AudioAsset *audioAsset );
        
        /**
            Creates a new deck from a provided AudioDecodeData. This is best used when attempting to
            create a deck from an alternative decoder or you've used AudioDecoder::RequestData. You may
            use this if you aren't using a standard file extension for your audio asset. Unless freeDecodeDataWithDeck
            is set to false, the deck will be responsible for destroying the data when it is freed.
         */
        static AudioDeck *NewDeck( AudioDecodeData *decodeData, bool freeDecodeDataWithDeck = true );
        
        /**
            Returns the last created AudioDeck in the stack.
         */
        static AudioDeck *LastDeck();
        
        /**
            Registers a decoder with a file extension for automatic assignment when calling NewDeck().
         
            @param decoder Pointer to an instance of the specified decoder.
            @param extension The file extension associated with this AudioDecoder, e.g. "ogg"
         */
        static void RegisterDecoder( AudioDecoder *decoder, std::string extension );
        
    private:
        
        static Audio *instance;        
        
        AudioDeck *firstDeck;
		
		std::map<std::string, void*> sfxMap;
        std::map<std::string, void*> musMap;
	};
}


