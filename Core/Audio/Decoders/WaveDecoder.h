//
//  OggDecoder.h
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#pragma once

#include "../AudioDecoder.h"
#include "../AudioAssetReader.h"

typedef struct { 
    short  wFormatTag; 
    short  nChannels; 
    int nSamplesPerSec; 
    int nAvgBytesPerSec; 
    short  nBlockAlign; 
    short  wBitsPerSample; 
    short  cbSize; 
} WAVEFORMATEX; 

namespace Monocle
{
    class WaveDecoder : public AudioDecoder
    {
    public:
        
        WaveDecoder( AudioAsset *asset );
        ~WaveDecoder();
        
        virtual unsigned long Render( unsigned long size, void *outputBuffer );        
        
        AudioAsset *audAsset;
        
        WAVEFORMATEX format;
        AudioAssetReader *reader;
        
    private:
        
        void WaveOpen();
    };
}

