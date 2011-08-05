//
//  OGGAudioDecoder.m
//  MonocleIOS
//
//  Created by Josh Whelchel on 8/4/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#if MONOCLE_IOS && !MONOCLE_AUDIO_OGG

#import "OGGAudioDecoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "../Debug.h"
#include "../Audio/AudioAsset.h"

#include "../Audio/AudioAssetReader.h"

namespace Monocle {
    
    /***
     THIS CODE is responsible for registering this decoder with the file extension.
     See more info in Audio::Init.
     ***/
    AudioDecoder *makeSTBOggFunc( AudioAsset *asset )
    {
        return new IOSOggDecoder(asset);
    }
    
    unsigned long IOSOggDecoder::Render( unsigned long size, void *buf )
    {
        if (!reader || !vorb){
            outOfData = true;
            return 0;
        }
        
        int ret = 1;
        unsigned long pos=0;
        
        if (seekOffset != -1)
        {
//            long seek = (long)(((float)seekOffset/1000.0f) * (float)samplerate);
            // DOES NOT SEEK ?!
            if (seekOffset==0){
                  stb_vorbis_seek_start(vorb);          
            }
            seekOffset = -1;
            outOfData = false;
        }
        
        while(ret && pos<size)
        {
//            ret = ov_read(&data->vf, (char*)buf+pos, size-pos, 0, 2, 1, &data->sec);
            ret = stb_vorbis_get_samples_short_interleaved(vorb, ch, (short*)((char*)buf+pos), (size-pos)/2) * 2 * ch;
            pos += ret;
        }
        
        // reached the end?
        if (!ret && (loopsRemaining!=0))
        {
            // we are looping so restart from the beginning
            ret = 1;
//            ov_pcm_seek(&data->vf, 0);  // Loop back position, TODO: If we want to loop to a specific point, here is the code for that!
            stb_vorbis_seek_start(vorb);
            while(ret && pos<size)
            {
//                ret = ov_read(&data->vf, (char*)buf+pos, size-pos, 0, 2, 1, &data->sec);
                ret = stb_vorbis_get_samples_short_interleaved(vorb, ch, (short*)((char*)buf+pos), (size-pos)/2) * 2 * ch;
                pos += ret;
            }
            
            // Decrease from Loops Remaining (don't touch the -1 infinite loop stuff)
            if (loopsRemaining > 0)
                loopsRemaining--;
        }
        else if (!ret && (loopsRemaining==0)){
            outOfData = true;
        }
        
        return pos;
    }
    
    IOSOggDecoder::IOSOggDecoder( AudioAsset *asset )
    {
        int error;
        
        Init(44100,16,2);
        this->audAsset = asset;
        this->reader = NULL;
        this->vorb = NULL;
        
        reader = new AudioAssetReader(audAsset,audAsset->GetDecodeString());
        
        if (!reader)
            return;
        
        vorb = stb_vorbis_open_reader(reader, &error, NULL);
        
        if (!vorb){
            delete reader;
            reader = NULL;
            
            return;
        }
        
//        vorbis_info *vi=ov_info(&data->vf,-1);
        stb_vorbis_info info = stb_vorbis_get_info(vorb);
        
        samplerate = (int)info.sample_rate;
        ch = info.channels;
        
        //total = (long)(((float)ov_pcm_total(&data->vf,-1)/(float)samplerate) * 1000.0f); 
        total = (long)(stb_vorbis_stream_length_in_seconds(vorb)*1000.0);
    }
    
    IOSOggDecoder::~IOSOggDecoder()
    {
        if (vorb)
            stb_vorbis_close(vorb);
        if (reader)
            delete reader;
    }
    
}

#endif // #if MONOCLE_IOS && !MONOCLE_AUDIO_OGG
