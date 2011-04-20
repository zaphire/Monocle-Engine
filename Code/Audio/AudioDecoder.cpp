
#include "AudioDecoder.h"

namespace Monocle
{
    
    AudioDecodeData::AudioDecodeData( int samplerate, int bit, int ch, AudioDecoder *decoder )
    {
        this->samplerate = samplerate;
        this->bit = bit;
        this->ch = ch;
        this->decoder = decoder;
        
        this->loopsRemaining = 1;
        this->done = 0;
        this->almostDone = false;
        this->seekOffset = -1;
        this->total = 0;
    }
    
}