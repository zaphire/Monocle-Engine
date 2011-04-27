#pragma once

#define BUFFER_SIZE (1024 * 128)
#define NUM_BUFFERS 3

namespace Monocle
{
    class ChannelStream
	{
    public:
        ChannelStream();
        ~ChannelStream();
        
    public:
        
        void Open( int channels, int bits, int samplerate );
        void Close();
        
        // Returns number of buffers to fill
        int NeedsUpdate();
        
        // When update is needed, use getBuffer() to obtain the buffer to fill
        unsigned char *GetBuffer( unsigned int *size );
        
        // Use lockBuffer() after you've filled the buffer obtained from getBuffer()
        void LockBuffer( unsigned int size );
        
        void LockNumberedBuffer( unsigned int size, unsigned int buff );
        unsigned char *GetStaticBuffer( unsigned int *size );
        
        void Play();
        void Stop();
        
        bool IsPlaying();
        void Check();                 // checks OpenAL error state
        
        static int Init();
        static void Exit();
        
        float GetPlaybackPosition();
        
    protected:
        
        void Empty();                 // empties the queue
        char *ErrorString(int code); // stringify an error code	

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
