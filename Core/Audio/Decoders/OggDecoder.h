#ifdef MONOCLE_AUDIO_OGG
//
//  OggDecoder.h
//  MonocleTest
//
//  Created by Josh Whelchel on 4/19/11.
//

#pragma once

#include "../AudioDecoder.h"

namespace Monocle
{
    class OggDecoder : public AudioDecoder
    {
    public:
        
        OggDecoder( AudioAsset *asset );
        ~OggDecoder();
        
        virtual unsigned long Render( unsigned long size, void *outputBuffer );        
        
        void *oggData;

        AudioAsset *audAsset;
    };
}

#endif