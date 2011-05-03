//
//  OggDecoder.h
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#pragma once

#include "../AudioDecoder.h"
#include "../AudioAssetReader.h"

namespace Monocle
{
    class WaveDecoderData;
    class WaveDecoder : public AudioDecoder
    {
    public:
        
        WaveDecoder() : AudioDecoder("wav wave pcm") { };
        
        virtual AudioDecodeData *RequestData( AudioAsset *asset );
        virtual unsigned long Render( unsigned long size, void *outputBuffer, AudioDecodeData &decodeData );        
        virtual void FreeDecoderData( AudioDecodeData &dd );
        
    private:
        
        void WaveOpen(WaveDecoderData *data);
    };
}

