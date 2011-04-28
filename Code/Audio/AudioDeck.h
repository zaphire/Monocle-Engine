//
//  AudioDeck.h
//
//  Created by Josh Whelchel on 4/26/11.
//

#pragma once

#include "ChannelStream.h"
#include "AudioDecoder.h"
#include "AudioVis.h"

namespace Monocle {
    // Structure containing fade information
    typedef struct aud_deck_fades_s
    {
        unsigned long	nFadeIn;		// Active Fade in time (at beginning of track)
        unsigned long	nFadeOut;		// Active Fade out time (at end of track)
        
        unsigned long	nDeckFadeIn;	// Deck default Fade In time (at beginning)
        unsigned long	nDeckFadeOut;	// Deck default Fade Out time (at end)
        
        unsigned long	aFadeOutStart;		// Immediate Fade Out (as in, Stop and Fade Out, or Pause and Fade Out)
        unsigned long	aFadeInStart;		// Immediate Fade In (as in, Resume and Fade In)
        
        unsigned long	aFadeOutEnd;		// Immediate Fade Out (as in, Stop and Fade Out, or Pause and Fade Out)
        unsigned long	aFadeInEnd;			// Immediate Fade In (as in, Resume and Fade In)
        
        bool	bPauseOnFadeOut;	// When aFadeOutEnd is reached, should we pause or stop?
        
    } aud_deck_fades;
    
    class AudioDeck
    {
    public:
        AudioDeck( AudioDeck **deckSetter, AudioDecodeData *decodeData, bool freeDataWithDeck = true );
        AudioDeck( AudioDeck *prevDeck, AudioDecodeData *decodeData, bool freeDataWithDeck = true );
        ~AudioDeck();
        void ResetDeck();
        void Update();
        
        void SetFadeIn( unsigned long msFade );
        void SetFadeOut( unsigned long msFade );
        
        void Play();
        
        void Pause();
        void PauseWithFade( unsigned long msFade );
        
        void Resume();
        void ResumeWithFade( unsigned long msFade );
        
        unsigned long GetTotalLength();
        
        ChannelStream	*cs;
        AudioDeck       *nextDeck;      // INTERNAL, Next Deck in the sequence
        AudioDeck       **prevDeckPointerToHere;     // INTERNAL, Pointer in the previous deck to this deck
        
        AudioDecodeData *decodeData;
        
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
        
        aud_deck_fades		fades;      // Structure containing fade information
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