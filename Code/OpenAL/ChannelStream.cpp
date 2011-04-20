#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ChannelStream.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace Monocle
{
    bool ChannelStream::isPlaying()
    {
        ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    void ChannelStream::open( int channels, int bits, int samplerate )
    {
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
            if (bits == 8)
                format = AL_FORMAT_MONO8;
        }
        else if (channels == 2)
        {
            format = AL_FORMAT_STEREO16;
            if (bits == 8)
                format = AL_FORMAT_STEREO8;
        }
        
        check();
        
        alGenBuffers(NUM_BUFFERS, buffers);
        
        check();
        
        alGenSources(1, &source);
        
        check();
        
        alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
        alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
        alSource3f(source, AL_DIRECTION, 0.0, 0.0, 0.0);
        
        check();
        
        alSourcei(source, AL_ROLLOFF_FACTOR, 0);
        alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
        
        check();
        
        memset(obtainedBuffer,0,BUFFER_SIZE);
        alBufferData(buffers[0], format, obtainedBuffer, BUFFER_SIZE, samplerate);
        alBufferData(buffers[1], format, obtainedBuffer, BUFFER_SIZE, samplerate);
        alBufferData(buffers[2], format, obtainedBuffer, BUFFER_SIZE, samplerate);
        
        printf("opening with sr:%d, nch:%d, format:%d\n",samplerate,channels,format);
        
        check();

        this->samplerate = samplerate;
        this->started = true;
    }

    void ChannelStream::close()
    {
        alSourceStop(source);
        empty();
        alDeleteSources(1,&source);
        alDeleteBuffers(NUM_BUFFERS,buffers);
        started = false;
    }

    int ChannelStream::needsUpdate()
    {
        int processed;
        
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
        return processed;
    }

    void ChannelStream::lockNumberedBuffer( unsigned int size, unsigned int buff )
    {
        alBufferData(buffers[buff], format, obtainedBuffer, size, samplerate);
        check();
    }

    unsigned char *ChannelStream::getBuffer( unsigned int *size )
    {
        alSourceUnqueueBuffers(source, 1, &active_buffer);
        check();
        
        return getStaticBuffer(size);
    }

    unsigned char *ChannelStream::getStaticBuffer( unsigned int *size )
    {
        size[0] = BUFFER_SIZE;	
        return obtainedBuffer;
    }

    void ChannelStream::lockBuffer( unsigned int size )
    {
        alBufferData(active_buffer, format, obtainedBuffer, size, samplerate);
        check();
        
        alSourceQueueBuffers(source, 1, &active_buffer);
        check();
    }

    void ChannelStream::play()
    {
        alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
        alSourcePlay(source);
        check();
    }

    void ChannelStream::stop()
    {
        alSourceStop(source);
    }

    void ChannelStream::empty()
    {
        int queued;
        
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        
        while(queued--)
        {
            ALuint buffer;
            
            alSourceUnqueueBuffers(source, 1, &buffer);
            check();
        }
    }

    void ChannelStream::check()
    {
        int error = alGetError();
            
        if(error != AL_NO_ERROR)
            printf("OpenAL Error: %d\n",error);
    }

    ChannelStream::ChannelStream()
    {
        started = false;
        format = AL_FORMAT_STEREO16;
    }

    ChannelStream::~ChannelStream()
    {
        if (started)
            close();
    }

    void __alexit()
    {
        ALCcontext *context;
        ALCdevice *device;
        
        context = alcGetCurrentContext();
        device = alcGetContextsDevice(context);
        
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    int ChannelStream::init()
    {
        ALCcontext *context;
        ALCdevice *device;
        const ALCchar *default_device;
        
        default_device = alcGetString(NULL,
                                      ALC_DEFAULT_DEVICE_SPECIFIER);
        
        printf("using default device: %s\n", default_device);
        
        if ((device = alcOpenDevice(default_device)) == NULL) {
            fprintf(stderr, "failed to open sound device\n");
            return -1;
        }
        
        context = alcCreateContext(device, NULL);
        alcMakeContextCurrent(context);
        alcProcessContext(context);
        
        atexit(__alexit);
        
        /* allocate buffers and sources here using alGenBuffers() and alGenSources() */
        /* ... */
        
        alGetError();    /* clear any AL errors beforehand */
        return 0;	
    }

    void ChannelStream::exit()
    {
        __alexit();
    }

    float ChannelStream::getPlaybackPosition()
    {
        float poso;
        int pos;
        alGetSourcei(source,AL_SAMPLE_OFFSET,&pos);
        
        poso = (float)pos/(float)this->samplerate;
        
        return poso;
    }
}
