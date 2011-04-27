
#include "AudioDecoder.h"

namespace Monocle
{
    
    AudioDecodeData::AudioDecodeData( int samplerate, int bit, int ch, AudioDecoder *decoder, void *decoderData, AudioAsset *audAsset )
    {
        this->samplerate = samplerate;
        this->bit = bit;
        this->ch = ch;
        this->decoder = decoder;
        this->decoderData = decoderData;
        this->audAsset = audAsset;
        
        this->loopsRemaining = 1;
        this->outOfData = false;
        this->seekOffset = -1;
        this->total = 0;
    }
 
    AudioDecodeData::~AudioDecodeData()
    {
        this->decoder->FreeDecoderData(*this);
    }
}