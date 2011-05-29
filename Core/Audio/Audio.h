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
            Returns true if all the decks are paused.
         */
        static bool IsPaused();
        
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
        static AudioDeck *NewDeck( AudioDecoder *decodeData, bool freeDecoderWithDeck = true );
        
        /**
            Returns the last created AudioDeck in the stack.
         */
        static AudioDeck *LastDeck();
        
        /**
            Registers a decoder with a file extension for automatic assignment when calling NewDeck().
         
            @param makeFunc Pointer to a function which will construct a new audio decoder for the appropriate extension
            @param extension The file extension associated with this AudioDecoder, e.g. "ogg"
            @see makeDecoderFunc
         */
        static void RegisterDecoder( makeDecoderFunc makeFunc, std::string extension );
        
        /**
            Plays an audio file. The deck it plays on will be freed automatically when the sound is complete.
         
            @param asset AudioAsset to play
            @param loops Number of times to loop
            @param volume Volume, between 0.0 and 1.0
            @param pan Panning, between -1.0(L) and 1.0(R), 0.0 for center
            @param pitch Pitch, 0.5 - 2.0
         */
        static void PlaySound( AudioAsset *asset, int loops = 1, float volume = 1.0, float pan = 0.0, float pitch = 1.0 );
        
        /**
            Plays an audio sound as an infinitely looping music deck. If music is already playing, the previous deck will fade out
            fadeTime. The new music will also fade in that much. To stop the music abruptly and fade this one in, call
            StopMusic() first.
         
            @param asset AudioAsset to play
            @param volume Volume between 0.0 and 1.0
            @param fadeTime Time to fade in (milliseconds) (and fade out current deck if playing)
         */
        static void PlayMusic( AudioAsset *asset, float volume = 1.0, long fadeTime = 0 );
        
        /**
            Stops the currently playing music deck.
         
             @param fadeTime Time to fade out (milliseconds)
         */
        static void StopMusic( long fadeTime = 0 );
        
        /**
            Pauses the music deck.
         */
        static void PauseMusic();
        
        /**
            Resumes the music deck.
         */
        static void ResumeMusic();
        
        /**
            Returns the static music AudioDeck.
         */
        static AudioDeck *GetMusicDeck();
    private:
        
        static Audio *instance;        
        
        AudioDeck *firstDeck;
        
        bool allPaused;
		
        /** For easy playback of music **/
        AudioDeck *musicDeck;
        
        std::map<std::string, AudioAsset*> musMap;
	};
}


