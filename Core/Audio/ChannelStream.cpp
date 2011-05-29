#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Audio/ChannelStream.h"

#include "../Debug.h"
#include "../MonocleToolkit.h"

#ifdef MONOCLE_MAC
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <AL/al.h>
	#include <AL/alc.h>
#endif

#include "../Platform.h"

#define VERBOSE(x) Debug::Log(std::string("VERBOSE: ") + x)
#undef VERBOSE
#define VERBOSE(x)

std::string GetALErrorString(ALenum err)
{
    switch(err)
    {
        case AL_NO_ERROR:
            return std::string("AL_NO_ERROR");
            break;
            
        case AL_INVALID_NAME:
            return std::string("AL_INVALID_NAME");
            break;
            
        case AL_INVALID_ENUM:
            return std::string("AL_INVALID_ENUM");
            break;
            
        case AL_INVALID_VALUE:
            return std::string("AL_INVALID_VALUE");
            break;
            
        case AL_INVALID_OPERATION:
            return std::string("AL_INVALID_OPERATION");
            break;
            
        case AL_OUT_OF_MEMORY:
            return std::string("AL_OUT_OF_MEMORY");
            break;
            
        default:
            return "??";
    };
}

namespace Monocle
{
    bool ChannelStream::IsPlaying()
    {
        if (!started)
            return false;
        
        ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }

    void ChannelStream::Open( int channels, int bits, int samplerate )
    {
        if (started)
            return;
        
        VERBOSE("ChannelStream Open");
        
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
        
        Check("opengenbuf");
        
        alGenSources(1, &source);
        
        Check("opengensource");
        
        alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
        alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
        alSource3f(source, AL_DIRECTION, 0.0, 0.0, 0.0);
        
        Check("opensetsource");
        
        alSourcei(source, AL_ROLLOFF_FACTOR, 0);
        alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
        
        Check("opensetsource2");
        
        memset(obtainedBuffer,0,BUFFER_SIZE);
        
        for (int i=0; i<NUM_BUFFERS; i++)
            alBufferData(buffers[i], format, obtainedBuffer, BUFFER_SIZE, samplerate);
        //alBufferData(buffers[1], format, obtainedBuffer, BUFFER_SIZE, samplerate);

        //Debug::Log("AUDIO: Opening Audio Channel: " + StringOf(samplerate) + "hz w/ " + StringOf(channels) + " channels (ALformat:" + StringOf(format) +")");
        
        Check("openbufferdata");

        this->samplerate = samplerate;
        this->started = true;
        this->startBuffer = 0;
        this->startedPlaying = false;
        this->pausePos = 0;
        this->playOffset = 0;
        this->playStart = 0;
    }
    
    void ChannelStream::Close()
    {
        if (!started)
            return;
        
        VERBOSE("ChannelStream Close");
        
        Check("preclose");
        alSourceStop(source);
        Check("stop");
        Empty();
        alDeleteSources(1,&source);
        Check("deletesources");
        alDeleteBuffers(NUM_BUFFERS,buffers);
        Check("deletebuffers");
        started = false;
    }

    int ChannelStream::NeedsUpdate()
    {
        int processed;
        
        if (!started)
            return 0;
        
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
        Check("locknumberbuff");
    }

    unsigned char *ChannelStream::GetBuffer( unsigned int *size )
    {
        if (this->startBuffer < NUM_BUFFERS){
            return GetStaticBuffer(size);
        }
        
        alSourceUnqueueBuffers(source, 1, &active_buffer);
        Check("getbuffer");
        
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
        Check("lockbd");
        
        alSourceQueueBuffers(source, 1, &active_buffer);
        Check("lockqb");
    }

    void ChannelStream::Play()
    {
        alSourceQueueBuffers(source, NUM_BUFFERS, buffers);
        alSourcePlay(source);
        Check("play");
        
        this->startedPlaying = true;
        this->playStart = Platform::GetMilliseconds();
    }

    void ChannelStream::Stop()
    {
        alSourceStop(source);
    }

    void ChannelStream::Empty()
    {
        int queued = 0;
        
        VERBOSE("Empty");
        
        alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
        Check("emptystart");
        
        while(queued--)
        {
            ALuint buffer;
            
            alSourceUnqueueBuffers(source, 1, &buffer);

            Check("empty " + StringOf(queued+1));
        }
    }

    void ChannelStream::Check( std::string erat )
    {
        int error = alGetError();
        
        VERBOSE("Checking " + erat);
            
        if(error != AL_NO_ERROR)
            Debug::Log("OpenAL Error on " + erat + ": " + StringOf(error) + " (" + GetALErrorString(error) + ")");
            //printf("OpenAL Error: %d\n",error);
    }

    ChannelStream::ChannelStream()
    {
        memset(this->buffers,0,sizeof(this->buffers));
        memset(this->obtainedBuffer,0,sizeof(this->obtainedBuffer));
        
        this->started = false;
        this->startBuffer=0;
        this->startedPlaying = false;
        this->format = AL_FORMAT_STEREO16;
        
        this->pausePos = 0;
        this->playOffset = 0;
        this->playStart = 0;
        
        this->source = 0;
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
        
        if ((device = alcOpenDevice(NULL)) == NULL) {
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
        if (!this->startedPlaying)
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
        if (started)
            alSourcef(source,AL_GAIN,vol);
    }
    
    void ChannelStream::SetPan( float pan )
    {
        if (started)
            alSource3f(source,AL_POSITION,pan,0.0,0.0);
    }

    void ChannelStream::SetPitch( float pitch )
    {
        if (started)
            alSourcef(source,AL_PITCH,pitch);
    }
    
    bool ChannelStream::IsOpen()
    {
        return started;
    }
    
}
