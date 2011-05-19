
#include "Audio.h"
#include "AudioDecoder.h"

#include <string>
#include <sstream>

namespace Monocle
{
    
    void AudioDecoder::Init( int samplerate, int bit, int ch )
    {
        this->samplerate = samplerate;
        this->bit = bit;
        this->ch = ch;
        
        this->loopsRemaining = 0;
        this->outOfData = false;
        this->seekOffset = -1;
        this->total = 0;
    }
 
    AudioDecoder::~AudioDecoder()
    {

    }
    
    AudioDecoder::AudioDecoder( )
    {

    }
}