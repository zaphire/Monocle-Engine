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
    
    enum VisBandLength
	{
		VIS_BAND_SHORT=0,
		VIS_BAND_MEDIUM,
        VIS_BAND_LONG
	};
    
    enum FadeOutAction
    {
        FADEOUT_AND_MUTE=0,
        FADEOUT_AND_PAUSE,
        FADEOUT_AND_STOP,
        
        FADEOUT_AND_COUNT
    };
    
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
        FadeOutAction	fadeAction;	// When aFadeOutEnd is reached, should we pause or keep playing, or die?.
        
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
        AudioDeck( AudioDeck **deckSetter, AudioDecoder *decoder, bool freeDecoderWithDeck = true );
        AudioDeck( AudioDeck *prevDeck, AudioDecoder *decoder, bool freeDecoderWithDeck = true );
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
            Determines if the deck is paused.
         
            @param absolute If false, IsPaused() returns whether or not the Deck is supposed to be paused. (for the audio engine)
         */
        bool IsPaused( bool absolute = true );
        
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
            Stops the deck.
         */
        void Stop();
        
        /**
            Stops the deck with a fade time in milliseconds.
         */
        void StopWithFade( unsigned long msFade );
        
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
        
        /**
            @return Returns playing position of track
         */
        unsigned long GetCurrentTime();
        
        /**
            Determines if the deck is finished playing. This occurs at the end of last loop for most files.
            At this point, it's safe to seek or to simply delete the deck.
         */
        bool IsDone();
        
        /**
            Sets the number of full plays the deck should have (0 for infinite). The current or first play will count as 1.
         
            @param loops Number of loops, using 0 for infinite
         */
        void SetLoops( int loops );
        
        /**
            Returns the number of times the deck has to loop back. 0 indicates the final loop is in progress. -1 indicates infinite looping.
         */
        int LoopsRemaining();
        
        /**
            Seeks to a position (in milliseconds) in the track if possible.
            
            @param pos Position in track to seek to, must be between 0 and GetTotalLength()
         */
        void Seek( long pos );
        
        /**
            Allows the deck to free on completion. Important for sound effects and other nonpersistent sounds.
         */
        void FreeDeckOnFinish( bool freeDeckOnFinish = true );
        
        /**
            Sets the volume of the deck, between 0.0 and 1.0 (default is 1.0)
         */
        void SetVolume( float volume );
        
        /**
            Sets the panning of the deck, between -1.0 (Left) and 1.0 (Right), with 0.0 being center
         */
        void SetPan( float pan );
        
        /**
            Sets the pitch of the deck, between 0.5 (half speed) and 2.0 (double speed), where 1.0 is default.
            This will disable visualizations if not at 1.0.
         */
        void SetPitch( float pitch );
        
        /**
         Gets the volume of the deck, between 0.0 and 1.0 (default is 1.0)
         */
        float GetVolume(  );
        
        /**
         Gets the panning of the deck, between -1.0 (Left) and 1.0 (Right), with 0.0 being center
         */
        float GetPan(  );
        
        /**
         Gets the pitch of the deck, between 0.5 (half speed) and 2.0 (double speed), where 1.0 is default.
         This will disable visualizations.
         */
        float GetPitch(  );
        
        /**
         Enables the processing of visualization spectrum data and
         waveform grabs.
         */
        void EnableVis( bool visEnable = true );
        
        /**
         @return true is Visualization data is enabled
         */
        bool IsVisEnabled();
        
        /**
         Returns the visual waveform data if visualizations are enabled.
         
         @param index A value between 0-575 to specify where in the wave we're retrieving
         @param channel Which channel data to get, 0 for left, 1 for right
         @return a value between -1.0 and 1.0
         */
        float GetVisWaveform( int index, int channel = 0 );
        
        /**
         Returns the visual spectrum data if visualizations are enabled. The best data is generally between indecies 10 and ~450.
         
         @param index A value between 0-511 to specify where in the wave we're retrieving
         @param channel Which channel data to get, 0 for left, 1 for right - if channel is -1 (default) then it will return the loudest of the two channels.
         @return a value between 0.0 and 1.0
         */
        float GetVisSpectrum( int index, int channel = -1 );
        
        /**
         Returns the index (and value) of the loudest spectrum sample in a given channel (-1 to ignore channels).
         
         @param loudestValue pointer to fill with the value of the loudest index.
         @param channel Which channel data to get, 0 for left, 1 for right - if channel is -1 (default) then it will return the loudest of the two channels.
         @param startIndex The index to begin looking at (0-511)
         @param endIndex The last index to look at (0-511)
         @warning startIndex must be less than endIndex.
         @return The index of the loudest spectrum, 0 - 511
         */
        int GetVisLoudestSpectrumIndex( float *loudestValue = NULL, int channel = -1, int startIndex = 0, int endIndex = 511); 
        
        /**
         Returns the average of one of 16 bands in the spectrum.
         
         @param band band index 0-15
         @param length One of VIS_BAND_SHORT, VIS_BAND_MEDIUM, and VIS_BAND_LONG to determine the length of the average
         @param channel 0 for left, 1 for right, -1 for loudest of either
         @return The average of the visual band
         */
        float GetVisBandAverage( int band, VisBandLength length = VIS_BAND_SHORT, int channel = -1 );
        
        /**
         Flushes the deck.
         */
        void Flush();
        
        /**
            Returns the ChannelStream class behind this deck.
            Advanced purposes only.
         */
        ChannelStream *GetChannelStream();
        
        AudioDeck       *nextDeck;      // INTERNAL, Next Deck in the sequence
        AudioDeck       **prevDeckPointerToHere;     // INTERNAL, Pointer in the previous deck to this deck
        
        AudioDecoder *decoder;
        
        AudioVis    *vis;
        bool		cleanVis;           // Set true if you need to clean the vis (on flushes and seeks)
                
        // Vis Cache!
        VisCache2   vc;
        
    private:
        
        ChannelStream	*cs;
        
        bool        freeDeckOnFinish;
        bool		pause;
        
        bool        freeDecoder;    // Free the decoder data when deconstructed.
        
        unsigned char temp_waveR[576];
        unsigned char temp_waveL[576];
        
        bool        playStarted;        // Regardless of pause, did we start to play?
        bool		failed;             // If the deck fails, throw this flag.
        
        AudioFades	fades;      // Structure containing fade information
        long		lastSeekPos;        // Last position seeked with "Seek Position"
        
        unsigned long writtenpos;		// An internal record keeper of the current written position. Not a good indicator of the current file position. Can be reset to 0 on seeks and flushes.
        
        // Useful External Information
        long		currentPosition;	// This holds an accurate indication of where the current file position is.
        
        void        UpdateVizJunk();
        bool        UpdateFades();
        void        FillBuffers();
        
        long        sampsize;
        
        bool        isFinished;         // Sets to true when the decoder runs out of data AND the silence has played
        int         bufferCountdown;    // Counts down to zero once the decodeData->outOfData is flagged. Used to signify when audio is truly done.
        
        /**
         Volume refers to the cumulative volume of the deck, regardless of fades. To mute a deck use Mute().
         */
        float       volume;             // 0.0 silent - 1.0 full volume
        float       pan;                // -1.0 left - 1.0 right
        float		pitchBend;          // Pitch Bend ranges from 0.5 to 2.0;
        
        unsigned long totsamps; // total rendered samples
        
        long vizlast;
        
        void        Init();             // Internal init.
    };
}
