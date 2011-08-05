//
//  OGGAudioDecoder.h
//  MonocleIOS
//
//  Created by Josh Whelchel on 8/4/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#if MONOCLE_IOS && !MONOCLE_AUDIO_OGG

#include "stb_vorbis.h"
#include "../Audio/Audio.h"
#include "../Audio/AudioAssetReader.h"
#include "../Audio/AudioDecoder.h"

namespace Monocle
{
    class IOSOggDecoder : public AudioDecoder
    {
    public:
        
        IOSOggDecoder( AudioAsset *asset );
        ~IOSOggDecoder();
        
        virtual unsigned long Render( unsigned long size, void *outputBuffer );        
        
        void *oggData;
        
        AudioAsset *audAsset;
        AudioAssetReader *reader;
        
        stb_vorbis *vorb;
    };
}

#endif // #if MONOCLE_IOS && !MONOCLE_AUDIO_OGG