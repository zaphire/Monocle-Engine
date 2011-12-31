#pragma once

#include <string>

namespace Monocle
{
    /**
        Streams audio to the platform audio engine (OpenAL, etc).
        
        This class is implemented by AudioDeck and should be reimplemented for different audio drivers.
        Use of this class should be limited to the AudioDeck and is abstracted for simplification of audio implementation.
     */
     
    class ChannelStream
	{
    public:
        ChannelStream();
        ~ChannelStream();
        
    public:
        
        /**
            Opens an audio channel with the specified settings
         
            @param channels Number of audio channels, 1 for mono 2 for stereo
            @param bits Bitrate of channel, 8, 16, etc.
            @param samplerate Samples per second (hz) of the audio channel
         */
        void Open( int channels, int bits, int samplerate );
        
        /**
            Closes the audio channel
         */
        void Close();
        
        // Returns number of buffers to fill
        /**
            Determines whether the channel needs an update (buffers to be filled).
         
            After calling this, each buffer should be filled with GetBuffer() and LockBuffer().
         
         
            @see GetBuffer()
            @see LockBuffer()
            @return The number of buffers to be filled.
         */
        int NeedsUpdate();
        
        // When update is needed, use getBuffer() to obtain the buffer to fill
        
        /**
            When an update is needed, GetBuffer() will return the pointer to the next available buffer. After the buffer is filled,
            LockBuffer() should be called.
            
            @param size Receives the size of the buffer in bytes (not samples)
            @return A pointer to the buffer
         */
        unsigned char *GetBuffer( unsigned int *size );
        
        // Use lockBuffer() after you've filled the buffer obtained from getBuffer()
        
        /**
            Once a buffer provided by GetBuffer() is filled, it should be "locked" and played via this function.
         
            @param size The size of the buffer (should match the value returned from GetBuffer()).
         */
        void LockBuffer( unsigned int size );
        
        /**
            Fills a static buffer by number. Usually good for filling the audio before playing or when flushing. Buffer data can be
            retrieved by GetStaticBuffer(). This is automatically implemented by LockBuffer() and shouldn't be used externally.
         
            @param size Size of buffer to fill
            @param buff Index of buffer to fill
         */
        void LockNumberedBuffer( unsigned int size, unsigned int buff );
        
        /**
            Retrieves the static buffer. Automatically implemented by GetBuffer() when needed.
         
         @param size Receives the size of the buffer in bytes (not samples)
         @return A pointer to the buffer
         */
        unsigned char *GetStaticBuffer( unsigned int *size );
        
        /**
            @return The total time in milliseconds the channel has been playing, regardless of plays, pauses, stops, etc.
         */
        unsigned long GetTotalPlayTime(); // Gets the total play time in ms of the object
        
        /**
            @return The total time in milliseconds that the channel has been open - resets on pause and stop.
         */
        unsigned long GetOutputTime(); // Gets the time in ms that the buffer, resets on pauses
        
        /**
            Sets the offset of the buffer, used when syncing to accomodate pausing etc.
         */
        void SetPlayOffset( unsigned long playOffset ); // This is for syncing the appropriate play time
        
        /**
            Sets the volume of the channel
         
         @param vol Volume, between 0.0 and 1.0
         */
        void SetVolume( float vol );
        
        /**
            Sets the pan of the channel
         @param pan Pan between -1.0 (L) and 1.0 (R). 0.0 for center.
         */
        void SetPan( float pan );
        
        /**
            Sets the pitch of the channel
         @param pitch Pitch between 0.5 and 2.0, 1.0 for default.
         */
        void SetPitch( float pitch );
        
        /**
            Begins channel playback
         */
        void Play();
        
        /**
            Stops channel playback
         */
        void Stop();
        
        /**
            Pauses channel playback
         */
        void Pause();
        
        /**
            Resumes channel playback
         */
        void Resume();
        
        /**
            Determines if the channel is playing
         */
        bool IsPlaying();
        
        static void Check( std::string erat = "unk" );                 // checks OpenAL error state
        
        /**
            Init() opens the sound drivers for the entire audio platform and is called once.
         */
        static int Init();
        static void Exit();
        
        // Particularly needed for iOS
        static void InterruptBegin();
        static void InterruptEnd();
        
        /**
            @return true if the ChannelStream successfully opened and is presently open
         */
        bool IsOpen();
        
        unsigned int GetNumberBuffers();
        unsigned long GetBufferSize();
        
    protected:
        
        void Empty();                 // empties the queue
        char *ErrorString(int code); // stringify an error code	

    private:
        
        unsigned long playOffset;
        unsigned long playStart;
        unsigned long pausePos;
        
        // This OpenAL implementation stuff needs to be moved
        
        unsigned int *buffers; // buffers
        unsigned int source;     // audio source
        int format;     // format
        unsigned int active_buffer; // active buffer (0 or 1)
        unsigned int samplerate;
        
        bool started;
        bool startedPlaying;
        
        unsigned int startBuffer;
        
        unsigned char *obtainedBuffer;
        
        float lastVol;
    };
}
