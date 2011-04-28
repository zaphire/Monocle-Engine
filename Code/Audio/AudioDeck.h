//
//  AudioDeck.h
//
//  Created by Josh Whelchel on 4/26/11.
//

#pragma once

#include "ChannelStream.h"
#include "AudioDecoder.h"
#include "AudioVis.h"

#if defined(MONOCLE_WINDOWS)
	#define __INT64_TYPE__ __int64
#elif defined(MONOCLE_LINUX)
	#define __INT64_TYPE__ long long
#endif

namespace Monocle {
    // Structure containing fade information
    
    /**
        Contains fade information for an AudioDeck. All times are in MS, and aFade* times are relative to ChannelStream::GetTotalPlayTime().
     */
    class AudioFades
    {
    public:
        AudioFades();
        
        /**
            Resets all values to 0
         */
        void Reset();
        
        /**
            Fade time in milliseconds at the start of a track
         */
        unsigned long	nFadeIn;		// Active Fade in time (at beginning of track)
        /**
            Fade time in milliseconds at the end of a track (last loop iteration)
         */
        unsigned long	nFadeOut;		// Active Fade out time (at end of track)
        
        /**
            Time in milliseconds to start an immdiate fade out
         */
        unsigned long	aFadeOutStart;		// Immediate Fade Out (as in, Stop and Fade Out, or Pause and Fade Out)
        
        /**
            Time in milliseconds to start an immdiate fade in
         */
        unsigned long	aFadeInStart;		// Immediate Fade In (as in, Resume and Fade In)
        
        /**
            Time in milliseconds to end an immdiate fade out
         */
        unsigned long	aFadeOutEnd;		// Immediate Fade Out (as in, Stop and Fade Out, or Pause and Fade Out)
        
        /**
            Time in milliseconds to end an immdiate fade in
         */
        unsigned long	aFadeInEnd;			// Immediate Fade In (as in, Resume and Fade In)
        
        /**
            Specifies whether or not an AudioDeck will pause (true) or continue playing at 0 vol (false) when aFadeOutEnd is reached
         */
        bool	bPauseOnFadeOut;	// When aFadeOutEnd is reached, should we pause or keep playing.
        
        /**
            Specifies whether or not the deck should be silent (e.g. after bPauseOnFadeOut).
         */
        bool    bSilent;
        
    };
    
    /**
        Responsible for controlling a Playback channel (implemented to the system through ChannelStream) and
        provides a basic control scheme for playback control.
     
        Creation of new AudioDeck's is solely the purpose of Audio::NewDeck().
     */
    class AudioDeck
    {
    public:
        AudioDeck( AudioDeck **deckSetter, AudioDecodeData *decodeData, bool freeDataWithDeck = true );
        AudioDeck( AudioDeck *prevDeck, AudioDecodeData *decodeData, bool freeDataWithDeck = true );
        ~AudioDeck();
        
        /**
            Resets the values in the deck.
         */
        void ResetDeck();
        
        /**
            Updates the audio engine with freshly decoded data and updates fade and volume levels.
         */
        void Update();
        
        /**
            Sets the fade in time for the start of playback in milliseconds.
         */
        void SetFadeIn( unsigned long msFade );
        
        /**
            Sets the fade out time for the end of playback in milliseconds.
         */
        void SetFadeOut( unsigned long msFade );
        
        /**
            Begins playback of the Deck (also resumes).
         */
        void Play();
        
        /**
            Pauses the deck.
         */
        void Pause();
        
        /**
            Pauses the deck with a specified fade time in milliseconds.
         */
        void PauseWithFade( unsigned long msFade );
        
        /**
            Resumes the deck.
         */
        void Resume();
        
        /**
            Resumes the deck with a specified fade time in milliseconds.
         */
        void ResumeWithFade( unsigned long msFade );
        
        /**
            Unmutes the deck
         */
        void Unmute();
        
        /**
            Unmutes the deck with a fadein.
         
         @todo Starts playing immediately if the deck is paused.
         */
        void UnmuteWithFade( unsigned long msFade );
        
        /**
            Fades the deck out into silence (can be resumed normally with Resume() or ResumeWithFade()).
            The deck will continue playing in silence and not pause.
         */
        void MuteWithFade( unsigned long msFade );
        
        /**
            Mutes the deck but continues playing it. This is different from setting the volume to 0 as it will allow
            fade in.
         */
        void Mute();
        
        /**
         @return If the deck is muted or is muting.
         */
        bool IsMuted();
        
        /**
            @return Total length in milliseconds of the deck.
         */
        unsigned long GetTotalLength();
        
        ChannelStream	*cs;
        AudioDeck       *nextDeck;      // INTERNAL, Next Deck in the sequence
        AudioDeck       **prevDeckPointerToHere;     // INTERNAL, Pointer in the previous deck to this deck
        
        AudioDecodeData *decodeData;
        
        /**
            Volume refers to the cumulative volume of the deck, regardless of fades. To mute a deck use Mute().
         */
        float       volume;             // 0.0 silent - 1.0 full volume
        float       pan;                // -1.0 left - 1.0 right
        float		pitchBend;          // Pitch Bend ranges from 0.5 to 2.0;
        
        AudioVis    *vis;
        bool		cleanVis;           // Set true if you need to clean the vis (on flushes and seeks)
                
        // Vis Cache!
        VisCache2   vc;
        
    private:
        
        bool		done;
        bool		pause;
        bool		stopping;			// Called to let the play function know that this deck is trying to stop
        
        bool        freeDecoderData;    // Free the decoder data when deconstructed.
        
        long		total;              // Total length in MS (not including looping)
        long		curpos;             // Current position in MS
        
        unsigned char temp_waveR[576];
        unsigned char temp_waveL[576];
        
        bool        playStarted;        // Regardless of pause, did we start to play?
        bool		failed;             // If the deck fails, throw this flag.
        void		*visdata;			// Reserved for internal use
        
        AudioFades	fades;      // Structure containing fade information
        bool		didSeek;            // Marks that the output buffer needs flushing (internal stuff)
        long		lastSeekPos;        // Last position seeked with "Seek Position"
        
        __INT64_TYPE__ writtenpos;		// An internal record keeper of the current written position. Not a good indicator of the current file position. Can be reset to 0 on seeks and flushes.
        
        long		bufLen;             // Buffer length in MS for next play (defaults to 1000) (100 < x < 10000)
        
        int			numLoops;           // The number of loops originally assigned to this deck. (For fade out information... <0 is infinite)
        
        // Useful External Information
        long		currentPosition;	// This holds an accurate indication of where the current file position is.
        
        bool		starting;           
        bool		triggerDisconnectOnFinish;      // (for sound effects...)
        
        void        UpdateVizJunk();
        bool        UpdateFades();
        
        unsigned long totsamps; // total rendered samples
        
        long vizlast;
        
        void        Init();             // Internal init.
    };
}
