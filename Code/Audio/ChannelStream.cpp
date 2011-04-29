#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Audio/ChannelStream.h"

#include "../Debug.h"
#include "../MonocleToolkit.h"

#ifdef MONOCLE_MAC
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#elif MONOCLE_LINUX
	#include <AL/al.h>
	#include <AL/alc.h>
#elif MONOCLE_WINDOWS
	// install OpenAL from http://connect.creativelabs.com/openal
	// add paths to VC dirs
	#include <al.h>
	#include <alc.h>
#endif

#include "../Platform.h"

namespace Monocle
{
    bool ChannelStream::IsPlaying()
    {
        ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    void ChannelStream::Open( int channels, int bits, int samplerate )
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
        
        Check();
        
        alGenBuffers(NUM_BUFFERS, buffers);
        
        Check();
        
        alGenSources(1, &source);
        
        Check();
        
        alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
        alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
        alSource3f(source, AL_DIRECTION, 0.0, 0.0, 0.0);
        
        Check();
        
        alSourcei(source, AL_ROLLOFF_FACTOR, 0);
        alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
        
        Check();
        
        memset(obtainedBuffer,0,BUFFER_SIZE);
        
        for (int i=0; i<NUM_BUFFERS; i++)
            alBufferData(buffers[i], format, obtainedBuffer, BUFFER_SIZE, samplerate);
        //alBufferData(buffers[1], format, obtainedBuffer, BUFFER_SIZE, samplerate);

        Debug::Log("AUDIO: Opening Audio Channel: " + StringOf(samplerate) + "hz w/ " + StringOf(channels) + " channels (ALformat:" + StringOf(format) +")");
        
        Check();

        this->samplerate = samplerate;
        this->started = true;
        this->startBuffer = 0;
        this->startedPlaying = false;
        this->pausePos = 0;
        this->playOffset = 0;
        this->playStart = -1;
    }
    
    void ChannelStream::Close()
    {
        alSourceStop(source);
        Empty();
        alDeleteSources(1,&source);
        alDeleteBuffers(NUM_BUFFERS,buffers);
        started = false;
    }

    int ChannelStream::NeedsUpdate()
    {
        int processed;
        
        if (this->startBuffer >= NUM_BUFFERS){
            alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
            return processed;
        }else{
            return NUM_BUFFERS-this->startBuffer;
        }
    }

    void ChannelStream::LockNumberedBuffer( unsigned int size, unsigned int buff )
    {
        alBufferData(buffers[buff], format, obtainedBuffer, size, samplerate);
        Check();
    }

    unsigned char *ChannelStream::GetBuffer( unsigned int *size )
    {
        if (this->startBuffer < NUM_BUFFERS){
            return GetStaticBuffer(size);
        }
        
        alSourceUnqueueBuffers(source, 1, &active_buffer);
        Check();
        
        return GetStaticBuffer(size);
    }

    unsigned char *ChannelStream::GetStaticBuffer( unsigned int *size )
    {
        size[0] = BUFFER_SIZE;	
        return obtainedBuffer;
    }

    void ChannelStream::LockBuffer( unsigned int size )
    {
        if (this->startBuffer < NUM_BUFFERS){
            LockNumberedBuffer(size, this->startBuffer);
            this->startBuffer++;
            return;
        }
        
        alBufferData(active_buffer, format, obtainedBuffer, size, samplerate);
        Check();
        
        alSourceQueueBuffers(source, 1, &active_buffer);
        Check();
    }

    void ChannelStream::Play()
    {
        alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
        alSourcePlay(source);
        Check();
        
        this->startedPlaying = true;
        this->playStart = Platform::GetMilliseconds();
    }

    void ChannelStream::Stop()
    {
        alSourceStop(source);
    }

    void ChannelStream::Empty()
    {
        int queued;
        
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        
        while(queued--)
        {
            ALuint buffer;
            
            alSourceUnqueueBuffers(source, 1, &buffer);
            Check();
        }
    }

    void ChannelStream::Check()
    {
        int error = alGetError();
            
        if(error != AL_NO_ERROR)
            printf("OpenAL Error: %d\n",error);
    }

    ChannelStream::ChannelStream()
    {
        started = false;
        startBuffer=0;
        this->startedPlaying = false;
        format = AL_FORMAT_STEREO16;
        
        this->pausePos = 0;
        this->playOffset = 0;
        this->playStart = -1;
    }

    ChannelStream::~ChannelStream()
    {
        if (started)
            Close();
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

    int ChannelStream::Init()
    {
        ALCcontext *context;
        ALCdevice *device;
        const ALCchar *default_device;
        
        default_device = alcGetString(NULL,
                                      ALC_DEFAULT_DEVICE_SPECIFIER);
        
        //printf("using default device: %s\n", default_device);
        Debug::Log("AUDIO: Opening Audio Device: " + std::string(default_device));
        
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

    void ChannelStream::Exit()
    {
        __alexit();
    }
    
    void ChannelStream::Pause()
    {
        //m_lpSBuff2->Stop();
        alSourceStop(source);
        this->pausePos = Platform::GetMilliseconds();
    }
    
    void ChannelStream::Resume()
    {
        if (IsPlaying()) return;
        alSourcePlay(source);
        
        this->playOffset += this->pausePos-this->playStart;
        this->playStart = Platform::GetMilliseconds();
    }
    
    inline unsigned long XMAX( unsigned long a, unsigned long b )
    {
        return ( a > b ) ? a : b;
    }
    
    unsigned long ChannelStream::GetOutputTime()
    {
        if (this->playStart == -1)
            return 0;
        
        return XMAX(Platform::GetMilliseconds() - this->playStart,0) + this->playOffset;
    }
    
    unsigned long ChannelStream::GetTotalPlayTime()
    {
        if (!IsPlaying()) {
            if (this->pausePos)
                return (this->pausePos-this->playStart) + this->playOffset;
            else
                return 0;
        }
        
        return GetOutputTime();
    }
    
    void ChannelStream::SetPlayOffset( unsigned long playOffset )
    {
        this->playOffset = playOffset;
    }
    
    void ChannelStream::SetVolume( float vol )
    {
        alSourcef(source,AL_GAIN,vol);
    }
    
    void ChannelStream::SetPan( float pan )
    {
        alSource3f(source,AL_POSITION,pan,0.0,0.0);
    }

    void ChannelStream::SetPitch( float pitch )
    {
        alSourcef(source,AL_PITCH,pitch);
    }
    
}
