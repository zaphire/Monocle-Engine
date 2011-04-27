#pragma once

#define BUFFER_SIZE (1024 * 128)
#define NUM_BUFFERS 3

namespace Monocle
{
    class ChannelStream {
        
    public:
        ChannelStream();
        ~ChannelStream();
        
    public:
        
        void open( int channels, int bits, int samplerate );
        void close();
        
        // Returns number of buffers to fill
        int needsUpdate();
        
        // When update is needed, use getBuffer() to obtain the buffer to fill
        unsigned char *getBuffer( unsigned int *size );
        
        // Use lockBuffer() after you've filled the buffer obtained from getBuffer()
        void lockBuffer( unsigned int size );
        
        void lockNumberedBuffer( unsigned int size, unsigned int buff );
        unsigned char *getStaticBuffer( unsigned int *size );
        
        unsigned long getTotalPlayTime(); // Gets the total play time in ms of the object
        unsigned long getOutputTime(); // Gets the time in ms that the buffer, resets on pauses
        void setPlayOffset( unsigned long playOffset ); // This is for syncing the appropriate play time
        
        void setVolume( float vol );
        void setPan( float pan );
        void setPitch( float pitch );
        
        void play();
        void stop();
        
        void pause();
        void resume();
        
        bool isPlaying();
        void check();                 // checks OpenAL error state
        
        static int init();
        static void exit();
        
    protected:
        
        void empty();                 // empties the queue
        char *errorString(int code); // stringify an error code	

    private:
        
        unsigned long playOffset;
        unsigned long playStart;
        unsigned long pausePos;
        
        // This OpenAL implementation stuff needs to be moved
        
        unsigned int buffers[NUM_BUFFERS]; // buffers
        unsigned int source;     // audio source
        int format;     // format
        unsigned int active_buffer; // active buffer (0 or 1)
        unsigned int samplerate;
        
        bool started;
        bool startedPlaying;
        
        unsigned int startBuffer;
        
        unsigned char obtainedBuffer[BUFFER_SIZE];
    };
}
