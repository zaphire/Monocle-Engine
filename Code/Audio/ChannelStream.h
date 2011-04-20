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
        
        void play();
        void stop();
        
        bool isPlaying();
        void check();                 // checks OpenAL error state
        
        static int init();
        static void exit();
        
        float getPlaybackPosition();
        
    protected:
        
        void empty();                 // empties the queue
        char *errorString(int code); // stringify an error code	

    private:
        
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
