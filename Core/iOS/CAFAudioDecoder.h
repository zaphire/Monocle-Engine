//
//  CAFAudioDecoder.h
//  MonocleIOS
//
//  Created by Josh Whelchel on 8/4/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_IOS

#include "../Audio/Audio.h"
#include "../Audio/AudioAssetReader.h"

#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>

namespace Monocle
{
    class CAFDecoder : public AudioDecoder
    {
    public:
        
        CAFDecoder( AudioAsset *asset );
        ~CAFDecoder();
        
        virtual unsigned long Render( unsigned long size, void *outputBuffer );
        
        AudioAssetReader *reader;
        AudioAsset *audAsset;
        
        ExtAudioFileRef					extRef;
        SInt64							theFileLengthInFrames;
        AudioStreamBasicDescription		theFileFormat;
        AudioStreamBasicDescription		theOutputFormat;
    };
}

#endif