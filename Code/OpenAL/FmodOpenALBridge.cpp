/*
Copyright (C) 2007, 2010 - Bit-Blot

This file is part of Aquaria.

Aquaria is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// This file implements just enough of the FMOD library with OpenAL to suit
//  the needs of the existing game code without having to actually ship FMOD.

#ifdef MONOCLE_FMOD_OPENAL_BRIDGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef BBGE_BUILD_UNIX
#include <signal.h>
#endif

#include "FmodOpenALBridge.h"

#include "AL/include/al.h"
#include "AL/include/alc.h"

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

#ifndef _DEBUG
//#define _DEBUG 1
#endif

/* for porting purposes... */
#ifndef STUBBED
#ifndef _DEBUG
#define STUBBED(x)
#else
#define STUBBED(x) { \
    static bool first_time = true; \
    if (first_time) { \
        first_time = false; \
        fprintf(stderr, "STUBBED: %s (%s, %s:%d)\n", x, __FUNCTION__, __FILE__, __LINE__); \
    } \
}
#endif
#endif

namespace FMOD {

#if _DEBUG
    #ifdef _MSC_VER
        #define bbgeDebugBreak _CrtDbgBreak
    #elif defined(__GNUC__) && ((__i386__) || (__x86_64__))
        #define bbgeDebugBreak() __asm__ __volatile__ ( "int $3\n\t" )
    #else
        #define bbgeDebugBreak() raise(SIGTRAP)
    #endif
    #define SANITY_CHECK_OPENAL_CALL() { \
        const ALenum err = alGetError(); \
        if (err != AL_NONE) { \
            fprintf(stderr, "WARNING: OpenAL error %s:%d: 0x%X\n", \
                        __FILE__, __LINE__, (int) err); \
            bbgeDebugBreak(); \
        } \
    }
#else
    #define SANITY_CHECK_OPENAL_CALL()
#endif


// simply nasty.
#define ALBRIDGE(cls,method,params,args) \
    FMOD_RESULT cls::method params { \
        if (!this) return FMOD_ERR_INTERNAL; \
        return ((OpenAL##cls *) this)->method args; \
    }

static ALenum GVorbisFormat = AL_NONE;

class OpenALChannelGroup;
class OpenALSound;

class OpenALChannel
{
public:
    OpenALChannel();
    FMOD_RESULT setVolume(const float _volume, const bool setstate=true);
    FMOD_RESULT setPaused(const bool _paused, const bool setstate=true);
    FMOD_RESULT setFrequency(const float _frequency);
    FMOD_RESULT setPriority(int _priority);
    FMOD_RESULT getPosition(unsigned int *position, FMOD_TIMEUNIT postype);
    FMOD_RESULT getVolume(float *_volume);
    FMOD_RESULT isPlaying(bool *isplaying);
    FMOD_RESULT setChannelGroup(ChannelGroup *channelgroup);
    FMOD_RESULT stop();
    void setGroupVolume(const float _volume);
    void setSourceName(const ALuint _sid) { sid = _sid; }
    ALuint getSourceName() const { return sid; }
    void update();
    void reacquire();
    bool isInUse() const { return inuse; }
    void setSound(OpenALSound *sound);

private:
    ALuint sid;  // source id.
    float groupvolume;
    float volume;
    bool paused;
    int priority;
    float frequency;
    OpenALChannelGroup *group;
    OpenALSound *sound;
    bool inuse;
    bool initial;
};


class OpenALChannelGroup
{
public:
    OpenALChannelGroup(const char *_name);
    ~OpenALChannelGroup();
    FMOD_RESULT stop();
    FMOD_RESULT addDSP(DSP *dsp, DSPConnection **connection);
    FMOD_RESULT getPaused(bool *_paused);
    FMOD_RESULT setPaused(const bool _paused);
    FMOD_RESULT getVolume(float *_volume);
    FMOD_RESULT setVolume(const float _volume);
    bool attachChannel(OpenALChannel *channel);
    void detachChannel(OpenALChannel *channel);

private:
    const char *name;
    bool paused;
    int channel_count;
    OpenALChannel **channels;
    float volume;
};


// FMOD::Channel implementation...

OpenALChannel::OpenALChannel()
    : sid(0)
    , groupvolume(1.0f)
    , volume(1.0f)
    , paused(false)
    , priority(0)
    , frequency(1.0f)
    , group(NULL)
    , sound(NULL)
    , inuse(false)
    , initial(true)
{
}

void OpenALChannel::reacquire()
{
    assert(!inuse);
    inuse = true;
    volume = 1.0f;
    paused = true;
    priority = 0;
    frequency = 1.0f;
    sound = NULL;
    initial = true;
}

void OpenALChannel::setGroupVolume(const float _volume)
{
    groupvolume = _volume;
    alSourcef(sid, AL_GAIN, volume * groupvolume);
    SANITY_CHECK_OPENAL_CALL();
}

void OpenALChannel::update()
{
    if (inuse)
    {
        ALint state = 0;
        alGetSourceiv(sid, AL_SOURCE_STATE, &state);
        SANITY_CHECK_OPENAL_CALL();
        if (state == AL_STOPPED)
            stop();
    }
}

ALBRIDGE(Channel,setVolume,(float volume),(volume))
FMOD_RESULT OpenALChannel::setVolume(const float _volume, const bool setstate)
{
    if (setstate)
        volume = _volume;
    alSourcef(sid, AL_GAIN, _volume * groupvolume);
    SANITY_CHECK_OPENAL_CALL();
    return FMOD_OK;
}

ALBRIDGE(Channel,getPosition,(unsigned int *position, FMOD_TIMEUNIT postype),(position,postype))
FMOD_RESULT OpenALChannel::getPosition(unsigned int *position, FMOD_TIMEUNIT postype)
{
    assert(postype == FMOD_TIMEUNIT_MS);
    ALfloat secs = 0.0f;
    alGetSourcefv(sid, AL_SEC_OFFSET, &secs);
    SANITY_CHECK_OPENAL_CALL();
    *position = (unsigned int) (secs * 1000.0f);
    return FMOD_OK;
}

ALBRIDGE(Channel,getVolume,(float *volume),(volume))
FMOD_RESULT OpenALChannel::getVolume(float *_volume)
{
    *_volume = volume;
    return FMOD_OK;
}

ALBRIDGE(Channel,isPlaying,(bool *isplaying),(isplaying))
FMOD_RESULT OpenALChannel::isPlaying(bool *isplaying)
{
    // Apple's Mac OS X has a bug; alSourceRewind() doesn't make the sources
    //  AL_INITIAL again, so we have to track this ourselves.  :/
    if (initial)
    {
        *isplaying = true;
        return FMOD_OK;
    }

    ALint state = 0;
    alGetSourceiv(sid, AL_SOURCE_STATE, &state);
    SANITY_CHECK_OPENAL_CALL();
    *isplaying = ((state == AL_PLAYING) || (state == AL_INITIAL));
    if (state == AL_PAUSED)
        STUBBED("Should paused channels count as playing?");  // !!! FIXME
    return FMOD_OK;
}

ALBRIDGE(Channel,setChannelGroup,(ChannelGroup *channelgroup),(channelgroup))
FMOD_RESULT OpenALChannel::setChannelGroup(ChannelGroup *_channelgroup)
{
    OpenALChannelGroup *channelgroup = ((OpenALChannelGroup *) _channelgroup);
    assert(channelgroup);
    if (!channelgroup->attachChannel(this))
        return FMOD_ERR_INTERNAL;
    if ((group != NULL) && (group != channelgroup))
        group->detachChannel(this);
    group = channelgroup;
    return FMOD_OK;
}

ALBRIDGE(Channel,setFrequency,(float frequency),(frequency))
FMOD_RESULT OpenALChannel::setFrequency(const float _frequency)
{
    frequency = _frequency;
STUBBED("read the docs, verify this");
    alSourcef(sid, AL_PITCH, _frequency);
    SANITY_CHECK_OPENAL_CALL();
    return FMOD_OK;
}

ALBRIDGE(Channel,setPaused,(bool paused),(paused))
FMOD_RESULT OpenALChannel::setPaused(const bool _paused, const bool setstate)
{
    ALint state = 0;
    alGetSourceiv(sid, AL_SOURCE_STATE, &state);
    SANITY_CHECK_OPENAL_CALL();
    if ((_paused) && (state == AL_PLAYING))
    {
        alSourcePause(sid);
        initial = false;
        SANITY_CHECK_OPENAL_CALL();
    }
    else if ((!_paused) && (initial || ((state == AL_INITIAL) || (state == AL_PAUSED))))
    {
        alSourcePlay(sid);
        initial = false;
        SANITY_CHECK_OPENAL_CALL();
    }

    if (setstate)
        paused = _paused;

    return FMOD_OK;
}

ALBRIDGE(Channel,setPriority,(int priority),(priority))
FMOD_RESULT OpenALChannel::setPriority(int _priority)
{
    priority = _priority;
    return FMOD_OK;
}


// FMOD::ChannelGroup implementation...

OpenALChannelGroup::OpenALChannelGroup(const char *_name)
    : name(NULL)
    , paused(false)
    , channel_count(0)
    , channels(NULL)
    , volume(1.0f)
{
    if (_name)
    {
        char *buf = new char[strlen(_name) + 1];
        strcpy(buf, _name);
        name = buf;
    }
}

OpenALChannelGroup::~OpenALChannelGroup()
{
    delete[] name;
}

bool OpenALChannelGroup::attachChannel(OpenALChannel *channel)
{
    channel->setGroupVolume(volume);

    for (int i = 0; i < channel_count; i++)
    {
        if (channels[i] == channel)
            return true;
    }

    void *ptr = realloc(channels, sizeof (OpenALChannel *) * (channel_count + 1));
    if (ptr == NULL)
        return false;

    channels = (OpenALChannel **) ptr;
    channels[channel_count++] = channel;
    return true;
}

void OpenALChannelGroup::detachChannel(OpenALChannel *channel)
{
    for (int i = 0; i < channel_count; i++)
    {
        if (channels[i] == channel)
        {
            if (i < (channel_count-1))
                memmove(&channels[i], &channels[i+1], sizeof (OpenALChannel *) * ((channel_count - i) - 1));
            channel_count--;
            return;
        }
    }

    assert(false && "Detached a channel that isn't part of the group!");
}


ALBRIDGE(ChannelGroup,addDSP,(DSP *dsp, DSPConnection **connection),(dsp,connection))
FMOD_RESULT OpenALChannelGroup::addDSP(DSP *dsp, DSPConnection **connection)
{
    STUBBED("write me");
    return FMOD_ERR_INTERNAL;
}

ALBRIDGE(ChannelGroup,getPaused,(bool *paused),(paused))
FMOD_RESULT OpenALChannelGroup::getPaused(bool *_paused)
{
    *_paused = paused;
    return FMOD_OK;
}

ALBRIDGE(ChannelGroup,getVolume,(float *volume),(volume))
FMOD_RESULT OpenALChannelGroup::getVolume(float *_volume)
{
    *_volume = volume;
    return FMOD_OK;
}

ALBRIDGE(ChannelGroup,setPaused,(bool paused),(paused))
FMOD_RESULT OpenALChannelGroup::setPaused(const bool _paused)
{
    for (int i = 0; i < channel_count; i++)
        channels[i]->setPaused(_paused, false);
    paused = _paused;
    return FMOD_OK;
}

ALBRIDGE(ChannelGroup,setVolume,(float volume),(volume))
FMOD_RESULT OpenALChannelGroup::setVolume(const float _volume)
{
    volume = _volume;
    for (int i = 0; i < channel_count; i++)
        channels[i]->setGroupVolume(_volume);
    return FMOD_OK;
}

ALBRIDGE(ChannelGroup,stop,(),())
FMOD_RESULT OpenALChannelGroup::stop()
{
    for (int i = 0; i < channel_count; i++)
        channels[i]->stop();
    return FMOD_OK;
}


// FMOD::DSP implementation...

FMOD_RESULT DSP::getActive(bool *active)
{
    STUBBED("write me");
    *active = false;
    return FMOD_ERR_INTERNAL;
}

FMOD_RESULT DSP::remove()
{
    STUBBED("write me");
    return FMOD_ERR_INTERNAL;
}

FMOD_RESULT DSP::setParameter(int index, float value)
{
    STUBBED("write me");
    return FMOD_ERR_INTERNAL;
}


// FMOD::Sound implementation ...

class OpenALSound
{
public:
    OpenALSound(const ALuint _bid, const bool _looping);
    bool isLooping() const { return looping; }
    ALuint getBufferName() const { return bid; }
    FMOD_RESULT release();
    void reference() { refcount++; }

private:
    const ALuint bid;  // buffer id
    const bool looping;
    int refcount;
};

OpenALSound::OpenALSound(const ALuint _bid, const bool _looping)
    : bid(_bid)
    , looping(_looping)
    , refcount(1)
{
}

ALBRIDGE(Sound,release,(),())
FMOD_RESULT OpenALSound::release()
{
    refcount--;
    if (refcount <= 0)
    {
        alDeleteBuffers(1, &bid);
        SANITY_CHECK_OPENAL_CALL();
        delete this;
    }
    return FMOD_OK;
}


void OpenALChannel::setSound(OpenALSound *_sound)
{
    if (sound)
        sound->release();

    sound = _sound;

    if (sound)
        sound->reference();
}


ALBRIDGE(Channel,stop,(),())
FMOD_RESULT OpenALChannel::stop()
{
    alSourceStop(sid);
    SANITY_CHECK_OPENAL_CALL();
    alSourcei(sid, AL_BUFFER, 0);
    SANITY_CHECK_OPENAL_CALL();
    if (sound)
    {
        sound->release();
        sound = NULL;
    }
    paused = false;
    inuse = false;
    initial = false;
    return FMOD_OK;
}



// FMOD::System implementation ...

class OpenALSystem
{
public:
    OpenALSystem();
    ~OpenALSystem();
    FMOD_RESULT init(const int maxchannels, const FMOD_INITFLAGS flags, const void *extradriverdata);
    FMOD_RESULT update();
    FMOD_RESULT release();
    FMOD_RESULT getVersion(unsigned int *version);
    FMOD_RESULT setSpeakerMode(const FMOD_SPEAKERMODE speakermode);
    FMOD_RESULT setFileSystem(FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, const int blockalign);
    FMOD_RESULT setDSPBufferSize(const unsigned int bufferlength, const int numbuffers);
    FMOD_RESULT createChannelGroup(const char *name, ChannelGroup **channelgroup);
    FMOD_RESULT createDSPByType(const FMOD_DSP_TYPE type, DSP **dsp);
    FMOD_RESULT createSound(const char *name_or_data, const FMOD_MODE mode, const FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
    FMOD_RESULT createStream(const char *name_or_data, const FMOD_MODE mode, const FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
    FMOD_RESULT getDriverCaps(const int id, FMOD_CAPS *caps, int *minfrequency, int *maxfrequency, FMOD_SPEAKERMODE *controlpanelspeakermode);
    FMOD_RESULT getMasterChannelGroup(ChannelGroup **channelgroup);
    FMOD_RESULT playSound(FMOD_CHANNELINDEX channelid, Sound *sound, bool paused, Channel **channel);

private:
    OpenALChannelGroup *master_channel_group;
    int num_channels;
    OpenALChannel *channels;
};


OpenALSystem::OpenALSystem()
    : master_channel_group(NULL)
    , num_channels(0)
    , channels(NULL)
{
}

OpenALSystem::~OpenALSystem()
{
    delete master_channel_group;
    delete[] channels;
}


FMOD_RESULT System_Create(FMOD_SYSTEM **system)
{
    *system = (FMOD_SYSTEM *) new OpenALSystem;
    return FMOD_OK;
}

ALBRIDGE(System,createChannelGroup,(const char *name, ChannelGroup **channelgroup),(name,channelgroup))
FMOD_RESULT OpenALSystem::createChannelGroup(const char *name, ChannelGroup **channelgroup)
{
    *channelgroup = (ChannelGroup *) new OpenALChannelGroup(name);
    return FMOD_OK;
}

ALBRIDGE(System,createDSPByType,(FMOD_DSP_TYPE type, DSP **dsp),(type,dsp))
FMOD_RESULT OpenALSystem::createDSPByType(const FMOD_DSP_TYPE type, DSP **dsp)
{
    *dsp = NULL;
    STUBBED("write me");
    return FMOD_ERR_INTERNAL;
}

static void *decode_to_pcm(const char *_fname, ALenum &format, ALsizei &size, ALuint &freq, const bool streaming)
{
#ifdef __POWERPC__
    const int bigendian = 1;
#else
    const int bigendian = 0;
#endif

    // !!! FIXME: if it's not Ogg, we don't have a decoder. I'm lazy.  :/
    char *fname = (char *) alloca(strlen(_fname) + 16);
    strcpy(fname, _fname);
    char *ptr = strrchr(fname, '.');
    if (ptr) *ptr = '\0';
    strcat(fname, ".ogg");

    // just in case...
    #undef fopen
    FILE *io = fopen(fname, "rb"); //core->adjustFilenameCase(fname).c_str()
    if (io == NULL)
        return NULL;

    ALubyte *retval = NULL;

    if ((streaming) && (GVorbisFormat != AL_NONE))
    {
        // Can we just feed it to the AL compressed?
        format = GVorbisFormat;
        freq = 44100;
        fseek(io, 0, SEEK_END);
        size = ftell(io);
        fseek(io, 0, SEEK_SET);
        retval = (ALubyte *) malloc(size);
        size_t rc = fread(retval, size, 1, io);
        fclose(io);
        if (rc != 1)
        {
            free(retval);
            return NULL;
        }
        return retval;
    }

    // Uncompress and feed to the AL.
    OggVorbis_File vf;
    memset(&vf, '\0', sizeof (vf));
    if (ov_open(io, &vf, NULL, 0) == 0)
    {
        int bitstream = 0;
        vorbis_info *info = ov_info(&vf, -1);
        size = 0;
        format = (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        freq = info->rate;

        if ((info->channels != 1) && (info->channels != 2))
        {
            ov_clear(&vf);
            return NULL;
        }

        char buf[1024 * 16];
        long rc = 0;
        size_t allocated = 64 * 1024;
        retval = (ALubyte *) malloc(allocated);
        while ( (rc = ov_read(&vf, buf, sizeof (buf), bigendian, 2, 1, &bitstream)) != 0 )
        {
            if (rc > 0)
            {
                size += rc;
                if (size >= allocated)
                {
                    allocated *= 2;
                    ALubyte *tmp = (ALubyte *) realloc(retval, allocated);
                    if (tmp == NULL)
                    {
                        free(retval);
                        retval = NULL;
                        break;
                    }
                    retval = tmp;
                }
                memcpy(retval + (size - rc), buf, rc);
            }
        }
        ov_clear(&vf);
        return retval;
    }

    fclose(io);
    return NULL;
}


ALBRIDGE(System,createSound,(const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound),(name_or_data,mode,exinfo,sound))
FMOD_RESULT OpenALSystem::createSound(const char *name_or_data, const FMOD_MODE mode, const FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound)
{
    assert(!exinfo);

    FMOD_RESULT retval = FMOD_ERR_INTERNAL;
    ALenum pcmfmt = AL_NONE;
    ALsizei pcmsize = 0;
    ALuint pcmfreq = 0;
    void *pcm = decode_to_pcm(name_or_data, pcmfmt, pcmsize, pcmfreq, (mode & FMOD_CREATESTREAM) != 0);
    if (pcm == NULL)
        return FMOD_ERR_INTERNAL;

    #if 0
    static int dump_id = 0;
    char buf[128];
    snprintf(buf, sizeof (buf), "dump_%d.pcm", dump_id++);
    FILE *io = fopen(buf, "wb");
    if (io != NULL)
    {
        fwrite(pcm, pcmsize, 1, io);
        fclose(io);
    }
    #endif

    ALuint bid = 0;
    alGetError();
    alGenBuffers(1, &bid);
    SANITY_CHECK_OPENAL_CALL();
    if (alGetError() == AL_NO_ERROR)
    {
        alBufferData(bid, pcmfmt, pcm, pcmsize, pcmfreq);
        SANITY_CHECK_OPENAL_CALL();
        *sound = (Sound *) new OpenALSound(bid, (((mode & FMOD_LOOP_OFF) == 0) && (mode & FMOD_LOOP_NORMAL)));
        retval = FMOD_OK;
    }

    free(pcm);

    return retval;
}

ALBRIDGE(System,createStream,(const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound),(name_or_data,mode,exinfo,sound))
FMOD_RESULT OpenALSystem::createStream(const char *name_or_data, const FMOD_MODE mode, const FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound)
{
    return createSound(name_or_data, mode | FMOD_CREATESTREAM, exinfo, sound);
}

ALBRIDGE(System,getDriverCaps,(int id, FMOD_CAPS *caps, int *minfrequency, int *maxfrequency, FMOD_SPEAKERMODE *controlpanelspeakermode),(id,caps,minfrequency,maxfrequency,controlpanelspeakermode))
FMOD_RESULT OpenALSystem::getDriverCaps(const int id, FMOD_CAPS *caps, int *minfrequency, int *maxfrequency, FMOD_SPEAKERMODE *controlpanelspeakermode)
{
    assert(!id);
    assert(!minfrequency);
    assert(!maxfrequency);
    *controlpanelspeakermode = FMOD_SPEAKERMODE_STEREO;  // not strictly true, but works for aquaria's usage.
    *caps = 0;   // aquaria only checks FMOD_CAPS_HARDWARE_EMULATED.
    return FMOD_OK;
}

ALBRIDGE(System,getMasterChannelGroup,(ChannelGroup **channelgroup),(channelgroup))
FMOD_RESULT OpenALSystem::getMasterChannelGroup(ChannelGroup **channelgroup)
{
    *channelgroup = (ChannelGroup *) master_channel_group;
    return FMOD_OK;
}

ALBRIDGE(System,getVersion,(unsigned int *version),(version))
FMOD_RESULT OpenALSystem::getVersion(unsigned int *version)
{
    *version = FMOD_VERSION;
    return FMOD_OK;
}

ALBRIDGE(System,init,(int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata),(maxchannels,flags,extradriverdata))
FMOD_RESULT OpenALSystem::init(const int maxchannels, const FMOD_INITFLAGS flags, const void *extradriverdata)
{
    ALCdevice *dev = alcOpenDevice(NULL);
    if (!dev)
        return FMOD_ERR_INTERNAL;

    ALCcontext *ctx = alcCreateContext(dev, NULL);
    if (!ctx)
    {
        alcCloseDevice(dev);
        return FMOD_ERR_INTERNAL;
    }

    alcMakeContextCurrent(ctx);
    alcProcessContext(ctx);

    #ifdef _DEBUG
    printf("AL_VENDOR: %s\n", (char *) alGetString(AL_VENDOR));
    printf("AL_RENDERER: %s\n", (char *) alGetString(AL_RENDERER));
    printf("AL_VERSION: %s\n", (char *) alGetString(AL_VERSION));
    printf("AL_EXTENSIONS: %s\n", (char *) alGetString(AL_EXTENSIONS));
    #endif

    SANITY_CHECK_OPENAL_CALL();

    GVorbisFormat = AL_NONE;
    if (alIsExtensionPresent("AL_EXT_vorbis"))
        GVorbisFormat = alGetEnumValue("AL_FORMAT_VORBIS_EXT");

#if 0  // Disabled output: every bug report thinks this is the culprit. --ryan.
    if (GVorbisFormat == AL_NONE)
        fprintf(stderr, "WARNING: no AL_EXT_vorbis support. We'll use more RAM.\n");
#endif

    SANITY_CHECK_OPENAL_CALL();

    master_channel_group = new OpenALChannelGroup("master");

    num_channels = maxchannels;
    channels = new OpenALChannel[maxchannels];
    alGetError();  // clear any existing error state.
    for (int i = 0; i < num_channels; i++)
    {
        ALuint sid = 0;
        alGenSources(1, &sid);
        SANITY_CHECK_OPENAL_CALL();
        assert(alGetError() == AL_NO_ERROR);  // !!! FIXME: handle this. hardware implementations will probably trigger this.
        alSourcei(sid, AL_SOURCE_RELATIVE, AL_TRUE);
        SANITY_CHECK_OPENAL_CALL();
        alSource3f(sid, AL_POSITION, 0.0f, 0.0f, 0.0f);  // no panning or spatialization in Aquaria.
        SANITY_CHECK_OPENAL_CALL();
        channels[i].setSourceName(sid);
        channels[i].setChannelGroup((ChannelGroup *) master_channel_group);
    }
    return FMOD_OK;
}

ALBRIDGE(System,playSound,(FMOD_CHANNELINDEX channelid, Sound *sound, bool paused, Channel **channel),(channelid,sound,paused,channel))
FMOD_RESULT OpenALSystem::playSound(FMOD_CHANNELINDEX channelid, Sound *_sound, bool paused, Channel **channel)
{
    *channel = NULL;

    if (channelid == FMOD_CHANNEL_FREE)
    {
        for (int i = 0; i < num_channels; i++)
        {
            if (!channels[i].isInUse())
            {
                channelid = (FMOD_CHANNELINDEX) i;
                break;
            }
        }
    }

    if ((channelid < 0) || (channelid >= num_channels))
        return FMOD_ERR_INTERNAL;

    OpenALSound *sound = (OpenALSound *) _sound;
    const ALuint sid = channels[channelid].getSourceName();
    // alSourceRewind doesn't work right on some versions of Mac OS X.
    alSourceStop(sid);  // stop any playback, set to AL_INITIAL.
    alSourceRewind(sid);  // stop any playback, set to AL_INITIAL.
    SANITY_CHECK_OPENAL_CALL();
    alSourcei(sid, AL_BUFFER, sound->getBufferName());
    SANITY_CHECK_OPENAL_CALL();
    alSourcei(sid, AL_LOOPING, sound->isLooping() ? AL_TRUE : AL_FALSE);
    SANITY_CHECK_OPENAL_CALL();

    channels[channelid].reacquire();
    channels[channelid].setPaused(paused);
    channels[channelid].setSound(sound);
    *channel = (Channel *) &channels[channelid];
    return FMOD_OK;
}


ALBRIDGE(System,release,(),())
FMOD_RESULT OpenALSystem::release()
{
    ALCcontext *ctx = alcGetCurrentContext();
    if (ctx)
    {
        for (int i = 0; i < num_channels; i++)
        {
            const ALuint sid = channels[i].getSourceName();
            channels[i].setSourceName(0);
            channels[i].setSound(NULL);
            alSourceStop(sid);
            alSourcei(sid, AL_BUFFER, 0);
            alDeleteSources(1, &sid);
        }
        ALCdevice *dev = alcGetContextsDevice(ctx);
        alcMakeContextCurrent(NULL);
        alcSuspendContext(ctx);
        alcDestroyContext(ctx);
        alcCloseDevice(dev);
    }
    delete this;
    return FMOD_OK;
}

ALBRIDGE(System,setDSPBufferSize,(unsigned int bufferlength, int numbuffers),(bufferlength,numbuffers))
FMOD_RESULT OpenALSystem::setDSPBufferSize(const unsigned int bufferlength, const int numbuffers)
{
    // aquaria only uses this for FMOD_CAPS_HARDWARE_EMULATED, so I skipped it.
    return FMOD_ERR_INTERNAL;
}

ALBRIDGE(System,setFileSystem,(FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, int blockalign),(useropen,userclose,userread,userseek,blockalign))
FMOD_RESULT OpenALSystem::setFileSystem(FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, const int blockalign)
{
    // Aquaria sets these, but they don't do anything fancy, so we ignore them for now.
    return FMOD_OK;
}

ALBRIDGE(System,setSpeakerMode,(FMOD_SPEAKERMODE speakermode),(speakermode))
FMOD_RESULT OpenALSystem::setSpeakerMode(const FMOD_SPEAKERMODE speakermode)
{
    return FMOD_OK;  // we ignore this for Aquaria.
}

ALBRIDGE(System,update,(),())
FMOD_RESULT OpenALSystem::update()
{
    alcProcessContext(alcGetCurrentContext());
    for (int i = 0; i < num_channels; i++)
        channels[i].update();
#if _DEBUG
    const ALenum err = alGetError();
    if (err != AL_NONE)
        fprintf(stderr, "WARNING: OpenAL error this frame: 0x%X\n", (int) err);
#endif
    return FMOD_OK;
}


// misc FMOD bits...

FMOD_RESULT Memory_GetStats(int *currentalloced, int *maxalloced, FMOD_BOOL blocking)
{
    // not ever used by Aquaria.
    *currentalloced = *maxalloced = 42;
    return FMOD_ERR_INTERNAL;
}

}  // namespace FMOD

#endif
// end of FmodOpenALBridge.cpp ...